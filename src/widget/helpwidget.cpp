// Note: the HelpWidget class is result of some refactored code taken from Qt
//       Assistant

#include "helpwidget.h"
#include "mainwindow.h"

#include <QDir>
#include <QTimer>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QHelpEngine>
#include <QNetworkReply>

HelpNetworkReply::HelpNetworkReply(const QNetworkRequest& request,
                                   const QByteArray& fileData,
                                   const QString& mimeType) :
    data(fileData),
    origLen(fileData.length())
{
    setRequest(request);
    setOpenMode(QIODevice::ReadOnly);
    setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
    setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(origLen));

    QTimer::singleShot(0, this, SIGNAL(metaDataChanged()));
    QTimer::singleShot(0, this, SIGNAL(readyRead()));
}

qint64 HelpNetworkReply::readData(char *buffer, qint64 maxlen)
{
    qint64 len = qMin(qint64(data.length()), maxlen);
    if (len) {
        qMemCopy(buffer, data.constData(), len);
        data.remove(0, len);
    }
    if (!data.length())
        QTimer::singleShot(0, this, SIGNAL(finished()));
    return len;
}

HelpNetworkAccessManager::HelpNetworkAccessManager(QHelpEngine *engine,
                                                   QObject *parent) :
    QNetworkAccessManager(parent),
    helpEngine(engine)
{
}

QNetworkReply *HelpNetworkAccessManager::createRequest(Operation,
                                                       const QNetworkRequest& request,
                                                       QIODevice*)
{
    QUrl url = request.url();
    QString mimeType = url.toString();

    if (mimeType.endsWith(".svg") || mimeType.endsWith(".svgz"))
        mimeType = "image/svg+xml";
    else if (mimeType.endsWith(".css"))
        mimeType = "text/css";
    else if (mimeType.endsWith(".js"))
        mimeType = "text/javascript";
    else if (mimeType.endsWith(".txt"))
        mimeType = "text/plain";
    else
        mimeType = "text/html";

    QByteArray data = helpEngine->findFile(url).isValid()?
                          helpEngine->fileData(url):
                          QByteArray("File not found!");

    return new HelpNetworkReply(request, data, mimeType);
}

HelpPage::HelpPage(QHelpEngine *engine, QObject *parent) :
    QWebPage(parent),
    helpEngine(engine)
{
}

static bool isLocalUrl(const QUrl& url)
{
    const QString scheme = url.scheme();

    return  scheme.isEmpty()    || (scheme == "file") ||
           (scheme == "qrc")    || (scheme == "data") ||
           (scheme == "qthelp") || (scheme == "about");
}

bool HelpPage::acceptNavigationRequest(QWebFrame*,
                                       const QNetworkRequest& request,
                                       QWebPage::NavigationType)
{
    QUrl url = request.url();

    if (isLocalUrl(url))
    {
        QString path = url.path();

        if (path.endsWith(".pdf"))
        {
            int lastDash = path.lastIndexOf('/');
            QString fileName = QDir::tempPath()+QDir::separator();

            if (lastDash < 0)
                fileName += path;
            else
                fileName += path.mid(lastDash + 1, path.length());

            QFile file(QDir::cleanPath(fileName));

            if (file.open(QIODevice::ReadWrite))
            {
                file.write(helpEngine->fileData(url));

                file.close();
            }

            QDesktopServices::openUrl(QUrl(file.fileName()));

            return false;
        }

        return true;
    }

    QDesktopServices::openUrl(url);

    return false;
}

HelpWidget::HelpWidget(QHelpEngine *engine, MainWindow *parent) :
    QWebView(parent),
    helpEngine(engine)
{
    setAcceptDrops(false);

    setPage(new HelpPage(helpEngine, this));

    page()->setNetworkAccessManager(new HelpNetworkAccessManager(engine, this));

    connect(pageAction(QWebPage::Back), SIGNAL(changed()), this, SLOT(actionChanged()));
    connect(pageAction(QWebPage::Forward), SIGNAL(changed()), this, SLOT(actionChanged()));

    installEventFilter(this);
    setContextMenuPolicy(Qt::NoContextMenu);
}

void HelpWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        int delta = event->delta();

        if (delta > 0)
            zoomIn(0.01*delta);
        else if (delta < 0)
            zoomOut(-0.01*delta);

        event->accept();

        return;
    }

    QWebView::wheelEvent(event);
}

void HelpWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::XButton1)
    {
        triggerPageAction(QWebPage::Back);

        return;
    }

    if (event->button() == Qt::XButton2)
    {
        triggerPageAction(QWebPage::Forward);

        return;
    }

    QWebView::mouseReleaseEvent(event);
}

void HelpWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit mouseDoubleClick(event);
}

bool HelpWidget::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
        switch (static_cast<QKeyEvent*>(event)->key())
        {
            case Qt::Key_Backspace:
                if (isBackwardAvailable() && !hasFocus())
                    backward();

                    return true;

                break;
            default:
                return QWidget::eventFilter(object, event);
        }

    return QWidget::eventFilter(object, event);
}

void HelpWidget::actionChanged()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action == pageAction(QWebPage::Back))
        emit backwardAvailable(action->isEnabled());
    else if (action == pageAction(QWebPage::Forward))
        emit forwardAvailable(action->isEnabled());
}

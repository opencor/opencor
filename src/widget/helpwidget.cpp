// Note: the HelpWidget class is result of some refactored code taken from Qt
//       Assistant

#include "helpwidget.h"

#include <QDesktopServices>
#include <QHelpEngine>
#include <QTimer>
#include <QWebPage>
#include <QWheelEvent>

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

    QTimer::singleShot(0, this, SIGNAL(readyRead()));
}

qint64 HelpNetworkReply::readData(char *buffer, qint64 maxlen)
{
    qint64 len = qMin(qint64(data.length()), maxlen);

    if (len)
    {
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

    if (mimeType.endsWith(".txt"))
        mimeType = "text/plain";
    else
        mimeType = "text/html";

    QByteArray data;

    if (url.scheme() == "qthelp")
        data = helpEngine->findFile(url).isValid()?
                   helpEngine->fileData(url):
                   QByteArray("Help file not found! ["+url.toString().toLatin1()+"]");
    else
        data = QByteArray("External address... working on getting it to load...");

    return new HelpNetworkReply(request, data, mimeType);
}

HelpWidget::HelpWidget(QHelpEngine *engine, QWidget *parent) :
    QWebView(parent),
    helpEngine(engine)
{
    setAcceptDrops(false);

    page()->setNetworkAccessManager(new HelpNetworkAccessManager(engine, this));

    setContextMenuPolicy(Qt::NoContextMenu);
}

void HelpWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        int delta = event->delta();

        if (delta > 0)
            zoomIn(0.01*delta);
        else if (delta < 0)
            zoomOut(-0.01*delta);

        event->accept();
    }
    else
        QWebView::wheelEvent(event);
}

void HelpWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::XButton1)
        triggerPageAction(QWebPage::Back);
    else if (event->button() == Qt::XButton2)
        triggerPageAction(QWebPage::Forward);
    else
        QWebView::mouseReleaseEvent(event);
}

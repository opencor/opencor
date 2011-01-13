#include <QMessageBox>

// Note: the HelpWidget class is result of some refactored code taken from Qt
//       Assistant

#include "helpwidget.h"

#include <QDesktopServices>
#include <QFile>
#include <QHelpEngine>
#include <QTimer>
#include <QWebHistory>
#include <QWebPage>
#include <QWheelEvent>

HelpNetworkReply::HelpNetworkReply(const QNetworkRequest& pRequest,
                                   const QByteArray& pData,
                                   const QString& pMimeType) :
    mData(pData),
    mOrigLen(pData.length())
{
    setRequest(pRequest);
    setOpenMode(QIODevice::ReadOnly);
    setHeader(QNetworkRequest::ContentTypeHeader, pMimeType);
    setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(mOrigLen));

    QTimer::singleShot(0, this, SIGNAL(readyRead()));
}

qint64 HelpNetworkReply::readData(char *pBuffer, qint64 pMaxlen)
{
    qint64 len = qMin(qint64(mData.length()), pMaxlen);

    if (len)
    {
        qMemCopy(pBuffer, mData.constData(), len);

        mData.remove(0, len);
    }

    if (!mData.length())
        QTimer::singleShot(0, this, SIGNAL(finished()));

    return len;
}

HelpNetworkAccessManager::HelpNetworkAccessManager(QHelpEngine *pEngine,
                                                   QObject *pParent) :
    QNetworkAccessManager(pParent),
    mHelpEngine(pEngine)
{
    // Retrieve the error message template

    QFile helpWidgetErrorFile(":helpWidgetError");

    helpWidgetErrorFile.open(QIODevice::ReadOnly);

    mErrorMsg = QString(helpWidgetErrorFile.readAll()).trimmed();

    helpWidgetErrorFile.close();

    // Default network access manager (to be used to retrieved external pages)

    mDefaultNetworkAccessManager = new QNetworkAccessManager(this);
}

HelpNetworkAccessManager::~HelpNetworkAccessManager()
{
    delete mDefaultNetworkAccessManager;
}

QString HelpNetworkAccessManager::errorMsg(const QString& pErrorMsg)
{
    QString title = tr("OpenCOR Help Error");
    QString description = tr("Description");
    QString copyright = tr("Copyright");
    QString contact = tr("Please use our <A HREF = \"http://sourceforge.net/projects/opencor/support\">support page</A> to tell us about this error.");

    return mErrorMsg.arg(title, title, description, pErrorMsg, contact, copyright);
}

QNetworkReply *HelpNetworkAccessManager::createRequest(Operation,
                                                       const QNetworkRequest& pRequest,
                                                       QIODevice*)
{
    QUrl url = pRequest.url();
    QString mimeType = url.toString();

    if (mimeType.endsWith(".txt"))
        mimeType = "text/plain";
    else
        mimeType = "text/html";

    QByteArray data;

    if (url.scheme() == "qthelp")
    {
        data = mHelpEngine->findFile(url).isValid()?
                   mHelpEngine->fileData(url):
                   QByteArray(errorMsg(tr("The following help file could not be found:")+" <SPAN CLASS = \"Filename\">"+url.toString()+"</SPAN>.").toLatin1());

        return new HelpNetworkReply(pRequest, data, mimeType);
    }
    else
        return mDefaultNetworkAccessManager->get(QNetworkRequest(url));
}

HelpWidget::HelpWidget(QHelpEngine *engine, const QUrl& pHomepage,
                       QWidget *pParent) :
    QWebView(pParent),
    mHomepage(pHomepage)
{
    setAcceptDrops(false);

    page()->setNetworkAccessManager(new HelpNetworkAccessManager(engine, this));

    installEventFilter(this);

    setContextMenuPolicy(Qt::NoContextMenu);

    // Load the homepage

    load(mHomepage);
}

void HelpWidget::resetAll()
{
    gotoHomepage();

    history()->clear();

    setTextSizeMultiplier(1.0);

    // One would expect the history()->clear(); call to clear all of the
    //  history, but for some reason it sometimes still leaves one visited
    // page, so...

    int maximumItemCount = history()->maximumItemCount();

    history()->setMaximumItemCount(0);
    history()->setMaximumItemCount(maximumItemCount);
}

void HelpWidget::gotoHomepage()
{
    load(mHomepage);
}

void HelpWidget::zoomIn(const qreal& pRange)
{
    setTextSizeMultiplier(textSizeMultiplier()+0.1*pRange);
}

void HelpWidget::zoomOut(const qreal& pRange)
{
    setTextSizeMultiplier(qMax(0.0, textSizeMultiplier()-0.1*pRange));
}

void HelpWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::XButton1)
        triggerPageAction(QWebPage::Back);
    else if (pEvent->button() == Qt::XButton2)
        triggerPageAction(QWebPage::Forward);
    else
        QWebView::mouseReleaseEvent(pEvent);
}

void HelpWidget::wheelEvent(QWheelEvent *pEvent)
{
    if (pEvent->modifiers() & Qt::ControlModifier)
    {
        int delta = pEvent->delta();

        if (delta > 0)
            zoomIn(0.01*delta);
        else if (delta < 0)
            zoomOut(-0.01*delta);

        pEvent->accept();
    }
    else
        QWebView::wheelEvent(pEvent);
}

bool HelpWidget::eventFilter(QObject *pObject, QEvent *pEvent)
{
    if (pEvent->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(pEvent);

        switch (keyEvent->key())
        {
#ifndef Q_WS_MAC
            case Qt::Key_Home:
                gotoHomepage();

                return true;
#endif
            case Qt::Key_Up:
#ifdef Q_WS_MAC
                if (keyEvent->modifiers() & Qt::ControlModifier)
#else
                if (keyEvent->modifiers() & Qt::AltModifier)
#endif
                {
                    gotoHomepage();

                    return true;
                }

                break;
            case Qt::Key_Left:
#ifdef Q_WS_MAC
                if (keyEvent->modifiers() & Qt::ControlModifier)
#else
                if (keyEvent->modifiers() & Qt::AltModifier)
#endif
                {
                    back();

                    return true;
                }

                break;
            case Qt::Key_Right:
#ifdef Q_WS_MAC
                if (keyEvent->modifiers() & Qt::ControlModifier)
#else
                if (keyEvent->modifiers() & Qt::AltModifier)
#endif
                {
                    forward();

                    return true;
                }

                break;
        }
    }

    return QWidget::eventFilter(pObject, pEvent);
}

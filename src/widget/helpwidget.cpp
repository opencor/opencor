// Note: the HelpWidget class is result of some refactored code taken from Qt
//       Assistant

#include "helpwidget.h"

#include <QDesktopServices>
#include <QFile>
#include <QHelpEngine>
#include <QTimer>
#include <QWebPage>
#include <QWheelEvent>

HelpNetworkReply::HelpNetworkReply(const QNetworkRequest& pRequest,
                                   const QByteArray& pFileData,
                                   const QString& pMimeType) :
    mData(pFileData),
    mOrigLen(pFileData.length())
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
        data = mHelpEngine->findFile(url).isValid()?
                   mHelpEngine->fileData(url):
                   QByteArray(errorMsg(tr("The following help file could not be found")+": <SPAN CLASS = \"Filename\">"+url.toString()+"</SPAN>.").toLatin1());
    else
        data = QByteArray(errorMsg(tr("You tried to open an external link. This is not yet possible to do this through this help viewer, but we are working on it...")).toLatin1());

    return new HelpNetworkReply(pRequest, data, mimeType);
}

HelpWidget::HelpWidget(QHelpEngine *engine, QWidget *pParent) :
    QWebView(pParent)
{
    setAcceptDrops(false);

    page()->setNetworkAccessManager(new HelpNetworkAccessManager(engine, this));

    setContextMenuPolicy(Qt::NoContextMenu);
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

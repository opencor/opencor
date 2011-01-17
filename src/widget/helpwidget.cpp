// Note: the HelpWidget class is result of some refactored code taken from Qt
//       Assistant

#include "helpwidget.h"

#include <QAction>
#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
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

HelpNetworkAccessManager::HelpNetworkAccessManager(QHelpEngine *pHelpEngine,
                                                   QObject *pParent) :
    QNetworkAccessManager(pParent),
    mHelpEngine(pHelpEngine)
{
    // Retrieve the error message template

    QFile helpWidgetErrorFile(":helpWidgetError");

    helpWidgetErrorFile.open(QIODevice::ReadOnly);

    mErrorMsgTemplate = QString(helpWidgetErrorFile.readAll()).trimmed();

    helpWidgetErrorFile.close();
}

QString HelpNetworkAccessManager::errorMsg(const QString& pErrorMsg)
{
    QString title = tr("OpenCOR Help Error");
    QString description = tr("Description");
    QString copyright = tr("Copyright");
    QString contact = tr("Please use our <A HREF = \"http://sourceforge.net/projects/opencor/support\">support page</A> to tell us about this error.");

    return mErrorMsgTemplate.arg(title, title, description, pErrorMsg, contact, copyright);
}

QNetworkReply *HelpNetworkAccessManager::createRequest(Operation,
                                                       const QNetworkRequest& pRequest,
                                                       QIODevice*)
{
    QUrl url = pRequest.url().toString();
    // Note: the conversion to a string shouldn't be necessary, but for some
    //       reason it's required if we want the help engine to find the URL
    //      (?!), so...

    QByteArray data = mHelpEngine->findFile(url).isValid()?
                          mHelpEngine->fileData(url):
                          QByteArray(errorMsg(tr("The following help file could not be found:")+" <SPAN CLASS = \"Filename\">"+url.toString()+"</SPAN>.").toLatin1());

    return new HelpNetworkReply(pRequest, data, "text/html");
}

HelpPage::HelpPage(QHelpEngine *pHelpEngine, QObject *pParent) :
    QWebPage(pParent),
    mHelpEngine(pHelpEngine)
{
}

bool HelpPage::acceptNavigationRequest(QWebFrame*,
                                       const QNetworkRequest& pRequest,
                                       QWebPage::NavigationType)
{
    QUrl url = pRequest.url();

    if (url.scheme() == "qthelp")
        return true;
    else
    {
        QDesktopServices::openUrl(url);

        return false;
    }
}

HelpWidget::HelpWidget(QHelpEngine *pHelpEngine, const QUrl& pHomePage,
                       QWidget *pParent) :
    QWebView(pParent),
    mHomePage(pHomePage)
{
    setAcceptDrops(false);
    setContextMenuPolicy(Qt::NoContextMenu);

    setPage(new HelpPage(pHelpEngine, this));

    page()->setNetworkAccessManager(new HelpNetworkAccessManager(pHelpEngine, this));

    connect(pageAction(QWebPage::Back), SIGNAL(changed()),
            this, SLOT(actionChanged()));
    connect(pageAction(QWebPage::Forward), SIGNAL(changed()),
            this, SLOT(actionChanged()));

    // Prevent text selection

    page()->settings()->setUserStyleSheetUrl(QUrl::fromLocalFile(":noTextSelection"));
}

void HelpWidget::defaultSettings()
{
    gotoHomePage();

    history()->clear();

    resetZoom();

    // One would expect history()->clear(); to clear all of the history, but
    // for some reason it sometimes still leaves one visited page, so...

    int maximumItemCount = history()->maximumItemCount();

    history()->setMaximumItemCount(0);
    history()->setMaximumItemCount(maximumItemCount);

    // Make sure we let whatever user of the widget know that we cannot go
    // back or forward

    emit backAvailable(false);
    emit forwardAvailable(false);
}

QUrl HelpWidget::homePage()
{
    return mHomePage;
}

void HelpWidget::gotoHomePage()
{
    load(mHomePage);
}

int HelpWidget::minimumZoomLevel()
{
    return 1;
}

int HelpWidget::defaultZoomLevel()
{
    return 10;
}

void HelpWidget::resetZoom()
{
    setZoomLevel(defaultZoomLevel());
}

void HelpWidget::zoomIn()
{
    setZoomLevel(++mZoomLevel);
}

void HelpWidget::zoomOut()
{
    setZoomLevel(qMax(minimumZoomLevel(), --mZoomLevel));
}

void HelpWidget::setZoomLevel(const int& pZoomLevel)
{
    mZoomLevel = pZoomLevel;

    setZoomFactor(0.1*mZoomLevel);

    emit zoomLevelChanged(mZoomLevel);
}

int HelpWidget::zoomLevel()
{
    return mZoomLevel;
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
            zoomIn();
        else if (delta < 0)
            zoomOut();

        pEvent->accept();
    }
    else
        QWebView::wheelEvent(pEvent);
}

QSize HelpWidget::sizeHint() const
{
    const double spaceRatio = 1.0/7.0;
    const QRect desktopGeometry = qApp->desktop()->availableGeometry();

    return QSize(spaceRatio*desktopGeometry.width(), spaceRatio*desktopGeometry.height());
}

void HelpWidget::actionChanged()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action == pageAction(QWebPage::Back))
        emit backAvailable(action->isEnabled());
    else if (action == pageAction(QWebPage::Forward))
        emit forwardAvailable(action->isEnabled());
}

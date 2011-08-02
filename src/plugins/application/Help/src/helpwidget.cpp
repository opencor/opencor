#include "helpwidget.h"

#include <QAction>
#include <QDesktopServices>
#include <QFile>
#include <QHelpEngine>
#include <QPaintEvent>
#include <QSettings>
#include <QTimer>
#include <QWebHistory>

namespace OpenCOR {
namespace Help {

HelpNetworkReply::HelpNetworkReply(const QNetworkRequest &pRequest,
                                   const QByteArray &pData,
                                   const QString &pMimeType) :
    mData(pData),
    mOrigLen(pData.length())
{
    // Set a few things for the network reply

    setRequest(pRequest);
    setOpenMode(QIODevice::ReadOnly);
    setHeader(QNetworkRequest::ContentTypeHeader, pMimeType);
    setHeader(QNetworkRequest::ContentLengthHeader,
              QByteArray::number(mOrigLen));

    // Let ourselves know immediately that data is available for reading

    QTimer::singleShot(0, this, SIGNAL(readyRead()));
}

void HelpNetworkReply::abort()
{
    // Do nothing on purpose...
}

qint64 HelpNetworkReply::bytesAvailable() const
{
    // Return the size of the data which is available for reading

    return mData.length()+QNetworkReply::bytesAvailable();
}

qint64 HelpNetworkReply::readData(char *pBuffer, qint64 pMaxlen)
{
    // Determine the lenght of the data to be read

    qint64 len = qMin(qint64(mData.length()), pMaxlen);

    // Read the data, should there be some to read

    if (len) {
        qMemCopy(pBuffer, mData.constData(), len);

        mData.remove(0, len);
    }

    // Should there be no data left to read, then let ourselves know
    // immediately that we are done

    if (!mData.length())
        QTimer::singleShot(0, this, SIGNAL(finished()));

    // Return the size of the data which was read

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

    mErrorMsgTemplate = QString(helpWidgetErrorFile.readAll());

    helpWidgetErrorFile.close();
}

QNetworkReply *HelpNetworkAccessManager::createRequest(Operation,
                                                       const QNetworkRequest &pRequest,
                                                       QIODevice*)
{
    // Reqested URL

    QUrl url = pRequest.url().toString();
    // Note: the conversion to a string shouldn't be necessary, but for some
    //       reason it's required if we want the help engine to find the URL
    //      (?!), so...

    // Retrieve, if possible, the requested help page

    QByteArray data = mHelpEngine->findFile(url).isValid()?
                          mHelpEngine->fileData(url):
                          QByteArray(mErrorMsgTemplate.arg("The following help file could not be found: <SPAN CLASS = \"Filename\">"+url.toString()+"</SPAN>.").toLatin1());

    // Return the requested help page or an error message

    return new HelpNetworkReply(pRequest, data, "text/html");
}

HelpPage::HelpPage(QHelpEngine *pHelpEngine, QObject *pParent) :
    QWebPage(pParent),
    mHelpEngine(pHelpEngine)
{
}

bool HelpPage::acceptNavigationRequest(QWebFrame*,
                                       const QNetworkRequest &pRequest,
                                       QWebPage::NavigationType)
{
    // Requested URL

    QUrl url = pRequest.url();

    // Determine whether the URL refers to an OpenCOR help page or an external
    // link of sorts

    if (url.scheme() == "qthelp") {
        // This an OpenCOR help page, so we are happy to handle the request

        return true;
    } else {
        // This is an external link of sorts, so we leave it to the user's
        // default web browser to handle the request

        QDesktopServices::openUrl(url);

        return false;
    }
}

HelpWidget::HelpWidget(const QString &pName, QHelpEngine *pHelpEngine,
                       const QUrl &pHomePage, QWidget *pParent) :
    QWebView(pParent),
    CommonWidget(pName, this, pParent),
    mHomePage(pHomePage),
    mBackAvailable(false),
    mForwardAvailable(false)
{
    // Add a small margin to the widget, so that no visual trace of the border
    // drawn by drawBorderIfDocked is left when scrolling

    setStyleSheet("QWebView { margin: 1px; }");
    // Note: not sure why, but no matter how many pixels are specified for the
    //       margin, no margin actually exists, but it addresses the issue with
    //       border drawn by drawBorderIfDocked, so...

    // Use our own help page and help network access manager classes

    setPage(new HelpPage(pHelpEngine, this));

    page()->setNetworkAccessManager(new HelpNetworkAccessManager(pHelpEngine,
                                                                 this));


    // Prevent objects from being dropped on us
    // Note: by default, QWebView allows for objects to be dropped on itself,
    //       so...

    setAcceptDrops(false);

    // Prevent the widget from taking over the scrolling of other widgets

    setFocusPolicy(Qt::NoFocus);

    // Set our initial zoom level to the default value
    // Note: to set mZoomLevel directly is not good enough since one of the
    //       things setZoomLevel does is to set our zoom factor, so...

    setZoomLevel(defaultZoomLevel());

    // Some connections

    connect(pageAction(QWebPage::Back), SIGNAL(changed()),
            this, SLOT(webPageChanged()));
    connect(pageAction(QWebPage::Forward), SIGNAL(changed()),
            this, SLOT(webPageChanged()));

    // Go to the home page

    gotoHomePage();
}

static const QString SettingsZoomLevel = "ZoomLevel";

void HelpWidget::loadSettings(QSettings *pSettings)
{
    pSettings->beginGroup(objectName());
        // Retrieve the zoom level

        setZoomLevel(pSettings->value(SettingsZoomLevel,
                                      defaultZoomLevel()).toInt());
    pSettings->endGroup();
}

void HelpWidget::saveSettings(QSettings *pSettings) const
{
    pSettings->beginGroup(objectName());
        // Keep track of the text size multiplier

        pSettings->setValue(SettingsZoomLevel, zoomLevel());
    pSettings->endGroup();
}

QUrl HelpWidget::homePage() const
{
    // Return the URL for the home page

    return mHomePage;
}

void HelpWidget::gotoHomePage()
{
    // Go to the home page

    load(mHomePage);
}

bool HelpWidget::isBackAvailable() const
{
    // Return whether we can go to the previous help or not

    return mBackAvailable;
}

bool HelpWidget::isForwardAvailable() const
{
    // Return whether we can go to the next help or not

    return mForwardAvailable;
}

int HelpWidget::minimumZoomLevel() const
{
    // Return the minimum zoom level

    return 1;
}

int HelpWidget::defaultZoomLevel() const
{
    // Return the default zoom level

    return 9;
}

void HelpWidget::resetZoom()
{
    // Reset the zoom level

    setZoomLevel(defaultZoomLevel());
}

void HelpWidget::zoomIn()
{
    // Zoom in the help page contents

    setZoomLevel(++mZoomLevel);
}

void HelpWidget::zoomOut()
{
    // Zoom out the help page contents

    setZoomLevel(qMax(minimumZoomLevel(), --mZoomLevel));
}

void HelpWidget::setZoomLevel(const int &pZoomLevel)
{
    // Set the zoom level of the help page contents to a particular value

    mZoomLevel = pZoomLevel;

    setZoomFactor(0.1*mZoomLevel);

    emit zoomLevelChanged(mZoomLevel);
}

int HelpWidget::zoomLevel() const
{
    // Return the current zoom level for the help page contents

    return mZoomLevel;
}

void HelpWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Handle some special mouse buttons for navigating the help

    if (pEvent->button() == Qt::XButton1) {
        // Special mouse button #1 which is used to go to the previous help
        // page

        triggerPageAction(QWebPage::Back);

        // Accept the event

        pEvent->accept();
    } else if (pEvent->button() == Qt::XButton2) {
        // Special mouse button #2 which is used to go to the next help page

        triggerPageAction(QWebPage::Forward);

        // Accept the event

        pEvent->accept();
    } else {
        // Something else, so use the default handling of the event

        QWebView::mouseReleaseEvent(pEvent);
    }
}

void HelpWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Handle the wheel mouse button for zooming in/out the help page contents

    if (pEvent->modifiers() & Qt::ControlModifier) {
        // The user has pressed the Ctrl key (on Windows and Linux) or the Alt
        // key (on Mac OS X)

        int delta = pEvent->delta();

        if (delta > 0)
            // We are going 'up' which means zooming in

            zoomIn();
        else if (delta < 0)
            // We are going 'down' which means zooming out

            zoomOut();

        // Accept the event

        pEvent->accept();
    } else {
        // Something else, so use the default handling of the event

        QWebView::wheelEvent(pEvent);
    }
}

QSize HelpWidget::sizeHint() const
{
    // Suggest a default size for the help widget
    // Note: this is critical if we want a docked widget, with a help widget
    //       on it, to have a decent size when docked to the main window

    return defaultSize(0.2);
}

void HelpWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QWebView::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked(true, true, false, false, false);
}

void HelpWidget::webPageChanged()
{
    // A new help page has been selected, resulting in the previous or next
    // help page becoming either available or not

    QAction *action = qobject_cast<QAction *>(sender());

    if (action) {
        // The QObject casting was successful, so we can carry on

        if (action == pageAction(QWebPage::Back)) {
            // The current action is to tell us whether the previous help page
            // is available or not

            mBackAvailable = action->isEnabled();

            // Send a signal to let the user know of the new status of the
            // previous help page

            emit backAvailable(mBackAvailable);
        } else if (action == pageAction(QWebPage::Forward)) {
            // The current action is to tell us whether the next help page is
            // available or not

            mForwardAvailable = action->isEnabled();

            // Send a signal to let the user know of the new status of the next
            // help page

            emit forwardAvailable(mForwardAvailable);
        }
    }
}

} }

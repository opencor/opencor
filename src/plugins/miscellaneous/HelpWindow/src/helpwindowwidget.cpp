/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Help window widget
//==============================================================================

#include "corecliutils.h"
#include "helpwindowwidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QDesktopServices>
#include <QHelpEngine>
#include <QIODevice>
#include <QMouseEvent>
#include <QSettings>
#include <QTimer>
#include <QWebPage>

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

HelpWindowNetworkReply::HelpWindowNetworkReply(const QNetworkRequest &pRequest,
                                               const QByteArray &pData,
                                               const QString &pMimeType) :
    mData(pData)
{
    // Set a few things for the network reply

    setRequest(pRequest);
    setOpenMode(QIODevice::ReadOnly);
    setHeader(QNetworkRequest::ContentTypeHeader, pMimeType);
    setHeader(QNetworkRequest::ContentLengthHeader,
              QByteArray::number(pData.length()));

    // Let ourselves know immediately that data is available for reading

    QTimer::singleShot(0, this, SIGNAL(readyRead()));
}

//==============================================================================

void HelpWindowNetworkReply::abort()
{
    // Do nothing on purpose...
}

//==============================================================================

qint64 HelpWindowNetworkReply::bytesAvailable() const
{
    // Return the size of the data which is available for reading

    return mData.length()+QNetworkReply::bytesAvailable();
}

//==============================================================================

qint64 HelpWindowNetworkReply::readData(char *pBuffer, qint64 pMaxlen)
{
    // Determine the lenght of the data to be read

    qint64 len = qMin(qint64(mData.length()), pMaxlen);

    // Read the data, should there be some to read

    if (len) {
        memcpy(pBuffer, mData.constData(), len);

        mData.remove(0, len);
    }

    // Should there be no data left to read, then let ourselves know immediately
    // that we are done

    if (!mData.length())
        QTimer::singleShot(0, this, SIGNAL(finished()));

    // Return the size of the data which was read

    return len;
}

//==============================================================================

HelpWindowNetworkAccessManager::HelpWindowNetworkAccessManager(QHelpEngine *pHelpEngine,
                                                               QObject *pParent) :
    QNetworkAccessManager(pParent),
    mHelpEngine(pHelpEngine)
{
    // Retrieve the error message template

    Core::readFileContentsFromFile(":/HelpWindow/error.html", mErrorMessageTemplate);
}

//==============================================================================

QNetworkReply * HelpWindowNetworkAccessManager::createRequest(Operation pOperation,
                                                              const QNetworkRequest &pRequest,
                                                              QIODevice *pOutgoingData)
{
    Q_UNUSED(pOperation);
    Q_UNUSED(pOutgoingData);

    // Retrieve, if possible, the requested document

    QUrl url = pRequest.url();
    QByteArray data = mHelpEngine->findFile(url).isValid()?
                          mHelpEngine->fileData(url):
                          mErrorMessageTemplate.arg(tr("Error"),
                                                    tr("The following help file could not be found:")+" <strong>"+url.toString()+"</strong>.",
                                                    tr("Please <a href=\"contactUs.html\">contact us</a> about this error."),
                                                    Core::copyright()).toUtf8();

    // Return the requested document or an error message

    return new HelpWindowNetworkReply(pRequest, data, "text/html");
}

//==============================================================================

HelpWindowWidget::HelpWindowWidget(QHelpEngine *pHelpEngine,
                                   const QUrl &pHomePage, QWidget *pParent) :
    WebViewerWidget::WebViewerWidget(pParent),
    mHelpEngine(pHelpEngine),
    mHomePage(pHomePage)
{
    // Use our own help network access manager classes

    page()->setNetworkAccessManager(new HelpWindowNetworkAccessManager(pHelpEngine, this));

    // Some connections

    connect(this, SIGNAL(urlChanged(const QUrl &)),
            this, SLOT(urlChanged(const QUrl &)));

    connect(page(), SIGNAL(selectionChanged()),
            this, SLOT(selectionChanged()));

    connect(pageAction(QWebPage::Back), SIGNAL(changed()),
            this, SLOT(documentChanged()));
    connect(pageAction(QWebPage::Forward), SIGNAL(changed()),
            this, SLOT(documentChanged()));

    // Go to the home page

    goToHomePage();
}

//==============================================================================

void HelpWindowWidget::retranslateUi()
{
    // Retranslate the current document, but only if it is an error document
    // since a valid document is hard-coded and therefore cannot be translated
    // Note: we use setUrl() rather than reload() since the latter won't work
    //       upon starting OpenCOR with a non-system locale...

    if (!mHelpEngine->findFile(url()).isValid())
        setUrl(url());
}

//==============================================================================

void HelpWindowWidget::loadSettings(QSettings *pSettings)
{
    // Default handling of the event

    WebViewerWidget::WebViewerWidget::loadSettings(pSettings);

    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emit notHomePage(false);

    emit backEnabled(false);
    emit forwardEnabled(false);

    emit copyTextEnabled(false);
}

//==============================================================================

void HelpWindowWidget::saveSettings(QSettings *pSettings) const
{
    // Default handling of the event

    WebViewerWidget::WebViewerWidget::saveSettings(pSettings);
}

//==============================================================================

bool HelpWindowWidget::isUrlSchemeSupported(const QString &pUrlScheme)
{
    // We only support URLs that refer to an OpenCOR document (qthelp://...)

    return !pUrlScheme.compare("qthelp");
}

//==============================================================================

void HelpWindowWidget::goToHomePage()
{
    // Go to the home page
    // Note: we use setUrl() rather than load() since the former will ensure
    //       that url() becomes valid straightaway (which is important for
    //       retranslateUi()) and that the document gets loaded immediately...

    setUrl(mHomePage);
}

//==============================================================================

QSize HelpWindowWidget::sizeHint() const
{
    // Suggest a default size for the help widget
    // Note: this is critical if we want a docked widget, with a help widget on
    //       it, to have a decent size when docked to the main window...

    return defaultSize(0.2);
}

//==============================================================================

void HelpWindowWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Handle some special mouse buttons for navigating the help

    if (pEvent->button() == Qt::XButton1) {
        // Special mouse button #1 which is used to go to the previous help
        // document

        triggerPageAction(QWebPage::Back);

        // Accept the event

        pEvent->accept();
    } else if (pEvent->button() == Qt::XButton2) {
        // Special mouse button #2 which is used to go to the next help document

        triggerPageAction(QWebPage::Forward);

        // Accept the event

        pEvent->accept();
    } else {
        // Something else, so use the default handling of the event

        WebViewerWidget::WebViewerWidget::mouseReleaseEvent(pEvent);
    }
}

//==============================================================================

void HelpWindowWidget::urlChanged(const QUrl &pUrl)
{
    // The URL has changed, so let the user know whether it's the home page

    emit notHomePage(pUrl != mHomePage);
}

//==============================================================================

void HelpWindowWidget::selectionChanged()
{
    // The text selection has changed, so let the user know whether some text is
    // now selected

    emit copyTextEnabled(!selectedText().isEmpty());
}

//==============================================================================

void HelpWindowWidget::documentChanged()
{
    // A new help document has been selected, resulting in the previous or next
    // help document becoming either available or not

    QAction *action = qobject_cast<QAction *>(sender());

    if (action == pageAction(QWebPage::Back))
        emit backEnabled(action->isEnabled());
    else if (action == pageAction(QWebPage::Forward))
        emit forwardEnabled(action->isEnabled());
}

//==============================================================================

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

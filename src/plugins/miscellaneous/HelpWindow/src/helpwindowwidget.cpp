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

#include <QAction>
#include <QDesktopServices>
#include <QDir>
#include <QHelpEngine>
#include <QIODevice>
#include <QMouseEvent>
#include <QSettings>
#include <QTimer>
#include <QWebPage>
#include <QWebView>

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

HelpWindowWidget::HelpWindowWidget(QWidget *pParent) :
    WebViewerWidget::WebViewerWidget(pParent)
{
    // Extract the help files

    QString applicationBaseFileName =  QDir::tempPath()+QDir::separator()
                                      +QFileInfo(qApp->applicationFilePath()).baseName();

    mQchFileName = applicationBaseFileName+".qch";
    mQhcFileName = applicationBaseFileName+".qhc";

    Core::writeResourceToFile(mQchFileName, ":HelpWindow_qchFile");
    Core::writeResourceToFile(mQhcFileName, ":HelpWindow_qhcFile");

    // Set up the help engine

    mHelpEngine = new QHelpEngine(mQhcFileName);

    // Use our own help network access manager classes

    webView()->page()->setNetworkAccessManager(new HelpWindowNetworkAccessManager(mHelpEngine, this));

    // Set and go to our home page
    // Note: we call setUrl() rather than goToHomePage() because we want and
    //       need our home page to be loaded straightaway (otherwise if our home
    //       page is wrong and OpenCOR is in a non-English locale then our
    //       contents will empty upon starting OpenCOR)...

    setHomePage("qthelp://opencor/doc/user/index.html");

    webView()->setUrl(homePage());
}

//==============================================================================

HelpWindowWidget::~HelpWindowWidget()
{
    // Delete some internal objects

    delete mHelpEngine;

    // Delete the help files

    QFile(mQchFileName).remove();
    QFile(mQhcFileName).remove();
}

//==============================================================================

void HelpWindowWidget::retranslateUi()
{
    // Retranslate the current document, but only if it is an error document
    // since a valid document is hard-coded and therefore cannot be retranslated

    if (!mHelpEngine->findFile(webView()->url()).isValid())
        webView()->load(webView()->url());
}

//==============================================================================

void HelpWindowWidget::loadSettings(QSettings *pSettings)
{
    // Default handling of the event

    WebViewerWidget::WebViewerWidget::loadSettings(pSettings);
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

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// PMR web service manager
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrauthentication.h"
#include "pmrwebservice.h"
#include "pmrwebservicemanager.h"
#include "pmrwebserviceresponse.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QDesktopServices>
#include <QMainWindow>

//==============================================================================

#include "o1requestor.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWebServiceManager::PmrWebServiceManager(PmrWebService *pPmrWebService) :
    QNetworkAccessManager(pPmrWebService),
    mPmrWebService(pPmrWebService),
    mWebViewer(0)
{
    // Create an OAuth client for authenticated requests to PMR

    mPmrAuthentication = new PmrAuthentication(PmrUrl, this);

    // Make sure that we get told when there are SSL errors (which would happen
    // if the website's certificate is invalid, e.g. it has expired)

    connect(this, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(sslErrors(QNetworkReply *, const QList<QSslError> &)));

    // Connect some signals

    connect(mPmrAuthentication, SIGNAL(linkingSucceeded()),
            this, SLOT(authenticationSucceeded()));
    connect(mPmrAuthentication, SIGNAL(linkingFailed()),
            this, SLOT(authenticationFailed()));

    connect(mPmrAuthentication, SIGNAL(openBrowser(const QUrl &)),
            this, SLOT(openBrowser(const QUrl &)));
    connect(mPmrAuthentication, SIGNAL(closeBrowser()),
            this, SLOT(closeBrowser()));
}

//==============================================================================

bool PmrWebServiceManager::isAuthenticated() const
{
    // Return whether we are authenticated to PMR

    return mPmrAuthentication->linked();
}

//==============================================================================

void PmrWebServiceManager::authenticate(const bool &pAuthenticate)
{
    // Authenticate ourselves to PMR

    if (pAuthenticate)
        mPmrAuthentication->link();
    else
        mPmrAuthentication->unlink();
}

//==============================================================================

void PmrWebServiceManager::authenticationSucceeded()
{
    // Let people know whether we are really authenticated, i.e. linked

    emit authenticated(qobject_cast<PmrAuthentication *>(sender())->linked());
}

//==============================================================================

void PmrWebServiceManager::authenticationFailed()
{
    // Let people know that authentication failed

    emit authenticated(false);
}

//==============================================================================

void PmrWebServiceManager::openBrowser(const QUrl &pUrl)
{
    // Open the given URL in our temporary web browser

    mWebViewer = new WebViewerWidget::WebViewerWidget(Core::mainWindow());

    mWebViewer->setContextMenuPolicy(Qt::NoContextMenu);
    mWebViewer->setUrl(pUrl);
    mWebViewer->setWindowFlags(Qt::Sheet);

    mWebViewer->show();
}

//==============================================================================

void PmrWebServiceManager::closeBrowser()
{
    // Close and delete our temporary web browser

    mWebViewer->close();

    delete mWebViewer;
}

//==============================================================================

void PmrWebServiceManager::sslErrors(QNetworkReply *pNetworkReply,
                                     const QList<QSslError> &pSslErrors)
{
    // Ignore SSL errors since we trust the website and therefore its
    // certificate (even if it is invalid, e.g. it has expired)

    pNetworkReply->ignoreSslErrors(pSslErrors);
}

//==============================================================================

PmrWebServiceResponse * PmrWebServiceManager::request(const QString &pUrl,
                                                      const bool &pSecureRequest,
                                                      const bool &pUsePost,
                                                      const QJsonDocument &pJsonDocument)
{
    // Check that we are connected to the Internet

    if (!Core::internetConnectionAvailable()) {
        emit error(Core::noInternetConnectionAvailableMessage());

        return 0;
    }

    // Let the user know that we are (going to be) busy

    emit busy(true);

    // Send our request to PMR, asking for the response to be compressed

    QNetworkRequest networkRequest;

    networkRequest.setRawHeader("Accept", "application/json");
    networkRequest.setRawHeader("Accept-Encoding", "gzip");

    networkRequest.setUrl(QUrl(pUrl));

    // Use the authenticated link if it's available

    QNetworkReply *networkReply;

    if (pSecureRequest && mPmrAuthentication->linked()) {
        O1Requestor *requestor = new O1Requestor(this, mPmrAuthentication, this);

        if (!pUsePost && pJsonDocument.isEmpty()) {
            networkReply = requestor->get(networkRequest, QList<O0RequestParameter>());
        } else {
            networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, RequestMimeType);

            networkReply = requestor->post(networkRequest, QList<O0RequestParameter>(),
                                           pJsonDocument.isNull()?
                                               QByteArray():
                                               pJsonDocument.toJson(QJsonDocument::Compact));
        }
    } else {
        if (!pUsePost && pJsonDocument.isEmpty()) {
            networkReply = get(networkRequest);
        } else {
            networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, RequestMimeType);

            networkReply = post(networkRequest,
                                pJsonDocument.isNull()?
                                    QByteArray():
                                    pJsonDocument.toJson(QJsonDocument::Compact));
        }
    }

    PmrWebServiceResponse *pmrWebServiceResponse = new PmrWebServiceResponse(networkReply);

    connect(pmrWebServiceResponse, SIGNAL(busy(const bool &)),
            mPmrWebService, SIGNAL(busy(const bool &)));
    connect(pmrWebServiceResponse, SIGNAL(error(const QString &)),
            mPmrWebService, SIGNAL(error(const QString &)));
    connect(pmrWebServiceResponse, SIGNAL(forbidden(const QString &)),
            mPmrWebService, SLOT(forbidden(const QString &)));

    return pmrWebServiceResponse;
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

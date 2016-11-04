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
#include "pmrauthentication.h"
#include "pmrwebservice.h"
#include "pmrwebservicemanager.h"
#include "pmrwebserviceresponse.h"

//==============================================================================

#include <QDesktopServices>

//==============================================================================

#include "o1requestor.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWebServiceManager::PmrWebServiceManager(PmrWebService *pPmrWebService) :
    QNetworkAccessManager(pPmrWebService),
    mPmrWebService(pPmrWebService)
{
    // Create an OAuth client for authenticated requests to PMR

    mPmrOAuthClient = new PmrOauthClient(PmrUrl, this);

    // Make sure that we get told when there are SSL errors (which would happen
    // if the website's certificate is invalid, e.g. it has expired)

    connect(this, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(sslErrors(QNetworkReply *, const QList<QSslError> &)));

    // Connect some signals

    connect(mPmrOAuthClient, SIGNAL(linkingSucceeded()),
            this, SLOT(authenticationSucceeded()));
    connect(mPmrOAuthClient, SIGNAL(linkingFailed()),
            this, SLOT(authenticationFailed()));

    connect(mPmrOAuthClient, SIGNAL(openBrowser(const QUrl &)),
            this, SLOT(openBrowser(const QUrl &)));
}

//==============================================================================

bool PmrWebServiceManager::isAuthenticated() const
{
    // Return whether we are authenticated to PMR

    return mPmrOAuthClient->linked();
}

//==============================================================================

void PmrWebServiceManager::authenticate(const bool &pAuthenticate)
{
    // Authenticate ourselves to PMR

    if (pAuthenticate)
        mPmrOAuthClient->link();
    else
        mPmrOAuthClient->unlink();
}

//==============================================================================

void PmrWebServiceManager::authenticationSucceeded()
{
    // Let people know whether we are really authenticated, i.e. linked

    emit authenticated(qobject_cast<PmrOauthClient *>(sender())->linked());
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
    // Open the given URL in the user's browser

    QDesktopServices::openUrl(pUrl);
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
        emit error(tr("Cannot connect to the Internet"), false);

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

    if (pSecureRequest && mPmrOAuthClient->linked()) {
        O1Requestor *requestor = new O1Requestor(this, mPmrOAuthClient, this);

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
    connect(pmrWebServiceResponse, SIGNAL(error(const QString &, const bool &)),
            mPmrWebService, SIGNAL(error(const QString &, const bool &)));
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

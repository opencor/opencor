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
// PMR network manager
//==============================================================================

#pragma once

//==============================================================================

#include <QJsonDocument>
#include <QList>
#include <QNetworkAccessManager>
#include <QSslError>

//==============================================================================

class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrOauthClient;
class PmrWebService;
class PmrWebServiceResponse;

//==============================================================================

class PmrWebServiceManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit PmrWebServiceManager(PmrWebService *pPmrWebService);
    ~PmrWebServiceManager();

    void authenticate(const bool &pLink);
    bool isAuthenticated() const;

    PmrWebServiceResponse * sendPmrRequest(const QString &pUrl,
                                           const bool &pSecureRequest,
                                           const bool &pUsePost = false,
                                           const QJsonDocument &pJsonDocument = QJsonDocument());
private:
    PmrOauthClient *mPmrOAuthClient;
    PmrWebService *mPmrWebService;

signals:
    void authenticated(const bool &pAuthenticated);
    void busy(const bool &pBusy);
    void error(const QString &pErrorMessage,
               const bool &pInternetConnectionAvailable);

private slots:
    void authenticationFailed();
    void authenticationSucceeded();
    void openBrowser(const QUrl &pUrl);

    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);

};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

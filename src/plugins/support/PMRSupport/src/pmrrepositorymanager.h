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

class PmrOAuthClient;
class PmrRepository;
class PmrRepositoryResponse;

//==============================================================================

class PmrRepositoryManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit PmrRepositoryManager(PmrRepository *pPmrRepository);
    virtual ~PmrRepositoryManager();

    void authenticate(const bool &pLink);
    bool isAuthenticated(void) const;

    PmrRepositoryResponse *sendPmrRequest(const QString &pUrl, const bool &secure,
                                          const QJsonDocument &pJsonDocument = QJsonDocument());
private:
    PmrOAuthClient *mPmrOAuthClient;
    PmrRepository *mPmrRepository;

Q_SIGNALS:
    void authenticated(const bool &pAuthenticated);
    void busy(const bool &pBusy);
    void error(const QString &pErrorMessage, const bool &pInternetConnectionAvailable);

private Q_SLOTS:
    void authenticationFailed();
    void authenticationSucceeded();
    void openBrowser(const QUrl &pUrl);

    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);

};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// PMR Web service manager
//==============================================================================

#pragma once

//==============================================================================

#include <QJsonDocument>
#include <QNetworkAccessManager>

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrAuthentication;
class PmrWebService;
class PmrWebServiceResponse;
class PmrWebViewerDialog;

//==============================================================================

class PmrWebServiceManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit PmrWebServiceManager(const QString &pPmrUrl,
                                  PmrWebService *pPmrWebService);

    bool isAuthenticated() const;
    void authenticate(bool pAuthenticate);

    PmrWebServiceResponse * request(const QString &pUrl, bool pSecureRequest,
                                    bool pUsePost = false,
                                    const QJsonDocument &pJsonDocument = {});

    void update(const QString &pPmrUrl);

private:
    PmrWebService *mPmrWebService;
    PmrAuthentication *mPmrAuthentication = nullptr;

    PmrWebViewerDialog *mWebViewerDialog = nullptr;

    bool mWebViewerDialogUsed = false;

signals:
    void busy(bool pBusy);
    void authenticated(bool pAuthenticated);

    void error(const QString &pErrorMessage);

    void authenticationCancelled();

private slots:
    void authenticationSucceeded();
    void authenticationFailed();

    void openBrowser(const QUrl &pUrl);
    void startCloseBrowser();
    void closeBrowser();

    void ignoreSslErrors(QNetworkReply *pNetworkReply,
                         const QList<QSslError> &pSslErrors);
};

//==============================================================================

} // namespace PMRSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// PMR web service manager
//==============================================================================

#pragma once

//==============================================================================

#include <QJsonDocument>
#include <QNetworkAccessManager>

//==============================================================================

class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class Dialog;
}   // namespace Core

//==============================================================================

namespace WebViewerWidget {
    class WebViewerWidget;
}   // namespace WebViewerWidget

//==============================================================================

namespace PMRSupport {

//==============================================================================

class PmrAuthentication;
class PmrWebService;
class PmrWebServiceResponse;

//==============================================================================

class PmrWebServiceManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit PmrWebServiceManager(const QString &pPmrUrl,
                                  PmrWebService *pPmrWebService);
    ~PmrWebServiceManager();

    bool isAuthenticated() const;
    void authenticate(const bool &pAuthenticate);

    PmrWebServiceResponse * request(const QString &pUrl,
                                    const bool &pSecureRequest,
                                    const bool &pUsePost = false,
                                    const QJsonDocument &pJsonDocument = QJsonDocument());

    void update(const QString &pPmrUrl);

private:
    QSettings *mSettings;

    PmrWebService *mPmrWebService;
    PmrAuthentication *mPmrAuthentication;

    Core::Dialog *mWebViewerDialog;
    WebViewerWidget::WebViewerWidget *mWebViewer;

    bool mWebViewerUsed;

signals:
    void busy(const bool &pBusy);
    void authenticated(const bool &pAuthenticated);

    void error(const QString &pErrorMessage);

    void authenticationCancelled();

private slots:
    void authenticationSucceeded();
    void authenticationFailed();

    void openBrowser(const QUrl &pUrl);
    void closeBrowser();

    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

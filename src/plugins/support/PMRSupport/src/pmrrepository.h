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
// PMR repository
//==============================================================================

#pragma once

//==============================================================================

#include "pmrauthentication.h"
#include "pmrexposure.h"
#include "pmrsupportglobal.h"
#include "pmrworkspace.h"

//==============================================================================

#include "o1.h"

//==============================================================================

#include <QList>
#include <QSslError>
#include <QUrl>
#include <QVariantMap>

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PMRSUPPORT_EXPORT PmrRepository : public QObject
{
    Q_OBJECT

public:
    static PmrRepository *newPmrRepository();
    ~PmrRepository();

    QString url(void) const;

    void cloneWorkspace(const QString &pUrl, const QString &pDirName);
    void requestExposuresList(void);
    void requestExposureFiles(const QString &pUrl);
    void requestWorkspacesList(void);
    void requestWorkspaceDetails(const QString &pUrl);

private:
    explicit PmrRepository();

    enum PmrRequest {
        ExposuresList,
        ExposureInformation,
        WorkspaceInformation,
        ExposureFileInformation,
        WorkspacesList,
        WorkspaceDetails
    };

    enum Action {
        None,
        CloneWorkspace,
        ShowExposureFiles
    };

    QNetworkAccessManager *mNetworkAccessManager;

    PmrOAuthClient *mPmrOAuthClient;

    int mNumberOfExposureFileUrlsLeft;

    QMap<QString, PmrWorkspace *> mWorkspaces;
    QMap<QString, QString> mExposureUrls;
    QMap<QString, QString> mExposureNames;
    QMap<QString, QString> mExposureFileNames;

    QString informationNoteMessage() const;

    void doShowExposureFiles(const QString &pExposureUrl);

    void sendPmrRequest(const PmrRequest &pPmrRequest,
                        const QString &pUrl = QString(),
                        const Action pAction = None,
                        const QString &pName = QString());

Q_SIGNALS:
    void authenticated(const bool &pAuthenticated);
    //void authenticationChanged(const bool &pAuthenticated);

    void busy(const bool &pBusy);

    void warning(const QString &pMessage);
    void information(const QString &pMessage);

    void exposuresList(const PMRSupport::PmrExposures &pExposures,
                       const QString &pErrorMessage,
                       const bool &pInternetConnectionAvailable);

    void addExposureFiles(const QString &pUrl,
                          const QStringList &pExposureFiles);
    void showExposureFiles(const QString &pUrl);

    void workspacesList(const PMRSupport::PmrWorkspaceList &pWorkspaceList);

public Q_SLOTS:
    void authenticate(const bool &pLink = true);
    void getAuthenticationStatus(void);

private Q_SLOTS:
    void authenticationFailed();
    void authenticationSucceeded();
    void openBrowser(const QUrl &pUrl);

    void finished(QNetworkReply *pNetworkReply = 0);
    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);
};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

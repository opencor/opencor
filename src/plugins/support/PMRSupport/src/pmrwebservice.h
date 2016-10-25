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
// PMR web service
//==============================================================================

#pragma once

//==============================================================================

#include "pmrauthentication.h"
#include "pmrexposure.h"
#include "pmrsupportglobal.h"
#include "pmrworkspace.h"

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrWebServiceManager;

//==============================================================================

static const auto PmrUrl = QStringLiteral("http://staging.physiomeproject.org");
//---OPENCOR--- THE ABOVE SHOULD EVENTUALLY BE REPLACED WITH THE BELOW...
//static const auto PmrUrl = QStringLiteral("https://models.physiomeproject.org");

//==============================================================================

static const auto RequestMimeType    = QStringLiteral("application/vnd.physiome.pmr2.json.0");
static const auto CollectionMimeType = QStringLiteral("application/vnd.physiome.pmr2.json.1");

//==============================================================================

class PMRSUPPORT_EXPORT PmrWebService : public QObject
{
    Q_OBJECT

public:
    explicit PmrWebService(QObject *pParent = 0);

    void requestExposures();
    void requestExposureFiles(const QString &pUrl);
    void requestExposureWorkspaceClone(const QString &pExposureUrl);

    PmrWorkspace * getWorkspace(const QString &pUrl);

    void requestWorkspaceClone(PmrWorkspace *pWorkspace,
                               const QString &pDirName);
    void requestWorkspaceInformation(const QString &pUrl);
    void requestWorkspaces();
    void requestWorkspaceSynchronise(PmrWorkspace *pWorkspace,
                                     const bool pOnlyPull);

    void requestNewWorkspace(const QString &pName, const QString &pDescription,
                             const QString &pDirName);

private:
    enum Action {
        None,
        CloneExposureWorkspace,
        RequestExposureFiles
    };

    PmrWebServiceManager *mPmrWebServiceManager;

    QMap<QString, PmrExposure *> mUrlExposures;

    void emitInformation(const QString &pMessage);
    QString informationNoteMessage() const;

    void getWorkspaceCredentials(PmrWorkspace *pWorkspace);
    void requestExposureFileInformation(PmrExposure *pExposure,
                                        const QString &pUrl);
    void requestExposureInformation(PmrExposure *pExposure,
                                    const Action &pNextAction);

    void requestWorkspaceInformation(const QString &pUrl,
                                     const QString &pDirName,
                                     PmrExposure *pExposure = 0);

signals:
    void authenticated(const bool &pAuthenticated);

    void busy(const bool &pBusy);
    void progress(const double &pProgress);

    void error(const QString &pErrorMessage,
               const bool &pInternetConnectionAvailable);
    void information(const QString &pMessage);
    void warning(const QString &pMessage);

    void exposureFiles(const QString &pUrl, const QStringList &pExposureFiles);
    void exposures(const PMRSupport::PmrExposures &pExposures);

    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceCreated(const QString &pUrl);
    void workspaces(const PMRSupport::PmrWorkspaces &pWorkspaces);
    void workspaceSynchronised(PMRSupport::PmrWorkspace *pWorkspace);

public slots:
    void authenticate(const bool &pLink = true);
    void getAuthenticationStatus();
    void unauthorised(const QString &pUrl);

private slots:
    void exposureFileInformationResponse(const QJsonDocument &pJsonDocument);
    void exposureInformationResponse(const QJsonDocument &pJsonDocument);
    void exposuresResponse(const QJsonDocument &pJsonDocument);

    void getWorkspaceResponse(const QJsonDocument &pJsonDocument);
    void workspaceUnauthorised(const QString &pUrl);

    void workspaceInformationResponse(const QJsonDocument &pJsonDocument);
    void workspacesResponse(const QJsonDocument &pJsonDocument);

    void workspaceCloneFinished(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceCreatedResponse(const QString &pUrl);
    void workspaceCredentialsResponse(const QJsonDocument &pJsonDocument);
    void workspaceSynchroniseFinished(PMRSupport::PmrWorkspace *pWorkspace);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

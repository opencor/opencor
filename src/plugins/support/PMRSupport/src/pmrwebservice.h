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

#include "pmrexposure.h"
#include "pmrsupportglobal.h"
#include "pmrworkspace.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrWebServiceManager;

//==============================================================================

static const auto RequestMimeType    = QStringLiteral("application/vnd.physiome.pmr2.json.0");
static const auto CollectionMimeType = QStringLiteral("application/vnd.physiome.pmr2.json.1");

//==============================================================================

class PMRSUPPORT_EXPORT PmrWebService : public QObject
{
    Q_OBJECT

public:
    explicit PmrWebService(const QString &pPmrUrl, QObject *pParent);
    explicit PmrWebService(QObject *pParent);

    bool isAuthenticated() const;
    void authenticate(const bool &pAuthenticate = true);

    void requestExposures() const;

    PmrWorkspace * workspace(const QString &pUrl) const;

    void requestNewWorkspace(const QString &pName, const QString &pDescription,
                             const QString &pPath) const;
    void requestWorkspaces() const;

    void requestWorkspaceClone(PmrWorkspace *pWorkspace,
                               const QString &pPath);
    void requestWorkspaceSynchronize(PmrWorkspace *pWorkspace,
                                     const bool &pPush);

    void update(const QString &pPmrUrl);

    QString siteName() const;

    static QString getEmptyDirectory();
    static QString getNonGitDirectory();

    static bool isGitDirectory(const QString &pDirName);

private:
    enum Action {
        None,
        CloneExposureWorkspace,
        RequestExposureFiles
    };

    QString mPmrUrl;

    PmrWebServiceManager *mPmrWebServiceManager;

    QMap<QString, PmrExposure *> mUrlExposures;
    QMap<PmrExposure *, int> mFileExposuresLeftCount;

    void constructor(const QString &pPmrUrl);

    void requestWorkspaceInformation(const QString &pUrl,
                                     const QString &pPath,
                                     PmrExposure *pExposure = 0);
    void requestWorkspaceCredentials(PmrWorkspace *pWorkspace);

    void requestExposureFileInformation(const QString &pUrl,
                                        PmrExposure *pExposure);

    void emitInformation(const QString &pMessage);

signals:
    void busy(const bool &pBusy);
    void authenticated(const bool &pAuthenticated);

    void information(const QString &pMessage);
    void warning(const QString &pMessage);
    void error(const QString &pMessage);

    void authenticationCancelled();

    void workspaces(const PMRSupport::PmrWorkspaces &pWorkspaces);

    void workspaceCreated(const QString &pUrl);
    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace);

    void exposures(const PMRSupport::PmrExposures &pExposures);
    void exposureFiles(const QString &pUrl, const QStringList &pExposureFiles);

public slots:
    void forbidden(const QString &pUrl);

    void requestExposureFiles(const QString &pUrl);
    void requestExposureWorkspaceClone(const QString &pUrl);

private slots:
    void exposuresResponse(const QJsonDocument &pJsonDocument);

    void workspaceResponse(const QJsonDocument &pJsonDocument);

    void newWorkspaceResponse(const QString &pUrl);
    void workspacesResponse(const QJsonDocument &pJsonDocument);

    void workspaceInformationResponse(const QJsonDocument &pJsonDocument);

    void workspaceErrored();
    void workspaceCloneFinished(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceSynchronizeFinished(PMRSupport::PmrWorkspace *pWorkspace);

    void workspaceCredentialsResponse(const QJsonDocument &pJsonDocument);

    void exposureInformationResponse(const QJsonDocument &pJsonDocument);
    void exposureFileInformationResponse(const QJsonDocument &pJsonDocument);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

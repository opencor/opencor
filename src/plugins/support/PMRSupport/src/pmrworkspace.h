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
// PMR workspace
//==============================================================================

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"
#include "pmrworkspacefilenode.h"

//==============================================================================

#include <QMap>
#include <QObject>

//==============================================================================

#include "libgit2begin.h"
    #include "git2/checkout.h"
    #include "git2/status.h"
#include "libgit2end.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrWebService;

//==============================================================================

using StagedFile = QPair<QString, git_status_t>;
using StagedFiles = QList<StagedFile>;

//==============================================================================

class PMRSUPPORT_EXPORT PmrWorkspace : public QObject
{
    Q_OBJECT

public:
    explicit PmrWorkspace(bool pOwned, const QString &pName,
                          const QString &pUrl, const QString &pDescription,
                          const QString &pOwner, PmrWebService *pParent);
    explicit PmrWorkspace(bool pOwned, const QString &pName,
                          const QString &pUrl, PmrWebService *pParent);
    ~PmrWorkspace() override;

    static bool compare(PmrWorkspace *pWorkspace1, PmrWorkspace *pWorkspace2);

    bool isLocal() const;
    bool isOwned() const;

    QString path() const;

    void setCredentials(const QString &pUsername, const QString &pPassword);

    QString name() const;
    QString url() const;
    QString description() const;
    QString owner() const;

    PmrWorkspaceFileNode * rootFileNode() const;

    void clone(const QString &pPath);
    void close();
    bool commit(const QString &pMessage);
    bool commitMerge();
    bool isMerging() const;
    bool isOpen() const;
    bool open(const QString &pPath, bool pRefreshStatus = true);
    void synchronize(bool pPush);

    QByteArray headFileContents(const QString &pFileName);

    enum WorkspaceStatus {
        StatusUnknown  = 0,
        StatusAhead    = 1,
        StatusBehind   = 2,
        StatusCurrent  = 4,
        StatusConflict = 32,
        StatusStaged   = 64,
        StatusUnstaged = 128
    };

    CharPair gitFileStatus(const QString &pPath) const;
    WorkspaceStatus gitWorkspaceStatus() const;

    void stageFile(const QString &pPath, bool pStage);
    StagedFiles stagedFiles();

private:
    bool mOwned;
    QString mPath;
    QString mUsername;
    QString mPassword;
    QString mName;
    QString mUrl;
    QString mDescription;
    QString mOwner;

    git_repository *mGitRepository;

    PmrWorkspaceFileNode *mRootFileNode;
    QMap<QString, PmrWorkspaceFileNode *> mRepositoryStatusMap;

    QStringList mConflictedFiles;
    QStringList mUpdatedFiles;

    int mStagedCount;
    int mUnstagedCount;

    bool commit(const char *pMessage, const size_t &pParentCount,
                const git_commit **pParents);

    CharPair gitStatusChars(uint pFlags) const;

    void setGitAuthorization(git_strarray *pAuthorizationStrArray);

    static int certificateCheckCallback(git_cert *pCertificate, int pValid,
                                        const char *pHost, void *pPayload);
    static int checkoutNotifyCallback(git_checkout_notify_t pNotification,
                                      const char *pPath,
                                      const git_diff_file *pBaseline,
                                      const git_diff_file *pTarget,
                                      const git_diff_file *pWorkDirectory,
                                      void *pPayload);
    static int fetchheadForeachCallback(const char *pReferenceName,
                                        const char *pRemoteUrl,
                                        const git_oid *pId, uint pMerge,
                                        void *pPayload);
    static int mergeheadForeachCallback(const git_oid *pOid, void *pPayload);

    bool fetch();
    bool merge();
    void push();

    PmrWorkspaceFileNode *parentFileNode(const QString &pPath,
                                         PmrWorkspaceFileNode *pParentFileNode = nullptr);

    void deleteFileNodes(PmrWorkspaceFileNode *pFileNode,
                         PmrWorkspaceFileNodes &pFileNodes);

    void emitGitError(const QString &pMessage) const;

signals:
    void information(const QString &pMessage) const;
    void warning(const QString &pMessage) const;

    void workspaceCloned(PmrWorkspace *pWorkspace);
    void workspaceUncloned(PmrWorkspace *pWorkspace);
    void workspaceSynchronized(PmrWorkspace *pWorkspace);

public slots:
    void refreshStatus();
};

//==============================================================================

using PmrWorkspaces = QList<PmrWorkspace *>;

//==============================================================================

} // namespace PMRSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

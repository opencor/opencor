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
// PMR workspace
//==============================================================================

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"
#include "pmrworkspacefilenode.h"

//==============================================================================

#include <QList>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>

//==============================================================================

#include "git2.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrWebService;

//==============================================================================

class PMRSUPPORT_EXPORT PmrWorkspace : public QObject
{
    Q_OBJECT

public:
    explicit PmrWorkspace(const QString &pUrl, const QString &pName,
                          const QString &pDescription, const QString &pOwner,
                          PmrWebService *pParent);
    explicit PmrWorkspace(const QString &pUrl, const QString &pName,
                          PmrWebService *pParent);
    ~PmrWorkspace();

    static QString getEmptyWorkspaceDirectory();

    static QString getUrlFromFile(const QString &pPath);
    static QString getUrlFromFolder(const QString &pFolder);

    static QString WorkspacesDirectory();

    bool isLocal() const;

    void setCredentials(const QString &pUsername, const QString &pPassword);
    void setPath(const QString &pPath);

    bool isOwned() const;
    void setOwned(const bool &pOwned);

    QString description() const;
    QString name() const;
    QString owner() const;
    QString path() const;
    QString url() const;

    PmrWorkspaceFileNode * rootFileNode() const;

    void clone(const QString &pDirName);
    void close();
    bool commit(const QString &pMessage);
    bool commitMerge();
    bool isMerging() const;
    bool isOpen() const;
    bool open();
    void refreshStatus();
    void synchronise(const bool pOnlyPull);

    enum WorkspaceStatus {
        StatusUnknown  = 0,
        StatusAhead    = 1,
        StatusBehind   = 2,
        StatusCurrent  = 4,
        StatusConflict = 32,
        StatusCommit   = 64,
        StatusUnstaged = 128
    };
    WorkspaceStatus gitWorkspaceStatus() const;
    const CharPair gitFileStatus(const QString &pPath) const;

    void stageFile(const QString &pPath, const bool &pStage);
    QStringList stagedFiles();

private:
    bool mOwned;

    QString mDescription;
    QString mName;
    QString mOwner;
    QString mUrl;

    QString mPassword;
    QString mUsername;

    git_repository *mGitRepository;
    QString mPath;

    QMap<QString, PmrWorkspaceFileNode *> mRepositoryStatusMap;
    PmrWorkspaceFileNode *mRootFileNode;

    QStringList mConflictedFiles;
    QStringList mUpdatedFiles;

    int mStagedCount;
    int mUnstagedCount;

    void constructor(const QString &pUrl, const QString &pName,
                     const QString &pDescription, const QString &pOwner,
                     PmrWebService *pParent);

    void setGitAuthorisation(git_strarray *pAuthorisationStrArray);

    static int certificate_check_cb(git_cert *cert, int valid, const char *host, void *payload);
    static int checkout_notify_cb(git_checkout_notify_t why, const char *path, const git_diff_file *baseline,
                                  const git_diff_file *target, const git_diff_file *workdir, void *payload);
    static void checkout_progress_cb(const char *path, size_t completed_steps, size_t total_steps,
                                     void *payload);
    static int fetchhead_foreach_cb(const char *ref_name, const char *remote_url, const git_oid *oid,
                                    unsigned int is_merge, void *payload);
    static int mergehead_foreach_cb(const git_oid *oid, void *payload);
    static int transfer_progress_cb(const git_transfer_progress *stats, void *payload);

    void emitGitError(const QString &pMessage) const;
    void emitProgress(const double &pProgress) const;

    static const CharPair gitStatusChars(const int &pFlags);

    bool doCommit(const char *pMessage, size_t pParentCount, const git_commit **pParents);
    bool fetch();
    bool merge();
    void push();

signals:
    void progress(const double &pProgress) const;
    void information(const QString &pMessage) const;
    void warning(const QString &pMessage) const;

    void workspaceCloned(PmrWorkspace *pWorkspace);
    void workspaceSynchronised(PmrWorkspace *pWorkspace);
};

//==============================================================================

class PMRSUPPORT_EXPORT PmrWorkspaces : public QList<PmrWorkspace *>
{
public:
    void sort();
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

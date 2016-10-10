/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR workspace
//==============================================================================

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"

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

class PmrRepository;
class PmrWorkspaceFileNode;

//==============================================================================

class PMRSUPPORT_EXPORT PmrWorkspace : public QObject
{
    Q_OBJECT

public:
    explicit PmrWorkspace(PmrRepository *parent=0);
    PmrWorkspace(const QString &pUrl, const QString &pName,
                           const QString &pDescription, const QString &pOwner,
     PmrRepository *parent);
    virtual ~PmrWorkspace();

    static bool compare(const PmrWorkspace *pFirst, const PmrWorkspace *pSecond);

    static QString getEmptyWorkspaceDirectory(void);

    static QString getUrlFromFile(const QString &pPath);
    static QString getUrlFromFolder(const QString &pFolder);

    static QString WorkspacesDirectory(void);

    bool isLocal(void) const;
    bool isNull(void) const;

    void setCredentials(const QString &pUsername, const QString &pPassword);
    void setPath(const QString &pPath);

    bool isOwned(void) const;
    void setOwned(const bool &pOwned);

    const QString &description(void) const;
    const QString &name(void) const;
    const QString &owner(void) const;
    const QString &path(void) const;
    const QString &url(void) const;

    const PmrWorkspaceFileNode *rootFileNode(void) const;

    void clone(const QString &pDirName);
    void close(void);
    bool commit(const QString &pMessage);
    bool open(void);
    bool opened(void) const;
    void refreshStatus(void);
    bool commitMerge();
    bool isMerging() const;
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
    const QPair<QChar, QChar> gitFileStatus(const QString &pPath) const;

    void stageFile(const QString &pPath, const bool &pStage);
    QStringList stagedFilesList(void);

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

//    void setDescription(const QString &pDescription);
//    void setName(const QString &pName);
//    void setOwner(const QString &pOwner);

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

    static const QPair<QChar, QChar> gitStatusChars(const int &pFlags);

    bool doCommit(const char *pMessage, size_t pParentCount, const git_commit **pParents);
    bool fetch(void);
    bool merge(void);
    void push(void);

signals:
    void information(const QString &pMessage) const;
    void progress(const double &pProgress) const;
    void warning(const QString &pMessage) const;

    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceSynchronised(PMRSupport::PmrWorkspace *pWorkspace);
};

//==============================================================================
//==============================================================================

#ifdef _MSC_VER
template class PMRSUPPORT_EXPORT QSet<PmrWorkspace *>;
template class PMRSUPPORT_EXPORT QVector<PmrWorkspace *>;
#endif

class PMRSUPPORT_EXPORT PmrWorkspaceList : public QList<PmrWorkspace *>
{
public:
    PmrWorkspaceList();

    void add(const QString &pUrl, const QString &pName, PmrRepository *parent);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

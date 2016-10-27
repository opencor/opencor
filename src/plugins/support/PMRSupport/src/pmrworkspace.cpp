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

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrwebservice.h"
#include "pmrworkspace.h"
#include "pmrworkspacefilenode.h"
#include "pmrworkspacesmanager.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QDir>
#include <QStandardPaths>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

void PmrWorkspace::constructor(const QString &pUrl, const QString &pName,
                               const QString &pDescription,
                               const QString &pOwner, PmrWebService *pParent)
{
    // Customise ourselves

    mOwned = false;
    mDescription = pDescription;
    mName = pName;
    mOwner = pOwner;
    mUrl = pUrl;
    mPassword = QString();
    mUsername = QString();
    mGitRepository = 0;
    mPath = QString();
    mRepositoryStatusMap = QMap<QString, PmrWorkspaceFileNode *>();
    mRootFileNode = 0;

    // Our messages are directly emitted by our parent PMR web service

    connect(this, SIGNAL(progress(const double &)),
            pParent, SIGNAL(progress(const double &)));
    connect(this, SIGNAL(information(const QString &)),
            pParent, SIGNAL(information(const QString &)));
    connect(this, SIGNAL(warning(const QString &)),
            pParent, SIGNAL(warning(const QString &)));
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const QString &pUrl, const QString &pName,
                           const QString &pDescription, const QString &pOwner,
                           PmrWebService *pParent) :
    QObject(pParent)
{
    // Construct our PMR workspace

    constructor(pUrl, pName, pDescription, pOwner, pParent);
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const QString &pUrl, const QString &pName,
                           PmrWebService *pParent) :
    QObject(pParent)
{
    // Construct our PMR workspace

    constructor(pUrl, pName, QString(), QString(), pParent);
}

//==============================================================================

PmrWorkspace::~PmrWorkspace()
{
    close();
}

//==============================================================================

bool PmrWorkspace::isLocal() const
{
    return !mPath.isEmpty();
}

//==============================================================================

QString PmrWorkspace::WorkspacesDirectory()
{
    static QString workspacesHome =  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                    +QDir::separator()+"OpenCOR"+QDir::separator()+"Workspaces";
    static bool checkedFolder = false;

    if (!checkedFolder) {
        QDir workspacesFolder = QDir(workspacesHome);

        if (!workspacesFolder.exists())
            workspacesFolder.mkpath(".");

        checkedFolder = true;
    }

    return workspacesHome;
}

//==============================================================================

QString PmrWorkspace::getEmptyWorkspaceDirectory()
{
    // Retrieve the name of an empty directory

    return Core::getExistingDirectory(tr("Select Empty Directory"),
                                      PmrWorkspace::WorkspacesDirectory(),
                                      true);
}

//==============================================================================

bool PmrWorkspace::isOwned() const
{
    return mOwned;
}

//==============================================================================

void PmrWorkspace::setOwned(const bool &pOwned)
{
    mOwned = pOwned;
}

//==============================================================================

void PmrWorkspace::setCredentials(const QString &pUsername, const QString &pPassword)
{
    mUsername = pUsername;
    mPassword = pPassword;
}

//==============================================================================

void PmrWorkspace::setPath(const QString &pPath)
{
    mPath = pPath;
}

//==============================================================================

QString PmrWorkspace::description() const
{
    return mDescription;
}

//==============================================================================

QString PmrWorkspace::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

QString PmrWorkspace::owner() const
{
    return mOwner;
}

//==============================================================================

QString PmrWorkspace::path() const
{
    return mPath;
}

//==============================================================================

QString PmrWorkspace::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

PmrWorkspaceFileNode * PmrWorkspace::rootFileNode() const
{
    return mRootFileNode;
}

//==============================================================================

void PmrWorkspace::emitProgress(const double &pProgress) const
{
    emit progress(pProgress);
}

//==============================================================================

void PmrWorkspace::emitGitError(const QString &pMessage) const
{
    emit warning(pMessage);
}

//==============================================================================

bool PmrWorkspace::open()
{
    close();

    if (!mPath.isEmpty()) {
        QByteArray pathByteArray = mPath.toUtf8();

        if (!git_repository_open(&mGitRepository, pathByteArray.constData())) {
            refreshStatus();

            return true;
        }

        emitGitError(tr("An error occurred while trying to open the workspace."));
    }

    return false;
}

//==============================================================================

bool PmrWorkspace::isOpen() const
{
    return mGitRepository;
}

//==============================================================================

void PmrWorkspace::close()
{
    if (mGitRepository) {
        git_repository_free(mGitRepository);
        mGitRepository = 0;
    }
}

//==============================================================================

void PmrWorkspace::refreshStatus()
{
    mStagedCount = 0;
    mUnstagedCount = 0;
    mRepositoryStatusMap.clear();

    mRootFileNode = new PmrWorkspaceFileNode(QString(), mPath);

    if (isOpen()) {

        git_status_options statusOptions;
        git_status_init_options(&statusOptions, GIT_STATUS_OPTIONS_VERSION);
        statusOptions.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
        statusOptions.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED
                            | GIT_STATUS_OPT_INCLUDE_UNMODIFIED
                            | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX
                            | GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY
                            | GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;

        git_status_list *statusList;

        if (!git_status_list_new(&statusList, mGitRepository, &statusOptions)) {
            size_t entries = git_status_list_entrycount(statusList);
            PmrWorkspaceFileNodes fileNodes = PmrWorkspaceFileNodes();
            PmrWorkspaceFileNode *currentFileNode = mRootFileNode;

            fileNodes << currentFileNode;

            for (size_t i = 0; i < entries; ++i) {
                const git_status_entry *status = git_status_byindex(statusList, i);
                const char *filePath = (status->head_to_index)    ? status->head_to_index->old_file.path
                                     : (status->index_to_workdir) ? status->index_to_workdir->old_file.path
                                     :                              0;
                if (filePath) {
                    QStringList pathComponents = QString(filePath).split('/');
                    const CharPair statusChars = gitStatusChars(status->status);

                    if (statusChars.first != ' ')
                        ++mStagedCount;

                    if (statusChars.second != ' ')
                        ++mUnstagedCount;

                    // Find correct place in tree to add file

                    int i = 0;
                    int n = std::min(pathComponents.size(), fileNodes.size()) - 1;

                    while (   (i < n )
                           && pathComponents[i].compare(fileNodes[i+1]->shortName(), Qt::CaseInsensitive)) {
                        ++i;
                    }
                    // Cut back stack to matching path component

                    while ((i + 1) < fileNodes.size())
                        fileNodes.removeLast();

                    currentFileNode = fileNodes[i];

                    // Add directory nodes as required
                    while (i < (pathComponents.size() - 1)) {
                        currentFileNode = currentFileNode->addChild(pathComponents[i]);

                        fileNodes << currentFileNode;

                        ++i;
                    }

                    mRepositoryStatusMap.insert(QString(filePath),
                                                currentFileNode->addChild(pathComponents[i], statusChars));
                }
            }
            git_status_list_free(statusList);
        }
    }
}

//==============================================================================

QStringList PmrWorkspace::stagedFiles()
{
    QStringList fileList = QStringList();
    git_index *index;

    if (!git_repository_index(&index, mGitRepository)) {
        git_status_options statusOptions;

        git_status_init_options(&statusOptions, GIT_STATUS_OPTIONS_VERSION);

        statusOptions.show = GIT_STATUS_SHOW_INDEX_ONLY;
        statusOptions.flags =  GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX
                              |GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY;

        git_status_list *statusList;

        if (!git_status_list_new(&statusList, mGitRepository, &statusOptions)) {
            size_t entries = git_status_list_entrycount(statusList);

            for (size_t i = 0; i < entries; ++i) {
                const git_status_entry *status = git_status_byindex(statusList, i);
                const char *filePath = status->head_to_index?
                                           status->head_to_index->old_file.path:
                                           status->index_to_workdir?
                                               status->index_to_workdir->old_file.path:
                                               0;
                if (filePath) {
                    git_status_t flags = status->status;
                    QString description = QString();

                    if (flags & GIT_STATUS_INDEX_TYPECHANGE)
                        description = "typechange";
                    else if (flags & GIT_STATUS_INDEX_RENAMED)
                        description = "renamed";
                    else if (flags & GIT_STATUS_INDEX_DELETED)
                        description = "deleted";
                    else if (flags & GIT_STATUS_INDEX_MODIFIED)
                        description = "modified";
                    else if (flags & GIT_STATUS_INDEX_NEW)
                        description = "new file";

                    fileList << description+":\t"+filePath;
                }
            }

            git_status_list_free(statusList);
        }

        git_index_free(index);
    }

    return fileList;
}

//==============================================================================

void PmrWorkspace::setGitAuthorisation(git_strarray *pAuthorisationStrArray)
{
    if (!mUsername.isEmpty()) {
        // Initialise a git_strarray with a Basic Authorization header

        QByteArray authorisationHeader = QByteArray("Authorization: Basic ");
        QByteArray usernamePassword = QString(mUsername+":"+mPassword).toUtf8();

        authorisationHeader += usernamePassword.toBase64();

        char *authorisationStrArrayData = (char *)malloc(authorisationHeader.count() + 1);
        memcpy(authorisationStrArrayData, authorisationHeader.constData(), authorisationHeader.count() + 1);

        char **authorisationStrArrayArray = (char **)malloc(sizeof(char *));
        authorisationStrArrayArray[0] = authorisationStrArrayData;

        pAuthorisationStrArray->strings = authorisationStrArrayArray;
        pAuthorisationStrArray->count = 1;
    }
}

//==============================================================================

QString PmrWorkspace::getUrlFromFolder(const QString &pFolder)
{
    // Return remote.origin.url if the folder contains a git repository
    // otherwise return an empty string.

    QString url = QString();

    git_repository *gitRepository = 0;
    QByteArray folderByteArray = pFolder.toUtf8();

    if (!git_repository_open(&gitRepository, folderByteArray.constData())) {
        git_strarray remotes;

        if (!git_remote_list(&remotes, gitRepository)) {
            for (int i = 0; i < (int)remotes.count; i++) {
                char *name = remotes.strings[i];

                if (!strcmp(name, "origin")) {
                    git_remote *remote = 0;

                    if (!git_remote_lookup(&remote, gitRepository, name)) {
                        const char *remoteUrl = git_remote_url(remote);

                        if (remoteUrl)
                            url = QString(remoteUrl);
                    }
                }
            }
        }

        git_repository_free(gitRepository);
    }

    return url;
}

//==============================================================================

int PmrWorkspace::certificateCheckCallback(git_cert *pCertificate, int pValid,
                                           const char *pHost, void *pPayload)
{
    Q_UNUSED(pCertificate);
    Q_UNUSED(pValid);
    Q_UNUSED(pHost);
    Q_UNUSED(pPayload);

    // Bypass the certificate check

    return 1;
}

//==============================================================================

int PmrWorkspace::transferProgressCallback(const git_transfer_progress *pProgress,
                                           void *pPayload)
{
    PmrWorkspace *workspace = (PmrWorkspace *) pPayload;

    workspace->emitProgress(double(pProgress->received_objects+pProgress->indexed_objects)/(pProgress->total_objects << 1));

    return 0;
}

//==============================================================================

void PmrWorkspace::checkoutProgressCallback(const char *pPath,
                                            size_t pCompletedSteps,
                                            size_t pTotalSteps,
                                            void *pPayload)
{
    Q_UNUSED(pPath);

    PmrWorkspace *workspace = (PmrWorkspace *) pPayload;

    workspace->emitProgress(double(pCompletedSteps)/pTotalSteps);
}

//==============================================================================

void PmrWorkspace::clone(const QString &pDirName)
{
    // Clone a workspace

    QByteArray workspaceByteArray = mUrl.toUtf8();
    QByteArray dirNameByteArray = pDirName.toUtf8();

    git_clone_options cloneOptions;
    git_clone_init_options(&cloneOptions, GIT_CLONE_OPTIONS_VERSION);

    // We trust PMR's SSL certificate

    cloneOptions.fetch_opts.callbacks.certificate_check = certificateCheckCallback;

    // Track clone and checkout progress

    cloneOptions.fetch_opts.callbacks.transfer_progress = transferProgressCallback;
    cloneOptions.fetch_opts.callbacks.payload = (void *)this;
    cloneOptions.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    cloneOptions.checkout_opts.progress_cb = checkoutProgressCallback;
    cloneOptions.checkout_opts.progress_payload = (void *)this;

    // Set up Basic authorization

    git_strarray authorisationStrArray = { 0, 0 };
    setGitAuthorisation(&authorisationStrArray);
    cloneOptions.fetch_opts.custom_headers = authorisationStrArray;

    // Perform the clone

    if (git_clone(&mGitRepository, workspaceByteArray.constData(),
                          dirNameByteArray.constData(), &cloneOptions))
        emitGitError(tr("An error occurred while trying to clone the workspace."));

    git_strarray_free(&authorisationStrArray);

    mPath = pDirName;

    PmrWorkspacesManager::instance()->emitWorkspaceCloned(this);
    emit workspaceCloned(this);
}

//==============================================================================

bool PmrWorkspace::fetch()
{
    // Fetch any updates for a workspace

    if (!isOpen()) return false;

    bool fetched = true;

    git_fetch_options fetchOptions;
    git_fetch_init_options(&fetchOptions, GIT_FETCH_OPTIONS_VERSION);

    // We trust PMR's SSL certificate

    fetchOptions.callbacks.certificate_check = certificateCheckCallback;

    // Track push progress

    fetchOptions.callbacks.transfer_progress = transferProgressCallback;
    fetchOptions.callbacks.payload = (void *)this;

    // Set up Basic authorization

    git_strarray authorisationStrArray = { 0, 0 };
    setGitAuthorisation(&authorisationStrArray);
    fetchOptions.custom_headers = authorisationStrArray;

    git_remote_callbacks remoteCallbacks;
    git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);
    remoteCallbacks.certificate_check = certificateCheckCallback;

    // Get the remote, connect to it, add a refspec, and do the push

    git_remote *gitRemote = 0;

    const char *masterReference = "refs/heads/master";
    git_strarray refSpecsStrArray = { (char **)(&masterReference), 1 };

    if (git_remote_lookup(&gitRemote, mGitRepository, "origin")
     || git_remote_fetch(gitRemote, &refSpecsStrArray, &fetchOptions, 0)) {
        emitGitError(tr("An error occurred while trying to fetch the remote workspace."));
        fetched = false;
    }
    if (gitRemote) git_remote_free(gitRemote);

    git_strarray_free(&authorisationStrArray);

    return fetched;
}

//==============================================================================

int PmrWorkspace::checkoutNotifyCallback(git_checkout_notify_t pNotification,
                                         const char *pPath,
                                         const git_diff_file *pBaseline,
                                         const git_diff_file *pTarget,
                                         const git_diff_file *pWorkDirectory,
                                         void *pPayload)
{
    Q_UNUSED(pBaseline);
    Q_UNUSED(pTarget);
    Q_UNUSED(pWorkDirectory);

    // Keep track of conflicted and updated files

    PmrWorkspace *workspace = (PmrWorkspace *) pPayload;

    if (pNotification == GIT_CHECKOUT_NOTIFY_CONFLICT)
        workspace->mConflictedFiles << pPath;
    else if (pNotification == GIT_CHECKOUT_NOTIFY_UPDATED)
        workspace->mUpdatedFiles << pPath;

    return 0;
}

//==============================================================================

typedef struct {
    size_t size;
    git_repository *repository;
    git_commit **parents;
} MergeHeadCallbackData;

//==============================================================================

int PmrWorkspace::mergeheadForeachCallback(const git_oid *oid, void *payload)
{
    int error = 0;
    MergeHeadCallbackData *mergeCallbackData = (MergeHeadCallbackData *)payload;

    if (mergeCallbackData->parents)
        error = git_commit_lookup(&(mergeCallbackData->parents[mergeCallbackData->size]), mergeCallbackData->repository, oid);

    if (!error)
        ++mergeCallbackData->size;

    return error;
}

//==============================================================================

bool PmrWorkspace::commitMerge()
{
    // Get the number of merge heads so we can allocate an array for parents

    MergeHeadCallbackData mergeCallbackData = { 0, 0, 0 };

    git_repository_mergehead_foreach(mGitRepository, mergeheadForeachCallback, &mergeCallbackData);

    size_t nbOfParents = mergeCallbackData.size+1;
    git_commit **parents = new git_commit*[nbOfParents]();

    // HEAD is always a parent

    bool res = false;
    git_oid parentId;

    if (   !git_reference_name_to_id(&parentId, mGitRepository, "HEAD")
        && !git_commit_lookup(parents, mGitRepository, &parentId)) {
        res = true;

        // Now populate the list of commit parents

        if (nbOfParents > 1) {
            mergeCallbackData.size = 0;
            mergeCallbackData.repository = mGitRepository;
            mergeCallbackData.parents = &(parents[1]);

            if (git_repository_mergehead_foreach(mGitRepository, mergeheadForeachCallback, &mergeCallbackData))
                res = false;
        }

        if (res) {
            std::string message = std::string("Merge branch 'master' of ");

            if (doCommit(message.c_str(), nbOfParents,
                         (const git_commit **) parents)) {
                git_repository_state_cleanup(mGitRepository);
            } else {
                res = false;
            }
        }
    }

    for (size_t n = 0; n < nbOfParents; ++n)
        git_commit_free(parents[n]);

    delete[] parents;

    if (!res)
        emitGitError(tr("An error occurred while trying to commit the merge."));

    return res;
}

//==============================================================================

int PmrWorkspace::fetchheadForeachCallback(const char *pReferenceName,
                                           const char *pRemoteUrl,
                                           const git_oid *pId,
                                           unsigned int pMerge, void *pPayload)
{
    Q_UNUSED(pReferenceName);

    int res = 1;

    PmrWorkspace *workspace = (PmrWorkspace *) pPayload;
    git_repository *repository = workspace->mGitRepository;

    workspace->mConflictedFiles = QStringList();
    workspace->mUpdatedFiles = QStringList();

    if (pMerge) {
        git_annotated_commit *remoteCommitHead = 0;

        if (git_annotated_commit_from_fetchhead(&remoteCommitHead, repository,
                                                "origin/master", pRemoteUrl, pId)) {
            res = 0;
        } else {
            // Initialise common checkout options

            git_checkout_options checkoutOptions;

            git_checkout_init_options(&checkoutOptions, GIT_CHECKOUT_OPTIONS_VERSION);

            checkoutOptions.checkout_strategy =  GIT_CHECKOUT_SAFE
                                                |GIT_CHECKOUT_RECREATE_MISSING
                                                |GIT_CHECKOUT_CONFLICT_STYLE_MERGE;
            checkoutOptions.notify_flags = GIT_CHECKOUT_NOTIFY_ALL;
            checkoutOptions.notify_cb = checkoutNotifyCallback;
            checkoutOptions.notify_payload = (void *) workspace;

            // Find the type of merge we can do

            git_merge_analysis_t analysis = GIT_MERGE_ANALYSIS_NONE;
            git_merge_preference_t preference = GIT_MERGE_PREFERENCE_NONE;

            git_merge_analysis(&analysis, &preference, repository,
                               (const git_annotated_commit**) &remoteCommitHead, 1);

            if (analysis & GIT_MERGE_ANALYSIS_UNBORN) {
                // We can simply set HEAD to the target commit.

                git_reference *newMaster = 0;

                res =    !git_reference_create(&newMaster, repository,
                                               "refs/heads/master",
                                               git_annotated_commit_id(remoteCommitHead),
                                               true, "initial pull")
                      && !git_repository_set_head(repository,
                                                  "refs/heads/master")
                      && !git_checkout_head(repository, &checkoutOptions);

                if (newMaster)
                    git_reference_free(newMaster);
            } else if (analysis & GIT_MERGE_ANALYSIS_FASTFORWARD) {
                // We can check out the newly fetched head without merging

                const git_oid *AnnotatedCommitId = git_annotated_commit_id(remoteCommitHead);
                git_commit *commit = 0;
                git_reference *headReference = 0;
                git_reference *newHeadReference = 0;

                res =    !git_commit_lookup(&commit, repository, AnnotatedCommitId)
                      && !git_checkout_tree(repository, (const git_object *) commit, &checkoutOptions)
                      && !git_reference_lookup(&headReference, repository, "refs/heads/master");

                if (res) {
                    const git_oid *commitId = git_commit_id(commit);
                    const char *msg = "pull: Fast-forward";

                    if (git_reference_type(headReference) == GIT_REF_OID) {
                        res = !git_reference_set_target(&newHeadReference,
                                                            headReference,
                                                            commitId, msg);
                    } else {
                        res = !git_reference_create(&newHeadReference,
                                                    repository,
                                                    "refs/heads/master",
                                                    commitId, false, msg);
                    }
                }

                if (commit)
                    git_commit_free(commit);

                if (headReference)
                    git_reference_free(headReference);

                if (newHeadReference)
                    git_reference_free(newHeadReference);
            } else if (analysis & GIT_MERGE_ANALYSIS_NORMAL) {
                // A "normal" merge, i.e. both HEAD and the given merge input
                // have diverged from their common ancestor, which means that
                // the divergent commits must be merged

                git_merge_options mergeOptions;
                git_merge_init_options(&mergeOptions, GIT_MERGE_OPTIONS_VERSION);

                res = !git_merge(repository,
                                 (const git_annotated_commit**) &remoteCommitHead,
                                 1, &mergeOptions, &checkoutOptions);
            }

            git_annotated_commit_free(remoteCommitHead);
        }
    }

    return !res;
}

//==============================================================================

bool PmrWorkspace::isMerging() const
{
    return     mGitRepository
           && (git_repository_state(mGitRepository) == GIT_REPOSITORY_STATE_MERGE);
}

//==============================================================================

bool PmrWorkspace::merge()
{
    // Merge and commit fetched updates

    if (!isOpen())
        return false;

    bool res = true;
    int error = git_repository_fetchhead_foreach(mGitRepository, fetchheadForeachCallback, this);

    if (!error) {
        // We only need to commit NORMAL merges

        if (git_repository_state(mGitRepository) == GIT_REPOSITORY_STATE_MERGE)
            res = commitMerge();

        // emit information(tr("Merge succeeded..."));

        // List of updated files:
        //    mUpdatedFiles.join("\n")
        // Just emit the list... ???
    } else if (error != GIT_ENOTFOUND) {
        QString errorMessage = tr("An error occurred while trying to merge the workspace.");

        if (mConflictedFiles.size()) {
            errorMessage +=  "\n\n"+tr("The following files have conflicts:")
                            +"\n\t"+mConflictedFiles.join("\n\t");

            emit warning(errorMessage);
        } else {
            emitGitError(errorMessage);
        }

        res = false;
    }

    return res;
}

//==============================================================================

void PmrWorkspace::synchronise(const bool pOnlyPull)
{
    // Synchronise our local workspace with PMR

    if (fetch()
      && merge()
      && !pOnlyPull) {
        // We've successfully fetched and merged
        // so push if we are allowed to.

        push();
    }

    emit workspaceSynchronised(this);
    }

//==============================================================================

void PmrWorkspace::push()
{
    // Push a workspace

    if (!isOpen()) return;

    git_push_options pushOptions;
    git_push_init_options(&pushOptions, GIT_PUSH_OPTIONS_VERSION);

    // We trust PMR's SSL certificate

    pushOptions.callbacks.certificate_check = certificateCheckCallback;

    // Track push progress

    pushOptions.callbacks.transfer_progress = transferProgressCallback;
    pushOptions.callbacks.payload = (void *)this;

    // Set up Basic authorization

    git_strarray authorisationStrArray = { 0, 0 };
    setGitAuthorisation(&authorisationStrArray);
    pushOptions.custom_headers = authorisationStrArray;

    git_remote_callbacks remoteCallbacks;
    git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);
    remoteCallbacks.certificate_check = certificateCheckCallback;

    // Get the remote, connect to it, add a refspec, and do the push

    git_remote *gitRemote = 0;

// TODO: Does first push need to create a master ??
//       it needs to create refs/remotes/origin/master

    const char *masterReference = "refs/heads/master";
    git_strarray refSpecsStrArray = { (char **)(&masterReference), 1 };

    if (git_remote_lookup(&gitRemote, mGitRepository, "origin")
     || git_remote_push(gitRemote, &refSpecsStrArray, &pushOptions)) {
        emitGitError(tr("An error occurred while trying to push the workspace."));
    }
    if (gitRemote) git_remote_free(gitRemote);

    git_strarray_free(&authorisationStrArray);
}

//==============================================================================

bool PmrWorkspace::doCommit(const char *pMessage, size_t pParentCount,
                            const git_commit **pParents)
{
    git_signature *author = 0;
    git_index *index = 0;
    git_tree *tree = 0;
    git_oid commitId;
    git_oid treeId;

// TODO: Get user's name and email... From preferences?? Git configuration settings??
//                                    Should OpenCOR set git's global configuration settings?? NO.
// `git_remote_push()` also obtains a signature to use in the reflog
//  so it would make sense to store the name/email with the local repository's .git configuration...

// `git config --local user.name "name"
// `git config --local user.email "email"

    bool error = git_signature_now(&author, "Test Author", "testing@staging.physiomeproject.org")
              || git_repository_index(&index, mGitRepository)
              || git_index_write_tree(&treeId, index)
              || git_tree_lookup(&tree, mGitRepository, &treeId)
              || git_commit_create(&commitId, mGitRepository, "HEAD", author,
                                   author, 0, pMessage, tree, pParentCount,
                                   pParents);
    if (tree)
        git_tree_free(tree);

    if (index)
        git_index_free(index);

    if (author)
        git_signature_free(author);

    return !error;
}

//==============================================================================

bool PmrWorkspace::commit(const QString &pMessage)
{
    if (!isOpen()) return false;

    // Get an empty buffer to hold the cleaned message
    git_buf message;
    message.ptr = 0;
    git_buf_set(&message, 0, 0);

    // Clean up message and remove `;` comments

    QByteArray messageByteArray = pMessage.toUtf8();

    git_message_prettify(&message, messageByteArray.constData(), true, ';');

    bool committed = true;
    if (message.size > 0) {
        int nParents = -1;
        git_commit *parent = 0;

        if (git_repository_head_unborn(mGitRepository) == 1) {
            // Committing to an empty repository
            // TODO: create a master branch...
            nParents = 0;
        } else {
            // Get HEAD as the commit object to use as the parent of the commit

            git_oid parentId;

            if (   !git_reference_name_to_id(&parentId, mGitRepository, "HEAD")
                && !git_commit_lookup(&parent, mGitRepository, &parentId)) {
                nParents = 1;
            }
        }

        if (nParents >= 0) {
            committed = doCommit(message.ptr, nParents, (const git_commit **)(&parent));

            if (!committed)
                emitGitError(tr("An error occurred while trying to commit to the workspace."));
        }

        if (parent)
            git_commit_free(parent);
    }

    git_buf_free(&message);

    return committed;
}

//==============================================================================

PmrWorkspace::WorkspaceStatus PmrWorkspace::gitWorkspaceStatus() const
{
    // Get the status of the repository

    WorkspaceStatus status = StatusUnknown;

    if (isOpen()) {
        if (git_repository_head_unborn(mGitRepository) == 1) {
            status = StatusCurrent;
        } else {
            bool error = false;

            git_oid masterOid;

            if (!git_reference_name_to_id(&masterOid, mGitRepository,
                                          "refs/heads/master")) {
                git_oid originMasterOid;

                if (!git_reference_name_to_id(&originMasterOid, mGitRepository,
                                              "refs/remotes/origin/master")) {
                    size_t ahead = 0;
                    size_t behind = 0;

                    if (!git_graph_ahead_behind(&ahead, &behind, mGitRepository,
                                                &masterOid, &originMasterOid)) {
                        status = ahead?
                                    StatusAhead:
                                    behind?
                                        StatusBehind:
                                        StatusCurrent;
                    } else {
                        error = true;
                    }
                } else {
                    // Need an initial `push origin master`

                    status = StatusAhead;
                }
            } else {
                error = true;
            }

            if (error)
                emitGitError(tr("An error occurred while trying to get the remote status of %1.").arg(mPath));
        }

        git_index *index;

        if (!git_repository_index(&index, mGitRepository)) {
            if (git_index_has_conflicts(index)) {
                status = WorkspaceStatus(status|StatusConflict);
            } else {
                if (mStagedCount)
                    status = WorkspaceStatus(status|StatusCommit);

                if (mUnstagedCount)
                    status = WorkspaceStatus(status|StatusUnstaged);
            }

            git_index_free(index);
        }
    }

    return status;
}

//==============================================================================

const CharPair PmrWorkspace::gitStatusChars(const int &pFlags)
{
    // Git status

    if (pFlags & GIT_STATUS_CONFLICTED)
        return CharPair(' ', 'C');

    if (pFlags & GIT_STATUS_IGNORED)
        return CharPair(' ', '!');

    // iStatus

    QChar iStatus = ' ';

    if (pFlags & GIT_STATUS_INDEX_TYPECHANGE)
        iStatus = 'T';
    else if (pFlags & GIT_STATUS_INDEX_RENAMED)
        iStatus = 'R';
    else if (pFlags & GIT_STATUS_INDEX_DELETED)
        iStatus = 'D';
    else if (pFlags & GIT_STATUS_INDEX_MODIFIED)
        iStatus = 'M';
    else if (pFlags & GIT_STATUS_INDEX_NEW)
        iStatus = 'A';

    // wStatus

    QChar wStatus = ' ';

    if (pFlags & GIT_STATUS_WT_TYPECHANGE)
        wStatus = 'T';
    else if (pFlags & GIT_STATUS_WT_RENAMED)
        wStatus = 'R';
    else if (pFlags & GIT_STATUS_WT_DELETED)
        wStatus = 'D';
    else if (pFlags & GIT_STATUS_WT_MODIFIED)
        wStatus = 'M';
    else if (pFlags & GIT_STATUS_WT_NEW)
        wStatus = 'A';

    return CharPair(iStatus, wStatus);
}

//==============================================================================

const CharPair PmrWorkspace::gitFileStatus(const QString &pPath) const
{
    CharPair status = CharPair(' ', ' ');

    if (isOpen()) {
        QDir repoDir = QDir(mPath);
        unsigned int statusFlags = 0;
        QByteArray relativePathByteArray = repoDir.relativeFilePath(pPath).toUtf8();

        if (!git_status_file(&statusFlags, mGitRepository, relativePathByteArray.constData())) {
            status = gitStatusChars(statusFlags);

            // Also update status in file tree

            if (mRepositoryStatusMap.contains(pPath))
                mRepositoryStatusMap.value(pPath)->setStatus(status);
        }
        else
            emitGitError(tr("An error occurred while trying to get the status of %1.").arg(pPath));
    }
    return status;
}

//==============================================================================

void PmrWorkspace::stageFile(const QString &pPath, const bool &pStage)
{
    if (isOpen()) {
        QDir repoDir = QDir(mPath);
        QByteArray relativePathByteArray = repoDir.relativeFilePath(pPath).toUtf8();
        const char *relativePath = relativePathByteArray.constData();
        bool success = false;
        git_index *index;

       if (!git_repository_index(&index, mGitRepository)) {
            if (pStage) {
                unsigned int statusFlags = 0;

                git_status_file(&statusFlags, mGitRepository, relativePath);

                if (statusFlags & GIT_STATUS_WT_DELETED)
                    success = !git_index_remove_bypath(index, relativePath);
                else
                    success = !git_index_add_bypath(index, relativePath);
            } else if (git_repository_head_unborn(mGitRepository) == 1) {
                success = !git_index_remove_bypath(index, relativePath);
            } else {
                // We need to add a "reset stage" to the index, which means
                // getting the tree for HEAD and tree_entry for the file

                git_reference *headReference;

                if (!git_repository_head(&headReference, mGitRepository)) {
                    git_tree *headTree;

                    if (!git_reference_peel((git_object **) &headTree, headReference, GIT_OBJ_TREE)) {
                        git_tree_entry *headEntry;

                        if (!git_tree_entry_bypath(&headEntry, headTree, relativePath)) {
                            git_index_entry indexEntry;

                            memset(&indexEntry, '\0', sizeof(git_index_entry));

                            indexEntry.id = *git_tree_entry_id(headEntry);
                            indexEntry.mode = git_tree_entry_filemode(headEntry);
                            indexEntry.path = relativePath;

                            git_index_add(index, &indexEntry);

                            git_tree_entry_free(headEntry);

                            success = true;
                        } else {
                            success = !git_index_remove_bypath(index, relativePath);
                        }

                        git_tree_free(headTree);
                    }

                    git_reference_free(headReference);
                }
            }

            if (success)
                git_index_write(index);

            git_index_free(index);
        }

        if (!success)
            emitGitError(tr("An error occurred while trying to stage %1.").arg(pPath));
    }
}

//==============================================================================

bool sortWorkspaces(const PmrWorkspace *pWorkspace1,
                    const PmrWorkspace *pWorkspace2)
{
    // Return whether the first workspace is lower than the second one (without
    // worrying about casing)

    return pWorkspace1->name().compare(pWorkspace2->name(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

void PmrWorkspaces::sort()
{
    // Sort our workspaces

    std::sort(begin(), end(), sortWorkspaces);
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

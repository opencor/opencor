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

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrrepository.h"
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

PmrWorkspace::PmrWorkspace(PmrRepository *parent) : QObject(parent), mOwned(false),
    mDescription(QString()), mName(QString()), mOwner(QString()), mUrl(QString()),
    mPassword(QString()), mUsername(QString()), mGitRepository(nullptr), mPath(QString()),
    mRepositoryStatusMap(QMap<QString, PmrWorkspaceFileNode *>()), mRootFileNode(nullptr)
{
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const QString &pUrl, const QString &pName,
                           const QString &pDescription, const QString &pOwner,
 PmrRepository *parent) :
    QObject(parent), mOwned(false),
    mDescription(pDescription), mName(pName), mOwner(pOwner), mUrl(pUrl),
    mPassword(QString()), mUsername(QString()), mGitRepository(nullptr), mPath(QString()),
    mRepositoryStatusMap(QMap<QString, PmrWorkspaceFileNode *>()), mRootFileNode(nullptr)
{
    // Description and owner are set when workspace information is received from PMR

    // Our messages are directly emitted by our parent PmrRepository

    connect(this, SIGNAL(information(QString)), parent, SIGNAL(information(QString)));
    connect(this, SIGNAL(progress(double)), parent, SIGNAL(progress(double)));
    connect(this, SIGNAL(warning(QString)), parent, SIGNAL(warning(QString)));
}

//==============================================================================

PmrWorkspace::~PmrWorkspace()
{
    close();
}

//==============================================================================

bool PmrWorkspace::isLocal(void) const
{
    return !mPath.isNull();
  }

//==============================================================================

bool PmrWorkspace::isNull(void) const
{
    return mUrl.isNull();
  }

//==============================================================================

bool PmrWorkspace::compare(const PmrWorkspace *pFirst, const PmrWorkspace *pSecond)
{
    // Return whether the first workspace is lower than the second one (without
    // worrying about casing)

    return pFirst->name().compare(pSecond->name(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

QString PmrWorkspace::WorkspacesDirectory()
{
    static QString workspacesHome = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                  + QDir::separator() + "OpenCOR" + QDir::separator() + "Workspaces";

    static bool checkedFolder = false;
    if (!checkedFolder) {
        auto workspacesFolder = QDir(workspacesHome);
        if (!workspacesFolder.exists()) workspacesFolder.mkpath(".");
        checkedFolder = true;
    }

    return workspacesHome;
}

//==============================================================================

QString PmrWorkspace::getEmptyWorkspaceDirectory(void)
{
    // Retrieve the name of an empty directory

    return Core::getExistingDirectory(tr("Select Empty Directory"),
                                      PMRSupport::PmrWorkspace::WorkspacesDirectory(),
                                      true);
}

//==============================================================================

bool PmrWorkspace::isOwned(void) const
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
/*
void PmrWorkspace::setDescription(const QString &pDescription)
{
    mDescription = pDescription;
}

//==============================================================================

void PmrWorkspace::setName(const QString &pName)
{
    mName = pName;
}

//==============================================================================

void PmrWorkspace::setOwner(const QString &pOwner)
{
    mOwner = pOwner;
}
*/
//==============================================================================

void PmrWorkspace::setPath(const QString &pPath)
{
    mPath = pPath;
}

//==============================================================================

const QString &PmrWorkspace::description() const
{
    return mDescription;
}

//==============================================================================

const QString &PmrWorkspace::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

const QString &PmrWorkspace::owner() const
{
    return mOwner;
}

//==============================================================================

const QString &PmrWorkspace::path() const
{
    return mPath;
}

//==============================================================================

const QString &PmrWorkspace::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

const PmrWorkspaceFileNode *PmrWorkspace::rootFileNode(void) const
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
    const git_error *gitError = giterr_last();

    QString gitErrorMessage = gitError ? (tr("Error %1: %2.").arg(QString::number(gitError->klass),
                                                                  Core::formatMessage(gitError->message)))
                                       : QString();
    if (gitError)
        qDebug() << gitErrorMessage;

    emit warning(pMessage + (gitError ? (QString("\n\n") + gitErrorMessage) : ""));
}

//==============================================================================
//==============================================================================

bool PmrWorkspace::open(void)
{
    close();

    if (!mPath.isEmpty()) {
        if (git_repository_open(&mGitRepository, mPath.toUtf8().constData()) == 0) {
            refreshStatus();
            return true;
        }
        emitGitError(tr("An error occurred while trying to open the workspace."));
    }
    return false;
}

//==============================================================================

bool PmrWorkspace::opened(void) const
{
    return (mGitRepository != nullptr);
}

//==============================================================================

void PmrWorkspace::close(void)
{
    if (mGitRepository != nullptr) {
        git_repository_free(mGitRepository);
        mGitRepository = nullptr;
    }
}

//==============================================================================

void PmrWorkspace::refreshStatus(void)
{
    mStagedCount = 0;
    mUnstagedCount = 0;
    mRepositoryStatusMap.clear();

    mRootFileNode = new PmrWorkspaceFileNode("", mPath);
// TODO: Need to check that we aren't in the middle of a (failed) merge.

    git_index *index;
    if (opened() && git_repository_index(&index, mGitRepository) == 0) {

        git_status_options statusOptions;
        git_status_init_options(&statusOptions, GIT_STATUS_OPTIONS_VERSION);
        statusOptions.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
        statusOptions.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED
                            | GIT_STATUS_OPT_INCLUDE_UNMODIFIED
                            | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX
                            | GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY
                            | GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;

        git_status_list *statusList;
        if (git_status_list_new(&statusList, mGitRepository, &statusOptions) == 0) {

            size_t entries = git_status_list_entrycount(statusList);
            auto lastComponents = QStringList();

            auto fileNodeStack = QList<PmrWorkspaceFileNode *>();
            auto currentFileNode = mRootFileNode;
            fileNodeStack.append(currentFileNode);

            for (size_t i = 0; i < entries; ++i) {
                auto status = git_status_byindex(statusList, i);
                const char *filePath = (status->head_to_index)    ? status->head_to_index->old_file.path
                                     : (status->index_to_workdir) ? status->index_to_workdir->old_file.path
                                     :                              nullptr;
                if (filePath) {
                    auto pathComponents = QString(filePath).split('/'); // This is also valid for Windows
                    auto statusChars = gitStatusChars(status->status);

                    if (statusChars.first != ' ') mStagedCount += 1;
                    if (statusChars.second != ' ') mUnstagedCount += 1;

                    // Find correct place in tree to add file
                    int i = 0;
                    int n = std::min(pathComponents.size(), fileNodeStack.size()) - 1;
                    while (i < n
                        && pathComponents[i].compare(fileNodeStack[i+1]->shortName(), Qt::CaseInsensitive)) {
                        i += 1;
                    }
                    // Cut back stack to matching path component
                    while ((i + 1) < fileNodeStack.size())
                        fileNodeStack.removeLast();
                    currentFileNode = fileNodeStack[i];

                    // Add directory nodes as required
                    while (i < (pathComponents.size() - 1)) {
                        currentFileNode = currentFileNode->addChild(pathComponents[i]);
                        fileNodeStack.append(currentFileNode);
                        i += 1;
                    }

                    mRepositoryStatusMap.insert(QString(filePath),
                                                currentFileNode->addChild(pathComponents[i], statusChars));
                }
            }
            git_status_list_free(statusList);
        }
        git_index_free(index);
    }
}

//==============================================================================

QStringList PmrWorkspace::stagedFilesList(void)
{
    auto fileList = QStringList();

    git_index *index;
    if (git_repository_index(&index, mGitRepository) == 0) {

        git_status_options statusOptions;
        git_status_init_options(&statusOptions, GIT_STATUS_OPTIONS_VERSION);
        statusOptions.show  = GIT_STATUS_SHOW_INDEX_ONLY;
        statusOptions.flags = GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX
                            | GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY;

        git_status_list *statusList;
        if (git_status_list_new(&statusList, mGitRepository, &statusOptions) == 0) {
            size_t entries = git_status_list_entrycount(statusList);
            for (size_t i = 0; i < entries; ++i) {
                auto status = git_status_byindex(statusList, i);
                const char *filePath = (status->head_to_index)    ? status->head_to_index->old_file.path
                                     : (status->index_to_workdir) ? status->index_to_workdir->old_file.path
                                     :                              nullptr;
                if (filePath) {
                    auto flags = status->status;
                    auto description = QString();
                    if (flags & GIT_STATUS_INDEX_NEW) description        = "new file";
                    if (flags & GIT_STATUS_INDEX_MODIFIED) description   = "modified";
                    if (flags & GIT_STATUS_INDEX_DELETED) description    = "deleted";
                    if (flags & GIT_STATUS_INDEX_RENAMED) description    = "renamed";
                    if (flags & GIT_STATUS_INDEX_TYPECHANGE) description = "typechange";
                    fileList << description + ":\t" + filePath;
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

        auto authorisationHeader = QByteArray();
        authorisationHeader.append("Authorization: Basic ");
        authorisationHeader.append((mUsername + ":" + mPassword).toUtf8().toBase64());

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

    auto url = QString();

    git_repository *gitRepository = 0;
    if (git_repository_open(&gitRepository, pFolder.toUtf8().constData()) == 0) {

        git_strarray remotes ;
        if (git_remote_list(&remotes, gitRepository) == 0) {

            for (int i = 0; i < (int)remotes.count; i++) {
                char *name = remotes.strings[i];
                if (strcmp(name, "origin")==0) {
                    git_remote *remote = {0};

                    if (git_remote_lookup(&remote, gitRepository, name) == 0) {
                        const char *remoteUrl = git_remote_url(remote);
                        if (remoteUrl) url = QString(remoteUrl);
                    }
                }
            }
        }
        git_repository_free(gitRepository);
    }
    return url;
}

//==============================================================================

// See https://libgit2.github.com/libgit2/#HEAD/type/git_remote_callbacks

// If cert verification fails, this is called to let the user make the final
// decision of whether to allow the connection to proceed. Returns 1 to allow
// the connection, 0 to disallow it or a negative value to indicate an error.

int PmrWorkspace::certificate_check_cb(git_cert *cert, int valid, const char *host, void *payload)
{
    Q_UNUSED(cert)
    Q_UNUSED(valid)
    Q_UNUSED(host)
    Q_UNUSED(payload)

    // pmrRepository.Url().compare(host)
    return 1; // since we trust PMR (but should check host matches PMR...)
              // 0 = disallow, -ve = error
}

//==============================================================================

int PmrWorkspace::transfer_progress_cb(const git_transfer_progress *stats, void *payload)
{
    auto workspace = (PmrWorkspace *)payload;

    workspace->emitProgress((double)(stats->received_objects + stats->indexed_objects)
                            /(2.0*stats->total_objects));
    return 0;
}

//==============================================================================

void PmrWorkspace::checkout_progress_cb(const char *path, size_t completed_steps, size_t total_steps,
                                        void *payload)
{
    Q_UNUSED(path)

    auto workspace = (PmrWorkspace *)payload;

    workspace->emitProgress((double)completed_steps/(double)total_steps);
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

    cloneOptions.fetch_opts.callbacks.certificate_check = certificate_check_cb;

    // Track clone and checkout progress

    cloneOptions.fetch_opts.callbacks.transfer_progress = transfer_progress_cb ;
    cloneOptions.fetch_opts.callbacks.payload = (void *)this;
    cloneOptions.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    cloneOptions.checkout_opts.progress_cb = checkout_progress_cb;
    cloneOptions.checkout_opts.progress_payload = (void *)this;

    // Set up Basic authorization

    git_strarray authorisationStrArray = { nullptr, 0 };
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

bool PmrWorkspace::fetch(void)
{
    // Fetch any updates for a workspace

    if (!this->opened()) return false;

    bool fetched = true;

    git_fetch_options fetchOptions;
    git_fetch_init_options(&fetchOptions, GIT_FETCH_OPTIONS_VERSION);

    // We trust PMR's SSL certificate

    fetchOptions.callbacks.certificate_check = certificate_check_cb;

    // Track push progress

    fetchOptions.callbacks.transfer_progress = transfer_progress_cb ;
    fetchOptions.callbacks.payload = (void *)this;

    // Set up Basic authorization

    git_strarray authorisationStrArray = { nullptr, 0 };
    setGitAuthorisation(&authorisationStrArray);
    fetchOptions.custom_headers = authorisationStrArray;

    git_remote_callbacks remoteCallbacks;
    git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);
    remoteCallbacks.certificate_check = certificate_check_cb;

    // Get the remote, connect to it, add a refspec, and do the push

    git_remote *gitRemote = nullptr;

    const char *masterReference = "refs/heads/master";
    git_strarray refSpecsStrArray = { (char **)(&masterReference), 1 };

    if (git_remote_lookup(&gitRemote, mGitRepository, "origin")
     || git_remote_fetch(gitRemote, &refSpecsStrArray, &fetchOptions, nullptr)) {
        emitGitError(tr("An error occurred while trying to fetch the remote workspace."));
        fetched = false;
    }
    if (gitRemote) git_remote_free(gitRemote);

    git_strarray_free(&authorisationStrArray);

    return fetched;
}

//==============================================================================

int PmrWorkspace::checkout_notify_cb(git_checkout_notify_t why, const char *path, const git_diff_file *baseline,
                                     const git_diff_file *target, const git_diff_file *workdir, void *payload)
{
    Q_UNUSED(baseline)
    Q_UNUSED(target)
    Q_UNUSED(workdir)

    auto workspace = (PmrWorkspace *)payload;

    if (why == GIT_CHECKOUT_NOTIFY_CONFLICT) {
        workspace->mConflictedFiles << QString(path);
// qDebug() << "Conflict: " << path;
    }
    else if (why == GIT_CHECKOUT_NOTIFY_DIRTY) {
        // TODO: We may want to report dirty files...
// qDebug() << "Dirty: " << path;
    }
    else if (why == GIT_CHECKOUT_NOTIFY_UPDATED) {
        workspace->mUpdatedFiles << QString(path);
// qDebug() << "Update: " << path;
    }

    return 0;
}

//==============================================================================

typedef struct {
    size_t size;
    git_repository *repository;
    git_commit **parents;
} MergeHeadCallbackData;

//==============================================================================

int PmrWorkspace::mergehead_foreach_cb(const git_oid *oid, void *payload)
{
    int error = 0;
    MergeHeadCallbackData *mergeCallbackData = (MergeHeadCallbackData *)payload;

    if (mergeCallbackData->parents)
        error = git_commit_lookup(&(mergeCallbackData->parents[mergeCallbackData->size]), mergeCallbackData->repository, oid);

    if (!error) mergeCallbackData->size += 1;

    return error;
}

//==============================================================================

bool PmrWorkspace::commitMerge()
{
    bool result = false;

    // Get the number of merge heads so we can allocate an array for parents
    MergeHeadCallbackData mergeCallbackData = {0, nullptr, nullptr};
    git_repository_mergehead_foreach(mGitRepository, mergehead_foreach_cb, &mergeCallbackData);
    size_t nParents = mergeCallbackData.size + 1;
    auto parents = new git_commit *[nParents]();

    // HEAD is always a parent

    git_oid parentId;
    if (git_reference_name_to_id(&parentId, mGitRepository, "HEAD") == 0
     && git_commit_lookup(parents, mGitRepository, &parentId) == 0) {
        result = true;

        // Now populate the list of commit parents

        if (nParents > 1) {
            mergeCallbackData.size = 0;
            mergeCallbackData.repository = mGitRepository;
            mergeCallbackData.parents = &(parents[1]);
            if (git_repository_mergehead_foreach(mGitRepository, mergehead_foreach_cb, &mergeCallbackData))
                result = false;
        }

        if (result) {
        // TODO: Name comes from fetch head??
        // Also to have in reflog: "pull: Merge made by the 'recursive' strategy." ??
// Use MERGE_MSG ???
            auto message = std::string("Merge branch 'master' of "); // + remote_url;
            if (doCommit(message.c_str(), nParents, (const git_commit **)parents)) {
                git_repository_state_cleanup(mGitRepository);
            }
            else {
                result = false;
        //Error 10: cannot create a tree from a not fully merged index.
            }
        }
    }

    for (size_t n = 0; n < nParents; ++n)
        git_commit_free(parents[n]);
    delete[] parents;

    if (!result)
        emitGitError(tr("An error occurred while trying to commit the merge."));

    return result;
}

//==============================================================================

int PmrWorkspace::fetchhead_foreach_cb(const char *ref_name, const char *remote_url, const git_oid *oid,
                                       unsigned int is_merge, void *payload)
{
    Q_UNUSED(ref_name)

    bool success = true;

    auto workspace = (PmrWorkspace *)payload;
    auto repository = workspace->mGitRepository;

    workspace->mConflictedFiles = QStringList();
    workspace->mUpdatedFiles = QStringList();

    if (is_merge) {
        git_annotated_commit *remoteCommitHead = nullptr;
        if (git_annotated_commit_from_fetchhead(&remoteCommitHead, repository,
                                                "origin/master", remote_url, oid) != 0) {
            success = false;
        }
        else {
            // Initialise common checkout options

            git_checkout_options checkoutOptions;
            git_checkout_init_options(&checkoutOptions, GIT_CHECKOUT_OPTIONS_VERSION);
            checkoutOptions.checkout_strategy = GIT_CHECKOUT_SAFE
                                              | GIT_CHECKOUT_RECREATE_MISSING
                                              | GIT_CHECKOUT_CONFLICT_STYLE_MERGE;  // DIFF3 ??
            checkoutOptions.notify_flags = GIT_CHECKOUT_NOTIFY_ALL;
            checkoutOptions.notify_cb = checkout_notify_cb;
            checkoutOptions.notify_payload = (void *)workspace;

            // Find the type of merge we can do

            git_merge_analysis_t analysis = GIT_MERGE_ANALYSIS_NONE;
            git_merge_preference_t preference = GIT_MERGE_PREFERENCE_NONE;
            git_merge_analysis(&analysis, &preference, repository, (const git_annotated_commit**)(&remoteCommitHead), 1);

            if (analysis & GIT_MERGE_ANALYSIS_UNBORN) {
                // We can simply set HEAD to the target commit.

                git_reference *newMaster = nullptr;
                success = git_reference_create(&newMaster, repository, "refs/heads/master",
                                                  git_annotated_commit_id(remoteCommitHead),
                                                  true, "initial pull") == 0
                          && git_repository_set_head(repository, "refs/heads/master") == 0
                          && git_checkout_head(repository, &checkoutOptions) == 0;

                if (newMaster != nullptr) git_reference_free(newMaster);
            }
            else if (analysis & GIT_MERGE_ANALYSIS_FASTFORWARD) {
                // We can check out the newly fetched head without merging

                const git_oid *AnnotatedCommitId = git_annotated_commit_id(remoteCommitHead);

                git_commit *commit = nullptr;
                git_reference *headReference = nullptr;
                git_reference *newHeadReference = nullptr;

                success = git_commit_lookup(&commit, repository, AnnotatedCommitId) == 0
                       && git_checkout_tree(repository, (const git_object *)commit, &checkoutOptions) == 0
                       && git_reference_lookup(&headReference, repository, "refs/heads/master") == 0;
                if (success) {
                    const git_oid *commitId = git_commit_id(commit);
                    const char *msg = "pull: Fast-forward";

                    if (git_reference_type(headReference) == GIT_REF_OID) {
                        success = git_reference_set_target(&newHeadReference, headReference, commitId, msg) == 0;
                    }
                    else {
                        success = git_reference_create(&newHeadReference, repository,
                                                       "refs/heads/master", commitId, false,  msg) == 0;
                    }
                }

                if (commit != nullptr) git_commit_free(commit);
                if (headReference != nullptr) git_reference_free(headReference);
                if (newHeadReference != nullptr) git_reference_free(newHeadReference);
            }

            else if (analysis & GIT_MERGE_ANALYSIS_NORMAL) {
                // A "normal" merge; both HEAD and the given merge input have diverged from
                // their common ancestor. The divergent commits must be merged.

                git_merge_options mergeOptions;
                git_merge_init_options(&mergeOptions, GIT_MERGE_OPTIONS_VERSION);

// Setting this means we don't update file with conflict lines...
//              mergeOptions.flags = GIT_MERGE_FAIL_ON_CONFLICT;

                success = git_merge(repository, (const git_annotated_commit**)(&remoteCommitHead), 1,
                                    &mergeOptions, &checkoutOptions) == 0;
            }

        git_annotated_commit_free(remoteCommitHead);
        }
    }

    return success ? 0 : 1;
}

//==============================================================================

bool PmrWorkspace::isMerging(void) const
{
    return mGitRepository != nullptr
      && git_repository_state(mGitRepository) == GIT_REPOSITORY_STATE_MERGE;
}

//==============================================================================

bool PmrWorkspace::merge(void)
{
    // Merge and commit fetched updates

    if (!this->opened()) return false;

    bool success = true;

    int error = git_repository_fetchhead_foreach(mGitRepository, fetchhead_foreach_cb, this);
    if (error == 0) {

        // We only need to commit NORMAL merges

        if (git_repository_state(mGitRepository) == GIT_REPOSITORY_STATE_MERGE)
            result = commitMerge();

        // emit information(tr("Merge succeeded..."));

        // List of updated files:
        //    mUpdatedFiles.join("\n")
        // Just emit the list... ???
    }
    else if (error != GIT_ENOTFOUND) {
        auto errorMessage = tr("An error occurred while trying to merge the workspace.");
        if (mConflictedFiles.size()) {
            errorMessage.append("\n\n" + tr("The following files have conflicts:")
                              + "\n\t" + mConflictedFiles.join("\n\t"));
            emit warning(errorMessage);
        }
        else {
            emitGitError(errorMessage);
        }
        success = false;
    }

    return success;
}

//==============================================================================

void PmrWorkspace::synchronise(const bool pOnlyPull)
{
    // Synchronise our local workspace with PMR

    if (this->fetch()
      && this->merge()
      && !pOnlyPull) {
        // We've successfully fetched and merged
        // so push if we are allowed to.

        this->push();
    }

    emit workspaceSynchronised(this);
    }

//==============================================================================

void PmrWorkspace::push(void)
{
    // Push a workspace

    if (!this->opened()) return;

    git_push_options pushOptions;
    git_push_init_options(&pushOptions, GIT_PUSH_OPTIONS_VERSION);

    // We trust PMR's SSL certificate

    pushOptions.callbacks.certificate_check = certificate_check_cb;

    // Track push progress

    pushOptions.callbacks.transfer_progress = transfer_progress_cb ;
    pushOptions.callbacks.payload = (void *)this;

    // Set up Basic authorization

    git_strarray authorisationStrArray = { nullptr, 0 };
    setGitAuthorisation(&authorisationStrArray);
    pushOptions.custom_headers = authorisationStrArray;

    git_remote_callbacks remoteCallbacks;
    git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);
    remoteCallbacks.certificate_check = certificate_check_cb;

    // Get the remote, connect to it, add a refspec, and do the push

    git_remote *gitRemote = nullptr;

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

bool PmrWorkspace::doCommit(const char *pMessage, size_t pParentCount, const git_commit **pParents)
{
    git_signature *author = nullptr;
    git_index *index = nullptr;
    git_tree *tree = nullptr;
    git_oid commitId;
    git_oid treeId;

// TODO: Get user's name and email... From preferences?? Git configuration settings??
//                                    Should OpenCOR set git's global configuration settings?? NO.
// `git_remote_push()` also obtains a signature to use in the reflog
//  so it would make sense to store the name/email with the local repository's .git configuration...

// `git config --local user.name "name"
// `git config --local user.email "email"

    bool error = git_signature_now(&author, "Test Author", "testing@staging.physiomeproject.org") != 0
              || git_repository_index(&index, mGitRepository) != 0
              || git_index_write_tree(&treeId, index) != 0
              || git_tree_lookup(&tree, mGitRepository, &treeId) != 0
              || git_commit_create(&commitId, mGitRepository, "HEAD", author, author,
                                   nullptr, pMessage, tree, pParentCount, pParents) != 0;
    if (tree != nullptr) git_tree_free(tree);
    if (index != nullptr) git_index_free(index);
    if (author != nullptr) git_signature_free(author);

    return !error;
}

//==============================================================================

bool PmrWorkspace::commit(const QString &pMessage)
{
    if (!this->opened()) return false;

    // Get an empty buffer to hold the cleaned message
    git_buf message;
    message.ptr = nullptr;
    git_buf_set(&message, nullptr, 0);

    // Clean up message and remove `;` comments

    git_message_prettify(&message, pMessage.toUtf8().constData(), true, ';');

    bool committed = true;
    if (message.size > 0) {
        int nParents = -1;
        git_commit *parent = nullptr;

        if (git_repository_head_unborn(mGitRepository) == 1) {
            // Committing to an empty repository
            // TODO: create a master branch...
            nParents = 0;
        }
        else {
            // Get HEAD as the commit object to use as the parent of the commit
            git_oid parentId;
            if (git_reference_name_to_id(&parentId, mGitRepository, "HEAD") == 0
             && git_commit_lookup(&parent, mGitRepository, &parentId) == 0) {
                nParents = 1;
            }
        }

        if (nParents >= 0) {
            committed = doCommit(message.ptr, nParents, (const git_commit **)(&parent));

            if (!committed)
                emitGitError(tr("An error occurred while trying to commit to the workspace."));
        }

        if (parent != nullptr) git_commit_free(parent);
    }

    git_buf_free(&message);

    return committed;
}

//==============================================================================

PmrWorkspace::WorkspaceStatus PmrWorkspace::gitWorkspaceStatus(void) const
{
    // Get the status of the repository

    auto status = StatusUnknown;

    if (this->opened()) {

        if (git_repository_head_unborn(mGitRepository) == 1) {
            status = StatusCurrent;
        }
        else {
            bool error=false;

            git_oid masterOid;
            if (git_reference_name_to_id(&masterOid, mGitRepository,
                                         "refs/heads/master") == 0) {
                git_oid originMasterOid;
                if (git_reference_name_to_id(&originMasterOid, mGitRepository,
                                             "refs/remotes/origin/master") == 0) {
                    size_t ahead = 0;
                    size_t behind = 0;
                    if (git_graph_ahead_behind(&ahead, &behind, mGitRepository,
                                               &masterOid, &originMasterOid) == 0) {
                        status = ahead  ? StatusAhead
                               : behind ? StatusBehind
                               :          StatusCurrent;
                    }
                    else error = true;
                }
                else {
                    status = StatusAhead;    // Need an initial `push origin master`
                }
            }
            else error = true;

            if (error)
                emitGitError(tr("An error occurred while trying to get the remote status of %1").arg(mPath));
        }

        git_index *index;
        if (git_repository_index(&index, mGitRepository) == 0) {
            if (git_index_has_conflicts(index)) {
                status = (WorkspaceStatus)(status | StatusConflict);
            }
            else {
                if (mStagedCount > 0) status = (WorkspaceStatus)(status | StatusCommit);
                if (mUnstagedCount > 0) status = (WorkspaceStatus)(status | StatusUnstaged);
            }
            git_index_free(index);
        }
    }

    return status;
}

//==============================================================================

const QPair<QChar, QChar> PmrWorkspace::gitStatusChars(const int &pFlags)
{
    char istatus = ' ';
    char wstatus = ' ';

    if (pFlags & GIT_STATUS_INDEX_NEW) istatus = 'A';
    if (pFlags & GIT_STATUS_INDEX_MODIFIED) istatus = 'M';
    if (pFlags & GIT_STATUS_INDEX_DELETED) istatus = 'D';
    if (pFlags & GIT_STATUS_INDEX_RENAMED) istatus = 'R';
    if (pFlags & GIT_STATUS_INDEX_TYPECHANGE) istatus = 'T';

    if (pFlags & GIT_STATUS_WT_NEW) wstatus = 'A';
    if (pFlags & GIT_STATUS_WT_MODIFIED) wstatus = 'M';
    if (pFlags & GIT_STATUS_WT_DELETED) wstatus = 'D';
    if (pFlags & GIT_STATUS_WT_RENAMED) wstatus = 'R';
    if (pFlags & GIT_STATUS_WT_TYPECHANGE) wstatus = 'T';

    if (pFlags & GIT_STATUS_IGNORED) {
        istatus = ' ';
        wstatus = '!';
    }

    if (pFlags & GIT_STATUS_CONFLICTED) {
        istatus = ' ';
        wstatus = 'C';
    }

    return QPair<QChar, QChar>(istatus, wstatus);
}

//==============================================================================

const QPair<QChar, QChar> PmrWorkspace::gitFileStatus(const QString &pPath) const
{
    auto status = QPair<QChar, QChar>(' ', ' ');

    if (this->opened()) {
        auto repoDir = QDir(mPath);
        auto relativePath = repoDir.relativeFilePath(pPath);

        unsigned int statusFlags = 0;
        if (git_status_file(&statusFlags, mGitRepository, relativePath.toUtf8().constData()) == 0) {
            status = gitStatusChars(statusFlags);
            // Also update status in file tree
            if (mRepositoryStatusMap.contains(pPath))
                mRepositoryStatusMap.value(pPath)->setStatus(status);
        }
        else
            emitGitError(tr("An error occurred while trying to get the status of %1").arg(pPath));
    }
    return status;
}

//==============================================================================

void PmrWorkspace::stageFile(const QString &pPath, const bool &pStage)
{
    if (this->opened()) {
        auto repoDir = QDir(mPath);
        auto relativePath = repoDir.relativeFilePath(pPath).toUtf8();
        bool success = false;

        git_index *index;
        if (git_repository_index(&index, mGitRepository) == 0) {

            if (pStage) {
                unsigned int statusFlags = 0;
                git_status_file(&statusFlags, mGitRepository, relativePath.constData());
                if (statusFlags & GIT_STATUS_WT_DELETED) {
                    success = (git_index_remove_bypath(index, relativePath.constData()) == 0);
                }
                else {
                    success = (git_index_add_bypath(index, relativePath.constData()) == 0);
                }
            }

            else if (git_repository_head_unborn(mGitRepository) == 1) {
                success = (git_index_remove_bypath(index, relativePath.constData()) == 0);
            }

            else {
                // Need to add a "reset stage" to the index
                // Get tree of HEAD, then tree_entry for file
                git_reference *headReference;
                if (git_repository_head(&headReference, mGitRepository) == 0) {
                    git_tree *headTree;
                    if (git_reference_peel((git_object **)&headTree, headReference, GIT_OBJ_TREE) == 0) {
                        git_tree_entry *headEntry;
                        if (git_tree_entry_bypath(&headEntry, headTree, relativePath.constData()) == 0) {
                            git_index_entry indexEntry;
                            memset(&indexEntry, '\0', sizeof(git_index_entry));
                            indexEntry.id = *git_tree_entry_id(headEntry);
                            indexEntry.mode = git_tree_entry_filemode(headEntry);
                            indexEntry.path = relativePath.constData();
                            git_index_add(index, &indexEntry);
                            git_tree_entry_free(headEntry);
                            success = true;
                        }
                        else {
                            success = (git_index_remove_bypath(index, relativePath.constData()) == 0);
                        }
                        git_tree_free(headTree);
                    }
                    git_reference_free(headReference);
                }
            }

            if (success) git_index_write(index);

            git_index_free(index);
        }
        if (!success)
            emitGitError(tr("An error occurred while trying to stage %1").arg(pPath));
    }
}

//==============================================================================
//==============================================================================

PmrWorkspaceList::PmrWorkspaceList() :
    QList<PmrWorkspace *>()
{
}

//==============================================================================

void PmrWorkspaceList::add(const QString &pUrl, const QString &pName, PmrRepository *parent)
{
    // Add a new workspace to the list

    QList::append(new PmrWorkspace(pUrl, pName, "", "", parent));
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

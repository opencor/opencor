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
    mRepositoryStatusMap(QMap<QString, QPair<QChar, QChar> >())

{
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const QString &pUrl, const QString &pName, PmrRepository *parent) :
    QObject(parent), mOwned(false),
    mDescription(QString()), mName(pName), mOwner(QString()), mUrl(pUrl),
    mPassword(QString()), mUsername(QString()), mGitRepository(nullptr), mPath(QString()),
    mRepositoryStatusMap(QMap<QString, QPair<QChar, QChar> >())
{
    // Name, description and owner are set from PMR workspace info
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

void PmrWorkspace::emitProgress(const double &pProgress) const
{
    emit progress(pProgress);
}

//==============================================================================

void PmrWorkspace::emitGitError(const QString &pMessage) const
{
    const git_error *gitError = giterr_last();

    emit warning(pMessage + (gitError ? (QString("<br></br><br></br>")
                                         + tr("Error %1: %2.").arg(QString::number(gitError->klass),
                                                                   Core::formatMessage(gitError->message)))
                                      : ""));
}

//==============================================================================

bool PmrWorkspace::open(void)
{
    close();
    if (!mPath.isEmpty()) {
        if (git_repository_open(&mGitRepository, mPath.toUtf8().constData()) == 0) {

            git_index *index;
            if (git_repository_index(&index, mGitRepository) == 0) {

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
                    for (size_t i = 0; i < entries; ++i) {
                        auto status = git_status_byindex(statusList, i);
                        const char *filePath = (status->head_to_index)    ? status->head_to_index->old_file.path
                                             : (status->index_to_workdir) ? status->index_to_workdir->old_file.path
                                             :                              nullptr;
                        if (filePath) {
                            auto pathComponents = QString(filePath).split('/'); // This is also valid for Windows
                            auto statusChars = gitStatusChars(status->status);

                            if (statusChars.first != ' ') mStagedCount += 1;


                            mRepositoryStatusMap.insert(QString(filePath), statusChars);
                        }
                    }
                    git_status_list_free(statusList);
                }
                git_index_free(index);
            }
            return true;
        }
        emitGitError(tr("An error occurred while trying to open the workspace."));
    }

// TODO Keep a map with the status of all workspace files (and folders).
//      Update it wheh getFileStatus(pPath) is called -- unknown file ==> Add etc

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
    mStagedCount = 0;
    mRepositoryStatusMap.clear();
    if (mGitRepository != nullptr) {
        git_repository_free(mGitRepository);
        mGitRepository = nullptr;
    }
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
                          dirNameByteArray.constData(), &cloneOptions)
     || git_remote_add_push(mGitRepository, "origin", "refs/heads/master:refs/heads/master"))
        emitGitError(tr("An error occurred while trying to clone the workspace."));

    git_strarray_free(&authorisationStrArray);

    mPath = pDirName;

    PmrWorkspacesManager::instance()->emitWorkspaceCloned(this);
    emit workspaceCloned(this);
}

//==============================================================================

void PmrWorkspace::push(void)
{
    // Push a workspace

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

    if (this->open()) {

        git_remote_callbacks remoteCallbacks;
        git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);
        remoteCallbacks.certificate_check = certificate_check_cb;

        // Get the remote, connect to it, add a refspec, and do the push

        git_remote *gitRemote = nullptr;
        git_strarray refSpecsStrArray = { nullptr, 0 };

        if (git_remote_lookup(&gitRemote, mGitRepository, "origin")
         || git_remote_push(gitRemote, &refSpecsStrArray, &pushOptions)) {
            emitGitError(tr("An error occurred while trying to push the workspace."));

        if (gitRemote) git_remote_free(gitRemote);
        }
    }

    git_strarray_free(&authorisationStrArray);

    emit workspacePushed(this);
}

//==============================================================================

bool PmrWorkspace::commit(const QString &pMessage)
{
    bool success = false;

    if (this->open()) {
        // Get an empty buffer to hold the cleaned message

        git_buf msg;
        msg.ptr = nullptr;
        git_buf_set(&msg, nullptr, 0);

        // Clean up message and remove `;` comments

        git_message_prettify(&msg, pMessage.toUtf8().constData(), true, ';');

        if (msg.size > 0) {

            git_index *index;
            if (git_repository_index(&index, mGitRepository) == 0) {

                git_oid treeId;
                if (git_index_write_tree(&treeId, index) == 0) {

                    git_tree *tree;
                    if (git_tree_lookup(&tree, mGitRepository, &treeId) == 0) {

                        // Get HEAD as a commit object to use as the parent of the commit

                        git_oid parentId;
                        if (git_reference_name_to_id(&parentId, mGitRepository, "HEAD") == 0) {

                            git_commit *parent;
                            if (git_commit_lookup(&parent, mGitRepository, &parentId) == 0) {
                                // Do the commit

                                git_signature *author;
                                if (git_signature_now(&author, "Test Author", "testing@staging.physiomeproject.org") == 0) {

                                    git_oid commitId;
                                    if (git_commit_create_v(&commitId, mGitRepository, "HEAD",
                                                            author, author, NULL, msg.ptr,
                                                            tree, 1, parent) == 0) {
                                        success = true;
                                    }
                                    git_signature_free(author);
                                }
                                git_commit_free(parent);
                            }
                        }
                    }
                    git_tree_free(tree);
                }
                git_index_free(index);
            }
            if (!success)
                emitGitError(tr("An error occurred while trying to commit to the workspace"));
        }
        git_buf_free(&msg);
    }
    return success;
}

//==============================================================================

PmrWorkspace::RemoteStatus PmrWorkspace::gitRemoteStatus(void) const
{
    // Get the status of the repository

    auto status = StatusUnknown;

    if (this->opened()) {
        git_oid masterOid, originMasterOid;

        if (git_reference_name_to_id(&masterOid, mGitRepository,
                                                 "refs/heads/master") == 0
         && git_reference_name_to_id(&originMasterOid, mGitRepository,
                                                 "refs/remotes/origin/master") == 0) {
            size_t ahead = 0;
            size_t behind = 0;

            if (git_graph_ahead_behind(&ahead, &behind, mGitRepository,
                                       &masterOid, &originMasterOid) == 0) {
                status = behind ? StatusBehind
                       : ahead  ? StatusAhead
                       :          StatusCurrent;
// TODO If there are uncommitted files or files in the index (to be committed)
//      then return StatusCommit
            }
        }
        if (status == StatusUnknown)
            emitGitError(tr("An error occurred while trying to get the status of %1").arg(mPath));
    }

        if (mStagedCount > 0) status = (RemoteStatus)(status | StatusCommit);
    return status;
}

//==============================================================================

const QPair<QChar, QChar> PmrWorkspace::gitStatusChars(const int &flags)
{
    char istatus = ' ';
    char wstatus = ' ';

    if (flags & GIT_STATUS_INDEX_NEW) istatus = 'A';
    if (flags & GIT_STATUS_INDEX_MODIFIED) istatus = 'M';
    if (flags & GIT_STATUS_INDEX_DELETED) istatus = 'D';
    if (flags & GIT_STATUS_INDEX_RENAMED) istatus = 'R';
    if (flags & GIT_STATUS_INDEX_TYPECHANGE) istatus = 'T';

    if (flags & GIT_STATUS_WT_NEW) wstatus = 'A';
    if (flags & GIT_STATUS_WT_MODIFIED) wstatus = 'M';
    if (flags & GIT_STATUS_WT_DELETED) wstatus = 'D';
    if (flags & GIT_STATUS_WT_RENAMED) wstatus = 'R';
    if (flags & GIT_STATUS_WT_TYPECHANGE) wstatus = 'T';

    if (flags & GIT_STATUS_IGNORED) {
        istatus = '!';
        wstatus = '!';
    }
    return QPair<QChar, QChar>(istatus, wstatus);
}

//==============================================================================

const QPair<QChar, QChar> PmrWorkspace::gitFileStatus(const QString &pPath) const
{
    // Get the status of a file
// TODO from statusmap

    auto status = QPair<QChar, QChar>(' ', ' ');
//qDebug() << "Get status: " << pPath;
    if (this->opened()) {
        auto repoDir = QDir(mPath);
        auto relativePath = repoDir.relativeFilePath(pPath);

        unsigned int statusFlags = 0;
        if (git_status_file(&statusFlags, mGitRepository, relativePath.toUtf8().constData()) == 0)
            status = gitStatusChars(statusFlags);
        else
            emitGitError(tr("An error occurred while trying to get the status of %1").arg(pPath));
    }
    return status;
}

//==============================================================================

void PmrWorkspace::stageFile(const QString &pPath, const bool &pStage)
{
    if (this->open()) {
        auto repoDir = QDir(mPath);
        auto relativePath = repoDir.relativeFilePath(pPath).toUtf8();
        bool success = false;

        git_index *index;
        if (git_repository_index(&index, mGitRepository) == 0) {

            if (pStage) {
                success = (git_index_add_bypath(index, relativePath.constData()) == 0);
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
                        else if (git_index_remove_bypath(index, relativePath.constData()) == 0) {
                            success = true;
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

    QList::append(new PmrWorkspace(pUrl, pName, parent));
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

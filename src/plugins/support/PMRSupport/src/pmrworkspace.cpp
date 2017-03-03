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

#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"
#include "pmrworkspace.h"
#include "pmrworkspacemanager.h"
#include "preferencesinterface.h"

//==============================================================================

#include <QDir>
#include <QStandardPaths>

//==============================================================================

#include "git2/blob.h"
#include "git2/clone.h"
#include "git2/commit.h"
#include "git2/errors.h"
#include "git2/graph.h"
#include "git2/index.h"
#include "git2/merge.h"
#include "git2/message.h"
#include "git2/remote.h"
#include "git2/repository.h"
#include "git2/signature.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

void PmrWorkspace::constructor(const bool &pOwned, const QString &pName,
                               const QString &pUrl, const QString &pDescription,
                               const QString &pOwner, PmrWebService *pParent)
{
    // Initialise ourselves

    mOwned = pOwned;
    mPath = QString();
    mUsername = QString();
    mPassword = QString();
    mName = pName;
    mUrl = pUrl;
    mDescription = pDescription;
    mOwner = pOwner;

    mGitRepository = 0;

    mRootFileNode = new PmrWorkspaceFileNode(0);
    mRepositoryStatusMap = QMap<QString, PmrWorkspaceFileNode *>();

    mConflictedFiles = QStringList();
    mUpdatedFiles = QStringList();

    mStagedCount = 0;
    mUnstagedCount = 0;

    // Make sure that the status of a workspace that has just been cloned is
    // up to date
    // Note: ideally, we would do this within the clone() method, but we can't
    //       since it's executed in a different thread...

    connect(this, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(refreshStatus()));

    // Forward our signals to the 'global' instance of our workspace manager
    // class

    PmrWorkspaceManager *workspaceManager = PmrWorkspaceManager::instance();

    connect(this, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            workspaceManager, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)));
    connect(this, SIGNAL(workspaceUncloned(PMRSupport::PmrWorkspace *)),
            workspaceManager, SIGNAL(workspaceUncloned(PMRSupport::PmrWorkspace *)));
    connect(this, SIGNAL(workspaceSynchronized(PMRSupport::PmrWorkspace *)),
            workspaceManager, SIGNAL(workspaceSynchronized(PMRSupport::PmrWorkspace *)));

    // Forward our signals to our parent PMR web service

    connect(this, SIGNAL(information(const QString &)),
            pParent, SIGNAL(information(const QString &)));
    connect(this, SIGNAL(warning(const QString &)),
            pParent, SIGNAL(warning(const QString &)));
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const bool &pOwned, const QString &pName,
                           const QString &pUrl, const QString &pDescription,
                           const QString &pOwner, PmrWebService *pParent) :
    QObject(pParent)
{
    // Construct our PMR workspace

    constructor(pOwned, pName, pUrl, pDescription, pOwner, pParent);
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const bool &pOwned, const QString &pName,
                           const QString &pUrl, PmrWebService *pParent) :
    QObject(pParent)
{
    // Construct our PMR workspace

    constructor(pOwned, pName, pUrl, QString(), QString(), pParent);
}

//==============================================================================

PmrWorkspace::~PmrWorkspace()
{
    // Close ourselves

    close();

    // Delete some internal objects
    // Note: unlike for our other file nodes, we need to delete our root one
    //       since it's not owned by a QObject-based object. This also means
    //       that by deleting our root file node, all our other file nodes will
    //       be deleted automatically...

    delete mRootFileNode;
}

//==============================================================================

bool PmrWorkspace::compare(PmrWorkspace *pWorkspace1, PmrWorkspace *pWorkspace2)
{
    // Return whether the first workspace is lower than the second one (without
    // worrying about casing)

    return pWorkspace1->name().compare(pWorkspace2->name(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

bool PmrWorkspace::isLocal() const
{
    // Return whether we are local, i.e. whether our path isn't empty

    return !mPath.isEmpty();
}

//==============================================================================

bool PmrWorkspace::isOwned() const
{
    // Return whether we are owned

    return mOwned;
}

//==============================================================================

QString PmrWorkspace::path() const
{
    // Return our path

    return mPath;
}

//==============================================================================

void PmrWorkspace::setCredentials(const QString &pUsername,
                                  const QString &pPassword)
{
    // Set our credentials

    mUsername = pUsername;
    mPassword = pPassword;
}

//==============================================================================

QString PmrWorkspace::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

QString PmrWorkspace::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

QString PmrWorkspace::description() const
{
    // Return our description

    return mDescription;
}

//==============================================================================

QString PmrWorkspace::owner() const
{
    // Return our owner

    return mOwner;
}

//==============================================================================

PmrWorkspaceFileNode * PmrWorkspace::rootFileNode() const
{
    // Return our root file node

    return mRootFileNode;
}

//==============================================================================

void PmrWorkspace::clone(const QString &pPath)
{
    // Clone the workspace to the given directory, using basic authentication

    git_clone_options cloneOptions;
    git_strarray authorizationStrArray = { 0, 0 };

    setGitAuthorization(&authorizationStrArray);

    git_clone_init_options(&cloneOptions, GIT_CLONE_OPTIONS_VERSION);

    cloneOptions.fetch_opts.callbacks.certificate_check = certificateCheckCallback;
    cloneOptions.fetch_opts.callbacks.payload = this;
    cloneOptions.fetch_opts.custom_headers = authorizationStrArray;

    cloneOptions.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;

    // Perform the cloning itself and let people know whether it didn't work

    bool cloned = true;

    if (git_clone(&mGitRepository, mUrl.toUtf8().constData(),
                  pPath.toUtf8().constData(), &cloneOptions, 1) < 0) {
        emitGitError(tr("An error occurred while trying to clone the workspace."));

        cloned = false;
    }

    git_strarray_free(&authorizationStrArray);

    // Open ourselves in the given path and ask the workspace manager to keep
    // track of us, if we have been successfully cloned
    // Note: we don't want to refresh our status as part of opening ourselves.
    //       Indeed, this may require updating the GUI and there is no guarantee
    //       that cloning is being done in the same thread as our GUI (see
    //       PmrWebService::requestWorkspaceClone() for example)...

    if (cloned && open(pPath, false)) {
        PmrWorkspaceManager::instance()->addWorkspace(this);

        emit workspaceCloned(this);
    }
}

//==============================================================================

void PmrWorkspace::close()
{
    // Close ourselves, i.e. reset both our path and our Git repository object

    mPath = QString();

    mRootFileNode->setPath(QString());

    git_repository_free(mGitRepository);

    mGitRepository = 0;
}

//==============================================================================

bool PmrWorkspace::doCommit(const char *pMessage, const size_t &pParentCount,
                            const git_commit **pParents)
{
    // Commit everything that is staged

    git_signature *author = 0;
    QByteArray name = PreferencesInterface::preference(PluginName, SettingsPreferencesName).toByteArray();
    QByteArray email = PreferencesInterface::preference(PluginName, SettingsPreferencesEmail).toByteArray();
    git_index *index = 0;
    git_oid treeId;
    git_tree *tree = 0;
    git_oid commitId;

    bool res =    git_signature_now(&author, name.data(), email.data())
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

    return !res;
}

//==============================================================================

bool PmrWorkspace::commit(const QString &pMessage)
{
    // Make sure that we are open

    if (!isOpen())
        return false;

    // Get an empty buffer to hold the cleaned message

    git_buf message;

    message.ptr = 0;

    git_buf_set(&message, 0, 0);

    // Clean up the message and remove comments (which start with ";")

    git_message_prettify(&message, pMessage.toUtf8().constData(), true, ';');

    bool res = true;

    if (message.size) {
        int parentsCount = -1;
        git_commit *parent = 0;

        if (git_repository_head_unborn(mGitRepository) == 1) {
            // We are committing to an empty repository

            parentsCount = 0;
        } else {
            // Get HEAD as the commit object to use as the parent of the commit

            git_oid parentId;

            if (   !git_reference_name_to_id(&parentId, mGitRepository, "HEAD")
                && !git_commit_lookup(&parent, mGitRepository, &parentId)) {
                parentsCount = 1;
            }
        }

        if (parentsCount >= 0) {
            res = doCommit(message.ptr, parentsCount,
                           (const git_commit **) (&parent));

            if (!res)
                emitGitError(tr("An error occurred while trying to commit to the workspace (you must provide both a name and an email)."));
        }

        if (parent)
            git_commit_free(parent);
    } else {
        emitGitError(tr("An error occurred while trying to commit to the workspace (you must provide a message)."));
    }

    git_buf_free(&message);

    return res;
}

//==============================================================================

typedef struct {
    size_t size;
    git_repository *repository;
    git_commit **parents;
} MergeheadForeachCallbackData;

//==============================================================================

bool PmrWorkspace::commitMerge()
{
    // Get the number of merge heads so we can allocate an array for parents

    MergeheadForeachCallbackData data = { 0, 0, 0 };

    git_repository_mergehead_foreach(mGitRepository, mergeheadForeachCallback, &data);

    size_t parentsCount = data.size+1;
    git_commit **parents = new git_commit*[parentsCount]();

    // HEAD is always a parent

    bool res = false;
    git_oid parentId;

    if (   !git_reference_name_to_id(&parentId, mGitRepository, "HEAD")
        && !git_commit_lookup(parents, mGitRepository, &parentId)) {
        res = true;

        // Populate the list of commit parents

        if (parentsCount > 1) {
            data.size = 0;
            data.repository = mGitRepository;
            data.parents = &(parents[1]);

            if (git_repository_mergehead_foreach(mGitRepository, mergeheadForeachCallback, &data))
                res = false;
        }

        if (res) {
            std::string message = std::string("Merge branch 'master' of ");

            if (doCommit(message.c_str(), parentsCount,
                         (const git_commit **) parents)) {
                git_repository_state_cleanup(mGitRepository);
            } else {
                res = false;
            }
        }
    }

    for (size_t n = 0; n < parentsCount; ++n)
        git_commit_free(parents[n]);

    delete[] parents;

    if (!res)
        emitGitError(tr("An error occurred while trying to commit the merge."));

    return res;
}

//==============================================================================

bool PmrWorkspace::isMerging() const
{
    // Return whether we are merging

    return git_repository_state(mGitRepository) == GIT_REPOSITORY_STATE_MERGE;
}

//==============================================================================

bool PmrWorkspace::isOpen() const
{
    // Return whether we are open

    return mGitRepository;
}

//==============================================================================

bool PmrWorkspace::open(const QString &pPath, const bool &pRefreshStatus)
{
    // Open ourselves by first making sure that we are closed

    close();

    // Now, set our path and get our Git repository object ready

    mPath = pPath;

    mRootFileNode->setPath(pPath);

    if (!pPath.isEmpty()) {
        if (!git_repository_open(&mGitRepository, pPath.toUtf8().constData())) {
            if (pRefreshStatus)
                refreshStatus();

            return true;
        }

        emitGitError(tr("An error occurred while trying to open the workspace."));
    }

    return false;
}

//==============================================================================

CharPair PmrWorkspace::gitStatusChars(const int &pFlags) const
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

void PmrWorkspace::refreshStatus()
{
    // Keep track of our 'old' file nodes

    PmrWorkspaceFileNodes oldFileNodes = mRepositoryStatusMap.values();

    // Refresh our status

    mStagedCount = 0;
    mUnstagedCount = 0;

    mRepositoryStatusMap.clear();

    if (isOpen()) {
        git_status_options statusOptions;

        git_status_init_options(&statusOptions, GIT_STATUS_OPTIONS_VERSION);

        statusOptions.flags =  GIT_STATUS_OPT_INCLUDE_UNTRACKED
                              |GIT_STATUS_OPT_INCLUDE_UNMODIFIED
                              |GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX
                              |GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY
                              |GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
        statusOptions.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;

        git_status_list *statusList;

        if (!git_status_list_new(&statusList, mGitRepository, &statusOptions)) {
            PmrWorkspaceFileNodes fileNodes = PmrWorkspaceFileNodes();
            PmrWorkspaceFileNode *currentFileNode = mRootFileNode;

            fileNodes << currentFileNode;

            // Go through the different entries

            for (size_t i = 0, iMax = git_status_list_entrycount(statusList); i < iMax; ++i) {
                const git_status_entry *status = git_status_byindex(statusList, i);
                const char *filePath = (status->head_to_index)?
                                           status->head_to_index->old_file.path:
                                           (status->index_to_workdir)?
                                               status->index_to_workdir->old_file.path:
                                               0;

                if (filePath) {
                    CharPair statusChars = gitStatusChars(status->status);

                    if (statusChars.first != ' ')
                        ++mStagedCount;

                    if (statusChars.second != ' ')
                        ++mUnstagedCount;

                    // Find the correct place in the tree to add the file

                    QStringList pathComponents = QString(filePath).split('/');
                    int pathComponentsCount = pathComponents.count();
                    int i = 0;
                    int n = qMin(pathComponentsCount, fileNodes.count())-1;

                    while (   (i < n)
                           && pathComponents[i].compare(fileNodes[i+1]->name(), Qt::CaseInsensitive)) {
                        ++i;
                    }

                    // Cut back the stack so that it matches the path component

                    while (i+1 < fileNodes.count())
                        fileNodes.removeLast();

                    currentFileNode = fileNodes[i];

                    // Add the directory nodes as required

                    while (i < pathComponentsCount-1) {
                        currentFileNode = currentFileNode->addChild(pathComponents[i]);

                        fileNodes << currentFileNode;

                        ++i;
                    }

                    mRepositoryStatusMap.insert(filePath, currentFileNode->addChild(pathComponents[i], statusChars));
                }
            }

            git_status_list_free(statusList);
        }

        // Delete any 'old' file node that is not being used anymore

        PmrWorkspaceFileNodes newFileNodes = mRepositoryStatusMap.values();
        PmrWorkspaceFileNodes oldFileNodesToDelete = PmrWorkspaceFileNodes();

        foreach (PmrWorkspaceFileNode *oldFileNode, oldFileNodes) {
            if (!newFileNodes.contains(oldFileNode))
                oldFileNodesToDelete << oldFileNode;
        }

        if (!oldFileNodesToDelete.isEmpty())
            deleteFileNodes(mRootFileNode, oldFileNodesToDelete);
    } else if (mRootFileNode->hasChildren()) {
        // We are not open, so clear our root file node

        foreach (PmrWorkspaceFileNode *child, mRootFileNode->children())
            delete child;
    }
}

//==============================================================================

void PmrWorkspace::deleteFileNodes(PmrWorkspaceFileNode *pFileNode,
                                   PmrWorkspaceFileNodes &pFileNodes)
{
    // Recursively delete the given file nodes, unless there is none left

    if (pFileNodes.isEmpty())
        return;

    if (pFileNode->hasChildren()) {
        foreach (PmrWorkspaceFileNode *child, pFileNode->children())
            deleteFileNodes(child, pFileNodes);

        // Remove the file node or let people know that we have been uncloned,
        // if the file node doesn't have any children anymore

        if (!pFileNode->hasChildren()) {
            if (pFileNode->parent()) {
                pFileNode->parent()->removeChild(pFileNode);
            } else {
                // The folder where we were cloned has been deleted, so close
                // ourselves and ask the workspace manager to stop tracking us,
                // if we are not owned

                close();

                if (!isOwned())
                    PmrWorkspaceManager::instance()->removeWorkspace(this);

                emit workspaceUncloned(this);
            }

            pFileNodes.removeOne(pFileNode);
        }
    } else if (pFileNodes.contains(pFileNode)) {
        pFileNode->parent()->removeChild(pFileNode);

        pFileNodes.removeOne(pFileNode);
    }
}

//==============================================================================

void PmrWorkspace::synchronize(const bool &pPush)
{
    // Synchronise our local workspace with PMR and let people know that we have
    // done so

    if (fetch() && merge() && pPush) {
        // We've successfully fetched and merged, so push if we are allowed to

        push();
    }

    emit workspaceSynchronized(this);
}

//==============================================================================

QByteArray PmrWorkspace::headFileContents(const QString &pFileName)
{
    // Retrieve the contents of the given file name at the HEAD revision
    // Note: the below code is based on Repository::GetHeadBlob() from git-utils
    //       (see https://github.com/atom/git-utils)...

    git_reference *head;

    if (git_repository_head(&head, mGitRepository) != GIT_OK)
        return QByteArray();

    const git_oid *sha = git_reference_target(head);
    git_commit *commit;
    int commitStatus = git_commit_lookup(&commit, mGitRepository, sha);

    git_reference_free(head);

    if (commitStatus != GIT_OK)
        return QByteArray();

    git_tree *tree;
    int treeStatus = git_commit_tree(&tree, commit);

    git_commit_free(commit);

    if (treeStatus != GIT_OK)
        return QByteArray();

    git_tree_entry *treeEntry;

    if (git_tree_entry_bypath(&treeEntry, tree, pFileName.toUtf8().constData()) != GIT_OK) {
        git_tree_free(tree);

        return QByteArray();
    }

    git_blob *blob = 0;
    const git_oid *blobSha = git_tree_entry_id(treeEntry);

    if (blobSha && (git_blob_lookup(&blob, mGitRepository, blobSha) != GIT_OK))
        blob = 0;

    git_tree_entry_free(treeEntry);
    git_tree_free(tree);

    if (!blob)
        return QByteArray();

    QByteArray res(static_cast<const char *>(git_blob_rawcontent(blob)), git_blob_rawsize(blob));

    git_blob_free(blob);

    return res;
}

//==============================================================================

CharPair PmrWorkspace::gitFileStatus(const QString &pPath) const
{
    // Retrieve and return the status of the file, which path is given

    CharPair res = CharPair(' ', ' ');

    if (isOpen()) {
        unsigned int statusFlags = 0;

        if (!git_status_file(&statusFlags, mGitRepository,
                             QDir(mPath).relativeFilePath(pPath).toUtf8().constData())) {
             // Retrieve the status itself

             res = gitStatusChars(statusFlags);

            // Also update the status in our file tree

            if (mRepositoryStatusMap.contains(pPath))
                mRepositoryStatusMap.value(pPath)->setStatus(res);
         } else {
             emitGitError(tr("An error occurred while trying to get the status of %1.").arg(pPath));
         }
    }

    return res;
}

//==============================================================================

PmrWorkspace::WorkspaceStatus PmrWorkspace::gitWorkspaceStatus() const
{
    // Get the status of the workspace

    WorkspaceStatus res = StatusUnknown;

    if (isOpen()) {
        if (git_repository_head_unborn(mGitRepository) == 1) {
            res = StatusCurrent;
        } else {
            git_oid masterOid;
            bool error = false;

            if (!git_reference_name_to_id(&masterOid, mGitRepository,
                                          "refs/heads/master")) {
                git_oid originMasterOid;

                if (!git_reference_name_to_id(&originMasterOid, mGitRepository,
                                              "refs/remotes/origin/master")) {
                    size_t ahead = 0;
                    size_t behind = 0;

                    if (!git_graph_ahead_behind(&ahead, &behind, mGitRepository,
                                                &masterOid, &originMasterOid)) {
                        res = ahead?
                                  StatusAhead:
                                  behind?
                                      StatusBehind:
                                      StatusCurrent;
                    } else {
                        error = true;
                    }
                } else {
                    res = StatusAhead;
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
                res = WorkspaceStatus(res|StatusConflict);
            } else {
                if (mStagedCount)
                    res = WorkspaceStatus(res|StatusCommit);

                if (mUnstagedCount)
                    res = WorkspaceStatus(res|StatusUnstaged);
            }

            git_index_free(index);
        }
    }

    return res;
}

//==============================================================================

void PmrWorkspace::stageFile(const QString &pPath, const bool &pStage)
{
    // Un/stage the file, which path is given, and let people know of the
    // outcome

    if (isOpen()) {
        QByteArray relativePathByteArray = QDir(mPath).relativeFilePath(pPath).toUtf8();
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

                git_reference *head;

                if (!git_repository_head(&head, mGitRepository)) {
                    git_tree *headTree;

                    if (!git_reference_peel((git_object **) &headTree,
                                            head, GIT_OBJ_TREE)) {
                        git_tree_entry *headEntry;

                        if (!git_tree_entry_bypath(&headEntry, headTree,
                                                   relativePath)) {
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

                    git_reference_free(head);
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

StagedFiles PmrWorkspace::stagedFiles()
{
    // Retrieve and return all current staged files

    StagedFiles res = StagedFiles();
    git_index *index;

    if (!git_repository_index(&index, mGitRepository)) {
        git_status_options statusOptions;

        git_status_init_options(&statusOptions, GIT_STATUS_OPTIONS_VERSION);

        statusOptions.flags =  GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX
                              |GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY;
        statusOptions.show = GIT_STATUS_SHOW_INDEX_ONLY;

        git_status_list *statusList;

        if (!git_status_list_new(&statusList, mGitRepository, &statusOptions)) {
            for (size_t i = 0, iMax = git_status_list_entrycount(statusList); i < iMax; ++i) {
                const git_status_entry *status = git_status_byindex(statusList, i);
                const char *filePath = status->head_to_index?
                                           status->head_to_index->old_file.path:
                                           status->index_to_workdir?
                                               status->index_to_workdir->old_file.path:
                                               0;

                if (filePath) {
                    if (status->status & GIT_STATUS_INDEX_TYPECHANGE)
                        res << StagedFile(filePath, GIT_STATUS_INDEX_TYPECHANGE);
                    else if (status->status & GIT_STATUS_INDEX_RENAMED)
                        res << StagedFile(filePath, GIT_STATUS_INDEX_RENAMED);
                    else if (status->status & GIT_STATUS_INDEX_DELETED)
                        res << StagedFile(filePath, GIT_STATUS_INDEX_DELETED);
                    else if (status->status & GIT_STATUS_INDEX_MODIFIED)
                        res << StagedFile(filePath, GIT_STATUS_INDEX_MODIFIED);
                    else if (status->status & GIT_STATUS_INDEX_NEW)
                        res << StagedFile(filePath, GIT_STATUS_INDEX_NEW);
                }
            }

            git_status_list_free(statusList);
        }

        git_index_free(index);
    }

    return res;
}

//==============================================================================

void PmrWorkspace::setGitAuthorization(git_strarray *pAuthorizationStrArray)
{
    // Set the Git authorisation

    if (!mUsername.isEmpty()) {
        // Initialise with a Basic Authorisation header

        QByteArray authorisationHeader =  QByteArray("Authorization: Basic ")
                                         +QString(mUsername+":"+mPassword).toUtf8().toBase64();

        char *authorizationStrArrayData = (char *) malloc(authorisationHeader.count()+1);
        char **authorizationStrArrayArray = (char **) malloc(sizeof(char *));

        memcpy(authorizationStrArrayData, authorisationHeader.constData(),
               authorisationHeader.count()+1);

        authorizationStrArrayArray[0] = authorizationStrArrayData;

        pAuthorizationStrArray->count = 1;
        pAuthorizationStrArray->strings = authorizationStrArrayArray;
    }
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

int PmrWorkspace::fetchheadForeachCallback(const char *pReferenceName,
                                           const char *pRemoteUrl,
                                           const git_oid *pId,
                                           unsigned int pMerge, void *pPayload)
{
    Q_UNUSED(pReferenceName);

    // Determine whether merging can be done

    int res = 1;

    PmrWorkspace *workspace = (PmrWorkspace *) pPayload;
    git_repository *gitRepository = workspace->mGitRepository;

    workspace->mConflictedFiles = QStringList();
    workspace->mUpdatedFiles = QStringList();

    if (pMerge) {
        git_annotated_commit *remoteCommitHead = 0;

        if (git_annotated_commit_from_fetchhead(&remoteCommitHead, gitRepository,
                                                "origin/master", pRemoteUrl, pId)) {
            res = 0;
        } else {
            // Initialise some common checkout options

            git_checkout_options checkoutOptions;

            git_checkout_init_options(&checkoutOptions, GIT_CHECKOUT_OPTIONS_VERSION);

            checkoutOptions.checkout_strategy =  GIT_CHECKOUT_SAFE
                                                |GIT_CHECKOUT_RECREATE_MISSING
                                                |GIT_CHECKOUT_CONFLICT_STYLE_MERGE;
            checkoutOptions.notify_cb = checkoutNotifyCallback;
            checkoutOptions.notify_flags = GIT_CHECKOUT_NOTIFY_ALL;
            checkoutOptions.notify_payload = workspace;

            // Determine the type of merge we can do

            static const char *RefsHeadMaster = "refs/heads/master";

            git_merge_analysis_t analysis = GIT_MERGE_ANALYSIS_NONE;
            git_merge_preference_t preference = GIT_MERGE_PREFERENCE_NONE;

            git_merge_analysis(&analysis, &preference, gitRepository,
                               (const git_annotated_commit**) &remoteCommitHead, 1);

            if (analysis & GIT_MERGE_ANALYSIS_UNBORN) {
                // We can simply set HEAD to the target commit

                git_reference *newMaster = 0;

                res =    !git_reference_create(&newMaster, gitRepository, RefsHeadMaster,
                                               git_annotated_commit_id(remoteCommitHead),
                                               true, "initial pull")
                      && !git_repository_set_head(gitRepository, RefsHeadMaster)
                      && !git_checkout_head(gitRepository, &checkoutOptions);

                if (newMaster)
                    git_reference_free(newMaster);
            } else if (analysis & GIT_MERGE_ANALYSIS_FASTFORWARD) {
                // We can check out the newly fetched head without merging

                const git_oid *commitId = git_annotated_commit_id(remoteCommitHead);
                git_commit *commit = 0;
                git_reference *headReference = 0;
                git_reference *newHeadReference = 0;

                res =    !git_commit_lookup(&commit, gitRepository, commitId)
                      && !git_checkout_tree(gitRepository, (const git_object *) commit, &checkoutOptions)
                      && !git_reference_lookup(&headReference, gitRepository, RefsHeadMaster);

                if (res) {
                    static const char *LogMessage = "pull: fast-forward";

                    const git_oid *commitId = git_commit_id(commit);

                    if (git_reference_type(headReference) == GIT_REF_OID) {
                        res = !git_reference_set_target(&newHeadReference,
                                                        headReference,
                                                        commitId, LogMessage);
                    } else {
                        res = !git_reference_create(&newHeadReference,
                                                    gitRepository,
                                                    RefsHeadMaster,
                                                    commitId, false, LogMessage);
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

                res = !git_merge(gitRepository,
                                 (const git_annotated_commit **) &remoteCommitHead,
                                 1, &mergeOptions, &checkoutOptions);
            }

            git_annotated_commit_free(remoteCommitHead);
        }
    }

    return !res;
}

//==============================================================================

int PmrWorkspace::mergeheadForeachCallback(const git_oid *pOid, void *pPayload)
{
    // Retrieve and return the number of merge heads

    int res = 0;
    MergeheadForeachCallbackData *data = (MergeheadForeachCallbackData *) pPayload;

    if (data->parents)
        res = git_commit_lookup(&data->parents[data->size], data->repository, pOid);

    if (!res)
        ++data->size;

    return res;
}

//==============================================================================

bool PmrWorkspace::fetch()
{
    // Fetch any updates for a workspace, if possible

    if (!isOpen())
        return false;

    bool res = true;
    git_fetch_options fetchOptions;
    git_strarray authorizationStrArray = { 0, 0 };
    git_remote_callbacks remoteCallbacks;

    git_fetch_init_options(&fetchOptions, GIT_FETCH_OPTIONS_VERSION);

    setGitAuthorization(&authorizationStrArray);

    git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);

    fetchOptions.callbacks.certificate_check = certificateCheckCallback;
    fetchOptions.callbacks.payload = this;

    fetchOptions.custom_headers = authorizationStrArray;

    remoteCallbacks.certificate_check = certificateCheckCallback;

    // Get the remote, connect to it, add a refspec, and do the fetch

    static const char *RefsHeadMaster = "refs/heads/master";

    git_remote *gitRemote = 0;
    git_strarray refSpecsStrArray = { (char **) &RefsHeadMaster, 1 };

    if (   git_remote_lookup(&gitRemote, mGitRepository, "origin")
        || git_remote_fetch(gitRemote, &refSpecsStrArray, &fetchOptions, 0)) {
        emitGitError(tr("An error occurred while trying to fetch the remote workspace."));

        res = false;
    }

    if (gitRemote)
        git_remote_free(gitRemote);

    git_strarray_free(&authorizationStrArray);

    return res;
}

//==============================================================================

bool PmrWorkspace::merge()
{
    // Merge and commit fetched updates, if possible

    if (!isOpen())
        return false;

    bool res = true;
    int error = git_repository_fetchhead_foreach(mGitRepository, fetchheadForeachCallback, this);

    if (!error) {
        // We only need to commit normal merges

        if (git_repository_state(mGitRepository) == GIT_REPOSITORY_STATE_MERGE)
            res = commitMerge();
    } else if (error != GIT_ENOTFOUND) {
        int nbOfConflictedFiles = mConflictedFiles.count();
        QString errorMessage = tr("An error occurred while trying to merge the workspace.");

        if (nbOfConflictedFiles) {
            int counter = 0;

            if (mConflictedFiles.count() == 1)
                errorMessage += "\n\n"+tr("The following file has conflicts:");
            else
                errorMessage += "\n\n"+tr("The following files have conflicts:");

            foreach (const QString &conflictedFile, mConflictedFiles) {
                ++counter;

                if (counter == nbOfConflictedFiles) {
                    errorMessage += "\n"+tr(" - %1.").arg(conflictedFile);
                } else if (counter == nbOfConflictedFiles-1) {
                    errorMessage += "\n"+tr(" - %1; and").arg(conflictedFile);
                } else {
                    errorMessage += "\n"+tr(" - %1;").arg(conflictedFile);
                }
            }
        }

        emitGitError(errorMessage);

        res = false;
    }

    return res;
}

//==============================================================================

void PmrWorkspace::push()
{
    // Push a workspace, if possible

    if (!isOpen())
        return;

    git_push_options pushOptions;
    git_strarray authorizationStrArray = { 0, 0 };
    git_remote_callbacks remoteCallbacks;

    git_push_init_options(&pushOptions, GIT_PUSH_OPTIONS_VERSION);

    setGitAuthorization(&authorizationStrArray);

    git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);

    pushOptions.callbacks.certificate_check = certificateCheckCallback;
    pushOptions.callbacks.payload = this;

    pushOptions.custom_headers = authorizationStrArray;

    remoteCallbacks.certificate_check = certificateCheckCallback;

    // Get the remote, connect to it, add a refspec, and do the push

    static const char *RefsHeadMaster = "refs/heads/master";

    git_remote *gitRemote = 0;
    git_strarray refSpecsStrArray = { (char **) &RefsHeadMaster, 1 };

    if (   git_remote_lookup(&gitRemote, mGitRepository, "origin")
        || git_remote_push(gitRemote, &refSpecsStrArray, &pushOptions)) {
        emitGitError(tr("An error occurred while trying to push the workspace."));
    }

    if (gitRemote)
        git_remote_free(gitRemote);

    git_strarray_free(&authorizationStrArray);
}

//==============================================================================

void PmrWorkspace::emitGitError(const QString &pMessage) const
{
    // Let people know, through a warning, about our Git error

    emit warning(pMessage);
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

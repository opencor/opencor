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

#include "corecliutils.h"
#include "pmrsupportplugin.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"
#include "pmrworkspace.h"
#include "pmrworkspacemanager.h"
#include "preferencesinterface.h"

//==============================================================================

#include <QDir>
#include <QStandardPaths>

//==============================================================================

#include "libgit2begin.h"
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
    #include "git2/status.h"
#include "libgit2end.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWorkspace::PmrWorkspace(bool pOwned, const QString &pName,
                           const QString &pUrl, const QString &pDescription,
                           const QString &pOwner, PmrWebService *pParent) :
    QObject(pParent)
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

    mGitRepository = nullptr;

    mRootFileNode = new PmrWorkspaceFileNode(nullptr);
    mRepositoryStatusMap = QMap<QString, PmrWorkspaceFileNode *>();

    mConflictedFiles = QStringList();
    mUpdatedFiles = QStringList();

    mStagedCount = 0;
    mUnstagedCount = 0;

    // Make sure that the status of a workspace that has just been cloned is
    // up to date
    // Note: ideally, we would do this within the clone() method, but we can't
    //       since it's executed in a different thread...

    connect(this, &PmrWorkspace::workspaceCloned,
            this, &PmrWorkspace::refreshStatus);

    // Forward our signals to the 'global' instance of our workspace manager
    // class

    PmrWorkspaceManager *workspaceManager = PmrWorkspaceManager::instance();

    connect(this, &PmrWorkspace::workspaceCloned,
            workspaceManager, &PmrWorkspaceManager::workspaceCloned);
    connect(this, &PmrWorkspace::workspaceUncloned,
            workspaceManager, &PmrWorkspaceManager::workspaceUncloned);
    connect(this, &PmrWorkspace::workspaceSynchronized,
            workspaceManager, &PmrWorkspaceManager::workspaceSynchronized);

    // Forward our signals to our parent PMR Web service

    connect(this, &PmrWorkspace::information,
            pParent, &PmrWebService::information);
    connect(this, &PmrWorkspace::warning,
            pParent, &PmrWebService::warning);
}

//==============================================================================

PmrWorkspace::PmrWorkspace(bool pOwned, const QString &pName,
                           const QString &pUrl, PmrWebService *pParent) :
    PmrWorkspace(pOwned, pName, pUrl, {}, {}, pParent)
{
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
    git_strarray authorizationStrArray = { nullptr, 0 };

    setGitAuthorization(&authorizationStrArray);

    git_clone_options_init(&cloneOptions, GIT_CLONE_OPTIONS_VERSION);

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

    mRootFileNode->setPath({});

    git_repository_free(mGitRepository);

    mGitRepository = nullptr;
}

//==============================================================================

bool PmrWorkspace::commit(const char *pMessage, const size_t &pParentCount,
                          const git_commit **pParents)
{
    // Commit everything that is staged

    git_signature *author = nullptr;
    QByteArray name = PreferencesInterface::preference(PluginName, SettingsPreferencesName, SettingsPreferencesNameDefault).toByteArray();
    QByteArray email = PreferencesInterface::preference(PluginName, SettingsPreferencesEmail, SettingsPreferencesEmailDefault).toByteArray();
    git_index *index = nullptr;
    git_oid treeId;
    git_tree *tree = nullptr;
    git_oid commitId;

    bool res =    (git_signature_now(&author, name.data(), email.data()) != GIT_OK)
               || (git_repository_index(&index, mGitRepository) != GIT_OK)
               || (git_index_write_tree(&treeId, index) != GIT_OK)
               || (git_tree_lookup(&tree, mGitRepository, &treeId) != GIT_OK)
               || (git_commit_create(&commitId, mGitRepository, "HEAD", author,
                                     author, nullptr, pMessage, tree,
                                     pParentCount, pParents) != GIT_OK);

    if (tree != nullptr) {
        git_tree_free(tree);
    }

    if (index != nullptr) {
        git_index_free(index);
    }

    if (author != nullptr) {
        git_signature_free(author);
    }

    return !res;
}

//==============================================================================

bool PmrWorkspace::commit(const QString &pMessage)
{
    // Make sure that we are open

    if (!isOpen()) {
        return false;
    }

    // Get an empty buffer to hold the cleaned message

    git_buf message;

    message.ptr = nullptr;

    git_buf_set(&message, nullptr, 0);

    // Clean up the message and remove comments (which start with ";")

    git_message_prettify(&message, pMessage.toUtf8().constData(), 1, ';');

    bool res = true;

    if (message.size != 0) {
        int parentsCount = -1;
        git_commit *parent = nullptr;

        if (git_repository_head_unborn(mGitRepository) == 1) {
            // We are committing to an empty repository

            parentsCount = 0;
        } else {
            // Get HEAD as the commit object to use as the parent of the commit

            git_oid parentId;

            if (   (git_reference_name_to_id(&parentId, mGitRepository, "HEAD") == GIT_OK)
                && (git_commit_lookup(&parent, mGitRepository, &parentId) == GIT_OK)) {
                parentsCount = 1;
            }
        }

        if (parentsCount >= 0) {
            res = commit(message.ptr, size_t(parentsCount),
                         const_cast<const git_commit **>(&parent));

            if (!res) {
                emitGitError(tr("An error occurred while trying to commit to the workspace (you must provide both a name and an email)."));
            }
        }

        if (parent != nullptr) {
            git_commit_free(parent);
        }
    } else {
        emitGitError(tr("An error occurred while trying to commit to the workspace (you must provide a message)."));
    }

    git_buf_dispose(&message);

    return res;
}

//==============================================================================

using MergeheadForeachCallbackData = struct {
                                                size_t size;
                                                git_repository *repository;
                                                git_commit **parents;
                                            };

//==============================================================================

bool PmrWorkspace::commitMerge()
{
    // Get the number of merge heads so we can allocate an array for parents

    MergeheadForeachCallbackData data = { 0, nullptr, nullptr };

    git_repository_mergehead_foreach(mGitRepository, mergeheadForeachCallback, &data);

    size_t parentsCount = data.size+1;
    auto parents = new git_commit*[parentsCount]();

    // HEAD is always a parent

    bool res = false;
    git_oid parentId;

    if (   (git_reference_name_to_id(&parentId, mGitRepository, "HEAD") == GIT_OK)
        && (git_commit_lookup(parents, mGitRepository, &parentId) == GIT_OK)) {
        res = true;

        // Populate the list of commit parents

        if (parentsCount > 1) {
            data.size = 0;
            data.repository = mGitRepository;
            data.parents = &(parents[1]);

            if (git_repository_mergehead_foreach(mGitRepository, mergeheadForeachCallback, &data) != GIT_OK) {
                res = false;
            }
        }

        if (res) {
            std::string message = std::string("Merge branch 'master' of ");

            if (commit(message.c_str(), parentsCount, const_cast<const git_commit **>(parents))) {
                git_repository_state_cleanup(mGitRepository);
            } else {
                res = false;
            }
        }
    }

    for (size_t n = 0; n < parentsCount; ++n) {
        git_commit_free(parents[n]);
    }

    delete[] parents;

    if (!res) {
        emitGitError(tr("An error occurred while trying to commit the merge."));
    }

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

    return mGitRepository != nullptr;
}

//==============================================================================

bool PmrWorkspace::open(const QString &pPath, bool pRefreshStatus)
{
    // Open ourselves by first making sure that we are closed

    close();

    // Now, set our path and get our Git repository object ready

    mPath = pPath;

    mRootFileNode->setPath(pPath);

    if (!pPath.isEmpty()) {
        if (git_repository_open(&mGitRepository, pPath.toUtf8().constData()) == GIT_OK) {
            if (pRefreshStatus) {
                refreshStatus();
            }

            return true;
        }

        emitGitError(tr("An error occurred while trying to open the workspace."));
    }

    return false;
}

//==============================================================================

CharPair PmrWorkspace::gitStatusChars(uint pFlags) const
{
    // Git status

    if ((pFlags & GIT_STATUS_CONFLICTED) != 0) {
        return { ' ', 'C' };
    }

    if ((pFlags & GIT_STATUS_IGNORED) != 0) {
        return { ' ', 'I' };
    }

    // iStatus

    QChar iStatus = ' ';

    if ((pFlags & GIT_STATUS_INDEX_TYPECHANGE) != 0) {
        iStatus = 'T';
    } else if ((pFlags & GIT_STATUS_INDEX_RENAMED) != 0) {
        iStatus = 'R';
    } else if ((pFlags & GIT_STATUS_INDEX_DELETED) != 0) {
        iStatus = 'D';
    } else if ((pFlags & GIT_STATUS_INDEX_MODIFIED) != 0) {
        iStatus = 'M';
    } else if ((pFlags & GIT_STATUS_INDEX_NEW) != 0) {
        iStatus = 'A';
    }

    // wStatus

    QChar wStatus = ' ';

    if ((pFlags & GIT_STATUS_WT_TYPECHANGE) != 0) {
        wStatus = 'T';
    } else if ((pFlags & GIT_STATUS_WT_RENAMED) != 0) {
        wStatus = 'R';
    } else if ((pFlags & GIT_STATUS_WT_DELETED) != 0) {
        wStatus = 'D';
    } else if ((pFlags & GIT_STATUS_WT_MODIFIED) != 0) {
        wStatus = 'M';
    } else if ((pFlags & GIT_STATUS_WT_NEW) != 0) {
        wStatus = 'A';
    } else if ((pFlags & GIT_STATUS_WT_UNREADABLE) != 0) {
        wStatus = 'U';
    }

    return { iStatus, wStatus };
}

//==============================================================================

PmrWorkspaceFileNode * PmrWorkspace::parentFileNode(const QString &pPath,
                                                    PmrWorkspaceFileNode *pParentFileNode)
{
    // Check whether the first folder in the path is a child of the given parent
    // file node

    PmrWorkspaceFileNode *realParentFileNode = (pParentFileNode != nullptr)?pParentFileNode:mRootFileNode;
    QStringList pathComponents = pPath.split('/');

    if (pathComponents.count() == 1) {
        // The path only consists of a file name, so return our parent file node

        return realParentFileNode;
    }

    // The path contains some folders, so check whether the top folder is a
    // child of our parent file node and, if so, use it to recursively retrieve
    // the parent file node of the given path, otherwise create one

    QString topFolder = pathComponents.first();
    PmrWorkspaceFileNode *newParentFileNode = nullptr;

    for (auto childFileNode : realParentFileNode->children()) {
        if (topFolder == childFileNode->name()) {
            newParentFileNode = childFileNode;

            break;
        }
    }

    if (newParentFileNode == nullptr) {
        newParentFileNode = realParentFileNode->addChild(topFolder);
    }

    pathComponents.removeFirst();

    return parentFileNode(pathComponents.join('/'), newParentFileNode);
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

        git_status_options_init(&statusOptions, GIT_STATUS_OPTIONS_VERSION);

        statusOptions.flags =  GIT_STATUS_OPT_INCLUDE_UNTRACKED
                              |GIT_STATUS_OPT_INCLUDE_UNMODIFIED
                              |GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX
                              |GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY
                              |GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
        statusOptions.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;

        git_status_list *statusList;

        if (git_status_list_new(&statusList, mGitRepository, &statusOptions) == GIT_OK) {
            // Go through the different entries and keep track of every single
            // one of them in mRepositoryStatusMap, as well as update
            // mStagedCount and mUnstagedCount, if needed

            for (size_t i = 0, iMax = git_status_list_entrycount(statusList); i < iMax; ++i) {
                const git_status_entry *status = git_status_byindex(statusList, i);
                const char *filePath = (status->head_to_index != nullptr)?
                                           status->head_to_index->old_file.path:
                                           (status->index_to_workdir != nullptr)?
                                               status->index_to_workdir->old_file.path:
                                               nullptr;

                if (filePath != nullptr) {
                    CharPair statusChars = gitStatusChars(status->status);

                    if (statusChars.first != ' ') {
                        ++mStagedCount;
                    }

                    if (statusChars.second != ' ') {
                        ++mUnstagedCount;
                    }

                    mRepositoryStatusMap.insert(filePath, parentFileNode(filePath)->addChild(QFileInfo(filePath).fileName(), statusChars));
                }
            }

            git_status_list_free(statusList);
        }

        // Delete any 'old' file node that is not being used anymore

        PmrWorkspaceFileNodes newFileNodes = mRepositoryStatusMap.values();
        PmrWorkspaceFileNodes oldFileNodesToDelete;

        for (auto oldFileNode : oldFileNodes) {
            if (!newFileNodes.contains(oldFileNode)) {
                oldFileNodesToDelete << oldFileNode;
            }
        }

        if (!oldFileNodesToDelete.isEmpty()) {
            deleteFileNodes(mRootFileNode, oldFileNodesToDelete);
        }
    } else if (mRootFileNode->hasChildren()) {
        // We are not open, so clear our root file node

        for (auto child : mRootFileNode->children()) {
            delete child;
        }
    }
}

//==============================================================================

void PmrWorkspace::deleteFileNodes(PmrWorkspaceFileNode *pFileNode,
                                   PmrWorkspaceFileNodes &pFileNodes)
{
    // Recursively delete the given file nodes, unless there is none left

    if (pFileNodes.isEmpty()) {
        return;
    }

    if (pFileNode->hasChildren()) {
        for (auto child : pFileNode->children()) {
            deleteFileNodes(child, pFileNodes);
        }

        // Remove the file node or let people know that we have been uncloned,
        // if the file node doesn't have any children anymore

        if (!pFileNode->hasChildren()) {
            if (pFileNode->parent() != nullptr) {
                pFileNode->parent()->removeChild(pFileNode);
            } else {
                // The folder where we were cloned has been deleted, so close
                // ourselves and ask the workspace manager to stop tracking us,
                // if we are not owned

                close();

                if (!isOwned()) {
                    PmrWorkspaceManager::instance()->removeWorkspace(this);
                }

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

void PmrWorkspace::synchronize(bool pPush)
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

    if (git_repository_head(&head, mGitRepository) != GIT_OK) {
        return {};
    }

    const git_oid *sha = git_reference_target(head);
    git_commit *commit;
    int commitStatus = git_commit_lookup(&commit, mGitRepository, sha);

    git_reference_free(head);

    if (commitStatus != GIT_OK) {
        return {};
    }

    git_tree *tree;
    int treeStatus = git_commit_tree(&tree, commit);

    git_commit_free(commit);

    if (treeStatus != GIT_OK) {
        return {};
    }

    git_tree_entry *treeEntry;

    if (git_tree_entry_bypath(&treeEntry, tree, pFileName.toUtf8().constData()) != GIT_OK) {
        git_tree_free(tree);

        return {};
    }

    git_blob *blob = nullptr;
    const git_oid *blobSha = git_tree_entry_id(treeEntry);

    if (   (blobSha != nullptr)
        && (git_blob_lookup(&blob, mGitRepository, blobSha) != GIT_OK)) {
        blob = nullptr;
    }

    git_tree_entry_free(treeEntry);
    git_tree_free(tree);

    if (blob == nullptr) {
        return {};
    }

    QByteArray res(static_cast<const char *>(git_blob_rawcontent(blob)),
                   int(git_blob_rawsize(blob)));

    git_blob_free(blob);

    return res;
}

//==============================================================================

CharPair PmrWorkspace::gitFileStatus(const QString &pPath) const
{
    // Retrieve and return the status of the file, which path is given

    CharPair res = CharPair(' ', ' ');

    if (isOpen()) {
        uint statusFlags = GIT_STATUS_CURRENT;

        if (git_status_file(&statusFlags, mGitRepository,
                            QDir(mPath).relativeFilePath(pPath).toUtf8().constData()) == GIT_OK) {
             // Retrieve the status itself

             res = gitStatusChars(statusFlags);

            // Also update the status in our file tree

            if (mRepositoryStatusMap.contains(pPath)) {
                mRepositoryStatusMap.value(pPath)->setStatus(res);
            }
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

            if (git_reference_name_to_id(&masterOid, mGitRepository,
                                         "refs/heads/master") == GIT_OK) {
                git_oid originMasterOid;

                if (git_reference_name_to_id(&originMasterOid, mGitRepository,
                                             "refs/remotes/origin/master") == GIT_OK) {
                    size_t ahead = 0;
                    size_t behind = 0;

                    if (git_graph_ahead_behind(&ahead, &behind, mGitRepository,
                                               &masterOid, &originMasterOid) == GIT_OK) {
                        res = (ahead != 0)?
                                  StatusAhead:
                                  (behind != 0)?
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

            if (error) {
                emitGitError(tr("An error occurred while trying to get the remote status of %1.").arg(mPath));
            }
        }

        git_index *index;

        if (git_repository_index(&index, mGitRepository) == GIT_OK) {
            if (git_index_has_conflicts(index) != 0) {
                res = WorkspaceStatus(res|StatusConflict);
            } else {
                if (mStagedCount != 0) {
                    res = WorkspaceStatus(res|StatusStaged);
                }

                if (mUnstagedCount != 0) {
                    res = WorkspaceStatus(res|StatusUnstaged);
                }
            }

            git_index_free(index);
        }
    }

    return res;
}

//==============================================================================

void PmrWorkspace::stageFile(const QString &pPath, bool pStage)
{
    // Un/stage the file, which path is given, and let people know of the
    // outcome

    if (isOpen()) {
        QByteArray relativePathByteArray = QDir(mPath).relativeFilePath(pPath).toUtf8();
        const char *relativePath = relativePathByteArray.constData();
        bool success = false;
        git_index *index;

       if (git_repository_index(&index, mGitRepository) == GIT_OK) {
            if (pStage) {
                uint statusFlags = 0;

                git_status_file(&statusFlags, mGitRepository, relativePath);

                if ((statusFlags & GIT_STATUS_WT_DELETED) != 0) {
                    success = git_index_remove_bypath(index, relativePath) == GIT_OK;
                } else {
                    success = git_index_add_bypath(index, relativePath) == GIT_OK;
                }
            } else if (git_repository_head_unborn(mGitRepository) == 1) {
                success = git_index_remove_bypath(index, relativePath) == GIT_OK;
            } else {
                // We need to add a "reset stage" to the index, which means
                // getting the tree for HEAD and tree_entry for the file

                git_reference *head;

                if (git_repository_head(&head, mGitRepository) == GIT_OK) {
                    git_tree *headTree;

                    if (git_reference_peel(reinterpret_cast<git_object **>(&headTree),
                                           head, GIT_OBJECT_TREE) == GIT_OK) {
                        git_tree_entry *headEntry;

                        if (git_tree_entry_bypath(&headEntry, headTree,
                                                  relativePath) == GIT_OK) {
                            git_index_entry indexEntry;

                            memset(&indexEntry, '\0', sizeof(git_index_entry));

                            indexEntry.id = *git_tree_entry_id(headEntry);
                            indexEntry.mode = uint32_t(git_tree_entry_filemode(headEntry));
                            indexEntry.path = relativePath;

                            git_index_add(index, &indexEntry);

                            git_tree_entry_free(headEntry);

                            success = true;
                        } else {
                            success = git_index_remove_bypath(index, relativePath) == GIT_OK;
                        }

                        git_tree_free(headTree);
                    }

                    git_reference_free(head);
                }
            }

            if (success) {
                git_index_write(index);
            }

            git_index_free(index);
        }

        if (!success) {
            emitGitError(tr("An error occurred while trying to stage %1.").arg(pPath));
        }
    }
}

//==============================================================================

StagedFiles PmrWorkspace::stagedFiles()
{
    // Retrieve and return all current staged files

    StagedFiles res;
    git_index *index;

    if (git_repository_index(&index, mGitRepository) == GIT_OK) {
        git_status_options statusOptions;

        git_status_options_init(&statusOptions, GIT_STATUS_OPTIONS_VERSION);

        statusOptions.flags =  GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX
                              |GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY;
        statusOptions.show = GIT_STATUS_SHOW_INDEX_ONLY;

        git_status_list *statusList;

        if (git_status_list_new(&statusList, mGitRepository, &statusOptions) == GIT_OK) {
            for (size_t i = 0, iMax = git_status_list_entrycount(statusList); i < iMax; ++i) {
                const git_status_entry *status = git_status_byindex(statusList, i);
                const char *filePath = (status->head_to_index != nullptr)?
                                           status->head_to_index->old_file.path:
                                           (status->index_to_workdir != nullptr)?
                                               status->index_to_workdir->old_file.path:
                                               nullptr;

                if (filePath != nullptr) {
                    if ((status->status & GIT_STATUS_INDEX_TYPECHANGE) != 0) {
                        res << StagedFile(filePath, GIT_STATUS_INDEX_TYPECHANGE);
                    } else if ((status->status & GIT_STATUS_INDEX_RENAMED) != 0) {
                        res << StagedFile(filePath, GIT_STATUS_INDEX_RENAMED);
                    } else if ((status->status & GIT_STATUS_INDEX_DELETED) != 0) {
                        res << StagedFile(filePath, GIT_STATUS_INDEX_DELETED);
                    } else if ((status->status & GIT_STATUS_INDEX_MODIFIED) != 0) {
                        res << StagedFile(filePath, GIT_STATUS_INDEX_MODIFIED);
                    } else if ((status->status & GIT_STATUS_INDEX_NEW) != 0) {
                        res << StagedFile(filePath, GIT_STATUS_INDEX_NEW);
                    }
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

        auto authorizationStrArrayData = new char[authorisationHeader.count()+1];
        auto authorizationStrArrayArray = new char*;

        memcpy(authorizationStrArrayData, authorisationHeader.constData(),
               size_t(authorisationHeader.count())+1);

        authorizationStrArrayArray[0] = authorizationStrArrayData;

        pAuthorizationStrArray->count = 1;
        pAuthorizationStrArray->strings = authorizationStrArrayArray;
    }
}

//==============================================================================

int PmrWorkspace::certificateCheckCallback(git_cert *pCertificate, int pValid,
                                           const char *pHost, void *pPayload)
{
    Q_UNUSED(pCertificate)
    Q_UNUSED(pValid)
    Q_UNUSED(pHost)
    Q_UNUSED(pPayload)

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
    Q_UNUSED(pBaseline)
    Q_UNUSED(pTarget)
    Q_UNUSED(pWorkDirectory)

    // Keep track of conflicted and updated files

    auto workspace = reinterpret_cast<PmrWorkspace *>(pPayload);

    if (pNotification == GIT_CHECKOUT_NOTIFY_CONFLICT) {
        workspace->mConflictedFiles << pPath;
    } else if (pNotification == GIT_CHECKOUT_NOTIFY_UPDATED) {
        workspace->mUpdatedFiles << pPath;
    }

    return 0;
}

//==============================================================================

int PmrWorkspace::fetchheadForeachCallback(const char *pReferenceName,
                                           const char *pRemoteUrl,
                                           const git_oid *pId, uint pMerge,
                                           void *pPayload)
{
    Q_UNUSED(pReferenceName)

    // Determine whether merging can be done

    bool res = true;

    auto workspace = reinterpret_cast<PmrWorkspace *>(pPayload);
    git_repository *gitRepository = workspace->mGitRepository;

    workspace->mConflictedFiles = QStringList();
    workspace->mUpdatedFiles = QStringList();

    if (pMerge != 0) {
        git_annotated_commit *remoteCommitHead = nullptr;

        if (git_annotated_commit_from_fetchhead(&remoteCommitHead, gitRepository,
                                                "origin/master", pRemoteUrl, pId) != GIT_OK) {
            res = false;
        } else {
            // Initialise some common checkout options

            git_checkout_options checkoutOptions;

            git_checkout_options_init(&checkoutOptions, GIT_CHECKOUT_OPTIONS_VERSION);

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
                               const_cast<const git_annotated_commit **>(&remoteCommitHead), 1);

            if ((analysis & GIT_MERGE_ANALYSIS_UNBORN) != 0) {
                // We can simply set HEAD to the target commit

                git_reference *newMaster = nullptr;

                res =    (git_reference_create(&newMaster, gitRepository, RefsHeadMaster,
                                               git_annotated_commit_id(remoteCommitHead),
                                               1, "initial pull") == GIT_OK)
                      && (git_repository_set_head(gitRepository, RefsHeadMaster) == GIT_OK)
                      && (git_checkout_head(gitRepository, &checkoutOptions) == GIT_OK);

                if (newMaster != nullptr) {
                    git_reference_free(newMaster);
                }
            } else if ((analysis & GIT_MERGE_ANALYSIS_FASTFORWARD) != 0) {
                // We can check out the newly fetched head without merging

                const git_oid *commitId = git_annotated_commit_id(remoteCommitHead);
                git_commit *commit = nullptr;
                git_reference *headReference = nullptr;
                git_reference *newHeadReference = nullptr;

                res =    (git_commit_lookup(&commit, gitRepository, commitId) == GIT_OK)
                      && (git_checkout_tree(gitRepository, reinterpret_cast<const git_object *>(commit), &checkoutOptions) == GIT_OK)
                      && (git_reference_lookup(&headReference, gitRepository, RefsHeadMaster) == GIT_OK);

                if (res) {
                    static const char *PullLogMessage = "pull: fast-forward";

                    const git_oid *pullCommitId = git_commit_id(commit);

                    if (git_reference_type(headReference) == GIT_REFERENCE_DIRECT) {
                        res = git_reference_set_target(&newHeadReference,
                                                       headReference,
                                                       pullCommitId,
                                                       PullLogMessage) == GIT_OK;
                    } else {
                        res = git_reference_create(&newHeadReference,
                                                   gitRepository,
                                                   RefsHeadMaster, pullCommitId,
                                                   0, PullLogMessage) == GIT_OK;
                    }
                }

                if (commit != nullptr) {
                    git_commit_free(commit);
                }

                if (headReference != nullptr) {
                    git_reference_free(headReference);
                }

                if (newHeadReference != nullptr) {
                    git_reference_free(newHeadReference);
                }
            } else if ((analysis & GIT_MERGE_ANALYSIS_NORMAL) != 0) {
                // A "normal" merge, i.e. both HEAD and the given merge input
                // have diverged from their common ancestor, which means that
                // the divergent commits must be merged

                git_merge_options mergeOptions;

                git_merge_options_init(&mergeOptions, GIT_MERGE_OPTIONS_VERSION);

                res = git_merge(gitRepository,
                                const_cast<const git_annotated_commit **>(&remoteCommitHead),
                                1, &mergeOptions, &checkoutOptions) == GIT_OK;
            }

            git_annotated_commit_free(remoteCommitHead);
        }
    }

    return int(!res);
}

//==============================================================================

int PmrWorkspace::mergeheadForeachCallback(const git_oid *pOid, void *pPayload)
{
    // Retrieve and return the number of merge heads

    int res = 0;
    auto data = reinterpret_cast<MergeheadForeachCallbackData *>(pPayload);

    if (data->parents != nullptr) {
        res = git_commit_lookup(&data->parents[data->size], data->repository, pOid);
    }

    if (res == 0) {
        ++data->size;
    }

    return res;
}

//==============================================================================

bool PmrWorkspace::fetch()
{
    // Fetch any updates for a workspace, if possible

    if (!isOpen()) {
        return false;
    }

    bool res = true;
    git_fetch_options fetchOptions;
    git_strarray authorizationStrArray = { nullptr, 0 };
    git_remote_callbacks remoteCallbacks;

    git_fetch_options_init(&fetchOptions, GIT_FETCH_OPTIONS_VERSION);

    setGitAuthorization(&authorizationStrArray);

    git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);

    fetchOptions.callbacks.certificate_check = certificateCheckCallback;
    fetchOptions.callbacks.payload = this;

    fetchOptions.custom_headers = authorizationStrArray;

    remoteCallbacks.certificate_check = certificateCheckCallback;

    // Get the remote, connect to it, add a refspec, and do the fetch

    static const char *RefsHeadMaster = "refs/heads/master";

    git_remote *gitRemote = nullptr;
    git_strarray refSpecsStrArray = { const_cast<char **>(&RefsHeadMaster), 1 };

    if (   (git_remote_lookup(&gitRemote, mGitRepository, "origin") != GIT_OK)
        || (git_remote_fetch(gitRemote, &refSpecsStrArray, &fetchOptions, nullptr) != GIT_OK)) {
        emitGitError(tr("An error occurred while trying to fetch the remote workspace."));

        res = false;
    }

    if (gitRemote != nullptr) {
        git_remote_free(gitRemote);
    }

    git_strarray_free(&authorizationStrArray);

    return res;
}

//==============================================================================

bool PmrWorkspace::merge()
{
    // Merge and commit fetched updates, if possible

    if (!isOpen()) {
        return false;
    }

    bool res = true;
    int error = git_repository_fetchhead_foreach(mGitRepository, fetchheadForeachCallback, this);

    if (error == GIT_OK) {
        // We only need to commit normal merges

        if (git_repository_state(mGitRepository) == GIT_REPOSITORY_STATE_MERGE) {
            res = commitMerge();
        }
    } else if (error != GIT_ENOTFOUND) {
        int nbOfConflictedFiles = mConflictedFiles.count();
        QString errorMessage = tr("An error occurred while trying to merge the workspace.");

        if (nbOfConflictedFiles != 0) {
            int counter = 0;

            if (mConflictedFiles.count() == 1) {
                errorMessage += "\n\n"+tr("The following file has conflicts:");
            } else {
                errorMessage += "\n\n"+tr("The following files have conflicts:");
            }

            for (const auto &conflictedFile : mConflictedFiles) {
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

    if (!isOpen()) {
        return;
    }

    git_push_options pushOptions;
    git_strarray authorizationStrArray = { nullptr, 0 };
    git_remote_callbacks remoteCallbacks;

    git_push_options_init(&pushOptions, GIT_PUSH_OPTIONS_VERSION);

    setGitAuthorization(&authorizationStrArray);

    git_remote_init_callbacks(&remoteCallbacks, GIT_REMOTE_CALLBACKS_VERSION);

    pushOptions.callbacks.certificate_check = certificateCheckCallback;
    pushOptions.callbacks.payload = this;

    pushOptions.custom_headers = authorizationStrArray;

    remoteCallbacks.certificate_check = certificateCheckCallback;

    // Get the remote, connect to it, add a refspec, and do the push

    static const char *RefsHeadMaster = "refs/heads/master";

    git_remote *gitRemote = nullptr;
    git_strarray refSpecsStrArray = { const_cast<char **>(&RefsHeadMaster), 1 };

    if (   (git_remote_lookup(&gitRemote, mGitRepository, "origin") != GIT_OK)
        || (git_remote_push(gitRemote, &refSpecsStrArray, &pushOptions) != GIT_OK)) {
        emitGitError(tr("An error occurred while trying to push the workspace."));
    }

    if (gitRemote != nullptr) {
        git_remote_free(gitRemote);
    }

    git_strarray_free(&authorizationStrArray);
}

//==============================================================================

void PmrWorkspace::emitGitError(const QString &pMessage) const
{
    // Let people know, through a warning, about our Git error

    const git_error *gitError = git_error_last();

    if (gitError != nullptr) {
        emit warning(tr("%1\n\nGit message: %2.").arg(pMessage,
                                                      Core::formatMessage(gitError->message)));
    } else {
        emit warning(pMessage);
    }
}

//==============================================================================

} // namespace PMRSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

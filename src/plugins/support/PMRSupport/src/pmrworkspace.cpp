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
#include "pmrworkspace.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QDir>
#include <QStandardPaths>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

QString PmrWorkspace::WorkspacesDirectory()
{
    static QString directory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                             + QDir::separator() + "OpenCOR" + QDir::separator() + "Workspaces";
    return directory;
}

//==============================================================================

PmrWorkspace::PmrWorkspace(QObject *parent) : QObject(parent),
     mDescription(QString()), mName(QString()), mOwner(QString()), mPath(QString()), mUrl(QString())
{
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const QString &pUrl, const QString &pName, QObject *parent) :
    QObject(parent), mOwned(false),
    mDescription(QString()), mName(pName), mOwner(QString()),
    mPassword(QString()), mPath(QString()), mUrl(pUrl), mUsername(QString())
{
    // Name, description and owner are set from PMR workspace info
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

void PmrWorkspace::emitProgress(const double &pProgress)
{
    emit progress(pProgress);
}

//==============================================================================
//==============================================================================

QString PmrWorkspace::getUrlFromFolder(const QString &pFolder)
{
    // Return remote.origin.url if the folder contains a git repository
    // otherwise return an empty string.

    auto url = QString();

    git_libgit2_init();
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

    git_libgit2_shutdown();

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

    git_libgit2_init();

    git_repository *gitRepository = 0;
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

    auto authorizationHeader = QByteArray();
    if (!mUsername.isEmpty()) {
        authorizationHeader.append("Authorization: Basic ");
        authorizationHeader.append((mUsername + ":" + mPassword).toUtf8().toBase64());

        const char *authorizationStrArrayData[] = { authorizationHeader.constData() };
        git_strarray authorizationStrArray = { (char **)authorizationStrArrayData, 1 };
        cloneOptions.fetch_opts.custom_headers = authorizationStrArray;
    }

    int error = git_clone(&gitRepository, workspaceByteArray.constData(),
                          dirNameByteArray.constData(), &cloneOptions);

    if (error) {
        const git_error *gitError = giterr_last();
        emit warning(gitError?
                         tr("Error %1: %2.").arg(QString::number(gitError->klass),
                                                 Core::formatMessage(gitError->message)):
                         tr("An error occurred while trying to clone the workspace."));
    }
    else if (gitRepository) {
        git_repository_free(gitRepository);
    }

    git_libgit2_shutdown();

    mPath = pDirName;

    emit workspaceCloned(this);
}

//==============================================================================

const QString PmrWorkspace::gitStatus(void) const
{
    // Get the status of the repository

    auto status = QString("");

    git_libgit2_init();

    git_repository *gitRepository = 0;

    int error = git_repository_open(&gitRepository, mPath.toUtf8().constData());

    if (error == 0) {
        git_oid masterOid, originMasterOid;

        error = git_reference_name_to_id(&masterOid, gitRepository,
                                         "refs/heads/master");
        if (error == 0) {
            error = git_reference_name_to_id(&originMasterOid, gitRepository,
                                             "refs/remotes/origin/master");
            if (error == 0) {
                size_t ahead = 0;
                size_t behind = 0;

                error = git_graph_ahead_behind(&ahead, &behind, gitRepository,
                                               &masterOid, &originMasterOid);
                if (error == 0) {
                    status = QString("%1 %2").arg((ahead  ? "A" : " "),
                                                  (behind ? "B" : " "));
                }
            }
        }
    }

    if (error) {
        const git_error *gitError = giterr_last();
        emit warning(gitError?
                         tr("Error %1: %2.").arg(QString::number(gitError->klass),
                                                 Core::formatMessage(gitError->message)):
                         tr("An error occurred while trying to get the status of %1").arg(mPath));
    }

    git_libgit2_shutdown();

    return status;
}

//==============================================================================

const QString PmrWorkspace::gitStatus(const QString &pPath) const
{
    // Get the status of a file

    auto status = QString("");

    git_libgit2_init();

    git_repository *gitRepository = 0;

    int error = git_repository_open(&gitRepository, mPath.toUtf8().constData());

    if (error == 0) {
        auto repoDir = QDir(mPath);
        auto relativePath = repoDir.relativeFilePath(pPath);

        unsigned int statusFlags = 0;
        error = git_status_file(&statusFlags, gitRepository, relativePath.toUtf8().constData());

        if (error == 0) {
            char istatus = ' ';
            char wstatus = ' ';

            if (statusFlags & GIT_STATUS_INDEX_NEW) istatus = 'A';
            if (statusFlags & GIT_STATUS_INDEX_MODIFIED) istatus = 'M';
            if (statusFlags & GIT_STATUS_INDEX_DELETED) istatus = 'D';
            if (statusFlags & GIT_STATUS_INDEX_RENAMED) istatus = 'R';
            if (statusFlags & GIT_STATUS_INDEX_TYPECHANGE) istatus = 'T';
            if (statusFlags & GIT_STATUS_WT_NEW) {
                if (istatus == ' ') istatus = '?';
                wstatus = '?';
            }
            if (statusFlags & GIT_STATUS_WT_MODIFIED) wstatus = 'M';
            if (statusFlags & GIT_STATUS_WT_DELETED) wstatus = 'D';
            if (statusFlags & GIT_STATUS_WT_RENAMED) wstatus = 'R';
            if (statusFlags & GIT_STATUS_WT_TYPECHANGE) wstatus = 'T';
            if (statusFlags & GIT_STATUS_IGNORED) {
                istatus = '!';
                wstatus = '!';
            }

            status = QString(QChar(istatus)) + wstatus;
        }

        git_repository_free(gitRepository);
    }

    if (error) {
        const git_error *gitError = giterr_last();
        emit warning(gitError?
                         tr("Error %1: %2.").arg(QString::number(gitError->klass),
                                                 Core::formatMessage(gitError->message)):
                         tr("An error occurred while trying to get the status of %1").arg(pPath));
    }

    git_libgit2_shutdown();

    return status;
}

//==============================================================================
//==============================================================================

PmrWorkspaceList::PmrWorkspaceList() :
    QList<PmrWorkspace *>()
{
}

//==============================================================================

void PmrWorkspaceList::add(const QString &pUrl, const QString &pName, QObject *parent)
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

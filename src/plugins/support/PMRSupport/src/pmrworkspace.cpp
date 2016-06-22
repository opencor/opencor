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

#include "git2.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

const QString PmrWorkspace::WorkspacesDirectory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                                + QDir::separator() + "OpenCOR" + QDir::separator() + "Workspaces";


//==============================================================================

PmrWorkspace::PmrWorkspace(QObject *parent) : QObject(parent),
     mDescription(QString()), mName(QString()), mOwner(QString()), mPath(QString()), mUrl(QString())
{
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const QString &pUrl, const QString &pName, QObject *parent) :
    QObject(parent),
    mDescription(QString()), mName(pName), mOwner(QString()),
    mPassword(QString()), mPath(QString()), mUrl(pUrl), mUsername(QString())
{
    // Set name from PMR workspace info
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

void PmrWorkspace::emitProgress(const double &pProgress)
{
    emit progress(pProgress);
}

//==============================================================================
//==============================================================================

class GitCredentials
{
public:
    GitCredentials(const QString &pUsername, const QString &pPassword) :
        mUsername(pUsername), mPassword(pPassword)
    {
    }

    const QString &username(void) const
    {
        return mUsername;
    }

    const QString &password(void) const
    {
        return mPassword;
    }

private:
    const QString mUsername;
    const QString mPassword;
};

//==============================================================================

// See https://libgit2.github.com/libgit2/#HEAD/type/git_remote_callbacks

// If cert verification fails, this is called to let the user make the final
// decision of whether to allow the connection to proceed. Returns 1 to allow
// the connection, 0 to disallow it or a negative value to indicate an error.

static int certificate_check_cb(git_cert *cert, int valid, const char *host, void *payload)
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

// This is called if the remote host requires authentication in order to connect to
// it. Returning GIT_PASSTHROUGH will make libgit2 behave as though this field isn't set.
int PmrWorkspace::transfer_progress_cb(const git_transfer_progress *stats, void *payload)
{
    auto workspace = (PmrWorkspace *)payload;

    workspace->emitProgress((double)(stats->received_objects + stats->indexed_objects)
                            /(2.0*stats->total_objects));
    return 0;
}

static int credentials_cb(git_cred **out, const char *url, const char *username_from_url,
                          unsigned int allowed_types, void *payload)
//==============================================================================

void PmrWorkspace::checkout_progress_cb(const char *path, size_t completed_steps, size_t total_steps,
                                        void *payload)
{
    Q_UNUSED(url)
    Q_UNUSED(username_from_url)
    Q_UNUSED(allowed_types)
    Q_UNUSED(path)

    auto credentials = (GitCredentials *)payload;
    auto workspace = (PmrWorkspace *)payload;

    return git_cred_userpass_plaintext_new(out, credentials->username().toUtf8().constData(),
                                                credentials->password().toUtf8().constData());
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

    auto gitCredentials = GitCredentials(mUsername, mPassword);
    // Track clone and checkout progress

    cloneOptions.fetch_opts.callbacks.transfer_progress = transfer_progress_cb ;
    cloneOptions.fetch_opts.callbacks.payload = (void *)this;
    cloneOptions.checkout_opts.progress_cb = checkout_progress_cb;
    cloneOptions.checkout_opts.progress_payload = (void *)this;
    if (!mUsername.isEmpty()) {
        cloneOptions.fetch_opts.callbacks.credentials = credentials_cb;
        cloneOptions.fetch_opts.callbacks.certificate_check = certificate_check_cb;
        cloneOptions.fetch_opts.callbacks.payload = (void *)&gitCredentials;
    }

    int res = git_clone(&gitRepository, workspaceByteArray.constData(),
                        dirNameByteArray.constData(), &cloneOptions);

    if (res) {
        const git_error *gitError = giterr_last();

        emit warning(gitError?
                         tr("Error %1: %2.").arg(QString::number(gitError->klass),
                                                 Core::formatMessage(gitError->message)):
                         tr("An error occurred while trying to clone the workspace."));
    } else if (gitRepository) {
        git_repository_free(gitRepository);
    }

    git_libgit2_shutdown();

    mPath = pDirName;

    emit workspaceCloned(this);
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

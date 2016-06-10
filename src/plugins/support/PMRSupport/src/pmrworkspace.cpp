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

#include "git2.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWorkspace::PmrWorkspace(QObject *parent) : QObject(parent), mUrl(""), mName("")
{
}

//==============================================================================

PmrWorkspace::PmrWorkspace(const QString &pUrl, const QString &pName, QObject *parent) :
    QObject(parent), mUrl(pUrl), mName(pName)
{
    connect(this, SIGNAL(warning(const QString &)), parent, SIGNAL(warning(const QString &)));
}

//==============================================================================

bool PmrWorkspace::compare(const PmrWorkspace *pFirst, const PmrWorkspace *pSecond)
{
    // Return whether the first workspace is lower than the second one (without
    // worrying about casing)

    return pFirst->name().compare(pSecond->name(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

QString PmrWorkspace::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

QString PmrWorkspace::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

PmrLocalWorkspace *PmrWorkspace::clone(const QString &pDirName)
{
   // Clone the workspace

   git_libgit2_init();

   git_repository *gitRepository = 0;
   QByteArray workspaceByteArray = mUrl.toUtf8();
   QByteArray dirNameByteArray = pDirName.toUtf8();

   int res = git_clone(&gitRepository, workspaceByteArray.constData(),
                       dirNameByteArray.constData(), 0);

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

   // Return our newly cloned local workspace

   return new PmrLocalWorkspace(pDirName, mName, parent());
}

//==============================================================================

PmrLocalWorkspace::PmrLocalWorkspace(QObject *parent) : PmrWorkspace(parent)
{
}

//==============================================================================

PmrLocalWorkspace::PmrLocalWorkspace(const QString &pPath,
                                     const QString &pName,
                                     QObject *parent) : PmrWorkspace("", pName, parent)
{
    Q_UNUSED(pPath);

    // Set pUrl from remote.origin.url
}

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

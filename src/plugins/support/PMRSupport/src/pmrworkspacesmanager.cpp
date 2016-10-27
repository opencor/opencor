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
// PMR workspaces manager
//==============================================================================

#include "pmrworkspacesmanager.h"
#include "corecliutils.h"

//==============================================================================

#include "git2/global.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWorkspacesManager::PmrWorkspacesManager() :
    mUrlWorkspaces(QMap<QString, PmrWorkspace *>())
{
    // Initialise libgit2

    git_libgit2_init();
}

//==============================================================================

PmrWorkspacesManager::~PmrWorkspacesManager()
{
    // Shutdown libgit2

    git_libgit2_shutdown();
}

//==============================================================================

PmrWorkspacesManager * PmrWorkspacesManager::instance()
{
    // Return the 'global' instance of our workspaces manager class

    static PmrWorkspacesManager instance;

    return static_cast<PmrWorkspacesManager *>(Core::globalInstance("OpenCOR::PMRSupport::PmrWorkspacesManager::instance()",
                                                                    &instance));
}

//==============================================================================

int PmrWorkspacesManager::count() const
{
    // Return our number of workspaces

    return mUrlWorkspaces.count();
}

//==============================================================================

PmrWorkspaces PmrWorkspacesManager::workspaces() const
{
    // Return our workspaces

    PmrWorkspaces res = PmrWorkspaces();

    foreach (PmrWorkspace *workspace, mUrlWorkspaces.values())
        res << workspace;

    return res;
}

//==============================================================================

bool PmrWorkspacesManager::hasWorkspace(const QString &pUrl) const
{
    // Return whether we hold the workspace which URL is given

    return mUrlWorkspaces.contains(pUrl);
}

//==============================================================================

PmrWorkspace * PmrWorkspacesManager::workspace(const QString &pUrl) const
{
    // Return the workspace, if any, with the given URL

    return mUrlWorkspaces.value(pUrl);
}

//==============================================================================

void PmrWorkspacesManager::addWorkspace(PmrWorkspace *pWorkspace)
{
    // Keep track of the given workspace, if any

    if (pWorkspace)
        mUrlWorkspaces.insert(pWorkspace->url(), pWorkspace);
}

//==============================================================================

void PmrWorkspacesManager::clearWorkspaces()
{
    // Stop tracking all of our workspaces

    foreach (PmrWorkspace * workspace, mUrlWorkspaces.values())
        workspace->close();

    mUrlWorkspaces.clear();
}

//==============================================================================

void PmrWorkspacesManager::emitWorkspaceCloned(PmrWorkspace *pWorkspace)
{
    // This tells the PMR Workspaces plugin that a workspace has been cloned

    emit workspaceCloned(pWorkspace);
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

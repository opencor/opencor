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
// PMR workspace manager
//==============================================================================

#include "pmrworkspacemanager.h"
#include "corecliutils.h"

//==============================================================================

#include "git2/global.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWorkspaceManager::PmrWorkspaceManager() :
    mUrlWorkspaces(QMap<QString, PmrWorkspace *>())
{
    // Initialise libgit2

    git_libgit2_init();
}

//==============================================================================

PmrWorkspaceManager::~PmrWorkspaceManager()
{
    // Shutdown libgit2

    git_libgit2_shutdown();
}

//==============================================================================

PmrWorkspaceManager * PmrWorkspaceManager::instance()
{
    // Return the 'global' instance of our workspace manager class

    static PmrWorkspaceManager instance;

    return static_cast<PmrWorkspaceManager *>(Core::globalInstance("OpenCOR::PMRSupport::PmrWorkspaceManager::instance()",
                                                                    &instance));
}

//==============================================================================

int PmrWorkspaceManager::count() const
{
    // Return our number of workspaces

    return mUrlWorkspaces.count();
}

//==============================================================================

PmrWorkspaces PmrWorkspaceManager::workspaces() const
{
    // Return our workspaces

    PmrWorkspaces res = PmrWorkspaces();

    foreach (PmrWorkspace *workspace, mUrlWorkspaces.values())
        res << workspace;

    return res;
}

//==============================================================================

bool PmrWorkspaceManager::hasWorkspace(const QString &pUrl) const
{
    // Return whether we hold the workspace which URL is given

    return mUrlWorkspaces.contains(pUrl);
}

//==============================================================================

PmrWorkspace * PmrWorkspaceManager::workspace(const QString &pUrl) const
{
    // Return the workspace, if any, with the given URL

    return mUrlWorkspaces.value(pUrl);
}

//==============================================================================

void PmrWorkspaceManager::addWorkspace(PmrWorkspace *pWorkspace)
{
    // Keep track of the given workspace, if any

    if (pWorkspace)
        mUrlWorkspaces.insert(pWorkspace->url(), pWorkspace);
}

//==============================================================================

void PmrWorkspaceManager::removeWorkspace(PmrWorkspace *pWorkspace)
{
    // Stop tracking the given workspace, if any

    if (pWorkspace) {
        pWorkspace->close();

        mUrlWorkspaces.remove(pWorkspace->url());
    }
}

//==============================================================================

void PmrWorkspaceManager::clearWorkspaces()
{
    // Stop tracking all of our workspaces

    foreach (PmrWorkspace *workspace, mUrlWorkspaces.values())
        workspace->close();

    mUrlWorkspaces.clear();
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

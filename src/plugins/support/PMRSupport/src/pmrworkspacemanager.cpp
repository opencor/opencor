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
// PMR workspace manager
//==============================================================================

#include "corecliutils.h"
#include "pmrworkspacemanager.h"

//==============================================================================

#include "libgit2begin.h"
    #include "git2/global.h"
#include "libgit2end.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWorkspaceManager::PmrWorkspaceManager()
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

    for (auto workspace : mUrlWorkspaces) {
        res << workspace;
    }

    return res;
}

//==============================================================================

bool PmrWorkspaceManager::hasWorkspaces() const
{
    // Return whether we hold some workspaces

    return !workspaces().isEmpty();
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

    if (pWorkspace != nullptr) {
        mUrlWorkspaces.insert(pWorkspace->url(), pWorkspace);
    }
}

//==============================================================================

void PmrWorkspaceManager::removeWorkspace(PmrWorkspace *pWorkspace)
{
    // Stop tracking the given workspace, if any

    if (pWorkspace != nullptr) {
        pWorkspace->close();

        mUrlWorkspaces.remove(pWorkspace->url());
    }
}

//==============================================================================

void PmrWorkspaceManager::clearWorkspaces()
{
    // Stop tracking all of our workspaces

    for (auto workspace : mUrlWorkspaces) {
        workspace->close();
    }

    mUrlWorkspaces.clear();
}

//==============================================================================

} // namespace PMRSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

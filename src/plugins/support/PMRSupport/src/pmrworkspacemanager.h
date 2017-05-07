/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// PMR workspace manager
//==============================================================================

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"
#include "pmrworkspace.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PMRSUPPORT_EXPORT PmrWorkspaceManager : public QObject
{
    Q_OBJECT

public:
    explicit PmrWorkspaceManager();
    ~PmrWorkspaceManager();

    static PmrWorkspaceManager * instance();

    int count() const;

    PmrWorkspaces workspaces() const;

    bool hasWorkspaces() const;
    bool hasWorkspace(const QString &pUrl) const;
    PmrWorkspace * workspace(const QString &pUrl) const;

    void addWorkspace(PmrWorkspace *pWorkspace);
    void removeWorkspace(PmrWorkspace *pWorkspace);
    void clearWorkspaces();

private:
    QMap<QString, PmrWorkspace *> mUrlWorkspaces;

signals:
    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceUncloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceSynchronized(PMRSupport::PmrWorkspace *pWorkspace);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

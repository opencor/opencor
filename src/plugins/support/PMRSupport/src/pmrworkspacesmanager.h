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

class PMRSUPPORT_EXPORT PmrWorkspacesManager : public QObject
{
    Q_OBJECT

public:
    explicit PmrWorkspacesManager();
    ~PmrWorkspacesManager();

    static PmrWorkspacesManager * instance();

    void emitWorkspaceCloned(PmrWorkspace *pWorkspace);

    void addWorkspace(PmrWorkspace *pWorkspace);
    void clearWorkspaces();
    size_t count() const;

    bool hasWorkspace(const QString &pUrl) const;
    PmrWorkspace * workspace(const QString &pUrl) const;
    PmrWorkspaces workspaces() const;

private:
    QMap<QString, PmrWorkspace *> mUrlWorkspaces;

signals:
    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================


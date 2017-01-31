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
// PMR Workspaces window item
//==============================================================================

#pragma once

//==============================================================================

#include <QStandardItem>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace PMRSupport {
    class PmrWorkspace;
    class PmrWorkspaceFileNode;
}   // namespace PMRSupport

//==============================================================================

namespace PMRWorkspacesWindow {

//==============================================================================

class PmrWorkspacesWindowItem : public QStandardItem
{
public:
    enum Type {
        OwnedWorkspace = QStandardItem::UserType,
        Workspace      = QStandardItem::UserType+1,
        Folder         = QStandardItem::UserType+2,
        File           = QStandardItem::UserType+3
    };

    explicit PmrWorkspacesWindowItem(const Type &pType,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     const QIcon &pCollapsedIcon,
                                     const QIcon &pExpandedIcon);
    explicit PmrWorkspacesWindowItem(const Type &pType,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                     const QIcon &pCollapsedIcon,
                                     const QIcon &pExpandedIcon);
    explicit PmrWorkspacesWindowItem(const Type &pType,
                                     PMRSupport::PmrWorkspace *pWorkspace,
                                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                     const QIcon &pIcon);

    virtual int type() const;

    PMRSupport::PmrWorkspace * workspace() const;
    PMRSupport::PmrWorkspaceFileNode * fileNode() const;

    QIcon collapsedIcon() const;
    void setCollapsedIcon(const QIcon &pCollapsedIcon);

    QIcon expandedIcon() const;
    void setExpandedIcon(const QIcon &pExpandedIcon);

    QString url() const;
    QString path() const;
    QString fileName() const;

private:
    Type mType;

    PMRSupport::PmrWorkspace *mWorkspace;
    PMRSupport::PmrWorkspaceFileNode *mFileNode;

    QIcon mCollapsedIcon;
    QIcon mExpandedIcon;

    void constructor(const Type &pType, PMRSupport::PmrWorkspace *pWorkspace,
                     PMRSupport::PmrWorkspaceFileNode *pFileNode,
                     const QIcon &pCollapsedIcon, const QIcon &pExpandedIcon);
};

//==============================================================================

typedef QList<PmrWorkspacesWindowItem *> PmrWorkspacesWindowItems;

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

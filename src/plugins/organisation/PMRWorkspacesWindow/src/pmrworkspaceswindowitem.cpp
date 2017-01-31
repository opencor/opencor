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

#include "pmrworkspace.h"
#include "pmrworkspaceswindowitem.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

void PmrWorkspacesWindowItem::constructor(const Type &pType,
                                          PMRSupport::PmrWorkspace *pWorkspace,
                                          PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                          const QIcon &pCollapsedIcon,
                                          const QIcon &pExpandedIcon)
{
    // Some initialisations

    mType = pType;
    mWorkspace = pWorkspace;
    mFileNode = pFileNode;
    mCollapsedIcon = pCollapsedIcon;
    mExpandedIcon = pExpandedIcon;

    // Customise ourselves

    setToolTip(text());
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 const QIcon &pCollapsedIcon,
                                                 const QIcon &pExpandedIcon) :
    QStandardItem(pCollapsedIcon, pWorkspace->name())
{
    // Construct our object

    constructor(pType, pWorkspace, 0, pCollapsedIcon, pExpandedIcon);
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                 const QIcon &pCollapsedIcon,
                                                 const QIcon &pExpandedIcon) :
    QStandardItem(pCollapsedIcon, pFileNode->name())
{
    // Construct our object

    constructor(pType, pWorkspace, pFileNode, pCollapsedIcon, pExpandedIcon);
}

//==============================================================================

PmrWorkspacesWindowItem::PmrWorkspacesWindowItem(const Type &pType,
                                                 PMRSupport::PmrWorkspace *pWorkspace,
                                                 PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                 const QIcon &pIcon) :
    QStandardItem(pIcon, pFileNode->name())
{
    // Construct our object

    constructor(pType, pWorkspace, pFileNode, QIcon(), QIcon());
}

//==============================================================================

int PmrWorkspacesWindowItem::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

PMRSupport::PmrWorkspace * PmrWorkspacesWindowItem::workspace() const
{
    // Return our workspace

    return mWorkspace;
}

//==============================================================================

PMRSupport::PmrWorkspaceFileNode * PmrWorkspacesWindowItem::fileNode() const
{
    // Return our file node

    return mFileNode;
}

//==============================================================================

QIcon PmrWorkspacesWindowItem::collapsedIcon() const
{
    // Return our collapsed icon

    return mCollapsedIcon;
}

//==============================================================================

void PmrWorkspacesWindowItem::setCollapsedIcon(const QIcon &pCollapsedIcon)
{
    // Set our collapsed icon

    mCollapsedIcon = pCollapsedIcon;
}

//==============================================================================

QIcon PmrWorkspacesWindowItem::expandedIcon() const
{
    // Return our expanded icon

    return mExpandedIcon;
}

//==============================================================================

void PmrWorkspacesWindowItem::setExpandedIcon(const QIcon &pExpandedIcon)
{
    // Set our expanded icon

    mExpandedIcon = pExpandedIcon;
}

//==============================================================================

QString PmrWorkspacesWindowItem::url() const
{
    // Return our URL

    return mWorkspace?mWorkspace->url():QString();
}

//==============================================================================

QString PmrWorkspacesWindowItem::path() const
{
    // Return our path

    return mWorkspace?mWorkspace->path():QString();
}

//==============================================================================

QString PmrWorkspacesWindowItem::fileName() const
{
    // Return our file name

    return mFileNode?mFileNode->path():QString();
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

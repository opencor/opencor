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
// PMR workspace file node
//==============================================================================

#include "pmrworkspacefilenode.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWorkspaceFileNode::PmrWorkspaceFileNode(const QString &pName,
                                           const QString &pPath,
                                           const CharPair &pStatus,
                                           PmrWorkspaceFileNode *pParent) :
    QObject(pParent)
{
    // Some initialisations

    mParent = pParent;

    mName = pName;
    mPath = pPath;

    mStatus = pStatus;

    mChildren = PmrWorkspaceFileNodes();
}

//==============================================================================

PmrWorkspaceFileNode::PmrWorkspaceFileNode(PmrWorkspaceFileNode *pParent) :
    PmrWorkspaceFileNode(QString(), QString(), CharPair(), pParent)
{
}

//==============================================================================

PmrWorkspaceFileNode * PmrWorkspaceFileNode::parent() const
{
    // Return our parent

    return mParent;
}

//==============================================================================

QString PmrWorkspaceFileNode::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

QString PmrWorkspaceFileNode::path() const
{
    // Return our path

    return mPath;
}

//==============================================================================

void PmrWorkspaceFileNode::setPath(const QString &pPath)
{
    // Set our path

    mPath = pPath;
}

//==============================================================================

CharPair PmrWorkspaceFileNode::status() const
{
    // Return our status

    return mStatus;
}

//==============================================================================

void PmrWorkspaceFileNode::setStatus(const CharPair &pStatus)
{
    // Set our status

    mStatus = pStatus;
}

//==============================================================================

bool PmrWorkspaceFileNode::hasChildren() const
{
    // Return whether we have children

    return mChildren.size();
}

//==============================================================================

PmrWorkspaceFileNodes PmrWorkspaceFileNode::children() const
{
    // Return our children

    return mChildren;
}

//==============================================================================

PmrWorkspaceFileNode * PmrWorkspaceFileNode::addChild(const QString &pName,
                                                      const CharPair &pStatus)
{
    // Check whether we already have the given child and, if so, return it

    QString childPath = path()+"/"+pName;

    foreach (PmrWorkspaceFileNode *child, mChildren) {
        if (!child->path().compare(childPath)) {
            // This is the child we are trying to add, so update its status and
            // return it

            child->setStatus(pStatus);

            return child;
        }
    }

    // We don't already have the given child, so add it to ourselves and return
    // it

    PmrWorkspaceFileNode *res = new PmrWorkspaceFileNode(pName, childPath,
                                                         pStatus, this);

    mChildren << res;

    return res;
}

//==============================================================================

bool PmrWorkspaceFileNode::removeChild(PmrWorkspaceFileNode *pChild)
{
    // Remove the given child, if possible

    if (mChildren.contains(pChild)) {
        delete pChild;

        mChildren.removeOne(pChild);

        return true;
    } else {
        return false;
    }
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

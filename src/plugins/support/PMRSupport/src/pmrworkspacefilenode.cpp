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
// PMR workspace file node
//==============================================================================

#include "pmrworkspacefilenode.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

void PmrWorkspaceFileNode::constructor(const QString &pName,
                                       const QString &pPath,
                                       const CharPair &pStatus)
{
    // Some initialisations

    mName = pName;
    mPath = pPath;

    mStatus = pStatus;

    mChildren = PmrWorkspaceFileNodes();
}

//==============================================================================

PmrWorkspaceFileNode::PmrWorkspaceFileNode(const QString &pName,
                                           const QString &pPath,
                                           const CharPair &pStatus,
                                           QObject *pParent) :
    QObject(pParent)
{
    // Construct ourselves

    constructor(pName, pPath, pStatus);
}

//==============================================================================

PmrWorkspaceFileNode::PmrWorkspaceFileNode(QObject *pParent) :
    QObject(pParent)
{
    // Construct ourselves

    constructor(QString(), QString(), CharPair());
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

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

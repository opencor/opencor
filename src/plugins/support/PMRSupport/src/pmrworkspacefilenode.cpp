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

PmrWorkspaceFileNode::PmrWorkspaceFileNode(const QString &pShortName,
                                           const QString &pFullName,
                                           const CharPair &pStatus,
                                           PmrWorkspaceFileNode *pParent) :
    QObject(pParent),
    mShortName(pShortName),
    mFullName(pFullName),
    mStatus(pStatus),
    mChildren(PmrWorkspaceFileNodes())
{
}

//==============================================================================

QString PmrWorkspaceFileNode::shortName() const
{
    // Return our short name

    return mShortName;
}
//==============================================================================

QString PmrWorkspaceFileNode::fullName() const
{
    // Return our full name

    return mFullName;
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

PmrWorkspaceFileNode * PmrWorkspaceFileNode::addChild(const QString &pShortName,
                                                      const CharPair &pStatus)
{
    // Add the given child to ourselves

    PmrWorkspaceFileNode *res = new PmrWorkspaceFileNode(pShortName,
                                                         fullName()+"/"+pShortName,
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

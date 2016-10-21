/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR workspace file node
//==============================================================================

#include "pmrworkspacefilenode.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrWorkspaceFileNode::PmrWorkspaceFileNode(const QString &pShortName, const QString &pFullName,
                                           const QPair<QChar, QChar> &pStatus,
                                           PmrWorkspaceFileNode *parent) :
    QObject(parent), mShortName(pShortName), mFullName(pFullName),
    mStatus(pStatus), mChildren(QList<PmrWorkspaceFileNode *>())
{

}

//==============================================================================

PmrWorkspaceFileNode *PmrWorkspaceFileNode::addChild(const QString &pShortName, const QPair<QChar, QChar> &pStatus)
{
    auto newNode = new PmrWorkspaceFileNode(pShortName, fullName() + "/" + pShortName, pStatus, this);
    mChildren.append(newNode);
    return newNode;
}

//==============================================================================

void PmrWorkspaceFileNode::setStatus(const QPair<QChar, QChar> &pStatus)
{
    mStatus = pStatus;
}

//==============================================================================

const QString &PmrWorkspaceFileNode::shortName(void) const
{
    return mShortName;
}
//==============================================================================

const QString &PmrWorkspaceFileNode::fullName(void) const
{
    return mFullName;
}

//==============================================================================

const QPair<QChar, QChar> &PmrWorkspaceFileNode::status(void) const
{
    return mStatus;
}

//==============================================================================

QList<PmrWorkspaceFileNode *> PmrWorkspaceFileNode::children(void) const
{
    return mChildren;
}

//==============================================================================

bool PmrWorkspaceFileNode::hasChildren(void) const
{
    return (mChildren.size() > 0);
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================


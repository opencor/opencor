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

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"

//==============================================================================

#include <QList>
#include <QObject>
#include <QPair>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

typedef QPair<QChar, QChar> CharPair;

//==============================================================================

class PmrWorkspaceFileNode;

//==============================================================================

typedef QList<PmrWorkspaceFileNode *> PmrWorkspaceFileNodes;

//==============================================================================

class PMRSUPPORT_EXPORT PmrWorkspaceFileNode : public QObject
{
    Q_OBJECT

public:
    explicit PmrWorkspaceFileNode(const QString &pShortName,
                                  const QString &pFullName,
                                  const CharPair &pStatus = CharPair(),
                                  PmrWorkspaceFileNode *pParent = 0);

    PmrWorkspaceFileNode * addChild(const QString &pName,
                                    const CharPair &pStatus = CharPair());

    QString shortName() const;
    QString fullName() const;
    CharPair status() const;

    bool hasChildren() const;
    PmrWorkspaceFileNodes children() const;

    void setStatus(const CharPair &pStatus);

private:
    QString mShortName;
    QString mFullName;
    CharPair mStatus;

    PmrWorkspaceFileNodes mChildren;
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

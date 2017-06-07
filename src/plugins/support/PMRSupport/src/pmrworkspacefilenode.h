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

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrWorkspaceFileNode;

//==============================================================================

typedef QPair<QChar, QChar> CharPair;
typedef QList<PmrWorkspaceFileNode *> PmrWorkspaceFileNodes;

//==============================================================================

class PMRSUPPORT_EXPORT PmrWorkspaceFileNode : public QObject
{
    Q_OBJECT

public:
    explicit PmrWorkspaceFileNode(const QString &pName, const QString &pPath,
                                  const CharPair &pStatus,
                                  PmrWorkspaceFileNode *pParent);
    explicit PmrWorkspaceFileNode(PmrWorkspaceFileNode *pParent);

    PmrWorkspaceFileNode * parent() const;

    QString name() const;

    QString path() const;
    void setPath(const QString &pPath);

    CharPair status() const;
    void setStatus(const CharPair &pStatus);

    bool hasChildren() const;
    PmrWorkspaceFileNodes children() const;
    PmrWorkspaceFileNode * addChild(const QString &pName,
                                    const CharPair &pStatus = CharPair());
    bool removeChild(PmrWorkspaceFileNode *pChild);

private:
    PmrWorkspaceFileNode *mParent;
    QString mName;
    QString mPath;
    CharPair mStatus;

    PmrWorkspaceFileNodes mChildren;
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

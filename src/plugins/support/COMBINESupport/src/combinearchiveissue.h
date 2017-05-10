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
// COMBINE archive issue
//==============================================================================

#pragma once

//==============================================================================

#include "combinesupportglobal.h"

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

class COMBINESUPPORT_EXPORT CombineArchiveIssue
{
public:
    enum Type {
        Information,
        Error,
        Warning,
        Fatal
    };

    explicit CombineArchiveIssue(const Type &pType, const QString &pMessage);

    Type type() const;
    QString message() const;

private:
    Type mType;
    QString mMessage;
};

//==============================================================================

typedef QList<CombineArchiveIssue> CombineArchiveIssues;

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// CellML file issue
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileIssue
{
public:
    enum class Type {
        Error,
        Warning
    };

    explicit CellmlFileIssue(Type pType, int pLine, int pColumn,
                             const QString &pMessage, const QString &pFileName,
                             const QString &pFileInfo);
    explicit CellmlFileIssue(Type pType, const QString &pMessage);

    bool operator==(const CellmlFileIssue &pIssue) const;

    static bool compare(const CellmlFileIssue &pIssue1,
                        const CellmlFileIssue &pIssue2);

    Type type() const;
    int line() const;
    int column() const;
    QString message() const;
    QString formattedMessage() const;
    QString fileName() const;
    QString fileInfo() const;

private:
    Type mType;
    int mLine;
    int mColumn;
    QString mMessage;
    QString mFileName;
    QString mFileInfo;
};

//==============================================================================

using CellmlFileIssues = QList<CellmlFileIssue>;

//==============================================================================

} // namespace CellMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

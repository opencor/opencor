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
// SED-ML file issue
//==============================================================================

#pragma once

//==============================================================================

#include "sedmlsupportglobal.h"

//==============================================================================

#include <QList>
#ifdef Q_OS_WIN
    #include <QSet>
    #include <QVector>
#endif

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

class SEDMLSUPPORT_EXPORT SedmlFileIssue
{
public:
    enum class Type {
        Information,
        Error,
        Warning,
        Fatal
    };

    explicit SedmlFileIssue(Type pType, int pLine, int pColumn,
                            const QString &pMessage);
    explicit SedmlFileIssue(Type pType, const QString &pMessage);

    bool operator==(const SedmlFileIssue &pIssue) const;

    Type type() const;
    int line() const;
    int column() const;
    QString message() const;

private:
    Type mType;
    int mLine;
    int mColumn;
    QString mMessage;
};

//==============================================================================

using SedmlFileIssues = QList<SedmlFileIssue>;

//==============================================================================

} // namespace SEDMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// CellML file issue
//==============================================================================

#include "cellmlfileissue.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileIssue::CellmlFileIssue(const Type &pType, const int &pLine,
                                 const int &pColumn, const QString &pMessage,
                                 const QString &pImportedFile)
{
    // Some initialisations

    mType = pType;

    mLine = pLine;
    mColumn = pColumn;

    mMessage = pMessage;

    mImportedFile = pImportedFile;
}

//==============================================================================

CellmlFileIssue::CellmlFileIssue(const Type &pType, const QString &pMessage) :
    CellmlFileIssue(pType, 0, 0, pMessage, QString())
{
}

//==============================================================================

bool CellmlFileIssue::compare(const CellmlFileIssue &pIssue1,
                              const CellmlFileIssue &pIssue2)
{
    // Determine which of the two issues should be first

    if (pIssue1.line() == pIssue2.line()) {
        if (pIssue1.column() == pIssue2.column()) {
            if (pIssue1.type() == pIssue2.type())
                return pIssue1.message().compare(pIssue2.message(), Qt::CaseInsensitive) < 0;
            else
                return pIssue1.type() < pIssue2.type();
        } else {
            return pIssue1.column() < pIssue2.column();
        }
    } else {
        return pIssue1.line() < pIssue2.line();
    }
}

//==============================================================================

CellmlFileIssue::Type CellmlFileIssue::type() const
{
    // Return the issue's type

    return mType;
}

//==============================================================================

int CellmlFileIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

int CellmlFileIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString CellmlFileIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

QString CellmlFileIssue::formattedMessage() const
{
    // Return the issue's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage[0].toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

QString CellmlFileIssue::importedFile() const
{
    // Return the issue's imported file

    return mImportedFile;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

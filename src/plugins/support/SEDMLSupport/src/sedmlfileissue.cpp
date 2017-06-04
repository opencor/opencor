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
// SED-ML file issue
//==============================================================================

#include "sedmlfileissue.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

SedmlFileIssue::SedmlFileIssue(const Type &pType, const int &pLine,
                               const int &pColumn, const QString &pMessage)
{
    // Some initialisations

    mType = pType;

    mLine = pLine;
    mColumn = pColumn;

    mMessage = pMessage;
}

//==============================================================================

SedmlFileIssue::SedmlFileIssue(const Type &pType, const QString &pMessage) :
    SedmlFileIssue(pType, 0, 0, pMessage)
{
}

//==============================================================================

SedmlFileIssue::Type SedmlFileIssue::type() const
{
    // Return the issue's type

    return mType;
}

//==============================================================================

int SedmlFileIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

int SedmlFileIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString SedmlFileIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// SED-ML file issue
//==============================================================================

#include "sedmlfileissue.h"

//==============================================================================

#include <Qt>

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

void SedmlFileIssue::constructor(const Type &pType, const int &pLine,
                                 const int &pColumn, const QString &pMessage)
{
    // Some initialisations

    mType = pType;

    mLine = pLine;
    mColumn = pColumn;

    mMessage = pMessage;
}

//==============================================================================

SedmlFileIssue::SedmlFileIssue(const Type &pType, const int &pLine,
                               const int &pColumn, const QString &pMessage)
{
    // Construct our object

    constructor(pType, pLine, pColumn, pMessage);
}

//==============================================================================

SedmlFileIssue::SedmlFileIssue(const Type &pType, const QString &pMessage)
{
    // Construct our object

    constructor(pType, 0, 0, pMessage);
}

//==============================================================================

bool SedmlFileIssue::operator<(const SedmlFileIssue &pIssue) const
{
    // Return whether the current issue is lower than the given one

    return (mLine < pIssue.line())?
               true:
               (mLine > pIssue.line())?
                   false:
                   (mColumn < pIssue.column())?
                       true:
                       (mColumn > pIssue.column())?
                           false:
                            (mType < pIssue.type())?
                                true:
                                (mType > pIssue.type())?
                                    false:
                                    mMessage.compare(pIssue.message(), Qt::CaseInsensitive) < 0;
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

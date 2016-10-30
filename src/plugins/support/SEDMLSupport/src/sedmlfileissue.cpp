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

void SedmlFileIssues::sort()
{
    // Sort our issues

    std::sort(begin(), end(), compare);
}

//==============================================================================

bool SedmlFileIssues::compare(SedmlFileIssue *pIssue1, SedmlFileIssue *pIssue2)
{
    // Determine which of the two issues should be first

    if (pIssue1->line() == pIssue2->line()) {
        if (pIssue1->column() == pIssue2->column()) {
            if (pIssue1->type() == pIssue2->type())
                return pIssue1->message().compare(pIssue2->message(), Qt::CaseInsensitive) < 0;
            else
                return pIssue1->type() < pIssue2->type();
        } else {
            return pIssue1->column() < pIssue2->column();
        }
    } else {
        return pIssue1->line() < pIssue2->line();
    }
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

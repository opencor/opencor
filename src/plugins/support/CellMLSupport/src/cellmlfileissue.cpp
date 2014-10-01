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
// CellML file issue
//==============================================================================

#include "cellmlfileissue.h"

//==============================================================================

#include <Qt>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

void CellmlFileIssue::constructor(const Type &pType, const int &pLine,
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

CellmlFileIssue::CellmlFileIssue(const Type &pType, const int &pLine,
                                 const int &pColumn, const QString &pMessage,
                                 const QString &pImportedFile)
{
    // Construct our object

    constructor(pType, pLine, pColumn, pMessage, pImportedFile);
}

//==============================================================================

CellmlFileIssue::CellmlFileIssue(const Type &pType, const QString &pMessage)
{
    // Construct our object

    constructor(pType, 0, 0, pMessage, QString());
}

//==============================================================================

bool CellmlFileIssue::operator<(const CellmlFileIssue &pIssue) const
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

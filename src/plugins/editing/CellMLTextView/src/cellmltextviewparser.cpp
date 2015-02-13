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
// Parser for the CellML text format
//==============================================================================

#include "cellmltextviewparser.h"
#include "cellmltextviewscanner.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewParserError::CellmlTextViewParserError(const int &pLine,
                                                     const int &pColumn,
                                                     const QString &pMessage) :
    mLine(pLine),
    mColumn(pColumn),
    mMessage(pMessage)
{
}

//==============================================================================

int CellmlTextViewParserError::line() const
{
    // Return our line number

    return mLine;
}

//==============================================================================

int CellmlTextViewParserError::column() const
{
    // Return our column number

    return mColumn;
}

//==============================================================================

QString CellmlTextViewParserError::message() const
{
    // Return our message

    return mMessage;
}

//==============================================================================

CellmlTextViewParser::CellmlTextViewParser() :
    mScanner(new CellmlTextViewScanner()),
    mDomDocument(QDomDocument()),
    mErrors(CellmlTextViewParserErrors())
{
}

//==============================================================================

CellmlTextViewParser::~CellmlTextViewParser()
{
    // Delete some internal objects

    delete mScanner;
}

//==============================================================================

bool CellmlTextViewParser::execute(const QString &pText)
{
    // Get ready for the parsing

    mScanner->setText(pText);

    mDomDocument = QDomDocument(QString());

    mErrors = CellmlTextViewParserErrors();

    // Look for "def"

    if (mScanner->token().symbol() == CellmlTextViewScannerToken::Def) {
        return true;
    } else {
        addError("'def'");

        return false;
    }
}

//==============================================================================

QDomDocument CellmlTextViewParser::domDocument() const
{
    // Return our DOM document

    return mDomDocument;
}

//==============================================================================

CellmlTextViewParserErrors CellmlTextViewParser::errors() const
{
    // Return our errors

    return mErrors;
}

//==============================================================================

void CellmlTextViewParser::addError(const QString &pExpected)
{
    // Return whether we have error messages

    mErrors << CellmlTextViewParserError(mScanner->token().line(),
                                         mScanner->token().column(),
                                         QObject::tr("%1 is expected, but '%2' was found instead.").arg(pExpected, mScanner->token().string()));
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

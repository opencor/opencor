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
// Scanner for the CellML text format
//==============================================================================

#include "cellmltextviewscanner.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewScannerToken::CellmlTextViewScannerToken() :
    mSymbol(Unknown),
    mLine(0),
    mColumn(0),
    mString(QString())
{
}

//==============================================================================

CellmlTextViewScannerToken::Symbol CellmlTextViewScannerToken::symbol() const
{
    // Return our symbol

    return mSymbol;
}

//==============================================================================

int CellmlTextViewScannerToken::line() const
{
    // Return our line number

    return mLine;
}

//==============================================================================

int CellmlTextViewScannerToken::column() const
{
    // Return our column number

    return mColumn;
}

//==============================================================================

QString CellmlTextViewScannerToken::string() const
{
    // Return our string

    return mString;
}

//==============================================================================

CellmlTextViewScanner::CellmlTextViewScanner() :
    mText(QString()),
    mToken(CellmlTextViewScannerToken())
{
}

//==============================================================================

void CellmlTextViewScanner::setText(const QString &pText)
{
    // Initialise ourselves with the text to scan

    mText = pText;
    mToken = CellmlTextViewScannerToken();
}

//==============================================================================

CellmlTextViewScannerToken CellmlTextViewScanner::token() const
{
    // Return our current token

    return mToken;
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

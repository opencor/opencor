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

#include <limits.h>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewScannerChar::CellmlTextViewScannerChar(const QChar &pValue) :
    mValue(pValue)
{
    static Type types[USHRT_MAX];
    static bool needTypesInitialization = true;

    if (needTypesInitialization) {
        // Default type of a character

        for (ushort i = 0; i < USHRT_MAX; ++i)
            types[i] = Other;

        // Letters

        for (ushort i = QChar('A').unicode(), iMax = QChar('Z').unicode(); i <= iMax; ++i)
           types[i] = Letter;

        for (ushort i = QChar('a').unicode(), iMax = QChar('z').unicode(); i <= iMax; ++i)
           types[i] = Letter;

        // Digits

        for (ushort i = QChar('0').unicode(), iMax = QChar('9').unicode(); i <= iMax; ++i)
           types[i] = Digit;

        // Special characters

        types[QChar(' ').unicode()]  = Space;
        types[QChar('_').unicode()]  = Underscore;
        types[QChar('"').unicode()]  = DoubleQuote;
        types[QChar('\'').unicode()] = Quote;
        types[QChar(',').unicode()]  = Comma;
        types[QChar('=').unicode()]  = Eq;
        types[QChar('<').unicode()]  = Lt;
        types[QChar('>').unicode()]  = Gt;
        types[QChar('+').unicode()]  = Plus;
        types[QChar('-').unicode()]  = Minus;
        types[QChar('*').unicode()]  = Times;
        types[QChar('/').unicode()]  = Divide;
        types[QChar(':').unicode()]  = Colon;
        types[QChar(';').unicode()]  = SemiColon;
        types[QChar('(').unicode()]  = OpeningBracket;
        types[QChar(')').unicode()]  = ClosingBracket;
        types[QChar('{').unicode()]  = OpeningCurlyBracket;
        types[QChar('}').unicode()]  = ClosingCurlyBracket;
        types[QChar('\r').unicode()] = Cr;
        types[QChar('\n').unicode()] = Lf;
        types[QChar().unicode()]     = Eof;

        needTypesInitialization = false;
    }

    mType = types[pValue.unicode()];
}

//==============================================================================

QChar CellmlTextViewScannerChar::value() const
{
    // Return our value

    return mValue;
}

//==============================================================================

CellmlTextViewScannerChar::Type CellmlTextViewScannerChar::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

CellmlTextViewScannerToken::CellmlTextViewScannerToken() :
    mSymbol(Unknown),
    mLine(1),
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

void CellmlTextViewScannerToken::incrementLine()
{
    // Increment our line number and therefore reset our column number

    mColumn = 1;

    ++mLine;
}

//==============================================================================

void CellmlTextViewScannerToken::incrementColumn()
{
    // Increment our column number

    ++mColumn;
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

QString CellmlTextViewScannerToken::symbolAsString(const Symbol &pSymbol)
{
    // Return the given symbol as a string

    switch (pSymbol) {
    case Def:
        return "def";
    default:
        // Unknown

#ifdef QT_DEBUG
        qFatal("FATAL ERROR | %s:%d: a symbol should never be unknown.", __FILE__, __LINE__);
#endif

        return "???";
    }
}

//==============================================================================

CellmlTextViewScanner::CellmlTextViewScanner() :
    mText(QString()),
    mTextPos(0),
    mChar(CellmlTextViewScannerChar()),
    mToken(CellmlTextViewScannerToken())
{
}

//==============================================================================

void CellmlTextViewScanner::setText(const QString &pText)
{
    // Initialise ourselves with the text to scan

    mText = pText;
    mToken = CellmlTextViewScannerToken();

    mTextPos = 0;

    nextChar();
    nextToken();
}

//==============================================================================

CellmlTextViewScannerToken CellmlTextViewScanner::token() const
{
    // Return our current token

    return mToken;
}

//==============================================================================

void CellmlTextViewScanner::nextChar()
{
    // Get the next char in our text

    if (mTextPos == mText.length()) {
        // We are the end of our text

        mChar = CellmlTextViewScannerChar();
    } else {
        mChar = CellmlTextViewScannerChar(mText[mTextPos++]);

        if (mChar.value() == CellmlTextViewScannerChar::Lf)
            mToken.incrementLine();
        else
            mToken.incrementColumn();
    }
}

//==============================================================================

void CellmlTextViewScanner::nextToken()
{
    // Get the next token in our text by first skipping all the spaces and
    // special characters

    while (   (mChar.type() == CellmlTextViewScannerChar::Cr)
           || (mChar.type() == CellmlTextViewScannerChar::Lf)
           || (mChar.type() == CellmlTextViewScannerChar::Space)) {
        nextChar();
    }
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

CellmlTextViewScanner::CellmlTextViewScanner() :
    mText(QString()),
    mChar(0),
    mCharType(Eof),
    mTokenType(Unknown),
    mTokenLine(1),
    mTokenColumn(0),
    mTokenString(QString())
{
}

//==============================================================================

void CellmlTextViewScanner::setText(const QString &pText)
{
    // Initialise ourselves with the text to scan

    mText = pText;

    mChar = mText.constData();
    mCharType = Eof;

    mTokenType = Unknown;
    mTokenLine = 1;
    mTokenColumn = 0;
    mTokenString = QString();

    nextChar();
    nextToken();
}

//==============================================================================

CellmlTextViewScanner::TokenType CellmlTextViewScanner::tokenType() const
{
    // Return our token type

    return mTokenType;
}

//==============================================================================

int CellmlTextViewScanner::tokenLine() const
{
    // Return our token line

    return mTokenLine;
}

//==============================================================================

int CellmlTextViewScanner::tokenColumn() const
{
    // Return our token column

    return mTokenColumn;
}

//==============================================================================

QString CellmlTextViewScanner::tokenString() const
{
    // Return our token string

    return mTokenString;
}

//==============================================================================

QString CellmlTextViewScanner::tokenTypeAsString(const TokenType &pTokenType)
{
    // Return the given token type as a string

    switch (pTokenType) {
    case Def:
        return "def";
    default:
        // Unknown

#ifdef QT_DEBUG
        qFatal("FATAL ERROR | %s:%d: a token type should never be unknown.", __FILE__, __LINE__);
#endif

        return "???";
    }
}

//==============================================================================

void CellmlTextViewScanner::nextChar()
{
    // Determine the type of our 'next' character

    switch (mChar->unicode()) {
    case 0:
        mCharType = Eof;

        break;
    case 9:
        mCharType = Tab;

        break;
    case 10:
        mCharType = Lf;

        break;
    case 13:
        mCharType = Cr;

        break;
    case 32:
        mCharType = Space;

        break;
    case 34:
        mCharType = DoubleQuote;

        break;
    case 39:
        mCharType = Quote;

        break;
    case 40:
        mCharType = OpeningBracket;

        break;
    case 41:
        mCharType = ClosingBracket;

        break;
    case 42:
        mCharType = Times;

        break;
    case 43:
        mCharType = Plus;

        break;
    case 44:
        mCharType = Comma;

        break;
    case 45:
        mCharType = Minus;

        break;
    case 47:
        mCharType = Divide;

        break;
    case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55:
    case 56: case 57:
        mCharType = Digit;

        break;
    case 58:
        mCharType = Colon;

        break;
    case 59:
        mCharType = SemiColon;

        break;
    case 60:
        mCharType = Lt;

        break;
    case 61:
        mCharType = Eq;

        break;
    case 62:
        mCharType = Gt;

        break;
    case 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72:
    case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80:
    case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88:
    case 89: case 90:
        mCharType = Letter;

        break;
    case 95:
        mCharType = Underscore;

        break;
    case 97: case 98: case 99: case 100: case 101: case 102: case 103:
    case 104: case 105: case 106: case 107: case 108: case 109: case 110:
    case 111: case 112: case 113: case 114: case 115: case 116: case 117:
    case 118: case 119: case 120: case 121: case 122:
        mCharType = Letter;

        break;
    case 123:
        mCharType = OpeningCurlyBracket;

        break;
    case 125:
        mCharType = ClosingCurlyBracket;

        break;
    default:
        mCharType = Other;
    }

    // Update our token line and/or column numbers

    if (mCharType == Lf) {
        mTokenColumn = 1;

        ++mTokenLine;
    } else {
        ++mTokenColumn;
    }

    // Move to the next character

    ++mChar;
}

//==============================================================================

void CellmlTextViewScanner::nextToken()
{
    // Get the next token in our text by first skipping all the spaces and
    // special characters

    while (   (mCharType == Space) || (mCharType == Tab)
           || (mCharType == Cr) || (mCharType == Lf)) {
        nextChar();
    }
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
    mCharType(EofChar),
    mTokenType(UnknownToken),
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
    mCharType = EofChar;

    mTokenType = UnknownToken;
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
    case DefToken:
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
        mCharType = EofChar;

        break;
    case 9:
        mCharType = TabChar;

        break;
    case 10:
        mCharType = LfChar;

        break;
    case 13:
        mCharType = CrChar;

        break;
    case 32:
        mCharType = SpaceChar;

        break;
    case 34:
        mCharType = DoubleQuoteChar;

        break;
    case 39:
        mCharType = QuoteChar;

        break;
    case 40:
        mCharType = OpeningBracketChar;

        break;
    case 41:
        mCharType = ClosingBracketChar;

        break;
    case 42:
        mCharType = TimesChar;

        break;
    case 43:
        mCharType = PlusChar;

        break;
    case 44:
        mCharType = CommaChar;

        break;
    case 45:
        mCharType = MinusChar;

        break;
    case 47:
        mCharType = DivideChar;

        break;
    case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55:
    case 56: case 57:
        mCharType = DigitChar;

        break;
    case 58:
        mCharType = ColonChar;

        break;
    case 59:
        mCharType = SemiColonChar;

        break;
    case 60:
        mCharType = LtChar;

        break;
    case 61:
        mCharType = EqChar;

        break;
    case 62:
        mCharType = GtChar;

        break;
    case 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72:
    case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80:
    case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88:
    case 89: case 90:
        mCharType = LetterChar;

        break;
    case 95:
        mCharType = UnderscoreChar;

        break;
    case 97: case 98: case 99: case 100: case 101: case 102: case 103:
    case 104: case 105: case 106: case 107: case 108: case 109: case 110:
    case 111: case 112: case 113: case 114: case 115: case 116: case 117:
    case 118: case 119: case 120: case 121: case 122:
        mCharType = LetterChar;

        break;
    case 123:
        mCharType = OpeningCurlyBracketChar;

        break;
    case 125:
        mCharType = ClosingCurlyBracketChar;

        break;
    default:
        mCharType = OtherChar;
    }

    // Update our token line and/or column numbers

    if (mCharType == LfChar) {
        mTokenColumn = 1;

        ++mTokenLine;
    } else {
        ++mTokenColumn;
    }

    // Move to the next character

    ++mChar;
}

//==============================================================================

void CellmlTextViewScanner::getWord()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void CellmlTextViewScanner::getNumber()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void CellmlTextViewScanner::getString()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void CellmlTextViewScanner::nextToken()
{
    // Get the next token in our text by first skipping all the spaces and
    // special characters

    while (   (mCharType == SpaceChar) || (mCharType == TabChar)
           || (mCharType == CrChar) || (mCharType == LfChar)) {
        nextChar();
    }

    // Determine the type of our next token

    mTokenString = *mChar;

    switch (mCharType) {
    case LetterChar: case UnderscoreChar:
        getWord();

        break;
    case DigitChar:
        getNumber();

        break;
    case DoubleQuoteChar:
        getString();

        break;
    case EqChar:
        mTokenType = EqToken;

        nextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = EqEqToken;

            nextChar();
        }

        break;
    case LtChar:
        mTokenType = LtToken;

        nextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = LeqToken;

            nextChar();
        } else if (mCharType == GtChar) {
            mTokenString += *mChar;

            mTokenType = NeqToken;

            nextChar();
        }

        break;
    case GtChar:
        mTokenType = GtToken;

        nextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = GeqToken;

            nextChar();
        }

        break;
    case EofChar:
        mTokenType = EofToken;

        break;
    default:
        switch (mCharType) {
        case QuoteChar:               mTokenType = QuoteToken; break;
        case CommaChar:               mTokenType = CommaToken; break;
        case PlusChar:                mTokenType = PlusToken; break;
        case MinusChar:               mTokenType = MinusToken;               break;
        case TimesChar:               mTokenType = TimesToken;               break;
        case DivideChar:              mTokenType = DivideToken;              break;
        case ColonChar:               mTokenType = ColonToken;               break;
        case SemiColonChar:           mTokenType = SemiColonToken;           break;
        case OpeningBracketChar:      mTokenType = OpeningBracketToken;      break;
        case ClosingBracketChar:      mTokenType = ClosingBracketToken;      break;
        case OpeningCurlyBracketChar: mTokenType = OpeningCurlyBracketToken; break;
        case ClosingCurlyBracketChar: mTokenType = ClosingCurlyBracketToken; break;
        default:                      mTokenType = UnknownToken;
        }

        nextChar();
    }
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

#ifndef CELLMLTEXTVIEWSCANNER_H
#define CELLMLTEXTVIEWSCANNER_H

//==============================================================================

#include <QString>

//==============================================================================

#include <limits.h>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellmlTextViewScanner
{
public:
    enum TokenType {
        Unknown,
        Def
    };

    explicit CellmlTextViewScanner();

    void setText(const QString &pText);

    TokenType tokenType() const;

    int tokenLine() const;
    int tokenColumn() const;

    QString tokenString() const;

    static QString tokenTypeAsString(const TokenType &pTokenType);

private:
    enum CharType {
        Other,
        Letter, Digit, Underscore,
        DoubleQuote, Quote,
        Comma,
        Eq, Lt, Gt,
        Plus, Minus, Times, Divide,
        Colon, SemiColon,
        OpeningBracket, ClosingBracket,
        OpeningCurlyBracket, ClosingCurlyBracket,
        Space, Tab, Cr, Lf, Eof
    };

    QString mText;

    const QChar *mChar;
    CharType mCharType;

    TokenType mTokenType;

    int mTokenLine;
    int mTokenColumn;

    QString mTokenString;

    void nextChar();
    void nextToken();
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Scanner for the CellML Text format
//==============================================================================

#pragma once

//==============================================================================

#include <QMap>
#include <QObject>
#include <QString>

//==============================================================================

#include <climits>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

class CellmlTextViewScanner : public QObject
{
    Q_OBJECT

public:
    enum class Token {
        Unknown, SingleLineComment, MultilineComment, String,
        IdentifierOrCmetaId, ProperCmetaId, Number, Invalid,

        // CellML Text keywords

        And, As, Between, Case, Comp, Def, EndComp, EndDef, EndSel, For, Group,
        Import, Incl, Map, Model, Otherwise, Sel, Unit, Using, Var, Vars,

        /*And,*/ Or, Xor, Not,

        Ode,

        Abs, Ceil, Exp, Fact, Floor, Ln, Sqr, Sqrt,

        Sin, Cos, Tan, Sec, Csc, Cot,
        Sinh, Cosh, Tanh, Sech, Csch, Coth,
        Asin, Acos, Atan, Asec, Acsc, Acot,
        Asinh, Acosh, Atanh, Asech, Acsch, Acoth,

        FirstOneArgumentMathematicalFunction = Abs,
        LastOneArgumentMathematicalFunction = Acoth,

        Log,

        FirstOneOrTwoArgumentMathematicalFunction = Log,
        LastOneOrTwoArgumentMathematicalFunction = Log,

        Pow, Rem, Root,

        FirstTwoArgumentMathematicalFunction = Pow,
        LastTwoArgumentMathematicalFunction = Root,

        Min, Max,

        Gcd, Lcm,

        FirstTwoOrMoreArgumentMathematicalFunction = Min,
        LastTwoOrMoreArgumentMathematicalFunction = Lcm,

        True, False, Nan, Pi, Inf, E,

        FirstMathematicalConstant = True,
        LastMathematicalConstant = E,

        Base, Encapsulation, Containment,

        Ampere, Becquerel, Candela, Celsius, Coulomb, Dimensionless, Farad,
        Gram, Gray, Henry, Hertz, Joule, Katal, Kelvin, Kilogram, Liter, Litre,
        Lumen, Lux, Meter, Metre, Mole, Newton, Ohm, Pascal, Radian, Second,
        Siemens, Sievert, Steradian, Tesla, Volt, Watt, Weber,

        FirstUnit = Ampere,
        LastUnit = Weber,

        // CellML Text parameter keywords

        Pref, Expo, Mult, Off,

        Init, Pub, Priv,

        Yotta, Zetta, Exa, Peta, Tera, Giga, Mega, Kilo, Hecto, Deka, Deci,
        Centi, Milli, Micro, Nano, Pico, Femto, Atto, Zepto, Yocto,

        FirstPrefix = Yotta,
        LastPrefix = Yocto,

        In, Out, None,

        // Miscellaneous

        Quote, Comma,
        Eq, EqEq, Neq, Lt, Leq, Gt, Geq,
        Plus, Minus, Times, Divide,
        Colon, SemiColon,
        OpeningBracket, ClosingBracket,
        OpeningCurlyBracket, ClosingCurlyBracket,
        Eof
    };

    using Tokens = QList<Token>;

    explicit CellmlTextViewScanner();

    void setText(const QString &pText);

    Token token() const;
    int line() const;
    int column() const;
    QString string() const;
    QString comment() const;

    void getNextToken();

private:
    enum class Char {
        Other,
        Letter, Digit, Underscore,
        DoubleQuote, Quote, Comma,
        Eq, Lt, Gt,
        Plus, Minus, Times, Divide,
        FullStop, Colon, SemiColon,
        OpeningBracket, ClosingBracket,
        OpeningCurlyBracket, ClosingCurlyBracket,
        Space, Tab, Cr, Lf, Eof
    };

    QString mText;

    const QChar *mChar = nullptr;

    Char mCharType = Char::Eof;
    int mCharLine = 1;
    int mCharColumn = 0;

    Token mToken = Token::Unknown;
    int mLine = 0;
    int mColumn = 0;
    QString mString;
    QString mComment;

    QMap<QString, Token> mKeywords;
    QMap<QString, Token> mSiUnitKeywords;
    QMap<QString, Token> mParameterKeywords;

    bool mWithinParameterBlock = false;

    void getNextChar();

    void getSingleLineComment();
    void getMultilineComment();
    void getWord();
    void getNumber();
    void getString();
};

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

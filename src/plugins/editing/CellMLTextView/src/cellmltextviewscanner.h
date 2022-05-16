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
    explicit CellmlTextViewScanner(const CellmlTextViewScanner &pScanner);

    void operator=(const CellmlTextViewScanner &pScanner);

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

    QMap<QString, Token> mKeywords = {
                                         { "and", Token::And },
                                         { "as", Token::As },
                                         { "between", Token::Between },
                                         { "case", Token::Case },
                                         { "comp", Token::Comp },
                                         { "def", Token::Def },
                                         { "endcomp", Token::EndComp },
                                         { "enddef", Token::EndDef },
                                         { "endsel", Token::EndSel },
                                         { "for", Token::For },
                                         { "group", Token::Group },
                                         { "import", Token::Import },
                                         { "incl", Token::Incl },
                                         { "map", Token::Map },
                                         { "model", Token::Model },
                                         { "otherwise", Token::Otherwise },
                                         { "sel", Token::Sel },
                                         { "unit", Token::Unit },
                                         { "using", Token::Using },
                                         { "var", Token::Var },
                                         { "vars", Token::Vars },

                                         { "abs", Token::Abs },
                                         { "ceil", Token::Ceil },
                                         { "exp", Token::Exp },
                                         { "fact", Token::Fact },
                                         { "floor", Token::Floor },
                                         { "ln", Token::Ln },
                                         { "log", Token::Log },
                                         { "pow", Token::Pow },
                                         { "rem", Token::Rem },
                                         { "root", Token::Root },
                                         { "sqr", Token::Sqr },
                                         { "sqrt", Token::Sqrt },

//                                         { "and", Token::And },
                                         { "or", Token::Or },
                                         { "xor", Token::Xor },
                                         { "not", Token::Not },

                                         { "ode", Token::Ode },

                                         { "min", Token::Min },
                                         { "max", Token::Max },

                                         { "gcd", Token::Gcd },
                                         { "lcm", Token::Lcm },

                                         { "sin", Token::Sin },
                                         { "cos", Token::Cos },
                                         { "tan", Token::Tan },
                                         { "sec", Token::Sec },
                                         { "csc", Token::Csc },
                                         { "cot", Token::Cot },
                                         { "sinh", Token::Sinh },
                                         { "cosh", Token::Cosh },
                                         { "tanh", Token::Tanh },
                                         { "sech", Token::Sech },
                                         { "csch", Token::Csch },
                                         { "coth", Token::Coth },
                                         { "asin", Token::Asin },
                                         { "acos", Token::Acos },
                                         { "atan", Token::Atan },
                                         { "asec", Token::Asec },
                                         { "acsc", Token::Acsc },
                                         { "acot", Token::Acot },
                                         { "asinh", Token::Asinh },
                                         { "acosh", Token::Acosh },
                                         { "atanh", Token::Atanh },
                                         { "asech", Token::Asech },
                                         { "acsch", Token::Acsch },
                                         { "acoth", Token::Acoth },

                                         { "true", Token::True },
                                         { "false", Token::False },
                                         { "nan", Token::Nan },
                                         { "pi", Token::Pi },
                                         { "inf", Token::Inf },
                                         { "e", Token::E },

                                         { "base", Token::Base },
                                         { "encapsulation", Token::Encapsulation },
                                         { "containment", Token::Containment }
                                     };
    QMap<QString, Token> mSiUnitKeywords = {
                                               { "ampere", Token::Ampere },
                                               { "becquerel", Token::Becquerel },
                                               { "candela", Token::Candela },
                                               { "celsius", Token::Celsius },
                                               { "coulomb", Token::Coulomb },
                                               { "dimensionless", Token::Dimensionless },
                                               { "farad", Token::Farad },
                                               { "gram", Token::Gram },
                                               { "gray", Token::Gray },
                                               { "henry", Token::Henry },
                                               { "hertz", Token::Hertz },
                                               { "joule", Token::Joule },
                                               { "katal", Token::Katal },
                                               { "kelvin", Token::Kelvin },
                                               { "kilogram", Token::Kilogram },
                                               { "liter", Token::Liter },
                                               { "litre", Token::Litre },
                                               { "lumen", Token::Lumen },
                                               { "lux", Token::Lux },
                                               { "meter", Token::Meter },
                                               { "metre", Token::Metre },
                                               { "mole", Token::Mole },
                                               { "newton", Token::Newton },
                                               { "ohm", Token::Ohm },
                                               { "pascal", Token::Pascal },
                                               { "radian", Token::Radian },
                                               { "second", Token::Second },
                                               { "siemens", Token::Siemens },
                                               { "sievert", Token::Sievert },
                                               { "steradian", Token::Steradian },
                                               { "tesla", Token::Tesla },
                                               { "volt", Token::Volt },
                                               { "watt", Token::Watt },
                                               { "weber", Token::Weber }
                                            };
    QMap<QString, Token> mParameterKeywords = {
                                                  { "pref", Token::Pref },
                                                  { "expo", Token::Expo },
                                                  { "mult", Token::Mult },
                                                  { "off", Token::Off },

                                                  { "init", Token::Init },
                                                  { "pub", Token::Pub },
                                                  { "priv", Token::Priv },

                                                  { "yotta", Token::Yotta },
                                                  { "zetta", Token::Zetta },
                                                  { "exa", Token::Exa },
                                                  { "peta", Token::Peta },
                                                  { "tera", Token::Tera },
                                                  { "giga", Token::Giga },
                                                  { "mega", Token::Mega },
                                                  { "kilo", Token::Kilo },
                                                  { "hecto", Token::Hecto },
                                                  { "deka", Token::Deka },
                                                  { "deci", Token::Deci },
                                                  { "centi", Token::Centi },
                                                  { "milli", Token::Milli },
                                                  { "micro", Token::Micro },
                                                  { "nano", Token::Nano },
                                                  { "pico", Token::Pico },
                                                  { "femto", Token::Femto },
                                                  { "atto", Token::Atto },
                                                  { "zepto", Token::Zepto },
                                                  { "yocto", Token::Yocto },

                                                  { "in", Token::In },
                                                  { "out", Token::Out },
                                                  { "none", Token::None }
                                              };

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

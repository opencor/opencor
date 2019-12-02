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

#include "cellmltextviewscanner.h"

//==============================================================================

#include <QObject>
#include <QRegularExpression>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewScanner::CellmlTextViewScanner()
{
    // Our various CellML Text keywords

    mKeywords.insert("and", Token::And);
    mKeywords.insert("as", Token::As);
    mKeywords.insert("between", Token::Between);
    mKeywords.insert("case", Token::Case);
    mKeywords.insert("comp", Token::Comp);
    mKeywords.insert("def", Token::Def);
    mKeywords.insert("endcomp", Token::EndComp);
    mKeywords.insert("enddef", Token::EndDef);
    mKeywords.insert("endsel", Token::EndSel);
    mKeywords.insert("for", Token::For);
    mKeywords.insert("group", Token::Group);
    mKeywords.insert("import", Token::Import);
    mKeywords.insert("incl", Token::Incl);
    mKeywords.insert("map", Token::Map);
    mKeywords.insert("model", Token::Model);
    mKeywords.insert("otherwise", Token::Otherwise);
    mKeywords.insert("sel", Token::Sel);
    mKeywords.insert("unit", Token::Unit);
    mKeywords.insert("using", Token::Using);
    mKeywords.insert("var", Token::Var);
    mKeywords.insert("vars", Token::Vars);

    mKeywords.insert("abs", Token::Abs);
    mKeywords.insert("ceil", Token::Ceil);
    mKeywords.insert("exp", Token::Exp);
    mKeywords.insert("fact", Token::Fact);
    mKeywords.insert("floor", Token::Floor);
    mKeywords.insert("ln", Token::Ln);
    mKeywords.insert("log", Token::Log);
    mKeywords.insert("pow", Token::Pow);
    mKeywords.insert("rem", Token::Rem);
    mKeywords.insert("root", Token::Root);
    mKeywords.insert("sqr", Token::Sqr);
    mKeywords.insert("sqrt", Token::Sqrt);

//    mKeywords.insert("and", TokenType::And);
    mKeywords.insert("or", Token::Or);
    mKeywords.insert("xor", Token::Xor);
    mKeywords.insert("not", Token::Not);

    mKeywords.insert("ode", Token::Ode);

    mKeywords.insert("min", Token::Min);
    mKeywords.insert("max", Token::Max);

    mKeywords.insert("gcd", Token::Gcd);
    mKeywords.insert("lcm", Token::Lcm);

    mKeywords.insert("sin", Token::Sin);
    mKeywords.insert("cos", Token::Cos);
    mKeywords.insert("tan", Token::Tan);
    mKeywords.insert("sec", Token::Sec);
    mKeywords.insert("csc", Token::Csc);
    mKeywords.insert("cot", Token::Cot);
    mKeywords.insert("sinh", Token::Sinh);
    mKeywords.insert("cosh", Token::Cosh);
    mKeywords.insert("tanh", Token::Tanh);
    mKeywords.insert("sech", Token::Sech);
    mKeywords.insert("csch", Token::Csch);
    mKeywords.insert("coth", Token::Coth);
    mKeywords.insert("asin", Token::Asin);
    mKeywords.insert("acos", Token::Acos);
    mKeywords.insert("atan", Token::Atan);
    mKeywords.insert("asec", Token::Asec);
    mKeywords.insert("acsc", Token::Acsc);
    mKeywords.insert("acot", Token::Acot);
    mKeywords.insert("asinh", Token::Asinh);
    mKeywords.insert("acosh", Token::Acosh);
    mKeywords.insert("atanh", Token::Atanh);
    mKeywords.insert("asech", Token::Asech);
    mKeywords.insert("acsch", Token::Acsch);
    mKeywords.insert("acoth", Token::Acoth);

    mKeywords.insert("true", Token::True);
    mKeywords.insert("false", Token::False);
    mKeywords.insert("nan", Token::Nan);
    mKeywords.insert("pi", Token::Pi);
    mKeywords.insert("inf", Token::Inf);
    mKeywords.insert("e", Token::E);

    mKeywords.insert("base", Token::Base);
    mKeywords.insert("encapsulation", Token::Encapsulation);
    mKeywords.insert("containment", Token::Containment);

    // Our various CellML Text SI unit keywords

    mSiUnitKeywords.insert("ampere", Token::Ampere);
    mSiUnitKeywords.insert("becquerel", Token::Becquerel);
    mSiUnitKeywords.insert("candela", Token::Candela);
    mSiUnitKeywords.insert("celsius", Token::Celsius);
    mSiUnitKeywords.insert("coulomb", Token::Coulomb);
    mSiUnitKeywords.insert("dimensionless", Token::Dimensionless);
    mSiUnitKeywords.insert("farad", Token::Farad);
    mSiUnitKeywords.insert("gram", Token::Gram);
    mSiUnitKeywords.insert("gray", Token::Gray);
    mSiUnitKeywords.insert("henry", Token::Henry);
    mSiUnitKeywords.insert("hertz", Token::Hertz);
    mSiUnitKeywords.insert("joule", Token::Joule);
    mSiUnitKeywords.insert("katal", Token::Katal);
    mSiUnitKeywords.insert("kelvin", Token::Kelvin);
    mSiUnitKeywords.insert("kilogram", Token::Kilogram);
    mSiUnitKeywords.insert("liter", Token::Liter);
    mSiUnitKeywords.insert("litre", Token::Litre);
    mSiUnitKeywords.insert("lumen", Token::Lumen);
    mSiUnitKeywords.insert("lux", Token::Lux);
    mSiUnitKeywords.insert("meter", Token::Meter);
    mSiUnitKeywords.insert("metre", Token::Metre);
    mSiUnitKeywords.insert("mole", Token::Mole);
    mSiUnitKeywords.insert("newton", Token::Newton);
    mSiUnitKeywords.insert("ohm", Token::Ohm);
    mSiUnitKeywords.insert("pascal", Token::Pascal);
    mSiUnitKeywords.insert("radian", Token::Radian);
    mSiUnitKeywords.insert("second", Token::Second);
    mSiUnitKeywords.insert("siemens", Token::Siemens);
    mSiUnitKeywords.insert("sievert", Token::Sievert);
    mSiUnitKeywords.insert("steradian", Token::Steradian);
    mSiUnitKeywords.insert("tesla", Token::Tesla);
    mSiUnitKeywords.insert("volt", Token::Volt);
    mSiUnitKeywords.insert("watt", Token::Watt);
    mSiUnitKeywords.insert("weber", Token::Weber);

    // Our various CellML Text parameter keywords

    mParameterKeywords.insert("pref", Token::Pref);
    mParameterKeywords.insert("expo", Token::Expo);
    mParameterKeywords.insert("mult", Token::Mult);
    mParameterKeywords.insert("off", Token::Off);

    mParameterKeywords.insert("init", Token::Init);
    mParameterKeywords.insert("pub", Token::Pub);
    mParameterKeywords.insert("priv", Token::Priv);

    mParameterKeywords.insert("yotta", Token::Yotta);
    mParameterKeywords.insert("zetta", Token::Zetta);
    mParameterKeywords.insert("exa", Token::Exa);
    mParameterKeywords.insert("peta", Token::Peta);
    mParameterKeywords.insert("tera", Token::Tera);
    mParameterKeywords.insert("giga", Token::Giga);
    mParameterKeywords.insert("mega", Token::Mega);
    mParameterKeywords.insert("kilo", Token::Kilo);
    mParameterKeywords.insert("hecto", Token::Hecto);
    mParameterKeywords.insert("deka", Token::Deka);
    mParameterKeywords.insert("deci", Token::Deci);
    mParameterKeywords.insert("centi", Token::Centi);
    mParameterKeywords.insert("milli", Token::Milli);
    mParameterKeywords.insert("micro", Token::Micro);
    mParameterKeywords.insert("nano", Token::Nano);
    mParameterKeywords.insert("pico", Token::Pico);
    mParameterKeywords.insert("femto", Token::Femto);
    mParameterKeywords.insert("atto", Token::Atto);
    mParameterKeywords.insert("zepto", Token::Zepto);
    mParameterKeywords.insert("yocto", Token::Yocto);

    mParameterKeywords.insert("in", Token::In);
    mParameterKeywords.insert("out", Token::Out);
    mParameterKeywords.insert("none", Token::None);
}

//==============================================================================

void CellmlTextViewScanner::setText(const QString &pText)
{
    // Initialise ourselves with the text to scan

    mText = pText;

    mChar = mText.constData()-1;

    mCharType = Char::Eof;
    mCharLine = 1;
    mCharColumn = 0;

    mToken = Token::Unknown;
    mLine = 0;
    mColumn = 0;
    mString = QString();

    mWithinParameterBlock = false;

    getNextChar();
    getNextToken();
}

//==============================================================================

CellmlTextViewScanner::Token CellmlTextViewScanner::token() const
{
    // Return our token type

    return mToken;
}

//==============================================================================

int CellmlTextViewScanner::line() const
{
    // Return our token line

    return mLine;
}

//==============================================================================

int CellmlTextViewScanner::column() const
{
    // Return our token column

    return mColumn;
}

//==============================================================================

QString CellmlTextViewScanner::string() const
{
    // Return our token as a string

    return mString;
}

//==============================================================================

QString CellmlTextViewScanner::comment() const
{
    // Return our token comment

    return mComment;
}

//==============================================================================

void CellmlTextViewScanner::getNextChar()
{
    // Determine the type of our next character

    switch ((++mChar)->unicode()) {
    case 0:
        mCharType = Char::Eof;

        break;
    case 9:
        mCharType = Char::Tab;

        break;
    case 10:
        mCharType = Char::Lf;

        break;
    case 13:
        mCharType = Char::Cr;

        break;
    case 32:
        mCharType = Char::Space;

        break;
    case 34:
        mCharType = Char::DoubleQuote;

        break;
    case 39:
        mCharType = Char::Quote;

        break;
    case 40:
        mCharType = Char::OpeningBracket;

        break;
    case 41:
        mCharType = Char::ClosingBracket;

        break;
    case 42:
        mCharType = Char::Times;

        break;
    case 43:
        mCharType = Char::Plus;

        break;
    case 44:
        mCharType = Char::Comma;

        break;
    case 45:
        mCharType = Char::Minus;

        break;
    case 46:
        mCharType = Char::FullStop;

        break;
    case 47:
        mCharType = Char::Divide;

        break;
    case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55:
    case 56: case 57:
        mCharType = Char::Digit;

        break;
    case 58:
        mCharType = Char::Colon;

        break;
    case 59:
        mCharType = Char::SemiColon;

        break;
    case 60:
        mCharType = Char::Lt;

        break;
    case 61:
        mCharType = Char::Eq;

        break;
    case 62:
        mCharType = Char::Gt;

        break;
    case 65: case 66: case 67: case 68: case 69: case 70: case 71: case 72:
    case 73: case 74: case 75: case 76: case 77: case 78: case 79: case 80:
    case 81: case 82: case 83: case 84: case 85: case 86: case 87: case 88:
    case 89: case 90:
        mCharType = Char::Letter;

        break;
    case 95:
        mCharType = Char::Underscore;

        break;
    case 97: case 98: case 99: case 100: case 101: case 102: case 103:
    case 104: case 105: case 106: case 107: case 108: case 109: case 110:
    case 111: case 112: case 113: case 114: case 115: case 116: case 117:
    case 118: case 119: case 120: case 121: case 122:
        mCharType = Char::Letter;

        break;
    case 123:
        mCharType = Char::OpeningCurlyBracket;

        break;
    case 125:
        mCharType = Char::ClosingCurlyBracket;

        break;
    default:
        mCharType = Char::Other;
    }

    // Update our token line and/or column numbers

    if (mCharType == Char::Lf) {
        mCharColumn = 0;

        ++mCharLine;
    } else {
        ++mCharColumn;
    }
}

//==============================================================================

void CellmlTextViewScanner::getSingleLineComment()
{
    // Retrieve a single line comment by looking for the end of the current line

    mToken = Token::SingleLineComment;
    mString = QString();

    forever {
        getNextChar();

        if (   (mCharType == Char::Cr) || (mCharType == Char::Lf)
            || (mCharType == Char::Eof)) {
            break;
        }

        mString += *mChar;
    }
}

//==============================================================================

void CellmlTextViewScanner::getMultilineComment()
{
    // Retrieve a multiline comment from our text by looking for the first
    // occurrence of "*/"

    mString = QString();

    forever {
        getNextChar();

        if (mCharType == Char::Times) {
            QChar timesChar = *mChar;

            getNextChar();

            if (mCharType == Char::Divide) {
                getNextChar();

                mToken = Token::MultilineComment;

                return;
            }

            mString += timesChar;

            if (mCharType == Char::Eof) {
                break;
            }

            mString += *mChar;
        } else if (mCharType == Char::Eof) {
            break;
        } else {
            mString += *mChar;
        }
    }

    mToken = Token::Invalid;
    mComment = tr("The comment is incomplete.");
}

//==============================================================================

void CellmlTextViewScanner::getWord()
{
    // Retrieve a word from our text

    forever {
        getNextChar();

        if (   (mCharType == Char::Letter) || (mCharType == Char::Digit) || (mCharType == Char::Underscore)
            || (mWithinParameterBlock && ((mCharType == Char::Minus) || (mCharType == Char::FullStop)))) {
            mString += *mChar;
        } else {
            break;
        }
    }

    // Check what kind of word we are dealing with, i.e. a keyword, an SI unit
    // keyword, a parameter keyword, an identifier or something else

    if (mWithinParameterBlock) {
        mToken = mParameterKeywords.value(mString, Token::Unknown);
    } else {
        mToken = mKeywords.value(mString, Token::Unknown);
    }

    if (mToken == Token::Unknown) {
        mToken = mSiUnitKeywords.value(mString, Token::Unknown);
    }

    if (mToken == Token::Unknown) {
        // We are not dealing with a keyword, but it might still be an
        // identifier or cmeta:id, as long as it doesn't only consist of
        // underscores, hyphens and periods

        static const QRegularExpression UnderscoresHyphensOrPeriodsRegEx = QRegularExpression("_|-|\\.");
        static const QRegularExpression HyphensOrPeriodsRegEx = QRegularExpression("-|\\.");

        if (!QString(mString).remove(UnderscoresHyphensOrPeriodsRegEx).isEmpty()) {
            if (mString.contains(HyphensOrPeriodsRegEx)) {
                mToken = Token::ProperCmetaId;
            } else {
                mToken = Token::IdentifierOrCmetaId;
            }
        }
    }
}

//==============================================================================

void CellmlTextViewScanner::getNumber()
{
    // Retrieve a number from our text

    bool fullStopFirstChar = mCharType == Char::FullStop;

    if (fullStopFirstChar) {
        // We started a number with a full stop, so reset mTokenString since it
        // is going to be updated with our full stop

        mString = QString();
    } else {
        // We started a number with a digit, so look for additional ones

        forever {
            getNextChar();

            if (mCharType == Char::Digit) {
                mString += *mChar;
            } else {
                break;
            }
        }
    }

    // Look for the fractional part, if any

    if (mCharType == Char::FullStop) {
        mString += *mChar;

        getNextChar();

        // Check whether the full stop is followed by some digits

        if (mCharType == Char::Digit) {
            mString += *mChar;

            forever {
                getNextChar();

                if (mCharType == Char::Digit) {
                    mString += *mChar;
                } else {
                    break;
                }
            }
        } else if (fullStopFirstChar) {
            // We started a number with a full stop, but it's not followed by
            // digits, so it's not a number after all

            mToken = Token::Unknown;

            return;
        }
    }

    // Check whether we have an exponent part

    if (   (mCharType == Char::Letter)
        && ((*mChar == QChar('e')) || (*mChar == QChar('E')))) {
        mString += *mChar;

        getNextChar();

        // Check whether we have a + or - sign

        if ((mCharType == Char::Plus) || (mCharType == Char::Minus)) {
            mString += *mChar;

            getNextChar();
        }

        // Check whether we have some digits

        if (mCharType == Char::Digit) {
            mString += *mChar;

            forever {
                getNextChar();

                if (mCharType == Char::Digit) {
                    mString += *mChar;
                } else {
                    break;
                }
            }
        } else {
            // We started an exponent part, but it isn't followed by digits

            mToken = Token::Invalid;
            mComment = tr("The exponent has no digits.");

            return;
        }
    }

    // At this stage, we have a number, but it may be invalid

    bool validNumber;

    mString.toDouble(&validNumber);

    mToken = Token::Number;

    if (!validNumber) {
        mComment = tr("The number is not valid (e.g. too big, too small).");
    }
}

//==============================================================================

void CellmlTextViewScanner::getString()
{
    // Retrieve a string from our text by looking for a double quote

    mString = QString();

    forever {
        getNextChar();

        if (   (mCharType == Char::DoubleQuote)
            || (mCharType == Char::Cr) || (mCharType == Char::Lf)
            || (mCharType == Char::Eof)) {
            break;
        }

        mString += *mChar;
    }

    if (mCharType == Char::DoubleQuote) {
        mToken = Token::String;

        getNextChar();
    } else {
        mToken = Token::Invalid;
        mComment = tr("The string is incomplete.");
    }
}

//==============================================================================

void CellmlTextViewScanner::getNextToken()
{
    // Get the next token in our text by first skipping all the spaces and
    // special characters

    while (   (mCharType == Char::Space) || (mCharType == Char::Tab)
           || (mCharType == Char::Cr)    || (mCharType == Char::Lf)) {
        getNextChar();
    }

    // Determine the type of our next token

    mLine = mCharLine;
    mColumn = mCharColumn;

    mString = *mChar;

    mComment = QString();

    if (   (mCharType == Char::Letter)
        || (mCharType == Char::Underscore)) {
        getWord();
    } else if (   (mCharType == Char::Digit)
               || (mCharType == Char::FullStop)) {
        getNumber();
    } else if (mCharType == Char::DoubleQuote) {
        getString();
    } else if (mCharType == Char::Eq) {
        mToken = Token::Eq;

        getNextChar();

        if (mCharType == Char::Eq) {
            mString += *mChar;

            mToken = Token::EqEq;

            getNextChar();
        }
    } else if (mCharType == Char::Lt) {
        mToken = Token::Lt;

        getNextChar();

        if (mCharType == Char::Eq) {
            mString += *mChar;

            mToken = Token::Leq;

            getNextChar();
        } else if (mCharType == Char::Gt) {
            mString += *mChar;

            mToken = Token::Neq;

            getNextChar();
        }
    } else if (mCharType == Char::Gt) {
        mToken = Token::Gt;

        getNextChar();

        if (mCharType == Char::Eq) {
            mString += *mChar;

            mToken = Token::Geq;

            getNextChar();
        }
    } else if (mCharType == Char::Divide) {
        mToken = Token::Divide;

        getNextChar();

        if (mCharType == Char::Divide) {
            getSingleLineComment();
        } else if (mCharType == Char::Times) {
            getMultilineComment();
        }
    } else if (   (mCharType == Char::OpeningCurlyBracket)
               || (mCharType == Char::ClosingCurlyBracket)) {
        mToken = (mCharType == Char::OpeningCurlyBracket)?
                     Token::OpeningCurlyBracket:
                     Token::ClosingCurlyBracket;
        mWithinParameterBlock = mCharType == Char::OpeningCurlyBracket;

        getNextChar();
    } else if (mCharType == Char::Eof) {
        mToken = Token::Eof;
        mString = tr("the end of the file");
    } else {
        if (mCharType == Char::Quote) {
            mToken = Token::Quote;
        } else if (mCharType == Char::Comma) {
            mToken = Token::Comma;
        } else if (mCharType == Char::Plus) {
            mToken = Token::Plus;
        } else if (mCharType == Char::Minus) {
            mToken = Token::Minus;
        } else if (mCharType == Char::Times) {
            mToken = Token::Times;
        } else if (mCharType == Char::Colon) {
            mToken = Token::Colon;
        } else if (mCharType == Char::SemiColon) {
            mToken = Token::SemiColon;
        } else if (mCharType == Char::OpeningBracket) {
            mToken = Token::OpeningBracket;
        } else if (mCharType == Char::ClosingBracket) {
            mToken = Token::ClosingBracket;
        } else {
            mToken = Token::Unknown;
        }

        getNextChar();
    }
}

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

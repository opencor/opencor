/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

CellmlTextViewScanner::CellmlTextViewScanner() :
    mText(QString()),
    mChar(nullptr),
    mCharType(EofChar),
    mCharLine(1),
    mCharColumn(0),
    mTokenType(TokenType::UnknownToken),
    mTokenLine(0),
    mTokenColumn(0),
    mTokenString(QString()),
    mTokenComment(QString()),
    mWithinParameterBlock(false)
{
    // Our various CellML Text keywords

    mKeywords.insert("and", TokenType::AndToken);
    mKeywords.insert("as", TokenType::AsToken);
    mKeywords.insert("between", TokenType::BetweenToken);
    mKeywords.insert("case", TokenType::CaseToken);
    mKeywords.insert("comp", TokenType::CompToken);
    mKeywords.insert("def", TokenType::DefToken);
    mKeywords.insert("endcomp", TokenType::EndCompToken);
    mKeywords.insert("enddef", TokenType::EndDefToken);
    mKeywords.insert("endsel", TokenType::EndSelToken);
    mKeywords.insert("for", TokenType::ForToken);
    mKeywords.insert("group", TokenType::GroupToken);
    mKeywords.insert("import", TokenType::ImportToken);
    mKeywords.insert("incl", TokenType::InclToken);
    mKeywords.insert("map", TokenType::MapToken);
    mKeywords.insert("model", TokenType::ModelToken);
    mKeywords.insert("otherwise", TokenType::OtherwiseToken);
    mKeywords.insert("sel", TokenType::SelToken);
    mKeywords.insert("unit", TokenType::UnitToken);
    mKeywords.insert("using", TokenType::UsingToken);
    mKeywords.insert("var", TokenType::VarToken);
    mKeywords.insert("vars", TokenType::VarsToken);

    mKeywords.insert("abs", TokenType::AbsToken);
    mKeywords.insert("ceil", TokenType::CeilToken);
    mKeywords.insert("exp", TokenType::ExpToken);
    mKeywords.insert("fact", TokenType::FactToken);
    mKeywords.insert("floor", TokenType::FloorToken);
    mKeywords.insert("ln", TokenType::LnToken);
    mKeywords.insert("log", TokenType::LogToken);
    mKeywords.insert("pow", TokenType::PowToken);
    mKeywords.insert("rem", TokenType::RemToken);
    mKeywords.insert("root", TokenType::RootToken);
    mKeywords.insert("sqr", TokenType::SqrToken);
    mKeywords.insert("sqrt", TokenType::SqrtToken);

//    mKeywords.insert("and", TokenType::AndToken);
    mKeywords.insert("or", TokenType::OrToken);
    mKeywords.insert("xor", TokenType::XorToken);
    mKeywords.insert("not", TokenType::NotToken);

    mKeywords.insert("ode", TokenType::OdeToken);

    mKeywords.insert("min", TokenType::MinToken);
    mKeywords.insert("max", TokenType::MaxToken);

    mKeywords.insert("gcd", TokenType::GcdToken);
    mKeywords.insert("lcm", TokenType::LcmToken);

    mKeywords.insert("sin", TokenType::SinToken);
    mKeywords.insert("cos", TokenType::CosToken);
    mKeywords.insert("tan", TokenType::TanToken);
    mKeywords.insert("sec", TokenType::SecToken);
    mKeywords.insert("csc", TokenType::CscToken);
    mKeywords.insert("cot", TokenType::CotToken);
    mKeywords.insert("sinh", TokenType::SinhToken);
    mKeywords.insert("cosh", TokenType::CoshToken);
    mKeywords.insert("tanh", TokenType::TanhToken);
    mKeywords.insert("sech", TokenType::SechToken);
    mKeywords.insert("csch", TokenType::CschToken);
    mKeywords.insert("coth", TokenType::CothToken);
    mKeywords.insert("asin", TokenType::AsinToken);
    mKeywords.insert("acos", TokenType::AcosToken);
    mKeywords.insert("atan", TokenType::AtanToken);
    mKeywords.insert("asec", TokenType::AsecToken);
    mKeywords.insert("acsc", TokenType::AcscToken);
    mKeywords.insert("acot", TokenType::AcotToken);
    mKeywords.insert("asinh", TokenType::AsinhToken);
    mKeywords.insert("acosh", TokenType::AcoshToken);
    mKeywords.insert("atanh", TokenType::AtanhToken);
    mKeywords.insert("asech", TokenType::AsechToken);
    mKeywords.insert("acsch", TokenType::AcschToken);
    mKeywords.insert("acoth", TokenType::AcothToken);

    mKeywords.insert("true", TokenType::TrueToken);
    mKeywords.insert("false", TokenType::FalseToken);
    mKeywords.insert("nan", TokenType::NanToken);
    mKeywords.insert("pi", TokenType::PiToken);
    mKeywords.insert("inf", TokenType::InfToken);
    mKeywords.insert("e", TokenType::EToken);

    mKeywords.insert("base", TokenType::BaseToken);
    mKeywords.insert("encapsulation", TokenType::EncapsulationToken);
    mKeywords.insert("containment", TokenType::ContainmentToken);

    // Our various CellML Text SI unit keywords

    mSiUnitKeywords.insert("ampere", TokenType::AmpereToken);
    mSiUnitKeywords.insert("becquerel", TokenType::BecquerelToken);
    mSiUnitKeywords.insert("candela", TokenType::CandelaToken);
    mSiUnitKeywords.insert("celsius", TokenType::CelsiusToken);
    mSiUnitKeywords.insert("coulomb", TokenType::CoulombToken);
    mSiUnitKeywords.insert("dimensionless", TokenType::DimensionlessToken);
    mSiUnitKeywords.insert("farad", TokenType::FaradToken);
    mSiUnitKeywords.insert("gram", TokenType::GramToken);
    mSiUnitKeywords.insert("gray", TokenType::GrayToken);
    mSiUnitKeywords.insert("henry", TokenType::HenryToken);
    mSiUnitKeywords.insert("hertz", TokenType::HertzToken);
    mSiUnitKeywords.insert("joule", TokenType::JouleToken);
    mSiUnitKeywords.insert("katal", TokenType::KatalToken);
    mSiUnitKeywords.insert("kelvin", TokenType::KelvinToken);
    mSiUnitKeywords.insert("kilogram", TokenType::KilogramToken);
    mSiUnitKeywords.insert("liter", TokenType::LiterToken);
    mSiUnitKeywords.insert("litre", TokenType::LitreToken);
    mSiUnitKeywords.insert("lumen", TokenType::LumenToken);
    mSiUnitKeywords.insert("lux", TokenType::LuxToken);
    mSiUnitKeywords.insert("meter", TokenType::MeterToken);
    mSiUnitKeywords.insert("metre", TokenType::MetreToken);
    mSiUnitKeywords.insert("mole", TokenType::MoleToken);
    mSiUnitKeywords.insert("newton", TokenType::NewtonToken);
    mSiUnitKeywords.insert("ohm", TokenType::OhmToken);
    mSiUnitKeywords.insert("pascal", TokenType::PascalToken);
    mSiUnitKeywords.insert("radian", TokenType::RadianToken);
    mSiUnitKeywords.insert("second", TokenType::SecondToken);
    mSiUnitKeywords.insert("siemens", TokenType::SiemensToken);
    mSiUnitKeywords.insert("sievert", TokenType::SievertToken);
    mSiUnitKeywords.insert("steradian", TokenType::SteradianToken);
    mSiUnitKeywords.insert("tesla", TokenType::TeslaToken);
    mSiUnitKeywords.insert("volt", TokenType::VoltToken);
    mSiUnitKeywords.insert("watt", TokenType::WattToken);
    mSiUnitKeywords.insert("weber", TokenType::WeberToken);

    // Our various CellML Text parameter keywords

    mParameterKeywords.insert("pref", TokenType::PrefToken);
    mParameterKeywords.insert("expo", TokenType::ExpoToken);
    mParameterKeywords.insert("mult", TokenType::MultToken);
    mParameterKeywords.insert("off", TokenType::OffToken);

    mParameterKeywords.insert("init", TokenType::InitToken);
    mParameterKeywords.insert("pub", TokenType::PubToken);
    mParameterKeywords.insert("priv", TokenType::PrivToken);

    mParameterKeywords.insert("yotta", TokenType::YottaToken);
    mParameterKeywords.insert("zetta", TokenType::ZettaToken);
    mParameterKeywords.insert("exa", TokenType::ExaToken);
    mParameterKeywords.insert("peta", TokenType::PetaToken);
    mParameterKeywords.insert("tera", TokenType::TeraToken);
    mParameterKeywords.insert("giga", TokenType::GigaToken);
    mParameterKeywords.insert("mega", TokenType::MegaToken);
    mParameterKeywords.insert("kilo", TokenType::KiloToken);
    mParameterKeywords.insert("hecto", TokenType::HectoToken);
    mParameterKeywords.insert("deka", TokenType::DekaToken);
    mParameterKeywords.insert("deci", TokenType::DeciToken);
    mParameterKeywords.insert("centi", TokenType::CentiToken);
    mParameterKeywords.insert("milli", TokenType::MilliToken);
    mParameterKeywords.insert("micro", TokenType::MicroToken);
    mParameterKeywords.insert("nano", TokenType::NanoToken);
    mParameterKeywords.insert("pico", TokenType::PicoToken);
    mParameterKeywords.insert("femto", TokenType::FemtoToken);
    mParameterKeywords.insert("atto", TokenType::AttoToken);
    mParameterKeywords.insert("zepto", TokenType::ZeptoToken);
    mParameterKeywords.insert("yocto", TokenType::YoctoToken);

    mParameterKeywords.insert("in", TokenType::InToken);
    mParameterKeywords.insert("out", TokenType::OutToken);
    mParameterKeywords.insert("none", TokenType::NoneToken);
}

//==============================================================================

void CellmlTextViewScanner::setText(const QString &pText)
{
    // Initialise ourselves with the text to scan

    mText = pText;

    mChar = mText.constData()-1;

    mCharType = EofChar;
    mCharLine = 1;
    mCharColumn = 0;

    mTokenType = TokenType::UnknownToken;
    mTokenLine = 0;
    mTokenColumn = 0;
    mTokenString = QString();

    mWithinParameterBlock = false;

    getNextChar();
    getNextToken();
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
    // Return our token as a string

    return mTokenString;
}

//==============================================================================

QString CellmlTextViewScanner::tokenComment() const
{
    // Return our token comment

    return mTokenComment;
}

//==============================================================================

void CellmlTextViewScanner::getNextChar()
{
    // Determine the type of our next character

    switch ((++mChar)->unicode()) {
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
    case 46:
        mCharType = FullStopChar;

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

    mTokenType = TokenType::SingleLineCommentToken;
    mTokenString = QString();

    forever {
        getNextChar();

        if (   (mCharType == CrChar) || (mCharType == LfChar)
            || (mCharType == EofChar)) {
            break;
        } else {
            mTokenString += *mChar;
        }
    }
}

//==============================================================================

void CellmlTextViewScanner::getMultilineComment()
{
    // Retrieve a multiline comment from our text by looking for the first
    // occurrence of "*/"

    mTokenString = QString();

    forever {
        getNextChar();

        if (mCharType == TimesChar) {
            QChar timesChar = *mChar;

            getNextChar();

            if (mCharType == DivideChar) {
                getNextChar();

                mTokenType = TokenType::MultilineCommentToken;

                return;
            } else {
                mTokenString += timesChar;

                if (mCharType == EofChar)
                    break;
                else
                    mTokenString += *mChar;
            }
        } else if (mCharType == EofChar) {
            break;
        } else {
            mTokenString += *mChar;
        }
    }

    mTokenType = TokenType::InvalidToken;
    mTokenComment = tr("The comment is incomplete.");
}

//==============================================================================

void CellmlTextViewScanner::getWord()
{
    // Retrieve a word from our text

    forever {
        getNextChar();

        if (   (mCharType == LetterChar) || (mCharType == DigitChar) || (mCharType == UnderscoreChar)
            || (mWithinParameterBlock && ((mCharType == MinusChar) || (mCharType == FullStopChar)))) {
            mTokenString += *mChar;
        } else {
            break;
        }
    }

    // Check what kind of word we are dealing with, i.e. a keyword, an SI unit
    // keyword, a parameter keyword, an identifier or something else

    if (mWithinParameterBlock)
        mTokenType = mParameterKeywords.value(mTokenString, TokenType::UnknownToken);
    else
        mTokenType = mKeywords.value(mTokenString, TokenType::UnknownToken);

    if (mTokenType == TokenType::UnknownToken)
        mTokenType = mSiUnitKeywords.value(mTokenString, TokenType::UnknownToken);

    if (mTokenType == TokenType::UnknownToken) {
        // We are not dealing with a keyword, but it might still be an
        // identifier or cmeta:id, as long as it doesn't only consist of
        // underscores, hyphens and periods

        static const QRegularExpression UnderscoresHyphensOrPeriodsRegEx = QRegularExpression("_|-|\\.");
        static const QRegularExpression HyphensOrPeriodsRegEx = QRegularExpression("-|\\.");

        if (!QString(mTokenString).remove(UnderscoresHyphensOrPeriodsRegEx).isEmpty()) {
            if (mTokenString.contains(HyphensOrPeriodsRegEx))
                mTokenType = TokenType::ProperCmetaIdToken;
            else
                mTokenType = TokenType::IdentifierOrCmetaIdToken;
        }
    }
}

//==============================================================================

void CellmlTextViewScanner::getNumber()
{
    // Retrieve a number from our text

    bool fullStopFirstChar = mCharType == FullStopChar;

    if (fullStopFirstChar) {
        // We started a number with a full stop, so reset mTokenString since it
        // is going to be updated with our full stop

        mTokenString = QString();
    } else {
        // We started a number with a digit, so look for additional ones

        forever {
            getNextChar();

            if (mCharType == DigitChar)
                mTokenString += *mChar;
            else
                break;
        }
    }

    // Look for the fractional part, if any

    if (mCharType == FullStopChar) {
        mTokenString += *mChar;

        getNextChar();

        // Check whether the full stop is followed by some digits

        if (mCharType == DigitChar) {
            mTokenString += *mChar;

            forever {
                getNextChar();

                if (mCharType == DigitChar)
                    mTokenString += *mChar;
                else
                    break;
            }
        } else if (fullStopFirstChar) {
            // We started a number with a full stop, but it's not followed by
            // digits, so it's not a number after all

            mTokenType = TokenType::UnknownToken;

            return;
        }
    }

    // Check whether we have an exponent part

    if (   (mCharType == LetterChar)
        && ((*mChar == QChar('e')) || (*mChar == QChar('E')))) {
        mTokenString += *mChar;

        getNextChar();

        // Check whether we have a + or - sign

        if ((mCharType == PlusChar) || (mCharType == MinusChar)) {
            mTokenString += *mChar;

            getNextChar();
        }

        // Check whether we have some digits

        if (mCharType == DigitChar) {
            mTokenString += *mChar;

            forever {
                getNextChar();

                if (mCharType == DigitChar)
                    mTokenString += *mChar;
                else
                    break;
            }
        } else {
            // We started an exponent part, but it isn't followed by digits

            mTokenType = TokenType::InvalidToken;
            mTokenComment = tr("The exponent has no digits.");

            return;
        }
    }

    // At this stage, we have a number, but it may be invalid

    bool validNumber;

    mTokenString.toDouble(&validNumber);

    mTokenType = TokenType::NumberToken;

    if (!validNumber)
        mTokenComment = tr("The number is not valid (e.g. too big, too small).");
}

//==============================================================================

void CellmlTextViewScanner::getString()
{
    // Retrieve a string from our text by looking for a double quote

    mTokenString = QString();

    forever {
        getNextChar();

        if (   (mCharType == DoubleQuoteChar)
            || (mCharType == CrChar) || (mCharType == LfChar)
            || (mCharType == EofChar)) {
            break;
        } else {
            mTokenString += *mChar;
        }
    }

    if (mCharType == DoubleQuoteChar) {
        mTokenType = TokenType::StringToken;

        getNextChar();
    } else {
        mTokenType = TokenType::InvalidToken;
        mTokenComment = tr("The string is incomplete.");
    }
}

//==============================================================================

void CellmlTextViewScanner::getNextToken()
{
    // Get the next token in our text by first skipping all the spaces and
    // special characters

    while (   (mCharType == SpaceChar) || (mCharType == TabChar)
           || (mCharType == CrChar) || (mCharType == LfChar)) {
        getNextChar();
    }

    // Determine the type of our next token

    mTokenLine = mCharLine;
    mTokenColumn = mCharColumn;

    mTokenString = *mChar;

    mTokenComment = QString();

    switch (mCharType) {
    case LetterChar:
    case UnderscoreChar:
        getWord();

        break;
    case DigitChar:
    case FullStopChar:
        getNumber();

        break;
    case DoubleQuoteChar:
        getString();

        break;
    case EqChar:
        mTokenType = TokenType::EqToken;

        getNextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = TokenType::EqEqToken;

            getNextChar();
        }

        break;
    case LtChar:
        mTokenType = TokenType::LtToken;

        getNextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = TokenType::LeqToken;

            getNextChar();
        } else if (mCharType == GtChar) {
            mTokenString += *mChar;

            mTokenType = TokenType::NeqToken;

            getNextChar();
        }

        break;
    case GtChar:
        mTokenType = TokenType::GtToken;

        getNextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = TokenType::GeqToken;

            getNextChar();
        }

        break;
    case DivideChar:
        mTokenType = TokenType::DivideToken;

        getNextChar();

        if (mCharType == DivideChar)
            getSingleLineComment();
        else if (mCharType == TimesChar)
            getMultilineComment();

        break;
    case OpeningCurlyBracketChar:
    case ClosingCurlyBracketChar:
        mTokenType = (mCharType == OpeningCurlyBracketChar)?
                         TokenType::OpeningCurlyBracketToken:
                         TokenType::ClosingCurlyBracketToken;
        mWithinParameterBlock = mCharType == OpeningCurlyBracketChar;

        getNextChar();

        break;
    case EofChar:
        mTokenType = TokenType::EofToken;
        mTokenString = tr("the end of the file");

        break;
    default:
        switch (mCharType) {
        case QuoteChar:          mTokenType = TokenType::QuoteToken;          break;
        case CommaChar:          mTokenType = TokenType::CommaToken;          break;
        case PlusChar:           mTokenType = TokenType::PlusToken;           break;
        case MinusChar:          mTokenType = TokenType::MinusToken;          break;
        case TimesChar:          mTokenType = TokenType::TimesToken;          break;
        case ColonChar:          mTokenType = TokenType::ColonToken;          break;
        case SemiColonChar:      mTokenType = TokenType::SemiColonToken;      break;
        case OpeningBracketChar: mTokenType = TokenType::OpeningBracketToken; break;
        case ClosingBracketChar: mTokenType = TokenType::ClosingBracketToken; break;
        default:                 mTokenType = TokenType::UnknownToken;
        }

        getNextChar();
    }
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
    mChar(0),
    mCharType(EofChar),
    mCharLine(1),
    mCharColumn(0),
    mTokenType(UnknownToken),
    mTokenLine(0),
    mTokenColumn(0),
    mTokenString(QString()),
    mTokenComment(QString()),
    mWithinParameterBlock(false)
{
    // Our various CellML Text keywords

    mKeywords.insert("and", AndToken);
    mKeywords.insert("as", AsToken);
    mKeywords.insert("between", BetweenToken);
    mKeywords.insert("case", CaseToken);
    mKeywords.insert("comp", CompToken);
    mKeywords.insert("def", DefToken);
    mKeywords.insert("endcomp", EndCompToken);
    mKeywords.insert("enddef", EndDefToken);
    mKeywords.insert("endsel", EndSelToken);
    mKeywords.insert("for", ForToken);
    mKeywords.insert("group", GroupToken);
    mKeywords.insert("import", ImportToken);
    mKeywords.insert("incl", InclToken);
    mKeywords.insert("map", MapToken);
    mKeywords.insert("model", ModelToken);
    mKeywords.insert("otherwise", OtherwiseToken);
    mKeywords.insert("sel", SelToken);
    mKeywords.insert("unit", UnitToken);
    mKeywords.insert("using", UsingToken);
    mKeywords.insert("var", VarToken);
    mKeywords.insert("vars", VarsToken);

    mKeywords.insert("abs", AbsToken);
    mKeywords.insert("ceil", CeilToken);
    mKeywords.insert("exp", ExpToken);
    mKeywords.insert("fact", FactToken);
    mKeywords.insert("floor", FloorToken);
    mKeywords.insert("ln", LnToken);
    mKeywords.insert("log", LogToken);
    mKeywords.insert("pow", PowToken);
    mKeywords.insert("rem", RemToken);
    mKeywords.insert("root", RootToken);
    mKeywords.insert("sqr", SqrToken);
    mKeywords.insert("sqrt", SqrtToken);

//    mKeywords.insert("and", AndToken);
    mKeywords.insert("or", OrToken);
    mKeywords.insert("xor", XorToken);
    mKeywords.insert("not", NotToken);

    mKeywords.insert("ode", OdeToken);

    mKeywords.insert("min", MinToken);
    mKeywords.insert("max", MaxToken);

    mKeywords.insert("gcd", GcdToken);
    mKeywords.insert("lcm", LcmToken);

    mKeywords.insert("sin", SinToken);
    mKeywords.insert("cos", CosToken);
    mKeywords.insert("tan", TanToken);
    mKeywords.insert("sec", SecToken);
    mKeywords.insert("csc", CscToken);
    mKeywords.insert("cot", CotToken);
    mKeywords.insert("sinh", SinhToken);
    mKeywords.insert("cosh", CoshToken);
    mKeywords.insert("tanh", TanhToken);
    mKeywords.insert("sech", SechToken);
    mKeywords.insert("csch", CschToken);
    mKeywords.insert("coth", CothToken);
    mKeywords.insert("asin", AsinToken);
    mKeywords.insert("acos", AcosToken);
    mKeywords.insert("atan", AtanToken);
    mKeywords.insert("asec", AsecToken);
    mKeywords.insert("acsc", AcscToken);
    mKeywords.insert("acot", AcotToken);
    mKeywords.insert("asinh", AsinhToken);
    mKeywords.insert("acosh", AcoshToken);
    mKeywords.insert("atanh", AtanhToken);
    mKeywords.insert("asech", AsechToken);
    mKeywords.insert("acsch", AcschToken);
    mKeywords.insert("acoth", AcothToken);

    mKeywords.insert("true", TrueToken);
    mKeywords.insert("false", FalseToken);
    mKeywords.insert("nan", NanToken);
    mKeywords.insert("pi", PiToken);
    mKeywords.insert("inf", InfToken);
    mKeywords.insert("e", EToken);

    mKeywords.insert("base", BaseToken);
    mKeywords.insert("encapsulation", EncapsulationToken);
    mKeywords.insert("containment", ContainmentToken);

    // Our various CellML Text SI unit keywords

    mSiUnitKeywords.insert("ampere", AmpereToken);
    mSiUnitKeywords.insert("becquerel", BecquerelToken);
    mSiUnitKeywords.insert("candela", CandelaToken);
    mSiUnitKeywords.insert("celsius", CelsiusToken);
    mSiUnitKeywords.insert("coulomb", CoulombToken);
    mSiUnitKeywords.insert("dimensionless", DimensionlessToken);
    mSiUnitKeywords.insert("farad", FaradToken);
    mSiUnitKeywords.insert("gram", GramToken);
    mSiUnitKeywords.insert("gray", GrayToken);
    mSiUnitKeywords.insert("henry", HenryToken);
    mSiUnitKeywords.insert("hertz", HertzToken);
    mSiUnitKeywords.insert("joule", JouleToken);
    mSiUnitKeywords.insert("katal", KatalToken);
    mSiUnitKeywords.insert("kelvin", KelvinToken);
    mSiUnitKeywords.insert("kilogram", KilogramToken);
    mSiUnitKeywords.insert("liter", LiterToken);
    mSiUnitKeywords.insert("litre", LitreToken);
    mSiUnitKeywords.insert("lumen", LumenToken);
    mSiUnitKeywords.insert("lux", LuxToken);
    mSiUnitKeywords.insert("meter", MeterToken);
    mSiUnitKeywords.insert("metre", MetreToken);
    mSiUnitKeywords.insert("mole", MoleToken);
    mSiUnitKeywords.insert("newton", NewtonToken);
    mSiUnitKeywords.insert("ohm", OhmToken);
    mSiUnitKeywords.insert("pascal", PascalToken);
    mSiUnitKeywords.insert("radian", RadianToken);
    mSiUnitKeywords.insert("second", SecondToken);
    mSiUnitKeywords.insert("siemens", SiemensToken);
    mSiUnitKeywords.insert("sievert", SievertToken);
    mSiUnitKeywords.insert("steradian", SteradianToken);
    mSiUnitKeywords.insert("tesla", TeslaToken);
    mSiUnitKeywords.insert("volt", VoltToken);
    mSiUnitKeywords.insert("watt", WattToken);
    mSiUnitKeywords.insert("weber", WeberToken);

    // Our various CellML Text parameter keywords

    mParameterKeywords.insert("pref", PrefToken);
    mParameterKeywords.insert("expo", ExpoToken);
    mParameterKeywords.insert("mult", MultToken);
    mParameterKeywords.insert("off", OffToken);

    mParameterKeywords.insert("init", InitToken);
    mParameterKeywords.insert("pub", PubToken);
    mParameterKeywords.insert("priv", PrivToken);

    mParameterKeywords.insert("yotta", YottaToken);
    mParameterKeywords.insert("zetta", ZettaToken);
    mParameterKeywords.insert("exa", ExaToken);
    mParameterKeywords.insert("peta", PetaToken);
    mParameterKeywords.insert("tera", TeraToken);
    mParameterKeywords.insert("giga", GigaToken);
    mParameterKeywords.insert("mega", MegaToken);
    mParameterKeywords.insert("kilo", KiloToken);
    mParameterKeywords.insert("hecto", HectoToken);
    mParameterKeywords.insert("deka", DekaToken);
    mParameterKeywords.insert("deci", DeciToken);
    mParameterKeywords.insert("centi", CentiToken);
    mParameterKeywords.insert("milli", MilliToken);
    mParameterKeywords.insert("micro", MicroToken);
    mParameterKeywords.insert("nano", NanoToken);
    mParameterKeywords.insert("pico", PicoToken);
    mParameterKeywords.insert("femto", FemtoToken);
    mParameterKeywords.insert("atto", AttoToken);
    mParameterKeywords.insert("zepto", ZeptoToken);
    mParameterKeywords.insert("yocto", YoctoToken);

    mParameterKeywords.insert("in", InToken);
    mParameterKeywords.insert("out", OutToken);
    mParameterKeywords.insert("none", NoneToken);
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

    mTokenType = UnknownToken;
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

    mTokenType = SingleLineCommentToken;
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

                mTokenType = MultilineCommentToken;

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

    mTokenType = InvalidToken;
    mTokenComment = QObject::tr("The comment is incomplete.");
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
        mTokenType = mParameterKeywords.value(mTokenString, UnknownToken);
    else
        mTokenType = mKeywords.value(mTokenString, UnknownToken);

    if (mTokenType == UnknownToken)
        mTokenType = mSiUnitKeywords.value(mTokenString, UnknownToken);

    if (mTokenType == UnknownToken) {
        // We are not dealing with a keyword, but it might still be an
        // identifier or cmeta:id, as long as it doesn't only consist of
        // underscores, hyphens and periods

        static const QRegularExpression UnderscoresHyphensOrPeriodsRegEx = QRegularExpression("_|-|\\.");
        static const QRegularExpression HyphensOrPeriodsRegEx = QRegularExpression("-|\\.");

        if (!QString(mTokenString).remove(UnderscoresHyphensOrPeriodsRegEx).isEmpty()) {
            if (mTokenString.contains(HyphensOrPeriodsRegEx))
                mTokenType = ProperCmetaIdToken;
            else
                mTokenType = IdentifierOrCmetaIdToken;
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

            mTokenType = UnknownToken;

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

            mTokenType = InvalidToken;
            mTokenComment = QObject::tr("The exponent has no digits.");

            return;
        }
    }

    // At this stage, we have a number, but it may be invalid

    bool validNumber;

    mTokenString.toDouble(&validNumber);

    mTokenType = NumberToken;

    if (!validNumber)
        mTokenComment = QObject::tr("The number is not valid (e.g. too big, too small).");
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
        mTokenType = StringToken;

        getNextChar();
    } else {
        mTokenType = InvalidToken;
        mTokenComment = QObject::tr("The string is incomplete.");
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
        mTokenType = EqToken;

        getNextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = EqEqToken;

            getNextChar();
        }

        break;
    case LtChar:
        mTokenType = LtToken;

        getNextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = LeqToken;

            getNextChar();
        } else if (mCharType == GtChar) {
            mTokenString += *mChar;

            mTokenType = NeqToken;

            getNextChar();
        }

        break;
    case GtChar:
        mTokenType = GtToken;

        getNextChar();

        if (mCharType == EqChar) {
            mTokenString += *mChar;

            mTokenType = GeqToken;

            getNextChar();
        }

        break;
    case DivideChar:
        mTokenType = DivideToken;

        getNextChar();

        if (mCharType == DivideChar)
            getSingleLineComment();
        else if (mCharType == TimesChar)
            getMultilineComment();

        break;
    case OpeningCurlyBracketChar:
    case ClosingCurlyBracketChar:
        mTokenType = (mCharType == OpeningCurlyBracketChar)?
                         OpeningCurlyBracketToken:
                         ClosingCurlyBracketToken;
        mWithinParameterBlock = mCharType == OpeningCurlyBracketChar;

        getNextChar();

        break;
    case EofChar:
        mTokenType = EofToken;
        mTokenString = QObject::tr("the end of the file");

        break;
    default:
        switch (mCharType) {
        case QuoteChar:          mTokenType = QuoteToken;          break;
        case CommaChar:          mTokenType = CommaToken;          break;
        case PlusChar:           mTokenType = PlusToken;           break;
        case MinusChar:          mTokenType = MinusToken;          break;
        case TimesChar:          mTokenType = TimesToken;          break;
        case ColonChar:          mTokenType = ColonToken;          break;
        case SemiColonChar:      mTokenType = SemiColonToken;      break;
        case OpeningBracketChar: mTokenType = OpeningBracketToken; break;
        case ClosingBracketChar: mTokenType = ClosingBracketToken; break;
        default:                 mTokenType = UnknownToken;
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

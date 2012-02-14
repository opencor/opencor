//==============================================================================
// Computer scanner class
//==============================================================================
// Note: the lexer is based on the LEX description of the ANSI C language:
//       http://www.lysator.liu.se/c/ANSI-C-grammar-l.html
//==============================================================================

#include "computerscanner.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

static const QChar Underscore           = QChar('_');
static const QChar OpeningBracket       = QChar('(');
static const QChar ClosingBracket       = QChar(')');
static const QChar OpeningCurlyBracket  = QChar('{');
static const QChar ClosingCurlyBracket  = QChar('}');
static const QChar OpeningSquareBracket = QChar('[');
static const QChar ClosingSquareBracket = QChar(']');
static const QChar Equal                = QChar('=');
static const QChar Comma                = QChar(',');
static const QChar Colon                = QChar(':');
static const QChar SemiColon            = QChar(';');
static const QChar QuestionMark         = QChar('?');
static const QChar ExclamationMark      = QChar('!');
static const QChar Or                   = QChar('|');
static const QChar Xor                  = QChar('^');
static const QChar And                  = QChar('&');
static const QChar LowerThan            = QChar('<');
static const QChar GreaterThan          = QChar('>');
static const QChar Times                = QChar('*');
static const QChar Divide               = QChar('/');
static const QChar Modulo               = QChar('%');
static const QChar Plus                 = QChar('+');
static const QChar Minus                = QChar('-');
static const QChar FullStop             = QChar('.');
static const QChar LowerCaseE           = QChar('e');
static const QChar UpperCaseE           = QChar('E');

//==============================================================================

ComputerScannerToken::ComputerScannerToken(const int pLine, const int pColumn) :
    mLine(pLine),
    mColumn(pColumn),
    mSymbol(Eof),
    mString(QString())
{
}

//==============================================================================

int ComputerScannerToken::line() const
{
    // Return the token's line

    return mLine;
}

//==============================================================================

int ComputerScannerToken::column() const
{
    // Return the token's column

    return mColumn;
}

//==============================================================================

ComputerEquation::Type ComputerScannerToken::equationType() const
{
    // Return the token's equation type equivalent

    switch (mSymbol) {
    // Mathematical operators

    case Times:
        return ComputerEquation::Times;
    case Divide:
        return ComputerEquation::Divide;
    case Modulo:
        return ComputerEquation::Modulo;
    case Plus:
        return ComputerEquation::Plus;
    case Minus:
        return ComputerEquation::Minus;
    case Or:
        return ComputerEquation::Or;
    case Xor:
        return ComputerEquation::Xor;
    case And:
        return ComputerEquation::And;
    case EqualEqual:
        return ComputerEquation::EqualEqual;
    case NotEqual:
        return ComputerEquation::NotEqual;
    case LowerThan:
        return ComputerEquation::LowerThan;
    case GreaterThan:
        return ComputerEquation::GreaterThan;
    case LowerOrEqualThan:
        return ComputerEquation::LowerOrEqualThan;
    case GreaterOrEqualThan:
        return ComputerEquation::GreaterOrEqualThan;

    // Mathematical functions with 1 argument

    case Abs:
        return ComputerEquation::Abs;
    case Exp:
        return ComputerEquation::Exp;
    case Log:
        return ComputerEquation::Log;
    case Ceil:
        return ComputerEquation::Ceil;
    case Floor:
        return ComputerEquation::Floor;
    case Factorial:
        return ComputerEquation::Factorial;
    case Sin:
        return ComputerEquation::Sin;
    case Cos:
        return ComputerEquation::Cos;
    case Tan:
        return ComputerEquation::Tan;
    case Sinh:
        return ComputerEquation::Sinh;
    case Cosh:
        return ComputerEquation::Cosh;
    case Tanh:
        return ComputerEquation::Tanh;
    case Asin:
        return ComputerEquation::Asin;
    case Acos:
        return ComputerEquation::Acos;
    case Atan:
        return ComputerEquation::Atan;
    case Asinh:
        return ComputerEquation::Asinh;
    case Acosh:
        return ComputerEquation::Acosh;
    case Atanh:
        return ComputerEquation::Atanh;

    // Mathematical functions with 2 arguments

    case ArbitraryLog:
        return ComputerEquation::ArbitraryLog;
    case Pow:
        return ComputerEquation::Pow;
    case Quotient:
        return ComputerEquation::Quotient;
    case Rem:
        return ComputerEquation::Rem;

    // Mathematical functions with 2+ arguments

    case Gcd:
        return ComputerEquation::Gcd;
    case Lcm:
        return ComputerEquation::Lcm;
    case Max:
        return ComputerEquation::Max;
    case Min:
        return ComputerEquation::Min;

    // Miscellaneous

    case Not:
        return ComputerEquation::Not;

    // We should never reach this point...

    default:
        qCritical("ComputerScannerToken::equationType: the symbol '%s' could not be converted to an equation node type. This is a bug! Please fix it!", qPrintable(symbolAsString()));

        return ComputerEquation::Unknown;
    }
}

//==============================================================================

ComputerScannerToken::Symbol ComputerScannerToken::symbol() const
{
    // Return the token's symbol

    return mSymbol;
}

//==============================================================================

QString ComputerScannerToken::symbolAsString() const
{
    // Return the token's symbol as a string
    // Note: this method is mainly used for debugging purposes, though who knows
    //       it may someday prove useful in some other cases...

    switch (mSymbol) {
    case Void:
        return "Void";
    case Double:
        return "Double";
    case Times:
        return "Times";
    case Divide:
        return "Divide";
    case Modulo:
        return "Modulo";
    case Plus:
        return "Plus";
    case Minus:
        return "Minus";
    case Or:
        return "Or";
    case Xor:
        return "Xor";
    case And:
        return "And";
    case EqualEqual:
        return "EqualEqual";
    case NotEqual:
        return "NotEqual";
    case LowerThan:
        return "LowerThan";
    case GreaterThan:
        return "GreaterThan";
    case LowerOrEqualThan:
        return "LowerOrEqualThan";
    case GreaterOrEqualThan:
        return "GreaterOrEqualThan";
    case Abs:
        return "Abs";
    case Exp:
        return "Exp";
    case Log:
        return "Log";
    case Ceil:
        return "Ceil";
    case Floor:
        return "Floor";
    case Factorial:
        return "Factorial";
    case Sin:
        return "Sin";
    case Cos:
        return "Cos";
    case Tan:
        return "Tan";
    case Sinh:
        return "Sinh";
    case Cosh:
        return "Cosh";
    case Tanh:
        return "Tanh";
    case Asin:
        return "Asin";
    case Acos:
        return "Acos";
    case Atan:
        return "Atan";
    case Asinh:
        return "Asinh";
    case Acosh:
        return "Acosh";
    case Atanh:
        return "Atanh";
    case ArbitraryLog:
        return "ArbitraryLog";
    case Pow:
        return "Pow";
    case Quotient:
        return "Quotient";
    case Rem:
        return "Rem";
    case Gcd:
        return "Gcd";
    case Lcm:
        return "Lcm";
    case Max:
        return "Max";
    case Min:
        return "Min";
    case OpeningBracket:
        return "OpeningBracket";
    case ClosingBracket:
        return "ClosingBracket";
    case OpeningCurlyBracket:
        return "OpeningCurlyBracket";
    case ClosingCurlyBracket:
        return "ClosingCurlyBracket";
    case OpeningSquareBracket:
        return "OpeningSquareBracket";
    case ClosingSquareBracket:
        return "ClosingSquareBracket";
    case Equal:
        return "Equal";
    case Not:
        return "Not";
    case Comma:
        return "Comma";
    case Colon:
        return "Colon";
    case SemiColon:
        return "SemiColon";
    case QuestionMark:
        return "QuestionMark";
    case Unknown:
        return "Unknown";
    case Identifier:
        return "Identifier";
    case IntegerValue:
        return "IntegerValue";
    case DoubleValue:
        return "DoubleValue";
    case Return:
        return "Return";
    case Eof:
        return "Eof";
    default:
        return "???";
    }
}

//==============================================================================

void ComputerScannerToken::setSymbol(const Symbol &pSymbol)
{
    // Set the token's symbol

    mSymbol = pSymbol;
}

//==============================================================================

QString ComputerScannerToken::string() const
{
    // Return the token's string

    return mString;
}

//==============================================================================

void ComputerScannerToken::setString(const QString &pString)
{
    // Set the token's string

    mString = pString;
}

//==============================================================================

ComputerScanner::ComputerScanner() :
    mInput(QString()),
    mPosition(0),
    mLastPosition(0),
    mChar(QChar()),
    mLine(1),
    mColumn(0)
{
    // Keywords for our small C mathematical grammar

    mKeywords.insert("void", ComputerScannerToken::Void);
    mKeywords.insert("double", ComputerScannerToken::Double);

    mKeywords.insert("fabs", ComputerScannerToken::Abs);
    mKeywords.insert("exp", ComputerScannerToken::Exp);
    mKeywords.insert("log", ComputerScannerToken::Log);
    mKeywords.insert("ceil", ComputerScannerToken::Ceil);
    mKeywords.insert("floor", ComputerScannerToken::Floor);
    mKeywords.insert("factorial", ComputerScannerToken::Factorial);
    mKeywords.insert("sin", ComputerScannerToken::Sin);
    mKeywords.insert("cos", ComputerScannerToken::Cos);
    mKeywords.insert("tan", ComputerScannerToken::Tan);
    mKeywords.insert("sinh", ComputerScannerToken::Sinh);
    mKeywords.insert("cosh", ComputerScannerToken::Cosh);
    mKeywords.insert("tanh", ComputerScannerToken::Tanh);
    mKeywords.insert("asin", ComputerScannerToken::Asin);
    mKeywords.insert("acos", ComputerScannerToken::Acos);
    mKeywords.insert("atan", ComputerScannerToken::Atan);
    mKeywords.insert("asinh", ComputerScannerToken::Asinh);
    mKeywords.insert("acosh", ComputerScannerToken::Acosh);
    mKeywords.insert("atanh", ComputerScannerToken::Atanh);

    mKeywords.insert("arbitraryLog", ComputerScannerToken::ArbitraryLog);
    mKeywords.insert("pow", ComputerScannerToken::Pow);
    mKeywords.insert("quotient", ComputerScannerToken::Quotient);
    mKeywords.insert("rem", ComputerScannerToken::Rem);

    mKeywords.insert("gcd", ComputerScannerToken::Gcd);
    mKeywords.insert("lcm", ComputerScannerToken::Lcm);
    mKeywords.insert("max", ComputerScannerToken::Max);
    mKeywords.insert("min", ComputerScannerToken::Min);

    mKeywords.insert("return", ComputerScannerToken::Return);
}

//==============================================================================

void ComputerScanner::initialise(const QString &pInput)
{
    // Initialise a few things

    mInput = pInput;
    mPosition = 0;
    mLastPosition = pInput.length();
    mChar = QChar(' ');   // Note: we initialise mChar with a space character,
                          //       so that we can get our first token
    mLine = 1;
    mColumn = 0;

    // Get the first token

    getNextToken();
}

//==============================================================================

void ComputerScanner::getNextChar()
{
    if (mPosition == mLastPosition) {
       // End of the input, so return an empty character

        mChar = QChar();

        // Update the column number

        ++mColumn;
    } else {
        // Not at the end of the input, so retrieve the current character

        mChar = mInput.at(mPosition);

        // Check whether the current character is a line feed

        if (mChar == QChar(10)) {
            // The current character is a line feed, so start a new line

            ++mLine;
            mColumn = 0;
        } else {
            // Update the column number

            ++mColumn;
        }

        // Get ready for the next character

        ++mPosition;
    }
}

//==============================================================================

void ComputerScanner::getWord()
{
    // Retrieve a word which EBNF grammar is as follows:
    //
    //   Word   = ( Letter | "_" ) { Letter | Digit | "_" } ;
    //   Letter = "a" | ... | "z" | "A" | ... "Z" ;
    //   Digit  = "0" | ... | "9" ;

    QString word = QString(mChar);

    getNextChar();

    while (mChar.isLetter() || mChar.isDigit() || (mChar == Underscore)) {
        // The new current character is either a letter, digit or underscore, so
        // add it to our word

        word += mChar;

        getNextChar();
    }

    // Update the token with the word we have just scanned

    mToken.setString(word);

    // Check whether the word is a known keyword

    ComputerScannerKeywords::const_iterator keyword = mKeywords.find(word);

    if (keyword != mKeywords.end()) {
        // The word we scanned is a known keyword, so retrieve its corresponding
        // symbol

        mToken.setSymbol(keyword.value());
    } else {
        // The word we scanned is not a keyword, so it has to be an identifier,
        // unless it's only made of underscores, so remove all the underscores
        // from our word and check whether we end up with an empty string

        word.replace(Underscore, "");

        if (word.isEmpty())
            // The word we scanned only contains underscores, so we are dealing
            // with an unknown symbol

            mToken.setSymbol(ComputerScannerToken::Unknown);
        else
            // The word we scanned doesn't only contain underscores, so we are
            // dealing with an identifier

            mToken.setSymbol(ComputerScannerToken::Identifier);
    }
}

//==============================================================================

void ComputerScanner::getNumber()
{
    // Retrieve a number which EBNF grammar is as follows:
    //
    //   Number   =   ( { Digit } "." Digit { Digit } [ Exponent ] )
    //              | ( Digit { Digit } [ "." { Digit } ] [ Exponent ] ) ;
    //   Exponent = ( "e" | "E" ) [ "+" | "-" ] Digit { Digit } ;

    // By default, we assume that we have got an unknown token

    mToken.setSymbol(ComputerScannerToken::Unknown);

    // Check whether we came here with a digit

    QString number = QString();
    bool firstCharIsDigit = mChar.isDigit();

    if (firstCharIsDigit)
        // We came here with a digit, so keep looking for more digits

        do {
            number += mChar;

            getNextChar();
        } while (mChar.isDigit());

    // If the current character is a full stop, then we should have 1+ digits if
    // the number started with a full stop or 0+ digits otherwise

    bool hasFractionalPart = mChar == FullStop;

    if (hasFractionalPart) {
        number += mChar;

        getNextChar();

        if (!firstCharIsDigit) {
            // The first character was a full stop, so the next character must
            // be a digit

            if (mChar.isDigit()) {
                number += mChar;

                getNextChar();
            } else {
                // We didn't get a digit, so...

                mToken.setString(number);

                return;
            }
        }

        // We must now look for 0+ digits

        while (mChar.isDigit()) {
            number += mChar;

            getNextChar();
        }
    }

    // If we are still dealing with a number, then the next character should be
    // either "e" or "E". If it is not, then the error processing will depend on
    // whether an exponent was needed or not...

    bool hasExponentPart = (mChar == LowerCaseE) || (mChar == UpperCaseE);

    if (hasExponentPart) {
        // We got either "e" or "E"

        number += mChar;

        getNextChar();

        // The next character can be "+" or "-"

        if ((mChar == Plus) || (mChar == Minus)) {
            // We got either "+" or "-"

            number += mChar;

            getNextChar();
        }

        // Now, we should have 1+ digits

        if (mChar.isDigit()) {
            // We got a digit, so add it and keep looking for more digits

            number += mChar;

            getNextChar();

            while (mChar.isDigit()) {
                number += mChar;

                getNextChar();
            }
        } else {
            // We didn't get a digit, so...

            mToken.setString(number);

            return;
        }
    }

    // At this stage, we've got a number. The question is whether it is an
    // integer value or a double value

    if (hasFractionalPart || hasExponentPart)
        // The number has a fractional part and/or an exponential part, so it is
        // a double number

        mToken.setSymbol(ComputerScannerToken::DoubleValue);
    else
        // The number has neither a fractional part nor an exponential part, so
        // it is an integer number

        mToken.setSymbol(ComputerScannerToken::IntegerValue);

    // Update the token's string

    mToken.setString(number);
}

//==============================================================================

ComputerScannerToken ComputerScanner::token()
{
    // Return the token

    return mToken;
}

//==============================================================================

void ComputerScanner::getNextToken()
{
    // Skip spaces of all sorts
    // Note: we must test the current character first before getting a new
    //       one in case two tokens follow one another without any space between
    //       them...

    if (mChar.isSpace())
        do {
            getNextChar();
        } while (mChar.isSpace());

    // Initialise the token

    mToken = ComputerScannerToken(mLine, mColumn);

    // Check the type of the current character

    if (mChar.isLetter() || (mChar == Underscore)) {
        // The current character is a letter or an underscore, so we should try
        // to retrieve a word

        getWord();
    } else if (mChar.isDigit() || (mChar == FullStop)) {
        // The current character is a digit or a full stop, so we should try to
        // retrieve a number

        getNumber();
    } else {
        // Neither a word nor a number, so it has to be a one- or two-character
        // token

        mToken.setString(mChar);

        if (mChar == OpeningBracket) {
            mToken.setSymbol(ComputerScannerToken::OpeningBracket);

            getNextChar();
        } else if (mChar == ClosingBracket) {
            mToken.setSymbol(ComputerScannerToken::ClosingBracket);

            getNextChar();
        } else if (mChar == OpeningCurlyBracket) {
            mToken.setSymbol(ComputerScannerToken::OpeningCurlyBracket);

            getNextChar();
        } else if (mChar == ClosingCurlyBracket) {
            mToken.setSymbol(ComputerScannerToken::ClosingCurlyBracket);

            getNextChar();
        } else if (mChar == OpeningSquareBracket) {
            mToken.setSymbol(ComputerScannerToken::OpeningSquareBracket);

            getNextChar();
        } else if (mChar == ClosingSquareBracket) {
            mToken.setSymbol(ComputerScannerToken::ClosingSquareBracket);

            getNextChar();
        } else if (mChar == Equal) {
            // Depending on the next character, the symbol may be an EqualEqual
            // rather than an Equal

            getNextChar();

            if (mChar == Equal) {
                // We got another Equal, so...

                mToken.setSymbol(ComputerScannerToken::EqualEqual);

                getNextChar();
            } else {
                // Only one Equal, so...

                mToken.setSymbol(ComputerScannerToken::Equal);
            }
        } else if (mChar == ExclamationMark) {
            // Depending on the next character, the symbol may be a NotEqual
            // rather than a Not

            getNextChar();

            if (mChar == Equal) {
                // We got an Equal, so...

                mToken.setSymbol(ComputerScannerToken::NotEqual);

                getNextChar();
            } else {
                // Only an exclamation mark, so...

                mToken.setSymbol(ComputerScannerToken::Not);
            }
        } else if (mChar == Comma) {
            mToken.setSymbol(ComputerScannerToken::Comma);

            getNextChar();
        } else if (mChar == Colon) {
            mToken.setSymbol(ComputerScannerToken::Colon);

            getNextChar();
        } else if (mChar == SemiColon) {
            mToken.setSymbol(ComputerScannerToken::SemiColon);

            getNextChar();
        } else if (mChar == QuestionMark) {
            mToken.setSymbol(ComputerScannerToken::QuestionMark);

            getNextChar();
        } else if (mChar == Times) {
            mToken.setSymbol(ComputerScannerToken::Times);

            getNextChar();
        } else if (mChar == Divide) {
            mToken.setSymbol(ComputerScannerToken::Divide);

            getNextChar();
        } else if (mChar == Modulo) {
            mToken.setSymbol(ComputerScannerToken::Modulo);

            getNextChar();
        } else if (mChar == Plus) {
            mToken.setSymbol(ComputerScannerToken::Plus);

            getNextChar();
        } else if (mChar == Minus) {
            mToken.setSymbol(ComputerScannerToken::Minus);

            getNextChar();
        } else if (mChar == Or) {
            // We don't support only one Or character, so we need a second one
            // to get a Or symbol

            getNextChar();

            if (mChar == Or) {
                // We got another Or, so...

                mToken.setSymbol(ComputerScannerToken::Or);

                getNextChar();
            } else {
                // Only one Or, so...

                mToken.setSymbol(ComputerScannerToken::Unknown);
            }
        } else if (mChar == Xor) {
            mToken.setSymbol(ComputerScannerToken::Xor);

            getNextChar();
        } else if (mChar == And) {
            // We don't support only one And character, so we need a second one
            // to get a And symbol

            getNextChar();

            if (mChar == And) {
                // We got another And, so...

                mToken.setSymbol(ComputerScannerToken::And);

                getNextChar();
            } else {
                // Only one And, so...

                mToken.setSymbol(ComputerScannerToken::Unknown);
            }
        } else if (mChar == LowerThan) {
            // Depending on the next character, the symbol may be a
            // LowerOrEqualThan rather than a LowerThan

            getNextChar();

            if (mChar == Equal) {
                // We got an Equal, so...

                mToken.setSymbol(ComputerScannerToken::LowerOrEqualThan);

                getNextChar();
            } else {
                // Only a LowerThan, so...

                mToken.setSymbol(ComputerScannerToken::LowerThan);
            }
        } else if (mChar == GreaterThan) {
            // Depending on the next character, the symbol may be a
            // GreaterOrEqualThan rather than a GreaterThan

            getNextChar();

            if (mChar == Equal) {
                // We got an Equal, so...

                mToken.setSymbol(ComputerScannerToken::GreaterOrEqualThan);

                getNextChar();
            } else {
                // Only a GreaterThan, so...

                mToken.setSymbol(ComputerScannerToken::GreaterThan);
            }
        }
    }
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

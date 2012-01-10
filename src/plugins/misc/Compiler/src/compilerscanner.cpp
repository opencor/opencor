//==============================================================================
// Compiler scanner class
//==============================================================================

#include "compilerscanner.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

static const QChar Underscore          = QChar('_');
static const QChar Times               = QChar('*');
static const QChar Plus                = QChar('+');
static const QChar Minus               = QChar('-');
static const QChar OpeningBracket      = QChar('(');
static const QChar ClosingBracket      = QChar(')');
static const QChar OpeningCurlyBracket = QChar('{');
static const QChar ClosingCurlyBracket = QChar('}');
static const QChar Comma               = QChar(',');
static const QChar SemiColon           = QChar(';');
static const QChar FullStop            = QChar('.');
static const QChar LowerCaseE          = QChar('e');
static const QChar UpperCaseE          = QChar('E');

//==============================================================================

CompilerScannerToken::CompilerScannerToken(const int pLine, const int pColumn) :
    mLine(pLine),
    mColumn(pColumn),
    mSymbol(Eof),
    mString(QString())
{
}

//==============================================================================

int CompilerScannerToken::line() const
{
    // Return the token's line

    return mLine;
}

//==============================================================================

int CompilerScannerToken::column() const
{
    // Return the token's column

    return mColumn;
}

//==============================================================================

CompilerScannerToken::Symbol CompilerScannerToken::symbol() const
{
    // Return the token's symbol

    return mSymbol;
}

//==============================================================================

QString CompilerScannerToken::symbolAsString() const
{
    // Return the token's symbol as a string

    switch (mSymbol) {
    case Void:
        return "Void";
    case Double:
        return "Double";
    case Times:
        return "Times";
    case OpeningBracket:
        return "OpeningBracket";
    case ClosingBracket:
        return "ClosingBracket";
    case OpeningCurlyBracket:
        return "OpeningCurlyBracket";
    case ClosingCurlyBracket:
        return "ClosingCurlyBracket";
    case Comma:
        return "Comma";
    case SemiColon:
        return "SemiColon";
    case Return:
        return "Return";
    case Unknown:
        return "Unknown";
    case Identifier:
        return "Identifier";
    case IntegerValue:
        return "IntegerValue";
    case DoubleValue:
        return "DoubleValue";
    case Eof:
        return "Eof";
    default:
        return "???";
    }
}

//==============================================================================

void CompilerScannerToken::setSymbol(const Symbol &pSymbol)
{
    // Set the token's symbol

    mSymbol = pSymbol;
}

//==============================================================================

QString CompilerScannerToken::string() const
{
    // Return the token's string

    return mString;
}

//==============================================================================

void CompilerScannerToken::setString(const QString &pString)
{
    // Set the token's string

    mString = pString;
}

//==============================================================================

CompilerScanner::CompilerScanner(const QString &pInput) :
    mInput(pInput),
    mPosition(0),
    mLastPosition(pInput.length()),
    mChar(' '),   // Note: we initialise mChar with a space character, so that
                  //       we can get our first token
    mLine(1),
    mColumn(0)
{
    // Keywords for our small C mathematical grammar

    mKeywords.insert("void", CompilerScannerToken::Void);
    mKeywords.insert("double", CompilerScannerToken::Double);

    mKeywords.insert("return", CompilerScannerToken::Return);

    // Get the first token

    getNextToken();
}

//==============================================================================

void CompilerScanner::getNextChar()
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

void CompilerScanner::getWord()
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

    CompilerScannerKeywords::const_iterator keyword = mKeywords.find(word);

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

            mToken.setSymbol(CompilerScannerToken::Unknown);
        else
            // The word we scanned doesn't only contain underscores, so we are
            // dealing with an identifier

            mToken.setSymbol(CompilerScannerToken::Identifier);
    }
}

//==============================================================================

void CompilerScanner::getNumber()
{
    // Retrieve a number which EBNF grammar is as follows:
    //
    //   Number   =   ( { Digit } "." Digit { Digit } [ Exponent ] )
    //              | ( Digit { Digit } [ "." { Digit } ] [ Exponent ] ) ;
    //   Exponent = ( "e" | "E" ) [ "+" | "-" ] Digit { Digit } ;

    // By default, we assume that we have got an unknown token

    mToken.setSymbol(CompilerScannerToken::Unknown);

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

        mToken.setSymbol(CompilerScannerToken::DoubleValue);
    else
        // The number has neither a fractional part nor an exponential part, so
        // it is an integer number

        mToken.setSymbol(CompilerScannerToken::IntegerValue);

    // Update the token's string

    mToken.setString(number);
}

//==============================================================================

CompilerScannerToken CompilerScanner::token()
{
    // Return the token

    return mToken;
}

//==============================================================================

void CompilerScanner::getNextToken()
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

    mToken = CompilerScannerToken(mLine, mColumn);

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

        if (mChar == Times)
            mToken.setSymbol(CompilerScannerToken::Times);
        else if (mChar == OpeningBracket)
            mToken.setSymbol(CompilerScannerToken::OpeningBracket);
        else if (mChar == ClosingBracket)
            mToken.setSymbol(CompilerScannerToken::ClosingBracket);
        else if (mChar == OpeningCurlyBracket)
            mToken.setSymbol(CompilerScannerToken::OpeningCurlyBracket);
        else if (mChar == ClosingCurlyBracket)
            mToken.setSymbol(CompilerScannerToken::ClosingCurlyBracket);
        else if (mChar == Comma)
            mToken.setSymbol(CompilerScannerToken::Comma);
        else if (mChar == SemiColon)
            mToken.setSymbol(CompilerScannerToken::SemiColon);

        // Get the next character

        getNextChar();
    }
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

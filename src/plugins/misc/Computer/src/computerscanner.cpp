//==============================================================================
// Computer scanner class
//==============================================================================

#include "computerscanner.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

static const QChar Underscore           = QChar('_');
static const QChar Times                = QChar('*');
static const QChar Plus                 = QChar('+');
static const QChar Minus                = QChar('-');
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
static const QChar InclusiveOr          = QChar('|');
static const QChar ExclusiveOr          = QChar('^');
static const QChar And                  = QChar('&');
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

ComputerScannerToken::Symbol ComputerScannerToken::symbol() const
{
    // Return the token's symbol

    return mSymbol;
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

ComputerScanner::ComputerScanner(const QString &pInput) :
    mInput(pInput),
    mPosition(0),
    mLastPosition(pInput.length()),
    mChar(' '),   // Note: we initialise mChar with a space character, so that
                  //       we can get our first token
    mLine(1),
    mColumn(0)
{
    // Keywords for our small C mathematical grammar

    mKeywords.insert("void", ComputerScannerToken::Void);
    mKeywords.insert("double", ComputerScannerToken::Double);

    mKeywords.insert("return", ComputerScannerToken::Return);

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

        if (mChar == Times) {
            mToken.setSymbol(ComputerScannerToken::Times);

            getNextChar();
        } else if (mChar == OpeningBracket) {
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
            mToken.setSymbol(ComputerScannerToken::Equal);

            getNextChar();
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
        } else if (mChar == InclusiveOr) {
            // Depending on the next character, the symbol may be a logical Or
            // rather than an inclusive Or

            getNextChar();

            if (mChar == InclusiveOr) {
                // We got another inclusive Or, so...

                mToken.setSymbol(ComputerScannerToken::LogicalOr);

                getNextChar();
            } else {
                // Only one bit Or, so...

                mToken.setSymbol(ComputerScannerToken::InclusiveOr);
            }
        } else if (mChar == ExclusiveOr) {
            mToken.setSymbol(ComputerScannerToken::ExclusiveOr);

            getNextChar();
        } else if (mChar == And) {
            // Depending on the next character, the symbol may be a logical And
            // rather than an And

            getNextChar();

            if (mChar == And) {
                // We got another And, so...

                mToken.setSymbol(ComputerScannerToken::LogicalAnd);

                getNextChar();
            } else {
                // Only one And, so...

                mToken.setSymbol(ComputerScannerToken::And);
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

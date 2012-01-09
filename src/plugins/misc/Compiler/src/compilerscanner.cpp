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
static const QChar OpeningBracket      = QChar('(');
static const QChar ClosingBracket      = QChar(')');
static const QChar OpeningCurlyBracket = QChar('{');
static const QChar ClosingCurlyBracket = QChar('}');
static const QChar Comma               = QChar(',');

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
    case Unknown:
        return "Unknown";
    case Identifier:
        return "Identifier";
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

    // Get the first token

    getNextToken();
}

//==============================================================================

QChar CompilerScanner::getChar()
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

    // Return the new current character

    return mChar;
}

//==============================================================================

void CompilerScanner::getWord()
{
    // Retrieve a word which EBNF grammar is as follows:
    //
    //   Word = ( Letter | "_" ) { Letter | Digit | "_" } ;

    QString word = QString(mChar);

    while (getChar().isLetter() || mChar.isDigit() || (mChar == Underscore))
        // The new current character is either a letter, digit or underscore, so
        // add it to our word

        word += mChar;

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
        while (getChar().isSpace());

    // Initialise the token

    mToken = CompilerScannerToken(mLine, mColumn);

    // Check the type of the current character

    if (mChar.isLetter() || (mChar == Underscore)) {
        // The current character is a letter or an underscore, so we should try
        // to retrieve a word

        getWord();
    } else {
        // Not a word or a number, so it has to be a one- or two-character token

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

        // Get the next character

        getChar();
    }
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

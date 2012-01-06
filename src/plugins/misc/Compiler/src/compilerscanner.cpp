//==============================================================================
// Compiler scanner class
//==============================================================================

#include "compilerscanner.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

static const QChar Underscore          = QChar('_');
static const QChar OpeningBracket      = QChar('(');
static const QChar ClosingBracket      = QChar(')');
static const QChar OpeningCurlyBracket = QChar('{');
static const QChar ClosingCurlyBracket = QChar('}');

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
    case OpeningBracket:
        return "OpeningBracket";
    case ClosingBracket:
        return "ClosingBracket";
    case OpeningCurlyBracket:
        return "OpeningCurlyBracket";
    case ClosingCurlyBracket:
        return "ClosingCurlyBracket";
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
}

//==============================================================================

QChar CompilerScanner::getChar()
{
    if (mPosition == mLastPosition) {
       // End of the input, so return an empty character

        mChar = QChar();
    } else {
        // Not at the end of the input, so retrieve the current character

        mChar = mInput.at(mPosition);

        // Check whether the current character is a line feed

        if (mChar == QChar(10)) {
            // The current character is a line feed, so start a new line

            ++mLine;
            mColumn = 0;
        }

        // Update the column number

        ++mColumn;

        // Get ready for the next character

        ++mPosition;
    }

    // Return the new current character

    return mChar;
}

//==============================================================================

void CompilerScanner::getWord(CompilerScannerToken &pToken)
{
    // Retrieve a word starting with either a letter or an underscore (which we
    // have already scanned) followed by zero or more letters, digits and/or
    // underscores
    // EBNF: Letter|"_" { Letter|Digit|"_" } .

    QString word = QString(mChar);

    while (getChar().isLetter() || mChar.isDigit() || (mChar == Underscore))
        // The new current character is either a letter, digit or underscore, so
        // add it to our word

        word += mChar;

    // Update the token with the word we have just scanned

    pToken.setString(word);

    // Check whether the word is a known keyword

    CompilerScannerKeywords::const_iterator keyword = mKeywords.find(word);

    if (keyword != mKeywords.end()) {
        // The word we scanned is a known keyword, so retrieve its corresponding
        // symbol

        pToken.setSymbol(keyword.value());
    } else {
        // The word we scanned is not a keyword, so it has to be an identifier,
        // unless it's only made of underscores, so remove all the underscores
        // from our word and check whether we end up with an empty string

        word.replace(Underscore, "");

        if (word.isEmpty())
            // The word we scanned only contains underscores, so we are dealing
            // with an unknown symbol

            pToken.setSymbol(CompilerScannerToken::Unknown);
        else
            // The word we scanned doesn't only contain underscores, so we are
            // dealing with an identifier

            pToken.setSymbol(CompilerScannerToken::Identifier);
    }
}

//==============================================================================

CompilerScannerToken CompilerScanner::getToken()
{
    // Skip spaces of all sorts
    // Note: we must test the current character first before getting a new
    //       one in case two tokens follow one another without any space between
    //       them...

    if (mChar.isSpace())
        while (getChar().isSpace());

    // Initialise the token

    CompilerScannerToken res = CompilerScannerToken(mLine, mColumn);

    // Check the type of the current character

    if (mChar.isLetter() || (mChar == Underscore)) {
        // The current character is a letter or an underscore, so we should try
        // to retrieve a word

        getWord(res);
    } else {
        // Not a word or a number, so it has to be a one- or two-character token

        res.setString(mChar);

        if (mChar == OpeningBracket)
            res.setSymbol(CompilerScannerToken::OpeningBracket);
        else if (mChar == ClosingBracket)
            res.setSymbol(CompilerScannerToken::ClosingBracket);
        else if (mChar == OpeningCurlyBracket)
            res.setSymbol(CompilerScannerToken::OpeningCurlyBracket);
        else if (mChar == ClosingCurlyBracket)
            res.setSymbol(CompilerScannerToken::ClosingCurlyBracket);

        // Get the next character

        getChar();
    }

    // Return the token

    return res;
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

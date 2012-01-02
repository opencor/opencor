//==============================================================================
// Compiler scanner class
//==============================================================================

#include "compilerscanner.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

CompilerScannerToken::CompilerScannerToken(const int pLine, const int pColumn) :
    mLine(pLine),
    mColumn(pColumn)
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
    mLastPosition(pInput.length()-1),
    mChar(),
    mLine(1),
    mColumn(0)
{
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

        // Go to the next character

        ++mPosition;
    }

    // Return the current character

    return mChar;
}

//==============================================================================

QString CompilerScanner::getWord()
{
    // Retrieve a word consisting of letters and numbers

    QString res = QString(mChar);

    while (getChar().isLetterOrNumber())
        res += mChar;

    // Return the word

    return res;
}

//==============================================================================

CompilerScannerToken CompilerScanner::getToken()
{
    // Skip spaces

    while (getChar().isSpace());

    // Initialise the token

    CompilerScannerToken res = CompilerScannerToken(mLine, mColumn);

    // Check the type of the current character

    if (mChar.isLetter())
        // The current character is a letter, so try to get a word

        res.setString(getWord());

    // Return the token

    return res;
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

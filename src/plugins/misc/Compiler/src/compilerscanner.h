//==============================================================================
// Compiler scanner class
//==============================================================================

#ifndef COMPILERSCANNER_H
#define COMPILERSCANNER_H

//==============================================================================

#include <QMap>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

class CompilerScannerToken
{
public:
    enum Symbol
    {
        // Type of variable

        Void, Double,

        // Mathematical operators

        Times,

        // Miscellaneous

        OpeningBracket, ClosingBracket,
        OpeningCurlyBracket, ClosingCurlyBracket,
        Comma,
        Unknown, Identifier, Eof
    };

    explicit CompilerScannerToken(const int pLine = 0, const int pColumn = 0);

    int line() const;
    int column() const;

    Symbol symbol() const;
    QString symbolAsString() const;
    void setSymbol(const Symbol &pSymbol);

    QString string() const;
    void setString(const QString &pString);

private:
    int mLine;
    int mColumn;

    Symbol mSymbol;

    QString mString;
};

//==============================================================================

typedef QMap<QString, CompilerScannerToken::Symbol> CompilerScannerKeywords;

class CompilerScanner
{
public:
    explicit CompilerScanner(const QString &pInput);

    CompilerScannerToken token();
    void getNextToken();

private:
    QString mInput;
    int mPosition;
    int mLastPosition;
    QChar mChar;
    CompilerScannerToken mToken;
    int mLine;
    int mColumn;
    CompilerScannerKeywords mKeywords;

    QChar getChar();

    void getWord();
};

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

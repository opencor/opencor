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
        // Unknown symbol

        Unknown,

        // Type of variable

        Void,
        Double,

        // Miscellaneous

        Identifier
    };

    explicit CompilerScannerToken(const int pLine, const int pColumn);

    int line() const;
    int column() const;

    Symbol symbol() const;
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

    CompilerScannerToken getToken();

private:
    QString mInput;
    int mPosition;
    int mLastPosition;
    QChar mChar;
    int mLine;
    int mColumn;
    CompilerScannerKeywords mKeywords;

    QChar getChar();

    void getWord(CompilerScannerToken &pToken);
};

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

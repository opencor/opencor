//==============================================================================
// Compiler scanner class
//==============================================================================

#ifndef COMPILERSCANNER_H
#define COMPILERSCANNER_H

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

class CompilerScannerToken
{
public:
    explicit CompilerScannerToken(const int pLine, const int pColumn);

    int line() const;
    int column() const;

    QString string() const;
    void setString(const QString &pString);

private:
    int mLine;
    int mColumn;

    QString mString;
};

//==============================================================================

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

    QChar getChar();

    QString getWord();
};

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

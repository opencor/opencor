//==============================================================================
// Computer scanner class
//==============================================================================

#ifndef COMPUTERSCANNER_H
#define COMPUTERSCANNER_H

//==============================================================================

#include "computerequation.h"

//==============================================================================

#include <QMap>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class ComputerScannerToken
{
public:
    enum Symbol
    {
        // Type of function/argument

        Void, Double,

        // Mathematical operators

        Times, Divide, Plus, Minus,
        LogicalOr, LogicalAnd, EqualEqual, NotEqual,
        LowerThan, GreaterThan, LowerOrEqualThan, GreaterOrEqualThan,

        // Mathematical functions with 1 argument

        Abs, Exp, Log, Ceil, Floor, Factorial,
        Sin, Cos, Tan, SinH, CosH, TanH,
        ASin, ACos, ATan, ASinH, ACosH, ATanH,

        // Mathematical functions with 2 arguments

        ArbitraryLog, FactorOf, Pow, Quotient, Rem, XOr,

        // Mathematical functions with 2+ arguments

        GCD, LCM, Max, Min,

        // Miscellaneous

        OpeningBracket, ClosingBracket,
        OpeningCurlyBracket, ClosingCurlyBracket,
        OpeningSquareBracket, ClosingSquareBracket,
        Equal, Not, Comma, Colon, SemiColon, QuestionMark,
        Unknown, Identifier, IntegerValue, DoubleValue, Return, Eof
    };

    typedef QList<Symbol> Symbols;

    explicit ComputerScannerToken(const int pLine = 0, const int pColumn = 0);

    int line() const;
    int column() const;

    ComputerEquation::Type equationType() const;

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

typedef QMap<QString, ComputerScannerToken::Symbol> ComputerScannerKeywords;

//==============================================================================

class ComputerScanner
{
public:
    explicit ComputerScanner();

    void initialise(const QString &pInput);

    ComputerScannerToken token();
    void getNextToken();

private:
    QString mInput;
    int mPosition;
    int mLastPosition;
    QChar mChar;
    ComputerScannerToken mToken;
    int mLine;
    int mColumn;
    ComputerScannerKeywords mKeywords;

    void getNextChar();

    void getWord();
    void getNumber();
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

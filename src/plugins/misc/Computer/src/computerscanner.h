//==============================================================================
// Computer scanner class
//==============================================================================

#ifndef COMPUTERSCANNER_H
#define COMPUTERSCANNER_H

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
        Void, Double,
        OpeningBracket, ClosingBracket,
        OpeningCurlyBracket, ClosingCurlyBracket,
        OpeningSquareBracket, ClosingSquareBracket,
        Equal, Not, Comma, Colon, SemiColon, QuestionMark, ExclamationMark,
        InclusiveOr, ExclusiveOr, LogicalOr, And, LogicalAnd,
        EqualEqual, NotEqual,
        LowerThan, GreaterThan, LowerOrEqualThan, GreaterOrEqualThan,
        Times, Divide, Percentage, Plus, PlusPlus, Minus, MinusMinus,
        Return,
        Unknown, Identifier, IntegerValue, DoubleValue, Eof
    };

    typedef QList<Symbol> Symbols;

    explicit ComputerScannerToken(const int pLine = 0, const int pColumn = 0);

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

typedef QMap<QString, ComputerScannerToken::Symbol> ComputerScannerKeywords;

//==============================================================================

class ComputerScanner
{
public:
    explicit ComputerScanner(const QString &pInput);

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

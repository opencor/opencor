//==============================================================================
// Computer equation class
//==============================================================================

#ifndef COMPUTEREQUATION_H
#define COMPUTEREQUATION_H

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class ComputerEquation
{
public:
    enum Type
    {
        Unknown,   // Note: not used, but necessary for converting a
                   //       ComputerScannerToken::Symbol to a
                   //       ComputerEquation::Type...

        // Type of parameter

        DirectParameter, IndirectParameter, Number,

        // Mathematical operators

        Times, Divide, Modulo, Plus, Minus,

        // Logical operators

        Not, Or, Xor, And, EqualEqual, NotEqual,
        LowerThan, GreaterThan, LowerOrEqualThan, GreaterOrEqualThan,

        // Mathematical functions with 1 argument

        Abs, Exp, Log, Ceil, Floor, Factorial,
        Sin, Cos, Tan, Sinh, Cosh, Tanh,
        Asin, Acos, Atan, Asinh, Acosh, Atanh,

        // Mathematical functions with 2 arguments

        ArbitraryLog, Pow, Quotient, Rem,

        // Mathematical functions with 2+ arguments

        Gcd, Lcm, Max, Min,

        // Miscellaneous

        Assign,
        Piecewise, PiecewiseCases,
        OtherArguments
    };

    explicit ComputerEquation(ComputerEquation *pEquation = 0);
    explicit ComputerEquation(const Type &pType,
                              ComputerEquation *pLeft,
                              ComputerEquation *pRight = 0);
    explicit ComputerEquation(const Type &pType,
                              const int &pArgumentsCount,
                              ComputerEquation **pArguments);
    explicit ComputerEquation(const QString &pParameterName);
    explicit ComputerEquation(const QString &pParameterName,
                              const int &pParameterIndex);
    explicit ComputerEquation(const double &pNumber);
    ~ComputerEquation();

    Type type() const;
    QString typeAsString() const;

    QString parameterName() const;
    int parameterIndex() const;

    double number() const;

    ComputerEquation * left() const;
    ComputerEquation * right() const;

    void simplify();

private:
    Type mType;

    QString mParameterName;
    int mParameterIndex;

    double mNumber;

    ComputerEquation *mLeft;
    ComputerEquation *mRight;

    void initialiseFrom(ComputerEquation *pEquation);

    bool numberArguments(ComputerEquation *pArguments);

    double gcd(ComputerEquation *pLeftNode, ComputerEquation *pRightNode);
    double lcm(ComputerEquation *pLeftNode, ComputerEquation *pRightNode);
    double max(ComputerEquation *pLeftNode, ComputerEquation *pRightNode);
    double min(ComputerEquation *pLeftNode, ComputerEquation *pRightNode);

    void simplifyNode(ComputerEquation *pNode);
    void replaceNodeWithNumber(ComputerEquation *pNode, const double &pNumber);
    void replaceNodeWithChildNode(ComputerEquation *pNode,
                                  ComputerEquation *pChildNode);
};

//==============================================================================

typedef QList<ComputerEquation *> ComputerEquations;

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

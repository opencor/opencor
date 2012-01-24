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

        Times, Minus,

        // Miscellaneous

        Equal, Piecewise, PiecewiseCases
    };

    explicit ComputerEquation(const Type &pType,
                              ComputerEquation *pLeft,
                              ComputerEquation *pRight);
    explicit ComputerEquation(const QString &pParameterName);
    explicit ComputerEquation(const QString &pParameterName,
                              const int &pParameterIndex);
    explicit ComputerEquation(const double &pNumber);
    ~ComputerEquation();

    Type type() const;

    QString parameterName() const;
    int parameterIndex() const;

    double number() const;

    ComputerEquation * left() const;
    ComputerEquation * right() const;

private:
    Type mType;

    QString mParameterName;
    int mParameterIndex;

    double mNumber;

    ComputerEquation *mLeft;
    ComputerEquation *mRight;
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

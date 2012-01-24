//==============================================================================
// Computer equation class
//==============================================================================

#include "computerequation.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerEquation::ComputerEquation(const Type &pType,
                                   ComputerEquation *pLeft,
                                   ComputerEquation *pRight) :
    mType(pType),
    mParameterName(QString()),
    mParameterIndex(-1),
    mLeft(pLeft),
    mRight(pRight)
{
}

//==============================================================================

ComputerEquation::ComputerEquation(const QString &pParameterName) :
    mType(DirectParameter),
    mParameterName(pParameterName),
    mParameterIndex(-1),
    mNumber(0),
    mLeft(0),
    mRight(0)
{
}

//==============================================================================

ComputerEquation::ComputerEquation(const QString &pParameterName,
                                   const int &pParameterIndex) :
    mType(IndirectParameter),
    mParameterName(pParameterName),
    mParameterIndex(pParameterIndex),
    mNumber(0),
    mLeft(0),
    mRight(0)
{
}

//==============================================================================

ComputerEquation::ComputerEquation(const double &pNumber) :
    mType(Number),
    mParameterName(QString()),
    mParameterIndex(-1),
    mNumber(pNumber),
    mLeft(0),
    mRight(0)
{
}

//==============================================================================

ComputerEquation::~ComputerEquation()
{
    // Delete some internal objects

    delete mLeft;
    delete mRight;
}

//==============================================================================

ComputerEquation::Type ComputerEquation::type() const
{
    // Return the equation's type

    return mType;
}

//==============================================================================

QString ComputerEquation::parameterName() const
{
    // Return the equation's parameter name

    return mParameterName;
}

//==============================================================================

int ComputerEquation::parameterIndex() const
{
    // Return the equation's parameter index

    return mParameterIndex;
}

//==============================================================================

double ComputerEquation::number() const
{
    // Return the equation's number

    return mNumber;
}

//==============================================================================

ComputerEquation * ComputerEquation::left() const
{
    // Return the equation's left branch

    return mLeft;
}

//==============================================================================

ComputerEquation * ComputerEquation::right() const
{
    // Return the equation's right branch

    return mRight;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

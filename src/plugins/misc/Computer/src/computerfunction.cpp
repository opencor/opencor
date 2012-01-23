//==============================================================================
// Computer function class
//==============================================================================

#include "computerfunction.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerFunction::ComputerFunction() :
    mType(Void),
    mName(QString()),
    mEquations(ComputerEquations()),
    mParameters(ComputerParameters())
{
}

//==============================================================================

ComputerFunction::~ComputerFunction()
{
    // Delete the equations

    foreach (ComputerEquation *equation, mEquations)
        delete equation;
}

//==============================================================================

ComputerFunction::Type ComputerFunction::type() const
{
    // Return the function's type

    return mType;
}

//==============================================================================

void ComputerFunction::setType(const Type &pType)
{
    // Set the function's type

    mType = pType;
}

//==============================================================================

QString ComputerFunction::name() const
{
    // Return the function's name

    return mName;
}

//==============================================================================

void ComputerFunction::setName(const QString &pName)
{
    // Set the function's name

    mName = pName;
}

//==============================================================================

ComputerParameters ComputerFunction::parameters() const
{
    // Return the function's parameters

    return mParameters;
}

//==============================================================================

bool ComputerFunction::addParameter(const ComputerParameter &pParameter)
{
    // Add a parameter to our list, but only if it isn't already there

    if (mParameters.contains(pParameter)) {
        // The parameter already exists, so...

        return false;
    } else {
        // The parameter is not yet in our list, so add it

        mParameters.append(pParameter);

        return true;
    }
}

//==============================================================================

ComputerEquations ComputerFunction::equations() const
{
    // Return the function's equations

    return mEquations;
}

//==============================================================================

void ComputerFunction::addEquation(ComputerEquation *pEquation)
{
    // Add an equation to the function

    mEquations.append(pEquation);
}

//==============================================================================

ComputerEquation * ComputerFunction::returnValue()
{
    // Return the function's return value

    return mReturnValue;
}

//==============================================================================

void ComputerFunction::setReturnValue(ComputerEquation *pReturnValue)
{
    // Set the function's return value

    mReturnValue = pReturnValue;
}

//==============================================================================

ComputerExternalFunctions ComputerFunction::externalFunctions() const
{
    // Return the function's external functions

    return mExternalFunctions;
}

//==============================================================================

bool ComputerFunction::addExternalFunction(const ComputerExternalFunction &pExternalFunction)
{
    // Add an external function to our list, but only if i isn't already there

    if (mExternalFunctions.contains(pExternalFunction)) {
        // The external function already exists, so...

        return false;
    } else {
        // The external function is not yet in our list, so add it

        mExternalFunctions.append(pExternalFunction);

        return true;
    }
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

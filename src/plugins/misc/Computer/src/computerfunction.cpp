//==============================================================================
// Computer function class
//==============================================================================

#include "computerfunction.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerFunction::ComputerFunction() :
    mIsValid(true),
    mIrCode(0),
    mType(Void),
    mName(QString()),
    mEquations(ComputerEquations()),
    mParameters(QStringList())
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

bool ComputerFunction::isValid() const
{
    // Return whether the function is valid

    return mIsValid;
}

//==============================================================================

void ComputerFunction::setIsValid(const bool &pIsValid)
{
    // Set whether the function is valid

    mIsValid = pIsValid;
}

//==============================================================================

llvm::Function * ComputerFunction::irCode() const
{
    // Return the function's IR code

    return mIrCode;
}

//==============================================================================

void ComputerFunction::setIrCode(llvm::Function *pIrCode)
{
    // Set the function's IR code

    mIrCode = pIrCode;
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

QStringList ComputerFunction::parameters() const
{
    // Return the function's parameters

    return mParameters;
}

//==============================================================================

bool ComputerFunction::addParameter(const QString &pParameter)
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

QString ComputerFunction::returnValue() const
{
    // Return the function's return value

    return mReturnValue;
}

//==============================================================================

void ComputerFunction::setReturnValue(const QString &pReturnValue)
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

void ComputerFunction::addExternalFunction(const ComputerExternalFunction &pExternalFunction)
{
    // Add an external function which is needed by the function, but only if the
    // external function isn't already in our list of external functions

    if (!mExternalFunctions.contains(pExternalFunction))
        // The external function isn't already in our list of external
        // functions, so add it

        mExternalFunctions.append(pExternalFunction);
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

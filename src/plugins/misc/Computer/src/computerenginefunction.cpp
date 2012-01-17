//==============================================================================
// Computer engine function class
//==============================================================================

#include "computerenginefunction.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerEngineFunction::ComputerEngineFunction() :
    mIrCode(0),
    mType(Void),
    mName(QString()),
    mEquations(ComputerEngineEquations()),
    mParameters(QStringList())
{
}

//==============================================================================

ComputerEngineFunction::~ComputerEngineFunction()
{
    // Delete the equations

    foreach (ComputerEngineEquation *equation, mEquations)
        delete equation;
}

//==============================================================================

llvm::Function * ComputerEngineFunction::irCode() const
{
    // Return the function's IR code

    return mIrCode;
}

//==============================================================================

void ComputerEngineFunction::setIrCode(llvm::Function *pIrCode)
{
    // Set the function's IR code

    mIrCode = pIrCode;
}

//==============================================================================

ComputerEngineFunction::Type ComputerEngineFunction::type() const
{
    // Return the function's type

    return mType;
}

//==============================================================================

void ComputerEngineFunction::setType(const Type &pType)
{
    // Set the function's type

    mType = pType;
}

//==============================================================================

QString ComputerEngineFunction::name() const
{
    // Return the function's name

    return mName;
}

//==============================================================================

void ComputerEngineFunction::setName(const QString &pName)
{
    // Set the function's name

    mName = pName;
}

//==============================================================================

QStringList ComputerEngineFunction::parameters() const
{
    // Return the function's parameters

    return mParameters;
}

//==============================================================================

bool ComputerEngineFunction::addParameter(const QString &pParameter)
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

ComputerEngineEquations ComputerEngineFunction::equations() const
{
    // Return the function's equations

    return mEquations;
}

//==============================================================================

void ComputerEngineFunction::addEquation(ComputerEngineEquation *pEquation)
{
    // Add an equation to the function

    mEquations.append(pEquation);
}

//==============================================================================

QString ComputerEngineFunction::returnValue() const
{
    // Return the function's return value

    return mReturnValue;
}

//==============================================================================

void ComputerEngineFunction::setReturnValue(const QString &pReturnValue)
{
    // Set the function's return value

    mReturnValue = pReturnValue;
}

//==============================================================================

ComputerEngineExternalFunctions ComputerEngineFunction::externalFunctions() const
{
    // Return the function's external functions

    return mExternalFunctions;
}

//==============================================================================

void ComputerEngineFunction::addExternalFunction(const ComputerEngineExternalFunction &pExternalFunction)
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

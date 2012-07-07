//==============================================================================
// Computer parameter class
//==============================================================================

#include "computerparameter.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerParameter::ComputerParameter(const QString &pName,
                                     const bool &pPointer) :
    mName(pName),
    mPointer(pPointer)
{
}

//==============================================================================

QString ComputerParameter::name() const
{
    // Return the parameter's name

    return mName;
}

//==============================================================================

bool ComputerParameter::pointer() const
{
    // Return whether the parameter is a pointer

    return mPointer;
}

//==============================================================================

bool ComputerParameters::contains(const ComputerParameter &pParameter) const
{
    // Check whether a parameter is already in our list of parameters
    // Note: we only check the name of the parameter, not whether it is a
    //       pointer...

    foreach (const ComputerParameter &parameter, *this)
        if (!parameter.name().compare(pParameter.name()))
            // The parameter is already in our list of parameters, so...

            return true;

    // The parameter couldn't be found in our list of parameters, so...

    return false;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

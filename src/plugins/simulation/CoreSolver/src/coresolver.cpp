//==============================================================================
// Core solver class
//==============================================================================

#include "coresolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreSolver::CoreSolver() :
    mProperties(Properties())
{
}

//==============================================================================

void CoreSolver::setProperty(const QString &pName, const QVariant &pValue)
{
    // Add/replace a property's value, but only if it is a valid one

    if (isValidProperty(pName))
        mProperties.insert(pName, pValue);
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

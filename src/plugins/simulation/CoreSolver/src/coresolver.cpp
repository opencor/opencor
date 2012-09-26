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

void CoreSolver::emitError(const QString &pErrorMsg)
{
    // Let people know that an error occured

    emit error(pErrorMsg);
}

//==============================================================================

bool CoreSolver::isValidProperty(const QString &pName) const
{
    Q_UNUSED(pName);

    // Check whether the property name is known to us which, by default, isn't
    // the case

    return false;
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
    // Let people know that an error occured, but first reformat the error a
    // bit, if needed

    QString errorMsg = pErrorMsg.at(0).toLower()+pErrorMsg.right(pErrorMsg.size()-1);

    if (!pErrorMsg.right(3).compare("..."))
        emit error(errorMsg.left(errorMsg.size()-3));
    else if (!pErrorMsg.right(1).compare("."))
        emit error(errorMsg.left(errorMsg.size()-1));
    else
        emit error(errorMsg);
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

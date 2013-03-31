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

void CoreSolver::setProperties(const Properties &pProperties)
{
    // Set a property's value, but only if it is a valid property

    mProperties = pProperties;
}

//==============================================================================

void CoreSolver::emitError(const QString &pErrorMsg)
{
    // Let people know that an error occured, but first reformat the error a
    // bit, if needed

    QString errorMsg = pErrorMsg[0].toLower()+pErrorMsg.right(pErrorMsg.size()-1);

    if (!pErrorMsg.right(3).compare("..."))
        emit error(errorMsg.left(errorMsg.size()-3));
    else if (!pErrorMsg.right(1).compare("."))
        emit error(errorMsg.left(errorMsg.size()-1));
    else
        emit error(errorMsg);
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

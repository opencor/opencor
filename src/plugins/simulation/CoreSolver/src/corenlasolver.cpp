//==============================================================================
// Core non-linear algebraic solver class
//==============================================================================

#include "corenlasolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

static CoreNlaSolver *gGlobalNlaSolver = 0;

//==============================================================================

CoreNlaSolver::CoreNlaSolver() :
    mComputeSystem(0),
    mParameters(0),
    mSize(0),
    mUserData(0)
{
}

//==============================================================================

void CoreNlaSolver::initialize(ComputeSystemFunction pComputeSystem,
                               double *pParameters, int pSize, void *pUserData)
{
    // Initialise the non-linear algebraic solver

    mComputeSystem = pComputeSystem;

    mParameters = pParameters;
    mSize = pSize;
    mUserData = pUserData;
}

//==============================================================================

void resetGlobalNlaSolver()
{
    // Reset our global non-linear algebraic solver, if needed

    if (gGlobalNlaSolver) {
        delete gGlobalNlaSolver;

        gGlobalNlaSolver = 0;
    }
}

//==============================================================================

void setGlobalNlaSolver(CoreNlaSolver *pGlobalNlaSolver)
{
    // Set our global non-linear algebraic solver after deleting the previous
    // one, if needed

    if (gGlobalNlaSolver)
        delete gGlobalNlaSolver;

    gGlobalNlaSolver = pGlobalNlaSolver;
}

//==============================================================================

CoreNlaSolver * globalNlaSolver()
{
    // Return our global non-linear algebraic solver

    return gGlobalNlaSolver;
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

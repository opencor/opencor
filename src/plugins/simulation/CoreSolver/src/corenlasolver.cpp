//==============================================================================
// Core non-linear algebraic solver class
//==============================================================================

#include "corenlasolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

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

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

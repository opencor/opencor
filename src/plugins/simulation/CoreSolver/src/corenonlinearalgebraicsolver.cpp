//==============================================================================
// Core non-linear algebraic solver class
//==============================================================================

#include "corenonlinearalgebraicsolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreNonLinearAlgebraicSolver::CoreNonLinearAlgebraicSolver() :
    mComputeSystem(0),
    mParameters(0),
    mSize(0),
    mUserData(0)
{
}

//==============================================================================

void CoreNonLinearAlgebraicSolver::initialize(ComputeSystemFunction pComputeSystem,
                                              double *pParameters, int pSize,
                                              void *pUserData)
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

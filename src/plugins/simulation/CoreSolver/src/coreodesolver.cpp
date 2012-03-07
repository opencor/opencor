//==============================================================================
// Core ODE solver class
//==============================================================================

#include "coreodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreOdeSolver::CoreOdeSolver() :
    CoreSolver(),
    mStatesCount(0),
    mConstants(0),
    mRates(0),
    mStates(0),
    mAlgebraic(0)
{
}

//==============================================================================

void CoreOdeSolver::initialize(const int &pStatesCount, double **pConstants,
                               double **pRates, double **pStates,
                               double **pAlgebraic)
{
    // Initialise the ODE solver

    mStatesCount = pStatesCount;

    mConstants = pConstants;
    mRates     = pRates;
    mStates    = pStates;
    mAlgebraic = pAlgebraic;
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

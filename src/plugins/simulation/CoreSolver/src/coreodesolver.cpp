//==============================================================================
// Core ODE solver class
//==============================================================================

#include "coreodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreOdeSolver::CoreOdeSolver() :
    mNbOfStates(0),
    mConstants(0),
    mRates(0),
    mStates(0),
    mAlgebraic(0)
{
}

//==============================================================================

void CoreOdeSolver::initialize(const int &pNbOfStates, double **pConstants,
                               double **pRates, double **pStates,
                               double **pAlgebraic)
{
    // Initialise the ODE solver

    mNbOfStates = pNbOfStates;

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

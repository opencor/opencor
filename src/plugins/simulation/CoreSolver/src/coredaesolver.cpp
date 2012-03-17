//==============================================================================
// Core DAE solver class
//==============================================================================

#include "coredaesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreDaeSolver::CoreDaeSolver() :
    CoreSolver(),
    mStatesCount(0),
    mConstants(0),
    mRates(0),
    mStates(0),
    mAlgebraic(0)
{
}

//==============================================================================

void CoreDaeSolver::initialize(const double &/* pVoiStart */,
                               const int &pStatesCount, const int &pCondVarCount,
                               double *pConstants, double *pRates,
                               double *pStates, double *pAlgebraic,
                               ComputeRatesFunction /* pComputeRates */)
{
    // Initialise the DAE solver

    mStatesCount  = pStatesCount;
    mCondVarCount = pCondVarCount;

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

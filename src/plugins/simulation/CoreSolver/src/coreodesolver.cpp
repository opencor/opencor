//==============================================================================
// Core ODE solver class
//==============================================================================

#include "coreodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreOdeSolver::CoreOdeSolver() :
    CoreVoiSolver(),
    mComputeRates(0)
{
}

//==============================================================================

void CoreOdeSolver::initialize(const double &pVoiStart, const int &pStatesCount,
                               double *pConstants, double *pStates,
                               double *pRates, double *pAlgebraic,
                               ComputeRatesFunction pComputeRates)
{
    Q_UNUSED(pVoiStart);

    // Initialise the ODE solver

    mStatesCount = pStatesCount;

    mConstants = pConstants;
    mStates    = pStates;
    mRates     = pRates;
    mAlgebraic = pAlgebraic;

    mComputeRates = pComputeRates;
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

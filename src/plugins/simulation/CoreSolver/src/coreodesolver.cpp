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

void CoreOdeSolver::initialize(const double &pVoiStart,
                               const int &pRatesStatesCount, double *pConstants,
                               double *pRates, double *pStates,
                               double *pAlgebraic,
                               ComputeRatesFunction pComputeRates)
{
    Q_UNUSED(pVoiStart);

    // Initialise the ODE solver

    mRatesStatesCount = pRatesStatesCount;

    mConstants = pConstants;
    mRates     = pRates;
    mStates    = pStates;
    mAlgebraic = pAlgebraic;

    mComputeRates = pComputeRates;
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

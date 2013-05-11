//==============================================================================
// Core DAE solver class
//==============================================================================

#include "coredaesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreDaeSolver::CoreDaeSolver() :
    CoreVoiSolver(),
    mCondVarCount(0),
    mOldRates(0),
    mOldStates(0),
    mCondVar(0)
{
}

//==============================================================================

CoreDaeSolver::~CoreDaeSolver()
{
    // Delete some internal objects

    delete[] mOldRates;
    delete[] mOldStates;
}

//==============================================================================

void CoreDaeSolver::initialize(const double &pVoiStart, const double &pVoiEnd,
                               const int &pRatesStatesCount,
                               const int &pCondVarCount, double *pConstants,
                               double *pRates, double *pStates,
                               double *pAlgebraic, double *pCondVar,
                               ComputeEssentialVariablesFunction pComputeEssentialVariables,
                               ComputeResidualsFunction pComputeResiduals,
                               ComputeRootInformationFunction pComputeRootInformation,
                               ComputeStateInformationFunction pComputeStateInformation)
{
    Q_UNUSED(pVoiStart);
    Q_UNUSED(pVoiEnd);
    Q_UNUSED(pComputeEssentialVariables);
    Q_UNUSED(pComputeResiduals);
    Q_UNUSED(pComputeRootInformation);
    Q_UNUSED(pComputeStateInformation);

    // Initialise the DAE solver

    mRatesStatesCount = pRatesStatesCount;
    mCondVarCount     = pCondVarCount;

    mConstants = pConstants;
    mRates     = pRates;
    mStates    = pStates;
    mAlgebraic = pAlgebraic;
    mCondVar   = pCondVar;

    delete[] mOldRates;
    delete[] mOldStates;

    mOldRates  = new double[pRatesStatesCount];
    mOldStates = new double[pRatesStatesCount];

    memcpy(mOldRates, pRates, pRatesStatesCount*SizeOfDouble);
    memcpy(mOldStates, pStates, pRatesStatesCount*SizeOfDouble);
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

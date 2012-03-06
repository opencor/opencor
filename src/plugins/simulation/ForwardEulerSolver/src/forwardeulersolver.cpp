//==============================================================================
// Forward Euler solver class
//==============================================================================

#include "forwardeulersolver.h"

//==============================================================================

namespace OpenCOR {
namespace ForwardEulerSolver {

//==============================================================================

void ForwardEulerSolver::initialize(const int &pNbOfStates, double **pConstants,
                                    double **pRates, double **pStates,
                                    double **pAlgebraic, const double &pStep)
{
    // Initialise the Forward Euler solver

    CoreSolver::CoreOdeSolver::initialize(pNbOfStates, pConstants, pRates,
                                          pStates, pAlgebraic);

    mStep = pStep;
}

//==============================================================================

void ForwardEulerSolver::solve(double &pVoi, const double &pVoiEnd,
                               CoreSolver::CoreOdeSolverComputeRatesFunction pComputeRates) const
{
    Q_ASSERT(mNbOfStates > 0);
    Q_ASSERT(mConstants);
    Q_ASSERT(mRates);
    Q_ASSERT(mStates);
    Q_ASSERT(mAlgebraic);
    Q_ASSERT(mStep > 0);

    // Y_n+1 = Y_n + h * f(t_n, Y_n)

    double voiStart = pVoi;

    int nbOfSteps = 0;
    double realStep = mStep;

    while (pVoi != pVoiEnd)
    {
        // Check that the time step is correct

        if (pVoi+realStep > pVoiEnd)
            realStep = pVoiEnd-pVoi;

        // Compute f(t_n, Y_n)

        pComputeRates(pVoi, *mConstants, *mRates, *mStates, *mAlgebraic);

        // Compute Y_n+1

        for (int i = 0; i < mNbOfStates; ++i)
            *mStates[i] += realStep*(*mRates[i]);

        // Advance through time

        if (realStep != mStep)
            pVoi = pVoiEnd;
        else
            pVoi = voiStart+(++nbOfSteps)*mStep;
    }
}

//==============================================================================

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

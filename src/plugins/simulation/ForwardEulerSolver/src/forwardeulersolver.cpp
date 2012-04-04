//==============================================================================
// Forward Euler solver class
//==============================================================================

#include "forwardeulersolver.h"

//==============================================================================

namespace OpenCOR {
namespace ForwardEulerSolver {

//==============================================================================

static const double InvalidStep = 0;

//==============================================================================

ForwardEulerSolver::ForwardEulerSolver() :
    mStep(InvalidStep)
{
}

//==============================================================================

void ForwardEulerSolver::initialize(const double &pVoiStart,
                                    const int &pStatesCount, double *pConstants,
                                    double *pRates, double *pStates,
                                    double *pAlgebraic,
                                    ComputeRatesFunction pComputeRates)
{
    // Initialise the ODE solver itself

    OpenCOR::CoreSolver::CoreOdeSolver::initialize(pVoiStart, pStatesCount,
                                                   pConstants, pRates, pStates,
                                                   pAlgebraic, pComputeRates);

    // Retrieve the solver's properties

    mStep = mProperties.contains(StepProperty)?
                mProperties.value(StepProperty).toDouble():
                InvalidStep;
}

//==============================================================================

void ForwardEulerSolver::solve(double &pVoi, const double &pVoiEnd) const
{
    Q_ASSERT(mStatesCount > 0);
    Q_ASSERT(mConstants);
    Q_ASSERT(mRates);
    Q_ASSERT(mStates);
    Q_ASSERT(mAlgebraic);
    Q_ASSERT(mStep > 0);
    Q_ASSERT(mComputeRates);

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

        mComputeRates(pVoi, mConstants, mRates, mStates, mAlgebraic);

        // Compute Y_n+1

        for (int i = 0; i < mStatesCount; ++i)
            mStates[i] += realStep*mRates[i];

        // Advance through time

        if (realStep != mStep)
            pVoi = pVoiEnd;
        else
            pVoi = voiStart+(++nbOfSteps)*mStep;
    }
}

//==============================================================================

bool ForwardEulerSolver::isValidProperty(const QString &pName) const
{
    // Check whether the property name is known to us

    return !pName.compare(StepProperty);
}

//==============================================================================

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

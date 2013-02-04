//==============================================================================
// Forward Euler solver class
//==============================================================================

#include "forwardeulersolver.h"

//==============================================================================

namespace OpenCOR {
namespace ForwardEulerSolver {

//==============================================================================

ForwardEulerSolver::ForwardEulerSolver() :
    mStep(DefaultStep)
{
}

//==============================================================================

void ForwardEulerSolver::initialize(const double &pVoiStart,
                                    const int &pStatesCount, double *pConstants,
                                    double *pStates, double *pRates,
                                    double *pAlgebraic,
                                    ComputeRatesFunction pComputeRates)
{
    // Initialise the ODE solver itself

    OpenCOR::CoreSolver::CoreOdeSolver::initialize(pVoiStart, pStatesCount,
                                                   pConstants, pStates, pRates,
                                                   pAlgebraic, pComputeRates);

    // Retrieve the solver's properties

    if (mProperties.contains(StepProperty)) {
        mStep = mProperties.value(StepProperty).toDouble();

        if (!mStep)
            emit error(QObject::tr("the 'step' property value cannot be equal to zero"));
    } else {
        emit error(QObject::tr("the 'step' property value could not be retrieved"));
    }
}

//==============================================================================

void ForwardEulerSolver::solve(double &pVoi, const double &pVoiEnd) const
{
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

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Second-order Runge-Kutta solver class
//==============================================================================

#include "secondorderrungekuttasolver.h"

//==============================================================================

namespace OpenCOR {
namespace SecondOrderRungeKuttaSolver {

//==============================================================================

SecondOrderRungeKuttaSolver::SecondOrderRungeKuttaSolver() :
    mStep(DefaultStep),
    mYk2(0)
{
}

//==============================================================================

SecondOrderRungeKuttaSolver::~SecondOrderRungeKuttaSolver()
{
    // Delete some internal objects

    delete[] mYk2;
}

//==============================================================================

void SecondOrderRungeKuttaSolver::initialize(const double &pVoiStart,
                                             const int &pStatesCount,
                                             double *pConstants,
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

        if (!mStep) {
            emit error(QObject::tr("the 'step' property value cannot be equal to zero"));

            return;
        }
    } else {
        emit error(QObject::tr("the 'step' property value could not be retrieved"));

        return;
    }

    // (Re-)create our mYk2 array

    delete[] mYk2;

    mYk2 = new double[pStatesCount];
}

//==============================================================================

void SecondOrderRungeKuttaSolver::solve(double &pVoi,
                                        const double &pVoiEnd) const
{
    // k1 = h * f(t_n, Y_n)
    // k2 = h * f(t_n + h / 2, Y_n + k1 / 2)
    // Y_n+1 = Y_n + k2

    // Note: the algorithm hereafter doesn't compute k1 and k2 as such and this
    //       simply for performance reasons...

    double voiStart = pVoi;

    int stepNumber = 0;
    double realStep = mStep;
    double realHalfStep = 0.5*realStep;

    while (pVoi != pVoiEnd) {
        // Check that the time step is correct

        if (pVoi+realStep > pVoiEnd) {
            realStep = pVoiEnd-pVoi;
            realHalfStep = 0.5*realStep;
        }

        // Compute f(t_n, Y_n)

        mComputeRates(pVoi, mConstants, mRates, mStates, mAlgebraic);

        // Compute k1 and therefore Yk2

        for (int i = 0; i < mStatesCount; ++i)
            mYk2[i] = mStates[i]+realHalfStep*mRates[i];

        // Compute f(t_n + h / 2, Y_n + k1 / 2)

        mComputeRates(pVoi+realHalfStep, mConstants, mRates, mYk2, mAlgebraic);

        // Compute Y_n+1

        for (int i = 0; i < mStatesCount; ++i)
            mStates[i] += realStep*mRates[i];

        // Advance through time

        if (realStep != mStep)
            pVoi = pVoiEnd;
        else
            pVoi = voiStart+(++stepNumber)*mStep;
    }
}

//==============================================================================

}   // namespace SecondOrderRungeKuttaSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Fourth-order Runge-Kutta solver class
//==============================================================================

#include "fourthorderrungekuttasolver.h"

//==============================================================================

namespace OpenCOR {
namespace FourthOrderRungeKuttaSolver {

//==============================================================================

FourthOrderRungeKuttaSolver::FourthOrderRungeKuttaSolver() :
    mStep(DefaultStep),
    mK1(0),
    mK23(0),
    mYk234(0)
{
}

//==============================================================================

FourthOrderRungeKuttaSolver::~FourthOrderRungeKuttaSolver()
{
    // Delete some internal objects

    delete[] mK1;
    delete[] mK23;
    delete[] mYk234;
}

//==============================================================================

void FourthOrderRungeKuttaSolver::initialize(const double &pVoiStart,
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

    // (Re-)create our various arrays

    delete[] mK1;
    delete[] mK23;
    delete[] mYk234;

    mK1    = new double[pStatesCount];
    mK23   = new double[pStatesCount];
    mYk234 = new double[pStatesCount];
}

//==============================================================================

void FourthOrderRungeKuttaSolver::solve(double &pVoi,
                                        const double &pVoiEnd) const
{
    // k1 = h * f(t_n, Y_n)
    // k2 = h * f(t_n + h / 2, Y_n + k1 / 2)
    // k3 = h * f(t_n + h / 2, Y_n + k2 / 2)
    // k4 = h * f(t_n + h, Y_n + k3)
    // Y_n+1 = Y_n + k1 / 6 + k2 / 3 + k3 / 3 + k4 / 6

    // Note: the algorithm hereafter doesn't compute k1, k2, k3 and k4 as such
    //       and this simply for performance reasons...

    static const double OneOverThree = 1.0/3.0;
    static const double OneOverSix   = 1.0/6.0;

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

        // Compute k1 and Yk2

        for (int i = 0; i < mStatesCount; ++i) {
            mK1[i]    = mRates[i];
            mYk234[i] = mStates[i]+realHalfStep*mK1[i];
        }

        // Compute f(t_n + h / 2, Y_n + k1 / 2)

        mComputeRates(pVoi+realHalfStep, mConstants, mRates, mYk234, mAlgebraic);

        // Compute k2 and Yk3

        for (int i = 0; i < mStatesCount; ++i) {
            mK23[i]   = mRates[i];
            mYk234[i] = mStates[i]+realHalfStep*mK23[i];
        }

        // Compute f(t_n + h / 2, Y_n + k2 / 2)

        mComputeRates(pVoi+realHalfStep, mConstants, mRates, mYk234, mAlgebraic);

        // Compute k3 and Yk4

        for (int i = 0; i < mStatesCount; ++i) {
            mK23[i]   += mRates[i];
            mYk234[i]  = mStates[i]+realStep*mK23[i];
        }

        // Compute f(t_n + h, Y_n + k3)

        mComputeRates(pVoi+realStep, mConstants, mRates, mYk234, mAlgebraic);

        // Compute k4 and therefore Y_n+1

        for (int i = 0; i < mStatesCount; ++i)
            mStates[i] += realStep*(OneOverSix*(mK1[i]+mRates[i])+OneOverThree*mK23[i]);

        // Advance through time

        if (realStep != mStep)
            pVoi = pVoiEnd;
        else
            pVoi = voiStart+(++stepNumber)*mStep;
    }
}

//==============================================================================

}   // namespace FourthOrderRungeKuttaSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

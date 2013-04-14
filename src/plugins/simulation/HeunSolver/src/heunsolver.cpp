//==============================================================================
// Heun solver class
//==============================================================================

#include "heunsolver.h"

//==============================================================================

namespace OpenCOR {
namespace HeunSolver {

//==============================================================================

HeunSolver::HeunSolver() :
    mStep(DefaultStep),
    mK(0),
    mYk(0)
{
}

//==============================================================================

HeunSolver::~HeunSolver()
{
    // Delete some internal objects

    delete[] mK;
    delete[] mYk;
}

//==============================================================================

void HeunSolver::initialize(const double &pVoiStart, const int &pStatesCount,
                            double *pConstants, double *pStates, double *pRates,
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

    // (Re-)create our various arrays

    delete[] mK;
    delete[] mYk;

    mK  = new double[pStatesCount];
    mYk = new double[pStatesCount];
}

//==============================================================================

void HeunSolver::solve(double &pVoi, const double &pVoiEnd) const
{
    // k = h * f(t_n, Y_n)
    // Y_n+1 = Y_n + h / 2 * ( f(t_n, Y_n) + f(t_n + h, Y_n + k) )

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

        // Compute k and Yk

        for (int i = 0; i < mStatesCount; ++i) {
            mK[i]  = mRates[i];
            mYk[i] = mStates[i]+realStep*mRates[i];
        }

        // Compute f(t_n + h, Y_n + k)

        mComputeRates(pVoi+realStep, mConstants, mRates, mYk, mAlgebraic);

        // Compute Y_n+1

        for (int i = 0; i < mStatesCount; ++i)
            mStates[i] += realHalfStep*(mK[i]+mRates[i]);

        // Advance through time

        if (realStep != mStep)
            pVoi = pVoiEnd;
        else
            pVoi = voiStart+(++stepNumber)*mStep;
    }
}

//==============================================================================

}   // namespace HeunSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Midpoint solver class
//==============================================================================

#include "midpointsolver.h"

//==============================================================================

namespace OpenCOR {
namespace MidpointSolver {

//==============================================================================

MidpointSolver::MidpointSolver() :
    mStep(StepDefaultValue),
    mYk(0)
{
}

//==============================================================================

MidpointSolver::~MidpointSolver()
{
    // Delete some internal objects

    delete[] mYk;
}

//==============================================================================

void MidpointSolver::initialize(const double &pVoiStart,
                                const int &pRatesStatesCount,
                                double *pConstants, double *pRates,
                                double *pStates, double *pAlgebraic,
                                ComputeRatesFunction pComputeRates)
{
    // Retrieve the solver's properties

    if (mProperties.contains(StepId)) {
        mStep = mProperties.value(StepId).toDouble();

        if (!mStep) {
            emit error(QObject::tr("the 'step' property value cannot be equal to zero"));

            return;
        }
    } else {
        emit error(QObject::tr("the 'step' property value could not be retrieved"));

        return;
    }

    // Initialise the ODE solver itself

    OpenCOR::CoreSolver::CoreOdeSolver::initialize(pVoiStart, pRatesStatesCount,
                                                   pConstants, pRates, pStates,
                                                   pAlgebraic, pComputeRates);

    // (Re-)create our mYk array

    delete[] mYk;

    mYk = new double[pRatesStatesCount];
}

//==============================================================================

void MidpointSolver::solve(double &pVoi, const double &pVoiEnd) const
{
    // k = h / 2 * f(t_n, Y_n)
    // Y_n+1 = Y_n + h * f(t_n + h / 2, Y_n + k))

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

        // Compute Yk

        for (int i = 0; i < mRatesStatesCount; ++i)
            mYk[i] = mStates[i]+realHalfStep*mRates[i];

        // Compute f(t_n + h / 2, Y_n + k)

        mComputeRates(pVoi+realHalfStep, mConstants, mRates, mYk, mAlgebraic);

        // Compute Y_n+1

        for (int i = 0; i < mRatesStatesCount; ++i)
            mStates[i] += realStep*(mRates[i]);

        // Advance through time

        if (realStep != mStep)
            pVoi = pVoiEnd;
        else
            pVoi = voiStart+(++stepNumber)*mStep;
    }
}

//==============================================================================

}   // namespace MidpointSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

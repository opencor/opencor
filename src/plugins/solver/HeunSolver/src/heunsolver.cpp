/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Heun solver
//==============================================================================

#include "heunsolver.h"

//==============================================================================

namespace OpenCOR {
namespace HeunSolver {

//==============================================================================

HeunSolver::HeunSolver() :
    mStep(StepDefaultValue),
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

void HeunSolver::initialize(const double &pVoiStart,
                            const int &pRatesStatesCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
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

    OpenCOR::Solver::OdeSolver::initialize(pVoiStart, pRatesStatesCount,
                                           pConstants, pRates, pStates,
                                           pAlgebraic, pComputeRates);

    // (Re)create our various arrays

    delete[] mK;
    delete[] mYk;

    mK = new double[pRatesStatesCount];
    mYk = new double[pRatesStatesCount];
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

        for (int i = 0; i < mRatesStatesCount; ++i) {
            mK[i] = mRates[i];
            mYk[i] = mStates[i]+realStep*mRates[i];
        }

        // Compute f(t_n + h, Y_n + k)

        mComputeRates(pVoi+realStep, mConstants, mRates, mYk, mAlgebraic);

        // Compute Y_n+1

        for (int i = 0; i < mRatesStatesCount; ++i)
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

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
// CVODE solver
//==============================================================================

#include "cvodesolver.h"

//==============================================================================

#include "cvodes/cvodes.h"
#include "cvodes/cvodes_band.h"
#include "cvodes/cvodes_bandpre.h"
#include "cvodes/cvodes_dense.h"
#include "cvodes/cvodes_diag.h"
#include "cvodes/cvodes_spbcgs.h"
#include "cvodes/cvodes_spgmr.h"
#include "cvodes/cvodes_sptfqmr.h"

//==============================================================================

#include <algorithm>

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

int rhsFunction(double pVoi, N_Vector pStates, N_Vector pRates, void *pUserData)
{
    // Compute the RHS function

    CvodeSolverUserData *userData = static_cast<CvodeSolverUserData *>(pUserData);

    userData->computeRates()(pVoi, userData->constants(),
                             N_VGetArrayPointer_Serial(pRates),
                             N_VGetArrayPointer_Serial(pStates),
                             userData->algebraic());

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char *pModule, const char *pFunction,
                  char *pErrorMessage, void *pUserData)
{
    Q_UNUSED(pModule);
    Q_UNUSED(pFunction);

    if (pErrorCode != CV_WARNING) {
        // CVODE generated an error, so forward it to the CvodeSolver object

        static_cast<CvodeSolver *>(pUserData)->emitError(pErrorMessage);
    }
}

//==============================================================================

CvodeSolverUserData::CvodeSolverUserData(double *pConstants, double *pAlgebraic,
                                         Solver::OdeSolver::ComputeRatesFunction pComputeRates) :
    mConstants(pConstants),
    mAlgebraic(pAlgebraic),
    mComputeRates(pComputeRates)
{
}

//==============================================================================

double * CvodeSolverUserData::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double * CvodeSolverUserData::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

Solver::OdeSolver::ComputeRatesFunction CvodeSolverUserData::computeRates() const
{
    // Return our compute rates function

    return mComputeRates;
}

//==============================================================================

CvodeSolver::CvodeSolver() :
    mSolver(0),
    mStatesVector(0),
    mUserData(0),
    mSensitivityVectors(0),
    mSensitivityVectorsSize(0),
    mInterpolateSolution(InterpolateSolutionDefaultValue)
{
}

//==============================================================================

CvodeSolver::~CvodeSolver()
{
    // Make sure that the solver has been initialised

    if (!mSolver)
        return;

    // Delete some internal objects

    N_VDestroy_Serial(mStatesVector);

    if (mSensitivityVectors)
        N_VDestroyVectorArray_Serial(mSensitivityVectors, mSensitivityVectorsSize);

    CVodeFree(&mSolver);

    delete mUserData;
}

//==============================================================================

void CvodeSolver::initialize(const double &pVoiStart,
                             const int &pRatesStatesCount, double *pConstants,
                             double *pRates, double *pStates,
                             double *pAlgebraic,
                             ComputeRatesFunction pComputeRates)
{
    initialize(pVoiStart, pRatesStatesCount, pConstants, pRates, pStates, pAlgebraic, pComputeRates, 0, 0, 0);
}

//==============================================================================

void CvodeSolver::initialize(const double &pVoiStart,
                             const int &pRatesStatesCount, double *pConstants,
                             double *pRates, double *pStates,
                             double *pAlgebraic,
                             ComputeRatesFunction pComputeRates,
                             const int &pGradientsCount,
                             int *pGradientsIndices,
                             double *pGradients)
{
    if (!mSolver) {
        // Retrieve some of the CVODE properties

        double maximumStep = MaximumStepDefaultValue;
        int maximumNumberOfSteps = MaximumNumberOfStepsDefaultValue;
        QString integrationMethod = IntegrationMethodDefaultValue;
        QString iterationType = IterationTypeDefaultValue;
        QString linearSolver = LinearSolverDefaultValue;
        QString preconditioner = PreconditionerDefaultValue;
        int upperHalfBandwidth = UpperHalfBandwidthDefaultValue;
        int lowerHalfBandwidth = LowerHalfBandwidthDefaultValue;
        double relativeTolerance = RelativeToleranceDefaultValue;
        double absoluteTolerance = AbsoluteToleranceDefaultValue;

        if (mProperties.contains(MaximumStepId)) {
            maximumStep = mProperties.value(MaximumStepId).toDouble();
        } else {
            emit error(tr("the 'maximum step' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(MaximumNumberOfStepsId)) {
            maximumNumberOfSteps = mProperties.value(MaximumNumberOfStepsId).toInt();
        } else {
            emit error(tr("the 'maximum number of steps' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(IntegrationMethodId)) {
            integrationMethod = mProperties.value(IntegrationMethodId).toString();
        } else {
            emit error(tr("the 'integration method' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(IterationTypeId)) {
            iterationType = mProperties.value(IterationTypeId).toString();

            if (!iterationType.compare(NewtonIteration)) {
                // We are dealing with a Newton iteration, so retrieve and check
                // its linear solver

                if (mProperties.contains(LinearSolverId)) {
                    linearSolver = mProperties.value(LinearSolverId).toString();

                    bool needUpperAndLowerHalfBandwidths = false;

                    if (   !linearSolver.compare(DenseLinearSolver)
                        || !linearSolver.compare(DiagonalLinearSolver)) {
                        // We are dealing with a dense/diagonal linear solver,
                        // so nothing more to do
                    } else if (!linearSolver.compare(BandedLinearSolver)) {
                        // We are dealing with a banded linear solver, so we
                        // need both an upper and a lower half bandwidth

                        needUpperAndLowerHalfBandwidths = true;
                    } else {
                        // We are dealing with a GMRES/Bi-CGStab/TFQMR linear
                        // solver, so retrieve and check its preconditioner

                        if (mProperties.contains(PreconditionerId)) {
                            preconditioner = mProperties.value(PreconditionerId).toString();
                        } else {
                            emit error(tr("the 'preconditioner' property value could not be retrieved"));

                            return;
                        }

                        if (!preconditioner.compare(BandedPreconditioner)) {
                            // We are dealing with a banded preconditioner, so
                            // we need both an upper and a lower half bandwidth

                            needUpperAndLowerHalfBandwidths = true;
                        }
                    }

                    if (needUpperAndLowerHalfBandwidths) {
                        if (mProperties.contains(UpperHalfBandwidthId)) {
                            upperHalfBandwidth = mProperties.value(UpperHalfBandwidthId).toInt();

                            if (   (upperHalfBandwidth < 0)
                                || (upperHalfBandwidth >= pRatesStatesCount)) {
                                emit error(tr("the 'upper half-bandwidth' property must have a value between 0 and %1").arg(pRatesStatesCount-1));

                                return;
                            }
                        } else {
                            emit error(tr("the 'upper half-bandwidth' property value could not be retrieved"));

                            return;
                        }

                        if (mProperties.contains(LowerHalfBandwidthId)) {
                            lowerHalfBandwidth = mProperties.value(LowerHalfBandwidthId).toInt();

                            if (   (lowerHalfBandwidth < 0)
                                || (lowerHalfBandwidth >= pRatesStatesCount)) {
                                emit error(tr("the 'lower half-bandwidth' property must have a value between 0 and %1").arg(pRatesStatesCount-1));

                                return;
                            }
                        } else {
                            emit error(tr("the 'lower half-bandwidth' property value could not be retrieved"));

                            return;
                        }
                    }
                } else {
                    emit error(tr("the 'linear solver' property value could not be retrieved"));

                    return;
                }
            }
        } else {
            emit error(tr("the 'iteration type' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(RelativeToleranceId)) {
            relativeTolerance = mProperties.value(RelativeToleranceId).toDouble();

            if (relativeTolerance < 0) {
                emit error(tr("the 'relative tolerance' property must have a value greater than or equal to 0"));

                return;
            }
        } else {
            emit error(tr("the 'relative tolerance' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(AbsoluteToleranceId)) {
            absoluteTolerance = mProperties.value(AbsoluteToleranceId).toDouble();

            if (absoluteTolerance < 0) {
                emit error(tr("the 'absolute tolerance' property must have a value greater than or equal to 0"));

                return;
            }
        } else {
            emit error(tr("the 'absolute tolerance' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(InterpolateSolutionId)) {
            mInterpolateSolution = mProperties.value(InterpolateSolutionId).toBool();
        } else {
            emit error(tr("the 'interpolate solution' property value could not be retrieved"));

            return;
        }

        // Initialise the ODE solver itself

        OpenCOR::Solver::OdeSolver::initialize(pVoiStart, pRatesStatesCount,
                                               pConstants, pRates, pStates,
                                               pAlgebraic, pComputeRates);

        // Create the states vector

        mStatesVector = N_VMake_Serial(pRatesStatesCount, pStates);

        // Create the CVODE solver

        bool newtonIteration = !iterationType.compare(NewtonIteration);

        mSolver = CVodeCreate(!integrationMethod.compare(BdfMethod)?CV_BDF:CV_ADAMS,
                              newtonIteration?CV_NEWTON:CV_FUNCTIONAL);

        // Use our own error handler

        CVodeSetErrHandlerFn(mSolver, errorHandler, this);

        // Initialise the CVODE solver

        CVodeInit(mSolver, rhsFunction, pVoiStart, mStatesVector);

        // Set some user data

        mUserData = new CvodeSolverUserData(pConstants, pAlgebraic,
                                            pComputeRates);

        CVodeSetUserData(mSolver, mUserData);

        // Set the maximum step

        CVodeSetMaxStep(mSolver, maximumStep);

        // Set the maximum number of steps

        CVodeSetMaxNumSteps(mSolver, maximumNumberOfSteps);

        // Set the linear solver, if needed

        if (newtonIteration) {
            if (!linearSolver.compare(DenseLinearSolver)) {
                CVDense(mSolver, pRatesStatesCount);
            } else if (!linearSolver.compare(BandedLinearSolver)) {
                CVBand(mSolver, pRatesStatesCount, upperHalfBandwidth, lowerHalfBandwidth);
            } else if (!linearSolver.compare(DiagonalLinearSolver)) {
                CVDiag(mSolver);
            } else {
                // We are dealing with a GMRES/Bi-CGStab/TFQMR linear solver

                if (!preconditioner.compare(BandedPreconditioner)) {
                    if (!linearSolver.compare(GmresLinearSolver))
                        CVSpgmr(mSolver, PREC_LEFT, 0);
                    else if (!linearSolver.compare(BiCgStabLinearSolver))
                        CVSpbcg(mSolver, PREC_LEFT, 0);
                    else
                        CVSptfqmr(mSolver, PREC_LEFT, 0);

                    CVBandPrecInit(mSolver, pRatesStatesCount, upperHalfBandwidth, lowerHalfBandwidth);
                } else {
                    if (!linearSolver.compare(GmresLinearSolver))
                        CVSpgmr(mSolver, PREC_NONE, 0);
                    else if (!linearSolver.compare(BiCgStabLinearSolver))
                        CVSpbcg(mSolver, PREC_NONE, 0);
                    else
                        CVSptfqmr(mSolver, PREC_NONE, 0);
                }
            }
        }

        // Set the relative and absolute tolerances

        CVodeSStolerances(mSolver, relativeTolerance, absoluteTolerance);

        // See if we are performing a sensitivity analysis

        if (pGradientsCount && pGradients) {

            // The number of constants that state variables have gradients computed

            mSensitivityVectorsSize = pGradientsCount;

            // Allocate senstivity vectors

            mSensitivityVectors = N_VCloneVectorArrayEmpty_Serial(mSensitivityVectorsSize, mStatesVector);

            for (int i = 0; i < mSensitivityVectorsSize; i++) {
                NV_DATA_S(mSensitivityVectors[i]) = pGradients;
                pGradients += pRatesStatesCount;
            }

            // Initialise sensitivity code

            CVodeSensInit1(mSolver, mSensitivityVectorsSize, CV_SIMULTANEOUS, NULL, mSensitivityVectors);

            CVodeSensEEtolerances(mSolver);

            CVodeSetSensErrCon(mSolver, TRUE);

            CVodeSetSensDQMethod(mSolver, CV_CENTERED, 0.0);

            // Specify which constants will have gradients calculated

            CVodeSetSensParams(mSolver, mUserData->constants(), NULL, pGradientsIndices);
        }
    } else {
        // Reinitialise the CVODE object

        CVodeReInit(mSolver, pVoiStart, mStatesVector);

        // Reinitialise sensitivity analysis

        if (mSensitivityVectors)
            CVodeSensReInit(mSolver, CV_SIMULTANEOUS, mSensitivityVectors);
    }
}

//==============================================================================

void CvodeSolver::solve(double &pVoi, const double &pVoiEnd) const
{
    // Solve the model

    if (!mInterpolateSolution)
        CVodeSetStopTime(mSolver, pVoiEnd);

    if (CVode(mSolver, pVoiEnd, mStatesVector, &pVoi, CV_NORMAL) >= 0) {
        // Get the sensitivity solution vectors if we are doing sensitivity analysis

        if (mSensitivityVectors)
            CVodeGetSens(mSolver, &pVoi, mSensitivityVectors);

        // Compute the rates one more time to get up to date values for the rates
        // Note: another way of doing this would be to copy the contents of the
        //       calculated rates in rhsFunction, but that's bound to be more time
        //       consuming since a call to CVode() is likely to generate at least a
        //       few calls to rhsFunction(), so that would be quite a few memory
        //       transfers while here we 'only' compute the rates one more time...

        mComputeRates(pVoiEnd, mConstants, mRates,
                      N_VGetArrayPointer_Serial(mStatesVector), mAlgebraic);
    }
}

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

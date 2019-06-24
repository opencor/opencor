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

#include "sundialsbegin.h"
    #include "cvodes/cvodes.h"
    #include "cvodes/cvodes_bandpre.h"
    #include "cvodes/cvodes_diag.h"
    #include "cvodes/cvodes_direct.h"
    #include "cvodes/cvodes_spils.h"
    #include "sunlinsol/sunlinsol_band.h"
    #include "sunlinsol/sunlinsol_dense.h"
    #include "sunlinsol/sunlinsol_spbcgs.h"
    #include "sunlinsol/sunlinsol_spgmr.h"
    #include "sunlinsol/sunlinsol_sptfqmr.h"
    #include "sunnonlinsol/sunnonlinsol_fixedpoint.h"
#include "sundialsend.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

int rhsFunction(double pVoi, N_Vector pStates, N_Vector pRates, void *pUserData)
{
    // Compute the RHS function

    auto userData = static_cast<CvodeSolverUserData *>(pUserData);

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
    Q_UNUSED(pModule)
    Q_UNUSED(pFunction)

    // Forward errors to our CvodeSolver object

    if (pErrorCode != CV_WARNING) {
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

CvodeSolver::~CvodeSolver()
{
    // Make sure that the solver has been initialised

    if (mSolver == nullptr) {
        return;
    }

    // Delete some internal objects

    N_VDestroy_Serial(mStatesVector);
    SUNLinSolFree(mLinearSolver);
    SUNNonlinSolFree(mNonLinearSolver);
    SUNMatDestroy(mMatrix);

    CVodeFree(&mSolver);

    delete mUserData;
}

//==============================================================================

void CvodeSolver::initialize(double pVoi, int pRatesStatesCount,
                             double *pConstants, double *pRates,
                             double *pStates, double *pAlgebraic,
                             ComputeRatesFunction pComputeRates)
{
    // Retrieve our properties

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
        emit error(tr(R"(the "Maximum step" property value could not be retrieved)"));

        return;
    }

    if (mProperties.contains(MaximumNumberOfStepsId)) {
        maximumNumberOfSteps = mProperties.value(MaximumNumberOfStepsId).toInt();
    } else {
        emit error(tr(R"(the "Maximum number of steps" property value could not be retrieved)"));

        return;
    }

    if (mProperties.contains(IntegrationMethodId)) {
        integrationMethod = mProperties.value(IntegrationMethodId).toString();
    } else {
        emit error(tr(R"(the "Integration method" property value could not be retrieved)"));

        return;
    }

    if (mProperties.contains(IterationTypeId)) {
        iterationType = mProperties.value(IterationTypeId).toString();

        if (iterationType == NewtonIteration) {
            // We are dealing with a Newton iteration, so retrieve and check its
            // linear solver

            if (mProperties.contains(LinearSolverId)) {
                linearSolver = mProperties.value(LinearSolverId).toString();

                bool needUpperAndLowerHalfBandwidths = false;

                if (   (linearSolver == DenseLinearSolver)
                    || (linearSolver == DiagonalLinearSolver)) {
                    // We are dealing with a dense/diagonal linear solver, so
                    // nothing more to do
                } else if (linearSolver == BandedLinearSolver) {
                    // We are dealing with a banded linear solver, so we need
                    // both an upper and a lower half bandwidth

                    needUpperAndLowerHalfBandwidths = true;
                } else {
                    // We are dealing with a GMRES/Bi-CGStab/TFQMR linear
                    // solver, so retrieve and check its preconditioner

                    if (mProperties.contains(PreconditionerId)) {
                        preconditioner = mProperties.value(PreconditionerId).toString();
                    } else {
                        emit error(tr(R"(the "Preconditioner" property value could not be retrieved)"));

                        return;
                    }

                    if (preconditioner == BandedPreconditioner) {
                        // We are dealing with a banded preconditioner, so we
                        // need both an upper and a lower half bandwidth

                        needUpperAndLowerHalfBandwidths = true;
                    }
                }

                if (needUpperAndLowerHalfBandwidths) {
                    if (mProperties.contains(UpperHalfBandwidthId)) {
                        upperHalfBandwidth = mProperties.value(UpperHalfBandwidthId).toInt();

                        if (upperHalfBandwidth >= pRatesStatesCount) {
                            emit error(tr(R"(the "Upper half-bandwidth" property must have a value between 0 and %1)").arg(pRatesStatesCount-1));

                            return;
                        }
                    } else {
                        emit error(tr(R"(the "Upper half-bandwidth" property value could not be retrieved)"));

                        return;
                    }

                    if (mProperties.contains(LowerHalfBandwidthId)) {
                        lowerHalfBandwidth = mProperties.value(LowerHalfBandwidthId).toInt();

                        if (lowerHalfBandwidth >= pRatesStatesCount) {
                            emit error(tr(R"(the "Lower half-bandwidth" property must have a value between 0 and %1)").arg(pRatesStatesCount-1));

                            return;
                        }
                    } else {
                        emit error(tr(R"(the "Lower half-bandwidth" property value could not be retrieved)"));

                        return;
                    }
                }
            } else {
                emit error(tr(R"(the "Linear solver" property value could not be retrieved)"));

                return;
            }
        }
    } else {
        emit error(tr(R"(the "Iteration type" property value could not be retrieved)"));

        return;
    }

    if (mProperties.contains(RelativeToleranceId)) {
        relativeTolerance = mProperties.value(RelativeToleranceId).toDouble();
    } else {
        emit error(tr(R"(the "Relative tolerance" property value could not be retrieved)"));

        return;
    }

    if (mProperties.contains(AbsoluteToleranceId)) {
        absoluteTolerance = mProperties.value(AbsoluteToleranceId).toDouble();
    } else {
        emit error(tr(R"(the "Absolute tolerance" property value could not be retrieved)"));

        return;
    }

    if (mProperties.contains(InterpolateSolutionId)) {
        mInterpolateSolution = mProperties.value(InterpolateSolutionId).toBool();
    } else {
        emit error(tr(R"(the "Interpolate solution" property value could not be retrieved)"));

        return;
    }

    // Initialise our ODE solver

    OdeSolver::initialize(pVoi, pRatesStatesCount, pConstants, pRates, pStates,
                          pAlgebraic, pComputeRates);

    // Create our states vector

    mStatesVector = N_VMake_Serial(pRatesStatesCount, pStates);

    // Create our CVODES solver

    bool newtonIteration = iterationType == NewtonIteration;

    mSolver = CVodeCreate((integrationMethod == BdfMethod)?CV_BDF:CV_ADAMS);

    // Use our own error handler

    CVodeSetErrHandlerFn(mSolver, errorHandler, this);

    // Initialise our CVODES solver

    CVodeInit(mSolver, rhsFunction, pVoi, mStatesVector);

    // Set our user data

    mUserData = new CvodeSolverUserData(pConstants, pAlgebraic, pComputeRates);

    CVodeSetUserData(mSolver, mUserData);

    // Set our maximum step

    CVodeSetMaxStep(mSolver, maximumStep);

    // Set our maximum number of steps

    CVodeSetMaxNumSteps(mSolver, maximumNumberOfSteps);

    // Set our linear solver, if needed

    if (newtonIteration) {
        if (linearSolver == DenseLinearSolver) {
            mMatrix = SUNDenseMatrix(pRatesStatesCount, pRatesStatesCount);
            mLinearSolver = SUNLinSol_Dense(mStatesVector, mMatrix);

            CVodeSetLinearSolver(mSolver, mLinearSolver, mMatrix);
        } else if (linearSolver == BandedLinearSolver) {
            mMatrix = SUNBandMatrix(pRatesStatesCount, upperHalfBandwidth,
                                                       lowerHalfBandwidth);
            mLinearSolver = SUNLinSol_Band(mStatesVector, mMatrix);

            CVodeSetLinearSolver(mSolver, mLinearSolver, mMatrix);
        } else if (linearSolver == DiagonalLinearSolver) {
            CVDiag(mSolver);
        } else {
            // We are dealing with a GMRES/Bi-CGStab/TFQMR linear solver

            if (preconditioner == BandedPreconditioner) {
                if (linearSolver == GmresLinearSolver) {
                    mLinearSolver = SUNLinSol_SPGMR(mStatesVector, PREC_LEFT, 0);
                } else if (linearSolver == BiCgStabLinearSolver) {
                    mLinearSolver = SUNLinSol_SPBCGS(mStatesVector, PREC_LEFT, 0);
                } else {
                    mLinearSolver = SUNLinSol_SPTFQMR(mStatesVector, PREC_LEFT, 0);
                }

                CVodeSetLinearSolver(mSolver, mLinearSolver, mMatrix);
                CVBandPrecInit(mSolver, pRatesStatesCount, upperHalfBandwidth,
                                                           lowerHalfBandwidth);
            } else {
                if (linearSolver == GmresLinearSolver) {
                    mLinearSolver = SUNLinSol_SPGMR(mStatesVector, PREC_NONE, 0);
                } else if (linearSolver == BiCgStabLinearSolver) {
                    mLinearSolver = SUNLinSol_SPBCGS(mStatesVector, PREC_NONE, 0);
                } else {
                    mLinearSolver = SUNLinSol_SPTFQMR(mStatesVector, PREC_NONE, 0);
                }

                CVodeSetLinearSolver(mSolver, mLinearSolver, mMatrix);
            }
        }
    } else {
        mNonLinearSolver = SUNNonlinSol_FixedPoint(mStatesVector, 0);

        CVodeSetNonlinearSolver(mSolver, mNonLinearSolver);
    }

    // Set our relative and absolute tolerances

    CVodeSStolerances(mSolver, relativeTolerance, absoluteTolerance);
}

//==============================================================================

void CvodeSolver::reinitialize(double pVoi)
{
    // Reinitialise our CVODES object

    CVodeReInit(mSolver, pVoi, mStatesVector);
}

//==============================================================================

void CvodeSolver::solve(double &pVoi, double pVoiEnd) const
{
    // Solve the model

    if (!mInterpolateSolution) {
        CVodeSetStopTime(mSolver, pVoiEnd);
    }

    CVode(mSolver, pVoiEnd, mStatesVector, &pVoi, CV_NORMAL);

    // Compute the rates one more time to get up to date values for the rates
    // Note: another way of doing this would be to copy the contents of the
    //       calculated rates in rhsFunction, but that's bound to be more time
    //       consuming since a call to CVode() is likely to generate at least a
    //       few calls to rhsFunction(), so that would be quite a few memory
    //       transfers while here we 'only' compute the rates one more time...

    mComputeRates(pVoiEnd, mConstants, mRates,
                  N_VGetArrayPointer_Serial(mStatesVector), mAlgebraic);
}

//==============================================================================

} // namespace CVODESolver
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

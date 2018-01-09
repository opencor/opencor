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
// IDAS solver
//==============================================================================

#include "idassolver.h"

//==============================================================================

#include "idas/idas.h"
#include "idas/idas_direct.h"
#include "idas/idas_spils.h"
#include "sunlinsol/sunlinsol_band.h"
#include "sunlinsol/sunlinsol_dense.h"
#include "sunlinsol/sunlinsol_spbcgs.h"
#include "sunlinsol/sunlinsol_spgmr.h"
#include "sunlinsol/sunlinsol_sptfqmr.h"

//==============================================================================

namespace OpenCOR {
namespace IDASSolver {

//==============================================================================

int residualFunction(double pVoi, N_Vector pStates, N_Vector pRates,
                     N_Vector pResiduals, void *pUserData)
{
    // Compute the residual function

    IdasSolverUserData *userData = static_cast<IdasSolverUserData *>(pUserData);

    double *rates = N_VGetArrayPointer(pRates);
    double *states = N_VGetArrayPointer(pStates);
    double *residuals = N_VGetArrayPointer(pResiduals);

    userData->computeRootInformation()(pVoi, userData->constants(), rates,
                                       userData->oldRates(), states,
                                       userData->oldStates(),
                                       userData->algebraic(),
                                       userData->condVar());

    userData->computeEssentialVariables()(pVoi, userData->constants(), rates,
                                          userData->oldRates(), states,
                                          userData->oldStates(),
                                          userData->algebraic(),
                                          userData->condVar());

    userData->computeRates()(pVoi, userData->constants(), rates,
                             userData->oldRates(), states,
                             userData->oldStates(), userData->algebraic(),
                             userData->condVar(), residuals);

    return 0;
}

//==============================================================================

int rootFindingFunction(double pVoi, N_Vector pStates, N_Vector pRates,
                        double *pRoots, void *pUserData)
{
    // Compute the root finding function

    IdasSolverUserData *userData = static_cast<IdasSolverUserData *>(pUserData);

    userData->computeRootInformation()(pVoi, userData->constants(),
                                       N_VGetArrayPointer(pRates),
                                       userData->oldRates(),
                                       N_VGetArrayPointer(pStates),
                                       userData->oldStates(),
                                       userData->algebraic(), pRoots);

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char *pModule, const char *pFunction,
                  char *pErrorMessage, void *pUserData)
{
    Q_UNUSED(pModule);
    Q_UNUSED(pFunction);

    if (pErrorCode != IDA_WARNING) {
        // IDAS generated an error, so forward it to the IdasSolver object

        static_cast<IdasSolver *>(pUserData)->emitError(pErrorMessage);
    }
}

//==============================================================================

IdasSolverUserData::IdasSolverUserData(double *pConstants, double *pOldRates,
                                       double *pOldStates, double *pAlgebraic,
                                       double *pCondVar,
                                       Solver::DaeSolver::ComputeRatesFunction pComputeRates,
                                       Solver::DaeSolver::ComputeEssentialVariablesFunction pComputeEssentialVariables,
                                       Solver::DaeSolver::ComputeRootInformationFunction pComputeRootInformation) :
    mConstants(pConstants),
    mOldRates(pOldRates),
    mOldStates(pOldStates),
    mAlgebraic(pAlgebraic),
    mCondVar(pCondVar),
    mComputeRates(pComputeRates),
    mComputeEssentialVariables(pComputeEssentialVariables),
    mComputeRootInformation(pComputeRootInformation)
{
}

//==============================================================================

double * IdasSolverUserData::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double * IdasSolverUserData::oldRates() const
{
    // Return our old rates array

    return mOldRates;
}

//==============================================================================

double * IdasSolverUserData::oldStates() const
{
    // Return our old states array

    return mOldStates;
}

//==============================================================================

double * IdasSolverUserData::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

double * IdasSolverUserData::condVar() const
{
    // Return our condVar array

    return mCondVar;
}

//==============================================================================

Solver::DaeSolver::ComputeRatesFunction IdasSolverUserData::computeRates() const
{
    // Return our compute rates function

    return mComputeRates;
}

//==============================================================================

Solver::DaeSolver::ComputeEssentialVariablesFunction IdasSolverUserData::computeEssentialVariables() const
{
    // Return our compute essential variables function

    return mComputeEssentialVariables;
}

//==============================================================================

Solver::DaeSolver::ComputeRootInformationFunction IdasSolverUserData::computeRootInformation() const
{
    // Return our compute root information function

    return mComputeRootInformation;
}

//==============================================================================

IdasSolver::IdasSolver() :
    mSolver(0),
    mRatesVector(0),
    mStatesVector(0),
    mMatrix(0),
    mLinearSolver(0),
    mUserData(0),
    mInterpolateSolution(InterpolateSolutionDefaultValue)
{
}

//==============================================================================

IdasSolver::~IdasSolver()
{
    // Make sure that the solver has been initialised

    if (!mSolver)
        return;

    // Delete some internal objects

    N_VDestroy_Serial(mRatesVector);
    N_VDestroy_Serial(mStatesVector);
    SUNLinSolFree(mLinearSolver);
    SUNMatDestroy(mMatrix);

    IDAFree(&mSolver);

    delete mUserData;
}

//==============================================================================

void IdasSolver::initialize(const double &pVoiStart, const double &pVoiEnd,
                           const int &pRatesStatesCount,
                           const int &pCondVarCount, double *pConstants,
                           double *pRates, double *pStates, double *pAlgebraic,
                           double *pCondVar, ComputeRatesFunction pComputeRates,
                           ComputeEssentialVariablesFunction pComputeEssentialVariables,
                           ComputeRootInformationFunction pComputeRootInformation,
                           ComputeStateInformationFunction pComputeStateInformation)
{
    // Check whether we need to initialise or reinitialise ourselves

    if (!mSolver) {
        // Retrieve our properties

        double maximumStep = MaximumStepDefaultValue;
        int maximumNumberOfSteps = MaximumNumberOfStepsDefaultValue;
        QString linearSolver = LinearSolverDefaultValue;
        int upperHalfBandwidth = UpperHalfBandwidthDefaultValue;
        int lowerHalfBandwidth = LowerHalfBandwidthDefaultValue;
        double relativeTolerance = RelativeToleranceDefaultValue;
        double absoluteTolerance = AbsoluteToleranceDefaultValue;

        if (mProperties.contains(MaximumStepId)) {
            maximumStep = mProperties.value(MaximumStepId).toDouble();
        } else {
            emit error(tr("the \"Maximum step\" property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(MaximumNumberOfStepsId)) {
            maximumNumberOfSteps = mProperties.value(MaximumNumberOfStepsId).toInt();
        } else {
            emit error(tr("the \"Maximum number of steps\" property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(LinearSolverId)) {
            linearSolver = mProperties.value(LinearSolverId).toString();

            if (!linearSolver.compare(BandedLinearSolver)) {
                if (mProperties.contains(UpperHalfBandwidthId)) {
                    upperHalfBandwidth = mProperties.value(UpperHalfBandwidthId).toInt();

                    if (   (upperHalfBandwidth < 0)
                        || (upperHalfBandwidth >= pRatesStatesCount)) {
                        emit error(tr("the \"Upper half-bandwidth\" property must have a value between 0 and %1").arg(pRatesStatesCount-1));

                        return;
                    }
                } else {
                    emit error(tr("the \"Upper half-bandwidth\" property value could not be retrieved"));

                    return;
                }

                if (mProperties.contains(LowerHalfBandwidthId)) {
                    lowerHalfBandwidth = mProperties.value(LowerHalfBandwidthId).toInt();

                    if (   (lowerHalfBandwidth < 0)
                        || (lowerHalfBandwidth >= pRatesStatesCount)) {
                        emit error(tr("the \"Lower half-bandwidth\" property must have a value between 0 and %1").arg(pRatesStatesCount-1));

                        return;
                    }
                } else {
                    emit error(tr("the \"Lower half-bandwidth\" property value could not be retrieved"));

                    return;
                }
            }
        } else {
            emit error(tr("the \"Linear solver\" property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(RelativeToleranceId)) {
            relativeTolerance = mProperties.value(RelativeToleranceId).toDouble();

            if (relativeTolerance < 0) {
                emit error(tr("the \"Relative tolerance\" property must have a value greater than or equal to 0"));

                return;
            }
        } else {
            emit error(tr("the \"Relative tolerance\" property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(AbsoluteToleranceId)) {
            absoluteTolerance = mProperties.value(AbsoluteToleranceId).toDouble();

            if (absoluteTolerance < 0) {
                emit error(tr("the \"Absolute tolerance\" property must have a value greater than or equal to 0"));

                return;
            }
        } else {
            emit error(tr("the \"Absolute tolerance\" property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(InterpolateSolutionId)) {
            mInterpolateSolution = mProperties.value(InterpolateSolutionId).toBool();
        } else {
            emit error(tr("the \"Interpolate solution\" property value could not be retrieved"));

            return;
        }

        // Initialise our DAE solver

        OpenCOR::Solver::DaeSolver::initialize(pVoiStart, pVoiEnd,
                                               pRatesStatesCount, pCondVarCount,
                                               pConstants, pRates, pStates,
                                               pAlgebraic, pCondVar,
                                               pComputeRates,
                                               pComputeEssentialVariables,
                                               pComputeRootInformation,
                                               pComputeStateInformation);

        // Create our states vector

        mRatesVector = N_VMake_Serial(pRatesStatesCount, pRates);
        mStatesVector = N_VMake_Serial(pRatesStatesCount, pStates);

        // Create our IDAS solver

        mSolver = IDACreate();

        // Use our own error handler

        IDASetErrHandlerFn(mSolver, errorHandler, this);

        // Initialise our IDAS solver

        IDAInit(mSolver, residualFunction, pVoiStart,
                mStatesVector, mRatesVector);

        IDARootInit(mSolver, pCondVarCount, rootFindingFunction);
        //---OPENCOR--- NEED TO CHECK THAT THINGS WORK AS EXPECTED BY TRYING IT
        //              OUT ON A MODEL THAT NEEDS ROOT FINDING (E.G. THE
        //              SAUCERMAN MODEL)...

        // Set our user data

        mUserData = new IdasSolverUserData(pConstants, mOldRates, mOldStates,
                                           pAlgebraic, pCondVar, pComputeRates,
                                           pComputeEssentialVariables,
                                           pComputeRootInformation);

        IDASetUserData(mSolver, mUserData);

        // Set our maximum step

        IDASetMaxStep(mSolver, maximumStep);

        // Set our maximum number of steps

        IDASetMaxNumSteps(mSolver, maximumNumberOfSteps);

        // Set our linear solver

        if (!linearSolver.compare(DenseLinearSolver)) {
            mMatrix = SUNDenseMatrix(pRatesStatesCount, pRatesStatesCount);
            mLinearSolver = SUNDenseLinearSolver(mStatesVector, mMatrix);

            IDADlsSetLinearSolver(mSolver, mLinearSolver, mMatrix);
        } else if (!linearSolver.compare(BandedLinearSolver)) {
            mMatrix = SUNBandMatrix(pRatesStatesCount,
                                    upperHalfBandwidth, lowerHalfBandwidth,
                                    upperHalfBandwidth+lowerHalfBandwidth);
            mLinearSolver = SUNBandLinearSolver(mStatesVector, mMatrix);

            IDADlsSetLinearSolver(mSolver, mLinearSolver, mMatrix);
        } else if (!linearSolver.compare(GmresLinearSolver)) {
            mLinearSolver = SUNSPGMR(mStatesVector, PREC_LEFT, 0);

            IDASpilsSetLinearSolver(mSolver, mLinearSolver);
        } else if (!linearSolver.compare(BiCgStabLinearSolver)) {
            mLinearSolver = SUNSPBCGS(mStatesVector, PREC_LEFT, 0);

            IDASpilsSetLinearSolver(mSolver, mLinearSolver);
        } else {
            mLinearSolver = SUNSPTFQMR(mStatesVector, PREC_LEFT, 0);

            IDASpilsSetLinearSolver(mSolver, mLinearSolver);
        }

        // Set our relative and absolute tolerances

        IDASStolerances(mSolver, relativeTolerance, absoluteTolerance);
    } else {
        // Reinitialise our IDAS object

        IDAReInit(mSolver, pVoiStart, mStatesVector, mRatesVector);
    }

    // Compute our model's (new) initial conditions

    double *id = new double[pRatesStatesCount];

    pComputeStateInformation(id);

    N_Vector idVector = N_VMake_Serial(pRatesStatesCount, id);

    IDASetId(mSolver, idVector);

    IDACalcIC(mSolver, IDA_YA_YDP_INIT, pVoiEnd);
    IDAGetConsistentIC(mSolver, mStatesVector, mRatesVector);

    N_VDestroy_Serial(idVector);

    delete[] id;
}

//==============================================================================

void IdasSolver::solve(double &pVoi, const double &pVoiEnd) const
{
    // Solve the model

    if (!mInterpolateSolution)
        IDASetStopTime(mSolver, pVoiEnd);

    IDASolve(mSolver, pVoiEnd, &pVoi, mStatesVector, mRatesVector, IDA_NORMAL);

    memcpy(mOldRates, N_VGetArrayPointer(mRatesVector), mRatesStatesCount*OpenCOR::Solver::SizeOfDouble);
    memcpy(mOldStates, N_VGetArrayPointer(mStatesVector), mRatesStatesCount*OpenCOR::Solver::SizeOfDouble);
}

//==============================================================================

}   // namespace IDASSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

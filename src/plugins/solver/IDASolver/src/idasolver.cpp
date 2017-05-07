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
// IDA solver
//==============================================================================

#include "idasolver.h"

//==============================================================================

#include "ida/ida.h"
#include "ida/ida_band.h"
#include "ida/ida_dense.h"
#include "ida/ida_spbcgs.h"
#include "ida/ida_spgmr.h"
#include "ida/ida_sptfqmr.h"

//==============================================================================

namespace OpenCOR {
namespace IDASolver {

//==============================================================================

int residualFunction(double pVoi, N_Vector pStates, N_Vector pRates,
                     N_Vector pResiduals, void *pUserData)
{
    // Compute the residual function

    IdaSolverUserData *userData = static_cast<IdaSolverUserData *>(pUserData);

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

    userData->computeResiduals()(pVoi, userData->constants(), rates,
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

    IdaSolverUserData *userData = static_cast<IdaSolverUserData *>(pUserData);

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
        // IDA generated an error, so forward it to the IdaSolver object

        static_cast<IdaSolver *>(pUserData)->emitError(pErrorMessage);
    }
}

//==============================================================================

IdaSolverUserData::IdaSolverUserData(double *pConstants, double *pOldRates,
                                     double *pOldStates, double *pAlgebraic,
                                     double *pCondVar,
                                     Solver::DaeSolver::ComputeEssentialVariablesFunction pComputeEssentialVariables,
                                     Solver::DaeSolver::ComputeResidualsFunction pComputeResiduals,
                                     Solver::DaeSolver::ComputeRootInformationFunction pComputeRootInformation) :
    mConstants(pConstants),
    mOldRates(pOldRates),
    mOldStates(pOldStates),
    mAlgebraic(pAlgebraic),
    mCondVar(pCondVar),
    mComputeEssentialVariables(pComputeEssentialVariables),
    mComputeResiduals(pComputeResiduals),
    mComputeRootInformation(pComputeRootInformation)
{
}

//==============================================================================

double * IdaSolverUserData::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double * IdaSolverUserData::oldRates() const
{
    // Return our old rates array

    return mOldRates;
}

//==============================================================================

double * IdaSolverUserData::oldStates() const
{
    // Return our old states array

    return mOldStates;
}

//==============================================================================

double * IdaSolverUserData::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

double * IdaSolverUserData::condVar() const
{
    // Return our condVar array

    return mCondVar;
}

//==============================================================================

Solver::DaeSolver::ComputeEssentialVariablesFunction IdaSolverUserData::computeEssentialVariables() const
{
    // Return our compute essential variables function

    return mComputeEssentialVariables;
}

//==============================================================================

Solver::DaeSolver::ComputeResidualsFunction IdaSolverUserData::computeResiduals() const
{
    // Return our compute residuals function

    return mComputeResiduals;
}

//==============================================================================

Solver::DaeSolver::ComputeRootInformationFunction IdaSolverUserData::computeRootInformation() const
{
    // Return our compute root information function

    return mComputeRootInformation;
}

//==============================================================================

IdaSolver::IdaSolver() :
    mSolver(0),
    mRatesVector(0),
    mStatesVector(0),
    mUserData(0),
    mInterpolateSolution(InterpolateSolutionDefaultValue)
{
}

//==============================================================================

IdaSolver::~IdaSolver()
{
    // Make sure that the solver has been initialised

    if (!mSolver)
        return;

    // Delete some internal objects

    N_VDestroy_Serial(mRatesVector);
    N_VDestroy_Serial(mStatesVector);

    IDAFree(&mSolver);

    delete mUserData;
}

//==============================================================================

void IdaSolver::initialize(const double &pVoiStart, const double &pVoiEnd,
                           const int &pRatesStatesCount,
                           const int &pCondVarCount, double *pConstants,
                           double *pRates, double *pStates, double *pAlgebraic,
                           double *pCondVar,
                           ComputeEssentialVariablesFunction pComputeEssentialVariables,
                           ComputeResidualsFunction pComputeResiduals,
                           ComputeRootInformationFunction pComputeRootInformation,
                           ComputeStateInformationFunction pComputeStateInformation)
{
    if (!mSolver) {
        // Retrieve some of the IDA properties

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
            emit error(QObject::tr("the 'maximum step' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(MaximumNumberOfStepsId)) {
            maximumNumberOfSteps = mProperties.value(MaximumNumberOfStepsId).toInt();
        } else {
            emit error(QObject::tr("the 'maximum number of steps' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(LinearSolverId)) {
            linearSolver = mProperties.value(LinearSolverId).toString();

            if (!linearSolver.compare(BandedLinearSolver)) {
                if (mProperties.contains(UpperHalfBandwidthId)) {
                    upperHalfBandwidth = mProperties.value(UpperHalfBandwidthId).toInt();

                    if (   (upperHalfBandwidth < 0)
                        || (upperHalfBandwidth >= pRatesStatesCount)) {
                        emit error(QObject::tr("the 'upper half-bandwidth' property must have a value between 0 and %1").arg(pRatesStatesCount-1));

                        return;
                    }
                } else {
                    emit error(QObject::tr("the 'upper half-bandwidth' property value could not be retrieved"));

                    return;
                }

                if (mProperties.contains(LowerHalfBandwidthId)) {
                    lowerHalfBandwidth = mProperties.value(LowerHalfBandwidthId).toInt();

                    if (   (lowerHalfBandwidth < 0)
                        || (lowerHalfBandwidth >= pRatesStatesCount)) {
                        emit error(QObject::tr("the 'lower half-bandwidth' property must have a value between 0 and %1").arg(pRatesStatesCount-1));

                        return;
                    }
                } else {
                    emit error(QObject::tr("the 'lower half-bandwidth' property value could not be retrieved"));

                    return;
                }
            }
        } else {
            emit error(QObject::tr("the 'linear solver' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(RelativeToleranceId)) {
            relativeTolerance = mProperties.value(RelativeToleranceId).toDouble();

            if (relativeTolerance < 0) {
                emit error(QObject::tr("the 'relative tolerance' property must have a value greater than or equal to 0"));

                return;
            }
        } else {
            emit error(QObject::tr("the 'relative tolerance' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(AbsoluteToleranceId)) {
            absoluteTolerance = mProperties.value(AbsoluteToleranceId).toDouble();

            if (absoluteTolerance < 0) {
                emit error(QObject::tr("the 'absolute tolerance' property must have a value greater than or equal to 0"));

                return;
            }
        } else {
            emit error(QObject::tr("the 'absolute tolerance' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(InterpolateSolutionId)) {
            mInterpolateSolution = mProperties.value(InterpolateSolutionId).toBool();
        } else {
            emit error(QObject::tr("the 'interpolate solution' property value could not be retrieved"));

            return;
        }

        // Initialise the DAE solver itself

        OpenCOR::Solver::DaeSolver::initialize(pVoiStart, pVoiEnd,
                                               pRatesStatesCount, pCondVarCount,
                                               pConstants, pRates, pStates,
                                               pAlgebraic, pCondVar,
                                               pComputeEssentialVariables,
                                               pComputeResiduals,
                                               pComputeRootInformation,
                                               pComputeStateInformation);

        // Create the states vector

        mRatesVector = N_VMake_Serial(pRatesStatesCount, pRates);
        mStatesVector = N_VMake_Serial(pRatesStatesCount, pStates);

        // Create the IDA solver

        mSolver = IDACreate();

        // Use our own error handler

        IDASetErrHandlerFn(mSolver, errorHandler, this);

        // Initialise the IDA solver

        IDAInit(mSolver, residualFunction, pVoiStart,
                mStatesVector, mRatesVector);

        IDARootInit(mSolver, pCondVarCount, rootFindingFunction);
        //---GRY--- NEED TO CHECK THAT THINGS WORK AS EXPECTED BY TRYING IT OUT
        //          ON A MODEL THAT NEEDS ROOT FINDING (E.G. THE SAUCERMAN
        //          MODEL)...

        // Set some user data

        mUserData = new IdaSolverUserData(pConstants, mOldRates, mOldStates,
                                          pAlgebraic, pCondVar,
                                          pComputeEssentialVariables,
                                          pComputeResiduals,
                                          pComputeRootInformation);

        IDASetUserData(mSolver, mUserData);

        // Set the linear solver

        if (!linearSolver.compare(DenseLinearSolver))
            IDADense(mSolver, pRatesStatesCount);
        else if (!linearSolver.compare(BandedLinearSolver))
            IDABand(mSolver, pRatesStatesCount, upperHalfBandwidth, lowerHalfBandwidth);
        else if (!linearSolver.compare(GmresLinearSolver))
            IDASpgmr(mSolver, 0);
        else if (!linearSolver.compare(BiCgStabLinearSolver))
            IDASpbcg(mSolver, 0);
        else
            IDASptfqmr(mSolver, 0);

        // Set the maximum step

        IDASetMaxStep(mSolver, maximumStep);

        // Set the maximum number of steps

        IDASetMaxNumSteps(mSolver, maximumNumberOfSteps);

        // Set the relative and absolute tolerances

        IDASStolerances(mSolver, relativeTolerance, absoluteTolerance);
    } else {
        // Reinitialise the IDA object

        IDAReInit(mSolver, pVoiStart, mStatesVector, mRatesVector);
    }

    // Compute the model's (new) initial conditions

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

void IdaSolver::solve(double &pVoi, const double &pVoiEnd) const
{
    // Solve the model

    if (!mInterpolateSolution)
        IDASetStopTime(mSolver, pVoiEnd);

    IDASolve(mSolver, pVoiEnd, &pVoi, mStatesVector, mRatesVector, IDA_NORMAL);

    memcpy(mOldRates, N_VGetArrayPointer(mRatesVector), mRatesStatesCount*OpenCOR::Solver::SizeOfDouble);
    memcpy(mOldStates, N_VGetArrayPointer(mStatesVector), mRatesStatesCount*OpenCOR::Solver::SizeOfDouble);
}

//==============================================================================

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

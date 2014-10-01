/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// IDA solver class
//==============================================================================

#include "idasolver.h"

//==============================================================================

#include "ida/ida.h"
#include "ida/ida_dense.h"
#include "ida/ida_impl.h"

//==============================================================================

namespace OpenCOR {
namespace IDASolver {

//==============================================================================

int residualFunction(double pVoi, N_Vector pStates, N_Vector pRates,
                     N_Vector pResiduals, void *pUserData)
{
    // Compute the residual function

    IdaSolverUserData *userData = static_cast<IdaSolverUserData *>(pUserData);

    double *rates     = N_VGetArrayPointer(pRates);
    double *states    = N_VGetArrayPointer(pStates);
    double *residuals = N_VGetArrayPointer(pResiduals);

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

    if (pErrorCode != IDA_WARNING)
        // IDA generated an error, so forward it to the IdaSolver object

        static_cast<IdaSolver *>(pUserData)->emitError(pErrorMessage);
}

//==============================================================================

IdaSolverUserData::IdaSolverUserData(double *pConstants, double *pOldRates,
                                     double *pOldStates, double *pAlgebraic,
                                     double *pCondVar,
                                     CoreSolver::CoreDaeSolver::ComputeEssentialVariablesFunction pComputeEssentialVariables,
                                     CoreSolver::CoreDaeSolver::ComputeResidualsFunction pComputeResiduals,
                                     CoreSolver::CoreDaeSolver::ComputeRootInformationFunction pComputeRootInformation) :
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

CoreSolver::CoreDaeSolver::ComputeEssentialVariablesFunction IdaSolverUserData::computeEssentialVariables() const
{
    // Return our compute essential variables function

    return mComputeEssentialVariables;
}

//==============================================================================

CoreSolver::CoreDaeSolver::ComputeResidualsFunction IdaSolverUserData::computeResiduals() const
{
    // Return our compute residuals function

    return mComputeResiduals;
}

//==============================================================================

CoreSolver::CoreDaeSolver::ComputeRootInformationFunction IdaSolverUserData::computeRootInformation() const
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
    mMaximumStep(MaximumStepDefaultValue),
    mMaximumNumberOfSteps(MaximumNumberOfStepsDefaultValue),
    mRelativeTolerance(RelativeToleranceDefaultValue),
    mAbsoluteTolerance(AbsoluteToleranceDefaultValue),
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

        if (mProperties.contains(MaximumStepId)) {
            mMaximumStep = mProperties.value(MaximumStepId).toDouble();
        } else {
            emit error(QObject::tr("the 'maximum step' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(MaximumNumberOfStepsId)) {
            mMaximumNumberOfSteps = mProperties.value(MaximumNumberOfStepsId).toInt();
        } else {
            emit error(QObject::tr("the 'maximum number of steps' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(RelativeToleranceId)) {
            mRelativeTolerance = mProperties.value(RelativeToleranceId).toDouble();
        } else {
            emit error(QObject::tr("the 'relative tolerance' property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(AbsoluteToleranceId)) {
            mAbsoluteTolerance = mProperties.value(AbsoluteToleranceId).toDouble();
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

        OpenCOR::CoreSolver::CoreDaeSolver::initialize(pVoiStart, pVoiEnd,
                                                       pRatesStatesCount,
                                                       pCondVarCount,
                                                       pConstants, pRates,
                                                       pStates, pAlgebraic,
                                                       pCondVar,
                                                       pComputeEssentialVariables,
                                                       pComputeResiduals,
                                                       pComputeRootInformation,
                                                       pComputeStateInformation);

        // Create the states vector

        mRatesVector  = N_VMake_Serial(pRatesStatesCount, pRates);
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
        //          ON A MODEL WHICH NEEDS ROOT FINDING (E.G. THE SAUCERMAN
        //          MODEL)...

        // Set some user data

        delete mUserData;   // Just in case the solver got initialised before

        mUserData = new IdaSolverUserData(pConstants, mOldRates, mOldStates,
                                          pAlgebraic, pCondVar,
                                          pComputeEssentialVariables,
                                          pComputeResiduals,
                                          pComputeRootInformation);

        IDASetUserData(mSolver, mUserData);

        // Set the linear solver

        IDADense(mSolver, pRatesStatesCount);

        // Set the maximum step

        IDASetMaxStep(mSolver, mMaximumStep);

        // Set the maximum number of steps

        IDASetMaxNumSteps(mSolver, mMaximumNumberOfSteps);

        // Set the relative and absolute tolerances

        IDASStolerances(mSolver, mRelativeTolerance, mAbsoluteTolerance);
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

    memcpy(mOldRates, N_VGetArrayPointer(mRatesVector), mRatesStatesCount*OpenCOR::CoreSolver::SizeOfDouble);
    memcpy(mOldStates, N_VGetArrayPointer(mStatesVector), mRatesStatesCount*OpenCOR::CoreSolver::SizeOfDouble);
}

//==============================================================================

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

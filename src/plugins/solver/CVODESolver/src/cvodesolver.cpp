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
// CVODE solver class
//==============================================================================

#include "cvodesolver.h"

//==============================================================================

#include "cvode/cvode.h"
#include "cvode/cvode_dense.h"

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

    if (pErrorCode != CV_WARNING)
        // CVODE generated an error, so forward it to the CvodeSolver object

        reinterpret_cast<CvodeSolver *>(pUserData)->emitError(pErrorMessage);
}

//==============================================================================

CvodeSolverUserData::CvodeSolverUserData(double *pConstants, double *pAlgebraic,
                                         CoreSolver::CoreOdeSolver::ComputeRatesFunction pComputeRates) :
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

CoreSolver::CoreOdeSolver::ComputeRatesFunction CvodeSolverUserData::computeRates() const
{
    // Return our compute rates function

    return mComputeRates;
}

//==============================================================================

CvodeSolver::CvodeSolver() :
    mSolver(0),
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

CvodeSolver::~CvodeSolver()
{
    // Make sure that the solver has been initialised

    if (!mSolver)
        return;

    // Delete some internal objects

    N_VDestroy_Serial(mStatesVector);

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
    if (!mSolver) {
        // Retrieve some of the CVODE properties

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

        // Initialise the ODE solver itself

        OpenCOR::CoreSolver::CoreOdeSolver::initialize(pVoiStart,
                                                       pRatesStatesCount,
                                                       pConstants, pRates,
                                                       pStates, pAlgebraic,
                                                       pComputeRates);

        // Create the states vector

        mStatesVector = N_VMake_Serial(pRatesStatesCount, pStates);

        // Create the CVODE solver

        mSolver = CVodeCreate(CV_BDF, CV_NEWTON);

        // Use our own error handler

        CVodeSetErrHandlerFn(mSolver, errorHandler, this);

        // Initialise the CVODE solver

        CVodeInit(mSolver, rhsFunction, pVoiStart, mStatesVector);

        // Set some user data

        delete mUserData;   // Just in case the solver got initialised before

        mUserData = new CvodeSolverUserData(pConstants, pAlgebraic,
                                            pComputeRates);

        CVodeSetUserData(mSolver, mUserData);

        // Set the linear solver

        CVDense(mSolver, pRatesStatesCount);

        // Set the maximum step

        CVodeSetMaxStep(mSolver, mMaximumStep);

        // Set the maximum number of steps

        CVodeSetMaxNumSteps(mSolver, mMaximumNumberOfSteps);

        // Set the relative and absolute tolerances

        CVodeSStolerances(mSolver, mRelativeTolerance, mAbsoluteTolerance);
    } else {
        // Reinitialise the CVODE object

        CVodeReInit(mSolver, pVoiStart, mStatesVector);
    }
}

//==============================================================================

void CvodeSolver::solve(double &pVoi, const double &pVoiEnd) const
{
    // Solve the model

    if (!mInterpolateSolution)
        CVodeSetStopTime(mSolver, pVoiEnd);

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

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

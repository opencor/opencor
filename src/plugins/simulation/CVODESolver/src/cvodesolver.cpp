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

    CvodeSolverUserData *userData = reinterpret_cast<CvodeSolverUserData *>(pUserData);

    userData->computeRates()(pVoi, userData->constants(),
                             N_VGetArrayPointer_Serial(pRates),
                             N_VGetArrayPointer_Serial(pStates),
                             userData->algebraic());

    // Everything went fine, so...

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char *pModule, const char *pFunction,
                  char *pErrorMsg, void *pUserData)
{
    Q_UNUSED(pModule);
    Q_UNUSED(pFunction);

    if (pErrorCode != CV_WARNING)
        // CVODE generated an error, so forward it to the CvodeSolver object

        reinterpret_cast<CvodeSolver *>(pUserData)->emitError(pErrorMsg);
}

//==============================================================================

CvodeSolverUserData::CvodeSolverUserData(double *pConstants, double *pAlgebraic,
                                         OpenCOR::CoreSolver::CoreOdeSolver::ComputeRatesFunction pComputeRates) :
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

OpenCOR::CoreSolver::CoreOdeSolver::ComputeRatesFunction CvodeSolverUserData::computeRates() const
{
    // Return our compute rates function

    return mComputeRates;
}

//==============================================================================

CvodeSolver::CvodeSolver() :
    mSolver(0),
    mStatesVector(0),
    mUserData(0),
    mMaximumStep(DefaultMaximumStep),
    mMaximumNumberOfSteps(DefaultMaximumNumberOfSteps),
    mRelativeTolerance(DefaultRelativeTolerance),
    mAbsoluteTolerance(DefaultAbsoluteTolerance)
{
}

//==============================================================================

CvodeSolver::~CvodeSolver()
{
    if (!mSolver)
        // The solver hasn't been initialised, so...

        return;

    // Delete some internal objects

    N_VDestroy_Serial(mStatesVector);

    CVodeFree(&mSolver);

    delete mUserData;
}

//==============================================================================

void CvodeSolver::initialize(const double &pVoiStart, const int &pStatesCount,
                             double *pConstants, double *pStates,
                             double *pRates, double *pAlgebraic,
                             ComputeRatesFunction pComputeRates)
{
    if (!mSolver) {
        // Initialise the ODE solver itself

        OpenCOR::CoreSolver::CoreOdeSolver::initialize(pVoiStart, pStatesCount,
                                                       pConstants, pStates,
                                                       pRates, pAlgebraic,
                                                       pComputeRates);

        // Retrieve some of the CVODE properties

        if (mProperties.contains(MaximumStepProperty))
            mMaximumStep = mProperties.value(MaximumStepProperty).toDouble();
        else
            emit error(QObject::tr("the 'maximum step' property value could not be retrieved"));

        if (mProperties.contains(MaximumNumberOfStepsProperty))
            mMaximumNumberOfSteps = mProperties.value(MaximumNumberOfStepsProperty).toInt();
        else
            emit error(QObject::tr("the 'maximum number of steps' property value could not be retrieved"));

        if (mProperties.contains(RelativeToleranceProperty))
            mRelativeTolerance = mProperties.value(RelativeToleranceProperty).toDouble();
        else
            emit error(QObject::tr("the 'relative tolerance' property value could not be retrieved"));

        if (mProperties.contains(AbsoluteToleranceProperty))
            mAbsoluteTolerance = mProperties.value(AbsoluteToleranceProperty).toDouble();
        else
            emit error(QObject::tr("the 'absolute tolerance' property value could not be retrieved"));

        // Create the states vector

        mStatesVector = N_VMake_Serial(pStatesCount, pStates);

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

        CVDense(mSolver, pStatesCount);

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

    CVode(mSolver, pVoiEnd, mStatesVector, &pVoi, CV_NORMAL);

    // Compute the rates one more time to get up-to-date values for the rates
    // Note: another way of doing this would be to copy the contents of the
    //       calculated rates in rhsFunction, but that's bound to be more time
    //       consuming since a call to CVode is likely to generate at least a
    //       few calls to rhsFunction, so that would be quite a few memory
    //       transfers while here we 'only' compute the rates one more time,
    //       so...

    mComputeRates(pVoiEnd, mConstants, mRates,
                  N_VGetArrayPointer_Serial(mStatesVector), mAlgebraic);
}

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

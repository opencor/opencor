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

// Default CVODE parameter values
// Note #1: a maximum step of 0 means that there is no maximum step as such and
//          that CVODE can use whatever step it sees fit...
// Note #2: CVODE's default maximum number of steps is 500 which ought to be big
//          enough in most cases...

static const double DefaultMaximumStep = 0;
static const int DefaultMaximumNumberOfSteps = 500;
static const double DefaultRelativeTolerance = 1e-7;
static const double DefaultAbsoluteTolerance = 1e-7;

//==============================================================================

int rhsFunction(double pVoi, N_Vector pStates, N_Vector pRates, void *pUserData)
{
    // Compute the model

    CVODESolverUserData *userData = reinterpret_cast<CVODESolverUserData *>(pUserData);

    userData->computeRates(pVoi, userData->constants,
                           N_VGetArrayPointer_Serial(pRates),
                           N_VGetArrayPointer_Serial(pStates),
                           userData->algebraic);

    // Everything went fine, so...

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char */* pModule */,
                  const char */* pFunction */, char *pErrorMsg, void *pUserData)
{
    if (pErrorCode != CV_WARNING) {
        // CVODE really generated an error, so forward it to the CVODESolver
        // object

        QString errorMsg = pErrorMsg;

        reinterpret_cast<CVODESolver *>(pUserData)->emitError(errorMsg.left(1).toLower()+errorMsg.right(errorMsg.size()-1));
    }
}

//==============================================================================

CVODESolver::CVODESolver() :
    mSolver(0),
    mStatesVector(0),
    mMaximumStep(DefaultMaximumStep),
    mMaximumNumberOfSteps(DefaultMaximumNumberOfSteps),
    mRelativeTolerance(DefaultRelativeTolerance),
    mAbsoluteTolerance(DefaultAbsoluteTolerance)
{
}

//==============================================================================

CVODESolver::~CVODESolver()
{
    // Delete some internal objects

    N_VDestroy_Serial(mStatesVector);
    CVodeFree(&mSolver);
}

//==============================================================================

void CVODESolver::initialize(const double &pVoiStart, const int &pStatesCount,
                             double *pConstants, double *pRates,
                             double *pStates, double *pAlgebraic,
                             ComputeRatesFunction pComputeRates)
{
    if (!mSolver) {
        // Initialise the ODE solver itself

        OpenCOR::CoreSolver::CoreOdeSolver::initialize(pVoiStart, pStatesCount,
                                                       pConstants, pRates,
                                                       pStates, pAlgebraic,
                                                       pComputeRates);

        // Retrieve some of the CVODE properties

        mMaximumStep = mProperties.contains(MaximumStepProperty)?
                           mProperties.value(MaximumStepProperty).toDouble():
                           DefaultMaximumStep;

        mMaximumNumberOfSteps = mProperties.contains(MaximumNumberOfStepsProperty)?
                                    mProperties.value(MaximumNumberOfStepsProperty).toDouble():
                                    DefaultMaximumNumberOfSteps;

        mRelativeTolerance = mProperties.contains(RelativeToleranceProperty)?
                                 mProperties.value(RelativeToleranceProperty).toDouble():
                                 DefaultRelativeTolerance;
        mAbsoluteTolerance = mProperties.contains(AbsoluteToleranceProperty)?
                                 mProperties.value(AbsoluteToleranceProperty).toDouble():
                                 DefaultAbsoluteTolerance;

        // Create the CVODE solver

        mSolver = CVodeCreate(CV_BDF, CV_NEWTON);

        // Use our own error handler

        CVodeSetErrHandlerFn(mSolver, errorHandler, this);

        // Create the states vector

        mStatesVector = N_VMake_Serial(pStatesCount, pStates);

        // Initialise the CVODE solver

        CVodeInit(mSolver, rhsFunction, pVoiStart, mStatesVector);

        // Set some user data

        mUserData.constants = pConstants;
        mUserData.algebraic = pAlgebraic;

        mUserData.computeRates = pComputeRates;

        CVodeSetUserData(mSolver, &mUserData);

        // Set the linear solver

        CVDense(mSolver, pStatesCount);

        // Set the maximum step

        CVodeSetMaxStep(mSolver, mMaximumStep);

        // Set the maximum number of steps

        CVodeSetMaxNumSteps(mSolver, mMaximumNumberOfSteps);

        // Set the relative and absolute tolerances

        CVodeSStolerances(mSolver, mRelativeTolerance, mAbsoluteTolerance);

        // Keep track of the compute rates function

        mComputeRates = pComputeRates;
    } else {
        // Reinitialise the CVODE object

        CVodeReInit(mSolver, pVoiStart, mStatesVector);
    }
}

//==============================================================================

void CVODESolver::solve(double &pVoi, const double &pVoiEnd) const
{
    Q_ASSERT(mStatesCount > 0);
    Q_ASSERT(mConstants);
    Q_ASSERT(mRates);
    Q_ASSERT(mStates);
    Q_ASSERT(mAlgebraic);

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

bool CVODESolver::isValidProperty(const QString &pName) const
{
    // Check whether the property name is known to us

    return    !pName.compare(MaximumStepProperty)
           || !pName.compare(MaximumNumberOfStepsProperty)
           || !pName.compare(RelativeToleranceProperty)
           || !pName.compare(AbsoluteToleranceProperty);
}

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

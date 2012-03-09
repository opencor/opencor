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
    CVODESolverUserData *userData = reinterpret_cast<CVODESolverUserData *>(pUserData);

    userData->computeRates(pVoi, userData->constants,
                           N_VectorContent_Serial(pRates->content)->data,
                           N_VectorContent_Serial(pStates->content)->data,
                           userData->algebraic);

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
    mStates(0),
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

    N_VDestroy_Serial(mStates);
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

        mStates = N_VMake_Serial(pStatesCount, pStates);

        // Initialise the CVODE solver

        CVodeInit(mSolver, rhsFunction, pVoiStart, mStates);

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
    } else {
        // Reinitialise the CVODE object

        CVodeReInit(mSolver, pVoiStart, mStates);
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

    CVode(mSolver, pVoiEnd, mStates, &pVoi, CV_NORMAL);
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

void CVODESolver::emitError(const QString &pErrorMsg)
{
    // Let people know that an error occured

    emit error(pErrorMsg);
}

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

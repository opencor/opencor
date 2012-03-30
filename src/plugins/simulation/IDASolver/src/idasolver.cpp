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

static const int SizeOfDouble = sizeof(double);

//==============================================================================

// Default IDA parameter values
// Note #1: a maximum step of 0 means that there is no maximum step as such and
//          that IDA can use whatever step it sees fit...
// Note #2: IDA's default maximum number of steps is 500 which ought to be big
//          enough in most cases...

static const double DefaultMaximumStep = 0;
static const int DefaultMaximumNumberOfSteps = 500;
static const double DefaultRelativeTolerance = 1e-7;
static const double DefaultAbsoluteTolerance = 1e-7;

//==============================================================================

int residualFunction(double pVoi, N_Vector pStates, N_Vector pRates,
                     N_Vector pResiduals, void *pUserData)
{
    // Compute the model

    IdaSolverUserData *userData = reinterpret_cast<IdaSolverUserData *>(pUserData);

    double *states    = N_VGetArrayPointer(pStates);
    double *rates     = N_VGetArrayPointer(pRates);
    double *residuals = N_VGetArrayPointer(pResiduals);

    userData->computeEssentialVariables(pVoi, userData->constants, rates,
                                        states, userData->algebraic,
                                        userData->condVar);

    userData->computeResiduals(pVoi, userData->constants, rates, states,
                               userData->algebraic, userData->condVar,
                               residuals);

    // Everything went fine, so...

    return 0;
}

//==============================================================================

int rootFindingFunction(double pVoi, N_Vector pStates, N_Vector pRates,
                        double *pRoots, void *pUserData)
{
    // Do the root finding

    IdaSolverUserData *userData = reinterpret_cast<IdaSolverUserData *>(pUserData);

    userData->computeRootInformation(pVoi, userData->constants,
                                     N_VGetArrayPointer(pRates),
                                     N_VGetArrayPointer(pStates),
                                     userData->algebraic, pRoots);

    // Everything went fine, so...

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char *pModule, const char *pFunction,
                  char *pErrorMsg, void *pUserData)
{
    if (pErrorCode != IDA_WARNING) {
        // IDA really generated an error, so forward it to the IdaSolver object

        QString errorMsg = pErrorMsg;

        reinterpret_cast<IdaSolver *>(pUserData)->emitError(errorMsg.left(1).toLower()+errorMsg.right(errorMsg.size()-1));
    }
}

//==============================================================================

IdaSolver::IdaSolver() :
    mSolver(0),
    mStatesVector(0),
    mRatesVector(0),
    mMaximumStep(DefaultMaximumStep),
    mMaximumNumberOfSteps(DefaultMaximumNumberOfSteps),
    mRelativeTolerance(DefaultRelativeTolerance),
    mAbsoluteTolerance(DefaultAbsoluteTolerance)
{
}

//==============================================================================

IdaSolver::~IdaSolver()
{
    // Delete some internal objects

    N_VDestroy_Serial(mStatesVector);
    N_VDestroy_Serial(mRatesVector);
    IDAFree(&mSolver);
}

//==============================================================================

void IdaSolver::initialize(const double &pVoiStart,
                           const bool &pPositiveDirection,
                           const int &pStatesCount, const int &pCondVarCount,
                           double *pConstants, double *pRates, double *pStates,
                           double *pAlgebraic, double *pCondVar,
                           ComputeEssentialVariablesFunction pComputeEssentialVariables,
                           ComputeResidualsFunction pComputeResiduals,
                           ComputeRootInformationFunction pComputeRootInformation, ComputeStateInformationFunction pComputeStateInformation)
{
    static const double VoiEpsilon = 1e-9;

    if (!mSolver) {
        // Initialise the ODE solver itself

        OpenCOR::CoreSolver::CoreDaeSolver::initialize(pVoiStart,
                                                       pPositiveDirection,
                                                       pStatesCount,
                                                       pCondVarCount,
                                                       pConstants, pRates,
                                                       pStates, pAlgebraic,
                                                       pCondVar,
                                                       pComputeEssentialVariables,
                                                       pComputeResiduals,
                                                       pComputeRootInformation,
                                                       pComputeStateInformation);

        // Retrieve some of the IDA properties

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

        // Create the IDA solver

        mSolver = IDACreate();

        // Use our own error handler

        IDASetErrHandlerFn(mSolver, errorHandler, this);

        // Create the states vector

        mStatesVector = N_VMake_Serial(pStatesCount, pStates);
        mRatesVector  = N_VMake_Serial(pStatesCount, pRates);

        // Initialise the IDA solver

        IDAInit(mSolver, residualFunction, pVoiStart,
                mStatesVector, mRatesVector);

        IDARootInit(mSolver, pCondVarCount, rootFindingFunction);
        //---GRY--- NEED TO CHECK THAT OUR IDA CODE WORKS AS EXPECTED BY TRYING
        //          IT OUT ON A MODEL WHICH NEEDS ROOT FINDING...

        // Set some user data

        mUserData.constants = pConstants;
        mUserData.algebraic = pAlgebraic;
        mUserData.condVar   = pCondVar;

        mUserData.computeResiduals          = pComputeResiduals;
        mUserData.computeEssentialVariables = pComputeEssentialVariables;
        mUserData.computeRootInformation    = pComputeRootInformation;

        IDASetUserData(mSolver, &mUserData);

        // Set the linear solver

        IDADense(mSolver, pStatesCount);

        // Set the maximum step

        IDASetMaxStep(mSolver, mMaximumStep);

        // Set the maximum number of steps

        IDASetMaxNumSteps(mSolver, mMaximumNumberOfSteps);

        // Set the relative and absolute tolerances

        IDASStolerances(mSolver, mRelativeTolerance, mAbsoluteTolerance);

        // Compute the model's initial conditions
        // Note: this requires retrieving the model's state information, setting
        //       the IDA object's id vector and then calling IDACalcIC...

        double *id = new double[pStatesCount];

        pComputeStateInformation(id);

        N_Vector idVector = N_VMake_Serial(pStatesCount, id);

        IDASetId(mSolver, idVector);
        IDACalcIC(mSolver, IDA_YA_YDP_INIT,
                  pVoiStart+pPositiveDirection?VoiEpsilon:-VoiEpsilon);

        N_VDestroy_Serial(idVector);
        delete[] id;

        // Initialise the states and rates with the values determined by IDA

        memcpy(pStates, N_VGetArrayPointer(((IDAMem) mSolver)->ida_ynew), pStatesCount*SizeOfDouble);
        memcpy(pRates, N_VGetArrayPointer(((IDAMem) mSolver)->ida_ypnew), pStatesCount*SizeOfDouble);
    } else {
        // Reinitialise the IDA object

        IDAReInit(mSolver, pVoiStart, mStatesVector, mRatesVector);

        // Compute the model's new initial conditions

        IDACalcIC(mSolver, IDA_YA_YDP_INIT,
                  pVoiStart+pPositiveDirection?VoiEpsilon:-VoiEpsilon);
    }
}

//==============================================================================

void IdaSolver::solve(double &pVoi, const double &pVoiEnd) const
{
    Q_ASSERT(mStatesCount > 0);
    Q_ASSERT(mCondVarCount >= 0);
    Q_ASSERT(mConstants);
    Q_ASSERT(mRates);
    Q_ASSERT(mStates);
    Q_ASSERT(mAlgebraic);
    Q_ASSERT(mCondVar);

    // Solve the model

    IDASolve(mSolver, pVoiEnd, &pVoi, mStatesVector, mRatesVector, IDA_NORMAL);
}

//==============================================================================

bool IdaSolver::isValidProperty(const QString &pName) const
{
    // Check whether the property name is known to us

    return    !pName.compare(MaximumStepProperty)
           || !pName.compare(MaximumNumberOfStepsProperty)
           || !pName.compare(RelativeToleranceProperty)
           || !pName.compare(AbsoluteToleranceProperty);
}

//==============================================================================

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

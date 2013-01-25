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

int residualFunction(double pVoi, N_Vector pStates, N_Vector pRates,
                     N_Vector pResiduals, void *pUserData)
{
    // Compute the residual function

    IdaSolverUserData *userData = reinterpret_cast<IdaSolverUserData *>(pUserData);

    double *states    = N_VGetArrayPointer(pStates);
    double *rates     = N_VGetArrayPointer(pRates);
    double *residuals = N_VGetArrayPointer(pResiduals);

    userData->computeEssentialVariables()(pVoi, userData->constants(), rates,
                                          states, userData->algebraic(),
                                          userData->condVar());

    userData->computeResiduals()(pVoi, userData->constants(), rates, states,
                                 userData->algebraic(), userData->condVar(),
                                 residuals);

    // Everything went fine, so...

    return 0;
}

//==============================================================================

int rootFindingFunction(double pVoi, N_Vector pStates, N_Vector pRates,
                        double *pRoots, void *pUserData)
{
    // Compute the root finding function

    IdaSolverUserData *userData = reinterpret_cast<IdaSolverUserData *>(pUserData);

    userData->computeRootInformation()(pVoi, userData->constants(),
                                       N_VGetArrayPointer(pRates),
                                       N_VGetArrayPointer(pStates),
                                       userData->algebraic(), pRoots);

    // Everything went fine, so...

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char *pModule, const char *pFunction,
                  char *pErrorMsg, void *pUserData)
{
    Q_UNUSED(pModule);
    Q_UNUSED(pFunction);

    if (pErrorCode != IDA_WARNING)
        // IDA generated an error, so forward it to the IdaSolver object

        reinterpret_cast<IdaSolver *>(pUserData)->emitError(pErrorMsg);
}

//==============================================================================

IdaSolverUserData::IdaSolverUserData(double *pConstants, double *pAlgebraic, double *pCondVar,
                                     OpenCOR::CoreSolver::CoreDaeSolver::ComputeEssentialVariablesFunction pComputeEssentialVariables,
                                     OpenCOR::CoreSolver::CoreDaeSolver::ComputeResidualsFunction pComputeResiduals,
                                     OpenCOR::CoreSolver::CoreDaeSolver::ComputeRootInformationFunction pComputeRootInformation) :
    mConstants(pConstants),
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

OpenCOR::CoreSolver::CoreDaeSolver::ComputeEssentialVariablesFunction IdaSolverUserData::computeEssentialVariables() const
{
    // Return our compute essential variables function

    return mComputeEssentialVariables;
}

//==============================================================================

OpenCOR::CoreSolver::CoreDaeSolver::ComputeResidualsFunction IdaSolverUserData::computeResiduals() const
{
    // Return our compute residuals function

    return mComputeResiduals;
}

//==============================================================================

OpenCOR::CoreSolver::CoreDaeSolver::ComputeRootInformationFunction IdaSolverUserData::computeRootInformation() const
{
    // Return our compute root information function

    return mComputeRootInformation;
}

//==============================================================================

IdaSolver::IdaSolver() :
    mSolver(0),
    mStatesVector(0),
    mRatesVector(0),
    mUserData(0),
    mMaximumStep(DefaultMaximumStep),
    mMaximumNumberOfSteps(DefaultMaximumNumberOfSteps),
    mRelativeTolerance(DefaultRelativeTolerance),
    mAbsoluteTolerance(DefaultAbsoluteTolerance)
{
}

//==============================================================================

IdaSolver::~IdaSolver()
{
    if (!mSolver)
        // The solver hasn't been initialised, so...

        return;

    // Delete some internal objects

    N_VDestroy_Serial(mStatesVector);
    N_VDestroy_Serial(mRatesVector);

    IDAFree(&mSolver);

    delete mUserData;
}

//==============================================================================

void IdaSolver::initialize(const double &pVoiStart, const double &pVoiEnd,
                           const int &pStatesCount, const int &pCondVarCount,
                           double *pConstants, double *pRates, double *pStates,
                           double *pAlgebraic, double *pCondVar,
                           ComputeEssentialVariablesFunction pComputeEssentialVariables,
                           ComputeResidualsFunction pComputeResiduals,
                           ComputeRootInformationFunction pComputeRootInformation,
                           ComputeStateInformationFunction pComputeStateInformation)
{
    static const double VoiEpsilon = 1.0e-9;

    if (!mSolver) {
        // Initialise the ODE solver itself

        OpenCOR::CoreSolver::CoreDaeSolver::initialize(pVoiStart, pVoiEnd,
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

        if (mProperties.contains(MaximumStepProperty))
            mMaximumStep = mProperties.value(MaximumStepProperty).toDouble();
        else
            emit error(QObject::tr("the 'maximum step' property value could not be retrieved"));

        if (mProperties.contains(MaximumNumberOfStepsProperty))
            mMaximumNumberOfSteps = mProperties.value(MaximumNumberOfStepsProperty).toDouble();
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
        mRatesVector  = N_VMake_Serial(pStatesCount, pRates);

        // Create the IDA solver

        mSolver = IDACreate();

        // Use our own error handler

        IDASetErrHandlerFn(mSolver, errorHandler, this);

        // Initialise the IDA solver

        IDAInit(mSolver, residualFunction, pVoiStart,
                mStatesVector, mRatesVector);

        IDARootInit(mSolver, pCondVarCount, rootFindingFunction);
        //---GRY--- NEED TO CHECK THAT OUR IDA CODE WORKS AS EXPECTED BY TRYING
        //          IT OUT ON A MODEL WHICH NEEDS ROOT FINDING (E.G. THE
        //          SAUCERMAN MODEL)...

        // Set some user data

        delete mUserData;   // Just in case the solver got initialised before

        mUserData = new IdaSolverUserData(pConstants, pAlgebraic, pCondVar,
                                          pComputeEssentialVariables,
                                          pComputeResiduals,
                                          pComputeRootInformation);

        IDASetUserData(mSolver, mUserData);

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
        //       the IDA object's id vector and then calling IDACalcIC()...

        double *id = new double[pStatesCount];

        pComputeStateInformation(id);

        N_Vector idVector = N_VMake_Serial(pStatesCount, id);

        IDASetId(mSolver, idVector);
        IDACalcIC(mSolver, IDA_YA_YDP_INIT,
                  pVoiStart+((pVoiEnd-pVoiStart > 0)?VoiEpsilon:-VoiEpsilon));

        N_VDestroy_Serial(idVector);

        delete[] id;
    } else {
        // Reinitialise the IDA object

        IDAReInit(mSolver, pVoiStart, mStatesVector, mRatesVector);

        // Compute the model's new initial conditions

        IDACalcIC(mSolver, IDA_YA_YDP_INIT,
                  pVoiStart+((pVoiEnd-pVoiStart > 0)?VoiEpsilon:-VoiEpsilon));
    }
}

//==============================================================================

void IdaSolver::solve(double &pVoi, const double &pVoiEnd) const
{
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

//==============================================================================
// KINSOL solver class
//==============================================================================

#include "kinsolsolver.h"

//==============================================================================

#include "kinsol/kinsol.h"
#include "kinsol/kinsol_dense.h"

//==============================================================================

namespace OpenCOR {
namespace KINSOLSolver {

//==============================================================================

int systemFunction(N_Vector pY, N_Vector pF, void *pUserData)
{
    // Compute the system function

    struct KinsolSolverUserData *userData = reinterpret_cast<struct KinsolSolverUserData *>(pUserData);

    userData->computeSystem(N_VGetArrayPointer_Serial(pY),
                            N_VGetArrayPointer_Serial(pF),
                            userData->userData);

    // Everything went fine, so...

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char *pModule, const char *pFunction,
                  char *pErrorMsg, void *pUserData)
{
    Q_UNUSED(pModule);
    Q_UNUSED(pFunction);

    if (pErrorCode != KIN_WARNING)
        // KINSOL generated an error, so forward it to the KinsolSolver object

        reinterpret_cast<KinsolSolver *>(pUserData)->emitError(pErrorMsg);
}

//==============================================================================

KinsolSolver::KinsolSolver() :
    mSolver(0),
    mParametersVector(0),
    mOnesVector(0)
{
}

//==============================================================================

KinsolSolver::~KinsolSolver()
{
    // Delete some internal objects

    reset();
}

//==============================================================================

void KinsolSolver::reset()
{
    if (!mSolver)
        // The solver hasn't been initialised, so...

        return;

    N_VDestroy_Serial(mParametersVector);
    N_VDestroy_Serial(mOnesVector);

    KINFree(&mSolver);
}

//==============================================================================

void KinsolSolver::initialize(ComputeSystemFunction pComputeSystem,
                              double *pParameters, int pSize, void *pUserData)
{
    if (mSolver)
        // The solver has already been initialised, so reset things...

        reset();

    // Initialise the ODE solver itself

    OpenCOR::CoreSolver::CoreNlaSolver::initialize(pComputeSystem, pParameters,
                                                   pSize, pUserData);

    // Create some vectors

    mParametersVector = N_VMake_Serial(pSize, pParameters);
    mOnesVector = N_VNew_Serial(pSize);

    N_VConst(1.0, mOnesVector);

    // Create the KINSOL solver

    mSolver = KINCreate();

    // Use our own error handler

    KINSetErrHandlerFn(mSolver, errorHandler, this);

    // Initialise the KINSOL solver

    KINInit(mSolver, systemFunction, mParametersVector);

    // Set some user data

    mKinsolUserData.computeSystem = pComputeSystem;
    mKinsolUserData.userData = pUserData;

    KINSetUserData(mSolver, &mKinsolUserData);

    // Set the linear solver

    KINDense(mSolver, pSize);
}

//==============================================================================

void KinsolSolver::solve() const
{
    // Solve the linear system

    KINSol(mSolver, mParametersVector, KIN_LINESEARCH,
           mOnesVector, mOnesVector);
}

//==============================================================================

}   // namespace KINSOLSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

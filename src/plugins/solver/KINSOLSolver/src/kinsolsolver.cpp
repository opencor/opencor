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
// KINSOL solver
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

    KinsolSolverUserData *userData = static_cast<KinsolSolverUserData *>(pUserData);

    userData->computeSystem()(N_VGetArrayPointer_Serial(pY),
                              N_VGetArrayPointer_Serial(pF),
                              userData->userData());

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char *pModule, const char *pFunction,
                  char *pErrorMessage, void *pUserData)
{
    Q_UNUSED(pModule);
    Q_UNUSED(pFunction);

    if (pErrorCode != KIN_WARNING) {
        // KINSOL generated an error, so forward it to the KinsolSolver object

        static_cast<KinsolSolver *>(pUserData)->emitError(pErrorMessage);
    }
}

//==============================================================================

KinsolSolverUserData::KinsolSolverUserData(void *pUserData,
                                           Solver::NlaSolver::ComputeSystemFunction pComputeSystem) :
    mUserData(pUserData),
    mComputeSystem(pComputeSystem)
{
}

//==============================================================================

void * KinsolSolverUserData::userData() const
{
    // Return our user data

    return mUserData;
}

//==============================================================================

Solver::NlaSolver::ComputeSystemFunction KinsolSolverUserData::computeSystem() const
{
    // Return our compute system function

    return mComputeSystem;
}

//==============================================================================

KinsolSolver::KinsolSolver() :
    mSolver(0),
    mParametersVector(0),
    mOnesVector(0),
    mUserData(0)
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
    // Make sure that the solver has been initialised

    if (!mSolver)
        return;

    N_VDestroy_Serial(mParametersVector);
    N_VDestroy_Serial(mOnesVector);

    KINFree(&mSolver);

    delete mUserData;
}

//==============================================================================

void KinsolSolver::initialize(ComputeSystemFunction pComputeSystem,
                              double *pParameters, int pSize, void *pUserData)
{
    // Reset things, if the solver has already been initialised

    if (mSolver)
        reset();

    // Initialise the ODE solver itself

    OpenCOR::Solver::NlaSolver::initialize(pComputeSystem, pParameters, pSize);

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

    mUserData = new KinsolSolverUserData(pUserData, pComputeSystem);

    KINSetUserData(mSolver, mUserData);

    // Set the linear solver

    KINDense(mSolver, pSize);
}

//==============================================================================

void KinsolSolver::solve() const
{
    // Solve the linear system

    KINSol(mSolver, mParametersVector, KIN_LINESEARCH, mOnesVector, mOnesVector);
}

//==============================================================================

}   // namespace KINSOLSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

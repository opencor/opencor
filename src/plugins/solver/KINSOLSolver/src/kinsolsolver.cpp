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
#include "kinsol/kinsol_direct.h"
#include "kinsol/kinsol_spils.h"
#include "sunlinsol/sunlinsol_band.h"
#include "sunlinsol/sunlinsol_dense.h"
#include "sunlinsol/sunlinsol_spbcgs.h"
#include "sunlinsol/sunlinsol_spgmr.h"
#include "sunlinsol/sunlinsol_sptfqmr.h"

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

KinsolSolverUserData::KinsolSolverUserData(Solver::NlaSolver::ComputeSystemFunction pComputeSystem,
                                           void *pUserData) :
    mComputeSystem(pComputeSystem),
    mUserData(pUserData)
{
}

//==============================================================================

Solver::NlaSolver::ComputeSystemFunction KinsolSolverUserData::computeSystem() const
{
    // Return our compute system function

    return mComputeSystem;
}

//==============================================================================

void * KinsolSolverUserData::userData() const
{
    // Return our user data

    return mUserData;
}

//==============================================================================

KinsolSolver::KinsolSolver() :
    mSolver(0),
    mParametersVector(0),
    mOnesVector(0),
    mMatrix(0),
    mLinearSolver(0),
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
    SUNLinSolFree(mLinearSolver);
    SUNMatDestroy(mMatrix);

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

    // Retrieve some of the KINSOL properties

    int maximumNumberOfIterations = MaximumNumberOfIterationsDefaultValue;
    QString linearSolver = LinearSolverDefaultValue;
    int upperHalfBandwidth = UpperHalfBandwidthDefaultValue;
    int lowerHalfBandwidth = LowerHalfBandwidthDefaultValue;

    if (mProperties.contains(MaximumNumberOfIterationsId)) {
        maximumNumberOfIterations = mProperties.value(MaximumNumberOfIterationsId).toInt();
    } else {
        emit error(tr("the \"Maximum number of iterations\" property value could not be retrieved"));

        return;
    }

    if (mProperties.contains(LinearSolverId)) {
        linearSolver = mProperties.value(LinearSolverId).toString();

        if (!linearSolver.compare(BandedLinearSolver)) {
            if (mProperties.contains(UpperHalfBandwidthId)) {
                upperHalfBandwidth = mProperties.value(UpperHalfBandwidthId).toInt();

                if (   (upperHalfBandwidth < 0)
                    || (upperHalfBandwidth >= pSize)) {
                    emit error(tr("the \"Upper half-bandwidth\" property must have a value between 0 and %1").arg(pSize-1));

                    return;
                }
            } else {
                emit error(tr("the \"Upper half-bandwidth\" property value could not be retrieved"));

                return;
            }

            if (mProperties.contains(LowerHalfBandwidthId)) {
                lowerHalfBandwidth = mProperties.value(LowerHalfBandwidthId).toInt();

                if (   (lowerHalfBandwidth < 0)
                    || (lowerHalfBandwidth >= pSize)) {
                    emit error(tr("the \"Lower half-bandwidth\" property must have a value between 0 and %1").arg(pSize-1));

                    return;
                }
            } else {
                emit error(tr("the \"Lower half-bandwidth\" property value could not be retrieved"));

                return;
            }
        }
    } else {
        emit error(tr("the \"Linear solver\" property value could not be retrieved"));

        return;
    }

    // Initialise the NLA solver itself

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

    mUserData = new KinsolSolverUserData(pComputeSystem, pUserData);

    KINSetUserData(mSolver, mUserData);

    // Set the linear solver

    if (!linearSolver.compare(DenseLinearSolver)) {
        mMatrix = SUNDenseMatrix(pSize, pSize);
        mLinearSolver = SUNDenseLinearSolver(mParametersVector, mMatrix);

        KINDlsSetLinearSolver(mSolver, mLinearSolver, mMatrix);
    } else if (!linearSolver.compare(BandedLinearSolver)) {
        mMatrix = SUNBandMatrix(pSize,
                                upperHalfBandwidth, lowerHalfBandwidth,
                                upperHalfBandwidth+lowerHalfBandwidth);
        mLinearSolver = SUNBandLinearSolver(mParametersVector, mMatrix);

        KINDlsSetLinearSolver(mSolver, mLinearSolver, mMatrix);
    } else if (!linearSolver.compare(GmresLinearSolver)) {
        mLinearSolver = SUNSPGMR(mParametersVector, PREC_BOTH, 0);

        KINSpilsSetLinearSolver(mSolver, mLinearSolver);
    } else if (!linearSolver.compare(BiCgStabLinearSolver)) {
        mLinearSolver = SUNSPBCGS(mParametersVector, PREC_BOTH, 0);

        KINSpilsSetLinearSolver(mSolver, mLinearSolver);
    } else {
        mLinearSolver = SUNSPTFQMR(mParametersVector, PREC_BOTH, 0);

        KINSpilsSetLinearSolver(mSolver, mLinearSolver);
    }

    // Set the maximum number of iterations

    KINSetNumMaxIters(mSolver, maximumNumberOfIterations);
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

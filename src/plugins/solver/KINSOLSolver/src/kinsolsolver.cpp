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

    // Forward errors to our KinsolSolver object

    if (pErrorCode != KIN_WARNING)
        static_cast<KinsolSolver *>(pUserData)->emitError(pErrorMessage);
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

KinsolSolverData::KinsolSolverData(void *pSolver, N_Vector pParametersVector,
                                   N_Vector pOnesVector, SUNMatrix pMatrix,
                                   SUNLinearSolver pLinearSolver,
                                   KinsolSolverUserData *pUserData) :
    mSolver(pSolver),
    mParametersVector(pParametersVector),
    mOnesVector(pOnesVector),
    mMatrix(pMatrix),
    mLinearSolver(pLinearSolver),
    mUserData(pUserData)
{
}

//==============================================================================

KinsolSolverData::~KinsolSolverData()
{
    // Delete some internal objects

    N_VDestroy_Serial(mParametersVector);
    N_VDestroy_Serial(mOnesVector);
    SUNLinSolFree(mLinearSolver);
    SUNMatDestroy(mMatrix);

    KINFree(&mSolver);

    delete mUserData;
}

//==============================================================================

void * KinsolSolverData::solver() const
{
    // Return our solver

    return mSolver;
}

//==============================================================================

N_Vector KinsolSolverData::parametersVector() const
{
    // Return our parameters vector

    return mParametersVector;
}

//==============================================================================

N_Vector KinsolSolverData::onesVector() const
{
    // Return our ones vector

    return mOnesVector;
}

//==============================================================================

KinsolSolver::KinsolSolver() :
    mData(QMap<ComputeSystemFunction, KinsolSolverData *>())
{
}

//==============================================================================

KinsolSolver::~KinsolSolver()
{
    // Delete some internal objects

    foreach (KinsolSolverData *data, mData.values())
        delete data;
}

//==============================================================================

void KinsolSolver::solve(ComputeSystemFunction pComputeSystem,
                         double *pParameters, const int &pSize, void *pUserData)
{
    // Check whether we need to initialise or update ourselves

    KinsolSolverData *data = mData.value(pComputeSystem);

    if (!data) {
        // Retrieve our properties

        int maximumNumberOfIterationsValue = MaximumNumberOfIterationsDefaultValue;
        QString linearSolverValue = LinearSolverDefaultValue;
        int upperHalfBandwidthValue = UpperHalfBandwidthDefaultValue;
        int lowerHalfBandwidthValue = LowerHalfBandwidthDefaultValue;

        if (mProperties.contains(MaximumNumberOfIterationsId)) {
            maximumNumberOfIterationsValue = mProperties.value(MaximumNumberOfIterationsId).toInt();
        } else {
            emit error(tr("the \"Maximum number of iterations\" property value could not be retrieved"));

            return;
        }

        if (mProperties.contains(LinearSolverId)) {
            linearSolverValue = mProperties.value(LinearSolverId).toString();

            if (!linearSolverValue.compare(BandedLinearSolver)) {
                if (mProperties.contains(UpperHalfBandwidthId)) {
                    upperHalfBandwidthValue = mProperties.value(UpperHalfBandwidthId).toInt();

                    if (   (upperHalfBandwidthValue < 0)
                        || (upperHalfBandwidthValue >= pSize)) {
                        emit error(tr("the \"Upper half-bandwidth\" property must have a value between 0 and %1").arg(pSize-1));

                        return;
                    }
                } else {
                    emit error(tr("the \"Upper half-bandwidth\" property value could not be retrieved"));

                    return;
                }

                if (mProperties.contains(LowerHalfBandwidthId)) {
                    lowerHalfBandwidthValue = mProperties.value(LowerHalfBandwidthId).toInt();

                    if (   (lowerHalfBandwidthValue < 0)
                        || (lowerHalfBandwidthValue >= pSize)) {
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

        // Create some vectors

        N_Vector parametersVector = N_VMake_Serial(pSize, pParameters);
        N_Vector onesVector = N_VNew_Serial(pSize);

        N_VConst(1.0, onesVector);

        // Create our KINSOL solver

        void *solver = KINCreate();

        // Use our own error handler

        KINSetErrHandlerFn(solver, errorHandler, this);

        // Initialise our KINSOL solver

        KINInit(solver, systemFunction, parametersVector);

        // Set our user data

        KinsolSolverUserData *userData = new KinsolSolverUserData(pComputeSystem, pUserData);

        KINSetUserData(solver, userData);

        // Set our maximum number of iterations

        KINSetNumMaxIters(solver, maximumNumberOfIterationsValue);

        // Set our linear solver

        SUNMatrix matrix = SUNMatrix();
        SUNLinearSolver linearSolver;

        if (!linearSolverValue.compare(DenseLinearSolver)) {
            matrix = SUNDenseMatrix(pSize, pSize);
            linearSolver = SUNDenseLinearSolver(parametersVector, matrix);

            KINDlsSetLinearSolver(solver, linearSolver, matrix);
        } else if (!linearSolverValue.compare(BandedLinearSolver)) {
            matrix = SUNBandMatrix(pSize,
                                   upperHalfBandwidthValue, lowerHalfBandwidthValue,
                                   upperHalfBandwidthValue+lowerHalfBandwidthValue);
            linearSolver = SUNBandLinearSolver(parametersVector, matrix);

            KINDlsSetLinearSolver(solver, linearSolver, matrix);
        } else if (!linearSolverValue.compare(GmresLinearSolver)) {
            linearSolver = SUNSPGMR(parametersVector, PREC_NONE, 0);

            KINSpilsSetLinearSolver(solver, linearSolver);
        } else if (!linearSolverValue.compare(BiCgStabLinearSolver)) {
            linearSolver = SUNSPBCGS(parametersVector, PREC_NONE, 0);

            KINSpilsSetLinearSolver(solver, linearSolver);
        } else {
            linearSolver = SUNSPTFQMR(parametersVector, PREC_NONE, 0);

            KINSpilsSetLinearSolver(solver, linearSolver);
        }

        // Keep track of our data

        data = new KinsolSolverData(solver, parametersVector, onesVector,
                                    matrix, linearSolver, userData);

        mData.insert(pComputeSystem, data);
    }

    // Solve our linear system

    KINSol(data->solver(), data->parametersVector(), KIN_LINESEARCH,
           data->onesVector(), data->onesVector());
}

//==============================================================================

}   // namespace KINSOLSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

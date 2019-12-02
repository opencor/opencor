/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// KINSOL solver
//==============================================================================

#include "kinsolsolver.h"

//==============================================================================

#include "sundialsbegin.h"
    #include "kinsol/kinsol.h"
    #include "kinsol/kinsol_direct.h"
    #include "kinsol/kinsol_spils.h"
    #include "sunlinsol/sunlinsol_band.h"
    #include "sunlinsol/sunlinsol_dense.h"
    #include "sunlinsol/sunlinsol_spbcgs.h"
    #include "sunlinsol/sunlinsol_spgmr.h"
    #include "sunlinsol/sunlinsol_sptfqmr.h"
#include "sundialsend.h"

//==============================================================================

namespace OpenCOR {
namespace KINSOLSolver {

//==============================================================================

int systemFunction(N_Vector pY, N_Vector pF, void *pUserData)
{
    // Compute the system function

    auto userData = static_cast<KinsolSolverUserData *>(pUserData);
    double *f = N_VGetArrayPointer_Serial(pF);

    userData->computeSystem()(N_VGetArrayPointer_Serial(pY), f, userData->userData());

    // Make sure that our solution is finite
    // Note: this is to prevent KINSOL from looping indefinitely when an ODE
    //       solver is badly set up (e.g. Forward Euler with an integration step
    //       that is too big)...

    for (qint64 i = 0, iMax = static_cast<N_VectorContent_Serial>(pF->content)->length; i < iMax; ++i) {
        if (!qIsFinite(f[i])) {
            return 1;
        }
    }

    return 0;
}

//==============================================================================

void errorHandler(int pErrorCode, const char *pModule, const char *pFunction,
                  char *pErrorMessage, void *pUserData)
{
    Q_UNUSED(pModule)
    Q_UNUSED(pFunction)

    // Forward errors to our KinsolSolver object

    if (pErrorCode != KIN_WARNING) {
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

KinsolSolverUserData * KinsolSolverData::userData() const
{
    // Return our user data

    return mUserData;
}

//==============================================================================

void KinsolSolverData::setUserData(KinsolSolverUserData *pUserData)
{
    // Set our user data, after having delete our 'old' ones

    delete mUserData;

    mUserData = pUserData;
}

//==============================================================================

KinsolSolver::~KinsolSolver()
{
    // Delete some internal objects

    for (auto data : mData) {
        delete data;
    }
}

//==============================================================================

void KinsolSolver::solve(ComputeSystemFunction pComputeSystem,
                         double *pParameters, int pSize, void *pUserData)
{
    // Check whether we need to initialise or update ourselves

    KinsolSolverData *data = mData.value(reinterpret_cast<void *>(pComputeSystem));

    if (data == nullptr) {
        // Retrieve our properties

        int maximumNumberOfIterationsValue = MaximumNumberOfIterationsDefaultValue;
        QString linearSolverValue = LinearSolverDefaultValue;
        int upperHalfBandwidthValue = UpperHalfBandwidthDefaultValue;
        int lowerHalfBandwidthValue = LowerHalfBandwidthDefaultValue;

        if (mProperties.contains(MaximumNumberOfIterationsId)) {
            maximumNumberOfIterationsValue = mProperties.value(MaximumNumberOfIterationsId).toInt();
        } else {
            emit error(tr(R"(the "Maximum number of iterations" property value could not be retrieved)"));

            return;
        }

        if (mProperties.contains(LinearSolverId)) {
            linearSolverValue = mProperties.value(LinearSolverId).toString();

            if (linearSolverValue == BandedLinearSolver) {
                if (mProperties.contains(UpperHalfBandwidthId)) {
                    upperHalfBandwidthValue = mProperties.value(UpperHalfBandwidthId).toInt();

                    if (upperHalfBandwidthValue >= pSize) {
                        emit error(tr(R"(the "Upper half-bandwidth" property must have a value between 0 and %1)").arg(pSize-1));

                        return;
                    }
                } else {
                    emit error(tr(R"(the "Upper half-bandwidth" property value could not be retrieved)"));

                    return;
                }

                if (mProperties.contains(LowerHalfBandwidthId)) {
                    lowerHalfBandwidthValue = mProperties.value(LowerHalfBandwidthId).toInt();

                    if (lowerHalfBandwidthValue >= pSize) {
                        emit error(tr(R"(the "Lower half-bandwidth" property must have a value between 0 and %1)").arg(pSize-1));

                        return;
                    }
                } else {
                    emit error(tr(R"(the "Lower half-bandwidth" property value could not be retrieved)"));

                    return;
                }
            }
        } else {
            emit error(tr(R"(the "Linear solver" property value could not be retrieved)"));

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

        auto userData = new KinsolSolverUserData(pComputeSystem, pUserData);

        KINSetUserData(solver, userData);

        // Set our maximum number of iterations

        KINSetNumMaxIters(solver, maximumNumberOfIterationsValue);

        // Set our linear solver

        SUNMatrix matrix = nullptr;
        SUNLinearSolver linearSolver;

        if (linearSolverValue == DenseLinearSolver) {
            matrix = SUNDenseMatrix(pSize, pSize);
            linearSolver = SUNDenseLinearSolver(parametersVector, matrix);

            KINSetLinearSolver(solver, linearSolver, matrix);
        } else if (linearSolverValue == BandedLinearSolver) {
            matrix = SUNBandMatrix(pSize, upperHalfBandwidthValue,
                                          lowerHalfBandwidthValue);
            linearSolver = SUNBandLinearSolver(parametersVector, matrix);

            KINSetLinearSolver(solver, linearSolver, matrix);
        } else if (linearSolverValue == GmresLinearSolver) {
            linearSolver = SUNSPGMR(parametersVector, PREC_NONE, 0);

            KINSetLinearSolver(solver, linearSolver, matrix);
        } else if (linearSolverValue == BiCgStabLinearSolver) {
            linearSolver = SUNSPBCGS(parametersVector, PREC_NONE, 0);

            KINSetLinearSolver(solver, linearSolver, matrix);
        } else {
            linearSolver = SUNSPTFQMR(parametersVector, PREC_NONE, 0);

            KINSetLinearSolver(solver, linearSolver, matrix);
        }

        // Keep track of our data

        data = new KinsolSolverData(solver, parametersVector, onesVector,
                                    matrix, linearSolver, userData);

        mData.insert(reinterpret_cast<void *>(pComputeSystem), data);
    } else {
        // We are already initiliased, so simply update our user data

        data->setUserData(new KinsolSolverUserData(pComputeSystem, pUserData));

        KINSetUserData(data->solver(), data->userData());
    }

    // Solve our linear system

    KINSol(data->solver(), data->parametersVector(), KIN_LINESEARCH,
           data->onesVector(), data->onesVector());
}

//==============================================================================

} // namespace KINSOLSolver
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

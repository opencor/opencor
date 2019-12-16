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

#pragma once

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include "sundialsbegin.h"
    #include "nvector/nvector_serial.h"
    #include "sundials/sundials_linearsolver.h"
    #include "sundials/sundials_matrix.h"
#include "sundialsend.h"

//==============================================================================

namespace OpenCOR {
namespace KINSOLSolver {

//==============================================================================

static const auto MaximumNumberOfIterationsId = QStringLiteral("MaximumNumberOfIterations");
static const auto LinearSolverId              = QStringLiteral("LinearSolver");
static const auto UpperHalfBandwidthId        = QStringLiteral("UpperHalfBandwidth");
static const auto LowerHalfBandwidthId        = QStringLiteral("LowerHalfBandwidth");

//==============================================================================

static const auto DenseLinearSolver    = QStringLiteral("Dense");
static const auto BandedLinearSolver   = QStringLiteral("Banded");
static const auto GmresLinearSolver    = QStringLiteral("GMRES");
static const auto BiCgStabLinearSolver = QStringLiteral("BiCGStab");
static const auto TfqmrLinearSolver    = QStringLiteral("TFQMR");

//==============================================================================

// Default KINSOL parameter values
// Note: KINSOL's default maximum number of iterations is 200, which ought to be
//       big enough in most cases...

enum {
    MaximumNumberOfIterationsDefaultValue = 200
};

static const auto LinearSolverDefaultValue = DenseLinearSolver;

enum {
    UpperHalfBandwidthDefaultValue = 0,
    LowerHalfBandwidthDefaultValue = 0
};

//==============================================================================

class KinsolSolverUserData
{
public:
    explicit KinsolSolverUserData(Solver::NlaSolver::ComputeSystemFunction pComputeSystem,
                                  void *pUserData);

    Solver::NlaSolver::ComputeSystemFunction computeSystem() const;

    void * userData() const;

private:
    Solver::NlaSolver::ComputeSystemFunction mComputeSystem;

    void *mUserData;
};

//==============================================================================

class KinsolSolverData
{
public:
    explicit KinsolSolverData(void *pSolver, N_Vector pParametersVector,
                              N_Vector pOnesVector, SUNMatrix pMatrix,
                              SUNLinearSolver pLinearSolver,
                              KinsolSolverUserData *pUserData);
    ~KinsolSolverData();

    void * solver() const;

    N_Vector parametersVector() const;
    N_Vector onesVector() const;

    KinsolSolverUserData * userData() const;
    void setUserData(KinsolSolverUserData *pUserData);

private:
    void *mSolver;

    N_Vector mParametersVector;
    N_Vector mOnesVector;

    SUNMatrix mMatrix;
    SUNLinearSolver mLinearSolver;

    KinsolSolverUserData *mUserData;
};

//==============================================================================

class KinsolSolver : public OpenCOR::Solver::NlaSolver
{
    Q_OBJECT

public:
    ~KinsolSolver() override;

    void solve(ComputeSystemFunction pComputeSystem, double *pParameters,
               int pSize, void *pUserData) override;

private:
    QMap<void *, KinsolSolverData *> mData;
};

//==============================================================================

} // namespace KINSOLSolver
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

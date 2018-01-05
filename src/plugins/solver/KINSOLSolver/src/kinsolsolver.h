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

#pragma once

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include "nvector/nvector_serial.h"
#include "sundials/sundials_linearsolver.h"
#include "sundials/sundials_matrix.h"

//==============================================================================

namespace OpenCOR {
namespace KINSOLSolver {

//==============================================================================

static const auto MaximumNumberOfIterationsId = QStringLiteral("MaximumNumberOfIterations");

//==============================================================================

// Default KINSOL parameter values
// Note: KINSOL's default maximum number of iterations is 200, which ought to be
//       big enough in most cases...

enum {
    MaximumNumberOfIterationsDefaultValue = 200
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

class KinsolSolver : public Solver::NlaSolver
{
public:
    explicit KinsolSolver();
    ~KinsolSolver();

    virtual void initialize(ComputeSystemFunction pComputeSystem,
                            double *pParameters, int pSize, void *pUserData);

    virtual void solve() const;

private:
    void *mSolver;
    N_Vector mParametersVector;
    N_Vector mOnesVector;
    SUNMatrix mMatrix;
    SUNLinearSolver mLinearSolver;
    KinsolSolverUserData *mUserData;

    void reset();
};

//==============================================================================

}   // namespace KINSOLSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

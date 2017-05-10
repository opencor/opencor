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
// IDA solver
//==============================================================================

#pragma once

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include "nvector/nvector_serial.h"

//==============================================================================

namespace OpenCOR {
namespace IDASolver {

//==============================================================================

static const auto MaximumStepId          = QStringLiteral("MaximumStep");
static const auto MaximumNumberOfStepsId = QStringLiteral("MaximumNumberOfSteps");
static const auto LinearSolverId         = QStringLiteral("LinearSolver");
static const auto UpperHalfBandwidthId   = QStringLiteral("UpperHalfBandwidth");
static const auto LowerHalfBandwidthId   = QStringLiteral("LowerHalfBandwidth");
static const auto RelativeToleranceId    = QStringLiteral("RelativeTolerance");
static const auto AbsoluteToleranceId    = QStringLiteral("AbsoluteTolerance");
static const auto InterpolateSolutionId  = QStringLiteral("InterpolateSolution");

//==============================================================================

static const auto DenseLinearSolver    = QStringLiteral("Dense");
static const auto BandedLinearSolver   = QStringLiteral("Banded");
static const auto GmresLinearSolver    = QStringLiteral("GMRES");
static const auto BiCgStabLinearSolver = QStringLiteral("BiCGStab");
static const auto TfqmrLinearSolver    = QStringLiteral("TFQMR");

//==============================================================================

// Default CVODE parameter values
// Note #1: a maximum step of 0 means that there is no maximum step as such and
//          that IDA can use whatever step it sees fit...
// Note #2: IDA's default maximum number of steps is 500 which ought to be big
//          enough in most cases...

static const double MaximumStepDefaultValue = 0.0;

enum {
    MaximumNumberOfStepsDefaultValue = 500
};

static const auto LinearSolverDefaultValue = DenseLinearSolver;

enum {
    UpperHalfBandwidthDefaultValue = 0,
    LowerHalfBandwidthDefaultValue = 0
};

static const double RelativeToleranceDefaultValue = 1.0e-7;
static const double AbsoluteToleranceDefaultValue = 1.0e-7;

static const bool InterpolateSolutionDefaultValue = true;

//==============================================================================

class IdaSolverUserData
{
public:
    explicit IdaSolverUserData(double *pConstants, double *pOldRates,
                               double *pOldStates, double *pAlgebraic,
                               double *pCondVar,
                               Solver::DaeSolver::ComputeEssentialVariablesFunction pComputeEssentialVariables,
                               Solver::DaeSolver::ComputeResidualsFunction pComputeResiduals,
                               Solver::DaeSolver::ComputeRootInformationFunction pComputeRootInformation);

    double * constants() const;
    double * oldRates() const;
    double * oldStates() const;
    double * algebraic() const;
    double * condVar() const;

    Solver::DaeSolver::ComputeEssentialVariablesFunction computeEssentialVariables() const;
    Solver::DaeSolver::ComputeResidualsFunction computeResiduals() const;
    Solver::DaeSolver::ComputeRootInformationFunction computeRootInformation() const;

private:
    double *mConstants;
    double *mOldRates;
    double *mOldStates;
    double *mAlgebraic;
    double *mCondVar;

    Solver::DaeSolver::ComputeEssentialVariablesFunction mComputeEssentialVariables;
    Solver::DaeSolver::ComputeResidualsFunction mComputeResiduals;
    Solver::DaeSolver::ComputeRootInformationFunction mComputeRootInformation;
};

//==============================================================================

class IdaSolver : public Solver::DaeSolver
{
public:
    explicit IdaSolver();
    ~IdaSolver();

    virtual void initialize(const double &pVoiStart, const double &pVoiEnd,
                            const int &pRatesStatesCount,
                            const int &pCondVarCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
                            double *pCondVar,
                            ComputeEssentialVariablesFunction pComputeEssentialVariables,
                            ComputeResidualsFunction pComputeResiduals,
                            ComputeRootInformationFunction pComputeRootInformation,
                            ComputeStateInformationFunction pComputeStateInformation);

    virtual void solve(double &pVoi, const double &pVoiEnd) const;

private:
    void *mSolver;
    N_Vector mRatesVector;
    N_Vector mStatesVector;
    IdaSolverUserData *mUserData;

    bool mInterpolateSolution;
};

//==============================================================================

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

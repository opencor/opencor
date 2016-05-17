/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// CVODE solver class
//==============================================================================

#pragma once

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include "nvector/nvector_serial.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

static const auto MaximumStepId          = QStringLiteral("MaximumStep");
static const auto MaximumNumberOfStepsId = QStringLiteral("MaximumNumberOfSteps");
static const auto IntegrationMethodId    = QStringLiteral("IntegrationMethod");
static const auto IterationTypeId        = QStringLiteral("IterationType");
static const auto LinearSolverId         = QStringLiteral("LinearSolver");
static const auto PreconditionerId       = QStringLiteral("Preconditioner");
static const auto UpperHalfBandwidthId   = QStringLiteral("UpperHalfBandwidth");
static const auto LowerHalfBandwidthId   = QStringLiteral("LowerHalfBandwidth");
static const auto RelativeToleranceId    = QStringLiteral("RelativeTolerance");
static const auto AbsoluteToleranceId    = QStringLiteral("AbsoluteTolerance");
static const auto InterpolateSolutionId  = QStringLiteral("InterpolateSolution");

//==============================================================================

static const auto AdamsMoultonMethod = QStringLiteral("Adams-Moulton");
static const auto BdfMethod          = QStringLiteral("BDF");

//==============================================================================

static const auto FunctionalIteration = QStringLiteral("Functional");
static const auto NewtonIteration     = QStringLiteral("Newton");

//==============================================================================

static const auto DenseLinearSolver    = QStringLiteral("Dense");
static const auto BandedLinearSolver   = QStringLiteral("Banded");
static const auto DiagonalLinearSolver = QStringLiteral("Diagonal");
static const auto GmresLinearSolver    = QStringLiteral("GMRES");
static const auto BiCgStabLinearSolver = QStringLiteral("BiCGStab");
static const auto TfqmrLinearSolver    = QStringLiteral("TFQMR");

//==============================================================================

static const auto NoPreconditioner     = QStringLiteral("None");
static const auto BandedPreconditioner = QStringLiteral("Banded");

//==============================================================================

// Default CVODE parameter values
// Note #1: a maximum step of 0 means that there is no maximum step as such and
//          that CVODE can use whatever step it sees fit...
// Note #2: CVODE's default maximum number of steps is 500 which ought to be big
//          enough in most cases...

static const double MaximumStepDefaultValue = 0.0;

enum {
    MaximumNumberOfStepsDefaultValue = 500
};

static const auto IntegrationMethodDefaultValue = BdfMethod;
static const auto IterationTypeDefaultValue = NewtonIteration;
static const auto LinearSolverDefaultValue = DenseLinearSolver;
static const auto PreconditionerDefaultValue = BandedPreconditioner;
static const int UpperHalfBandwidthDefaultValue = 0;
static const int LowerHalfBandwidthDefaultValue = 0;

static const double RelativeToleranceDefaultValue = 1.0e-7;
static const double AbsoluteToleranceDefaultValue = 1.0e-7;

static const bool InterpolateSolutionDefaultValue = true;

//==============================================================================

class CvodeSolverUserData
{
public:
    explicit CvodeSolverUserData(double *pConstants, double *pAlgebraic,
                                 Solver::OdeSolver::ComputeRatesFunction pComputeRates);

    double * constants() const;
    double * algebraic() const;

    Solver::OdeSolver::ComputeRatesFunction computeRates() const;

private:
    double *mConstants;
    double *mAlgebraic;

    Solver::OdeSolver::ComputeRatesFunction mComputeRates;
};

//==============================================================================

class CvodeSolver : public Solver::OdeSolver
{
public:
    explicit CvodeSolver();
    ~CvodeSolver();

    virtual void initialize(const double &pVoiStart,
                            const int &pRatesStatesCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
                            ComputeRatesFunction pComputeRates);

    virtual void solve(double &pVoi, const double &pVoiEnd) const;

private:
    void *mSolver;
    N_Vector mStatesVector;
    CvodeSolverUserData *mUserData;

    bool mInterpolateSolution;
};

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

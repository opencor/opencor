/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CVODE solver class
//==============================================================================

#ifndef CVODESOLVER_H
#define CVODESOLVER_H

//==============================================================================

#include "coreodesolver.h"

//==============================================================================

#include "nvector/nvector_serial.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

static const auto MaximumStepId          = QStringLiteral("MaximumStep");
static const auto MaximumNumberOfStepsId = QStringLiteral("MaximumNumberOfSteps");
static const auto RelativeToleranceId    = QStringLiteral("RelativeTolerance");
static const auto AbsoluteToleranceId    = QStringLiteral("AbsoluteTolerance");

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

static const double RelativeToleranceDefaultValue = 1.0e-7;
static const double AbsoluteToleranceDefaultValue = 1.0e-7;

//==============================================================================

class CvodeSolverUserData
{
public:
    explicit CvodeSolverUserData(double *pConstants, double *pAlgebraic,
                                 CoreSolver::CoreOdeSolver::ComputeRatesFunction pComputeRates);

    double * constants() const;
    double * algebraic() const;

    CoreSolver::CoreOdeSolver::ComputeRatesFunction computeRates() const;

private:
    double *mConstants;
    double *mAlgebraic;

    CoreSolver::CoreOdeSolver::ComputeRatesFunction mComputeRates;
};

//==============================================================================

class CvodeSolver : public CoreSolver::CoreOdeSolver
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

    double mMaximumStep;
    int mMaximumNumberOfSteps;
    double mRelativeTolerance;
    double mAbsoluteTolerance;
};

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

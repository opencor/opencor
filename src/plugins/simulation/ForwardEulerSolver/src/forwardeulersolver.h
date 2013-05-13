//==============================================================================
// Forward Euler solver class
//==============================================================================

#ifndef FORWARDEULERSOLVER_H
#define FORWARDEULERSOLVER_H

//==============================================================================

#include "coreodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace ForwardEulerSolver {

//==============================================================================

static const QString StepId = "Step";

//==============================================================================

static const double StepDefaultValue = 1.0;

//==============================================================================

class ForwardEulerSolver : public CoreSolver::CoreOdeSolver
{
public:
    explicit ForwardEulerSolver();

    virtual void initialize(const double &pVoiStart,
                            const int &pRatesStatesCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
                            ComputeRatesFunction pComputeRates);

    virtual void solve(double &pVoi, const double &pVoiEnd) const;

private:
    double mStep;
};

//==============================================================================

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

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

static const QString StepProperty = "Step";

//==============================================================================

static const double DefaultStep = 1.0;

//==============================================================================

class ForwardEulerSolver : public CoreSolver::CoreOdeSolver
{
public:
    explicit ForwardEulerSolver();

    virtual void initialize(const double &pVoiStart, const double &pVoiEnd,
                            const int &pStatesCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
                            ComputeRatesFunction pComputeRates);

    virtual void solve(double &pVoi, const double &pVoiEnd) const;

private:
    double mStep;

    virtual bool isValidProperty(const QString &pName) const;
};

//==============================================================================

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

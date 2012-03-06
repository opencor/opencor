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

class ForwardEulerSolver : public CoreSolver::CoreOdeSolver
{
public:
    void initializeForwardEulerSolver(const double &pStep);

    virtual void solve(double &pVoi, const double &pVoiEnd,
                       CoreSolver::CoreOdeSolverComputeRatesFunction pComputeRates) const;

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

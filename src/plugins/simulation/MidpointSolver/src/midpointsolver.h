//==============================================================================
// Midpoint solver class
//==============================================================================

#ifndef MIDPOINTSOLVER_H
#define MIDPOINTSOLVER_H

//==============================================================================

#include "coreodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace MidpointSolver {

//==============================================================================

static const QString StepProperty = "Step";

//==============================================================================

static const double DefaultStep = 1.0;

//==============================================================================

class MidpointSolver : public CoreSolver::CoreOdeSolver
{
public:
    explicit MidpointSolver();
    ~MidpointSolver();

    virtual void initialize(const double &pVoiStart, const int &pStatesCount,
                            double *pConstants, double *pStates, double *pRates,
                            double *pAlgebraic, ComputeRatesFunction pComputeRates);

    virtual void solve(double &pVoi, const double &pVoiEnd) const;

private:
    double mStep;

    double *mYk;
};

//==============================================================================

}   // namespace MidpointSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

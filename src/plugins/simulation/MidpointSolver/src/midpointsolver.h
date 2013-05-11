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

static const QString StepId = "Step";

//==============================================================================

static const double StepDefaultValue = 1.0;

//==============================================================================

class MidpointSolver : public CoreSolver::CoreOdeSolver
{
public:
    explicit MidpointSolver();
    ~MidpointSolver();

    virtual void initialize(const double &pVoiStart,
                            const int &pRatesStatesCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
                            ComputeRatesFunction pComputeRates);

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

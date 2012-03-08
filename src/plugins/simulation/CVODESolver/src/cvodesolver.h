//==============================================================================
// CVODE solver class
//==============================================================================

#ifndef CVODESOLVER_H
#define CVODESOLVER_H

//==============================================================================

#include "coreodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

static const QString StepProperty = "Step";

//==============================================================================

class CVODESolver : public CoreSolver::CoreOdeSolver
{
public:
    explicit CVODESolver();

    virtual void initialize(const int &pNbOfStates, double *pConstants,
                            double *pRates, double *pStates,
                            double *pAlgebraic);

    virtual void solve(double &pVoi, const double &pVoiEnd,
                       OpenCOR::CoreSolver::CoreOdeSolver::ComputeRatesFunction pComputeRates) const;

private:
    double mStep;

    virtual bool isValidProperty(const QString &pName) const;
};

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

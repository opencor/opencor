//==============================================================================
// Heun solver class
//==============================================================================

#ifndef HEUNSOLVER_H
#define HEUNSOLVER_H

//==============================================================================

#include "coreodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace HeunSolver {

//==============================================================================

static const QString StepId = "Step";

//==============================================================================

static const double StepDefaultValue = 1.0;

//==============================================================================

class HeunSolver : public CoreSolver::CoreOdeSolver
{
public:
    explicit HeunSolver();
    ~HeunSolver();

    virtual void initialize(const double &pVoiStart,
                            const int &pRatesStatesCount, double *pConstants,
                            double *pRates, double *pStates, double *pAlgebraic,
                            ComputeRatesFunction pComputeRates);

    virtual void solve(double &pVoi, const double &pVoiEnd) const;

private:
    double mStep;

    double *mK;
    double *mYk;
};

//==============================================================================

}   // namespace HeunSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Fourth-order Runge-Kutta solver class
//==============================================================================

#ifndef FOURTHORDERRUNGEKUTTASOLVER_H
#define FOURTHORDERRUNGEKUTTASOLVER_H

//==============================================================================

#include "coreodesolver.h"

//==============================================================================

namespace OpenCOR {
namespace FourthOrderRungeKuttaSolver {

//==============================================================================

static const QString StepProperty = "Step";

//==============================================================================

static const double DefaultStep = 1.0;

//==============================================================================

class FourthOrderRungeKuttaSolver : public CoreSolver::CoreOdeSolver
{
public:
    explicit FourthOrderRungeKuttaSolver();
    ~FourthOrderRungeKuttaSolver();

    virtual void initialize(const double &pVoiStart, const int &pStatesCount,
                            double *pConstants, double *pStates, double *pRates,
                            double *pAlgebraic, ComputeRatesFunction pComputeRates);

    virtual void solve(double &pVoi, const double &pVoiEnd) const;

private:
    double mStep;

    double *mK1;
    double *mK23;
    double *mYk234;
};

//==============================================================================

}   // namespace FourthOrderRungeKuttaSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

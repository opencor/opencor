//==============================================================================
// Core ODE solver class
//==============================================================================

#ifndef COREODESOLVER_H
#define COREODESOLVER_H

//==============================================================================

#include "coresolverglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

typedef void (*CoreOdeSolverComputeRatesFunction)(double, double *, double *, double *, double *);

//==============================================================================

class CORESOLVER_EXPORT CoreOdeSolver
{
public:
    explicit CoreOdeSolver();

    virtual void initialize(const int &pNbOfStates, double **pConstants,
                            double **pRates, double **pStates,
                            double **pAlgebraic);

    virtual void solve(double &pVoi, const double &pVoiEnd,
                       CoreOdeSolverComputeRatesFunction pComputeRates) const = 0;

protected:
    int mNbOfStates;

    double **mConstants;
    double **mRates;
    double **mStates;
    double **mAlgebraic;
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

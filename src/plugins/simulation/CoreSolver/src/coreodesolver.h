//==============================================================================
// Core ODE solver class
//==============================================================================

#ifndef COREODESOLVER_H
#define COREODESOLVER_H

//==============================================================================

#include "coresolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

class CORESOLVER_EXPORT CoreOdeSolver : public CoreSolver
{
public:
    typedef void (*ComputeRatesFunction)(double, double *, double *, double *, double *);

    explicit CoreOdeSolver();

    virtual void initialize(const double &pVoiStart, const int &pStatesCount,
                            double *pConstants, double *pRates, double *pStates,
                            double *pAlgebraic,
                            ComputeRatesFunction pComputeRates);

    virtual void solve(double &pVoi, const double &pVoiEnd) const = 0;

protected:
    int mStatesCount;

    double *mConstants;
    double *mRates;
    double *mStates;
    double *mAlgebraic;
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

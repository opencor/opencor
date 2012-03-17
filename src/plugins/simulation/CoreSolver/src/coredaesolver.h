//==============================================================================
// Core DAE solver class
//==============================================================================

#ifndef COREDAESOLVER_H
#define COREDAESOLVER_H

//==============================================================================

#include "coresolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

class CORESOLVER_EXPORT CoreDaeSolver : public CoreSolver
{
public:
    typedef void (*ComputeRatesFunction)(double, double *, double *, double *, double *, double *, double *, double *, double *);

    explicit CoreDaeSolver();

    virtual void initialize(const double &/* pVoiStart */,
                            const int &pStatesCount, const int &pCondVarCount,
                            double *pConstants, double *pRates, double *pStates,
                            double *pAlgebraic,
                            ComputeRatesFunction /* pComputeRates */);

    virtual void solve(double &pVoi, const double &pVoiEnd) const = 0;

protected:
    int mStatesCount;
    int mCondVarCount;

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

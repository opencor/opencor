//==============================================================================
// Core ODE solver class
//==============================================================================

#ifndef COREODESOLVER_H
#define COREODESOLVER_H

//==============================================================================

#include "corevoisolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

class CORESOLVER_EXPORT CoreOdeSolver : public CoreVoiSolver
{
public:
    typedef int (*ComputeRatesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);

    explicit CoreOdeSolver();

    virtual void initialize(const double &pVoiStart, const int &pStatesCount,
                            double *pConstants, double *pStates, double *pRates,
                            double *pAlgebraic, ComputeRatesFunction pComputeRates);

protected:
    ComputeRatesFunction mComputeRates;
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

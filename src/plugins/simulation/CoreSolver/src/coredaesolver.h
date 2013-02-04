//==============================================================================
// Core DAE solver class
//==============================================================================

#ifndef COREDAESOLVER_H
#define COREDAESOLVER_H

//==============================================================================

#include "corevoisolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

class CORESOLVER_EXPORT CoreDaeSolver : public CoreVoiSolver
{
public:
    typedef int (*ComputeEssentialVariablesFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR);
    typedef int (*ComputeResidualsFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR, double *resid);
    typedef int (*ComputeRootInformationFunction)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC, double *CONDVAR);
    typedef int (*ComputeStateInformationFunction)(double *SI);

    explicit CoreDaeSolver();

    virtual void initialize(const double &pVoiStart, const double &pVoiEnd,
                            const int &pStatesCount, const int &pCondVarCount,
                            double *pConstants, double *pStates, double *pRates,
                            double *pAlgebraic, double *pCondVar,
                            ComputeEssentialVariablesFunction pComputeEssentialVariables,
                            ComputeResidualsFunction pComputeResiduals,
                            ComputeRootInformationFunction pComputeRootInformation,
                            ComputeStateInformationFunction pComputeStateInformation);

protected:
    int mCondVarCount;

    double *mCondVar;
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

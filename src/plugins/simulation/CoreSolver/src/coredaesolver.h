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
    typedef int (*ComputeEssentialVariablesFunction)(double, double *, double *, double *, double *, double *);
    typedef int (*ComputeResidualsFunction)(double, double *, double *, double *, double *, double *, double *);
    typedef int (*ComputeRootInformationFunction)(double, double *, double *, double *, double *, double *);
    typedef int (*ComputeStateInformationFunction)(double *);

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

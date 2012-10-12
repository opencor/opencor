//==============================================================================
// Core non-linear algebraic solver class
//==============================================================================

#ifndef CORENONLINEARALGEBRAICSOLVER_H
#define CORENONLINEARALGEBRAICSOLVER_H

//==============================================================================

#include "coresolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

class CORESOLVER_EXPORT CoreNonLinearAlgebraicSolver : public CoreSolver
{
public:
    typedef int (*ComputeSystemFunction)(double *, double *, void *);

    explicit CoreNonLinearAlgebraicSolver();

    virtual void initialize(ComputeSystemFunction pComputeSystem,
                            double *pParameters, int pSize, void *pUserData);

    virtual void solve() const = 0;

private:
    ComputeSystemFunction mComputeSystem;

    double *mParameters;
    int mSize;
    void *mUserData;
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

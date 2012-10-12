//==============================================================================
// Core non-linear algebraic solver class
//==============================================================================

#ifndef CORENLASOLVER_H
#define CORENLASOLVER_H

//==============================================================================

#include "coresolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

class CORESOLVER_EXPORT CoreNlaSolver : public CoreSolver
{
public:
    typedef int (*ComputeSystemFunction)(double *, double *, void *);

    explicit CoreNlaSolver();

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

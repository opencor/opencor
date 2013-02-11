//==============================================================================
// Core NLA solver class
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
    typedef void (*ComputeSystemFunction)(double *, double *, void *);

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

void CORESOLVER_EXPORT resetGlobalNlaSolver();
void CORESOLVER_EXPORT setGlobalNlaSolver(CoreNlaSolver *pGlobalNlaSolver);
CoreNlaSolver CORESOLVER_EXPORT * globalNlaSolver();

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

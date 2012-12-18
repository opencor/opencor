//==============================================================================
// KINSOL solver class
//==============================================================================

#ifndef KINSOLSOLVER_H
#define KINSOLSOLVER_H

//==============================================================================

#include "corenlasolver.h"

//==============================================================================

#include "nvector/nvector_serial.h"

//==============================================================================

namespace OpenCOR {
namespace KINSOLSolver {

//==============================================================================

struct KinsolSolverUserData {
    void *userData;

    OpenCOR::CoreSolver::CoreNlaSolver::ComputeSystemFunction computeSystem;
};

//==============================================================================

class KinsolSolver : public CoreSolver::CoreNlaSolver
{
public:
    explicit KinsolSolver();
    ~KinsolSolver();

    virtual void initialize(ComputeSystemFunction pComputeSystem,
                            double *pParameters, int pSize, void *pUserData);

    virtual void solve() const;

private:
    void *mSolver;
    N_Vector mParametersVector;
    N_Vector mOnesVector;
    KinsolSolverUserData mKinsolUserData;

    void reset();
};

//==============================================================================

}   // namespace KINSOLSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

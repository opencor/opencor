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

class KinsolSolverUserData
{
public:
    explicit KinsolSolverUserData(void *pUserData,
                                  CoreSolver::CoreNlaSolver::ComputeSystemFunction pComputeSystem);

    void * userData() const;

    CoreSolver::CoreNlaSolver::ComputeSystemFunction computeSystem() const;

private:
    void *mUserData;

    CoreSolver::CoreNlaSolver::ComputeSystemFunction mComputeSystem;
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
    KinsolSolverUserData *mUserData;

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

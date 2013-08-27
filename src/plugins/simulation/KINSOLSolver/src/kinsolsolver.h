/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

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

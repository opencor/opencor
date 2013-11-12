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
// Core DAE solver class
//==============================================================================

#include "coredaesolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreDaeSolver::CoreDaeSolver() :
    CoreVoiSolver(),
    mCondVarCount(0),
    mOldRates(0),
    mOldStates(0),
    mCondVar(0)
{
}

//==============================================================================

CoreDaeSolver::~CoreDaeSolver()
{
    // Delete some internal objects

    delete[] mOldRates;
    delete[] mOldStates;
}

//==============================================================================

void CoreDaeSolver::initialize(const double &pVoiStart, const double &pVoiEnd,
                               const int &pRatesStatesCount,
                               const int &pCondVarCount, double *pConstants,
                               double *pRates, double *pStates,
                               double *pAlgebraic, double *pCondVar,
                               ComputeEssentialVariablesFunction pComputeEssentialVariables,
                               ComputeResidualsFunction pComputeResiduals,
                               ComputeRootInformationFunction pComputeRootInformation,
                               ComputeStateInformationFunction pComputeStateInformation)
{
    Q_UNUSED(pVoiStart);
    Q_UNUSED(pVoiEnd);
    Q_UNUSED(pComputeEssentialVariables);
    Q_UNUSED(pComputeResiduals);
    Q_UNUSED(pComputeRootInformation);
    Q_UNUSED(pComputeStateInformation);

    // Initialise the DAE solver

    mRatesStatesCount = pRatesStatesCount;
    mCondVarCount     = pCondVarCount;

    mConstants = pConstants;
    mRates     = pRates;
    mStates    = pStates;
    mAlgebraic = pAlgebraic;
    mCondVar   = pCondVar;

    delete[] mOldRates;
    delete[] mOldStates;

    mOldRates  = new double[pRatesStatesCount];
    mOldStates = new double[pRatesStatesCount];

    memcpy(mOldRates, pRates, pRatesStatesCount*SizeOfDouble);
    memcpy(mOldStates, pStates, pRatesStatesCount*SizeOfDouble);
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

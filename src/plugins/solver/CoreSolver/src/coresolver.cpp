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
// Core solver class
//==============================================================================

#include "coresolver.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

CoreSolver::CoreSolver() :
    mProperties(Properties())
{
}

//==============================================================================

void CoreSolver::setProperties(const Properties &pProperties)
{
    // Set a property's value, but only if it is a valid property

    mProperties = pProperties;
}

//==============================================================================

void CoreSolver::emitError(const QString &pErrorMessage)
{
    // Let people know that an error occured, but first reformat the error a
    // bit, if needed

    QString errorMessage;

    if (pErrorMessage.startsWith("Newton"))
        errorMessage = pErrorMessage;
    else
        errorMessage = pErrorMessage[0].toLower()+pErrorMessage.right(pErrorMessage.size()-1);

    if (!pErrorMessage.right(3).compare("..."))
        emit error(errorMessage.left(errorMessage.size()-3));
    else if (!pErrorMessage.right(1).compare("."))
        emit error(errorMessage.left(errorMessage.size()-1));
    else
        emit error(errorMessage);
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// View interface
//==============================================================================

#include "viewinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto ViewModeUnknown    = QStringLiteral("Unknown");
static const auto ViewModeEditing    = QStringLiteral("Editing");
static const auto ViewModeSimulation = QStringLiteral("Simulation");
static const auto ViewModeAnalysis   = QStringLiteral("Analysis");

//==============================================================================

QString ViewInterface::viewModeAsString(const ViewInterface::Mode &pMode)
{
    // Return the mode corresponding to the given mode string

    switch (pMode) {
    case ViewInterface::Editing:
        return ViewModeEditing;
    case ViewInterface::Simulation:
        return ViewModeSimulation;
    case ViewInterface::Analysis:
        return ViewModeAnalysis;
    default:   // ViewInterface::Unknown
        return ViewModeUnknown;
    }
}

//==============================================================================

ViewInterface::Mode ViewInterface::viewModeFromString(const QString &pMode)
{
    // Return the mode string corresponding to the given mode

    if (!pMode.compare(ViewModeEditing))
        return ViewInterface::Editing;
    else if (!pMode.compare(ViewModeSimulation))
        return ViewInterface::Simulation;
    else if (!pMode.compare(ViewModeAnalysis))
        return ViewInterface::Analysis;
    else
        return ViewInterface::Unknown;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

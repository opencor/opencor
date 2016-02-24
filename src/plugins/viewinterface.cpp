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

static const auto ViewModeUnknown    = QStringLiteral("UnknownMode");
#ifdef ENABLE_SAMPLES
static const auto ViewModeSample     = QStringLiteral("SampleMode");
#endif
static const auto ViewModeEditing    = QStringLiteral("EditingMode");
static const auto ViewModeSimulation = QStringLiteral("SimulationMode");
static const auto ViewModeAnalysis   = QStringLiteral("AnalysisMode");

//==============================================================================

QString ViewInterface::viewModeAsString(const ViewInterface::Mode &pMode)
{
    // Return the mode corresponding to the given mode string

    switch (pMode) {
#ifdef ENABLE_SAMPLES
    case ViewInterface::SampleMode:
        return ViewModeSample;
#endif
    case ViewInterface::EditingMode:
        return ViewModeEditing;
    case ViewInterface::SimulationMode:
        return ViewModeSimulation;
    case ViewInterface::AnalysisMode:
        return ViewModeAnalysis;
    default:   // ViewInterface::UnknownMode
        return ViewModeUnknown;
    }
}

//==============================================================================

ViewInterface::Mode ViewInterface::viewModeFromString(const QString &pMode)
{
    // Return the mode string corresponding to the given mode

#ifdef ENABLE_SAMPLES
    if (!pMode.compare(ViewModeSample))
        return ViewInterface::SampleMode;
    else if (!pMode.compare(ViewModeEditing))
#else
    if (!pMode.compare(ViewModeEditing))
#endif
        return ViewInterface::EditingMode;
    else if (!pMode.compare(ViewModeSimulation))
        return ViewInterface::SimulationMode;
    else if (!pMode.compare(ViewModeAnalysis))
        return ViewInterface::AnalysisMode;
    else
        return ViewInterface::UnknownMode;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

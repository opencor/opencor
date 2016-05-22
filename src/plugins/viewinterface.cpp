/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

QString ViewInterface::modeAsString(const Mode &pMode)
{
    // Return the mode corresponding to the given mode string

    switch (pMode) {
    case UnknownMode:
        return ViewModeUnknown;
#ifdef ENABLE_SAMPLES
    case SampleMode:
        return ViewModeSample;
#endif
    case EditingMode:
        return ViewModeEditing;
    case SimulationMode:
        return ViewModeSimulation;
    case AnalysisMode:
        return ViewModeAnalysis;
    }

    return "???";
    // Note: we can't reach this point, but without it we may be told that not
    //       all control paths return a value...
}

//==============================================================================

ViewInterface::Mode ViewInterface::modeFromString(const QString &pMode)
{
    // Return the mode string corresponding to the given mode

#ifdef ENABLE_SAMPLES
    if (!pMode.compare(ViewModeSample))
        return SampleMode;
    else if (!pMode.compare(ViewModeEditing))
#else
    if (!pMode.compare(ViewModeEditing))
#endif
        return EditingMode;
    else if (!pMode.compare(ViewModeSimulation))
        return SimulationMode;
    else if (!pMode.compare(ViewModeAnalysis))
        return AnalysisMode;
    else
        return UnknownMode;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

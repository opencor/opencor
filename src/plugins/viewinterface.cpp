/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// View interface
//==============================================================================

#include "viewinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int viewInterfaceVersion()
{
    // Version of the view interface

    return 1;
}

//==============================================================================

static const auto ViewModeUnknown    = QStringLiteral("UnknownMode");
#ifdef ENABLE_SAMPLE_PLUGINS
static const auto ViewModeSample     = QStringLiteral("SampleMode");
#endif
#ifdef ENABLE_TEST_PLUGINS
static const auto ViewModeTest       = QStringLiteral("TestMode");
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
#ifdef ENABLE_SAMPLE_PLUGINS
    case SampleMode:
        return ViewModeSample;
#endif
#ifdef ENABLE_TEST_PLUGINS
    case TestMode:
        return ViewModeTest;
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

#if defined(ENABLE_SAMPLE_PLUGINS) && defined(ENABLE_TEST_PLUGINS)
    if (!pMode.compare(ViewModeSample))
        return SampleMode;
    else if (!pMode.compare(ViewModeTest))
        return TestMode;
    else if (!pMode.compare(ViewModeEditing))
#elif defined(ENABLE_SAMPLE_PLUGINS)
    if (!pMode.compare(ViewModeSample))
        return SampleMode;
    else if (!pMode.compare(ViewModeEditing))
#elif defined(ENABLE_TEST_PLUGINS)
    if (!pMode.compare(ViewModeTest))
        return TestMode;
    else if (!pMode.compare(ViewModeEditing))
#else
    if (!pMode.compare(ViewModeEditing))
#endif
        return EditingMode;
    else if (!pMode.compare(ViewModeSimulation))
        return SimulationMode;
    else if (!pMode.compare(ViewModeAnalysis))
        return AnalysisMode;

    return UnknownMode;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

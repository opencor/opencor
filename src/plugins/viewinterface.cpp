/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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

    return 2;
}

//==============================================================================

ViewInterface::~ViewInterface() = default;

//==============================================================================

static const char *ViewModeUnknown    = "UnknownMode";
static const char *ViewModeEditing    = "EditingMode";
static const char *ViewModeSimulation = "SimulationMode";
#ifdef ENABLE_SAMPLE_PLUGINS
static const char *ViewModeSample     = "SampleMode";
#endif
#ifdef ENABLE_TEST_PLUGINS
static const char *ViewModeTest       = "TestMode";
#endif

//==============================================================================

QString ViewInterface::modeAsString(Mode pMode)
{
    // Return the mode corresponding to the given mode string

    switch (pMode) {
    case Mode::Unknown:
        return ViewModeUnknown;
    case Mode::Editing:
        return ViewModeEditing;
    case Mode::Simulation:
        return ViewModeSimulation;
#ifdef ENABLE_SAMPLE_PLUGINS
    case Mode::Sample:
        return ViewModeSample;
#endif
#ifdef ENABLE_TEST_PLUGINS
    case Mode::Test:
        return ViewModeTest;
#endif
    }

    return "???";
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
}

//==============================================================================

ViewInterface::Mode ViewInterface::modeFromString(const QString &pMode)
{
    // Return the mode string corresponding to the given mode

    if (pMode == ViewModeEditing) {
        return Mode::Editing;
    }

    if (pMode == ViewModeSimulation) {
        return Mode::Simulation;
    }

#ifdef ENABLE_SAMPLE_PLUGINS
    if (pMode == ViewModeSample) {
        return Mode::Sample;
    }
#endif

#ifdef ENABLE_TEST_PLUGINS
    if (pMode == ViewModeTest) {
        return Mode::Test;
    }
#endif

    return Mode::Unknown;
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

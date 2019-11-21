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

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int viewInterfaceVersion();

//==============================================================================

class ViewInterface
{
public:
    enum class Mode {
        Unknown,
#ifdef ENABLE_SAMPLE_PLUGINS
        Sample,
#endif
#ifdef ENABLE_TEST_PLUGINS
        Test,
#endif
        Editing,
        Simulation
    };

    virtual ~ViewInterface();

#define INTERFACE_DEFINITION
    #include "viewinterface.inl"
#undef INTERFACE_DEFINITION

    static QString modeAsString(Mode pMode);
    static Mode modeFromString(const QString &pMode);
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::ViewInterface, "OpenCOR::ViewInterface")

//==============================================================================
// End of file
//==============================================================================

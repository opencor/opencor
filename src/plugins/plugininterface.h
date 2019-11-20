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
// Plugin interface
//==============================================================================

#pragma once

//==============================================================================

#include "plugin.h"

//==============================================================================

class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int pluginInterfaceVersion();

//==============================================================================

class PluginInterface
{
public:
    virtual ~PluginInterface();

#define INTERFACE_DEFINITION
    #include "plugininterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

using PluginInterfaces = QList<PluginInterface *>;

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::PluginInterface, "OpenCOR::PluginInterface")

//==============================================================================
// End of file
//==============================================================================

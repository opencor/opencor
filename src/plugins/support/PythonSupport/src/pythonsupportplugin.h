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
// Python support plugin
//==============================================================================

#pragma once

//==============================================================================

#include "plugininfo.h"
#include "plugininterface.h"
#include "pythonsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace PythonSupport {

//==============================================================================

PLUGININFO_FUNC PythonSupportPluginInfo();

//==============================================================================

class PYTHONSUPPORT_EXPORT PythonSupportPlugin : public QObject,
                                                 public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.PythonSupportPlugin" FILE "pythonsupportplugin.json")

    Q_INTERFACES(OpenCOR::PluginInterface)

public:
#include "plugininterface.inl"
};

//==============================================================================

} // namespace PythonSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

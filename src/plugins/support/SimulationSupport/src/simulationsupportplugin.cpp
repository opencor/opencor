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
// Simulation support plugin
//==============================================================================

#include "simulationsupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

PLUGININFO_FUNC SimulationSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to support simulations."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter des simulations."));

    return new PluginInfo(PluginInfo::Support, false, false,
                          QStringList() << "COMBINESupport",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void SimulationSupportPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

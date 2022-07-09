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
// BondGraphUIWidgets plugin
//==============================================================================

#include "bondgraphuiwidgetsplugin.h"

//==============================================================================

namespace OpenCOR {
namespace BondGraphUIWidgets {

//==============================================================================

PLUGININFO_FUNC BondGraphUIWidgetsPluginInfo()
{
    static const Descriptions descriptions = {
                                                 { "en", QString::fromUtf8(R"(a plugin to access the <a href="https://github.com/OpenCMISS/zinc">BondGraph UI widgets</a> interface.)") },
                                                 { "fr", QString::fromUtf8(R"(une extension pour accéder la interface-librairie <a href="https://github.com/OpenCMISS/zinc">BondGraph UI widgets</a>.)") }
                                             };

    return new PluginInfo(PluginInfo::Category::ThirdParty, false, false,
                          {"BondGraphAPI"},
                          descriptions);
}

//==============================================================================

} // namespace BondGraphUIWidgets
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

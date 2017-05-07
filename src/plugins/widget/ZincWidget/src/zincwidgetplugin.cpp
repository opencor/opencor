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
// Zinc widget plugin
//==============================================================================

#include "zincwidgetplugin.h"

//==============================================================================

namespace OpenCOR {
namespace ZincWidget {

//==============================================================================

PLUGININFO_FUNC ZincWidgetPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to visualise a <a href=\"https://github.com/OpenCMISS/zinc\">Zinc</a> scene."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour visualiser une scène <a href=\"https://github.com/OpenCMISS/zinc\">Zinc</a>."));

    return new PluginInfo(PluginInfo::Widget, false, false,
                          QStringList() << "Core" << "Zinc",
                          descriptions);
}

//==============================================================================

}   // namespace ZincWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

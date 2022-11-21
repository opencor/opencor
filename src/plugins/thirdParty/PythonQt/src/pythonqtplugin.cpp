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
// PythonQt plugin
//==============================================================================

#include "pythonqtplugin.h"

//==============================================================================

#include <Qt>

//==============================================================================

namespace OpenCOR {
namespace PythonQt {

//==============================================================================

PLUGININFO_FUNC PythonQtPluginInfo()
{
    static const Descriptions descriptions = {
                                                 { "en", QString::fromUtf8("a plugin to access <a href=\"https://mevislab.github.io/pythonqt/\">PythonQt</a>.") },
                                                 { "fr", QString::fromUtf8("un plugin pour accéder <a href=\"https://mevislab.github.io/pythonqt/\">PythonQt</a>.") }
                                             };

    return new PluginInfo(PluginInfo::Category::ThirdParty, false, false,
                          { "Python" },
                          descriptions);
}

//==============================================================================

} // namespace PythonQt
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

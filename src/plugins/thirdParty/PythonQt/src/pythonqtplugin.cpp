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
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to use <a href=\"https://www.qt.io/\">Qt</a> with <a href=\"https://www.python.org/\">Python</a>."));
    descriptions.insert("fr", QString::fromUtf8("un plugin pour utiliser <a href=\"https://www.qt.io/\">Qt</a> avec <a href=\"https://www.python.org/\">Python</a>"));

    return new PluginInfo(PluginInfo::Category::ThirdParty, false, false,
                          QStringList() << "Python",
                          descriptions);
}

//==============================================================================

} // namespace PythonQt
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// zlib plugin
//==============================================================================

#include "zlibplugin.h"

//==============================================================================

namespace OpenCOR {
namespace zlib {

//==============================================================================

PLUGININFO_FUNC zlibPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access <a href=\"http://www.zlib.net/\">zlib</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder <a href=\"http://www.zlib.net/\">zlib</a>."));

    return new PluginInfo(PluginInfo::ThirdParty, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace zlib
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

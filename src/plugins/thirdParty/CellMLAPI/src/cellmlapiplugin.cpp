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
// CellML API plugin
//==============================================================================

#include "cellmlapiplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAPI {

//==============================================================================

PLUGININFO_FUNC CellMLAPIPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to access the <a href="https://github.com/cellmlapi/cellml-api/">CellML API</a>.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour accéder l'<a href="https://github.com/cellmlapi/cellml-api/">API CellML</a>.)"));

    return new PluginInfo(PluginInfo::Category::ThirdParty, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

} // namespace CellMLAPI
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

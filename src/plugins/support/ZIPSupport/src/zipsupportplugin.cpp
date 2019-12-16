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
// ZIP support plugin
//==============================================================================

#include "zipsupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace ZIPSupport {

//==============================================================================

PLUGININFO_FUNC ZIPSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"|(a plugin to support the <a href="https://en.wikipedia.org/wiki/Zip_(file_format)">ZIP</a> format.)|"));
    descriptions.insert("fr", QString::fromUtf8(R"|(une extension pour supporter le format <a href="https://en.wikipedia.org/wiki/Zip_(file_format)">ZIP</a>.)|"));
    // Note: normally, we wouldn't use a delimiter ("|" in our case) for our raw
    //       string literal, but if we didn't then we would have problems with
    //       the ZIP file format link (since it ends with a closing bracket)...

    return new PluginInfo(PluginInfo::Category::Support, false, false,
                          QStringList() << "Core" << "zlib",
                          descriptions);
}

//==============================================================================

} // namespace ZIPSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

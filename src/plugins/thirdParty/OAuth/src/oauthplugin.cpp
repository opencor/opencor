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
// OAuth plugin
//==============================================================================

#include "oauthplugin.h"

//==============================================================================

namespace OpenCOR {
namespace OAuth {

//==============================================================================

PLUGININFO_FUNC OAuthPluginInfo()
{
    static const Descriptions descriptions = {
                                                 { "en", QString::fromUtf8(R"(a plugin to access <a href="https://github.com/pipacs/o2">OAuth for Qt</a>.)") },
                                                 { "fr", QString::fromUtf8(R"(une extension pour accéder <a href="https://github.com/pipacs/o2">OAuth for Qt</a>.)") }
                                             };

    return new PluginInfo(PluginInfo::Category::ThirdParty, false, false,
                          {},
                          descriptions);
}

//==============================================================================

} // namespace OAuth
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

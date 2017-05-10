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
// Web Viewer widget plugin
//==============================================================================

#include "webviewerwidgetplugin.h"

//==============================================================================

namespace OpenCOR {
namespace WebViewerWidget {

//==============================================================================

PLUGININFO_FUNC WebViewerWidgetPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to visualise Web documents."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour visualiser des documents Web."));

    return new PluginInfo(PluginInfo::Widget, false, false,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================

}   // namespace WebViewerWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

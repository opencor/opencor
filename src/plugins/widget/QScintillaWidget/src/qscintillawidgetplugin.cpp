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
// QScintilla widget plugin
//==============================================================================

#include "qscintillawidgetplugin.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaWidget {

//==============================================================================

PLUGININFO_FUNC QScintillaWidgetPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to support <a href="https://www.riverbankcomputing.com/software/qscintilla/intro">QScintilla</a>.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour supporter <a href="https://www.riverbankcomputing.com/software/qscintilla/intro">QScintilla</a>.)"));

    return new PluginInfo(PluginInfo::Category::Widget, false, false,
                          QStringList() << "Core" << "QScintilla",
                          descriptions);
}

//==============================================================================

} // namespace QScintillaWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

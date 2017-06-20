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
// Compiler plugin
//==============================================================================

#include "compilerplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

PLUGININFO_FUNC CompilerPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to support code compilation."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter la compilation de code."));

    return new PluginInfo(PluginInfo::Miscellaneous, false, false,
                          QStringList() << "Core" << "LLVMClang",
                          descriptions);
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

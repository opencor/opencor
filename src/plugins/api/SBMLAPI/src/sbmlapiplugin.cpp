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
// SBML API plugin
//==============================================================================

#include "sbmlapiplugin.h"

//==============================================================================

namespace OpenCOR {
namespace SBMLAPI {

//==============================================================================

PLUGININFO_FUNC SBMLAPIPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a>."));

    return new PluginInfo(PluginInfo::Api, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace SBMLAPI
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

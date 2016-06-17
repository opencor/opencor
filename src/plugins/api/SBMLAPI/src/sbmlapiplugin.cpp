/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

    descriptions.insert("en", QString::fromUtf8("a plugin to access <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a> 5.13.0."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a> 5.13.0."));

    return new PluginInfo("API", false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace SBMLAPI
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

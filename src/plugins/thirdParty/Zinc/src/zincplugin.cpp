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
// Zinc plugin
//==============================================================================

#include "zincplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Zinc {

//==============================================================================

PLUGININFO_FUNC ZincPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"https://github.com/OpenCMISS/zinc\">Zinc</a> library."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder la librairie <a href=\"https://github.com/OpenCMISS/zinc\">Zinc</a>."));

    return new PluginInfo(PluginInfo::ThirdParty, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace Zinc
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

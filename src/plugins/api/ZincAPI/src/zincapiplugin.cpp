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
// Zinc API plugin
//==============================================================================

#include "zincapiplugin.h"

//==============================================================================

namespace OpenCOR {
namespace ZincAPI {

//==============================================================================

PLUGININFO_FUNC ZincAPIPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"http://physiomeproject.org/software/opencmiss/zinc\">Zinc API</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder l'<a href=\"http://physiomeproject.org/software/opencmiss/zinc\">API Zinc</a>."));

    return new PluginInfo(PluginInfo::Api, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace ZincAPI
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

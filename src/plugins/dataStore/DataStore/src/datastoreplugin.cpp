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
// Data store plugin
//==============================================================================

#include "datastoreplugin.h"

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

PLUGININFO_FUNC DataStorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides core data store facilities."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit les fonctionalités de base d'un magasin de données."));

    return new PluginInfo(PluginInfo::DataStore, false, false,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void DataStorePlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================

}   // namespace DataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

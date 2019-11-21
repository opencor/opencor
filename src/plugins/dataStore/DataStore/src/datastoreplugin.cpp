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
// Data store plugin
//==============================================================================

#include "datastoreplugin.h"
#include "datastorepythonwrapper.h"

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

PLUGININFO_FUNC DataStorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides core data store facilities."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit les fonctionalités de base d'un magasin de données."));

    return new PluginInfo(PluginInfo::Category::DataStore, false, false,
                          QStringList() << "Core" << "PythonQtSupport",
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
// Python interface
//==============================================================================

void DataStorePlugin::registerPythonClasses(void *pModule)
{
    // Register our Python classes

    new DataStorePythonWrapper(pModule, this);
}

//==============================================================================

} // namespace DataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

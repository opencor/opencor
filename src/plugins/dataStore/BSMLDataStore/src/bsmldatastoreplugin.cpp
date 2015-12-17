/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// BSMLDataStore plugin
//==============================================================================

#include "bsmldatastoreexporter.h"
#include "bsmldatastoreplugin.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace BSMLDataStore {

//==============================================================================

PLUGININFO_FUNC BSMLDataStorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a BioSignalML specific data store plugin."));
    descriptions.insert("fr", QString::fromUtf8("une extension de magasin de données spécifique à BioSignalML."));

    return new PluginInfo("Data Store", true, false,
                          QStringList() << "BioSignalMLAPI",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void BSMLDataStorePlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Data store interface
//==============================================================================

DataStore::DataStoreExporter * BSMLDataStorePlugin::newDataStoreExporterInstance(const QString &pId) const
{
    Q_UNUSED(pId);

    return mExporter;
}

//==============================================================================

void BSMLDataStorePlugin::deleteDataStoreExporterInstance(DataStore::DataStoreExporter *pDataStoreExporterInstance)
{
    Q_UNUSED(pDataStoreExporterInstance);

    // We don't handle this interface...
}

//==============================================================================

QString BSMLDataStorePlugin::dataStoreName() const
{
    // Return the name of the data store

    return "BioSignalML";
}

//==============================================================================
// Plugin interface
//==============================================================================

void BSMLDataStorePlugin::initializePlugin()
{
    // Create our BSML data store exporter

    mExporter = new BioSignalMLExporter();
}

//==============================================================================

void BSMLDataStorePlugin::finalizePlugin()
{
    // Delete our BSML data store exporter

    delete mExporter;
}

//==============================================================================

void BSMLDataStorePlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

//==============================================================================

void BSMLDataStorePlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void BSMLDataStorePlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void BSMLDataStorePlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace BSMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

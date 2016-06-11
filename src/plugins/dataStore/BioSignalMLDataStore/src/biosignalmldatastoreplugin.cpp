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
// BioSignalMLDataStore plugin
//==============================================================================

#include "biosignalmldatastoreexporter.h"
#include "biosignalmldatastoreplugin.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

PLUGININFO_FUNC BioSignalMLDataStorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a BioSignalML specific data store plugin."));
    descriptions.insert("fr", QString::fromUtf8("une extension de magasin de données spécifique à BioSignalML."));

    return new PluginInfo("Data Store", true, false,
                          QStringList() << "BioSignalMLAPI" << "Core",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void BioSignalMLDataStorePlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Data store interface
//==============================================================================

DataStore::DataStoreExporter * BioSignalMLDataStorePlugin::newDataStoreExporterInstance(const QString &pId) const
{
    Q_UNUSED(pId);

    return mExporter;
}

//==============================================================================

void BioSignalMLDataStorePlugin::deleteDataStoreExporterInstance(DataStore::DataStoreExporter *pDataStoreExporterInstance)
{
    Q_UNUSED(pDataStoreExporterInstance);

    // We don't handle this interface...
}

//==============================================================================

QString BioSignalMLDataStorePlugin::dataStoreName() const
{
    // Return the name of the data store

    return "BioSignalML";
}

//==============================================================================
// Plugin interface
//==============================================================================

void BioSignalMLDataStorePlugin::initializePlugin()
{
    // Create our BioSignalML data store exporter

    mExporter = new BioSignalMLExporter();
}

//==============================================================================

void BioSignalMLDataStorePlugin::finalizePlugin()
{
    // Delete our BioSignalML data store exporter

    delete mExporter;
}

//==============================================================================

void BioSignalMLDataStorePlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void BioSignalMLDataStorePlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void BioSignalMLDataStorePlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void BioSignalMLDataStorePlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

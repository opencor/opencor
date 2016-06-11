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
// CSVDataStore plugin
//==============================================================================

#include "csvdatastoreexporter.h"
#include "csvdatastoreplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

PLUGININFO_FUNC CSVDataStorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a CSV specific data store plugin."));
    descriptions.insert("fr", QString::fromUtf8("une extension de magasin de données spécifique à CSV."));

    return new PluginInfo("Data Store", true, false,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void CSVDataStorePlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Data store interface
//==============================================================================

DataStore::DataStoreExporter * CSVDataStorePlugin::newDataStoreExporterInstance(const QString &pId) const
{
    Q_UNUSED(pId);

    // Return the instance of our CSV data store exporter

    return mCsvDataStoreExporter;
}

//==============================================================================

void CSVDataStorePlugin::deleteDataStoreExporterInstance(DataStore::DataStoreExporter *pDataStoreExporterInstance)
{
    Q_UNUSED(pDataStoreExporterInstance);

    // We don't handle this interface...
}

//==============================================================================

QString CSVDataStorePlugin::dataStoreName() const
{
    // Return the name of the data store

    return "CSV";
}

//==============================================================================
// Plugin interface
//==============================================================================

void CSVDataStorePlugin::initializePlugin()
{
    // Create our CSV data store explorer instance

    mCsvDataStoreExporter = new CsvDataStoreExporter();
}

//==============================================================================

void CSVDataStorePlugin::finalizePlugin()
{
    // Create our CSV data store explorer instance

    delete mCsvDataStoreExporter;
}

//==============================================================================

void CSVDataStorePlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CSVDataStorePlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CSVDataStorePlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CSVDataStorePlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// CSV data store plugin
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
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

QString CSVDataStorePlugin::dataStoreName() const
{
    // Return the name of the data store

    return "CSV";
}

//==============================================================================

DataStore::DataStoreData * CSVDataStorePlugin::getData(const QString &pFileName,
                                                       DataStore::DataStore *pDataStore) const
{
    Q_UNUSED(pDataStore);

    // Retrieve the name of the CSV file where our data is to be exported

    QString csvFilter = QObject::tr("CSV File")+" (*.csv)";
    QString fileName = Core::getSaveFileName(QObject::tr("Export To CSV"),
                                             Core::newFileName(pFileName, QObject::tr("Data"), false, "csv"),
                                             QStringList() << csvFilter,
                                             &csvFilter);

    if (fileName.isEmpty())
        return 0;
    else
        return new DataStore::DataStoreData(fileName);
}

//==============================================================================

DataStore::DataStoreExporter * CSVDataStorePlugin::dataStoreExporterInstance(const QString &pFileName,
                                                                             DataStore::DataStore *pDataStore,
                                                                             DataStore::DataStoreData *pDataStoreData) const
{
    // Return an instance of our CSV data store exporter

    return new CsvDataStoreExporter(pFileName, pDataStore, pDataStoreData);
}

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

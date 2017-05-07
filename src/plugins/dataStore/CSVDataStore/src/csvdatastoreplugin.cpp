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
// CSV data store plugin
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "csvdatastoreexporter.h"
#include "csvdatastoreplugin.h"
#include "datastoredialog.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

PLUGININFO_FUNC CSVDataStorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a CSV specific data store plugin."));
    descriptions.insert("fr", QString::fromUtf8("une extension de magasin de données spécifique à CSV."));

    return new PluginInfo(PluginInfo::DataStore, true, false,
                          QStringList() << "DataStore",
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
    // Ask which data should be exported

    DataStore::DataStoreDialog dataStoreDialog(pDataStore, true, Core::mainWindow());
    QSettings settings;

    settings.beginGroup(SettingsPlugins);
    settings.beginGroup("CSVDataStore");
    settings.beginGroup("DataStoreDialog");

    if (dataStoreDialog.exec(&settings)) {
        // Now that we know which data to export, we can ask for the name of the
        // CSV file where it is to be exported

        QString csvFilter = QObject::tr("CSV File")+" (*.csv)";
        QString fileName = Core::getSaveFileName(QObject::tr("Export To CSV"),
                                                 Core::newFileName(pFileName, QObject::tr("Data"), false, "csv"),
                                                 QStringList() << csvFilter,
                                                 &csvFilter);

        if (!fileName.isEmpty())
            return new DataStore::DataStoreData(fileName, dataStoreDialog.selectedData());
    }

    return 0;
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

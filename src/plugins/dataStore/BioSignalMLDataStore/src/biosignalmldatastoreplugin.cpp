/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// BioSignalML data store plugin
//==============================================================================

#include "biosignalmldatastoredata.h"
#include "biosignalmldatastoredialog.h"
#include "biosignalmldatastoreexporter.h"
#include "biosignalmldatastoreimporter.h"
#include "biosignalmldatastoreplugin.h"
#include "biosignalmlinterface.h"
#include "corecliutils.h"
#include "coreguiutils.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

#include "libbiosignalmlbegin.h"
    #include "biosignalml/data/hdf5.h"
#include "libbiosignalmlend.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

PLUGININFO_FUNC BioSignalMLDataStorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a BioSignalML specific data store plugin."));
    descriptions.insert("fr", QString::fromUtf8("une extension de magasin de données spécifique à BioSignalML."));

    return new PluginInfo(PluginInfo::Category::DataStore, true, false,
                          { "DataStore", "libBioSignalML" },
                          descriptions);
}

//==============================================================================

BioSignalMLDataStorePlugin::BioSignalMLDataStorePlugin()
{
    // Keep track of our file type interface

    static BiosignalmlInterfaceData data(qobject_cast<FileTypeInterface *>(this));

    Core::globalInstance(BiosignalmlInterfaceDataSignature, &data);
}

//==============================================================================
// Data store interface
//==============================================================================

QString BioSignalMLDataStorePlugin::dataStoreName() const
{
    // Return the name of the data store

    return "BioSignalML";
}

//==============================================================================

DataStore::DataStoreImportData * BioSignalMLDataStorePlugin::getImportData(const QString &pFileName,
                                                                           DataStore::DataStore *pImportDataStore,
                                                                           DataStore::DataStore *pResultsDataStore,
                                                                           const QList<quint64> &pRunSizes) const
{
    // Determine the number of variables in our BioSignalML file

    DataStore::DataStoreImportData *res = nullptr;

    try {
        auto recording = new bsml::HDF5::Recording(pFileName.toStdString(), true);

        res = new DataStore::DataStoreImportData(pFileName, pImportDataStore,
                                                 pResultsDataStore,
                                                 int(recording->get_signal_uris().size()),
                                                 recording->get_clock(recording->get_clock_uris().front())->read().size(),
                                                 pRunSizes);

        recording->close();

        delete recording;
    } catch (...) {
    }

    // Return some information about the data we want to import

    return res;
}

//==============================================================================

DataStore::DataStoreExportData * BioSignalMLDataStorePlugin::getExportData(const QString &pFileName,
                                                                           DataStore::DataStore *pDataStore,
                                                                           const QMap<int, QIcon> &pIcons) const
{
    // Ask which data should be exported, as well as some other information

    BiosignalmlDataStoreDialog biosignalmlDataStoreDialog(pDataStore, pIcons, Core::mainWindow());

    if (biosignalmlDataStoreDialog.exec() != 0) {
        // Now that we have the information we need, we can ask for the name of
        // the BioSignalML file where to do the export

        QStringList biosignalmlFilters = Core::filters({ fileTypeInterface() });
        QString firstBiosignalmlFilter = biosignalmlFilters.first();
        QString fileName = Core::getSaveFileName(tr("Export To BioSignalML"),
                                                 Core::newFileName(pFileName, tr("Data"), false, BiosignalmlFileExtension),
                                                 biosignalmlFilters,
                                                 &firstBiosignalmlFilter);

        if (!fileName.isEmpty()) {
            return new BiosignalmlDataStoreData(fileName,
                                                biosignalmlDataStoreDialog.name(),
                                                biosignalmlDataStoreDialog.author(),
                                                biosignalmlDataStoreDialog.description(),
                                                tr("Generated by %1 at %2 from %3.").arg(Core::version(),
                                                                                         QDateTime::currentDateTimeUtc().toString(Qt::ISODate),
                                                                                         pDataStore->uri()),
                                                pDataStore,
                                                biosignalmlDataStoreDialog.selectedData());
        }
    }

    return nullptr;
}

//==============================================================================

DataStore::DataStoreImporter * BioSignalMLDataStorePlugin::dataStoreImporterInstance() const
{
    // Return the 'global' instance of our BioSignalML data store importer

    static BiosignalmlDataStoreImporter instance;

    return static_cast<BiosignalmlDataStoreImporter *>(Core::globalInstance("OpenCOR::BioSignalMLDataStore::BiosignalmlDataStoreImporter::instance()",
                                                                            &instance));
}

//==============================================================================

DataStore::DataStoreExporter * BioSignalMLDataStorePlugin::dataStoreExporterInstance() const
{
    // Return the 'global' instance of our BioSignalML data store exporter

    static BiosignalmlDataStoreExporter instance;

    return static_cast<BiosignalmlDataStoreExporter *>(Core::globalInstance("OpenCOR::BioSignalMLDataStore::BiosignalmlDataStoreExporter::instance()",
                                                                            &instance));
}

//==============================================================================
// File interface
//==============================================================================

bool BioSignalMLDataStorePlugin::isFile(const QString &pFileName) const
{
    // Return whether the given file is of the type that we support

    bsml::HDF5::Recording *recording = nullptr;

    try {
        recording = new bsml::HDF5::Recording(pFileName.toStdString(), true);

        recording->close();

        delete recording;
    } catch (...) {
        // Something went wrong, so clearly not a BioSignalML file

        return false;
    }

    return true;
}

//==============================================================================

QString BioSignalMLDataStorePlugin::mimeType() const
{
    // Return the MIME type we support

    return BiosignalmlMimeType;
}

//==============================================================================

QString BioSignalMLDataStorePlugin::fileExtension() const
{
    // Return the extension of the type of file we support

    return BiosignalmlFileExtension;
}

//==============================================================================

QString BioSignalMLDataStorePlugin::fileTypeDescription() const
{
    // Return the description of the type of file we support

    return tr("BioSignalML File");
}

//==============================================================================

QStringList BioSignalMLDataStorePlugin::fileTypeDefaultViews() const
{
    // Return the default views to use for the type of file we support

    return {};
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

} // namespace BioSignalMLDataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

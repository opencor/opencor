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
// CSV data store importer
//==============================================================================

#include "corecliutils.h"
#include "csvdatastoreimporter.h"

//==============================================================================

#include <QFile>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

CsvDataStoreImporterWorker::CsvDataStoreImporterWorker(DataStore::DataStoreImportData *pImportData) :
    DataStore::DataStoreImporterWorker(pImportData)
{
}

//==============================================================================

void CsvDataStoreImporterWorker::run()
{
    // Import our CSV file in our data store
    // Note: we rely on our CSV file to be well-formed...

    QFile file(mImportData->fileName());
    QString errorMessage = QString();

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine(); // Header, which we ignore
        DataStore::DataStore *importDataStore = mImportData->importDataStore();
        double *importValues = mImportData->importValues();
        int nbOfVariables = mImportData->nbOfVariables();

        for (quint64 i = 0, iMax = mImportData->nbOfDataPoints(); i < iMax; ++i) {
            line = in.readLine().trimmed();

            QStringList fields = line.split(",");

            for (int j = 0; j < nbOfVariables; ++j) {
                importValues[j] = fields[j+1].toDouble();
            }

            importDataStore->addValues(fields[0].toDouble());

            emit progress(mImportData, mImportData->progress());
        }

        file.close();
    } else {
        errorMessage = tr("The file could not be opened.");
    }

    // Let people know that our import is done

    emit done(mImportData, errorMessage);
}

//==============================================================================

DataStore::DataStoreImporterWorker * CsvDataStoreImporter::workerInstance(DataStore::DataStoreImportData *pImportData)
{
    // Return an instance of our worker

    return new CsvDataStoreImporterWorker(pImportData);
}

//==============================================================================

} // namespace CSVDataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

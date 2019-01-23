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
    // Note: we rely on the fact that our CSV file is well-formed...

    QFile file(mImportData->fileName());
    QString errorMessage = QString();

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();   // Header, which we ignore
        DataStore::DataStore *dataStore = mImportData->dataStore();
        double *values = mImportData->values();
        int nbOfVariables = mImportData->nbOfVariables();
        quint64 nbOfDataPoints = mImportData->nbOfDataPoints();
        double oneOverNbOfDataPoints = 1.0/nbOfDataPoints;

        for (quint64 i = 1; i <= nbOfDataPoints; ++i) {
            line = in.readLine().trimmed();

            QStringList fields = line.split(",");

            for (int j = 0; j < nbOfVariables; ++j)
                values[j] = fields[j+1].toDouble();

            dataStore->addValues(fields[0].toDouble());

            emit progress(mImportData, i*oneOverNbOfDataPoints);
        }

        file.close();
    } else {
        errorMessage = tr("The file could not be opened.");
    }

    // Let people know that our import is done

    emit done(mImportData, QString());
}

//==============================================================================

DataStore::DataStoreImporterWorker * CsvDataStoreImporter::workerInstance(DataStore::DataStoreImportData *pImportData)
{
    // Return an instance of our worker

    return new CsvDataStoreImporterWorker(pImportData);
}

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

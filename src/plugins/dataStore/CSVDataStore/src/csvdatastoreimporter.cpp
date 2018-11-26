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

CsvDataStoreImporterWorker::CsvDataStoreImporterWorker(DataStore::DataStoreImportedData *pDataStoreImportedData) :
    DataStore::DataStoreImporterWorker(pDataStoreImportedData)
{
}

//==============================================================================

void CsvDataStoreImporterWorker::run()
{
    // Import our CSV file in our data store
    // Note: we rely on the fact that our CSV file is well-formed...

    QFile file(mDataStoreImportedData->fileName());
    QString errorMessage = QString();

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        // Determine the number of non-empty lines

        QTextStream in(&file);
        QString line;
        quint64 nbOfLines = 0;

        while (!in.atEnd()) {
            line = in.readLine().trimmed();

            if (!line.isEmpty())
                ++nbOfLines;
        }

        // Read our header line and set up our data store

        DataStore::DataStore *dataStore = mDataStoreImportedData->dataStore();

        double oneOverNbOfLines = 1.0/nbOfLines;

        in.seek(0);

        line = in.readLine().trimmed();

        int nbOfValues = line.split(",").count()-1;

        double *values = new double[nbOfValues] {};

        dataStore->addVariables(values, nbOfValues);

        emit progress(oneOverNbOfLines);

        // Add a run to our data store
        // Note: of capacity nbOfLines-1 because the first line is our header...

        if (dataStore->addRun(nbOfLines-1)) {
            // Read our data lines and have them stored in our data store

            for (quint64 i = 2; i <= nbOfLines; ++i) {
                line = in.readLine().trimmed();

                QStringList fields = line.split(",");

                for (int i = 1, iMax = fields.count(); i < iMax; ++i)
                    values[i-1] = fields[i].toDouble();

                dataStore->addValues(fields[0].toDouble());

                emit progress(i*oneOverNbOfLines);
            }
        } else {
            errorMessage = tr("The data could not be imported.");
        }

        delete[] values;

        file.close();
    }

    // Let people know that our import is done

    emit done(errorMessage);
}

//==============================================================================

DataStore::DataStoreImporterWorker * CsvDataStoreImporter::workerInstance(DataStore::DataStoreImportedData *pDataStoreImportedData)
{
    // Return an instance of our worker

    return new CsvDataStoreImporterWorker(pDataStoreImportedData);
}

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

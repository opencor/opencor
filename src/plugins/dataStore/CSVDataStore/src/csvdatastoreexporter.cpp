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
// CSV data store exporter
//==============================================================================

#include "corecliutils.h"
#include "csvdatastoreexporter.h"

//==============================================================================

#include <QDir>
#include <QSet>

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

CsvDataStoreExporterWorker::CsvDataStoreExporterWorker(DataStore::DataStoreExportData *pDataStoreData) :
    DataStore::DataStoreExporterWorker(pDataStoreData)
{
}

//==============================================================================

void CsvDataStoreExporterWorker::run()
{
    // Export our data store to a CSV file
    // Note: we would normally rely on a string to which we would append our
    //       header and then data, and then use that string as a parameter to
    //       Core::writeFile(). However, although this works fine with 'small'
    //       amounts of data to export, this can crash OpenCOR if we really have
    //       a lot of data to write. So, instead, we do what Core::writeFile()
    //       does, but rather than writing one potentially humongous string, we
    //       first write our header and then our data, one row at a time...

    QFile file(Core::temporaryFileName());
    QString errorMessage = QString();

    if (file.open(QIODevice::WriteOnly)) {
        // Determine whether we need to export the VOI and, if so, remove it
        // from our variables since it gets exported separately

        DataStore::DataStore *dataStore = mDataStoreData->dataStore();
        DataStore::DataStoreVariables variables = mDataStoreData->variables();
        DataStore::DataStoreVariable *voi = variables.contains(dataStore->voi())?dataStore->voi():nullptr;

        variables.removeOne(voi);

        // Retrieve the different values for our VOI and sets of variables
        // Note #1: this is needed when we have two runs with different
        //          starting/ending points and/or point intervals...
        // Note #2: after our for loop, our list may contain duplicates and may
        //          not be sorted. When it comes to removing duplicates, we do
        //          this by converting our list to a set and back. Indeed, this
        //          is much faster than preventing ourselves from adding
        //          duplicates in the first place...

        QList<DataStore::DataStoreVariables> variablesRuns = QList<DataStore::DataStoreVariables>();

        for (int i = 0, iMax = variables.count(); i < iMax; ++i) {
            variablesRuns << DataStore::DataStoreVariables();
        }

        int nbOfRuns = dataStore->runsCount();
        QList<quint64> runsIndex = QList<quint64>();
        QDoubleList voiValues = QDoubleList();

        for (int i = 0; i < nbOfRuns; ++i) {
            // Original index for the current run

            runsIndex << 0;

            // VOI values

            for (quint64 j = 0, jMax = dataStore->size(i); j < jMax; ++j) {
                voiValues << dataStore->voi()->value(j, i);
            }

            // Variables

            int j = -1;

            for (auto variable : variables) {
                variablesRuns[++j] << variable;
            }
        }

        voiValues = voiValues.toSet().toList();

        std::sort(voiValues.begin(), voiValues.end());

        // Determine the number of steps to export everything, i.e. one for our
        // header and then some for our number of VOI values

        double oneOverNbOfSteps = 1.0/(1+voiValues.count());
        int stepNb = 0;

        // Output our header

        static const QString Header = "%1 (%2)%3";
        static const QString RunNb  = " | Run #%1";
        static const QString CrLf   = "\r\n";

        QString header = QString();

        if (voi != nullptr) {
            header += Header.arg(voi->uri().replace("/prime", "'").replace('/', " | "),
                                 voi->unit(),
                                 QString());
        }

        for (auto variable : variables) {
            for (int i = 0; i < nbOfRuns; ++i) {
                if (!header.isEmpty()) {
                    header += ',';
                }

                header += Header.arg(variable->uri().replace("/prime", "'").replace('/', " | "),
                                     variable->unit(),
                                     (nbOfRuns == 1)?
                                         QString():
                                         RunNb.arg(i+1));
            }
        }

        header += CrLf;

        bool res = file.write(header.toUtf8()) != -1;

        // Output our different sets of data, one row at a time, if we were able
        // to output our header

        if (res) {
            emit progress(mDataStoreData, ++stepNb*oneOverNbOfSteps);

            for (int i = 0, iMax = voiValues.count(); i < iMax; ++i) {
                QString rowData = QString();
                double voiValue = voiValues[i];

                if (voi != nullptr) {
                    rowData += QString::number(voiValue);
                }

                bool firstRowData = true;
                QBoolList updateRunsIndex = QBoolList();

                for (int j = 0; j < nbOfRuns; ++j) {
                    updateRunsIndex << false;
                }

                for (const auto &variableRuns : variablesRuns) {
                    int j = 0;

                    for (auto variableRun : variableRuns) {
                        if (firstRowData && rowData.isEmpty()) {
                            firstRowData = false;
                        } else {
                            rowData += ',';
                        }

                        if (   (runsIndex[j] < dataStore->size(j))
                            && qFuzzyCompare(dataStore->voi()->value(runsIndex[j], j), voiValue)) {
                            rowData += QString::number(variableRun->value(runsIndex[j], j));

                            updateRunsIndex[j] = true;
                        }

                        ++j;
                    }
                }

                for (int j = 0; j < nbOfRuns; ++j) {
                    if (updateRunsIndex[j]) {
                        ++runsIndex[j];
                    }
                }

                rowData += CrLf;

                res = file.write(rowData.toUtf8()) != -1;

                if (!res) {
                    break;
                }

                emit progress(mDataStoreData, ++stepNb*oneOverNbOfSteps);
            }
        }

        // Close our temporary file and rename it to our final file, if we were
        // able to output all of our data

        file.close();

        if (res) {
            QDir dir(QFileInfo(mDataStoreData->fileName()).path());

            res = dir.exists() || dir.mkpath(dir.dirName());

            if (res) {
                if (QFile::exists(mDataStoreData->fileName())) {
                    QFile::remove(mDataStoreData->fileName());
                }

                res = file.rename(mDataStoreData->fileName());
            }
        }

        if (!res) {
            file.remove();

            errorMessage = tr("The data could not be written.");
        }
    } else {
        errorMessage = tr("The CSV file could not be created.");
    }

    // Let people know that our export is done

    emit done(mDataStoreData, errorMessage);
}

//==============================================================================

DataStore::DataStoreExporterWorker * CsvDataStoreExporter::workerInstance(DataStore::DataStoreExportData *pDataStoreData)
{
    // Return an instance of our worker

    return new CsvDataStoreExporterWorker(pDataStoreData);
}

//==============================================================================

} // namespace CSVDataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

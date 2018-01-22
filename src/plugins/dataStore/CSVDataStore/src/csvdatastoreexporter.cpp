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

CsvDataStoreExporter::CsvDataStoreExporter(DataStore::DataStoreData *pDataStoreData) :
    DataStore::DataStoreExporter(pDataStoreData)
{
}

//==============================================================================

void CsvDataStoreExporter::execute(QString &pErrorMessage) const
{
    // Determine what should be exported

    DataStore::DataStores dataStores = mDataStoreData->dataStores();
    int nbOfRuns = dataStores.count();
    DataStore::DataStoreVariables selectedVariables = mDataStoreData->selectedVariables();
    DataStore::DataStoreVariable *voi = selectedVariables.contains(dataStores.last()->voi())?dataStores.last()->voi():0;

    selectedVariables.removeOne(voi);

    // Do the export itself
    // Note: we would normally rely on a string to which we would append our
    //       header and then data, and then use that string as a parameter to
    //       Core::writeFileContentsToFile(). However, although this works fine
    //       with 'small' amounts of data to export, this can crash OpenCOR if
    //       we really have a lot of data to write. So, instead, we do what
    //       Core::writeFileContentsToFile() does, but rather than writing one
    //       potentially humongous string, we first write our header and then
    //       our data, one row at a time...

    QFile file(Core::temporaryFileName());

    if (file.open(QIODevice::WriteOnly)) {
        // Output our header

        static const QString Header = "%1 (%2)%3";
        static const QString RunNb = " | Run #%1";

        QString header = QString();

        if (voi) {
            header += Header.arg(voi->uri().replace("/prime", "'").replace('/', " | "),
                                 voi->unit(), QString());
        }

        foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
            for (int i = 0; i < nbOfRuns; ++i) {
                if (!header.isEmpty())
                    header += ',';

                header += Header.arg(selectedVariable->uri().replace("/prime", "'").replace('/', " | "),
                                     selectedVariable->unit(),
                                     (nbOfRuns == 1)?
                                         QString():
                                         RunNb.arg(i+1));
            }
        }

        header += '\n';

        bool res = file.write(header.toUtf8()) != -1;

        // Output our different sets of data, one row at a time, if we were able
        // to output our header

        if (res) {
            // Retrieve our different VOI values and sets of selected variables
            // Note #1: this is needed when we have two runs with different
            //          starting/ending points and/or point intervals...
            // Note #2: after our for loop, our list may contain duplicates and
            //          may not be sorted. When it comes to removing duplicates,
            //          we do this by converting our list to a set and back.
            //          Indeed, this is much faster than preventing ourselves
            //          from adding duplicates in the first place...

            QList<qulonglong> dataStoresIndex = QList<qulonglong>();
            QDoubleList voiValues = QDoubleList();
            QStringList selectedVariablesUri = QStringList();
            QList<DataStore::DataStoreVariables> selectedVariablesDataStores = QList<DataStore::DataStoreVariables>();

            foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
                selectedVariablesUri << selectedVariable->uri();

                selectedVariablesDataStores << DataStore::DataStoreVariables();
            }

            for (int i = 0; i < nbOfRuns; ++i) {
                // Original index

                dataStoresIndex << 0;

                // VOI values

                DataStore::DataStore *dataStore = dataStores[i];

                for (qulonglong j = 0, jMax = dataStore->size(); j < jMax; ++j)
                    voiValues << dataStore->voi()->value(j);

                // Selected variables

                int j = 0;

                foreach (DataStore::DataStoreVariable *variable, dataStore->variables()) {
                    if (selectedVariablesUri.contains(variable->uri())) {
                        selectedVariablesDataStores[j] << variable;

                        ++j;
                    }
                }
            }

            voiValues = voiValues.toSet().toList();

            std::sort(voiValues.begin(), voiValues.end());

            // Now, we can output our data

            for (qulonglong i = 0, iMax = voiValues.count(); i < iMax; ++i) {
                QString rowData = QString();
                double voiValue = voiValues[i];

                if (voi)
                    rowData += QString::number(voiValue);

                bool firstRowData = true;
                QBoolList updateDataStoresIndex = QBoolList();

                for (int j = 0; j < nbOfRuns; ++j)
                    updateDataStoresIndex << false;

                foreach (const DataStore::DataStoreVariables &selectedVariableDataStores, selectedVariablesDataStores) {
                    int j = 0;

                    foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariableDataStores) {
                        if (firstRowData && rowData.isEmpty())
                            firstRowData = false;
                        else
                            rowData += ',';

                        if (dataStores[j]->voi()->values()[dataStoresIndex[j]] == voiValue) {
                            rowData += QString::number(selectedVariable->value(dataStoresIndex[j]));

                            updateDataStoresIndex[j] = true;
                        }

                        ++j;
                    }
                }

                for (int j = 0; j < nbOfRuns; ++j) {
                    if (updateDataStoresIndex[j])
                        ++dataStoresIndex[j];
                }

                rowData += "\n";

                res = file.write(rowData.toUtf8()) != -1;

                if (!res)
                    break;

                emit progress(double(i)/(iMax-1));
            }
        }

        // Close our temporary file and rename it to our final file, if we were
        // able to output all of our data

        file.close();

        if (res) {
            QDir dir(QFileInfo(mDataStoreData->fileName()).path());

            res = dir.exists() || dir.mkpath(dir.dirName());

            if (res) {
                if (QFile::exists(mDataStoreData->fileName()))
                    QFile::remove(mDataStoreData->fileName());

                res = file.rename(mDataStoreData->fileName());
            }
        }

        if (!res) {
            file.remove();

            pErrorMessage = tr("The data could not be exported to CSV.");
        }
    } else {
        pErrorMessage = tr("The CSV file could not be created.");
    }
}

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

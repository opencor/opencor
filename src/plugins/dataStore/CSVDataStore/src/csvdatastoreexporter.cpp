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
// CSV data store exporter
//==============================================================================

#include "corecliutils.h"
#include "csvdatastoreexporter.h"

//==============================================================================

#include <QCoreApplication>

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

CsvDataStoreExporter::CsvDataStoreExporter(const QString &pFileName,
                                           DataStore::DataStore *pDataStore,
                                           DataStore::DataStoreData *pDataStoreData) :
    DataStore::DataStoreExporter(pFileName, pDataStore, pDataStoreData)
{
}

//==============================================================================

void CsvDataStoreExporter::execute(QString &pErrorMessage) const
{
    Q_UNUSED(pErrorMessage);

    // Determine what should be exported

    DataStore::DataStoreVariable *voi = mDataStoreData->selectedVariables().contains(mDataStore->voi())?mDataStore->voi():0;
    DataStore::DataStoreVariables selectedVariables = mDataStoreData->selectedVariables();

    selectedVariables.removeOne(voi);

    // Header

    static const QString Header = "%1 (%2)";

    QString data = QString();

    if (voi) {
        data += Header.arg(voi->uri().replace("/prime", "'").replace("/", " | "),
                           voi->unit());
    }

    foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
        if (!data.isEmpty())
            data += ",";

        data += Header.arg(selectedVariable->uri().replace("/prime", "'").replace("/", " | "),
                           selectedVariable->unit());
    }

    data += "\n";

    // Data itself

    for (qulonglong i = 0, iMax = mDataStore->size(); i < iMax; ++i) {
        QString rowData = QString();

        if (voi)
            rowData += QString::number(voi->value(i));

        foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
            if (!rowData.isEmpty())
                rowData += ",";

            rowData += QString::number(selectedVariable->value(i));
        }

        data += rowData+"\n";

        emit progress(double(i)/(iMax-1));
    }

    // The data is ready, so write it to the file

    Core::writeFileContentsToFile(mDataStoreData->fileName(), data);
}

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
    DataStore::DataStoreVariables variables = DataStore::DataStoreVariables();

    foreach (DataStore::DataStoreVariable *variable, mDataStore->variables()) {
        if (mDataStoreData->selectedVariables().contains(variable))
            variables << variable;
    }

    // Header

    static const QString Header = "%1 (%2)";

    QByteArray data = QByteArray();

    if (voi) {
        data += Header.arg(voi->uri().replace("/prime", "'").replace("/", " | "),
                           voi->unit());
    }

    foreach (DataStore::DataStoreVariable *variable, variables) {
        if (!data.isEmpty())
            data += ",";

        data += Header.arg(variable->uri().replace("/prime", "'").replace("/", " | "),
                           variable->unit());
    }

    data += "\n";

    // Data itself

    for (qulonglong i = 0; i < mDataStore->size(); ++i) {
        QString rowData = QString();

        if (voi)
            rowData += QString::number(voi->value(i));

        foreach (DataStore::DataStoreVariable *variable, variables) {
            if (!rowData.isEmpty())
                rowData += ",";

            rowData += QString::number(variable->value(i));
        }

        data += rowData+"\n";

        emit progress(double(i)/(mDataStore->size()-1));
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

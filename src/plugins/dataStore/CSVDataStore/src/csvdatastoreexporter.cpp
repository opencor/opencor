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
        data += Header.arg(voi->uri().replace("/prime", "'").replace('/', " | "),
                           voi->unit());
    }

    foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
        if (!data.isEmpty())
            data += ',';

        data += Header.arg(selectedVariable->uri().replace("/prime", "'").replace('/', " | "),
                           selectedVariable->unit());
    }

    data += '\n';

    // Data itself

    for (qulonglong i = 0, iMax = mDataStore->size(); i < iMax; ++i) {
        QString rowData = QString();

        if (voi)
            rowData += QString::number(voi->value(i));

        foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
            if (!rowData.isEmpty())
                rowData += ',';

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

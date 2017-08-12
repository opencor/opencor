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
    // Determine what should be exported

    DataStore::DataStoreVariable *voi = mDataStoreData->selectedVariables().contains(mDataStore->voi())?mDataStore->voi():0;
    DataStore::DataStoreVariables selectedVariables = mDataStoreData->selectedVariables();

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

        static const QString Header = "%1 (%2)";

        QString header = QString();

        if (voi) {
            header += Header.arg(voi->uri().replace("/prime", "'").replace('/', " | "),
                               voi->unit());
        }

        foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
            if (!header.isEmpty())
                header += ',';

            header += Header.arg(selectedVariable->uri().replace("/prime", "'").replace('/', " | "),
                               selectedVariable->unit());
        }

        header += '\n';

        bool res = file.write(header.toUtf8()) != -1;

        // Output our data, one row at a time, if we were able to output our
        // header

        if (res) {
            for (qulonglong i = 0, iMax = mDataStore->size(); i < iMax; ++i) {
                QString rowData = QString();

                if (voi)
                    rowData += QString::number(voi->value(i));

                foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
                    if (!rowData.isEmpty())
                        rowData += ',';

                    rowData += QString::number(selectedVariable->value(i));
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

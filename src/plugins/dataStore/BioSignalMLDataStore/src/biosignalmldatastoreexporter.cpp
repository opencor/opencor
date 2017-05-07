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
// BioSignalML data store exporter
//==============================================================================

#include "biosignalmldatastoredata.h"
#include "biosignalmldatastoreexporter.h"

//==============================================================================

#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QUrl>

//==============================================================================

#include "biosignalml/biosignalml.h"
#include "biosignalml/data/hdf5.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BiosignalmlDataStoreExporter::BiosignalmlDataStoreExporter(const QString &pFileName,
                                                           DataStore::DataStore *pDataStore,
                                                           DataStore::DataStoreData *pDataStoreData) :
    DataStore::DataStoreExporter(pFileName, pDataStore, pDataStoreData)
{
}

//==============================================================================

void BiosignalmlDataStoreExporter::execute(QString &pErrorMessage) const
{
    // Export the given data store to a BioSignalML file

    BiosignalmlDataStoreData *dataStoreData = static_cast<BiosignalmlDataStoreData *>(mDataStoreData);
    QString fileName = dataStoreData->fileName();
    bsml::HDF5::Recording *recording = 0;

    try {
        // Create and populate a recording
        // Note: although a variable of integration may not officially be
        //       visible to a user, we still need to export it since it's needed
        //       by BioSignalML...

        DataStore::DataStoreVariable *voi = mDataStore->voi();
        std::string recordingUri = QUrl::fromLocalFile(fileName).toEncoded().toStdString();
        std::string baseUnits = mDataStore->uri().toStdString()+"/units#";

        recording = new bsml::HDF5::Recording(recordingUri, fileName.toStdString(), true);

        recording->add_prefix(rdf::Namespace("units", baseUnits));

        recording->set_label(dataStoreData->name().toStdString());
        recording->set_investigator(rdf::Literal(dataStoreData->author().toStdString()));
        recording->set_description(dataStoreData->description().toStdString());
        recording->set_comment(dataStoreData->comment().toStdString());
        recording->set_duration(xsd::Duration(voi->value(voi->size()-1)-voi->value(0),
                                              voi->unit().toStdString()));

        // Create and populate a clock

        bsml::HDF5::Clock::Ptr clock = recording->new_clock(recordingUri+"/clock/"+voi->uri().toStdString(),
                                                            rdf::URI(baseUnits+voi->unit().toStdString()),
                                                            voi->values(),
                                                            voi->size());

        clock->set_label(voi->label().toStdString());

        // Determine what should be exported (minus the variable of integration
        // which should always be exported in the case of a BioSignalML file)

        DataStore::DataStoreVariables selectedVariables = mDataStoreData->selectedVariables();

        selectedVariables.removeOne(mDataStore->voi());

        // Retrieve some information about the different variables that are to
        // be exported

        std::vector<std::string> uris;
        std::vector<rdf::URI> units;

        foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables) {
            uris.push_back(recordingUri+"/signal/"+selectedVariable->uri().toStdString());
            units.push_back(rdf::URI(baseUnits+selectedVariable->unit().toStdString()));
        }

        // Create and populate a signal array

        enum {
            BufferRows = 50000
        };

        bsml::HDF5::SignalArray::Ptr signalArray = recording->new_signalarray(uris, units, clock);
        int i = -1;

        foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables)
            (*signalArray)[++i]->set_label(selectedVariable->label().toStdString());

        double *data = new double[selectedVariables.count()*BufferRows];
        double *dataPointer = data;
        int rowCount = 0;

        for (qulonglong i = 0, iMax = mDataStore->size(); i < iMax; ++i) {
            foreach (DataStore::DataStoreVariable *selectedVariable, selectedVariables)
                *dataPointer++ = selectedVariable->value(i);

            ++rowCount;

            if (rowCount >= BufferRows) {
                signalArray->extend(data, selectedVariables.count()*BufferRows);
                dataPointer = data;
                rowCount = 0;
            }

            emit progress(double(i)/(iMax-1));
        }

        signalArray->extend(data, selectedVariables.count()*rowCount);

        delete[] data;
    } catch (bsml::data::Exception e) {
        // Something went wrong, so retrieve the error message and delete our
        // BioSignalML file

        pErrorMessage = QObject::tr("The data could not be exported to BioSignalML (%1).").arg(e.what());

        QFile::remove(fileName);
    }

    // Close and delete our recording, if any

    if (recording) {
        recording->close();

        delete recording;
    }
}

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
    std::string recordingUri = QUrl::fromLocalFile(fileName).toEncoded().toStdString();
    std::string baseUnits = mDataStore->uri().toStdString()+"/units#";
    bsml::HDF5::Recording *recording = 0;

    try {
        // Create and populate a recording

        DataStore::DataStoreVariable *voi = mDataStore->voi();
        recording = new bsml::HDF5::Recording(recordingUri, fileName.toStdString(), true);

        recording->add_prefix(rdf::Namespace("units", baseUnits));

        recording->set_comment(dataStoreData->comment().toStdString());
        recording->set_description(dataStoreData->description().toStdString());
        recording->set_duration(xsd::Duration(voi->value(voi->size()-1)-voi->value(0),
                                              voi->unit().toStdString()));
        recording->set_investigator(rdf::Literal(dataStoreData->author().toStdString()));
        recording->set_label(dataStoreData->shortName().toStdString());

        // Create and poluate a clock

        bsml::HDF5::Clock::Ptr clock = recording->new_clock(recordingUri+"/clock/"+voi->uri().toStdString(),
                                                            rdf::URI(baseUnits+voi->unit().toStdString()),
                                                            voi->values(),
                                                            voi->size());

        clock->set_label(voi->label().toStdString());

        // Retrieve some information about the different variables that are to
        // be exported

        std::vector<std::string> uris;
        std::vector<rdf::URI> units;
        std::vector<int> indexes;
        DataStore::DataStoreVariables variables = mDataStore->variables();
        int nbOfVariables = variables.size();

        for (int i = 0;  i < nbOfVariables; ++i) {
            if (dataStoreData->selectedVariables()[i]) {
                uris.push_back(recordingUri+"/signal/"+variables[i]->uri().toStdString());
                units.push_back(rdf::URI(baseUnits+variables[i]->unit().toStdString()));
                indexes.push_back(i);
            }
        }

        // Create and populate a signal array

        static const int BufferRows = 50000;

        bsml::HDF5::SignalArray::Ptr signalArray = recording->new_signalarray(uris, units, clock);
        nbOfVariables = signalArray->size();

        for (int i = 0;  i < nbOfVariables;  ++i)
            (*signalArray)[i]->set_label(variables[indexes[i]]->label().toStdString());

        double *data = new double[nbOfVariables*BufferRows];
        double *dataPointer = data;
        int rowcount = 0;

        for (qulonglong i = 0; i < mDataStore->size(); ++i) {
            for (int j = 0; j < nbOfVariables; ++j)
                *dataPointer++ = variables[indexes[j]]->value(i);

            ++rowcount;

            if (rowcount >= BufferRows) {
                signalArray->extend(data, BufferRows*nbOfVariables);
                dataPointer = data;
                rowcount = 0;
            }

            emit progress(double(i)/(mDataStore->size()-1));
        }

        signalArray->extend(data, nbOfVariables*rowcount);

        delete[] data;
    } catch (bsml::data::Exception e) {
        // Something went wrong, so retrieve the error message and delete our
        // BioSignalML file

        pErrorMessage = QObject::tr("The simulation data could not be exported to BioSignalML (%1).").arg(e.what());

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

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
// BioSignalML data store exporter
//==============================================================================

#include "biosignalmldatastoredata.h"
#include "biosignalmldatastoreexporter.h"

//==============================================================================

#include <QFile>
#include <QUrl>

//==============================================================================

#include "libbiosignalmlbegin.h"
    #include "biosignalml/data/hdf5.h"
#include "libbiosignalmlend.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BiosignalmlDataStoreExporterWorker::BiosignalmlDataStoreExporterWorker(DataStore::DataStoreExportData *pDataStoreData) :
    DataStore::DataStoreExporterWorker(pDataStoreData)
{
}

//==============================================================================

void BiosignalmlDataStoreExporterWorker::run()
{
    // Determine the number of steps to export everything

    auto dataStoreData = static_cast<BiosignalmlDataStoreData *>(mDataStoreData);
    DataStore::DataStore *dataStore = dataStoreData->dataStore();
    int nbOfRuns = dataStore->runsCount();
    int nbOfSteps = 0;

    for (int i = 0; i < nbOfRuns; ++i) {
        nbOfSteps += dataStore->size(i);
    }

    double oneOverNbOfSteps = 1.0/nbOfSteps;
    int stepNb = 0;

    // Export our data store to a BioSignalML file

    bsml::HDF5::Recording *recording = nullptr;
    QString errorMessage;

    try {
        // Create and populate a recording
        // Note: although a VOI may not officially be visible to a user, we
        //       still need to export it since it's needed by BioSignalML...

        DataStore::DataStoreVariable *voi = dataStore->voi();
        std::string recordingUri = QUrl::fromLocalFile(dataStoreData->fileName()).toEncoded().toStdString();
        std::string baseUnits = dataStore->uri().toStdString()+"/units#";

        recording = new bsml::HDF5::Recording(recordingUri, dataStoreData->fileName().toStdString(), true);

        recording->add_prefix(rdf::Namespace("units", baseUnits));

        recording->set_label(dataStoreData->name().toStdString());
        recording->set_investigator(rdf::Literal(dataStoreData->author().toStdString()));
        recording->set_description(dataStoreData->description().toStdString());
        recording->set_comment(dataStoreData->comment().toStdString());

        for (int i = 0; i < nbOfRuns; ++i) {
            // Create and populate a clock

            std::string runNb = (nbOfRuns == 1)?"":"/"+QString::number(i+1).toStdString();
            bsml::HDF5::Clock::Ptr clock = recording->new_clock(std::string().append(recordingUri).append("/clock/").append(voi->uri().toStdString()).append(runNb),
                                                                rdf::URI(baseUnits+voi->unit().toStdString()),
                                                                voi->values(i),
                                                                voi->size(i));

            clock->set_label(voi->name().toStdString());

            // Determine what should be exported (minus the VOI, which should
            // always be exported in the case of a BioSignalML file)

            DataStore::DataStoreVariables variables = mDataStoreData->variables();

            variables.removeOne(dataStore->voi());

            // Retrieve some information about the different variables that are
            // to be exported

            std::vector<std::string> uris;
            std::vector<rdf::URI> units;

            for (auto variable : variables) {
                uris.emplace_back(std::string().append(recordingUri).append("/signal/").append(variable->uri().toStdString()).append(runNb));
                units.emplace_back(rdf::URI(baseUnits+variable->unit().toStdString()));
            }

            // Create and populate a signal array

            enum {
                BufferRows = 50000
            };

            bsml::HDF5::SignalArray::Ptr signalArray = recording->new_signalarray(uris, units, clock);
            bsml::HDF5::SignalArray::size_type n = 0;

            for (auto variable : variables) {
                (*signalArray)[n]->set_label(variable->name().toStdString());

                ++n;
            }

            auto data = new double[quint64(variables.count()*BufferRows)] {};
            double *dataPointer = data;
            size_t rowCount = 0;

            for (quint64 j = 0, jMax = dataStore->size(i); j < jMax; ++j) {
                for (auto variable : variables) {
                    *dataPointer++ = variable->value(j, i);
                }

                ++rowCount;

                if (rowCount >= BufferRows) {
                    signalArray->extend(data, size_t(variables.count())*BufferRows);

                    dataPointer = data;

                    rowCount = 0;
                }

                emit progress(mDataStoreData, ++stepNb*oneOverNbOfSteps);
            }

            signalArray->extend(data, size_t(variables.count())*rowCount);

            delete[] data;
        }
    } catch (bsml::data::Exception &exception) {
        // Something went wrong, so retrieve the error message and delete our
        // BioSignalML file

        errorMessage = tr("The data could not be exported to BioSignalML (%1).").arg(exception.what());

        QFile::remove(dataStoreData->fileName());
    }

    // Close and delete our recording, if any

    if (recording != nullptr) {
        recording->close();

        delete recording;
    }

    // Let people know that our export is done

    emit done(mDataStoreData, errorMessage);
}

//==============================================================================

DataStore::DataStoreExporterWorker * BiosignalmlDataStoreExporter::workerInstance(DataStore::DataStoreExportData *pDataStoreData)
{
    // Return an instance of our worker

    return new BiosignalmlDataStoreExporterWorker(pDataStoreData);
}

//==============================================================================

} // namespace BioSignalMLDataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

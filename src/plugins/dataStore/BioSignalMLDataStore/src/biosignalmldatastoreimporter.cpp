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
// BioSignalML data store importer
//==============================================================================

#include "biosignalmldatastoreimporter.h"

//==============================================================================

#include "libbiosignalmlbegin.h"
    #include "biosignalml/data/hdf5.h"
#include "libbiosignalmlend.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BiosignalmlDataStoreImporterWorker::BiosignalmlDataStoreImporterWorker(DataStore::DataStoreImportData *pImportData) :
    DataStore::DataStoreImporterWorker(pImportData)
{
}

//==============================================================================

void BiosignalmlDataStoreImporterWorker::run()
{
    // Import our BioSignalML file in our data store

    QString errorMessage;

    try {
        // Retrieve our clock and determine our number of data points

        auto recording = new bsml::HDF5::Recording(mImportData->fileName().toStdString(), true);
        std::vector<double> clockTicks = recording->get_clock(recording->get_clock_uris().front())->read();

        // Retrieve the values of our different variables

        int nbOfVariables = mImportData->nbOfVariables();
        auto variables = new bsml::data::TimeSeries::Ptr[nbOfVariables] {};
        bsml::data::TimeSeries::Ptr *variable = variables-1;

        for (const auto &signalUri : recording->get_signal_uris()) {
            *(++variable) = recording->get_signal(signalUri)->read();
        }

        // Add a run to our data store and store the values of our different
        // variables to it

        DataStore::DataStore *importDataStore = mImportData->importDataStore();
        double *importValues = mImportData->importValues();

        for (quint64 i = 0, iMax = mImportData->nbOfDataPoints(); i < iMax; ++i) {
            for (int j = 0; j < nbOfVariables; ++j) {
                importValues[j] = variables[j]->data()[i];
            }

            importDataStore->addValues(clockTicks[i]);

            emit progress(mImportData, mImportData->progress());
        }

        delete[] variables;

        recording->close();

        delete recording;
    } catch (bsml::data::Exception &exception) {
        // Something went wrong, so retrieve the error message

        errorMessage = tr("The data could not be imported from BioSignalML (%1).").arg(exception.what());
    }

    // Let people know that our import is done

    emit done(mImportData, errorMessage);
}

//==============================================================================

DataStore::DataStoreImporterWorker * BiosignalmlDataStoreImporter::workerInstance(DataStore::DataStoreImportData *pImportData)
{
    // Return an instance of our worker

    return new BiosignalmlDataStoreImporterWorker(pImportData);
}

//==============================================================================

} // namespace BioSignalMLDataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

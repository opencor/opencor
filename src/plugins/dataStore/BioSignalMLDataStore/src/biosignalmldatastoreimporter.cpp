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
// BioSignalML data store importer
//==============================================================================

#include "biosignalmldatastoreimporter.h"

//==============================================================================

#include "biosignalml/data/hdf5.h"

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

    QString errorMessage = QString();

    try {
        // Retrieve our clock and determine our number of data points

        bsml::HDF5::Recording *recording = new bsml::HDF5::Recording(mImportData->fileName().toStdString());
        std::vector<double> clockTicks = recording->get_clock(recording->get_clock_uris().front())->read();
        quint64 nbOfDataPoints = clockTicks.size();

        // Retrieve the values of our different variables

        std::list<rdf::URI> signalUris = recording->get_signal_uris();
        int nbOfVariables = int(signalUris.size());
        bsml::data::TimeSeries::Ptr *variables = new bsml::data::TimeSeries::Ptr[nbOfVariables] {};
        bsml::data::TimeSeries::Ptr *variable = variables-1;

        for (auto signalUri : signalUris)
            *(++variable) = recording->get_signal(signalUri)->read();

        // Set up our data store

        DataStore::DataStore *dataStore = mImportData->dataStore();
        double *values = new double[nbOfVariables] {};

        dataStore->addVariables(values, nbOfVariables);

        // Add a run to our data store and store the values of our different
        // variables to it

        double oneOverNbOfDataPoints = 1.0/nbOfDataPoints;

        if (dataStore->addRun(nbOfDataPoints)) {
            for (quint64 i = 1; i <= nbOfDataPoints; ++i) {
                for (int j = 0; j < nbOfVariables; ++j)
                    values[j] = variables[j]->data()[i-1];

                dataStore->addValues(clockTicks[i-1]);

                emit progress(mImportData, i*oneOverNbOfDataPoints);
            }
        } else {
            errorMessage = tr("The memory needed to store the data could not be allocated.");
        }

        delete[] values;
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

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

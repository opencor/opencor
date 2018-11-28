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
//---ISSUE1845--- TO BE DONE...
    // Let people know that our import is done

    emit done(mImportData, QString());
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

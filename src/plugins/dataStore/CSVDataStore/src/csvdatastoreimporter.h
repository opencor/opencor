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
// CSV data store importer
//==============================================================================

#pragma once

//==============================================================================

#include "datastoreinterface.h"

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

class CsvDataStoreImporterWorker : public DataStore::DataStoreImporterWorker
{
    Q_OBJECT

public:
    explicit CsvDataStoreImporterWorker(DataStore::DataStoreImportData *pImportData);

public slots:
    void run() override;
};

//==============================================================================

class CsvDataStoreImporter : public DataStore::DataStoreImporter
{
    Q_OBJECT

protected:
    DataStore::DataStoreImporterWorker * workerInstance(DataStore::DataStoreImportData *pImportData) override;
};

//==============================================================================

} // namespace CSVDataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

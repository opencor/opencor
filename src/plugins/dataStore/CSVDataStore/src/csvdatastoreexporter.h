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
// CSV data store exporter class
//==============================================================================

#pragma once

//==============================================================================

#include "csvdatastoreglobal.h"
#include "datastoreinterface.h"

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

class CSVDATASTORE_EXPORT CsvDataStoreExporter : public DataStore::DataStoreExporter
{
public:
    virtual void execute(const QString &pFileName,
                         DataStore::DataStore *pDataStore) const;
};

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

#pragma once

//==============================================================================

#include "datastoreinterface.h"

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

class CsvDataStoreExporter : public DataStore::DataStoreExporter
{
public:
    explicit CsvDataStoreExporter(const QString &pFileName,
                                  DataStore::DataStore *pDataStore,
                                  DataStore::DataStoreData *pDataStoreData);

    virtual void execute(QString &pErrorMessage) const;
};

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

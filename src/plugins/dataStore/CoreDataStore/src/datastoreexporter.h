/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Core data store exporter class
//==============================================================================

#ifndef DATASTOREEXPORTER_H
#define DATASTOREEXPORTER_H

//==============================================================================

#include "coredatastore.h"
#include "coredatastoreglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CoreDataStore {

//==============================================================================

class COREDATASTORE_EXPORT DataStoreExporter
{
public:
    explicit DataStoreExporter();
    virtual ~DataStoreExporter();

    virtual void execute(CoreDataStore *pDataStore) const = 0;
};

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

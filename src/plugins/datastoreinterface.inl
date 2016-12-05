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
// Data store interface
//==============================================================================

#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    // Note: make sure to update dataStoreInterfaceVersion() whenever you update
    //       this interface...

    virtual QString dataStoreName() const PURE;

    virtual DataStore::DataStoreData * getData(const QString &pFileName,
                                               DataStore::DataStore *pDataStore) const PURE;

    virtual DataStore::DataStoreExporter * dataStoreExporterInstance(const QString &pFileName,
                                                                     DataStore::DataStore *pDataStore,
                                                                     DataStore::DataStoreData *pDataStoreData) const PURE;

#undef PURE

//==============================================================================
// End of file
//==============================================================================

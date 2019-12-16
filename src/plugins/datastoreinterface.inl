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
// Data store interface
//==============================================================================

#include "interfacebegin.h"
    // Note: make sure to update dataStoreInterfaceVersion() whenever you update
    //       this interface...

    VIRTUAL QString dataStoreName() const PURE_OR_OVERRIDE;

    VIRTUAL DataStore::DataStoreImportData * getImportData(const QString &pFileName,
                                                           DataStore::DataStore *pImportDataStore,
                                                           DataStore::DataStore *pResultsDataStore,
                                                           const QList<quint64> &pRunSizes) const PURE_OR_OVERRIDE;
    VIRTUAL DataStore::DataStoreExportData * getExportData(const QString &pFileName,
                                                           DataStore::DataStore *pDataStore,
                                                           const QMap<int, QIcon> &pIcons) const PURE_OR_OVERRIDE;

    VIRTUAL DataStore::DataStoreImporter * dataStoreImporterInstance() const PURE_OR_OVERRIDE;
    VIRTUAL DataStore::DataStoreExporter * dataStoreExporterInstance() const PURE_OR_OVERRIDE;
#include "interfaceend.h"

//==============================================================================
// End of file
//==============================================================================

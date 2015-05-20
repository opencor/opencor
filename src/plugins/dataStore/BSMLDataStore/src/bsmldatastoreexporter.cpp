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
// BioSignalML data store exporter class
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "coredatastore.h"
#include "bsmldatastoreexporter.h"
#include "datastorevariable.h"

//==============================================================================

#include <QApplication>
#include <QFile>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace BSMLDataStore {

//==============================================================================

void BioSignalMLExporter::execute(CoreDataStore::CoreDataStore *pDataStore) const
{
    // Export the given data store to a BioSignalML file

    QString fileName = Core::getSaveFileName(QObject::tr("Export to a BioSignalML file"),
                                             QString(),
                                             QObject::tr("BioSignalML File")+" (*.h5)");

    if (!fileName.isEmpty()) {

        Q_UNUSED(pDataStore);

    }
}

//==============================================================================

}   // namespace BSMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

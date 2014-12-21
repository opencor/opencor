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
// CSV data store class
//==============================================================================

#include "corecliutils.h"
#include "coredatastore.h"
#include "csvdatastore.h"
#include "datastorevariable.h"

//==============================================================================

#include <QApplication>
#include <QFile>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

bool exportDataStore(CoreDataStore::CoreDataStore *pDataStore,
                     const QString &pFileName)
{
    // Export the given data store to a CSV file with the given file name

    QString data = QString();

    // Header

    static const QString Header = "%1 (%2)";

    CoreDataStore::DataStoreVariable *voi = pDataStore->voi();
    CoreDataStore::DataStoreVariables variables = pDataStore->variables();

    data += Header.arg(voi->uri().replace("/prime", "'").replace("/", " | "),
                       voi->unit());

    auto variableBegin = variables.begin();
    auto variableEnd = variables.end();

    for (auto variable = variableBegin; variable != variableEnd; ++variable)
        data += ","+Header.arg((*variable)->uri().replace("/prime", "'").replace("/", " | "),
                               (*variable)->unit());

    data += "\n";

    // Data itself

   for (qulonglong i = 0; i < pDataStore->size(); ++i) {
       data += QString::number(voi->value(i));

       for (auto variable = variableBegin; variable != variableEnd; ++variable)
           data += ","+QString::number((*variable)->value(i));

       data += "\n";

       qApp->processEvents();
//---GRY--- THE CALL TO qApp->processEvents() IS TEMPORARY, I.E. UNTIL WE HAVE
//          IMPLEMENTED ISSUE #451 (see https://github.com/opencor/opencor/issues/451)
//          AND THEREFORE HAVE THOSE DATA STORES DO THE EXPORT IN THEIR OWN
//          THREAD THUS NOT BLOCKING THE MAIN THREAD (IN CASE THE EXPORT TAKES A
//          LONG TIME)...
   }

   // The data is ready, so write it to the file

   return Core::writeTextToFile(pFileName, data);
}

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

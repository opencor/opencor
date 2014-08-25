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

#include "csvdatastore.h"

//==============================================================================

#include <QtCore>

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

bool exportDataSet(const CoreDataStore::CoreDataStore *pDataset, const QString &pFileName)
{
    // Export the contents of a dataset to a CSV file

   QFile file(pFileName);
   if (!file.open(QIODevice::WriteOnly)) {
       // The file can't be opened, so...
       file.remove();
       return false;
   }

   QTextStream out(&file);

   const CoreDataStore::CoreDataStoreVariable *voi = pDataset->getVoi();
   QVector<CoreDataStore::CoreDataStoreVariable *> variables = pDataset->getVariables();

   // File header
   static const QString Header = "%1 (%2)";
   out << Header.arg(voi->getUri().replace("/prime", "'").replace("/", " | "),
                     voi->getUnits());
   for (auto v = variables.begin();  v != variables.end();  ++v) {
       out << "," << Header.arg((*v)->getUri().replace("/prime", "'").replace("/", " | "),
                                (*v)->getUnits());
   }
   out << "\n";

   // File data
   for (qulonglong j = 0;  j < pDataset->getSize();  ++j) {
       out << voi->getPoint(j);
       for (auto v = variables.begin();  v != variables.end();  ++v) {
           out << "," << (*v)->getPoint(j);
       }
       out << "\n";
   }

   file.close();

   return true;
}

//==============================================================================

}   // namespace CSVDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

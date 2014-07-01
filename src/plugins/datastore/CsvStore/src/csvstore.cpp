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
// A CSV store for simulation data
//==============================================================================

#include "csvstore.h"

//==============================================================================

#include <QtCore>

//==============================================================================

namespace OpenCOR {

namespace CsvStore {

//==============================================================================

/**
 * Export the contents of a dataset as a CSV file.
 *
**/
bool exportDataSet(const CoreData::DataSet *pDataset, const QString &pFileName)
/*---------------------------------------------------------------------------*/
{
   QFile file(pFileName);
   if (!file.open(QIODevice::WriteOnly)) {
       // The file can't be opened, so...
       file.remove();
       return false;
   }

   QTextStream out(&file);

   const CoreData::DataVariable *voi = pDataset->getVoi() ;
   QVector<CoreData::DataVariable *> variables = pDataset->getVariables() ;

   // File header
   static const QString Header = "%1 (%2)";
   out << Header.arg(voi->getUri().replace("/prime", "'").replace("/", " | "),
                     voi->getUnits());
   for (auto v = variables.begin() ;  v != variables.end() ;  ++v) {
       if (*v != voi) {
           out << "," << Header.arg((*v)->getUri().replace("/prime", "'").replace("/", " | "),
                                    (*v)->getUnits());
       }
   }
   out << "\n";

   // File data
   for (CoreData::SizeType j = 0 ;  j < pDataset->getSize() ;  ++j) {
       out << voi->getPoint(j) ;
       for (auto v = variables.begin() ;  v != variables.end() ;  ++v) {
           if (*v != voi) out << "," << (*v)->getPoint(j) ;
       }
       out << "\n";
   }

   file.close();
   
   return true ;
}

//==============================================================================

}   // namespace CsvStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

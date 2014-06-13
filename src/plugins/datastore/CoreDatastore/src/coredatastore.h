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
// A store for simulation data
//==============================================================================

#ifndef COREDATASTORE_H
#define COREDATASTORE_H

//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <string>
#include <vector>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CoreDatastore {

//==============================================================================

typedef qulonglong SizeType ;      // Large array sizes and indexes
class DataVariable {

 public:
  DataVariable(const qulonglong &pSize, const double *pValuePointer=0) ;
  ~DataVariable() ;

  void reset(void) ;
  SizeType storeValue(void) ;
  SizeType storeValue(const double &pValue) ;

 private:
  const std::string mUri ;
  const double *mValuePointer ;
  std::vector<double> mBuffer ;  
  } ;

//==============================================================================

class DataStore {

 public:
  DataStore(const qulonglong &pSize) ;
  ~DataStore() ;

  DataVariable *newVariable(const double *pValuePointer=0) ;
  void holdElements(const size_t &count, const double *values) ;
  void reset(void) ;
  void storeValues(void) ;

 private:
  const qulonglong mSize ;
  std::vector<DataVariable *> mVariables ;
  } ;

//==============================================================================

}   // namespace CoreDatastore
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

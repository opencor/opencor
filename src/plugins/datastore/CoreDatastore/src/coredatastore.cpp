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

#include "coredatastore.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CoreDatastore {

//==============================================================================

DataVariable::DataVariable(const qulonglong &pSize, const double *pValuePointer)
: mUri(""), mValuePointer(pValuePointer), mBuffer(0)
{
  mBuffer.reserve(pSize) ;
  }

DataVariable::~DataVariable()
{
  }

void DataVariable::reset(void)
{
  mBuffer.clear() ;
  }

void DataVariable::storeValue(void)
{
  mBuffer.push_back(mValuePointer ? *mValuePointer : 0.0) ;
  }

void DataVariable::storeValue(const double &pValue)
{
  mBuffer.push_back(pValue) ;
  }

//==============================================================================

DataStore::DataStore(const qulonglong &pSize)
: mSize(pSize), mVariables(0)
{
  }

DataStore::~DataStore()
{
  for (auto v = mVariables.begin() ;  v != mVariables.end() ;  ++v) {
    delete *v ;
    }
  }

DataVariable *DataStore::newVariable(const double *pValuePointer)
{
  DataVariable *v = new DataVariable(mSize, pValuePointer) ;
  mVariables.push_back(v) ;
  return v ;
  }

void DataStore::holdElements(const size_t &pCount, const double *pValues)
{
  const double *v = pValues ;
  for (size_t n = 0 ;  n < pCount ;  ++n, ++v) {
    mVariables.push_back(new DataVariable(mSize, v)) ;
    }
  }

void DataStore::reset(void)
{
  for (auto v = mVariables.begin() ;  v != mVariables.end() ;  ++v) {
    (*v)->reset() ;
    }
  }

void DataStore::storeValues(void)
{
  for (auto v = mVariables.begin() ;  v != mVariables.end() ;  ++v) {
    (*v)->storeValue() ;
    }
  }

//==============================================================================

}   // namespace CoreDatastore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

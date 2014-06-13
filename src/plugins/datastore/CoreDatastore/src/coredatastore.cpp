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
/*----------------------------------------------------------------------------*/
: mUri(""), mValuePointer(pValuePointer), mBuffer(0)
{
  mBuffer.reserve(pSize) ;
  }

DataVariable::~DataVariable()
/*-------------------------*/
{
  }

void DataVariable::reset(void)
/*--------------------------*/
{
  mBuffer.clear() ;
  }

SizeType DataVariable::storeValue(void)
/*-----------------------------------*/
{
  const qulonglong index = mBuffer.size() ;
  if (mValuePointer) mBuffer.push_back(*mValuePointer) ;
  return index ;
  }

SizeType DataVariable::storeValue(const double &pValue)
/*---------------------------------------------------*/
{
  const SizeType index = mBuffer.size() ;
  mBuffer.push_back(pValue) ;
  return index ;
  }

double DataVariable::getValue(const SizeType &pIndex) const  // also [] operator...
/*-------------------------------------------------------*/
{
  return mBuffer[pIndex] ;
  }

const double *DataVariable::data(void) const
/*----------------------------------------*/
{
  return mBuffer.data() ;
  }

//==============================================================================

DataStore::DataStore(const SizeType &pSize)
/*---------------------------------------*/
: mSize(pSize), mVariables(0)
{
  }

DataStore::~DataStore()
/*-------------------*/
{
  for (auto v = mVariables.begin() ;  v != mVariables.end() ;  ++v) {
    delete *v ;
    }
  }

IndexType DataStore::newVariable(const double *pValuePointer)
/*---------------------------------------------------------*/
{
  IndexType index = mVariables.size() ;
  DataVariable *v = new DataVariable(mSize, pValuePointer) ;
  mVariables.push_back(v) ;
  return index ;
  }

IndexType DataStore::holdElements(const IndexType &pCount, const double *pValues)
/*-----------------------------------------------------------------------------*/
{
  const double *v = pValues ;
  IndexType start = mVariables.size() ;
  for (IndexType n = 0 ;  n < pCount ;  ++n, ++v) {
    mVariables.push_back(new DataVariable(mSize, v)) ;
    }
  return start ;
  }

DataVariable *DataStore::getVariable(const IndexType pIndex) const
/*--------------------------------------------------------------*/
{
  return mVariables[pIndex] ;
  }

void DataStore::reset(void)
/*-----------------------*/
{
  for (auto v = mVariables.begin() ;  v != mVariables.end() ;  ++v) {
    (*v)->reset() ;
    }
  }

void DataStore::storeValues(void)
/*-----------------------------*/
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

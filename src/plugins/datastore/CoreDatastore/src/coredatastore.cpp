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

SizeType DataVariable::size(void)
/*-----------------------------*/
{
  return mBuffer.size() ;
  }

void DataVariable::savePoint(void)
/*----------------------------------*/
{
  if (mValuePointer) mBuffer.push_back(*mValuePointer) ;
  }

void DataVariable::savePoint(const double &pValue)
/*--------------------------------------------------*/
{
  mBuffer.push_back(pValue) ;
  }

double DataVariable::getPoint(const SizeType &pIndex) const  // also [] operator...
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

DataVariable *DataStore::holdPoint(const double *pPoint)
/*----------------------------------------------------*/
{
  DataVariable *var = new DataVariable(mSize, pPoint) ;
  mVariables.push_back(var) ;
  return var ;
  }

std::vector<const DataVariable *> DataStore::holdPoints(const IndexType &pCount, const double *pPoints)
/*---------------------------------------------------------------------------------------------------*/
{
  const double *v = pPoints ;
  std::vector<const DataVariable *> vars(pCount) ;
  for (IndexType n = 0 ;  n < pCount ;  ++n, ++v) {
    DataVariable *var = new DataVariable(mSize, v) ;
    mVariables.push_back(var) ;
    vars[n] = var ;
    }
  return vars ;
  }

void DataStore::reset(void)
/*-----------------------*/
{
  for (auto v = mVariables.begin() ;  v != mVariables.end() ;  ++v) {
    (*v)->reset() ;
    }
  }

void DataStore::savePoints(void)
/*----------------------------*/
{
  for (auto v = mVariables.begin() ;  v != mVariables.end() ;  ++v) {
    (*v)->savePoint() ;
    }
  }

//==============================================================================

}   // namespace CoreDatastore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

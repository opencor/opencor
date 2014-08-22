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

#include "coredata.h"

//==============================================================================

#include <cassert>

//==============================================================================

namespace OpenCOR {
namespace CoreData {

//==============================================================================

DataVariable::DataVariable(const qulonglong &pSize, const double *pValuePointer)
/*----------------------------------------------------------------------------*/
: mUri(""), mUnits(""), mLabel(""), mValuePointer(pValuePointer), mBuffer(0)
{
  mBuffer = new double[pSize] ;
  mSize = pSize ;
  }

DataVariable::~DataVariable()
/*-------------------------*/
{
  delete[] mBuffer ;
  }

void DataVariable::savePoint(const SizeType &pPos)
/*----------------------------------------------*/
{
  assert(pPos < mSize) ;
  if (mValuePointer) mBuffer[pPos] = *mValuePointer ;
  }

void DataVariable::savePoint(const SizeType &pPos, const double &pValue)
/*--------------------------------------------------------------------*/
{
  assert(pPos < mSize) ;
  mBuffer[pPos] = pValue ;
  }

double DataVariable::getPoint(const SizeType &pPos) const
/*-----------------------------------------------------*/
{
  assert(pPos < mSize) ;
  return mBuffer[pPos] ;
  }

const double *DataVariable::getData(void) const
/*-------------------------------------------*/
{
  return mBuffer ;
  }

SizeType DataVariable::getSize(void) const
/*--------------------------------------*/
{
  return mSize ;
  }

void DataVariable::setUri(const QString &pUri)
/*------------------------------------------*/
{
  mUri = pUri ;
  }

void DataVariable::setUnits(const QString &pUnits)
/*----------------------------------------------*/
{
  mUnits = pUnits ;
  }

void DataVariable::setLabel(const QString &pLabel)
/*----------------------------------------------*/
{
  mLabel = pLabel ;
  }

QString DataVariable::getUri(void) const
/*------------------------------------*/
{
  return mUri ;
  }

QString DataVariable::getUnits(void) const
/*--------------------------------------*/
{
  return mUnits ;
  }

QString DataVariable::getLabel(void) const
/*--------------------------------------*/
{
  return mLabel ;
  }


//==============================================================================

DataSet::DataSet(const SizeType &pSize)
/*-----------------------------------*/
: mSize(pSize), mVariables(0), mVoi(0)
{
  }

DataSet::~DataSet()
/*---------------*/
{
  if (mVoi) delete mVoi ;
  for (auto v = mVariables.begin() ;  v != mVariables.end() ;  ++v) {
    delete *v ;
    }
  }

DataVariable *DataSet::getVoi(void) const
/*-------------------------------------*/
{
  return mVoi ;
  }

DataVariable *DataSet::getVariable(long index) const
/*------------------------------------------------*/
{
  return mVariables[index] ;
  }

const QVector<DataVariable *> &DataSet::getVariables(void) const
/*------------------------------------------------------------*/
{
  return mVariables ;
  }

DataVariable *DataSet::holdPoint(const double *pPoint, const bool &pVoi)
/*--------------------------------------------------------------------*/
{
  if (pVoi && mVoi) delete mVoi ;
  DataVariable *var = new DataVariable(mSize, pPoint) ;
  if (pVoi) mVoi = var ;
  else mVariables.push_back(var) ;
  return var ;
  }

QVector<DataVariable *> DataSet::holdPoints(const IndexType &pCount, const double *pPoints)
/*---------------------------------------------------------------------------------------*/
{
  const double *v = pPoints ;
  QVector<DataVariable *> vars(pCount) ;
  for (IndexType n = 0 ;  n < pCount ;  ++n, ++v) {
    DataVariable *var = new DataVariable(mSize, v) ;
    mVariables.push_back(var) ;
    vars[n] = var ;
    }
  return vars ;
  }

void DataSet::savePoints(const SizeType &pPos)
/*------------------------------------------*/
{
  for (auto vp = mVariables.begin() ;  vp != mVariables.end() ;  ++vp)
    (*vp)->savePoint(pPos) ;
  }

SizeType DataSet::getSize(void) const
/*---------------------------------*/
{
  return mSize ;
  }

IndexType DataSet::length(void) const
/*---------------------------------*/
{
  return mVariables.size() ;
  }

//==============================================================================

}   // namespace CoreData
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

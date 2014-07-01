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

#ifndef COREDATA_H
#define COREDATA_H

//==============================================================================

#include <QString>
#include <QVector>

//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <cassert>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CoreData {

//==============================================================================

typedef qulonglong SizeType ;   // Large array sizes and indexes
typedef long       IndexType ;  // Object counts and indexes (-1 ==> not inited)

//==============================================================================

class DataVariable {

 public:
  DataVariable(const SizeType &pSize, const double *pValuePointer=0) ;
  ~DataVariable() ;

  void setUri(const QString &pUri) ;
  void setUnits(const QString &pUnits) ;
  void setLabel(const QString &pLavel) ;
  const QString getUri(void) const ;
  const QString getLabel(void) const ;
  const QString getUnits(void) const ;

/*
   The following code is time-critical and having it
   in a separate implementation file that is compiled
   to a library can double execution time (under OS/X).
*/

  void savePoint(const SizeType &pPos)
  /*--------------------------------*/
  {
    assert(pPos >= 0 && pPos < mSize) ;
    if (mValuePointer) mBuffer[pPos] = *mValuePointer ;
    }

  void savePoint(const SizeType &pPos, const double &pValue)
  /*------------------------------------------------------*/
  {
    assert(pPos >= 0 && pPos < mSize) ;
    mBuffer[pPos] = pValue ;
    }

  double getPoint(const SizeType &pPos) const
  /*---------------------------------------*/
  {
    assert(pPos >= 0 && pPos < mSize) ;
    return mBuffer[pPos] ;
    }

  const double *data(void) const
  /*--------------------------*/
  {
    return mBuffer ;
    }

 private:
  QString mUri ;
  QString mUnits ;
  QString mLabel ;
  const double *mValuePointer ;
  double *mBuffer ;
  SizeType mSize ;
  } ;

//==============================================================================

class DataSet {

 public:
  DataSet(const SizeType &pSize) ;
  ~DataSet() ;

  DataVariable *holdPoint(const double *pPoint=0, const bool &pVoi=false) ;
  QVector<DataVariable *> holdPoints(const IndexType &pCount, const double *pPoints) ;

  void savePoints(const SizeType &pPos)
  /*---------------------------------*/
  {
    for (auto vp = mVariables.begin() ;  vp != mVariables.end() ;  ++vp)
      (*vp)->savePoint(pPos) ;
    }

 private:
  const SizeType mSize ;
  QVector<DataVariable *> mVariables ;
  DataVariable *mVoi ;
  } ;

//==============================================================================

}   // namespace CoreData
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

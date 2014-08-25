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

#include "coredataglobal.h"

//==============================================================================

#include <QString>
#include <QVector>

//==============================================================================

#include <QtGlobal>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CoreData {

//==============================================================================

class COREDATA_EXPORT DataVariable {

 public:
  DataVariable(const qulonglong &pSize, const double *pValuePointer=0) ;
  virtual ~DataVariable() ;

  void setUri(const QString &pUri) ;
  void setUnits(const QString &pUnits) ;
  void setLabel(const QString &pLabel) ;

  QString getUri() const ;
  QString getLabel() const ;
  QString getUnits() const ;

  void savePoint(const qulonglong &pPos) ;
  void savePoint(const qulonglong &pPos, const double &pValue) ;

  double getPoint(const qulonglong &pPos) const ;
  const double *getData() const ;
  qulonglong getSize() const ;

 private:
  QString mUri ;
  QString mUnits ;
  QString mLabel ;
  const double *mValuePointer ;
  double *mBuffer ;
  qulonglong mSize ;
  } ;

//==============================================================================

class COREDATA_EXPORT DataSet {

 public:
  DataSet(const qulonglong &pSize) ;
  virtual ~DataSet() ;

  DataVariable * getVoi() const ;
  DataVariable * getVariable(long index) const ;
  const QVector<DataVariable *> &getVariables() const ;

  DataVariable * holdPoint(const double *pPoint=0, const bool &pVoi=false) ;
  QVector<DataVariable *> holdPoints(const long &pCount, const double *pPoints) ;

  void savePoints(const qulonglong &pPos) ;

  qulonglong getSize() const ;
  long length() const ;

 private:
  const qulonglong mSize ;
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

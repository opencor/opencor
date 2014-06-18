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

#include <QString>
#include <QVector>

//==============================================================================

#include <QtGlobal>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CoreDatastore {

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

  void reset(void) ;
  SizeType size(void) ;
  void savePoint(void) ;
  void savePoint(const double &pValue) ;
  double getPoint(const SizeType &pIndex) const ;  // also [] operator...
  const double *data(void) const ;

 private:
  QString mUri ;
  QString mUnits ;
  QString mLabel ;
  const double *mValuePointer ;
  QVector<double> mBuffer ;
  } ;

//==============================================================================

class DataSet {

 public:
  DataSet(const SizeType &pSize) ;
  ~DataSet() ;

  DataVariable *holdPoint(const double *pPoint=0) ;
  QVector<DataVariable *> holdPoints(const IndexType &pCount, const double *pPoints) ;
  void reset(void) ;
  void savePoints(void) ;

 private:
  const SizeType mSize ;
  QVector<DataVariable *> mVariables ;
  } ;

//==============================================================================

}   // namespace CoreDatastore
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

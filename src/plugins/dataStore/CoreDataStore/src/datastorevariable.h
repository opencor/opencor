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
// Core data store variable class
//==============================================================================

#ifndef COREDATASTOREVARIABLE_H
#define COREDATASTOREVARIABLE_H

//==============================================================================

#include "coredatastoreglobal.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <QString>
#include <QVector>

//==============================================================================

namespace OpenCOR {
namespace CoreDataStore {

//==============================================================================

class COREDATASTORE_EXPORT DataStoreVariable
{
public:
    explicit DataStoreVariable(const qulonglong &pSize, double *pValue = 0);
    virtual ~DataStoreVariable();

    QString uri() const;
    void setUri(const QString &pUri);

    QString label() const;
    void setLabel(const QString &pLabel);

    QString unit() const;
    void setUnit(const QString &pUnit);

    qulonglong size() const;

    void setValue(const qulonglong &pPosition);
    void setValue(const qulonglong &pPosition, const double &pValue);

    double value(const qulonglong &pPosition) const;
    double * values() const;

private:
    QString mUri;
    QString mName;
    QString mUnit;

    qulonglong mSize;

    double *mValue;
    double *mValues;
};

//==============================================================================

typedef QVector<DataStoreVariable *> DataStoreVariables;

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

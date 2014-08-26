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

#include "datastorevariable.h"

//==============================================================================

namespace OpenCOR {
namespace CoreDataStore {

//==============================================================================

DataStoreVariable::DataStoreVariable(const qulonglong &pSize,
                                     const double *pValue) :
    mUri(QString()),
    mUnits(QString()),
    mLabel(QString()),
    mValue(pValue),
    mSize(pSize)
{
    mData = new double[pSize];
}

//==============================================================================

DataStoreVariable::~DataStoreVariable()
{
    delete[] mData;
}

//==============================================================================

void DataStoreVariable::savePoint(const qulonglong &pPosition)
{
    Q_ASSERT(pPosition < mSize);

    if (mValue)
        mData[pPosition] = *mValue;
}

//==============================================================================

void DataStoreVariable::savePoint(const qulonglong &pPosition,
                                  const double &pValue)
{
    Q_ASSERT(pPosition < mSize);

    mData[pPosition] = pValue;
}

//==============================================================================

double DataStoreVariable::getPoint(const qulonglong &pPosition) const
{
    Q_ASSERT(pPosition < mSize);

    return mData[pPosition];
}

//==============================================================================

const double *DataStoreVariable::getData() const
{
    return mData;
}

//==============================================================================

qulonglong DataStoreVariable::getSize() const
{
    return mSize;
}

//==============================================================================

void DataStoreVariable::setUri(const QString &pUri)
{
    mUri = pUri;
}

//==============================================================================

void DataStoreVariable::setUnits(const QString &pUnits)
{
    mUnits = pUnits;
}

//==============================================================================

void DataStoreVariable::setLabel(const QString &pLabel)
{
    mLabel = pLabel;
}

//==============================================================================

QString DataStoreVariable::getUri() const
{
    return mUri;
}

//==============================================================================

QString DataStoreVariable::getUnits() const
{
    return mUnits;
}

//==============================================================================

QString DataStoreVariable::getLabel() const
{
    return mLabel;
}

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

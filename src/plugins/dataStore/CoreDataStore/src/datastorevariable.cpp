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

DataStoreVariable::DataStoreVariable(const qulonglong &pSize, double *pValue) :
    mUri(QString()),
    mName(QString()),
    mUnit(QString()),
    mSize(pSize),
    mValue(pValue)
{
    // Create our array of values

    mValues = new double[pSize];
}

//==============================================================================

DataStoreVariable::~DataStoreVariable()
{
    // Delete some internal objects

    delete[] mValues;
}

//==============================================================================

QString DataStoreVariable::uri() const
{
    // Return our URI

    return mUri;
}

//==============================================================================

void DataStoreVariable::setUri(const QString &pUri)
{
    // Set our URI

    mUri = pUri;
}

//==============================================================================

QString DataStoreVariable::label() const
{
    // Return our label

    return mName;
}

//==============================================================================

void DataStoreVariable::setLabel(const QString &pLabel)
{
    // Set our label

    mName = pLabel;
}

//==============================================================================

QString DataStoreVariable::unit() const
{
    // Return our unit

    return mUnit;
}

//==============================================================================

void DataStoreVariable::setUnit(const QString &pUnit)
{
    // Set our unit

    mUnit = pUnit;
}

//==============================================================================

qulonglong DataStoreVariable::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

void DataStoreVariable::setValue(const qulonglong &pPosition)
{
    // Set the value of the variable at the given position

    Q_ASSERT(pPosition < mSize);
    Q_ASSERT(mValue);

    mValues[pPosition] = *mValue;
}

//==============================================================================

void DataStoreVariable::setValue(const qulonglong &pPosition,
                                 const double &pValue)
{
    // Set the value of the variable at the given position using the given value

    Q_ASSERT(pPosition < mSize);

    mValues[pPosition] = pValue;
}

//==============================================================================

double DataStoreVariable::value(const qulonglong &pPosition) const
{
    // Return our value at the given position

    Q_ASSERT(pPosition < mSize);

    return mValues[pPosition];
}

//==============================================================================

double * DataStoreVariable::values() const
{
    // Return our values

    return mValues;
}

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

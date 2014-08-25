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

#include "coredatastorevariable.h"

//==============================================================================

namespace OpenCOR {
namespace CoreDataStore {

//==============================================================================

CoreDataStoreVariable::CoreDataStoreVariable(const qulonglong &pSize,
                           const double *pValuePointer) :
    mUri(QString()),
    mUnits(QString()),
    mLabel(QString()),
    mValuePointer(pValuePointer),
    mSize(pSize)
{
    mBuffer = new double[pSize];
}

//==============================================================================

CoreDataStoreVariable::~CoreDataStoreVariable()
{
    delete[] mBuffer;
}

//==============================================================================

void CoreDataStoreVariable::savePoint(const qulonglong &pPos)
{
    Q_ASSERT(pPos < mSize);

    if (mValuePointer)
        mBuffer[pPos] = *mValuePointer;
}

//==============================================================================

void CoreDataStoreVariable::savePoint(const qulonglong &pPos, const double &pValue)
{
    Q_ASSERT(pPos < mSize);

    mBuffer[pPos] = pValue;
}

//==============================================================================

double CoreDataStoreVariable::getPoint(const qulonglong &pPos) const
{
    Q_ASSERT(pPos < mSize);

    return mBuffer[pPos];
}

//==============================================================================

const double *CoreDataStoreVariable::getData() const
{
    return mBuffer;
}

//==============================================================================

qulonglong CoreDataStoreVariable::getSize() const
{
    return mSize;
}

//==============================================================================

void CoreDataStoreVariable::setUri(const QString &pUri)
{
    mUri = pUri;
}

//==============================================================================

void CoreDataStoreVariable::setUnits(const QString &pUnits)
{
    mUnits = pUnits;
}

//==============================================================================

void CoreDataStoreVariable::setLabel(const QString &pLabel)
{
    mLabel = pLabel;
}

//==============================================================================

QString CoreDataStoreVariable::getUri() const
{
    return mUri;
}

//==============================================================================

QString CoreDataStoreVariable::getUnits() const
{
    return mUnits;
}

//==============================================================================

QString CoreDataStoreVariable::getLabel() const
{
    return mLabel;
}

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

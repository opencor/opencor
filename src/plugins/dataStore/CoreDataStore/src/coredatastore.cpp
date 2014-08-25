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
// Core data store class
//==============================================================================

#include "coredatastore.h"
#include "coredatastorevariable.h"

//==============================================================================

namespace OpenCOR {
namespace CoreDataStore {

//==============================================================================

CoreDataStore::CoreDataStore(const qulonglong &pSize) :
    mSize(pSize),
    mVariables(0),
    mVoi(0)
{
}

//==============================================================================

CoreDataStore::~CoreDataStore()
{
    delete mVoi;

    for (auto var = mVariables.begin(), varEnd = mVariables.end(); var != varEnd; ++var)
        delete *var;
}

//==============================================================================

CoreDataStoreVariable * CoreDataStore::getVoi() const
{
    return mVoi;
}

//==============================================================================

CoreDataStoreVariable * CoreDataStore::getVariable(long index) const
{
    return mVariables[index];
}

//==============================================================================

const QVector<CoreDataStoreVariable *> &CoreDataStore::getVariables() const
{
    return mVariables;
}

//==============================================================================

CoreDataStoreVariable * CoreDataStore::holdPoint(const double *pPoint,
                                                 const bool &pVoi)
{
    if (pVoi)
        delete mVoi;

    CoreDataStoreVariable *var = new CoreDataStoreVariable(mSize, pPoint);

    if (pVoi)
        mVoi = var;
    else
        mVariables.push_back(var);

    return var;
}

//==============================================================================

QVector<CoreDataStoreVariable *> CoreDataStore::holdPoints(const long &pCount,
                                            const double *pPoints)
{
    const double *points = pPoints;

    QVector<CoreDataStoreVariable *> vars(pCount);

    for (long i = 0;  i < pCount;  ++i, ++points) {
        CoreDataStoreVariable *variable = new CoreDataStoreVariable(mSize, points);

        mVariables.push_back(variable);

        vars[i] = variable;
    }

    return vars;
}

//==============================================================================

void CoreDataStore::savePoints(const qulonglong &pPos)
{
    for (auto var = mVariables.begin(), varEnd = mVariables.end(); var != varEnd; ++var)
        (*var)->savePoint(pPos);
}

//==============================================================================

qulonglong CoreDataStore::getSize() const
{
    return mSize;
}

//==============================================================================

long CoreDataStore::length() const
{
    return mVariables.size();
}

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

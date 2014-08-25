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

//==============================================================================

namespace OpenCOR {
namespace CoreDataStore {

//==============================================================================

CoreDataStore::CoreDataStore(const qulonglong &pSize) :
    mSize(pSize),
    mVoi(0),
    mVariables(0)
{
}

//==============================================================================

CoreDataStore::~CoreDataStore()
{
    // Delete some internal objects

    delete mVoi;

    for (auto variable = mVariables.begin(), variableEnd = mVariables.end();
         variable != variableEnd; ++variable) {
        delete *variable;
    }
}

//==============================================================================

qulonglong CoreDataStore::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

CoreDataStoreVariable * CoreDataStore::voi() const
{
    // Return our variable of integration

    return mVoi;
}

//==============================================================================

CoreDataStoreVariables CoreDataStore::variables() const
{
    // Return all our variables

    return mVariables;
}

//==============================================================================

CoreDataStoreVariable * CoreDataStore::holdPoint(const double *pPoint,
                                                 const bool &pVoi)
{
    if (pVoi)
        delete mVoi;

    CoreDataStoreVariable *variable = new CoreDataStoreVariable(mSize, pPoint);

    if (pVoi)
        mVoi = variable;
    else
        mVariables << variable;

    return variable;
}

//==============================================================================

CoreDataStoreVariables CoreDataStore::holdPoints(const int &pCount,
                                                 const double *pPoints)
{
    const double *points = pPoints;

    CoreDataStoreVariables variables(pCount);

    for (int i = 0;  i < pCount;  ++i, ++points) {
        CoreDataStoreVariable *variable = new CoreDataStoreVariable(mSize, points);

        mVariables << variable;

        variables[i] = variable;
    }

    return variables;
}

//==============================================================================

void CoreDataStore::savePoints(const qulonglong &pPosition)
{
    for (auto variable = mVariables.begin(), variableEnd = mVariables.end();
         variable != variableEnd; ++variable) {
        (*variable)->savePoint(pPosition);
    }
}

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

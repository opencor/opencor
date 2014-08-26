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

DataStoreVariable * CoreDataStore::voi() const
{
    // Return our variable of integration

    return mVoi;
}

//==============================================================================

DataStoreVariable * CoreDataStore::addVoi()
{
    // Add a (new) variable of integration to our data store

    delete mVoi;

    mVoi = new DataStoreVariable(mSize);

    return mVoi;
}

//==============================================================================

DataStoreVariables CoreDataStore::variables() const
{
    // Return all our variables

    return mVariables;
}

//==============================================================================

DataStoreVariable * CoreDataStore::addVariable(double *pPoint)
{
    // Add a variable to our data store

    DataStoreVariable *variable = new DataStoreVariable(mSize, pPoint);

    mVariables << variable;

    return variable;
}

//==============================================================================

DataStoreVariables CoreDataStore::addVariables(const int &pCount, double *pData)
{
    // Add some variables to our data store

    DataStoreVariables variables(pCount);

    for (int i = 0;  i < pCount;  ++i, ++pData) {
        variables[i] = new DataStoreVariable(mSize, pData);

        mVariables << variables[i];
    }

    return variables;
}

//==============================================================================

void CoreDataStore::setValues(const qulonglong &pPosition)
{
    // Set the value at the given position of all our variables

    for (auto variable = mVariables.begin(), variableEnd = mVariables.end();
         variable != variableEnd; ++variable) {
        (*variable)->setValue(pPosition);
    }
}

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

#include <Qt>

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

bool sortVariables(DataStoreVariable *pVariable1, DataStoreVariable *pVariable2)
{
    // Determine which of the two variables should be first based on their URI
    // Note: the comparison is case insensitive, so that it's easier for people
    //       to find a variable...

    return pVariable1->uri().compare(pVariable2->uri(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

DataStoreVariables CoreDataStore::variables()
{
    // Return all our variables, after making sure that they are sorted

    std::sort(mVariables.begin(), mVariables.end(), sortVariables);

    return mVariables;
}

//==============================================================================

DataStoreVariable * CoreDataStore::addVariable(double *pValue)
{
    // Add a variable to our data store

    DataStoreVariable *variable = new DataStoreVariable(mSize, pValue);

    mVariables << variable;

    return variable;
}

//==============================================================================

DataStoreVariables CoreDataStore::addVariables(const int &pCount,
                                               double *pValues)
{
    // Add some variables to our data store

    DataStoreVariables variables(pCount);

    for (int i = 0; i < pCount; ++i, ++pValues) {
        variables[i] = new DataStoreVariable(mSize, pValues);

        mVariables << variables[i];
    }

    return variables;
}

//==============================================================================

void CoreDataStore::setValues(const qulonglong &pPosition, const double &pValue)
{
    // Set the value at the given position of all our variables including our
    // variable of integration, which value is directly given to us

    if (mVoi)
        mVoi->setValue(pPosition, pValue);

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

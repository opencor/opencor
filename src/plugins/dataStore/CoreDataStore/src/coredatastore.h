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

#ifndef COREDATASTORE_H
#define COREDATASTORE_H

//==============================================================================

#include "coredatastoreglobal.h"
#include "coredatastorevariable.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <QVector>

//==============================================================================

namespace OpenCOR {
namespace CoreDataStore {

//==============================================================================

class COREDATASTORE_EXPORT CoreDataStore
{
public:
    explicit CoreDataStore(const qulonglong &pSize);
    virtual ~CoreDataStore();

    qulonglong size() const;

    CoreDataStoreVariable * voi() const;
    CoreDataStoreVariables variables() const;

    CoreDataStoreVariable * holdPoint(const double *pPoint = 0,
                                      const bool &pVoi = false);
    CoreDataStoreVariables holdPoints(const int &pCount, const double *pPoints);

    void savePoints(const qulonglong &pPosition);

private:
    const qulonglong mSize;

    CoreDataStoreVariable *mVoi;
    CoreDataStoreVariables mVariables;
};

//==============================================================================

}   // namespace CoreDataStore
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

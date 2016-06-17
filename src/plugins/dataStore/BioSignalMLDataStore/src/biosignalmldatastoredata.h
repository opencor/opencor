/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// BioSignalML data store data
//==============================================================================

#ifndef BIOSIGNALMLDATASTOREDATA_H
#define BIOSIGNALMLDATASTOREDATA_H

//==============================================================================

#include "datastoreinterface.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

class BiosignalmlDataStoreData : public DataStore::DataStoreData
{
public:
    explicit BiosignalmlDataStoreData(const QString &pFileName,
                                      const QString &pShortName,
                                      const QString &pAuthor,
                                      const QString &pDescription,
                                      const QVector<bool> &pSelectedVariables,
                                      const QString &pComment);

    QString shortName() const;
    QString author() const;
    QString description() const;
    QVector<bool> selectedVariables() const;
    QString comment() const;

private:
    QString mShortName;
    QString mAuthor;
    QString mDescription;
    QVector<bool> mSelectedVariables;
    QString mComment;
};

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

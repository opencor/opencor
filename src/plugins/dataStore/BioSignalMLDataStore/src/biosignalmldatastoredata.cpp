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
// BioSignalML data store data class
//==============================================================================

#include "biosignalmldatastoredata.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BiosignalmlDataStoreData::BiosignalmlDataStoreData(const QString &pFileName,
                                                   const QString &pShortName,
                                                   const QString &pAuthor,
                                                   const QString &pDescription,
                                                   const QVector<bool> &pSelectedVariables,
                                                   const QString &pComment) :
    DataStore::DataStoreData(pFileName),
    mShortName(pShortName),
    mAuthor(pAuthor),
    mDescription(pDescription),
    mSelectedVariables(pSelectedVariables),
    mComment(pComment)
{
}

//==============================================================================

QString BiosignalmlDataStoreData::shortName() const
{
    // Return our short name

    return mShortName;
}

//==============================================================================

QString BiosignalmlDataStoreData::author() const
{
    // Return our author

    return mAuthor;
}

//==============================================================================

QString BiosignalmlDataStoreData::description() const
{
    // Return our description

    return mDescription;
}

//==============================================================================

QVector<bool> BiosignalmlDataStoreData::selectedVariables() const
{
    // Return our selected variables

    return mSelectedVariables;
}

//==============================================================================

QString BiosignalmlDataStoreData::comment() const
{
    // Return our comment

    return mComment;
}

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

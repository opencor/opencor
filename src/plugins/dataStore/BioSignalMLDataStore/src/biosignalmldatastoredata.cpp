/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// BioSignalML data store data
//==============================================================================

#include "biosignalmldatastoredata.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BiosignalmlDataStoreData::BiosignalmlDataStoreData(const QString &pFileName,
                                                   const QString &pName,
                                                   const QString &pAuthor,
                                                   const QString &pDescription,
                                                   const QString &pComment,
                                                   const DataStore::DataStoreVariables &pSelectedVariables) :
    DataStore::DataStoreData(pFileName, pSelectedVariables),
    mName(pName),
    mAuthor(pAuthor),
    mDescription(pDescription),
    mComment(pComment)
{
}

//==============================================================================

QString BiosignalmlDataStoreData::name() const
{
    // Return our short name

    return mName;
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

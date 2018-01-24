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

#pragma once

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
                                      const QString &pName,
                                      const QString &pAuthor,
                                      const QString &pDescription,
                                      const QString &pComment,
                                      const DataStore::DataStoreVariables &pSelectedVariables);

    QString name() const;
    QString author() const;
    QString description() const;
    QString comment() const;

private:
    QString mName;
    QString mAuthor;
    QString mDescription;
    QString mComment;
};

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

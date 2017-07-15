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
// Interfaces
//==============================================================================

#include "corecliutils.h"
#include "interfaces.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

InterfacesData::InterfacesData(const FileTypeInterfaces &pFileTypeInterfaces,
                               const SolverInterfaces &pSolverInterfaces,
                               const DataStoreInterfaces &pDataStoreInterfaces) :
    mFileTypeInterfaces(pFileTypeInterfaces),
    mSolverInterfaces(pSolverInterfaces),
    mDataStoreInterfaces(pDataStoreInterfaces)
{
}

//==============================================================================

FileTypeInterfaces InterfacesData::fileTypeInterfaces() const
{
    // Return our file type interfaces

    return mFileTypeInterfaces;
}

//==============================================================================

SolverInterfaces InterfacesData::solverInterfaces() const
{
    // Return our solver interfaces

    return mSolverInterfaces;
}

//==============================================================================

DataStoreInterfaces InterfacesData::dataStoreInterfaces() const
{
    // Return our data store interfaces

    return mDataStoreInterfaces;
}

//==============================================================================

FileTypeInterfaces fileTypeInterfaces()
{
    // Return our file type interfaces

    return static_cast<InterfacesData *>(Core::globalInstance(InterfacesDataSignature))->fileTypeInterfaces();
}

//==============================================================================

SolverInterfaces solverInterfaces()
{
    // Return our solver interfaces

    return static_cast<InterfacesData *>(Core::globalInstance(InterfacesDataSignature))->solverInterfaces();
}

//==============================================================================

DataStoreInterfaces dataStoreInterfaces()
{
    // Return our data store interfaces

    return static_cast<InterfacesData *>(Core::globalInstance(InterfacesDataSignature))->dataStoreInterfaces();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

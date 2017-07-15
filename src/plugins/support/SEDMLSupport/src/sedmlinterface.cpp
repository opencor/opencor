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
// SED-ML interface
//==============================================================================

#include "corecliutils.h"
#include "sedmlinterface.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

SedmlInterfaceData::SedmlInterfaceData(FileTypeInterface *pFileTypeInterface) :
    mFileTypeInterface(pFileTypeInterface)
{
}

//==============================================================================

FileTypeInterface * SedmlInterfaceData::fileTypeInterface() const
{
    // Return our file type interface

    return mFileTypeInterface;
}

//==============================================================================

FileTypeInterface * fileTypeInterface()
{
    // Return our file type interface

    return static_cast<SedmlInterfaceData *>(Core::globalInstance(SedmlInterfaceDataSignature))->fileTypeInterface();
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

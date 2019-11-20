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
// COMBINE interface
//==============================================================================

#pragma once

//==============================================================================

#include "combinesupportglobal.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class FileTypeInterface;

//==============================================================================

namespace COMBINESupport {

//==============================================================================

static const auto CombineInterfaceDataSignature = QStringLiteral("OpenCOR::COMBINESupport::CombineInterfaceData");

//==============================================================================

class CombineInterfaceData
{
public:
    explicit CombineInterfaceData(FileTypeInterface *pFileTypeInterface);

    FileTypeInterface * fileTypeInterface() const;

private:
    FileTypeInterface *mFileTypeInterface;
};

//==============================================================================

FileTypeInterface COMBINESUPPORT_EXPORT * fileTypeInterface();

//==============================================================================

} // namespace COMBINESupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

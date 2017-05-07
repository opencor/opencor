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
// CellML file manager
//==============================================================================

#include "cellmlfilemanager.h"
#include "corecliutils.h"
#include "filemanager.h"

//==============================================================================

#include "cellmlapibegin.h"
    #include "CellMLBootstrap.hpp"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileManager * CellmlFileManager::instance()
{
    // Return the 'global' instance of our CellML file manager class

    static CellmlFileManager instance;

    return static_cast<CellmlFileManager *>(Core::globalInstance("OpenCOR::CellMLSupport::CellmlFileManager",
                                                                 &instance));
}

//==============================================================================

bool CellmlFileManager::isCellmlFile(const QString &pFileName) const
{
    // Return whether the given file is a CellML file

    return instance()->isFile(pFileName);
}

//==============================================================================

CellmlFile * CellmlFileManager::cellmlFile(const QString &pFileName)
{
    // Return the CellmlFile object, if any, associated with the given file

    return static_cast<CellMLSupport::CellmlFile *>(instance()->file(pFileName));
}

//==============================================================================

bool CellmlFileManager::canLoadFile(const QString &pFileName) const
{
    // Try to load the CellML file

    CellmlFile cellmlFile(pFileName);

    return cellmlFile.load();
}

//==============================================================================

QObject * CellmlFileManager::newFile(const QString &pFileName) const
{
    // Create and return a new CellML file

    return new CellmlFile(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

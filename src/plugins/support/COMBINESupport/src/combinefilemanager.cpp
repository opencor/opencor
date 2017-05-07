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
// COMBINE file manager
//==============================================================================

#include "combinefilemanager.h"
#include "combinesupportplugin.h"
#include "corecliutils.h"
#include "filemanager.h"

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

CombineFileManager * CombineFileManager::instance()
{
    // Return the 'global' instance of our COMBINE file manager class

    static CombineFileManager instance;

    return static_cast<CombineFileManager *>(Core::globalInstance("OpenCOR::COMBINESupport::CombineFileManager",
                                                                  &instance));
}

//==============================================================================

bool CombineFileManager::isCombineArchive(const QString &pFileName) const
{
    // Return whether the given file is a COMBINE archive

    return instance()->isFile(pFileName);
}

//==============================================================================

CombineArchive * CombineFileManager::combineArchive(const QString &pFileName)
{
    // Return the CombineArchive object, if any, associated with the given file

    return static_cast<COMBINESupport::CombineArchive *>(instance()->file(pFileName));
}

//==============================================================================

bool CombineFileManager::canLoadFile(const QString &pFileName) const
{
    // Try to load the COMBINE archive

    CombineArchive combineArchive(pFileName);

    return combineArchive.load();
}

//==============================================================================

QObject * CombineFileManager::newFile(const QString &pFileName) const
{
    // Create and return a new COMBINE archive

    return new CombineArchive(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

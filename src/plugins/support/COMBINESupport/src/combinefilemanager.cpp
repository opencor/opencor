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

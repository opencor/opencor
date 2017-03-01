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

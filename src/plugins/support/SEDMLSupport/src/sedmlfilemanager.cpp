/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// SED-ML file manager
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "sedmlfilemanager.h"
#include "sedmlsupportplugin.h"

//==============================================================================

#include "libsedmlbegin.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedReader.h"
#include "libsedmlend.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

SedmlFileManager * SedmlFileManager::instance()
{
    // Return the 'global' instance of our SED-ML file manager class

    static SedmlFileManager instance;

    return static_cast<SedmlFileManager *>(Core::globalInstance("OpenCOR::SEDMLSupport::SedmlFileManager::instance()",
                                                                &instance));
}

//==============================================================================

bool SedmlFileManager::isSedmlFile(const QString &pFileName) const
{
    // Return whether the given file is a SED-ML file

    return isFile(pFileName);
}

//==============================================================================

SedmlFile * SedmlFileManager::sedmlFile(const QString &pFileName) const
{
    // Return the SedmlFile object, if any, associated with the given file

    return static_cast<SedmlFile *>(file(pFileName));
}

//==============================================================================

bool SedmlFileManager::canLoad(const QString &pFileName) const
{
    // Try to load the SED-ML file

    SedmlFile sedmlFile(pFileName);

    return sedmlFile.load();
}

//==============================================================================

StandardSupport::StandardFile * SedmlFileManager::create(const QString &pFileName) const
{
    // Create and return a new SED-ML file

    return new SedmlFile(Core::canonicalFileName(pFileName));
}

//==============================================================================

} // namespace SEDMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

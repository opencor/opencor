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
// Standard file
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "standardfile.h"

//==============================================================================

namespace OpenCOR {
namespace StandardSupport {

//==============================================================================

StandardFile::StandardFile(const QString &pFileName) :
    mFileName(Core::canonicalFileName(pFileName)),
    mNew(Core::FileManager::instance()->isNew(pFileName))
{
}

//==============================================================================

bool StandardFile::reload()
{
    // We want to reload the file, so we must first reset everything

    reset();

    // Now, we can try to (re)load the file

    return load();
}

//==============================================================================

bool StandardFile::save(const QString &pFileName)
{
    // Our file being saved, so it cannot be new and cannot be modified (should
    // it have been modified before)
    // Note: we must do this before updating mFileName (should it be given a new
    //       value) since we use it to update our modified status...

    mNew = false;

    setModified(false);

    // Make sure that mFileName is up to date

    if (!pFileName.isEmpty()) {
        setFileName(pFileName);
    }

    return true;
}

//==============================================================================

QString StandardFile::fileName() const
{
    // Return the standard file's file name

    return mFileName;
}

//==============================================================================

void StandardFile::setFileName(const QString &pFileName)
{
    // Set the standard file's file name

    mFileName = Core::canonicalFileName(pFileName);
}

//==============================================================================

bool StandardFile::isNew() const
{
    // Return whether we are new

    return mNew;
}

//==============================================================================

bool StandardFile::isModified() const
{
    // Return whether we have been modified

    return Core::FileManager::instance()->isModified(mFileName);
}

//==============================================================================

void StandardFile::setModified(bool pModified) const
{
    // Set our modified status

    Core::FileManager::instance()->setModified(mFileName, pModified);
}

//==============================================================================

} // namespace StandardSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

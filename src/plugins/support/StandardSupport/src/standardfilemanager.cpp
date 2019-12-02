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
// Standard file manager
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "standardfilemanager.h"
#include "standardsupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace StandardSupport {

//==============================================================================

StandardFileManager::StandardFileManager()
{
    // Create some connections to keep track of some events related to our
    // 'global' file manager

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    connect(fileManagerInstance, &Core::FileManager::fileManaged,
            this, &StandardFileManager::manage);
    connect(fileManagerInstance, &Core::FileManager::fileUnmanaged,
            this, &StandardFileManager::unmanage);

    connect(fileManagerInstance, &Core::FileManager::fileReloaded,
            this, QOverload<const QString &>::of(&StandardFileManager::reload));
    connect(fileManagerInstance, &Core::FileManager::fileRenamed,
            this, &StandardFileManager::rename);

    connect(fileManagerInstance, &Core::FileManager::fileSaved,
            this, &StandardFileManager::save);
}

//==============================================================================

bool StandardFileManager::isFile(const QString &pFileName, bool pForceChecking) const
{
    // If the given file is already managed, then we consider that it's of the
    // right type (e.g. CellML file), even though it may not be of the right
    // type anymore after having been edited and saved, but in this case it's
    // good to keep considering the file as of the right type, so that the user
    // can continue editing it without any problem, for example

    if (!pForceChecking && (file(pFileName) != nullptr)) {
        return true;
    }

    // The given file is not managed, so consider it of the right type if it is
    // an empty file (after having been trimmed) or it can be loaded

    QByteArray fileContents;

    if (Core::readFile(pFileName, fileContents)) {
        if (fileContents.trimmed().isEmpty()) {
            return true;
        }

        return canLoad(pFileName);
    }

    return false;
}

//==============================================================================

bool StandardFileManager::isFile(const QString &pFileName) const
{
    // Check whether the given file is of the right type, i.e. whether it can be
    // loaded

    return isFile(pFileName, false);
}

//==============================================================================

StandardFile * StandardFileManager::file(const QString &pFileName) const
{
    // Return the File object, if any, associated with the given file

    return mFiles.value(Core::canonicalFileName(pFileName));
}

//==============================================================================

void StandardFileManager::manage(const QString &pFileName)
{
    // Create the given file and add it to our list of managed files, if we are
    // dealing with a file that is not already managed,

    if ((file(pFileName) == nullptr) && isFile(pFileName, false)) {
        mFiles.insert(Core::canonicalFileName(pFileName), create(pFileName));
    }
}

//==============================================================================

void StandardFileManager::unmanage(const QString &pFileName)
{
    StandardFile *crtFile = file(pFileName);

    if (crtFile != nullptr) {
        // We are dealing with a file, so we can remove it from our list of
        // managed files after having deleted it

        delete crtFile;

        mFiles.remove(Core::canonicalFileName(pFileName));
    }
}

//==============================================================================

void StandardFileManager::save(const QString &pFileName)
{
    // The file is to be saved, so we need to reload it

    StandardFile *crtFile = file(pFileName);

    if (crtFile != nullptr) {
        reload(pFileName, crtFile->isNew());
    }
}

//==============================================================================

void StandardFileManager::reload(const QString &pFileName, bool pForceChecking)
{
    // The file is to be reloaded (either because it has been changed or because
    // one or several of its dependencies has changed), so reload it
    // Note: to reload a file here ensures that our different standard-based
    //       views won't each do it, thus saving time and ensuring that a
    //       standard-based view doesn't forget to do it...

    StandardFile *crtFile = file(pFileName);

    if (crtFile != nullptr) {
        // The file is managed, but should it still be (i.e. can it still be
        // considered as being a file)?

        if (isFile(pFileName, pForceChecking)) {
            crtFile->reload();
        } else {
            unmanage(pFileName);
        }
    } else {
        // The file is not managed, which means that previously it wasn't
        // considered as being a file, but things may be different now, so try
        // to remanage it and load it, if possible

        manage(pFileName);

        crtFile = file(pFileName);

        if (crtFile != nullptr) {
            crtFile->load();
        }
    }
}

//==============================================================================

void StandardFileManager::reload(const QString &pFileName)
{
    // Reload the given file

    reload(pFileName, true);
}

//==============================================================================

void StandardFileManager::rename(const QString &pOldFileName,
                                 const QString &pNewFileName)
{
    // The file has been renamed, so we need to update our files mapping, if
    // needed

    StandardFile *crtFile = file(pOldFileName);

    if (crtFile == nullptr) {
        return;
    }

    mFiles.insert(Core::canonicalFileName(pNewFileName), crtFile);
    mFiles.remove(Core::canonicalFileName(pOldFileName));

    // We also need to ensure that our file object has its file name updated

    crtFile->setFileName(pNewFileName);
}

//==============================================================================

} // namespace StandardSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

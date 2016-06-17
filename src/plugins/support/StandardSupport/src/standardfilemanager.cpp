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

StandardFileManager::StandardFileManager() :
    mFiles(Files())
{
    // Create some connections to keep track of some events related to our
    // 'global' file manager

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    connect(fileManagerInstance, SIGNAL(fileManaged(const QString &)),
            this, SLOT(manageFile(const QString &)));
    connect(fileManagerInstance, SIGNAL(fileUnmanaged(const QString &)),
            this, SLOT(unmanageFile(const QString &)));

    connect(fileManagerInstance, SIGNAL(fileReloaded(const QString &, const bool &)),
            this, SLOT(reloadFile(const QString &, const bool &)));
    connect(fileManagerInstance, SIGNAL(fileRenamed(const QString &, const QString &)),
            this, SLOT(renameFile(const QString &, const QString &)));

    connect(fileManagerInstance, SIGNAL(fileSaved(const QString &)),
            this, SLOT(saveFile(const QString &)));
}

//==============================================================================

StandardFileManager::~StandardFileManager()
{
}

//==============================================================================

bool StandardFileManager::isFile(const QString &pFileName)
{
    // If the given file is already managed, then we consider that it's of the
    // right type (e.g. CellML file), even though it may not be of the right
    // type anymore after having been edited and saved, but in this case it's
    // good to keep considering the file as of the right type, so that the user
    // can continue editing it without any problem, for example

    QString nativeFileName = Core::nativeCanonicalFileName(pFileName);

    if (file(nativeFileName))
        return true;

    // The given file is not managed, so consider it of the right type if it is
    // an empty file (after having been trimmed) or it can be loaded

    QByteArray fileContents;

    if (Core::readFileContentsFromFile(nativeFileName, fileContents)) {
        if (fileContents.trimmed().isEmpty())
            return true;

        return canLoadFile(nativeFileName);
    } else {
        return false;
    }
}

//==============================================================================

QObject * StandardFileManager::file(const QString &pFileName)
{
    // Return the File object, if any, associated with the given file

    return mFiles.value(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

void StandardFileManager::manageFile(const QString &pFileName)
{
    QString nativeFileName = Core::nativeCanonicalFileName(pFileName);

    if (!file(nativeFileName) && isFile(nativeFileName)) {
        // We are dealing with a file, which is not already managed, so we can
        // add it to our list of managed files

        mFiles.insert(nativeFileName, newFile(nativeFileName));
    }
}

//==============================================================================

void StandardFileManager::unmanageFile(const QString &pFileName)
{
    QObject *crtFile = file(pFileName);

    if (crtFile) {
        // We are dealing with a file, so we can remove it from our list of
        // managed files after having deleted it

        delete crtFile;

        mFiles.remove(Core::nativeCanonicalFileName(pFileName));
    }
}

//==============================================================================

void StandardFileManager::reloadFile(const QString &pFileName,
                                     const bool &pFileChanged)
{
    Q_UNUSED(pFileChanged);

    // The file is to be reloaded (either because it has been changed or because
    // one or several of its dependencies has changed), so reload it
    // Note: to reload a file here ensures that our different standard-based
    //       views won't each do it, thus saving time and ensuring that a
    //       standard-based view doesn't forget to do it...

    QObject *crtFile = file(pFileName);

    if (crtFile) {
        // The file is managed, but should it still be (i.e. can it still be
        // considered as being a file)?

        if (isFile(pFileName))
            static_cast<StandardFile *>(crtFile)->reload();
        else
            unmanageFile(pFileName);
    } else {
        // The file is not managed, which means that previously it wasn't
        // considered as being a file, but things may be different now, so try
        // to remanage it and load it, if possible

        manageFile(pFileName);

        crtFile = file(pFileName);

        if (crtFile)
            static_cast<StandardFile *>(crtFile)->load();
    }
}

//==============================================================================

void StandardFileManager::renameFile(const QString &pOldFileName,
                                     const QString &pNewFileName)
{
    // The file has been renamed, so we need to update our files mapping, if
    // needed

    QObject *crtFile = file(pOldFileName);

    if (!crtFile)
        return;

    mFiles.insert(pNewFileName, crtFile);
    mFiles.remove(pOldFileName);

    // We also need to ensure that our file object has its file name updated

    static_cast<StandardFile *>(crtFile)->setFileName(pNewFileName);
}

//==============================================================================

void StandardFileManager::saveFile(const QString &pFileName)
{
    // The file has been (modified and) saved, so we need to reload it

    reloadFile(pFileName, true);
}

//==============================================================================

}   // namespace StandardSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

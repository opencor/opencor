/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// File manager
//==============================================================================

#include "coreutils.h"
#include "filemanager.h"

//==============================================================================

#include <QDir>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

FileManager::FileManager(const int &pTimerInterval)
{
    // Create our timer

    mTimer = new QTimer(this);

    // A connection to handle the timing out of our timer

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(checkFiles()));

    // Start our timer

    mTimer->start(pTimerInterval);
}

//==============================================================================

FileManager::~FileManager()
{
    // Delete some internal objects

    delete mTimer;

    // Remove all the managed files

    foreach (File *file, mFiles)
        delete file;
}

//==============================================================================

FileManager * FileManager::instance()
{
    // Return the 'global' instance of our file manager class

    static FileManager instance;

    return static_cast<FileManager *>(Core::globalInstance("OpenCOR::Core::FileManager::instance()",
                                                           &instance));
}

//==============================================================================

FileManager::Status FileManager::manage(const QString &pFileName)
{
    QString nativeFileName = nativeCanonicalFileName(pFileName);

    if (QFileInfo(nativeFileName).exists()) {
        if (isManaged(nativeFileName)) {
            // The file is already managed, so...

            return AlreadyManaged;
        } else {
            // The file isn't already managed, so add it to our list of managed
            // files

            mFiles << new File(nativeFileName);

            emit fileManaged(nativeFileName);

            return Added;
        }
    } else {
        // The file doesn't exist, so...

        return DoesNotExist;
    }
}

//==============================================================================

FileManager::Status FileManager::unmanage(const QString &pFileName)
{
    QString nativeFileName = nativeCanonicalFileName(pFileName);

    if (QFileInfo(nativeFileName).exists()) {
        File *file = isManaged(nativeFileName);

        if (file) {
            // The file is managed, so we can remove it

            mFiles.removeAt(mFiles.indexOf(file));

            delete file;

            emit fileUnmanaged(nativeFileName);

            return Removed;
        } else {
            // The file isn't managed, so...

            return NotManaged;
        }
    } else {
        // The file doesn't exist, so...

        return NotManaged;
    }
}

//==============================================================================

File * FileManager::isManaged(const QString &pFileName) const
{
    QString nativeFileName = nativeCanonicalFileName(pFileName);

    foreach (File *file, mFiles)
        if (!file->fileName().compare(nativeFileName))
            // The file has been found meaning it is managed

            return file;

    // The file couldn't be found meaning it's not managed

    return 0;
}

//==============================================================================

bool FileManager::isModified(const QString &pFileName) const
{
    // Return whether the file, if it is being managed, has been modified

    File *file = isManaged(nativeCanonicalFileName(pFileName));

    if (file)
        return file->isModified();
    else
        return false;
}

//==============================================================================

void FileManager::setModified(const QString &pFileName, const bool &pModified)
{
    // Set the modified status of the file, should it be managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);

    File *file = isManaged(nativeFileName);

    if (file) {
        // We are dealing with a managed file, so we can check its modified
        // status and update it, if necessary, and then let people know about
        // the new modified status

        if (pModified == file->isModified())
            return;

        file->setModified(pModified);

        emit fileModified(nativeFileName, pModified);
    }
}

//==============================================================================

FileManager::Status FileManager::rename(const QString &pOldFileName,
                                        const QString &pNewFileName)
{
    // Make sure that the file names are different

    QString oldNativeFileName = nativeCanonicalFileName(pOldFileName);
    QString newNativeFileName = nativeCanonicalFileName(pNewFileName);

    if (!newNativeFileName.compare(oldNativeFileName))
        return NotNeeded;

    // Check whether the 'old' file is managed

    File *file = isManaged(oldNativeFileName);

    if (!file) {
        // The 'old' file is not managed, so...

        return NotManaged;
    } else {
        // The 'old' file is managed, so we can rename it

        file->setFileName(newNativeFileName);

        emit fileRenamed(oldNativeFileName, newNativeFileName);

        return Renamed;
    }
}

//==============================================================================

int FileManager::count() const
{
    // Return the number of files currently being managed

    return mFiles.count();
}

//==============================================================================

void FileManager::checkFiles()
{
    // Check our various files

    foreach (File *file, mFiles)
        switch (file->check())
        {
            case File::Changed:
                // The contents of the file has changed, so...

                emit fileContentsChanged(file->fileName());

                break;
            case File::Deleted:
                // The file has been deleted, so...

                emit fileDeleted(file->fileName());

                break;
            default:
                // The file is unchanged, so do nothing...

                ;
        }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

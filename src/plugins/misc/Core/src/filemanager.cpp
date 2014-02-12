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

#include "cliutils.h"
#include "filemanager.h"

//==============================================================================

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QTemporaryFile>
#include <QTextStream>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

FileManager::FileManager(const int &pTimerInterval) :
    mFiles(QList<File *>()),
    mFilesReadable(QMap<QString, bool>()),
    mFilesWritable(QMap<QString, bool>())
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

FileManager::Status FileManager::manage(const QString &pFileName,
                                        const bool &pNew)
{
    // Manage the given file, should it not be already managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);

    if (QFileInfo(nativeFileName).exists()) {
        if (isManaged(nativeFileName)) {
            // The file is already managed, so...

            return AlreadyManaged;
        } else {
            // The file isn't already managed, so add it to our list of managed
            // files, let people know about it being now managed

            mFiles << new File(nativeFileName, pNew);

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
    // Unmanage the given file, should it be managed

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
    // Return whether the given file is managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);

    foreach (File *file, mFiles)
        if (!file->fileName().compare(nativeFileName))
            // The file has been found meaning it is managed

            return file;

    // The file couldn't be found meaning it's not managed

    return 0;
}

//==============================================================================

bool FileManager::isNew(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is new

    File *file = isManaged(nativeCanonicalFileName(pFileName));

    if (file)
        return file->isNew();
    else
        return false;
}

//==============================================================================

int FileManager::newIndex(const QString &pFileName) const
{
    // Return the given file's new index, if it is being managed

    File *file = isManaged(nativeCanonicalFileName(pFileName));

    if (file)
        return file->newIndex();
    else
        return 0;
}

//==============================================================================

bool FileManager::isModified(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, has been modified

    File *file = isManaged(nativeCanonicalFileName(pFileName));

    if (file)
        return file->isModified();
    else
        return false;
}

//==============================================================================

void FileManager::setModified(const QString &pFileName, const bool &pModified)
{
    // Set the modified status of the given file, should it be managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *file = isManaged(nativeFileName);

    if (file) {
        file->setModified(pModified);

        emit fileModified(nativeFileName, pModified);
    }
}

//==============================================================================

bool FileManager::isReadable(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is readable

    File *file = isManaged(nativeCanonicalFileName(pFileName));

    if (file)
        return file->isReadable();
    else
        return false;
}

//==============================================================================

bool FileManager::isWritable(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is writable

    File *file = isManaged(nativeCanonicalFileName(pFileName));

    if (file)
        return file->isWritable();
    else
        return false;
}

//==============================================================================

bool FileManager::isReadableAndWritable(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is readable and
    // writable

    return isReadable(pFileName) && isWritable(pFileName);
}

//==============================================================================

bool FileManager::isLocked(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is locked

    File *file = isManaged(nativeCanonicalFileName(pFileName));

    if (file)
        return file->isLocked();
    else
        return false;
}

//==============================================================================

FileManager::Status FileManager::setLocked(const QString &pFileName,
                                           const bool &pLocked)
{
    // Set the locked status of the given file, should it be managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *file = isManaged(nativeFileName);

    if (file) {
        File::Status status = file->setLocked(pLocked);

        if (status == File::LockedSet)
            emit filePermissionsChanged(nativeFileName);

        if (status == File::LockedNotNeeded)
            return LockedNotNeeded;
        else if (status == File::LockedSet)
            return LockedSet;
        else
            return LockedNotSet;
    } else {
        return NotManaged;
    }
}

//==============================================================================

void FileManager::reload(const QString &pFileName)
{
    // Make sure that the given file is managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);

    if (isManaged(nativeFileName))
        // The file is managed, so let people know that it should be reloaded

        emit fileReloaded(nativeFileName);
}

//==============================================================================

FileManager::Status FileManager::rename(const QString &pOldFileName,
                                        const QString &pNewFileName)
{
    // Make sure that the given file names are different

    QString oldNativeFileName = nativeCanonicalFileName(pOldFileName);
    QString newNativeFileName = nativeCanonicalFileName(pNewFileName);

    if (!newNativeFileName.compare(oldNativeFileName))
        return RenamingNotNeeded;

    // Make sure that the given 'old' file is managed

    File *file = isManaged(oldNativeFileName);

    if (file) {
        // The 'old' file is managed, so we can rename it and let people know
        // about it

        file->setFileName(newNativeFileName);

        emit fileRenamed(oldNativeFileName, newNativeFileName);

        return Renamed;
    } else {
        return NotManaged;
    }
}

//==============================================================================

FileManager::Status FileManager::duplicate(const QString &pFileName)
{
    // Make sure that the given file is managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *file = isManaged(nativeFileName);

    if (file) {
        // The file is managed, so retrieve its contents

        QFile file(pFileName);

        if (file.open(QIODevice::ReadOnly)) {
            QString fileContents = QTextStream(&file).readAll();

            file.close();

            // Now, we can create a new file, which contents will be that of our
            // given file

            QTemporaryFile duplicatedFile(QDir::tempPath()+QDir::separator()+QFileInfo(qApp->applicationFilePath()).baseName()+"_XXXXXX."+QFileInfo(pFileName).completeSuffix());

            if (duplicatedFile.open()) {
                duplicatedFile.setAutoRemove(false);
                // Note: by default, a temporary file is to autoremove itself,
                //       but we clearly don't want that here...

                QTextStream duplicatedFileOut(&duplicatedFile);

                duplicatedFileOut << fileContents;

                duplicatedFile.close();

                // Let people know that we have duplicated a file

                emit fileDuplicated(duplicatedFile.fileName());

                return Duplicated;
            } else {
                return NotDuplicated;
            }
        } else {
            return NotDuplicated;
        }
    } else {
        return NotManaged;
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
    // Check our various files, as well as their locked status

    foreach (File *file, mFiles) {
        switch (file->check()) {
            case File::Changed:
                // The file has changed, so let people know about it

                emit fileChanged(file->fileName());

                break;
            case File::Deleted:
                // The file has been deleted, so let people know about it

                emit fileDeleted(file->fileName());

                break;
            default:
                // The file is unchanged, so do nothing...

                ;
        }

        bool fileReadable = isLocked(file->fileName());
        bool fileWritable = isLocked(file->fileName());

        if (    (fileReadable != mFilesReadable.value(file->fileName(), false))
            ||  (fileWritable != mFilesWritable.value(file->fileName(), false))
            || !(   mFilesReadable.contains(file->fileName())
                 && mFilesWritable.contains(file->fileName()))) {
            emit filePermissionsChanged(file->fileName());

            mFilesReadable.insert(file->fileName(), fileReadable);
            mFilesWritable.insert(file->fileName(), fileWritable);
        }
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

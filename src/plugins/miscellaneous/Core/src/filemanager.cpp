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

#include "corecliutils.h"
#include "filemanager.h"

//==============================================================================

#include <QApplication>
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

FileManager::FileManager() :
    mCanCheckFiles(true),
    mFiles(QMap<QString, File *>()),
    mFilesReadable(QMap<QString, bool>()),
    mFilesWritable(QMap<QString, bool>())
{
    // Create our timer

    mTimer = new QTimer(this);

    // A connection to handle the timing out of our timer

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(checkFiles()));
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
                                        const File::Type &pType,
                                        const QString &pUrl)
{
    // Manage the given file, should it not be already managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);

    if (QFile::exists(nativeFileName)) {
        if (file(nativeFileName)) {
            return AlreadyManaged;
        } else {
            // The file isn't already managed, so add it to our list of managed
            // files and let people know about it being now managed

            mFiles.insert(nativeFileName, new File(nativeFileName, pType, pUrl));

            if (!mTimer->isActive())
                mTimer->start(1000);

            emit fileManaged(nativeFileName);

            return Added;
        }
    } else {
        return DoesNotExist;
    }
}

//==============================================================================

FileManager::Status FileManager::unmanage(const QString &pFileName)
{
    // Unmanage the given file, should it be managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *nativeFile = file(nativeFileName);

    if (nativeFile) {
        // The file is managed, so we can remove it

        mFiles.remove(nativeFileName);;

        delete nativeFile;

        if (mFiles.isEmpty())
            mTimer->stop();

        emit fileUnmanaged(nativeFileName);

        return Removed;
    } else {
        return NotManaged;
    }
}

//==============================================================================

File * FileManager::file(const QString &pFileName) const
{
    // Return the File object, if any, associated with the given file

    return mFiles.value(nativeCanonicalFileName(pFileName), 0);
}

//==============================================================================

bool FileManager::canCheckFiles() const
{
    // Return whether we can check files

    return mCanCheckFiles;
}

//==============================================================================

void FileManager::setCanCheckFiles(const bool &pCanCheckFiles)
{
    // Set whether we can check files

    mCanCheckFiles = pCanCheckFiles;
}

//==============================================================================

QString FileManager::sha1(const QString &pFileName) const
{
    // Return the SHA-1 value of the given file, should it be managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->sha1();
    else
        return QString();
}

//==============================================================================

void FileManager::reset(const QString &pFileName)
{
    // Reset the given file, should it be managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        nativeFile->reset();
}

//==============================================================================

int FileManager::newIndex(const QString &pFileName) const
{
    // Return the given file's new index, if it is being managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->newIndex();
    else
        return 0;
}

//==============================================================================

QString FileManager::url(const QString &pFileName) const
{
    // Return the given file's URL, if it is being managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->url();
    else
        return QString();
}

//==============================================================================

bool FileManager::isDifferent(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is different from
    // its corresponding physical version

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->isDifferent();
    else
        return false;
}

//==============================================================================

bool FileManager::isDifferent(const QString &pFileName,
                              const QString &pFileContents) const
{
    // Return whether the given file, if it is being managed, has the same
    // contents has the given one

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->isDifferent(pFileContents);
    else
        return false;
}

//==============================================================================

bool FileManager::isNew(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is new

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->isNew();
    else
        return false;
}

//==============================================================================

bool FileManager::isRemote(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is a remote one

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->isRemote();
    else
        return false;
}

//==============================================================================

bool FileManager::isModified(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, has been modified

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->isModified();
    else
        return false;
}

//==============================================================================

bool FileManager::isNewOrModified(const QString &pFileName) const
{
    // Return whether the given file is new or modified

    return isNew(pFileName) || isModified(pFileName);
}

//==============================================================================

void FileManager::makeNew(const QString &pFileName)
{
    // Make the given file new, should it be managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile) {
        QString newFileName;

        if (newFile(QString(), newFileName))
            nativeFile->makeNew(newFileName);
    }
}

//==============================================================================

void FileManager::setModified(const QString &pFileName, const bool &pModified)
{
    // Set the modified state of the given file, should it be managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *nativeFile = file(nativeFileName);

    if (nativeFile && nativeFile->setModified(pModified))
        emit fileModified(nativeFileName);
}

//==============================================================================

bool FileManager::isReadable(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is readable

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->isReadable();
    else
        return false;
}

//==============================================================================

bool FileManager::isWritable(const QString &pFileName) const
{
    // Return whether the given file, if it is being managed, is writable

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->isWritable();
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

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->isLocked();
    else
        return false;
}

//==============================================================================

FileManager::Status FileManager::setLocked(const QString &pFileName,
                                           const bool &pLocked)
{
    // Set the locked status of the given file, should it be managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *nativeFile = file(nativeFileName);

    if (nativeFile) {
        File::Status status = nativeFile->setLocked(pLocked);

        if (status == File::LockedSet)
            emitFilePermissionsChanged(nativeFileName);

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
    File *nativeFile = file(nativeFileName);

    if (nativeFile) {
        // The file is managed, so reset its settings and let people know that
        // it should be reloaded

        nativeFile->reset();

        emit fileReloaded(nativeFileName);
    }
}

//==============================================================================

bool FileManager::newFile(const QString &pContents, QString &pFileName)
{
    // Retrieve a temporary file name for our new file

    QString newFileName = Core::temporaryFileName();

    // Create a new file with the given contents

    if (Core::writeTextToFile(newFileName, pContents)) {
        pFileName = newFileName;

        return true;
    } else {
        pFileName = QString();

        return false;
    }
}

//==============================================================================

FileManager::Status FileManager::create(const QString &pUrl,
                                        const QString &pContents)
{
    // Create a new file

    QString newFileName;

    if (newFile(pContents, newFileName)) {
        // Let people know that we have created a file

        emit fileCreated(newFileName, pUrl);

        return Created;
    } else {
        return NotCreated;
    }
}

//==============================================================================

FileManager::Status FileManager::rename(const QString &pOldFileName,
                                        const QString &pNewFileName)
{
    // Make sure that the given 'old' file is managed

    QString oldNativeFileName = nativeCanonicalFileName(pOldFileName);
    File *nativeFile = file(oldNativeFileName);

    if (nativeFile) {
        // The 'old' file is managed, so rename it and let people know about it

        QString newNativeFileName = nativeCanonicalFileName(pNewFileName);

        if (nativeFile->setFileName(newNativeFileName)) {
            mFiles.insert(newNativeFileName, mFiles.value(oldNativeFileName));
            mFiles.remove(oldNativeFileName);

            emit fileRenamed(oldNativeFileName, newNativeFileName);

            return Renamed;
        } else {
            return RenamingNotNeeded;
        }
    } else {
        return NotManaged;
    }
}

//==============================================================================

FileManager::Status FileManager::duplicate(const QString &pFileName)
{
    // Make sure that the given file is managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile) {
        // The file is managed, so retrieve its contents

        QString fileContents;

        if (Core::readTextFromFile(pFileName, fileContents)) {
            // Now, we can create a new file, which contents will be that of our
            // given file

            QString newFileName;

            if (newFile(fileContents, newFileName)) {
                // Let people know that we have duplicated a file

                emit fileDuplicated(newFileName);

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

void FileManager::save(const QString &pFileName)
{
    // Make sure that the given file is managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *nativeFile = file(nativeFileName);

    if (nativeFile) {
        // The file is managed, so reset its settings and let people know that
        // it has been saved

        nativeFile->reset();

        emit fileSaved(nativeFileName);
    }
}

//==============================================================================

int FileManager::count() const
{
    // Return the number of files currently being managed

    return mFiles.count();
}

//==============================================================================

void FileManager::emitFilePermissionsChanged(const QString &pFileName)
{
    // Update our internals and let people know that the given file has had its
    // permissions changed

    mFilesReadable.insert(pFileName, isReadable(pFileName));
    mFilesWritable.insert(pFileName, isWritable(pFileName));

    emit filePermissionsChanged(pFileName);
}

//==============================================================================

void FileManager::checkFiles()
{
    // We only want to check our files if we can check files and if there is no
    // currently active dialog box (which requires at least one top level
    // widget)

    if (   !mCanCheckFiles
        || !QApplication::topLevelWidgets().count()
        ||  QApplication::activePopupWidget())
        return;

    // Check our various files, as well as their locked status, but only if they
    // are not being ignored

    foreach (File *file, mFiles) {
        QString fileName = file->fileName();

        switch (file->check()) {
        case File::Changed:
            // The file has changed, so let people know about it

            emit fileChanged(fileName);

            break;
        case File::Deleted:
            // The file has been deleted, so let people know about it

            emit fileDeleted(fileName);

            break;
        default:
            // The file has neither changed nor been deleted, so check whether
            // its permissions have changed

            if (    (mFilesReadable.value(fileName, false) != isReadable(fileName))
                ||  (mFilesWritable.value(fileName, false) != isWritable(fileName))
                || !(   mFilesReadable.contains(fileName)
                     && mFilesWritable.contains(fileName))) {
                emitFilePermissionsChanged(fileName);
            }
        }
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

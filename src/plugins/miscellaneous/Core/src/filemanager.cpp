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
// File manager
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QApplication>
#include <QFile>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

FileManager::FileManager() :
    mFiles(QMap<QString, File *>()),
    mFilesReadable(QMap<QString, bool>()),
    mFilesWritable(QMap<QString, bool>()),
    mCheckFilesEnabled(true)
{
    // Create our timer

    mTimer = new QTimer(this);

    // A connection to handle the timing out of our timer

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(checkFiles()));

    // Keep track of when OpenCOR gets/loses the focus
    // Note: the focusWindowChanged() signal comes from QGuiApplication, so we
    //       need to check whether we are running the console version of OpenCOR
    //       or its GUI version...

    if (dynamic_cast<QGuiApplication *>(QCoreApplication::instance())) {
        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(focusWindowChanged()));
    }
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

void FileManager::startStopTimer()
{
    // Start our timer if OpenCOR is active and we have files, or stop it if
    // either OpenCOR is not active or we don't have files anymore
    // Note #1: if we are to start our timer, then we check files first since
    //          waiting one second may seem long to a user (when some files have
    //          changed and after having reactivated OpenCOR)...
    // Note #2: checking files may result in a message box being shown and,
    //          therefore, in a focusWindowChanged() signal being emitted. To
    //          handle that signal would result in reentry, so we temporarily
    //          disable our handling of it...

    if (   !mTimer->isActive()
        &&  Core::opencorActive() && !mFiles.isEmpty()) {
        disconnect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                   this, SLOT(focusWindowChanged()));

        checkFiles();

        connect(qApp, SIGNAL(focusWindowChanged(QWindow *)),
                this, SLOT(focusWindowChanged()));

        mTimer->start(1000);
    } else if (   mTimer->isActive()
               && (!Core::opencorActive() || mFiles.isEmpty())) {
        mTimer->stop();
    }
}

//==============================================================================

FileManager * FileManager::instance()
{
    // Return the 'global' instance of our file manager class

    static FileManager instance;

    return static_cast<FileManager *>(globalInstance("OpenCOR::Core::FileManager::instance()",
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

            startStopTimer();

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

        mFiles.remove(nativeFileName);

        delete nativeFile;

        startStopTimer();

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

bool FileManager::isLocalNewOrModified(const QString &pFileName) const
{
    // Return whether the given file is a local one, as well as is new or
    // modified

    return !isRemote(pFileName) && (isNew(pFileName) || isModified(pFileName));
}

//==============================================================================

void FileManager::makeNew(const QString &pFileName)
{
    // Make the given file new, should it be managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile) {
        QString fileName;

        if (newFile(fileName))
            nativeFile->makeNew(fileName);
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

void FileManager::setDependenciesModified(const QString &pFileName,
                                          const bool &pModified)
{
    // Set the dependencies modified state of the given file, should it be
    // managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *nativeFile = file(nativeFileName);

    if (nativeFile)
        nativeFile->setDependenciesModified(pModified);
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

QStringList FileManager::dependencies(const QString &pFileName) const
{
    // Return the given file's dependencies, should it be managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        return nativeFile->dependencies();
    else
        return QStringList();
}

//==============================================================================

void FileManager::setDependencies(const QString &pFileName,
                                  const QStringList &pDependencies)
{
    // Set the dependencies of the given file, should it be managed

    File *nativeFile = file(nativeCanonicalFileName(pFileName));

    if (nativeFile)
        nativeFile->setDependencies(pDependencies);
}

//==============================================================================

void FileManager::reload(const QString &pFileName,
                         const bool &pForceFileChanged)
{
    // Make sure that the given file is managed

    QString nativeFileName = nativeCanonicalFileName(pFileName);
    File *nativeFile = file(nativeFileName);

    if (nativeFile) {
        // The file is managed, so determine whether the file itself has been
        // modified, reset its settings and let people know that it should be
        // reloaded

        File::Status nativeFileStatus = nativeFile->check();

        nativeFile->reset();

        emit fileReloaded(nativeFileName,
                              pForceFileChanged
                          || (nativeFileStatus == File::Changed) || (nativeFileStatus == File::AllChanged));
    }
}

//==============================================================================

bool FileManager::newFile(QString &pFileName, const QByteArray &pContents)
{
    // Retrieve a temporary file name for our new file

    QString fileName = temporaryFileName();

    // Create a new file with the given contents

    if (writeFileContentsToFile(fileName, pContents)) {
        pFileName = fileName;

        return true;
    } else {
        pFileName = QString();

        return false;
    }
}

//==============================================================================

FileManager::Status FileManager::create(const QString &pUrl,
                                        const QByteArray &pContents)
{
    // Create a new file

    QString fileName;

    if (newFile(fileName, pContents)) {
        // Let people know that we have created a new file

        emit fileCreated(fileName, pUrl);

        return Created;
    } else {
        return NotCreated;
    }
}

//==============================================================================

FileManager::Status FileManager::create(const QString &pUrl,
                                        const QString &pContents)
{
    // Create a new file

    return create(pUrl, pContents.toUtf8());
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

        QByteArray fileContents;

        if (readFileContentsFromFile(pFileName, fileContents)) {
            // Now, we can create a new file, which contents will be that of our
            // given file

            QString fileName;

            if (newFile(fileName, fileContents)) {
                // Let people know that we have duplicated a file

                emit fileDuplicated(fileName);

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

        nativeFile->reset(false);

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

void FileManager::setCheckFilesEnabled(const bool &pCheckFilesEnabled)
{
    // Specify whether we can check files

    mCheckFilesEnabled = pCheckFilesEnabled;
}

//==============================================================================

void FileManager::focusWindowChanged()
{
    // Start/stop our timer

    startStopTimer();
}

//==============================================================================

void FileManager::checkFiles()
{
    // Make sure that OpenCOR is active
    // Note: indeed, although we try our best to enable/disable our timer as
    //       needed, there are cases (e.g. a QFileDialog is opened) that don't
    //       result in the focusWindowChanged() signal being emitted, which
    //       means that we can't enable/disable our timer in those acses, hence
    //       our checking that OpenCOR is really active indeed...

    if (!Core::opencorActive() || !mCheckFilesEnabled)
        return;

    // Check our various files, as well as their locked status, but only if they
    // are not being ignored

    foreach (File *file, mFiles) {
        QString fileName = file->fileName();
        File::Status fileStatus = file->check();

        switch (fileStatus) {
        case File::Changed:
        case File::DependenciesChanged:
        case File::AllChanged:
            // The file and/or one or several of its dependencies has changed,
            // so let people know about it

            emit fileChanged(fileName,
                             (fileStatus == File::Changed) || (fileStatus == File::AllChanged),
                             (fileStatus == File::DependenciesChanged) || (fileStatus == File::AllChanged));

            break;
        case File::Unchanged:
            // The file has neither changed nor been deleted, so check whether
            // its permissions have changed

            if (    (mFilesReadable.value(fileName, false) != isReadable(fileName))
                ||  (mFilesWritable.value(fileName, false) != isWritable(fileName))
                || !(   mFilesReadable.contains(fileName)
                     && mFilesWritable.contains(fileName))) {
                emitFilePermissionsChanged(fileName);
            }

            break;
        case File::Deleted:
            // The file has been deleted, so let people know about it

            emit fileDeleted(fileName);

            break;
        default:
            // Not a relevant status, so do nothing

            ;
        }
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

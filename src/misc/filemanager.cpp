#include "filemanager.h"

#include <QCryptographicHash>
#include <QFileInfo>
#include <QTextStream>
#include <QTimer>

File::File(const QString &pFileName) :
    mFileName(pFileName),
    mSha1(sha1())
{
}

QString File::fileName() const
{
    // Return the file name of the file

    return mFileName;
}

File::FileStatus File::check()
{
    // Get the current SHA1 value for the file and compare it to its currently
    // stored value

    QString crtSha1 = sha1();

    if (crtSha1.isEmpty()) {
        // The current SHA1 value is empty, meaning that the file has been
        // deleted, so update our currently stored value and let the caller know

        mSha1 = crtSha1;

        return File::Deleted;
    } else if (!crtSha1.compare(mSha1)) {
        // The current SHA1 value is the same as our currently stored value,
        // so...

        return File::Unchanged;
    } else {
        // The current SHA1 value is different from our currently stored value,
        // so update the latter and make the caller aware of the change

        mSha1 = crtSha1;

        return File::Changed;
    }
}

QString File::sha1() const
{
    // Compute the SHA1 value for the file, if it still exists

    if (QFileInfo(mFileName).exists()) {
        // The file still exists, so we can go ahead and compute its SHA1 value

        QFile file(mFileName);

        file.open(QIODevice::ReadOnly);

        QString res = QCryptographicHash::hash(QTextStream(&file).readAll().toLatin1(),
                                               QCryptographicHash::Sha1).toHex();

        file.close();

        return res;
    } else {
        // The file doesn't exist anymore, so...

        return QString();
    }
}

FileManager::FileManager(const int &pTimerInterval)
{
    // Create our timer

    mTimer = new QTimer(this);

    // A connection to handle the timing out of our timer

    connect(mTimer, SIGNAL(timeout()), this, SLOT(check()));

    // Start our timer

    mTimer->start(pTimerInterval);
}

FileManager::~FileManager()
{
    // Delete the timer

    delete mTimer;

    // Remove all the managed files

    foreach (File *file, mFiles)
        delete file;
}

FileManager::ManageStatus FileManager::manage(const QString &pFileName)
{
    QFileInfo fileInfo = pFileName;

    if (fileInfo.exists()) {
        if (isManaged(fileInfo.canonicalFilePath())) {
            // The file is already managed, so...

            return AlreadyManaged;
        } else {
            // The file isn't already managed, so add it to our list of managed
            // files

            mFiles.append(new File(fileInfo.canonicalFilePath()));

            return Added;
        }
    } else {
        // The file doesn't exist, so...

        return DoesNotExist;
    }
}

FileManager::UnmanageStatus FileManager::unmanage(const QString &pFileName)
{
    QFileInfo fileInfo = pFileName;

    if (fileInfo.exists()) {
        File *file = isManaged(fileInfo.canonicalFilePath());

        if (file) {
            // The file is managed, so we can remove it

            mFiles.removeAt(mFiles.indexOf(file));

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

File * FileManager::isManaged(const QString &pFileName)
{
    foreach (File *file, mFiles)
        if (file->fileName() == pFileName)
            // The file has been found meaning it is managed

            return file;

    // The file couldn't be found meaning it's not managed

    return 0;
}

int FileManager::count() const
{
    // Return the number of files currently being managed

    return mFiles.count();
}

void FileManager::check()
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

            break;
        }
}

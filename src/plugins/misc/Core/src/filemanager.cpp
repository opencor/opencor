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

    connect(mTimer, SIGNAL(timeout()), this, SLOT(checkFiles()));

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

    return (FileManager *) Core::instance("OpenCOR::Core::FileManager", &instance);
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
        if (file->fileName() == nativeFileName)
            // The file has been found meaning it is managed

            return file;

    // The file couldn't be found meaning it's not managed

    return 0;
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

                break;
        }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// File manager
//==============================================================================

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QList>
#include <QObject>

//==============================================================================

class QString;
class QTimer;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static const QString FileSystemMimeType = "text/uri-list";

//==============================================================================

class File
{
public:
    enum Status
    {
        Changed,
        Unchanged,
        Deleted
    };

    explicit File(const QString &pFileName);

    QString fileName() const;

    Status check();

private:
    QString mFileName;
    QString mSha1;

    QString sha1() const;
};

//==============================================================================

class CORE_EXPORT FileManager : public QObject
{
    Q_OBJECT

public:
    enum Status
    {
        // As a result of managing a file

        DoesNotExist,
        Added,
        AlreadyManaged,

        // As a result of unmanaging a file

        Removed,
        NotManaged
    };

    explicit FileManager(const int &pTimerInterval = 1000);
    ~FileManager();

    static FileManager * instance();

    Status manage(const QString &pFileName);
    Status unmanage(const QString &pFileName);

    int count() const;

private:
    QTimer *mTimer;
    QList<File *> mFiles;

    File * isManaged(const QString &pFileName) const;

Q_SIGNALS:
    void fileContentsChanged(const QString &pFileName);
    void fileDeleted(const QString &pFileName);

private Q_SLOTS:
    void checkFiles();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

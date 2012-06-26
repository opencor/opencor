//==============================================================================
// File manager
//==============================================================================

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

//==============================================================================

#include "coreglobal.h"
#include "file.h"

//==============================================================================

#include <QObject>

//==============================================================================

class QTimer;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static const QString FileSystemMimeType = "text/uri-list";

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

    File * isManaged(const QString &pFileName) const;

    bool isModified(const QString &pFileName) const;

    int count() const;

private:
    QTimer *mTimer;
    QList<File *> mFiles;

Q_SIGNALS:
    void fileManaged(const QString &pFileName);
    void fileUnmanaged(const QString &pFileName);

    void fileContentsChanged(const QString &pFileName);
    void fileDeleted(const QString &pFileName);

    void fileModified(const QString &pFileName, const bool &pModified);

public Q_SLOTS:
    void setModified(const QString &pFileName, const bool &pModified);

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

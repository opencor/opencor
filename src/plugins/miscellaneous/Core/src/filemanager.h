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

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "file.h"

//==============================================================================

#include <QMap>
#include <QObject>

//==============================================================================

class QTimer;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static const auto FileSystemMimeType = QStringLiteral("text/uri-list");

//==============================================================================

class CORE_EXPORT FileManager : public QObject
{
    Q_OBJECT

public:
    enum Status {
        // As a result of managing a file

        DoesNotExist,
        Added,
        AlreadyManaged,

        // As a result of unmanaging a file

        Removed,
        NotManaged,

        // As a result of updating the locked status of a file

        LockedNotNeeded,
        LockedSet,
        LockedNotSet,

        // As a result of creating a file

        Created,
        NotCreated,

        // As a result of renaming a file

        Renamed,
        RenamingNotNeeded,

        // As a result of duplicating a file

        Duplicated,
        NotDuplicated
    };

    explicit FileManager();
    ~FileManager();

    static FileManager * instance();

    Status manage(const QString &pFileName,
                  const File::Type &pType = File::Local,
                  const QString &pUrl = QString());
    Status unmanage(const QString &pFileName);

    File * file(const QString &pFileName) const;

    QString sha1(const QString &pFileName) const;

    void reset(const QString &pFileName);

    int newIndex(const QString &pFileName) const;
    QString url(const QString &pFileName) const;

    bool isDifferent(const QString &pFileName) const;
    bool isDifferent(const QString &pFileName,
                     const QString &pFileContents) const;

    bool isNew(const QString &pFileName) const;
    bool isRemote(const QString &pFileName) const;
    bool isModified(const QString &pFileName) const;
    bool isNewOrModified(const QString &pFileName) const;
    bool isLocalNewOrModified(const QString &pFileName) const;

    void makeNew(const QString &pFileName);

    void setModified(const QString &pFileName, const bool &pModified);
    void setDependenciesModified(const QString &pFileName,
                                 const bool &pModified);

    bool isReadable(const QString &pFileName) const;
    bool isWritable(const QString &pFileName) const;
    bool isReadableAndWritable(const QString &pFileName) const;

    bool isLocked(const QString &pFileName) const;
    Status setLocked(const QString &pFileName, const bool &pLocked);

    QStringList dependencies(const QString &pFileName) const;
    void setDependencies(const QString &pFileName, const QStringList &pDependencies);

    void reload(const QString &pFileName,
                const bool &pForceFileChanged = false);

    Status create(const QString &pUrl = QString(),
                  const QByteArray &pContents = QByteArray());
    Status create(const QString &pUrl, const QString &pContents);
    Status rename(const QString &pOldFileName, const QString &pNewFileName);
    Status duplicate(const QString &pFileName);

    void save(const QString &pFileName);

    int count() const;

    void setCheckFilesEnabled(const bool &pCheckFilesEnabled);

private:
    QTimer *mTimer;

    QMap<QString, File *> mFiles;

    QMap<QString, bool> mFilesReadable;
    QMap<QString, bool> mFilesWritable;

    bool mCheckFilesEnabled;

    void startStopTimer();

    bool newFile(QString &pFileName,
                 const QByteArray &pContents = QByteArray());

    void emitFilePermissionsChanged(const QString &pFileName);

signals:
    void fileManaged(const QString &pFileName);
    void fileUnmanaged(const QString &pFileName);

    void fileChanged(const QString &pFileName, const bool &pFileChanged,
                     const bool &pDependenciesChanged);
    void fileDeleted(const QString &pFileName);

    void filePermissionsChanged(const QString &pFileName);
    void fileModified(const QString &pFileName);

    void fileReloaded(const QString &pFileName, const bool &pFileChanged);

    void fileCreated(const QString &pFileName, const QString &pUrl);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileDuplicated(const QString &pFileName);

    void fileSaved(const QString &pFileName);

private slots:
    void focusWindowChanged();

    void checkFiles();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

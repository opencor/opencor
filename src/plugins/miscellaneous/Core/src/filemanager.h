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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

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

    bool canCheckFiles() const;
    void setCanCheckFiles(const bool &pCanCheckFiles);

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

    void makeNew(const QString &pFileName);

    void setModified(const QString &pFileName, const bool &pModified);

    bool isReadable(const QString &pFileName) const;
    bool isWritable(const QString &pFileName) const;
    bool isReadableAndWritable(const QString &pFileName) const;

    bool isLocked(const QString &pFileName) const;
    Status setLocked(const QString &pFileName, const bool &pLocked);

    void reload(const QString &pFileName);

    Status create(const QString &pUrl = QString(),
                  const QString &pContents = QString());
    Status rename(const QString &pOldFileName, const QString &pNewFileName);
    Status duplicate(const QString &pFileName);

    void save(const QString &pFileName);

    int count() const;

    void emitFilePermissionsChanged(const QString &pFileName);

private:
    bool mCanCheckFiles;

    QTimer *mTimer;

    QMap<QString, File *> mFiles;

    QMap<QString, bool> mFilesReadable;
    QMap<QString, bool> mFilesWritable;

    bool newFile(const QString &pContents, QString &pFileName);

Q_SIGNALS:
    void fileManaged(const QString &pFileName);
    void fileUnmanaged(const QString &pFileName);

    void fileChanged(const QString &pFileName);
    void fileDeleted(const QString &pFileName);

    void filePermissionsChanged(const QString &pFileName);
    void fileModified(const QString &pFileName);

    void fileReloaded(const QString &pFileName);

    void fileCreated(const QString &pFileName, const QString &pUrl);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileDuplicated(const QString &pFileName);

    void fileSaved(const QString &pFileName);

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

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
// File
//==============================================================================

#include "cliutils.h"
#include "file.h"

//==============================================================================

#include <QCryptographicHash>
#include <QFile>
#include <QFileDevice>
#include <QFileInfo>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

File::File(const QString &pFileName, const bool &pNew) :
    mFileName(nativeCanonicalFileName(pFileName)),
    mSha1(sha1()),
    mModified(false)
{
    static int newIndex = 0;

    mNewIndex = pNew?++newIndex:0;
}

//==============================================================================

QString File::fileName() const
{
    // Return the file name of the file

    return mFileName;
}

//==============================================================================

void File::setFileName(const QString &pFileName)
{
    // Set the new file name of the file

    mFileName = pFileName;

    // Also update our SHA-1 value in case the physical state of our file has
    // changed

    mSha1 = sha1();
}

//==============================================================================

File::Status File::check()
{
    // Retrieve the SHA-1 value of our file and compare it to its stored value

    QString newSha1 = sha1();

    if (!newSha1.compare(mSha1)) {
        // The SHA-1 values are the same, so...

        return File::Unchanged;
    } else {
        // We have a new SHA-1 value, so keep track of it

        mSha1 = newSha1;

        if (newSha1.isEmpty())
            // The SHA-1 value of our file is now empty, which means that our
            // file has been deleted

            return File::Deleted;
        else
            // The SHA-1 value of our file is different from our stored value,
            // which means that our file has changed

            return File::Changed;
    }
}

//==============================================================================

QString File::sha1() const
{
    // Compute the SHA-1 value for the file, if it still exists

    if (QFileInfo(mFileName).exists()) {
        // The file still exists, so we can go ahead and compute its SHA-1 value

        QFile file(mFileName);

        file.open(QIODevice::ReadOnly);

        QString res = QCryptographicHash::hash(QTextStream(&file).readAll().toUtf8(),
                                               QCryptographicHash::Sha1).toHex();

        file.close();

        return res;
    } else {
        // The file doesn't exist anymore, so...

        return QString();
    }
}

//==============================================================================

bool File::isNew() const
{
    // Return whether the file is new

    return mNewIndex != 0;
}

//==============================================================================

int File::newIndex() const
{
    // Return the file's new index

    return mNewIndex;
}

//==============================================================================

bool File::isModified() const
{
    // Return whether the file has been modified

    return mModified;
}

//==============================================================================

void File::setModified(const bool &pModified)
{
    // Set the modified status of the file

    mModified = pModified;

    // Also update our SHA-1 value in case the physical state of our file has
    // changed

    mSha1 = sha1();
}

//==============================================================================

bool File::isReadable() const
{
    // Return whether the file is readable

    return QFileInfo(mFileName).isReadable();
}

//==============================================================================

bool File::isWritable() const
{
    // Return whether the file is writable

    return QFileInfo(mFileName).isWritable();
}

//==============================================================================

bool File::isReadableAndWritable() const
{
    // Return whether the file is readable and writable

    return isReadable() && isWritable();
}

//==============================================================================

bool File::isLocked() const
{
    // Return whether the file is locked

    return !QFileInfo(mFileName).isWritable();
}

//==============================================================================

File::Status File::setLocked(const bool &pLocked)
{
    // Set the locked status of the file, but only if it is readable

    if (pLocked == isLocked())
        return LockedNotNeeded;

    QFileDevice::Permissions newPermissions = QFile::permissions(mFileName);

    if (pLocked) {
        if (newPermissions & QFileDevice::WriteOwner)
            newPermissions ^= QFileDevice::WriteOwner;

#ifdef Q_OS_WIN
        if (newPermissions & QFileDevice::WriteGroup)
            newPermissions ^= QFileDevice::WriteGroup;

        if (newPermissions & QFileDevice::WriteOther)
            newPermissions ^= QFileDevice::WriteOther;
#endif

        if (newPermissions & QFileDevice::WriteUser)
            newPermissions ^= QFileDevice::WriteUser;
    } else {
        newPermissions |= QFileDevice::WriteOwner;
#ifdef Q_OS_WIN
        newPermissions |= QFileDevice::WriteGroup;
        newPermissions |= QFileDevice::WriteOther;
#endif
        newPermissions |= QFileDevice::WriteUser;
    }

    if (QFile::setPermissions(mFileName, newPermissions))
        return LockedSet;
    else
        return LockedNotSet;
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

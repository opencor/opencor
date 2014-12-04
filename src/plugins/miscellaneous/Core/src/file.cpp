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

#include "corecliutils.h"
#include "file.h"

//==============================================================================

#include <QFile>
#include <QFileDevice>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static int gNewIndex = 0;

//==============================================================================

File::File(const QString &pFileName, const Type &pType, const QString &pUrl) :
    mFileName(nativeCanonicalFileName(pFileName)),
    mUrl(pUrl)
{
    // Initialise ourselves by 'resetting' ourselves

    reset();

    // Set our index, in case we are a new file

    if (pType == New)
        mNewIndex = ++gNewIndex;
}

//==============================================================================

File::~File()
{
    // Delete our corresponding physical file, in case we refer to a new or a
    // remote file (in which case our corresponding phsyical file was always
    // supposed to be only a temporary file)

    if (isNew() || isRemote())
        QFile::remove(mFileName);
}

//==============================================================================

QString File::fileName() const
{
    // Return the file name of the file

    return mFileName;
}

//==============================================================================

bool File::setFileName(const QString &pFileName)
{
    // Set the new file name of the file

    if (pFileName.compare(mFileName)) {
        mFileName = pFileName;

        mSha1 = sha1();
        // Note: we will typically set a new file name when a file has been
        //       saved under a new name, and depending on it was done, the SHA-1
        //       value of the file may end up being different, hence we need to
        //       recompute it, just to be on the safe side...

        return true;
    } else {
        return false;
    }
}

//==============================================================================

File::Status File::check()
{
    // Retrieve the 'new' SHA-1 value of our file and check whether it's
    // different from the one we currently have

    QString newSha1 = sha1();

    if (!newSha1.compare(mSha1) || !mUrl.isEmpty()) {
        // The SHA-1 values are the same or we are a remote file, which means
        // that our hasn't been changed

        return File::Unchanged;
    } else {
        if (newSha1.isEmpty()) {
            // The SHA-1 value of our file is now empty, which means that either
            // our file has been deleted or that it is unreadable (which, in
            // effect, means that it has changed)

            if (QFile::exists(mFileName))
                return File::Changed;
            else
                return File::Deleted;
        } else {
            // The SHA-1 value of our file is different from our stored value,
            // which means that our file has changed

            return File::Changed;
        }
    }
}

//==============================================================================

QString File::sha1() const
{
    // Compute the SHA-1 value for the file, if it still exists and can be
    // opened

    QString fileContents;

    if (Core::readTextFromFile(mFileName, fileContents))
        return Core::sha1(fileContents);
    else
        return QString();
}

//==============================================================================

void File::reset()
{
    // Reset our modified state, new index and SHA-1 value

    mSha1 = sha1();

    mNewIndex = 0;

    mModified = false;
}

//==============================================================================

bool File::isDifferent() const
{
    // Return whether the file is different from its corresponding physical
    // version by comparing their SHA-1 values

    return mSha1.compare(sha1());
}

//==============================================================================

bool File::isDifferent(const QString pFileContents) const
{
    // Return whether the file is different from the given file contents by
    // comparing their SHA-1 values

    return mSha1.compare(Core::sha1(pFileContents));
}

//==============================================================================

bool File::isNew() const
{
    // Return whether the file is new

    return mNewIndex != 0;
}

//==============================================================================

bool File::makeNew(const QString &pFileName)
{
    // Make the file new

    mFileName = nativeCanonicalFileName(pFileName);
    mUrl = QString();

    reset();

    mNewIndex = ++gNewIndex;

    return true;
}

//==============================================================================

int File::newIndex() const
{
    // Return the file's new index

    return mNewIndex;
}

//==============================================================================

bool File::isRemote() const
{
    // Return whether the file is a remote one

    return !mUrl.isEmpty();
}

//==============================================================================

QString File::url() const
{
    // Return the file's URL

    return mUrl;
}

//==============================================================================

bool File::isModified() const
{
    // Return whether the file has been modified

    return mModified;
}

//==============================================================================

bool File::setModified(const bool &pModified)
{
    // Set the modified status of the file

    if (pModified != mModified) {
        mModified = pModified;

        return true;
    } else {
        return false;
    }
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

    return QFileInfo(mFileName).isWritable() && mUrl.isEmpty();
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

    return !isWritable();
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

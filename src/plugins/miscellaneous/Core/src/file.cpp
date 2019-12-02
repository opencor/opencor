/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static int gNewIndex = 0;

//==============================================================================

File::File(const QString &pFileName, Type pType, const QString &pUrl) :
    mFileName(canonicalFileName(pFileName)),
    mUrl(pUrl)
{
    // Initialise ourselves by 'resetting' ourselves

    reset();

    // Set our index, in case we are a new file

    if (pType == Type::New) {
        mNewIndex = ++gNewIndex;
    }
}

//==============================================================================

File::~File()
{
    // Delete our corresponding physical file, in case we refer to a new or a
    // remote file (in which case our corresponding phsyical file was always
    // supposed to be only a temporary file)

    if (isNew() || isRemote()) {
        QFile::remove(mFileName);
    }
}

//==============================================================================

QString File::fileName() const
{
    // Return our file name

    return mFileName;
}

//==============================================================================

bool File::setFileName(const QString &pFileName)
{
    // Set our file name

    if (pFileName != mFileName) {
        mFileName = pFileName;

        mSha1 = sha1();
        // Note: we will typically set our file name when we have been saved
        //       under a new name, meaning that our SHA-1 value may end up being
        //       different, hence we need to recompute it, just to be on the
        //       safe side...

        return true;
    }

    return false;
}

//==============================================================================

File::Status File::check()
{
    // Always consider ourselves unchanged if we are a remote file

    if (!mUrl.isEmpty()) {
        return Status::Unchanged;
    }

    // Check whether the file and/or one or several of its dependencies has been
    // modified

    if (mModified) {
        return mDependenciesModified?Status::AllModified:Status::Modified;
    }

    if (mDependenciesModified) {
        return Status::DependenciesModified;
    }

    // Retrieve our 'new' SHA-1 value and that of our dependencies (if any), and
    // check whether they are different from the one(s) we currently have

    QString newSha1 = sha1();
    QStringList newDependenciesSha1 = QStringList();

    for (const auto &dependency : mDependencies) {
        newDependenciesSha1 << sha1(dependency);
    }

    if (newSha1.isEmpty()) {
        // Our SHA-1 value is now empty, which means that either we have been
        // deleted or that we are unreadable (which, in effect, means that we
        // have been changed)

        return QFile::exists(mFileName)?Status::Changed:Status::Deleted;
    }

    // Our SHA-1 value and/or that of one or several of our dependencies is
    // different from our stored value, which means that we and/or one or
    // several of our dependencies has changed

    return (newSha1 != mSha1)?
               (newDependenciesSha1 != mDependenciesSha1)?Status::AllChanged:Status::Changed:
               (newDependenciesSha1 != mDependenciesSha1)?Status::DependenciesChanged:Status::Unchanged;
}

//==============================================================================

QString File::sha1(const QString &pFileName)
{
    // Return the SHA-1 value of the given file

    return Core::fileSha1(pFileName);
}

//==============================================================================

QString File::sha1() const
{
    // Return our SHA-1 value

    return sha1(mFileName);
}

//==============================================================================

void File::reset(bool pResetDependencies)
{
    // Reset our modified state, new index and SHA-1 value

    mSha1 = sha1();

    mNewIndex = 0;

    mModified = false;

    if (pResetDependencies) {
        mDependencies.clear();
        mDependenciesSha1.clear();

        mDependenciesModified = false;
    }
}

//==============================================================================

bool File::isDifferent() const
{
    // Return whether we are different from our corresponding physical version
    // by comparing our respective SHA-1 values

    return mSha1 != sha1();
}

//==============================================================================

bool File::isDifferent(const QString &pFileContents) const
{
    // Return whether we are different from the given file contents by comparing
    // our respective SHA-1 values
    // Note: if we are considered modified, then we want to compare the given
    //       file contents against our corresponding physical version otherwise
    //       our internal SHA-1 value...

    return mModified?
               sha1() != Core::sha1(pFileContents):
               mSha1 != Core::sha1(pFileContents);
}

//==============================================================================

bool File::isNew() const
{
    // Return whether we are new

    return mNewIndex != 0;
}

//==============================================================================

bool File::makeNew(const QString &pFileName)
{
    // Make ourselves new

    mFileName = canonicalFileName(pFileName);
    mUrl = QString();

    reset();

    mNewIndex = ++gNewIndex;

    return true;
}

//==============================================================================

int File::newIndex() const
{
    // Return our new index

    return mNewIndex;
}

//==============================================================================

bool File::isLocal() const
{
    // Return whether we are a local file

    return mUrl.isEmpty();
}

//==============================================================================

bool File::isRemote() const
{
    // Return whether we are a remote file

    return !mUrl.isEmpty();
}

//==============================================================================

QString File::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

bool File::isModified() const
{
    // Return whether we have been modified

    return mModified;
}

//==============================================================================

bool File::setModified(bool pModified)
{
    // Set our modified status

    if (pModified != mModified) {
        mModified = pModified;

        // Update our SHA-1 value, if we are considered as not modified anymore
        // Note: indeed, say that you are editing a file in the Raw Text view
        //       and that the file gets modified outside of OpenCOR and that you
        //       decline reloading it. In that case, the file will be rightly
        //       considered modified. Then, if you modify it so that it now
        //       corresponds to the physical version of the file, the Raw Text
        //       view will update the modified state of the file, but the SHA-1
        //       value will be out-of-date, hence we need to update it...

        if (!pModified) {
            mSha1 = sha1();
        }

        return true;
    }

    return false;
}

//==============================================================================

bool File::isReadable() const
{
    // Return whether we are readable

    return QFileInfo(mFileName).isReadable();
}

//==============================================================================

bool File::isWritable() const
{
    // Return whether we are writable

    return QFileInfo(mFileName).isWritable() && mUrl.isEmpty();
}

//==============================================================================

bool File::isReadableAndWritable() const
{
    // Return whether we are both readable and writable

    return isReadable() && isWritable();
}

//==============================================================================

bool File::isLocked() const
{
    // Return whether we are locked

    return !isWritable();
}

//==============================================================================

File::Status File::setLocked(bool pLocked)
{
    // Set our locked status, but only if we are readable

    if (pLocked == isLocked()) {
        return Status::LockedNotNeeded;
    }

    QFileDevice::Permissions newPermissions = QFile::permissions(mFileName);

    if (pLocked) {
        if ((newPermissions & QFileDevice::WriteOwner) != 0) {
            newPermissions ^= QFileDevice::WriteOwner;
        }

#ifdef Q_OS_WIN
        if ((newPermissions & QFileDevice::WriteGroup) != 0) {
            newPermissions ^= QFileDevice::WriteGroup;
        }

        if ((newPermissions & QFileDevice::WriteOther) != 0) {
            newPermissions ^= QFileDevice::WriteOther;
        }
#endif

        if ((newPermissions & QFileDevice::WriteUser) != 0) {
            newPermissions ^= QFileDevice::WriteUser;
        }
    } else {
        newPermissions |= QFileDevice::WriteOwner;
#ifdef Q_OS_WIN
        newPermissions |= QFileDevice::WriteGroup;
        newPermissions |= QFileDevice::WriteOther;
#endif
        newPermissions |= QFileDevice::WriteUser;
    }

    if (QFile::setPermissions(mFileName, newPermissions)) {
        return Status::LockedSet;
    }

    return Status::LockedNotSet;
}

//==============================================================================

QStringList File::dependencies() const
{
    // Return our dependencies

    return mDependencies;
}

//==============================================================================

bool File::setDependencies(const QStringList &pDependencies)
{
    // Set our dependencies

    if (pDependencies != mDependencies) {
        mDependencies = pDependencies;

        mDependenciesSha1.clear();

        for (const auto &dependency : pDependencies) {
            mDependenciesSha1 << sha1(dependency);
        }

        return true;
    }

    return false;
}

//==============================================================================

bool File::setDependenciesModified(bool pDependenciesModified)
{
    // Set our dependencies modified status

    if (pDependenciesModified != mDependenciesModified) {
        mDependenciesModified = pDependenciesModified;

        return true;
    }

    return false;
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

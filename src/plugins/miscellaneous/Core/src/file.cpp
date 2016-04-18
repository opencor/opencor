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
    // Return our file name

    return mFileName;
}

//==============================================================================

bool File::setFileName(const QString &pFileName)
{
    // Set our file name

    if (pFileName.compare(mFileName)) {
        mFileName = pFileName;

        mSha1 = sha1();
        // Note: we will typically set our file name when we have been saved
        //       under a new name, meaning that our SHA-1 value may end up being
        //       different, hence we need to recompute it, just to be on the
        //       safe side...

        return true;
    } else {
        return false;
    }
}

//==============================================================================

File::Status File::check()
{
    // Always consider ourselves unchanged if we are a remote file

    if (!mUrl.isEmpty())
        return Unchanged;

    // Check whether the file and/or one or several of its dependencies has been
    // modified

    if (mModified)
        return mDependenciesModified?AllModified:Modified;
    else if (mDependenciesModified)
        return DependenciesModified;

    // Retrieve our 'new' SHA-1 value and that of our dependencies (if any), and
    // check whether they are different from the one(s) we currently have

    QString newSha1 = sha1();
    QStringList newDependenciesSha1 = QStringList();

    foreach (const QString &dependency, mDependencies)
        newDependenciesSha1 << sha1(dependency);

    bool dependenciesChanged = !qSameStringLists(newDependenciesSha1, mDependenciesSha1);

    if (newSha1.isEmpty()) {
        // Our SHA-1 value is now empty, which means that either we have been
        // deleted or that we are unreadable (which, in effect, means that we
        // have been changed)

        return (QFile::exists(mFileName))?Changed:Deleted;
    } else {
        // Our SHA-1 value and/or that of one or several of our dependencies is
        // different from our stored value, which means that we and/or one or
        // several of our dependencies has changed

        return newSha1.compare(mSha1)?
                   dependenciesChanged?AllChanged:Changed:
                   dependenciesChanged?DependenciesChanged:Unchanged;
    }
}

//==============================================================================

QString File::sha1(const QString &pFileName) const
{
    // Compute the SHA-1 value for the given file or ourselves (if no file is
    // given), if it/we still exist/s and can be opened

    QByteArray fileContents;

    if (readFileContentsFromFile(pFileName.isEmpty()?mFileName:pFileName, fileContents))
        return Core::sha1(fileContents);
    else
        return QString();
}

//==============================================================================

void File::reset(const bool &pResetDependencies)
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
    // by comparing our SHA-1 values

    return mSha1.compare(sha1());
}

//==============================================================================

bool File::isDifferent(const QByteArray &pFileContents) const
{
    // Return whether we are different from the given file contents by comparing
    // our SHA-1 values

    return mSha1.compare(Core::sha1(pFileContents));
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

    mFileName = nativeCanonicalFileName(pFileName);
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

bool File::setModified(const bool &pModified)
{
    // Set our modified status

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

File::Status File::setLocked(const bool &pLocked)
{
    // Set our locked status, but only if we are readable

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

QStringList File::dependencies() const
{
    // Return our dependencies

    return mDependencies;
}

//==============================================================================

bool File::setDependencies(const QStringList &pDependencies)
{
    // Set our dependencies

    if (!qSameStringLists(pDependencies, mDependencies)) {
        mDependencies = pDependencies;

        mDependenciesSha1.clear();

        foreach (const QString &dependency, pDependencies)
            mDependenciesSha1 << sha1(dependency);

        return true;
    } else {
        return false;
    }
}

//==============================================================================

bool File::setDependenciesModified(const bool &pDependenciesModified)
{
    // Set our dependencies modified status

    if (pDependenciesModified != mDependenciesModified) {
        mDependenciesModified = pDependenciesModified;

        return true;
    } else {
        return false;
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

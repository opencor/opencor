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

#include "coreutils.h"
#include "file.h"

//==============================================================================

#include <QCryptographicHash>
#include <QFileInfo>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

File::File(const QString &pFileName) :
    mFileName(nativeCanonicalFileName(pFileName)),
    mSha1(sha1()),
    mModified(false)
{
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

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

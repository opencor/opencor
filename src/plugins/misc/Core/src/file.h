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

#ifndef FILE_H
#define FILE_H

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class File
{
public:
    enum Status {
        // As a result of checking a file

        Changed,
        Unchanged,
        Deleted,

        // As a result of setting the locked status of a file

        LockedNotNeeded,
        LockedSet,
        LockedNotSet
    };

    explicit File(const QString &pFileName, const bool &pNew);

    QString fileName() const;
    void setFileName(const QString &pFileName);

    Status check();

    void reset();

    bool isNew() const;

    int newIndex() const;

    bool isModified() const;
    void setModified(const bool &pModified);

    bool isReadable() const;
    bool isWritable() const;
    bool isReadableAndWritable() const;

    bool isLocked() const;
    Status  setLocked(const bool &pLocked);

private:
    QString mFileName;
    QString mSha1;

    int mNewIndex;

    bool mModified;

    QString sha1() const;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

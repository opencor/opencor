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
// File
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT File
{
public:
    enum Type {
        New,
        Local,
        Remote
    };

    enum Status {
        // As a result of checking a file

        Changed,
        DependenciesChanged,
        AllChanged,
        Unchanged,
        Deleted,
        Modified,
        DependenciesModified,
        AllModified,

        // As a result of setting the locked status of a file

        LockedNotNeeded,
        LockedSet,
        LockedNotSet
    };

    explicit File(const QString &pFileName, const Type &pType,
                  const QString &pUrl);
    ~File();

    QString fileName() const;
    bool setFileName(const QString &pFileName);

    Status check();

    static QString sha1(const QString &pFileName);

    QString sha1() const;

    void reset(const bool &pResetDependencies = true);

    bool isDifferent() const;
    bool isDifferent(const QString &pFileContents) const;

    bool isNew() const;
    bool makeNew(const QString &pFileName);

    int newIndex() const;

    bool isLocal() const;
    bool isRemote() const;

    QString url() const;

    bool isModified() const;
    bool setModified(const bool &pModified);

    bool isReadable() const;
    bool isWritable() const;
    bool isReadableAndWritable() const;

    bool isLocked() const;
    Status setLocked(const bool &pLocked);

    QStringList dependencies() const;
    bool setDependencies(const QStringList &pDependencies);

    bool setDependenciesModified(const bool &pDependenciesModified);

private:
    QString mFileName;
    QString mUrl;
    QString mSha1;

    int mNewIndex;

    bool mModified;

    QStringList mDependencies;
    QStringList mDependenciesSha1;

    bool mDependenciesModified;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

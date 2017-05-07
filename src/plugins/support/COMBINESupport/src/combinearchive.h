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
// COMBINE archive
//==============================================================================

#pragma once

//==============================================================================

#include "combinearchiveissue.h"
#include "combinesupportglobal.h"
#include "standardfile.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace COMBINESupport {

//==============================================================================

class COMBINESUPPORT_EXPORT CombineArchiveFile
{
public:
    enum Format {
        Unknown,
        Cellml,
        Cellml_1_0,
        Cellml_1_1,
        Omex,
        Sedml
    };

    explicit CombineArchiveFile(const QString &pFileName,
                                const QString &pLocation, const Format &pFormat,
                                const bool &pMaster);

    QString fileName() const;

    QString location() const;
    Format format() const;
    bool isMaster() const;

    static Format format(const QString &pFormat);

private:
    QString mFileName;

    QString mLocation;
    Format mFormat;
    bool mMaster;
};

//==============================================================================

typedef QList<CombineArchiveFile> CombineArchiveFiles;

//==============================================================================

class COMBINESUPPORT_EXPORT CombineArchive : public StandardSupport::StandardFile
{
    Q_OBJECT

public:
    explicit CombineArchive(const QString &pFileName, const bool &pNew = false);
    ~CombineArchive();

    virtual bool load();
    virtual bool save(const QString &pFileName = QString());

    bool isValid(CombineArchiveIssues &pIssues);

    QString location(const CombineArchiveFile &pFile) const;

    CombineArchiveFiles masterFiles();

    bool addFile(const QString &pFileName, const QString &pLocation,
                 const CombineArchiveFile::Format &pFormat,
                 const bool &pMaster = false);

    CombineArchiveIssues issues() const;

private:
    QString mDirName;

    bool mNew;
    bool mLoadingNeeded;

    CombineArchiveFiles mFiles;
    CombineArchiveIssues mIssues;

    virtual void reset();
};

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

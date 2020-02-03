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

namespace SEDMLSupport {
    class SedmlFile;
} // namespace SEDMLSupport

//==============================================================================

namespace COMBINESupport {

//==============================================================================

class COMBINESUPPORT_EXPORT CombineArchiveFile
{
public:
    enum class Format {
        Unknown,
        Cellml,
        Cellml_1_0,
        Cellml_1_1,
        Omex,
        Sedml
    };

    explicit CombineArchiveFile(const QString &pFileName,
                                const QString &pLocation, Format pFormat,
                                bool pMaster);

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

using CombineArchiveFiles = QList<CombineArchiveFile>;

//==============================================================================

class COMBINESUPPORT_EXPORT CombineArchive : public StandardSupport::StandardFile
{
    Q_OBJECT

public:
    explicit CombineArchive(const QString &pFileName, bool pNew = false);
    ~CombineArchive() override;

    bool load() override;
    bool save(const QString &pFileName = {}) override;

    bool update(const QString &pFileName = {});

    void forceNew();

    bool isValid();
    bool isSupported();

    QString location(const CombineArchiveFile &pFile) const;

    CombineArchiveFiles masterFiles();

    bool addFile(const QString &pFileName, const QString &pLocation,
                 CombineArchiveFile::Format pFormat, bool pMaster = false);

    SEDMLSupport::SedmlFile * sedmlFile();

    CombineArchiveIssues issues() const;

private:
    QString mDirName;

    bool mLoadingNeeded = true;

    SEDMLSupport::SedmlFile *mSedmlFile = nullptr;

    CombineArchiveFiles mFiles;
    CombineArchiveIssues mIssues;

    bool mUpdated = false;

    void reset() override;
};

//==============================================================================

} // namespace COMBINESupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

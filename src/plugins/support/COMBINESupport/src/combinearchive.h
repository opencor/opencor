/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// COMBINE archive class
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

    bool isValid();

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

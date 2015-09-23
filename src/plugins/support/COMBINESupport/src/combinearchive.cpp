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
// COMBINE archive class
//==============================================================================

#include "combinearchive.h"
#include "corecliutils.h"

//==============================================================================

#include <QFile>
#include <QTemporaryDir>
#include <QTextStream>

//==============================================================================

#include <QZipWriter>

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

CombineArchiveFile::CombineArchiveFile(const QString &pFileName,
                                       const QString &pLocation,
                                       const Format &pFormat) :
    mFileName(pFileName),
    mLocation(pLocation),
    mFormat(pFormat)
{
}

//==============================================================================

QString CombineArchiveFile::fileName() const
{
    // Return our file name

    return mFileName;
}

//==============================================================================

QString CombineArchiveFile::location() const
{
    // Return our location

    return mLocation;
}

//==============================================================================

CombineArchiveFile::Format CombineArchiveFile::format() const
{
    // Return our format

    return mFormat;
}

//==============================================================================

CombineArchive::CombineArchive(const QString &pFileName) :
    StandardSupport::StandardFile(pFileName),
    mCombineArchiveFiles(CombineArchiveFiles())
{
}

//==============================================================================

bool CombineArchive::load()
{
    // Consider the file loaded

    return true;
}

//==============================================================================

bool CombineArchive::save(const QString &pNewFileName)
{
    // Create a temporary directory where we are going to put ourselves

    QTemporaryDir temporaryDirName;

    if (temporaryDirName.isValid()) {
        // Keep track of our current directory

        QString origPath = QDir::currentPath();

        // Create and go to a sub-directory where we are effecitvely going to
        // put ourselves

        QString fileName = pNewFileName.isEmpty()?mFileName:pNewFileName;
        QString baseDirName = QFileInfo(fileName).baseName();
        QString dirName = temporaryDirName.path()+QDir::separator()+baseDirName;

        QDir().mkpath(dirName);

        QDir::setCurrent(dirName);

        // Create our manifest file

        static const QString ManifestFileName = "manifest.xml";

        QByteArray manifestFileContents = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";

        if (!Core::writeTextToFile(ManifestFileName, manifestFileContents))
            return false;

        // Go back to our original path

        QDir::setCurrent(origPath);

        // Save ourselves to either the given file, which name is given, or to
        // our current file

        OpenCOR::ZIPSupport::QZipWriter zipWriter(fileName);

        zipWriter.addFile(baseDirName+QDir::separator()+ManifestFileName,
                          manifestFileContents);

        zipWriter.close();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

void CombineArchive::addFile(const QString &pFileName, const QString &pLocation,
                             const CombineArchiveFile::Format &pFormat)
{
    // Add the given file to our list
qDebug("---[ADDING FILE]---");
qDebug("File name: %s", qPrintable(pFileName));
qDebug("Location:  %s", qPrintable(pLocation));
qDebug("Format:    %d", pFormat);
qDebug("Master:    %s", pMaster?"true":"false");

    mCombineArchiveFiles << CombineArchiveFile(pFileName, pLocation, pFormat);
}

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

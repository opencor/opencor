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
#include <QRegularExpression>
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
                                       const Format &pFormat,
                                       const bool &pMaster) :
    mFileName(pFileName),
    mLocation(pLocation),
    mFormat(pFormat),
    mMaster(pMaster)
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

bool CombineArchiveFile::master() const
{
    // Return whether we are a master file

    return mMaster;
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

        QDir dir;
        QString fileName = pNewFileName.isEmpty()?mFileName:pNewFileName;
        QString baseDirName = QFileInfo(fileName).baseName();
        QString dirName = temporaryDirName.path()+QDir::separator()+baseDirName;

        dir.mkpath(dirName);

        QDir::setCurrent(dirName);

        // Create our manifest file

        static const QString ManifestFileName = "manifest.xml";

        QByteArray manifestFileContents = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                                          "<omexManifest xmlns=\"http://identifiers.org/combine.specifications/omex-manifest\">\n"
                                          "    <content location=\".\" format=\"http://identifiers.org/combine.specifications/omex\"/>\n"
                                          "</omexManifest>\n";

        if (!Core::writeTextToFile(ManifestFileName, manifestFileContents))
            return false;

        // Get a copy of our various files, after creating the sub-folder(s) in
        // which they are, if any

        static const QRegularExpression FileNameRegEx = QRegularExpression("/[^/]*$");

        QString origFileName;
        QString destFileName;
        QString destDirName;

        foreach (const CombineArchiveFile &combineArchiveFile, mCombineArchiveFiles) {
            origFileName = combineArchiveFile.fileName();
            destFileName = dirName+QDir::separator()+combineArchiveFile.location();
            destDirName = QString(destDirName).remove(FileNameRegEx);

            if (!QDir(destDirName).exists()) {
                if (!dir.mkpath(destDirName))
                    return false;
            }

            if (!QFile::copy(combineArchiveFile.fileName(), destFileName))
                return false;
        }

        // Go back to our original path

        QDir::setCurrent(origPath);

        // Save ourselves to either the given file, which name is given, or to
        // our current file

        OpenCOR::ZIPSupport::QZipWriter zipWriter(fileName);

        zipWriter.addFile(baseDirName+QDir::separator()+ManifestFileName,
                          manifestFileContents);

        QString combineArchiveFileContents;
        QByteArray combineArchiveFileContentsAsByteArray;

        foreach (const CombineArchiveFile &combineArchiveFile, mCombineArchiveFiles) {
            if (!Core::readTextFromFile(dirName+QDir::separator()+combineArchiveFile.location(), combineArchiveFileContents))
                return false;

            combineArchiveFileContentsAsByteArray = combineArchiveFileContents.toUtf8();

            zipWriter.addFile(baseDirName+QDir::separator()+combineArchiveFile.location(),
                              combineArchiveFileContentsAsByteArray);
        }

        zipWriter.close();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

void CombineArchive::addFile(const QString &pFileName, const QString &pLocation,
                             const CombineArchiveFile::Format &pFormat,
                             const bool &pMaster)
{
    // Add the given file to our list

    mCombineArchiveFiles << CombineArchiveFile(pFileName, pLocation, pFormat, pMaster);
}

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

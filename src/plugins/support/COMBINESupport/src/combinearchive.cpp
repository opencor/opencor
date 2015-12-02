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

#include <QZipReader>
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

bool CombineArchiveFile::isMaster() const
{
    // Return whether we are a master file

    return mMaster;
}

//==============================================================================

CombineArchive::CombineArchive(const QString &pFileName) :
    StandardSupport::StandardFile(pFileName),
    mDirName(Core::temporaryDirName()),
    mCombineArchiveFiles(CombineArchiveFiles())
{
}

//==============================================================================

CombineArchive::~CombineArchive()
{
    // Delete our temporary directory

    QDir(mDirName).removeRecursively();
}

//==============================================================================

bool CombineArchive::load()
{
    // Make sure that our file exists

    if (!QFile::exists(mFileName))
        return false;

    // Make sure that our file starts with 0x04034b50, which is the signature of
    // a ZIP file and should therefore be that of our file

    static const int SignatureSize = 4;

    OpenCOR::ZIPSupport::QZipReader zipReader(mFileName);
    uchar signatureData[SignatureSize];

    if (zipReader.device()->read((char *) signatureData, SignatureSize) != SignatureSize)
        return false;

    uint signature =   signatureData[0]
                     +(signatureData[1] <<  8)
                     +(signatureData[2] << 16)
                     +(signatureData[3] << 24);

    if (signature != 0x04034b50)
        return false;

    // Try to extract all of our contents

    if (!zipReader.extractAll(mDirName))
        return false;

    return true;
}

//==============================================================================

static const auto CellmlFormat      = QStringLiteral("http://identifiers.org/combine.specifications/cellml");
static const auto Cellml_1_0_Format = QStringLiteral("http://identifiers.org/combine.specifications/cellml.1.0");
static const auto Cellml_1_1_Format = QStringLiteral("http://identifiers.org/combine.specifications/cellml.1.1");
static const auto SedmlFormat       = QStringLiteral("http://identifiers.org/combine.specifications/sed-ml");

//==============================================================================

bool CombineArchive::save(const QString &pNewFileName)
{
    // Keep track of our current directory and go to our own directory

    QString origPath = QDir::currentPath();

    QDir::setCurrent(mDirName);

    // Generate the contents our manifest file

    QString fileList = QString();
    QString fileFormat;

    foreach (const CombineArchiveFile &combineArchiveFile, mCombineArchiveFiles) {
        switch (combineArchiveFile.format()) {
        case CombineArchiveFile::Cellml:
            fileFormat = CellmlFormat;

            break;
        case CombineArchiveFile::Cellml_1_0:
            fileFormat = Cellml_1_0_Format;

            break;
        case CombineArchiveFile::Cellml_1_1:
            fileFormat = Cellml_1_1_Format;

            break;
        case CombineArchiveFile::Sedml:
            fileFormat = SedmlFormat;

            break;
        default:   // CombineArchiveFile::Unknown
            return false;
        }

        fileList += "    <content location=\""+combineArchiveFile.location()+"\" format=\""+fileFormat+"\"";

        if (combineArchiveFile.isMaster())
            fileList += " master=\"true\"";

        fileList += "/>\n";
    }

    // Save ourselves to either the given file, which name is given, or to our
    // current file

    OpenCOR::ZIPSupport::QZipWriter zipWriter(pNewFileName.isEmpty()?mFileName:pNewFileName);

    zipWriter.addFile("manifest.xml",
                       "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                       "<omexManifest xmlns=\"http://identifiers.org/combine.specifications/omex-manifest\">\n"
                       "    <content location=\".\" format=\"http://identifiers.org/combine.specifications/omex\"/>\n"
                      +fileList.toUtf8()
                      +"</omexManifest>\n");

    foreach (const CombineArchiveFile &combineArchiveFile, mCombineArchiveFiles) {
        QString combineArchiveFileContents;

        if (!Core::readTextFromFile(combineArchiveFile.location(), combineArchiveFileContents))
            return false;

        zipWriter.addFile(combineArchiveFile.location(),
                          combineArchiveFileContents.toUtf8());
    }

    zipWriter.close();

    // Go back to our original path

    QDir::setCurrent(origPath);

    return true;
}

//==============================================================================

bool CombineArchive::addFile(const QString &pFileName, const QString &pLocation,
                             const CombineArchiveFile::Format &pFormat,
                             const bool &pMaster)
{
    // Make sure the format is known

    if (pFormat == CombineArchiveFile::Unknown)
        return false;

    // Add the given file to our list

    mCombineArchiveFiles << CombineArchiveFile(pFileName, pLocation, pFormat, pMaster);

    // Get a copy of the given file, after creating the sub-folder(s) in which
    // it is, if necessary

#if defined(Q_OS_WIN)
    static const QRegularExpression FileNameRegEx = QRegularExpression("\\\\[^\\\\]*$");
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    static const QRegularExpression FileNameRegEx = QRegularExpression("/[^/]*$");
#else
    #error Unsupported platform
#endif

    static QDir dir;

    QString destFileName = Core::nativeCanonicalFileName(mDirName+QDir::separator()+pLocation);
    QString destDirName = QString(destFileName).remove(FileNameRegEx);

    if (!QDir(destDirName).exists() && !dir.mkpath(destDirName))
        return false;

    if (!QFile::copy(pFileName, destFileName))
        return false;

    return true;
}

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

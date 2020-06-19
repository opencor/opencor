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

#include "combinearchive.h"
#include "corecliutils.h"
#include "sedmlfile.h"

//==============================================================================

#include <QFile>
#include <QRegularExpression>
#include <QTemporaryDir>

//==============================================================================

#include <QZipReader>
#include <QZipWriter>

//==============================================================================

#include <array>

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

CombineArchiveFile::CombineArchiveFile(const QString &pFileName,
                                       const QString &pLocation,
                                       Format pFormat, bool pMaster) :
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

static const char *CellmlFormat      = "http://identifiers.org/combine.specifications/cellml";
static const char *Cellml_1_0_Format = "http://identifiers.org/combine.specifications/cellml.1.0";
static const char *Cellml_1_1_Format = "http://identifiers.org/combine.specifications/cellml.1.1";
static const char *OmexFormat        = "http://identifiers.org/combine.specifications/omex";
static const char *SedmlFormat       = "http://identifiers.org/combine.specifications/sed-ml";

//==============================================================================

CombineArchiveFile::Format CombineArchiveFile::format(const QString &pFormat)
{
    // Return the effective format associated with the given format

    if (pFormat == CellmlFormat) {
        return Format::Cellml;
    }

    if (pFormat == Cellml_1_0_Format) {
        return Format::Cellml_1_0;
    }

    if (pFormat == Cellml_1_1_Format) {
        return Format::Cellml_1_1;
    }

    if (pFormat == OmexFormat) {
        return Format::Omex;
    }

    if (pFormat == SedmlFormat) {
        return Format::Sedml;
    }

    return Format::Unknown;
}

//==============================================================================

CombineArchive::CombineArchive(const QString &pFileName, bool pNew) :
    StandardSupport::StandardFile(pFileName),
    mDirName(Core::temporaryDirName())
{
    // Override our inherited value of mNew with our given one

    mNew = pNew;

    // Reset ourselves

    reset();
}

//==============================================================================

CombineArchive::~CombineArchive()
{
    // Reset ourselves

    reset();

    // Delete our temporary directory

    QDir(mDirName).removeRecursively();
}

//==============================================================================

void CombineArchive::reset()
{
    // Don't reset ourselves if we were updated

    if (mUpdated) {
        mUpdated = false;

        return;
    }

    // Reset all of our properties

    mLoadingNeeded = true;

    mFiles.clear();
    mIssues.clear();

    delete mSedmlFile;

    mSedmlFile = nullptr;
}

//==============================================================================

static const char *ManifestFileName = "manifest.xml";

//==============================================================================

bool CombineArchive::load()
{
    // Check whether we are already loaded and without an issue

    if (!mLoadingNeeded) {
        return mIssues.isEmpty();
    }

    mLoadingNeeded = false;

    // Make sure that our file exists, unless it is new in which case we
    // consider it loaded

    if (mNew) {
        return true;
    }

    if (!QFile::exists(mFileName)) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Error,
                                       tr("the archive does not exist"));

        return false;
    }

    // Make sure that our file starts with 0x04034b50, which is the signature of
    // a ZIP file and should therefore be that of our file

    enum {
        SignatureSize = 4
    };

    ZIPSupport::QZipReader zipReader(mFileName);
    std::array<uchar, SignatureSize> signatureData = {};

    if (zipReader.device()->read(reinterpret_cast<char *>(signatureData.data()), SignatureSize) != SignatureSize) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Error,
                                       tr("the archive is not signed"));

        return false;
    }

    if (signatureData[0]+256*signatureData[1]+65536*signatureData[2]+16777216*signatureData[3] != 0x04034b50) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Error,
                                       tr("the archive does not have the correct signature"));

        return false;
    }

    // Our file is effectively a ZIP file, so extract all of its contents

    zipReader.device()->reset();

    if (!zipReader.extractAll(mDirName)) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Error,
                                       tr("the contents of the archive could not be extracted"));

        return false;
    }

    return true;
}

//==============================================================================

bool CombineArchive::save(const QString &pFileName)
{
    // Make sure that we are properly loaded and have no issue

    if (mLoadingNeeded || !mIssues.isEmpty()) {
        return false;
    }

    // Generate the contents of our manifest

    QByteArray fileList;
    QString fileFormat;

    for (const auto &file : mFiles) {
        switch (file.format()) {
        case CombineArchiveFile::Format::Unknown:
            return false;
        case CombineArchiveFile::Format::Cellml:
            fileFormat = CellmlFormat;

            break;
        case CombineArchiveFile::Format::Cellml_1_0:
            fileFormat = Cellml_1_0_Format;

            break;
        case CombineArchiveFile::Format::Cellml_1_1:
            fileFormat = Cellml_1_1_Format;

            break;
        case CombineArchiveFile::Format::Omex:
            fileFormat = OmexFormat;

            break;
        case CombineArchiveFile::Format::Sedml:
            fileFormat = SedmlFormat;

            break;
        }

        fileList += R"(    <content location=")"+file.location()+R"(" format=")"+fileFormat+R"(")";

        if (file.isMaster()) {
            fileList += R"( master="true")";
        }

        fileList += "/>\n";
    }

    // Save ourselves to either the given file, which name is given, or to our
    // current file

    static const QString Dot = ".";

    ZIPSupport::QZipWriter zipWriter(pFileName.isEmpty()?mFileName:pFileName);

    zipWriter.addFile(ManifestFileName,
                       QByteArray()
                      + "<?xml version='1.0' encoding='UTF-8' standalone='yes'?>\n"
                       R"(<omexManifest xmlns="http://identifiers.org/combine.specifications/omex-manifest">)""\n"
                       R"(    <content location="." format=")"+OmexFormat+R"("/>)""\n"
                      +fileList
                      +"</omexManifest>\n");

    for (const auto &file : mFiles) {
        if (file.location() != Dot) {
            QByteArray fileContents;

            if (!Core::readFile(mDirName+"/"+file.location(), fileContents)) {
                return false;
            }

            zipWriter.addFile(file.location(), fileContents);
        }
    }

    mNew = false;

    return StandardFile::save(pFileName);
}

//==============================================================================

bool CombineArchive::update(const QString &pFileName)
{
    // Our COMBINE archive has been updated (e.g. through the Simulation
    // Experiment view) and we want to update ourselves accordingly, so save
    // ourselves and keep track of the fact that we were 'simply' updated

    mUpdated = save(pFileName);

    return mUpdated;
}

//==============================================================================

void CombineArchive::forceNew()
{
    // Force our COMBINE archive to act as if it was 'new'

    mNew = true;

    mLoadingNeeded = true;

    mFiles.clear();
}

//==============================================================================

bool CombineArchive::isValid()
{
    // Make sure that we are loaded and fine

    if (!load()) {
        return false;
    }

    mIssues.clear();

    // Consider ourselves valid if new

    if (mNew) {
        return true;
    }

    // A COMBINE archive must contain a manifest at its root

    QString manifestFileName = mDirName+"/"+ManifestFileName;

    if (!QFile::exists(manifestFileName)) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Error,
                                       tr("the archive does not have a manifest"));

        return false;
    }

    // Make sure that the manifest is a valid OMEX file

    QByteArray manifestContents;
    QByteArray schemaContents;

    Core::readFile(manifestFileName, manifestContents);
    Core::readFile(":/COMBINESupport/omex.xsd", schemaContents);

    if (!Core::validXml(manifestContents, schemaContents)) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Error,
                                       tr("the manifest is not a valid OMEX file"));

        return false;
    }

    // Retrieve the COMBINE archive files from the manifest, making sure that
    // they have been physically extracted

    static const QString True = "true";
    static const QString One  = "1";

    QDomDocument domDocument;

    domDocument.setContent(manifestContents, true);

    mFiles.clear();

    for (QDomElement childElement = domDocument.documentElement().firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        QString location = childElement.attribute("location");
        QString fileName = mDirName+"/"+location;

        if (!QFile::exists(fileName)) {
            mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Error,
                                           tr("'%1' could not be found").arg(location));

            mFiles.clear();

            return false;
        }

        mFiles << CombineArchiveFile(fileName, location,
                                     CombineArchiveFile::format(childElement.attribute("format")),
                                        (childElement.attribute("master") == True)
                                     || (childElement.attribute("master") == One));
    }

    // Make sure that one of our COMBINE archive files represents our COMBINE
    // archive itself

    static const QString Dot = ".";

    bool combineArchiveReferenceFound = false;

    for (int i = 0, iMax = mFiles.count(); i < iMax; ++i) {
        const CombineArchiveFile &file = mFiles[i];

        if (    (file.location() == Dot)
            &&  (file.format() == CombineArchiveFile::Format::Omex)
            && !file.isMaster()) {
            combineArchiveReferenceFound = true;

            mFiles.removeAt(i);
            // Note: we don't need to track it anymore...

            break;
        }
    }

    if (!combineArchiveReferenceFound) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Error,
                                       tr("no reference to the COMBINE archive itself could be found"));

        mFiles.clear();

        return false;
    }

    return true;
}

//==============================================================================

bool CombineArchive::isSupported()
{
    // Make sure that we are valid

    if (!isValid()) {
        return false;
    }

    // Make sure that we have only one master file

    if (masterFiles().count() != 1) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Type::Information,
                                       tr("only COMBINE archives with one master file are supported"));

        return false;
    }

    return true;
}

//==============================================================================

QString CombineArchive::location(const CombineArchiveFile &pFile) const
{
    // Return the (full) location of the given file

    return mDirName+"/"+pFile.location();
}

//==============================================================================

CombineArchiveFiles CombineArchive::masterFiles()
{
    // Make sure that we are properly loaded

    if (!load()) {
        return CombineArchiveFiles();
    }

    // Return a list of our master files

    CombineArchiveFiles res;

    for (const auto &file : mFiles) {
        if (file.isMaster()) {
            res << file;
        }
    }

    return res;
}

//==============================================================================

bool CombineArchive::addFile(const QString &pFileName, const QString &pLocation,
                             CombineArchiveFile::Format pFormat, bool pMaster)
{
    // Make sure that we are properly loaded

    if (!load()) {
        return false;
    }

    // Make sure that the format is known

    if (pFormat == CombineArchiveFile::Format::Unknown) {
        return false;
    }

    // Add the given file to our list

    mFiles << CombineArchiveFile(pFileName, pLocation, pFormat, pMaster);

    // Get a copy of the given file, after creating the sub-folder(s) in which
    // it is, if necessary
    // Note: to ensure that a COMBINE archive can be updated, we need to check
    //       that the given file and its destination are not the same and that
    //       its destination doesn't exist...

    static const QRegularExpression FileNameRegEx = QRegularExpression("/[^/]*$");

    static QDir dir;

    QString destFileName = Core::canonicalFileName(mDirName+"/"+pLocation);
    QString destDirName = QString(destFileName).remove(FileNameRegEx);

    if (!QDir(destDirName).exists() && !dir.mkpath(destDirName)) {
        return false;
    }

    if (destFileName != pFileName) {
        QFile::remove(destFileName);

        if (!QFile::copy(pFileName, destFileName)) {
            return false;
        }
    }

    return true;
}

//==============================================================================

SEDMLSupport::SedmlFile * CombineArchive::sedmlFile()
{
    // Return our SED-ML file, after having created it, if necessary

    if ((mSedmlFile == nullptr) && isSupported()) {
        mSedmlFile = new SEDMLSupport::SedmlFile(masterFiles().first().fileName(),
                                                 mFileName);
    }

    return mSedmlFile;
}

//==============================================================================

CombineArchiveIssues CombineArchive::issues() const
{
    // Return our archive's issues

    return mIssues;
}

//==============================================================================

} // namespace COMBINESupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

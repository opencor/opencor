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

#include "cellmlfile.h"
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

static const auto CellmlFormat      = QStringLiteral("http://identifiers.org/combine.specifications/cellml");
static const auto Cellml_1_0_Format = QStringLiteral("http://identifiers.org/combine.specifications/cellml.1.0");
static const auto Cellml_1_1_Format = QStringLiteral("http://identifiers.org/combine.specifications/cellml.1.1");
static const auto OmexFormat        = QStringLiteral("http://identifiers.org/combine.specifications/omex");
static const auto SedmlFormat       = QStringLiteral("http://identifiers.org/combine.specifications/sed-ml");

//==============================================================================

CombineArchiveFile::Format CombineArchiveFile::format(const QString &pFormat)
{
    // Return the effective format associated with the given format

    if (!pFormat.compare(CellmlFormat))
        return Cellml;
    else if (!pFormat.compare(Cellml_1_0_Format))
        return Cellml_1_0;
    else if (!pFormat.compare(Cellml_1_1_Format))
        return Cellml_1_1;
    else if (!pFormat.compare(OmexFormat))
        return Omex;
    else if (!pFormat.compare(SedmlFormat))
        return Sedml;
    else
        return Unknown;
}

//==============================================================================

CombineArchive::CombineArchive(const QString &pFileName, const bool &pNew) :
    StandardSupport::StandardFile(pFileName),
    mDirName(Core::temporaryDirName()),
    mNew(pNew)
{
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
    // Reset all of our properties

    mLoadingNeeded = true;

    mFiles.clear();
    mIssues.clear();
}

//==============================================================================

static const auto ManifestFileName = QStringLiteral("manifest.xml");

//==============================================================================

bool CombineArchive::load()
{
    // Check whether we are already loaded and without an issue

    if (!mLoadingNeeded)
        return mIssues.isEmpty();

    mLoadingNeeded = false;

    // Make sure that our file exists, unless it is new in which case we
    // consider it loaded

    if (mNew) {
        return true;
    } else if (!QFile::exists(mFileName)) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Error,
                                       QObject::tr("the archive does not exist"));

        return false;
    }

    // Make sure that our file starts with 0x04034b50, which is the signature of
    // a ZIP file and should therefore be that of our file

    static const int SignatureSize = 4;

    OpenCOR::ZIPSupport::QZipReader zipReader(mFileName);
    uchar signatureData[SignatureSize];

    if (zipReader.device()->read((char *) signatureData, SignatureSize) != SignatureSize) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Error,
                                       QObject::tr("the archive is not signed"));

        return false;
    }

    uint signature =   signatureData[0]
                     +(signatureData[1] <<  8)
                     +(signatureData[2] << 16)
                     +(signatureData[3] << 24);

    if (signature != 0x04034b50) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Error,
                                       QObject::tr("the archive does not have the correct signature"));

        return false;
    }

    // Our file is effectively a ZIP file, so extract all of our contents

    zipReader.device()->reset();

    if (!zipReader.extractAll(mDirName)) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Error,
                                       QObject::tr("the contents of the archive could not be extracted"));

        return false;
    }

    return true;
}

//==============================================================================

bool CombineArchive::save(const QString &pFileName)
{
    // Make sure that we are properly loaded and have no issue

    if (mLoadingNeeded || !mIssues.isEmpty())
        return false;

    // Generate the contents our manifest

    QByteArray fileList = QByteArray();
    QString fileFormat;

    foreach (const CombineArchiveFile &file, mFiles) {
        switch (file.format()) {
        case CombineArchiveFile::Unknown:
            return false;
        case CombineArchiveFile::Cellml:
            fileFormat = CellmlFormat;

            break;
        case CombineArchiveFile::Cellml_1_0:
            fileFormat = Cellml_1_0_Format;

            break;
        case CombineArchiveFile::Cellml_1_1:
            fileFormat = Cellml_1_1_Format;

            break;
        case CombineArchiveFile::Omex:
            fileFormat = OmexFormat;

            break;
        case CombineArchiveFile::Sedml:
            fileFormat = SedmlFormat;

            break;
        }

        fileList += "    <content location=\""+file.location()+"\" format=\""+fileFormat+"\"";

        if (file.isMaster())
            fileList += " master=\"true\"";

        fileList += "/>\n";
    }

    // Save ourselves to either the given file, which name is given, or to our
    // current file

    OpenCOR::ZIPSupport::QZipWriter zipWriter(pFileName.isEmpty()?mFileName:pFileName);

    zipWriter.addFile(ManifestFileName,
                       "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                       "<omexManifest xmlns=\"http://identifiers.org/combine.specifications/omex-manifest\">\n"
                       "    <content location=\".\" format=\""+OmexFormat.toUtf8()+"\"/>\n"
                      +fileList
                      +"</omexManifest>\n");

    foreach (const CombineArchiveFile &file, mFiles) {
        if (file.location().compare(".")) {
            QByteArray combineArchiveFileContents;

            if (!Core::readFileContentsFromFile(mDirName+QDir::separator()+file.location(),
                                                combineArchiveFileContents)) {
                return false;
            }

            zipWriter.addFile(file.location(), combineArchiveFileContents);
        }
    }

    return true;
}

//==============================================================================

bool CombineArchive::isValid()
{
    // Make sure that we are loaded and fine

    if (!load())
        return false;
    else
        mIssues.clear();

    // Consider ourselves valid if new

    if (mNew)
        return true;

    // A COMBINE archive must contain a manifest at its root

    QString manifestFileName = mDirName+QDir::separator()+ManifestFileName;

    if (!QFile::exists(manifestFileName)) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Error,
                                       QObject::tr("the archive does not have a manifest"));

        return false;
    }

    // Make sure that the manifest is a valid OMEX file

    QByteArray manifestContents;
    QByteArray schemaContents;

    Core::readFileContentsFromFile(manifestFileName, manifestContents);
    Core::readFileContentsFromFile(":omex.xsd", schemaContents);

    if (!Core::validXml(manifestContents, schemaContents)) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Error,
                                       QObject::tr("the manifest is not a valid OMEX file"));

        return false;
    }

    // Retrieve the COMBINE archive files from the manifest, making sure that
    // they have been physically extracted

    QDomDocument domDocument;

    domDocument.setContent(manifestContents, true);

    mFiles.clear();

    for (QDomElement childElement = domDocument.documentElement().firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        QString location = childElement.attribute("location");
        QString fileName = mDirName+QDir::separator()+location;

        if (!QFile::exists(fileName)) {
            mIssues << CombineArchiveIssue(CombineArchiveIssue::Error,
                                           QObject::tr("<strong>%1</strong> could not be found").arg(location));

            mFiles.clear();

            return false;
        } else {
            mFiles << CombineArchiveFile(fileName, location,
                                         CombineArchiveFile::format(childElement.attribute("format")),
                                         !childElement.attribute("master").compare("true"));
        }
    }

    // Make sure that one of our COMBINE archive files represents our COMBINE
    // archive itself

    bool combineArchiveReferenceFound = false;

    for (int i = 0, iMax = mFiles.count(); i < iMax; ++i) {
        const CombineArchiveFile &file = mFiles[i];

        if (   !file.location().compare(".")
            &&  (file.format() == CombineArchiveFile::Omex)
            && !file.isMaster()) {
            combineArchiveReferenceFound = true;

            mFiles.removeAt(i);
            // Note: we don't need to track it anymore...

            break;
        }
    }

    if (!combineArchiveReferenceFound) {
        mIssues << CombineArchiveIssue(CombineArchiveIssue::Error,
                                       QObject::tr("no reference to the COMBINE archive itself could be found"));

        mFiles.clear();

        return false;
    }

    return true;
}

//==============================================================================

QString CombineArchive::location(const CombineArchiveFile &pFile) const
{
    // Return the (full) location of the given file

    return mDirName+QDir::separator()+pFile.location();
}

//==============================================================================

CombineArchiveFiles CombineArchive::masterFiles()
{
    // Make sure that we are properly loaded

    if (!load())
        return CombineArchiveFiles();

    // Return a list of our master files

    CombineArchiveFiles res = CombineArchiveFiles();

    foreach (const CombineArchiveFile &file, mFiles) {
        if (file.isMaster())
            res << file;
    }

    return res;
}

//==============================================================================

bool CombineArchive::addFile(const QString &pFileName, const QString &pLocation,
                             const CombineArchiveFile::Format &pFormat,
                             const bool &pMaster)
{
    // Make sure that we are properly loaded

    if (!load())
        return false;

    // Make sure that the format is known

    if (pFormat == CombineArchiveFile::Unknown)
        return false;

    // Add the given file to our list

    mFiles << CombineArchiveFile(pFileName, pLocation, pFormat, pMaster);

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

CombineArchiveIssues CombineArchive::issues() const
{
    // Return our archive's issues

    return mIssues;
}

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

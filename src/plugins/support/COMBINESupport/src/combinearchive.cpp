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

CombineArchive::CombineArchive(const QString &pFileName) :
    StandardSupport::StandardFile(pFileName),
    mDirName(Core::temporaryDirName())
{
    // Reset ourselves

    reset();
}

//==============================================================================

CombineArchive::~CombineArchive()
{
    // Delete our temporary directory

    QDir(mDirName).removeRecursively();
}

//==============================================================================

void CombineArchive::reset()
{
    // Reset all of our properties

    mLoadingNeeded = true;

    mFiles.clear();

    mIssue = QString();
}

//==============================================================================

static const auto ManifestFileName = QStringLiteral("manifest.xml");

//==============================================================================

bool CombineArchive::load()
{
    // Check whether the file is already loaded and without an issue

    if (!mLoadingNeeded)
        return mIssue.isEmpty();

    mLoadingNeeded = false;

    // Make sure that our file exists

    if (!QFile::exists(mFileName)) {
        mIssue = QObject::tr("the archive does not exist");

        return false;
    }

    // Consider the file loaded if it is empty (i.e. new)

    QString fileContents;

    Core::readTextFromFile(mFileName, fileContents);

    if (fileContents.isEmpty())
        return true;

    // We assume our file to be a ZIP file, so extract all of its contents

    OpenCOR::ZIPSupport::QZipReader zipReader(mFileName);

    if (!zipReader.extractAll(mDirName)) {
        mIssue = QObject::tr("the contents of the archive could not be extracted");

        return false;
    }

    // A COMBINE archive must contain a manifest at its root

    QString manifestFileName = mDirName+QDir::separator()+ManifestFileName;

    if (!QFile::exists(manifestFileName)) {
        mIssue = QObject::tr("the archive does not have a manifest");

        return false;
    }

    // Make sure that the manifest is a valid OMEX file

    QString manifestContents;
    QString schemaContents;

    Core::readTextFromFile(manifestFileName, manifestContents);
    Core::readTextFromFile(":omex.xsd", schemaContents);

    if (!Core::validXml(manifestContents, schemaContents)) {
        mIssue = QObject::tr("the manifest is not a valid OMEX file");

        return false;
    }

    // Retrieve the COMBINE archive files from the manifest, making sure that
    // they have been physically extracted

    QDomDocument domDocument;

    domDocument.setContent(manifestContents, true);

    for (QDomElement childElement = domDocument.documentElement().firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        QString location = childElement.attribute("location");
        QString fileName = mDirName+QDir::separator()+location;

        if (!QFile::exists(fileName)) {
            mIssue = QObject::tr("<strong>%1</strong> could not be found").arg(location);

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

    foreach (const CombineArchiveFile &file, mFiles) {
        if (   !file.location().compare(".")
            &&  (file.format() == CombineArchiveFile::Omex)
            && !file.isMaster()) {
            combineArchiveReferenceFound = true;

            break;
        }
    }

    if (!combineArchiveReferenceFound) {
        mIssue = QObject::tr("no reference to the COMBINE archive itself could be found");

        mFiles.clear();

        return false;
    }

    return true;
}

//==============================================================================

bool CombineArchive::save(const QString &pNewFileName)
{
    // Generate the contents our manifest

    QString fileList = QString();
    QString fileFormat;

    foreach (const CombineArchiveFile &file, mFiles) {
        switch (file.format()) {
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
        default:   // CombineArchiveFile::Unknown
            return false;
        }

        fileList += "    <content location=\""+file.location()+"\" format=\""+fileFormat+"\"";

        if (file.isMaster())
            fileList += " master=\"true\"";

        fileList += "/>\n";
    }

    // Save ourselves to either the given file, which name is given, or to our
    // current file

    OpenCOR::ZIPSupport::QZipWriter zipWriter(pNewFileName.isEmpty()?mFileName:pNewFileName);

    zipWriter.addFile(ManifestFileName,
                       "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                       "<omexManifest xmlns=\"http://identifiers.org/combine.specifications/omex-manifest\">\n"
                       "    <content location=\".\" format=\""+OmexFormat.toUtf8()+"\"/>\n"
                      +fileList.toUtf8()
                      +"</omexManifest>\n");

    foreach (const CombineArchiveFile &file, mFiles) {
        QString combineArchiveFileContents;

        if (!Core::readTextFromFile(mDirName+QDir::separator()+file.location(),
                                    combineArchiveFileContents)) {
            return false;
        }

        zipWriter.addFile(file.location(),
                          combineArchiveFileContents.toUtf8());
    }

    return true;
}

//==============================================================================

bool CombineArchive::addFile(const QString &pFileName, const QString &pLocation,
                             const CombineArchiveFile::Format &pFormat,
                             const bool &pMaster)
{
    // Make sure that the file is properly loaded

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

QString CombineArchive::issue() const
{
    // Return our archive's issue

    return mIssue;
}

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

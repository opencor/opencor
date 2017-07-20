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
// SED-ML file
//==============================================================================

#include "cellmlfile.h"
#include "corecliutils.h"
#include "interfaces.h"
#include "sedmlfile.h"
#include "solverinterface.h"

//==============================================================================

#include <QRegularExpression>
#include <QTemporaryFile>

//==============================================================================

#include "sedmlapibegin.h"
    #include "sedml/SedAlgorithm.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedReader.h"
    #include "sedml/SedWriter.h"
#include "sedmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

SedmlFile::SedmlFile(const QString &pFileName, const QString &pOwnerFileName,
                     const bool &pNew) :
    StandardSupport::StandardFile(pFileName),
    mOwnerFileName(pOwnerFileName),
    mSedmlDocument(0),
    mNew(pNew)
{
    // Reset ourselves

    reset();
}

//==============================================================================

SedmlFile::SedmlFile(const QString &pFileName, const bool &pNew) :
    SedmlFile(pFileName, QString(), pNew)
{
}

//==============================================================================

SedmlFile::~SedmlFile()
{
    // Reset ourselves

    reset();
}

//==============================================================================

void SedmlFile::reset()
{
    // Reset all of our properties

    delete mSedmlDocument;

    mSedmlDocument = 0;

    mLoadingNeeded = true;

    mIssues = SedmlFileIssues();
}

//==============================================================================

libsedml::SedDocument * SedmlFile::sedmlDocument()
{
    // Return the SED-ML document associated with our SED-ML file, after loading
    // ourselves if necessary

    load();

    return mSedmlDocument;
}

//==============================================================================

bool SedmlFile::load()
{
    // Check whether the file is already loaded and without any issues

    if (!mLoadingNeeded)
        return !mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_ERROR);

    mLoadingNeeded = false;

    // Create a new SED-ML document, if needed, or try to load our file

    if (mNew)
        mSedmlDocument = new libsedml::SedDocument();
    else
        mSedmlDocument = libsedml::readSedML(mFileName.toUtf8().constData());

    return !mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_ERROR);
}

//==============================================================================

bool SedmlFile::save(const QString &pFileName)
{
    // Make sure that we are properly loaded and have no issue

    if (mLoadingNeeded || mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_ERROR))
        return false;

    // Save ourselves, after having reformatted ourselves, and stop considering
    // ourselves as new anymore (in case we were), if the saving went fine

    QDomDocument domDocument;

    domDocument.setContent(QString(libsedml::writeSedMLToString(mSedmlDocument)));

    bool res = Core::writeFileContentsToFile(pFileName, Core::serialiseDomDocument(domDocument));

    if (res)
        mNew = false;

    return res;
}

//==============================================================================

bool SedmlFile::isValid(const QString &pFileContents, SedmlFileIssues &pIssues)
{
    // Make sure that we are loaded, if the given file contents is empty (i.e.
    // we want to validate ourselves rather than some given file contents)

    if (pFileContents.isEmpty())
        load();

    // Check whether our SED-ML document or the given file contents is SED-ML
    // valid and, if not, populate pIssues with the problems found (after having
    // emptied its contents)
    // Note: in case we have to validate the given file contents, we would
    //       normally create a temporary SED-ML document using
    //       libsedml::readSedMLFromString(), but if the given file contents
    //       doesn't start with:
    //           <?xml version='1.0' encoding='UTF-8'?>
    //       then libsedml::readSedMLFromString() will prepend it to our given
    //       file contents, which is not what we want. So, instead, we create a
    //       temporary file which contents is that of our given file contents,
    //       and simply call libsedml::readSedML()...

    pIssues.clear();

    libsedml::SedDocument *sedmlDocument = mSedmlDocument;

    if (!pFileContents.isEmpty()) {
        QTemporaryFile file;
        QByteArray fileContentsByteArray = pFileContents.toUtf8();

        file.open();

        file.write(fileContentsByteArray);
        file.flush();

        sedmlDocument = libsedml::readSedML(file.fileName().toUtf8().constData());

        file.close();
    }

    libsedml::SedErrorLog *errorLog = sedmlDocument->getErrorLog();

    for (uint i = 0, iMax = errorLog->getNumErrors(); i < iMax; ++i) {
        const libsedml::SedError *error = errorLog->getError(i);
        SedmlFileIssue::Type issueType;

        switch (error->getSeverity()) {
        case LIBSBML_SEV_INFO:
            issueType = SedmlFileIssue::Information;

            break;
        case LIBSBML_SEV_ERROR:
            issueType = SedmlFileIssue::Error;

            break;
        case LIBSBML_SEV_WARNING:
            issueType = SedmlFileIssue::Warning;

            break;
        case LIBSBML_SEV_FATAL:
            issueType = SedmlFileIssue::Fatal;

            break;
        }

        static const QRegularExpression TrailingEmptyLinesRegEx = QRegularExpression("[\\n]*$");

        QString errorMessage = QString::fromStdString(error->getMessage()).remove(TrailingEmptyLinesRegEx);

        pIssues << SedmlFileIssue(issueType, error->getLine(), error->getColumn(), errorMessage);
    }

    // Only consider our SED-ML document valid if it has no errors

    bool res = !sedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_ERROR);

    if (!pFileContents.isEmpty())
        delete sedmlDocument;

    return res;
}

//==============================================================================

bool SedmlFile::isValid()
{
    // Return whether we are valid

    return isValid(QString(), mIssues);
}

//==============================================================================

bool SedmlFile::algorithmSupported(const libsedml::SedAlgorithm *pSedmlAlgorithm,
                                   SedmlFileIssues &pIssues)
{
    // Make sure that we have an algorithm

    if (!pSedmlAlgorithm) {
        pIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files with one or two simulations with an algorithm are supported"));

        return false;
    }

    // Make sure that the given algorithm relies on an algorithm that we support

    SolverInterface *usedSolverInterface = 0;
    QString kisaoId = QString::fromStdString(pSedmlAlgorithm->getKisaoID());

    foreach (SolverInterface *solverInterface, Core::solverInterfaces()) {
        if (!solverInterface->id(kisaoId).compare(solverInterface->solverName())) {
            usedSolverInterface = solverInterface;

            break;
        }
    }

    if (!usedSolverInterface) {
        pIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("unsupported algorithm (%1)").arg(kisaoId));

        return false;
    }

    // Make sure that the algorithm parameters are also supported

    for (int i = 0, iMax = pSedmlAlgorithm->getNumAlgorithmParameters(); i < iMax; ++i) {
        QString kisaoId = QString::fromStdString(pSedmlAlgorithm->getAlgorithmParameter(i)->getKisaoID());
        QString id = usedSolverInterface->id(kisaoId);

        if (id.isEmpty() || !id.compare(usedSolverInterface->solverName())) {
            pIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("unsupported algorithm parameter (%1)").arg(kisaoId));

            return false;
        }
    }

    // Make sure that the annotation, if any, contains at least the kind of
    // information we would expect

    libsbml::XMLNode *annotation = pSedmlAlgorithm->getAnnotation();

    if (annotation) {
        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const XMLNode &node = annotation->getChild(i);

            if (   QString::fromStdString(node.getURI()).compare(OpencorNamespace)
                || QString::fromStdString(node.getName()).compare(SolverProperties)) {
                continue;
            }

            bool validSolverProperties = true;

            for (uint j = 0, jMax = node.getNumChildren(); j < jMax; ++j) {
                const XMLNode &solverPropertyNode = node.getChild(j);

                if (   QString::fromStdString(solverPropertyNode.getURI()).compare(OpencorNamespace)
                    || QString::fromStdString(solverPropertyNode.getName()).compare(SolverProperty)) {
                    continue;
                }

                int idIndex = solverPropertyNode.getAttrIndex(SolverPropertyId.toStdString());
                int valueIndex = solverPropertyNode.getAttrIndex(SolverPropertyValue.toStdString());

                if (   (idIndex == -1) || (valueIndex == -1)
                    || solverPropertyNode.getAttrValue(idIndex).empty()
                    || solverPropertyNode.getAttrValue(valueIndex).empty()) {
                    validSolverProperties = false;

                    break;
                }
            }

            if (!validSolverProperties) {
                pIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                          tr("incomplete algorithm annotation (missing algorithm property information)"));

                return false;
            }
        }
    }

    return true;
}

//==============================================================================

SedmlFileIssues SedmlFile::issues() const
{
    // Return our issues

    return mIssues;
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

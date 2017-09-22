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
#include "coreguiutils.h"
#include "filemanager.h"
#include "interfaces.h"
#include "sedmlfile.h"
#include "sedmlsupport.h"
#include "solverinterface.h"

//==============================================================================

#include <QDir>
#include <QRegularExpression>
#include <QTemporaryFile>

//==============================================================================

#include "sedmlapibegin.h"
    #include "sedml/SedAlgorithm.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedOneStep.h"
    #include "sedml/SedPlot2D.h"
    #include "sedml/SedReader.h"
    #include "sedml/SedRepeatedTask.h"
    #include "sedml/SedWriter.h"
    #include "sedml/SedUniformTimeCourse.h"
    #include "sedml/SedVectorRange.h"
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
    mNew(pNew),
    mCellmlFile(0)
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
    // Ask our file manager to unmanage our corresponding CellML file, if we
    // have previously retrieved it, and if it is a remote one (indeed, it will
    // have been managed by cellmlFile() below, so that CellML 1.1 files can be
    // properly instantiated)

    if (mCellmlFile) {
        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        QString cellmlFileName = mCellmlFile->fileName();

        if (fileManagerInstance->isRemote(cellmlFileName))
            fileManagerInstance->unmanage(cellmlFileName);
    }

    // Reset all of our properties

    delete mSedmlDocument;

    mSedmlDocument = 0;

    mLoadingNeeded = true;

    delete mCellmlFile;

    mCellmlFile = 0;

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
    // Check whether the file is already loaded and without any (fatal) errors

    if (!mLoadingNeeded) {
        return    !mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_ERROR)
               && !mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_FATAL);
    }

    mLoadingNeeded = false;

    // Create a new SED-ML document, if needed, or try to load our file

    if (mNew)
        mSedmlDocument = new libsedml::SedDocument();
    else
        mSedmlDocument = libsedml::readSedML(mFileName.toUtf8().constData());

    return    !mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_ERROR)
           && !mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_FATAL);
}

//==============================================================================

bool SedmlFile::save(const QString &pFileName)
{
    // Make sure that we are properly loaded and have no (fatal) errors

    if (   mLoadingNeeded
        || mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_ERROR)
        || mSedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_FATAL)) {
        return false;
    }

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

    // Only consider our SED-ML document valid if it has no (fatal) errors

    bool res =    !sedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_ERROR)
               && !sedmlDocument->getNumErrors(libsedml::LIBSEDML_SEV_FATAL);

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

bool SedmlFile::validColorPropertyValue(const libsbml::XMLNode &pPropertyNode,
                                        const QString &pPropertyNodeValue,
                                        const QString &pPropertyName)
{
    // Check whether the given color property is valid

    static const QRegularExpression ColorRegEx = QRegularExpression("^#[[:xdigit:]]{6}$");

    if (!ColorRegEx.match(pPropertyNodeValue).hasMatch()) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                  pPropertyNode.getLine(),
                                  pPropertyNode.getColumn(),
                                  tr("the '%1' property must have a value of '#RRGGBB'").arg(pPropertyName));

        return false;
    } else {
        return true;
    }
}

//==============================================================================

bool SedmlFile::isSupported()
{
    // Make sure that we are valid

    if (!isValid())
        return false;

    // Make sure that there is only one model

    if (mSedmlDocument->getNumModels() != 1) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files with one model are supported"));

        return false;
    }

    // Make sure that the model is of CellML type

    libsedml::SedModel *model = mSedmlDocument->getModel(0);
    QString language = QString::fromStdString(model->getLanguage());

    if (   language.compare(Language::Cellml)
        && language.compare(Language::Cellml_1_0)
        && language.compare(Language::Cellml_1_1)) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files with a CellML file are supported"));

        return false;
    }

    // Make sure that there is either one or two simulations

    int nbOfSimulations = mSedmlDocument->getNumSimulations();

    if ((nbOfSimulations != 1) && (nbOfSimulations != 2)) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files with one or two simulations are supported"));

        return false;
    }

    // Make sure that the first simulation is a uniform time course simulation

    libsedml::SedSimulation *firstSimulation = mSedmlDocument->getSimulation(0);

    if (firstSimulation->getTypeCode() != libsedml::SEDML_SIMULATION_UNIFORMTIMECOURSE) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files with a uniform time course as a (first) simulation are supported"));

        return false;
    }

    // Make sure that the initial time and output start time are the same, that
    // the output start time and output end time are different, and that the
    // number of points is greater than zero

    libsedml::SedUniformTimeCourse *uniformTimeCourse = static_cast<libsedml::SedUniformTimeCourse *>(firstSimulation);
    double initialTime = uniformTimeCourse->getInitialTime();
    double outputStartTime = uniformTimeCourse->getOutputStartTime();
    double outputEndTime = uniformTimeCourse->getOutputEndTime();
    int nbOfPoints = uniformTimeCourse->getNumberOfPoints();

    if (initialTime != outputStartTime) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files with the same values for 'initialTime' and 'outputStartTime' are supported"));

        return false;
    }

    if (outputStartTime == outputEndTime) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                  tr("the values for 'outputStartTime' and 'outputEndTime' must be different"));

        return false;
    }

    if (nbOfPoints <= 0) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                  tr("the value for 'numberOfPoints' must be greater than zero"));

        return false;
    }

    // Make sure that we have an algorithm for the first simulation

    const libsedml::SedAlgorithm *firstSimulationAlgorithm = firstSimulation->getAlgorithm();

    if (firstSimulationAlgorithm) {
        // Make sure that the algorithm relies on an algorithm that we support

        SolverInterface *usedSolverInterface = 0;
        QString kisaoId = QString::fromStdString(firstSimulationAlgorithm->getKisaoID());

        foreach (SolverInterface *solverInterface, Core::solverInterfaces()) {
            if (!solverInterface->id(kisaoId).compare(solverInterface->solverName())) {
                usedSolverInterface = solverInterface;

                break;
            }
        }

        if (!usedSolverInterface) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("unsupported algorithm (%1)").arg(kisaoId));

            return false;
        }

        // Make sure that the algorithm parameters are also supported

        for (int i = 0, iMax = firstSimulationAlgorithm->getNumAlgorithmParameters(); i < iMax; ++i) {
            QString kisaoId = QString::fromStdString(firstSimulationAlgorithm->getAlgorithmParameter(i)->getKisaoID());
            QString id = usedSolverInterface->id(kisaoId);

            if (id.isEmpty() || !id.compare(usedSolverInterface->solverName())) {
                mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                          tr("unsupported algorithm parameter (%1)").arg(kisaoId));

                return false;
            }
        }

        // Make sure that the first simulation algorithm annotation, if any,
        // contains at least the kind of information we would expect

        libsbml::XMLNode *firstSimulationAlgorithmAnnotation = firstSimulationAlgorithm->getAnnotation();

        if (firstSimulationAlgorithmAnnotation) {
            for (uint i = 0, iMax = firstSimulationAlgorithmAnnotation->getNumChildren(); i < iMax; ++i) {
                const libsbml::XMLNode &solverPropertiesNode = firstSimulationAlgorithmAnnotation->getChild(i);

                if (   QString::fromStdString(solverPropertiesNode.getURI()).compare(OpencorNamespace)
                    || QString::fromStdString(solverPropertiesNode.getName()).compare(SolverProperties)) {
                    continue;
                }

                bool validSolverProperties = true;

                for (uint j = 0, jMax = solverPropertiesNode.getNumChildren(); j < jMax; ++j) {
                    const libsbml::XMLNode &solverPropertyNode = solverPropertiesNode.getChild(j);

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
                    mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                              solverPropertiesNode.getLine(),
                                              solverPropertiesNode.getColumn(),
                                              tr("incomplete algorithm annotation (missing algorithm property information)"));

                    return false;
                }
            }
        }

        // Make sure that the first simulation annotation, if any, contains at
        // least the kind of information we would expect

        libsbml::XMLNode *firstSimulationAnnotation = firstSimulation->getAnnotation();

        if (firstSimulationAnnotation) {
            bool hasNlaSolver = false;

            for (uint i = 0, iMax = firstSimulationAnnotation->getNumChildren(); i < iMax; ++i) {
                const libsbml::XMLNode &nlaSolverNode = firstSimulationAnnotation->getChild(i);

                if (   QString::fromStdString(nlaSolverNode.getURI()).compare(OpencorNamespace)
                    || QString::fromStdString(nlaSolverNode.getName()).compare(NlaSolver)) {
                    continue;
                }

                int nameIndex = nlaSolverNode.getAttrIndex(NlaSolverName.toStdString());

                if ((nameIndex != -1) && !nlaSolverNode.getAttrValue(nameIndex).empty()) {
                    if (hasNlaSolver) {
                        mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                                  tr("only one NLA solver is allowed"));

                        return false;
                    } else {
                        hasNlaSolver = true;
                    }
                } else {
                    mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                              nlaSolverNode.getLine(),
                                              nlaSolverNode.getColumn(),
                                              tr("incomplete simulation annotation (missing NLA solver name)"));

                    return false;
                }
            }
        }
    } else {
        mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files with one or two simulations with an algorithm are supported"));

        return false;
    }

    // Check whether there is a second simulation

    libsedml::SedSimulation *secondSimulation = mSedmlDocument->getSimulation(1);

    if (secondSimulation) {
        // Make sure that the second simulation is a one-step simulation

        if (secondSimulation->getTypeCode() != libsedml::SEDML_SIMULATION_ONESTEP) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("only SED-ML files with a one-step as a second simulation are supported"));

            return false;
        }

        // Make sure that its step is greater than zero

        if (static_cast<libsedml::SedOneStep *>(secondSimulation)->getStep() <= 0) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                      tr("the value for step must be greater than zero"));

            return false;
        }

        // Make sure that its algorithm and annotation(s), if any, are the same
        // as for the first simulation

        std::stringstream firstStream;
        std::stringstream secondStream;
        libsbml::XMLOutputStream firstXmlStream(firstStream);
        libsbml::XMLOutputStream secondXmlStream(secondStream);
        const libsedml::SedAlgorithm *secondSimulationAlgorithm = secondSimulation->getAlgorithm();

        firstSimulationAlgorithm->write(firstXmlStream);

        if (secondSimulationAlgorithm)
            secondSimulationAlgorithm->write(secondXmlStream);

        libsbml::XMLNode *firstSimulationAlgorithmAnnotation = firstSimulationAlgorithm->getAnnotation();
        libsbml::XMLNode *secondSimulationAlgorithmAnnotation = secondSimulationAlgorithm->getAnnotation();

        if (firstSimulationAlgorithmAnnotation)
            firstSimulationAlgorithmAnnotation->write(firstXmlStream);

        if (secondSimulationAlgorithmAnnotation)
            secondSimulationAlgorithmAnnotation->write(secondXmlStream);

        libsbml::XMLNode *firstSimulationAnnotation = firstSimulation->getAnnotation();
        libsbml::XMLNode *secondSimulationAnnotation = secondSimulation->getAnnotation();

        if (firstSimulationAnnotation)
            firstSimulationAnnotation->write(firstXmlStream);

        if (secondSimulationAnnotation)
            secondSimulationAnnotation->write(secondXmlStream);

        if (firstStream.str().compare(secondStream.str())) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("only SED-ML files with two simulations with the same algorithm are supported"));

            return false;
        }
    }

    // Make sure that we have only one repeated task, which aim is to execute
    // each simulation (using a sub-task) once

    uint totalNbOfTasks = secondSimulation?3:2;

    if (mSedmlDocument->getNumTasks() != totalNbOfTasks) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files that execute one or two simulations once are supported"));

        return false;
    }

    libsedml::SedRepeatedTask *repeatedTask = 0;

    bool repeatedTaskOk = false;
    std::string repeatedTaskFirstSubTaskId = std::string();
    std::string repeatedTaskSecondSubTaskId = std::string();

    bool firstSubTaskOk = false;
    std::string firstSubTaskId = std::string();

    bool secondSubTaskOk = false;
    std::string secondSubTaskId = std::string();

    for (uint i = 0; i < totalNbOfTasks; ++i) {
        libsedml::SedTask *task = mSedmlDocument->getTask(i);

        if (task->getTypeCode() == libsedml::SEDML_TASK_REPEATEDTASK) {
            // Make sure that the repeated task asks for the model to be reset,
            // that it has one range, no task change and one/two sub-task/s

            repeatedTask = static_cast<libsedml::SedRepeatedTask *>(task);

            if (    repeatedTask->getResetModel()
                &&  (repeatedTask->getNumRanges() == 1)
                && !repeatedTask->getNumTaskChanges()
                &&  (repeatedTask->getNumSubTasks() == totalNbOfTasks-1)) {
                // Make sure that the range is a vector range and that it's the
                // one referenced in the repeated task

                libsedml::SedRange *range = repeatedTask->getRange(0);

                if (    (range->getTypeCode() == libsedml::SEDML_RANGE_VECTORRANGE)
                    && !repeatedTask->getRangeId().compare(range->getId())) {
                    // Make sure that the vector range has one value that is
                    // equal to 1

                    libsedml::SedVectorRange *vectorRange = static_cast<libsedml::SedVectorRange *>(range);

                    if (   (vectorRange->getNumValues() == 1)
                        && (vectorRange->getValues().front() == 1)) {
                        // Make sure that the one/two sub-tasks have the correct
                        // order and retrieve their id

                        for (uint i = 0, iMax = totalNbOfTasks-1; i < iMax; ++i) {
                            libsedml::SedSubTask *subTask = repeatedTask->getSubTask(i);

                            if (subTask->getOrder() == 1)
                                repeatedTaskFirstSubTaskId = subTask->getTask();
                            else if (subTask->getOrder() == 2)
                                repeatedTaskSecondSubTaskId = subTask->getTask();
                        }

                        repeatedTaskOk = true;
                    }
                }
            }
        } else if (task->getTypeCode() == libsedml::SEDML_TASK) {
            // Make sure the sub-task references the correct model and
            // simulation

            if (   !task->getModelReference().compare(model->getId())
                && !task->getSimulationReference().compare(firstSimulation->getId())) {
                firstSubTaskOk = true;
                firstSubTaskId = task->getId();
            } else if (   secondSimulation
                       && !task->getModelReference().compare(model->getId())
                       && !task->getSimulationReference().compare(secondSimulation->getId())) {
                secondSubTaskOk = true;
                secondSubTaskId = task->getId();
            }
        }
    }

    if (   !repeatedTaskOk
        || !firstSubTaskOk || repeatedTaskFirstSubTaskId.compare(firstSubTaskId)
        || (     secondSimulation
            && (!secondSubTaskOk || repeatedTaskSecondSubTaskId.compare(secondSubTaskId)))) {
        mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                  tr("only SED-ML files that execute one or two simulations once are supported"));

        return false;
    }

    // Make sure that all the data generators have one variable that references
    // the repeated task, that follows the correct CellML format for their
    // target (and OpenCOR format for their degree, if any), and that is not
    // modified

    for (uint i = 0, iMax = mSedmlDocument->getNumDataGenerators(); i < iMax; ++i) {
        libsedml::SedDataGenerator *dataGenerator = mSedmlDocument->getDataGenerator(i);

        if ((dataGenerator->getNumVariables() != 1) || dataGenerator->getNumParameters()) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("only SED-ML files with data generators for one variable are supported"));

            return false;
        }

        libsedml::SedVariable *variable = dataGenerator->getVariable(0);

        if (variable->getSymbol().size() || variable->getModelReference().size()) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("only SED-ML files with data generators for one variable with a target and a task reference are supported"));

            return false;
        }

        if (variable->getTaskReference().compare(repeatedTask->getId())) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("only SED-ML files with data generators for one variable with a reference to a repeated task are supported"));

            return false;
        }

        static const QRegularExpression TargetStartRegEx  = QRegularExpression("^\\/cellml:model\\/cellml:component\\[@name='");
        static const QRegularExpression TargetMiddleRegEx = QRegularExpression("']\\/cellml:variable\\[@name='");
        static const QRegularExpression TargetEndRegEx    = QRegularExpression("'\\]$");

        bool referencingCellmlVariable = false;
        QString target = QString::fromStdString(variable->getTarget());

        if (target.contains(TargetStartRegEx) && target.contains(TargetEndRegEx)) {
            static const QString Separator = "|";

            target.remove(TargetStartRegEx);
            target.replace(TargetMiddleRegEx, Separator);
            target.remove(TargetEndRegEx);

            QStringList identifiers = target.split(Separator);

            if (identifiers.count() == 2) {
                static const QRegularExpression IdentifierRegEx = QRegularExpression("^[[:alpha:]_][[:alnum:]_]*$");

                QString componentName = identifiers.first();
                QString variableName = identifiers.last();

                referencingCellmlVariable =    IdentifierRegEx.match(componentName).hasMatch()
                                            && IdentifierRegEx.match(variableName).hasMatch();
            }
        }

        if (!referencingCellmlVariable) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("only SED-ML files with data generators for one variable with a reference to a CellML variable are supported"));

            return false;
        }

        libsbml::XMLNode *variableAnnotation = variable->getAnnotation();

        if (variableAnnotation) {
            for (uint i = 0, iMax = variableAnnotation->getNumChildren(); i < iMax; ++i) {
                const libsbml::XMLNode &variableDegreeNode = variableAnnotation->getChild(i);

                if (   QString::fromStdString(variableDegreeNode.getURI()).compare(OpencorNamespace)
                    || QString::fromStdString(variableDegreeNode.getName()).compare(VariableDegree)) {
                    continue;
                }

                bool validVariableDegree = false;

                if (variableDegreeNode.getNumChildren() == 1) {
                    bool conversionOk;
                    int variableDegree = QString::fromStdString(variableDegreeNode.getChild(0).getCharacters()).toInt(&conversionOk);

                    validVariableDegree = conversionOk && (variableDegree >= 0);
                }

                if (!validVariableDegree) {
                    mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                              variableDegreeNode.getLine(),
                                              variableDegreeNode.getColumn(),
                                              tr("only SED-ML files with data generators for one variable that is derived or not are supported"));

                    return false;
                }
            }
        }

        const libsbml::ASTNode *mathNode = dataGenerator->getMath();

        if (   (mathNode->getType() != libsbml::AST_NAME)
            || variable->getId().compare(mathNode->getName())) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("only SED-ML files with data generators for one variable that is not modified are supported"));

            return false;
        }
    }

    // Make sure that all the outputs are 2D outputs

    for (uint i = 0, iMax = mSedmlDocument->getNumOutputs(); i < iMax; ++i) {
        libsedml::SedOutput *output = mSedmlDocument->getOutput(i);

        if (output->getTypeCode() != libsedml::SEDML_OUTPUT_PLOT2D) {
            mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                      tr("only SED-ML files with 2D outputs are supported"));

            return false;
        }

        // Make sure that the curves reference listed data generators and don't
        // use logarithmic axes

        libsedml::SedPlot2D *plot = static_cast<libsedml::SedPlot2D *>(output);
        bool initialiseLogs = true;
        bool logX;
        bool logY;

        for (uint j = 0, jMax = plot->getNumCurves(); j < jMax; ++j) {
            libsedml::SedCurve *curve = plot->getCurve(j);

            if (initialiseLogs) {
                initialiseLogs = false;

                logX = curve->getLogX();
                logY = curve->getLogY();
            }

            if ((curve->getLogX() != logX) || (curve->getLogY() != logY)) {
                mIssues << SedmlFileIssue(SedmlFileIssue::Information,
                                          tr("only SED-ML files with curves of the same type (with regards to linear/logarithmic scaling) are supported"));

                return false;
            }

            if (   !mSedmlDocument->getDataGenerator(curve->getXDataReference())
                || !mSedmlDocument->getDataGenerator(curve->getYDataReference())) {
                mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                          tr("a curve must reference existing data generators"));

                return false;
            }

            libsbml::XMLNode *curveAnnotation = curve->getAnnotation();

            if (curveAnnotation) {
                for (uint i = 0, iMax = curveAnnotation->getNumChildren(); i < iMax; ++i) {
                    const libsbml::XMLNode &curvePropertiesNode = curveAnnotation->getChild(i);

                    if (   QString::fromStdString(curvePropertiesNode.getURI()).compare(OpencorNamespace)
                        || QString::fromStdString(curvePropertiesNode.getName()).compare(CurveProperties)) {
                        continue;
                    }

                    for (uint j = 0, jMax = curvePropertiesNode.getNumChildren(); j < jMax; ++j) {
                        const libsbml::XMLNode &lineOrSymbolPropertiesNode = curvePropertiesNode.getChild(j);
                        QString lineOrSymbolPropertiesNodeName = QString::fromStdString(lineOrSymbolPropertiesNode.getName());
                        bool isLinePropertiesNode = !lineOrSymbolPropertiesNodeName.compare(LineProperties);
                        bool isSymbolPropertiesNode = !lineOrSymbolPropertiesNodeName.compare(SymbolProperties);

                        if (!isLinePropertiesNode && !isSymbolPropertiesNode)
                            continue;

                        if (isLinePropertiesNode) {
                            for (uint k = 0, kMax = lineOrSymbolPropertiesNode.getNumChildren(); k < kMax; ++k) {
                                const libsbml::XMLNode &linePropertyNode = lineOrSymbolPropertiesNode.getChild(k);
                                QString linePropertyNodeName = QString::fromStdString(linePropertyNode.getName());
                                bool linePropertyNodeHasOneChild = linePropertyNode.getNumChildren() == 1;
                                QString linePropertyNodeValue = linePropertyNodeHasOneChild?QString::fromStdString(linePropertyNode.getChild(0).getCharacters()):QString();

                                if (!linePropertyNodeName.compare(LineStyle)) {
                                    static const QStringList LineStyles = lineStyles();
                                    static const int LineStylesCountMinusOne = LineStyles.count()-1;

                                    if (!LineStyles.contains(linePropertyNodeValue)) {
                                        QString lineStyles = QString();
                                        int i = -1;

                                        foreach (const QString &lineStyle, LineStyles) {
                                            if (++i) {
                                                if (i == LineStylesCountMinusOne)
                                                    lineStyles += " "+tr("or")+" ";
                                                else
                                                    lineStyles += ", ";
                                            }

                                            lineStyles += "'"+lineStyle+"'";
                                        }

                                        mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                                                  linePropertyNode.getLine(),
                                                                  linePropertyNode.getColumn(),
                                                                  tr("the '%1' property must have a value of %2").arg(linePropertyNodeName, lineStyles));

                                        return false;
                                    }
                                } else if (!linePropertyNodeName.compare(LineWidth)) {
                                    static const QRegularExpression DoubleGt0RegEx = QRegularExpression("^[+]?(([1-9]\\d*)?(\\.\\d*)?|[0]?\\.\\d+)([eE][+-]?\\d+)?$");

                                    if (!DoubleGt0RegEx.match(linePropertyNodeValue).hasMatch()) {
                                        mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                                                  linePropertyNode.getLine(),
                                                                  linePropertyNode.getColumn(),
                                                                  tr("the '%1' property value must be a number greater than zero").arg(linePropertyNodeName));

                                        return false;
                                    }
                                } else if (   !linePropertyNodeName.compare(LineColor)
                                           && !validColorPropertyValue(linePropertyNode, linePropertyNodeValue, LineColor)) {
                                    return false;
                                }
                            }
                        } else {
                            for (uint k = 0, kMax = lineOrSymbolPropertiesNode.getNumChildren(); k < kMax; ++k) {
                                const libsbml::XMLNode &symbolPropertyNode = lineOrSymbolPropertiesNode.getChild(k);
                                QString symbolPropertyNodeName = QString::fromStdString(symbolPropertyNode.getName());
                                bool symbolPropertyNodeHasOneChild = symbolPropertyNode.getNumChildren() == 1;
                                QString symbolPropertyNodeValue = symbolPropertyNodeHasOneChild?QString::fromStdString(symbolPropertyNode.getChild(0).getCharacters()):QString();

                                if (!symbolPropertyNodeName.compare(SymbolStyle)) {
                                    static const QStringList SymbolStyles = symbolStyles();
                                    static const int SymbolStylesCountMinusOne = SymbolStyles.count()-1;

                                    if (!SymbolStyles.contains(symbolPropertyNodeValue)) {
                                        QString symbolStyles = QString();
                                        int i = -1;

                                        foreach (const QString &symbolStyle, SymbolStyles) {
                                            if (++i) {
                                                if (i == SymbolStylesCountMinusOne)
                                                    symbolStyles += " "+tr("or")+" ";
                                                else
                                                    symbolStyles += ", ";
                                            }

                                            symbolStyles += "'"+symbolStyle+"'";
                                        }

                                        mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                                                  symbolPropertyNode.getLine(),
                                                                  symbolPropertyNode.getColumn(),
                                                                  tr("the '%1' property must have a value of %2").arg(symbolPropertyNodeName, symbolStyles));

                                        return false;
                                    }
                                } else if (!symbolPropertyNodeName.compare(SymbolSize)) {
                                    static const QRegularExpression IntegerGt0RegEx = QRegularExpression("^[+]?[1-9]\\d*$");

                                    if (!IntegerGt0RegEx.match(symbolPropertyNodeValue).hasMatch()) {
                                        mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                                                  symbolPropertyNode.getLine(),
                                                                  symbolPropertyNode.getColumn(),
                                                                  tr("the '%1' property value must be an integer greater than zero").arg(symbolPropertyNodeName));

                                        return false;
                                    }
                                } else if (   !symbolPropertyNodeName.compare(SymbolColor)
                                           && !validColorPropertyValue(symbolPropertyNode, symbolPropertyNodeValue, SymbolColor)) {
                                    return false;
                                } else if (   !symbolPropertyNodeName.compare(SymbolFilled)
                                           &&  symbolPropertyNodeValue.compare("true")
                                           && symbolPropertyNodeValue.compare("false")) {
                                    mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                                              symbolPropertyNode.getLine(),
                                                              symbolPropertyNode.getColumn(),
                                                              tr("the 'symbolFilled' property must have a value of 'true' or 'false'"));

                                    return false;
                                } else if (   !symbolPropertyNodeName.compare(SymbolFillColor)
                                           && !validColorPropertyValue(symbolPropertyNode, symbolPropertyNodeValue, SymbolFillColor)) {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

//==============================================================================

CellMLSupport::CellmlFile * SedmlFile::cellmlFile()
{
    // Return our CellML file, after having created it, if necessary

    if (!mCellmlFile && isSupported()) {
        // Retrieve the source of the CellML file, if any

        QString modelSource = QString::fromStdString(sedmlDocument()->getModel(0)->getSource());

        // Check whether we are dealing with a local file (which location is
        // relative to ourselves) or a remote file

        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        bool hasOwner = !mOwnerFileName.isEmpty();
        QString realFileName = hasOwner?mOwnerFileName:mFileName;
        QString url = fileManagerInstance->file(realFileName)->url();
        bool isLocalFile;
        QString dummy;

        Core::checkFileNameOrUrl(modelSource, isLocalFile, dummy);

        if (isLocalFile && (url.isEmpty() || hasOwner)) {
            // By default, our model source refers to a file name relative to
            // ourselves

            QString cellmlFileName = Core::nativeCanonicalFileName(QFileInfo(mFileName).path()+QDir::separator()+modelSource);

#ifdef Q_OS_WIN
            // On Windows, if our model source exists, it means that it refers
            // to a file on a different drive rather than to a file name
            // relative to ourselves

            if (QFile::exists(modelSource))
                cellmlFileName = modelSource;
#endif

            if (QFile::exists(cellmlFileName)) {
                mCellmlFile = new CellMLSupport::CellmlFile(cellmlFileName);

                // Set our CellML file and its dependencies, if any, as
                // dependencies for ourselves, if we don't have an owner

                if (!hasOwner) {
                    fileManagerInstance->setDependencies(realFileName,
                                                         QStringList() << mCellmlFile->fileName()
                                                                       << mCellmlFile->dependencies(true));
                }
            } else {
                mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                          tr("%1 could not be found").arg(modelSource));
            }
        } else {
            // Handle the case where our model source is a relative remote file

            static const QRegularExpression FileNameRegEx = QRegularExpression("/[^/]*$");

            if (isLocalFile)
                modelSource = url.remove(FileNameRegEx)+"/"+modelSource;

            // Retrieve the contents of our model source

            QString fileContents;
            QString errorMessage;

            if (Core::readFileContentsFromUrlWithBusyWidget(modelSource, fileContents, &errorMessage)) {
                // Save the contents of our model source to a local file and use
                // that to create a CellML file object after having asked our
                // file manager to manage it (so that CellML 1.1 files can be
                // properly instantiated)

                QString cellmlFileName = Core::temporaryFileName();

                if (Core::writeFileContentsToFile(cellmlFileName, fileContents)) {
                    fileManagerInstance->manage(cellmlFileName, Core::File::Remote, modelSource);

                    mCellmlFile = new CellMLSupport::CellmlFile(cellmlFileName);
                } else {
                    mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                              tr("%1 could not be saved").arg(modelSource));
                }
            } else {
                mIssues << SedmlFileIssue(SedmlFileIssue::Error,
                                          tr("%1 could not be retrieved (%2)").arg(modelSource, Core::formatMessage(errorMessage)));
            }
        }
    }

    return mCellmlFile;
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

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
// CellML tools plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlinterface.h"
#include "cellmltoolsplugin.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QMenu>

//==============================================================================

#include <iostream>

//==============================================================================

namespace OpenCOR {
namespace CellMLTools {

//==============================================================================

PLUGININFO_FUNC CellMLToolsPluginInfo()
{
    static const Descriptions descriptions = {
                                                 { "en", QString::fromUtf8(R"(a plugin to access various <a href="https://cellml.org/">CellML</a>-related tools.)") },
                                                 { "fr", QString::fromUtf8(R"(une extension pour accéder divers outils en rapport avec <a href="https://cellml.org/">CellML</a>.)") }
                                             };

    return new PluginInfo(PluginInfo::Category::Tools, true, true,
                          { "CellMLSupport" },
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

bool CellMLToolsPlugin::executeCommand(const QString &pCommand,
                                       const QStringList &pArguments, int &pRes)
{
    Q_UNUSED(pRes)

    // Run the given CLI command

    static const QString Help     = "help";
    static const QString Export   = "export";
    static const QString Validate = "validate";

    if (pCommand == Help) {
        // Display the commands that we support

        runHelpCommand();

        return true;
    }

    if (pCommand == Export) {
        // Export a file from one format to another

        return runExportCommand(pArguments);
    }

    if (pCommand == Validate) {
        // Validate a file

        return runValidateCommand(pArguments);
    }

    // Not a CLI command that we support

    runHelpCommand();

    return false;
}

//==============================================================================
// GUI interface
//==============================================================================

void CellMLToolsPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin)

    // Enable/disable our tools in case we are dealing with a CellML file

    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::fileVersion(pFileName);

    mExportToCellml10Action->setEnabled(cellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_1);

    bool exportToLanguageEnabled =    (cellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_0)
                                   || (cellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_1);

    mExportToCAction->setEnabled(exportToLanguageEnabled);
    mExportToFortran77Action->setEnabled(exportToLanguageEnabled);
    mExportToMatlabAction->setEnabled(exportToLanguageEnabled);
    mExportToPythonAction->setEnabled(exportToLanguageEnabled);

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

Gui::Menus CellMLToolsPlugin::guiMenus() const
{
    // We don't handle this interface...

    return {};
}

//==============================================================================

Gui::MenuActions CellMLToolsPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::Type::Tools, mCellmlFileExportToMenu->menuAction())
                              << Gui::MenuAction(Gui::MenuAction::Type::Tools, Core::newSeparator(Core::mainWindow()));
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLToolsPlugin::retranslateUi()
{
    // Retranslate our different Tools actions

    retranslateMenu(mCellmlFileExportToMenu, tr("CellML File Export To"));

    retranslateAction(mExportToCellml10Action, tr("CellML 1.0..."),
                      tr("Export the CellML file to CellML 1.0"));

    retranslateAction(mExportToCAction, tr("C..."),
                      tr("Export the CellML file to C"));
    retranslateAction(mExportToFortran77Action, tr("FORTRAN 77..."),
                      tr("Export the CellML file to FORTRAN 77"));
    retranslateAction(mExportToMatlabAction, tr("MATLAB..."),
                      tr("Export the CellML file to MATLAB"));
    retranslateAction(mExportToPythonAction, tr("Python..."),
                      tr("Export the CellML file to Python"));
}

//==============================================================================
// Plugin interface
//==============================================================================

bool CellMLToolsPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLToolsPlugin::pluginInterfacesOk(const QString &pFileName,
                                           QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CellMLToolsPlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (Core::mainWindow() == nullptr) {
        return;
    }

    // Create our Tools | Export To menu

    mCellmlFileExportToMenu = Core::newMenu("CellmlFileExportTo", Core::mainWindow());

    // Create our different Tools | Export To actions, and add them to our
    // Tools | Export To menu

    mExportToCellml10Action = Core::newAction(Core::mainWindow());

    mExportToCAction = Core::newAction(Core::mainWindow());
    mExportToFortran77Action = Core::newAction(Core::mainWindow());
    mExportToMatlabAction = Core::newAction(Core::mainWindow());
    mExportToPythonAction = Core::newAction(Core::mainWindow());

    mCellmlFileExportToMenu->addAction(mExportToCellml10Action);
    mCellmlFileExportToMenu->addSeparator();
    mCellmlFileExportToMenu->addAction(mExportToCAction);
    mCellmlFileExportToMenu->addAction(mExportToFortran77Action);
    mCellmlFileExportToMenu->addAction(mExportToMatlabAction);
    mCellmlFileExportToMenu->addAction(mExportToPythonAction);

    // Some connections to handle our different Tools | Export To actions

    connect(mExportToCellml10Action, &QAction::triggered,
            this, &CellMLToolsPlugin::exportToCellml10);

    connect(mExportToCAction, &QAction::triggered,
            this, &CellMLToolsPlugin::exportToLanguage);
    connect(mExportToFortran77Action, &QAction::triggered,
            this, &CellMLToolsPlugin::exportToLanguage);
    connect(mExportToMatlabAction, &QAction::triggered,
            this, &CellMLToolsPlugin::exportToLanguage);
    connect(mExportToPythonAction, &QAction::triggered,
            this, &CellMLToolsPlugin::exportToLanguage);
}

//==============================================================================

void CellMLToolsPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLToolsPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void CellMLToolsPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void CellMLToolsPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void CellMLToolsPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CellMLToolsPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the CellMLTools plugin:" << std::endl;
    std::cout << " * Display the commands supported by the CellMLTools plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Export <file> to a given <format> or a given <language>:" << std::endl;
    std::cout << "      export <file> <format>|<language>" << std::endl;
    std::cout << "   <format> can take one of the following values:" << std::endl;
    std::cout << "      cellml_1_0: to export a CellML 1.1 file to CellML 1.0" << std::endl;
    std::cout << "   <language> can take one of the following values:" << std::endl;
    std::cout << "      c: to export a CellML file to C" << std::endl;
    std::cout << "      fortran_77: to export a CellML file to FORTRAN 77" << std::endl;
    std::cout << "      matlab: to export a CellML file to MATLAB" << std::endl;
    std::cout << "      python: to export a CellML file to Python" << std::endl;
    std::cout << " * Validate <file>:" << std::endl;
    std::cout << "      validate <file>" << std::endl;
}

//==============================================================================

bool CellMLToolsPlugin::runCommand(Command pCommand,
                                   const QStringList &pArguments)
{
    // Make sure that we have the correct number of arguments

    if (   ((pCommand == Command::Export)   && (pArguments.count() != 2))
        || ((pCommand == Command::Validate) && (pArguments.count() != 1))) {
        runHelpCommand();

        return false;
    }

    // Check whether we are dealing with a local or a remote file

    QString output;
    bool isLocalFile;
    QString fileNameOrUrl;

    Core::checkFileNameOrUrl(pArguments[0], isLocalFile, fileNameOrUrl);

    QString fileName = fileNameOrUrl;

    if (!isLocalFile) {
        // We are dealing with a remote file, so try to get a local copy of it

        QByteArray fileContents;

        if (Core::readFile(fileNameOrUrl, fileContents, &output)) {
            // We were able to retrieve the contents of the remote file, so save
            // it locally to a 'temporary' file

            fileName = Core::temporaryFileName();

            if (!Core::writeFile(fileName, fileContents)) {
                output = "The file could not be saved locally.";
            }
        } else {
            output = QString("The file could not be opened (%1).").arg(Core::formatMessage(output));
        }
    }

    // At this stage, we should have a real file (be it originally local or
    // remote), so carry on with the command to run

    bool fileExists = QFile::exists(fileName);
    bool validFile = false;

    if (output.isEmpty()) {
        // Before actually running the command, we need to check and do a few
        // things

        if (!fileExists) {
            output = "The file could not be found.";
        } else if (    (pCommand == Command::Export)
                   && !CellMLSupport::CellmlFileManager::instance()->isCellmlFile(fileName)) {
            output = "The file is not a CellML file.";
        } else {
            Core::FileManager *fileManagerInstance = Core::FileManager::instance();

            if (fileManagerInstance->manage(fileName,
                                            isLocalFile?
                                                Core::File::Type::Local:
                                                Core::File::Type::Remote,
                                            isLocalFile?
                                                QString():
                                                fileNameOrUrl) != Core::FileManager::Status::Added) {
                output = "The file could not be managed.";
            } else {
                auto cellmlFile = new CellMLSupport::CellmlFile(fileName);

                switch (pCommand) {
                case Command::Export:
                    if (!cellmlFile->load()) {
                        output = "The file could not be loaded.";
                    } else {
                        // Check the type of export the user wants and that it
                        // is compatible with the CellML version of our file

                        static const QString Cellml10 = "cellml_1_0";
                        static const QString C = "c";
                        static const QString Fortran77 = "fortran_77";
                        static const QString Matlab = "matlab";
                        static const QString Python = "python";
                        static const QStringList formatsAndLanguages = { Cellml10, C, Fortran77, Matlab, Python };

                        QString formatOrLanguage = pArguments[1];
                        CellMLSupport::CellmlFile::Version cellmlVersion = cellmlFile->version();

                        if (!formatsAndLanguages.contains(formatOrLanguage)) {
                            output = "The format or language is not valid.";
                        } else if (   (formatOrLanguage == Cellml10)
                                   && (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_1)) {
                            output = "The file must be a CellML 1.1 file.";
                        } else {
                            // Everything seems to be fine, so attempt the
                            // export itself

                            bool exportOk;

                            if (formatOrLanguage == Cellml10) {
                                exportOk = cellmlFile->exportTo({}, CellMLSupport::CellmlFile::Version::Cellml_1_0);
                            } else if (formatOrLanguage == C) {
                                exportOk = cellmlFile->exportTo({}, CellMLSupport::CellmlFile::Language::C);
                            } else if (formatOrLanguage == Fortran77) {
                                exportOk = cellmlFile->exportTo({}, CellMLSupport::CellmlFile::Language::Fortran77);
                            } else if (formatOrLanguage == Matlab) {
                                exportOk = cellmlFile->exportTo({}, CellMLSupport::CellmlFile::Language::Matlab);
                            } else {
                                exportOk = cellmlFile->exportTo({}, CellMLSupport::CellmlFile::Language::Python);
                            }

                            if (!exportOk) {
                                output = "The file could not be exported";

                                CellMLSupport::CellmlFileIssues cellmlFileIssues = cellmlFile->issues();

                                if (!cellmlFileIssues.isEmpty()) {
                                    output += " ("+cellmlFileIssues.first().message()+")";
                                    // Note: if there are CellML issues then
                                    //       there can be only one of them
                                    //       following a CellML export...
                                }

                                output += '.';
                            }
                        }
                    }

                    break;
                case Command::Validate:
                    // Validate our file and report all errors and warnings

                    validFile = cellmlFile->isValid();

                    const CellMLSupport::CellmlFileIssues cellmlFileIssues = cellmlFile->issues();

                    for (const auto &cellmlFileIssue : cellmlFileIssues) {
                        output += QString("%1[%2] [%3:%4] %5").arg(output.isEmpty()?QString():"\n",
                                                                   (cellmlFileIssue.type() == CellMLSupport::CellmlFileIssue::Type::Error)?"Error":"Warning")
                                                              .arg(cellmlFileIssue.line())
                                                              .arg(cellmlFileIssue.column())
                                                              .arg(cellmlFileIssue.formattedMessage());
                    }

                    break;
                }

                // We are done (whether the command was successful or not), so
                // delete our CellML file object and unmanage our file

                delete cellmlFile;

                fileManagerInstance->unmanage(fileName);
            }
        }
    }

    // Delete the temporary file, if any, i.e. we are dealing with a remote file
    // and it has a temporay file associated with it

    if (!isLocalFile && fileExists) {
        QFile::remove(fileName);
    }

    // Let the user know about any output we got and leave with the appropriate
    // command code

    if (!output.isEmpty()) {
        std::cout << output.toStdString() << std::endl;
    }

    return    ((pCommand == Command::Export) && output.isEmpty())
           || ((pCommand == Command::Validate) && validFile);
}

//==============================================================================

bool CellMLToolsPlugin::runExportCommand(const QStringList &pArguments)
{
    // Export an existing file to the console using a given format as the
    // destination format

    return runCommand(Command::Export, pArguments);
}

//==============================================================================

bool CellMLToolsPlugin::runValidateCommand(const QStringList &pArguments)
{
    // Validate an existing file

    return runCommand(Command::Validate, pArguments);
}

//==============================================================================

void CellMLToolsPlugin::exportToCellml10()
{
    // Ask for the name of the file that will contain the export

    static const QString Cellml_1_0 = "CellML 1.0";

    QStringList cellmlFilters = Core::filters(FileTypeInterfaces() << CellMLSupport::fileTypeInterface());
    QString firstCellmlFilter = cellmlFilters.first();
    QString fileName = Core::getSaveFileName(tr("Export CellML File To %1").arg(Cellml_1_0),
                                             Core::newFileName(mFileName, tr("Exported"), false),
                                             cellmlFilters, &firstCellmlFilter);

    if (fileName.isEmpty()) {
        return;
    }

    // Now that we have a file name, we can do the export itself

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);

    if (!cellmlFile->exportTo(fileName, CellMLSupport::CellmlFile::Version::Cellml_1_0)) {
        QString errorMessage;

        if (!cellmlFile->issues().isEmpty()) {
            CellMLSupport::CellmlFileIssues cellmlFileIssues = cellmlFile->issues();

            errorMessage = " ("+cellmlFileIssues.first().message()+")";
            // Note: if there are 'cellmlFile->issues()', then there can be only
            //       one of them following a CellML export...
        }

        Core::warningMessageBox(tr("Export CellML File To %1").arg(Cellml_1_0),
                                tr("<strong>%1</strong> could not be exported to <strong>%2</strong>%3.").arg(QDir::toNativeSeparators(fileName),
                                                                                                              Cellml_1_0,
                                                                                                              errorMessage));
    }
}

//==============================================================================

void CellMLToolsPlugin::exportToLanguage()
{
    // Ask for the name of the file that will contain the export

    auto action = qobject_cast<QAction *>(sender());
    QString languageString = (action == mExportToCAction)?
                                 "C":
                                 (action == mExportToFortran77Action)?
                                     "FORTRAN 77":
                                     (action == mExportToMatlabAction)?
                                         "MATLAB":
                                         "Python";
    QString fileExtension = (action == mExportToCAction)?
                                "c":
                                (action == mExportToFortran77Action)?
                                    "f":
                                    (action == mExportToMatlabAction)?
                                        "m":
                                        "py";
    QString fileName = Core::getSaveFileName(tr("Export CellML File To %1").arg(languageString),
                                             Core::newFileName(mFileName, fileExtension));

    if (fileName.isEmpty()) {
        return;
    }

    // Now that we have both a user-defined format file and output file, we can
    // do the export itself

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);
    CellMLSupport::CellmlFile::Language language = (action == mExportToCAction)?
                                                   CellMLSupport::CellmlFile::Language::C:
                                                   (action == mExportToFortran77Action)?
                                                       CellMLSupport::CellmlFile::Language::Fortran77:
                                                       (action == mExportToMatlabAction)?
                                                           CellMLSupport::CellmlFile::Language::Matlab:
                                                           CellMLSupport::CellmlFile::Language::Python;

    if (!cellmlFile->exportTo(fileName, language)) {
        CellMLSupport::CellmlFileIssues cellmlFileIssues = cellmlFile->issues();
        QString errorMessage;

        if (!cellmlFileIssues.isEmpty()) {
            errorMessage = " ("+cellmlFileIssues.first().message()+")";
            // Note: if there are CellML file issues, then there can be only one
            //       of them following a CellML export...
        }

        Core::warningMessageBox(tr("Export CellML File To %1").arg(languageString),
                                tr("<strong>%1</strong> could not be exported to <strong>%2</strong>%3.").arg(QDir::toNativeSeparators(fileName),
                                                                                                              languageString,
                                                                                                              errorMessage));
    }
}

//==============================================================================

} // namespace CellMLTools
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// CellMLTools plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "cellmltoolsplugin.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QTemporaryFile>

//==============================================================================

#include <iostream>

//==============================================================================

namespace OpenCOR {
namespace CellMLTools {

//==============================================================================

PLUGININFO_FUNC CellMLToolsPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access various <a href=\"http://www.cellml.org/\">CellML</a>-related tools."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder à divers outils en rapport avec <a href=\"http://www.cellml.org/\">CellML</a>."));

    return new PluginInfo(PluginInfo::Miscellaneous, true, true,
                          QStringList() << "CellMLSupport",
                          descriptions);
}

//==============================================================================

CellMLToolsPlugin::CellMLToolsPlugin() :
    mCellmlFileTypes(FileTypes()),
    mFileName(QString())
{
}

//==============================================================================
// CLI interface
//==============================================================================

int CellMLToolsPlugin::executeCommand(const QString &pCommand,
                                      const QStringList &pArguments)
{
    // Run the given CLI command

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return 0;
    } else if (!pCommand.compare("export")) {
        // Export a file from one format to another

        return runExportCommand(pArguments);
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return -1;
    }
}

//==============================================================================
// GUI interface
//==============================================================================

void CellMLToolsPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);

    // Enable/disable and show/hide our tools in case we are dealing with a
    // CellML-based view plugin

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mExportToCellml10Action->setEnabled(   cellmlFile && cellmlFile->model()
                                        && QString::fromStdWString(cellmlFile->model()->cellmlVersion()).compare(CellMLSupport::Cellml_1_0));
    mExportToCellml11Action->setEnabled(   cellmlFile && cellmlFile->model()
                                        && QString::fromStdWString(cellmlFile->model()->cellmlVersion()).compare(CellMLSupport::Cellml_1_1));
//---GRY--- DISABLED UNTIL WE ACTUALLY SUPPORT EXPORT TO CellML 1.1...
Core::showEnableAction(mExportToCellml11Action, false);

    mExportToUserDefinedFormatAction->setEnabled(cellmlFile && cellmlFile->model());

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

Gui::Menus CellMLToolsPlugin::guiMenus() const
{
    // We don't handle this interface...

    return Gui::Menus();
}

//==============================================================================

Gui::MenuActions CellMLToolsPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::Tools, mCellmlFileExportToMenu->menuAction())
                              << Gui::MenuAction(Gui::MenuAction::Tools);
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
    retranslateAction(mExportToCellml11Action, tr("CellML 1.1..."),
                      tr("Export the CellML file to CellML 1.1"));

    retranslateAction(mExportToUserDefinedFormatAction, tr("User-Defined Format..."),
                      tr("Export the CellML file to some user-defined format"));
}

//==============================================================================
// Plugin interface
//==============================================================================

void CellMLToolsPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Keep track of our main window

    mMainWindow = pMainWindow;

    // Create our Tools | Export To menu

    mCellmlFileExportToMenu = Core::newMenu("CellmlFileExportTo", pMainWindow);

    // Create our different Tools | Export To actions, and add them to our
    // Tools | Export To menu

    mExportToCellml10Action = new QAction(pMainWindow);
    mExportToCellml11Action = new QAction(pMainWindow);

    mExportToUserDefinedFormatAction = new QAction(pMainWindow);

    mCellmlFileExportToMenu->addAction(mExportToCellml10Action);
    mCellmlFileExportToMenu->addAction(mExportToCellml11Action);
    mCellmlFileExportToMenu->addSeparator();
    mCellmlFileExportToMenu->addAction(mExportToUserDefinedFormatAction);

    // Some connections to handle our different Tools | Export To actions

    connect(mExportToCellml10Action, SIGNAL(triggered(bool)),
            this, SLOT(exportToCellml10()));
    connect(mExportToCellml11Action, SIGNAL(triggered(bool)),
            this, SLOT(exportToCellml11()));

    connect(mExportToUserDefinedFormatAction, SIGNAL(triggered(bool)),
            this, SLOT(exportToUserDefinedFormat()));
}

//==============================================================================

void CellMLToolsPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLToolsPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    // Retrieve the file types supported by the CellMLSupport plugin

    foreach (Plugin *plugin, pLoadedPlugins) {
        FileTypeInterface *fileTypeInterface = qobject_cast<FileTypeInterface *>(plugin->instance());

        if (!plugin->name().compare("CellMLSupport") && fileTypeInterface) {
            // This is the CellMLSupport plugin and, as expected, it implements
            // our file type interface, so retrieve the file types it supports

            mCellmlFileTypes << fileTypeInterface->fileTypes();

            break;
        }
    }
}

//==============================================================================

void CellMLToolsPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CellMLToolsPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CellMLToolsPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CellMLToolsPlugin::exportTo(const CellMLSupport::CellmlFile::Version &pVersion)
{
    // Ask for the name of the file that will contain the export

    QString format = (pVersion == CellMLSupport::CellmlFile::Cellml_1_0)?
                         "CellML 1.0":
                         "CellML 1.1";
    QString fileTypes = QString();

    foreach (FileType *fileType, mCellmlFileTypes) {
        if (!fileTypes.isEmpty())
            fileTypes += ";;";

        fileTypes +=  fileType->description()
                     +" (*."+fileType->fileExtension()+")";
    }

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    QString fileName = Core::getSaveFileName(tr("Export CellML File To %1").arg(format),
                                             fileManagerInstance->isRemote(mFileName)?
                                                 fileManagerInstance->url(mFileName):
                                                 mFileName,
                                             fileTypes);

    if (fileName.isEmpty())
        return;

    // Now that we have a file name, we can do the export itself

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);

    if (!cellmlFile->exportTo(fileName, pVersion)) {
        CellMLSupport::CellmlFileIssues issues = cellmlFile->issues();
        QString errorMessage = QString();

        if (issues.count())
            errorMessage = " ("+issues.first().message()+")";
            // Note: if there are 'issues', then there can be only one of them
            //       following a CellML export...

        QMessageBox::warning(mMainWindow, tr("Export CellML File To %1").arg(format),
                             tr("<strong>%1</strong> could not be exported to <strong>%2</strong>%3.").arg(fileName, format, errorMessage));
    }
}

//==============================================================================

void CellMLToolsPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by CellMLTools:" << std::endl;
    std::cout << " * Display the commands supported by CellMLTools:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Export <in_file> to <out_file> using <predefined_format> as the destination format or <user_defined_format_file> as the file describing the destination format:" << std::endl;
    std::cout << "      export <in_file> <out_file> [<predefined_format>|<user_defined_format_file>]" << std::endl;
    std::cout << "   <predefined_format> can take one of the following values:" << std::endl;
    std::cout << "      cellml_1_0: to export a CellML 1.1 file to CellML 1.0" << std::endl;
}

//==============================================================================

int CellMLToolsPlugin::runExportCommand(const QStringList &pArguments)
{
    // Export an existing file to another file using a given format as the
    // destination format

    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 3) {
        runHelpCommand();

        return -1;
    }

    // Check whether we are dealing with a local or a remote input file

    QString errorMessage = QString();
    bool inIsLocalFile;
    QString inFileNameOrUrl;

    Core::checkFileNameOrUrl(pArguments.at(0), inIsLocalFile, inFileNameOrUrl);

    QString inFileName = inFileNameOrUrl;

    if (inIsLocalFile) {
        // We are dealing with a local file, so just update inFileName

        inFileName = inFileNameOrUrl;
    } else {
        // We are dealing with a remote input file, so try to get a local copy
        // of it

        QString fileContents;

        if (Core::readTextFromUrl(inFileNameOrUrl, fileContents, &errorMessage)) {
            // We were able to retrieve the contents of the remote file, so save
            // it locally to a 'temporary' file

            QTemporaryFile localFile(QDir::tempPath()+QDir::separator()+"XXXXXX.tmp");

            if (localFile.open()) {
                localFile.setAutoRemove(false);
                // Note: by default, a temporary file is to autoremove itself,
                //       but we clearly don't want that here...

                localFile.close();

                inFileName = localFile.fileName();

                if (!Core::writeTextToFile(inFileName, fileContents))
                    errorMessage = "The input file could not be saved locally.";
            } else {
                errorMessage = "The input file could not be saved locally.";
            }
        } else {
            errorMessage = QString("The input file could not be opened (%1).").arg(Core::formatErrorMessage(errorMessage));
        }
    }

    // At this stage, we should have a real input file (be it originally local
    // or remote), so carry on with the export

    if (errorMessage.isEmpty()) {
        // Before actually doing the export, we need to make sure that the input
        // file exists, that it is a valid CellML file, that it can be managed
        // and that it can be loaded

        if (!QFile::exists(inFileName)) {
            errorMessage = "The input file could not be found.";
        } else if (!CellMLSupport::isCellmlFile(inFileName)) {
            errorMessage = "The input file is not a CellML file.";
        } else {
            if (Core::FileManager::instance()->manage(inFileName,
                                                      inIsLocalFile?
                                                          Core::File::Local:
                                                          Core::File::Remote,
                                                      inIsLocalFile?
                                                          QString():
                                                          inFileNameOrUrl) != Core::FileManager::Added) {
                errorMessage = "The input file could not be managed.";
            } else {
                CellMLSupport::CellmlFile *inCellmlFile = new CellMLSupport::CellmlFile(inFileName);

                if (!inCellmlFile->load()) {
                    errorMessage = "A problem occurred while loading the input file.";
                } else {
                    // At this stage, everything is fine with the input file, so
                    // now we need to check the type of export the user wants

                    QString predefinedFormatOrUserDefinedFormatFileName = pArguments.at(2);
                    bool wantExportToUserDefinedFormat = predefinedFormatOrUserDefinedFormatFileName.compare("cellml_1_0");

                    // If we want to export to a CellML 1.0, then we need to
                    // make sure that the input file is not already in that
                    // format

                    if (    wantExportToUserDefinedFormat
                        && !QFile::exists(predefinedFormatOrUserDefinedFormatFileName)) {
                        errorMessage = "The user-defined format file could not be found.";
                    } else if (   !wantExportToUserDefinedFormat
                               && !QString::fromStdWString(inCellmlFile->model()->cellmlVersion()).compare(CellMLSupport::Cellml_1_0)) {
                        errorMessage = "The input file is already a CellML 1.0 file.";
                    } else {
                        // Everything seems to be fine, so attempt the export
                        // itself

                        if (   ( wantExportToUserDefinedFormat && !inCellmlFile->exportTo(pArguments.at(1), predefinedFormatOrUserDefinedFormatFileName))
                            || (!wantExportToUserDefinedFormat && !inCellmlFile->exportTo(pArguments.at(1), CellMLSupport::CellmlFile::Cellml_1_0))) {
                            errorMessage = "The input file could not be exported";

                            CellMLSupport::CellmlFileIssues issues = inCellmlFile->issues();

                            if (issues.count())
                                errorMessage += " ("+issues.first().message()+")";
                                // Note: if there are 'issues', then there can
                                //       be only one of them following a CellML
                                //       export...

                            errorMessage += ".";
                        }
                    }
                }

                // We are done (whether the export was successful or not), so
                // delete our input CellML file object

                delete inCellmlFile;
            }
        }
    }

    // Delete the temporary input file, if any, i.e. we are dealing with a
    // remote file and it has a temporay input file associated with it

    if (!inIsLocalFile && QFile::exists(inFileName))
        QFile::remove(inFileName);

    // Let the user know if something went wrong at some point and then leave

    if (errorMessage.isEmpty()) {
        return 0;
    } else {
        std::cout << errorMessage.toStdString() << std::endl;

        return -1;
    }
}

//==============================================================================

void CellMLToolsPlugin::exportToCellml10()
{
    // Export the current file to CellML 1.0

    exportTo(CellMLSupport::CellmlFile::Cellml_1_0);
}

//==============================================================================

void CellMLToolsPlugin::exportToCellml11()
{
    // Export the current file to CellML 1.1

    exportTo(CellMLSupport::CellmlFile::Cellml_1_1);
}

//==============================================================================

void CellMLToolsPlugin::exportToUserDefinedFormat()
{
    // Ask for the name of the file that contains the user-defined format

    QString userDefinedFormatFileName = Core::getOpenFileName(tr("Select User-Defined Format File"),
                                                              tr("User-Defined Format File")+" (*.xml)");

    if (userDefinedFormatFileName.isEmpty())
        return;

    // Ask for the name of the file that will contain the export

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    QString fileName = Core::getSaveFileName(tr("Export CellML File To User-Defined Format"),
                                             fileManagerInstance->isRemote(mFileName)?
                                                 fileManagerInstance->url(mFileName):
                                                 mFileName);

    if (fileName.isEmpty())
        return;

    // Now that we have both a user-defined format file and output file, we can
    // do the export itself

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);

    if (!cellmlFile->exportTo(fileName, userDefinedFormatFileName)) {
        CellMLSupport::CellmlFileIssues issues = cellmlFile->issues();
        QString errorMessage = QString();

        if (issues.count())
            errorMessage = " ("+issues.first().message()+")";
            // Note: if there are 'issues', then there can be only one of them
            //       following a CellML export...

        QMessageBox::warning(mMainWindow, tr("Export CellML File To User-Defined Format"),
                             tr("<strong>%1</strong> could not be exported to the user-defined format described in <strong>%2</strong>%3.").arg(fileName, userDefinedFormatFileName, errorMessage));
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

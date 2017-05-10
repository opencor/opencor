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
// CellML tools plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmltoolsplugin.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QApplication>
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
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access various <a href=\"http://www.cellml.org/\">CellML</a>-related tools."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder divers outils en rapport avec <a href=\"http://www.cellml.org/\">CellML</a>."));

    return new PluginInfo(PluginInfo::Tools, true, true,
                          QStringList() << "CellMLSupport",
                          descriptions);
}

//==============================================================================

CellMLToolsPlugin::CellMLToolsPlugin() :
    mCellmlFileTypeInterface(0),
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
    CellMLSupport::CellmlFile::Version cellmlVersion = cellmlFile?cellmlFile->version():CellMLSupport::CellmlFile::Unknown;

    mExportToCellml10Action->setEnabled(   cellmlFile && cellmlFile->model()
                                        && (cellmlVersion != CellMLSupport::CellmlFile::Unknown)
                                        && (cellmlVersion != CellMLSupport::CellmlFile::Cellml_1_0));
    mExportToCellml11Action->setEnabled(   cellmlFile && cellmlFile->model()
                                        && (cellmlVersion != CellMLSupport::CellmlFile::Unknown)
                                        && (cellmlVersion != CellMLSupport::CellmlFile::Cellml_1_1));
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

bool CellMLToolsPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLToolsPlugin::pluginInterfacesOk(const QString &pFileName,
                                           QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CellMLToolsPlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (!Core::mainWindow())
        return;

    // Create our Tools | Export To menu

    mCellmlFileExportToMenu = Core::newMenu("CellmlFileExportTo", Core::mainWindow());

    // Create our different Tools | Export To actions, and add them to our
    // Tools | Export To menu

    mExportToCellml10Action = Core::newAction(Core::mainWindow());
    mExportToCellml11Action = Core::newAction(Core::mainWindow());

    mExportToUserDefinedFormatAction = Core::newAction(Core::mainWindow());

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
    // Retrieve the file type interfaces supported by the CellMLSupport plugin

    foreach (Plugin *plugin, pLoadedPlugins) {
        FileTypeInterface *fileTypeInterface = qobject_cast<FileTypeInterface *>(plugin->instance());

        if (!plugin->name().compare("CellMLSupport") && fileTypeInterface) {
            // This is the CellMLSupport plugin and, as expected, it implements
            // our file type interface, so keep track of it

            mCellmlFileTypeInterface = fileTypeInterface;

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

void CellMLToolsPlugin::handleUrl(const QUrl &pUrl)
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
    QStringList cellmlFilters = Core::filters(FileTypeInterfaces() << mCellmlFileTypeInterface);
    QString firstCellmlFilter = cellmlFilters.first();
    QString fileName = Core::getSaveFileName(tr("Export CellML File To %1").arg(format),
                                             Core::newFileName(mFileName, tr("Exported"), false),
                                             cellmlFilters, &firstCellmlFilter);

    if (fileName.isEmpty())
        return;

    // Now that we have a file name, we can do the export itself

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);

    if (!cellmlFile->exportTo(fileName, pVersion, true)) {
        QString errorMessage = QString();

        if (cellmlFile->issues().count()) {
            errorMessage = " ("+cellmlFile->issues().first().message()+")";
            // Note: if there are 'cellmlFile->issues()', then there can be only
            //       one of them following a CellML export...
        }

        Core::warningMessageBox(tr("Export CellML File To %1").arg(format),
                                tr("<strong>%1</strong> could not be exported to <strong>%2</strong>%3.").arg(fileName, format, errorMessage));
    }
}

//==============================================================================

void CellMLToolsPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the CellMLTools plugin:" << std::endl;
    std::cout << " * Display the commands supported by the CellMLTools plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Export <file> to a <predefined_format> or a <user_defined_format_file>:" << std::endl;
    std::cout << "      export <file> <predefined_format>|<user_defined_format_file>" << std::endl;
    std::cout << "   <predefined_format> can take one of the following values:" << std::endl;
    std::cout << "      cellml_1_0: to export a CellML 1.1 file to CellML 1.0" << std::endl;
}

//==============================================================================

int CellMLToolsPlugin::runExportCommand(const QStringList &pArguments)
{
    // Export an existing file to the console using a given format as the
    // destination format

    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 2) {
        runHelpCommand();

        return -1;
    }

    // Check whether we are dealing with a local or a remote file

    QString errorMessage = QString();
    bool isLocalFile;
    QString fileNameOrUrl;

    Core::checkFileNameOrUrl(pArguments[0], isLocalFile, fileNameOrUrl);

    QString fileName = fileNameOrUrl;

    if (!isLocalFile) {
        // We are dealing with a remote file, so try to get a local copy of it

        QString fileContents;

        if (Core::readFileContentsFromUrl(fileNameOrUrl, fileContents, &errorMessage)) {
            // We were able to retrieve the contents of the remote file, so save
            // it locally to a 'temporary' file

            fileName = Core::temporaryFileName();

            if (!Core::writeFileContentsToFile(fileName, fileContents))
                errorMessage = "The file could not be saved locally.";
        } else {
            errorMessage = QString("The file could not be opened (%1).").arg(Core::formatMessage(errorMessage));
        }
    }

    // At this stage, we should have a real file (be it originally local or
    // remote), so carry on with the export

    if (errorMessage.isEmpty()) {
        // Before actually doing the export, we need to make sure that the file
        // exists, that it is a valid CellML file, that it can be managed and
        // that it can be loaded

        if (!QFile::exists(fileName)) {
            errorMessage = "The file could not be found.";
        } else if (!CellMLSupport::CellmlFileManager::instance()->isCellmlFile(fileName)) {
            errorMessage = "The file is not a CellML file.";
        } else {
            Core::FileManager *fileManagerInstance = Core::FileManager::instance();

            if (fileManagerInstance->manage(fileName,
                                            isLocalFile?
                                                Core::File::Local:
                                                Core::File::Remote,
                                            isLocalFile?
                                                QString():
                                                fileNameOrUrl) != Core::FileManager::Added) {
                errorMessage = "The file could not be managed.";
            } else {
                CellMLSupport::CellmlFile *cellmlFile = new CellMLSupport::CellmlFile(fileName);

                if (!cellmlFile->load()) {
                    errorMessage = "The file could not be loaded.";
                } else {
                    // At this stage, everything is fine with the file, so now
                    // we need to check the type of export the user wants

                    QString predefinedFormatOrUserDefinedFormatFileName = pArguments[1];
                    bool wantExportToUserDefinedFormat = predefinedFormatOrUserDefinedFormatFileName.compare("cellml_1_0");

                    // If we want to export to CellML 1.0, then we need to make
                    // sure that the file is not already in that format

                    if (    wantExportToUserDefinedFormat
                        && !QFile::exists(predefinedFormatOrUserDefinedFormatFileName)) {
                        errorMessage = "The user-defined format file could not be found.";
                    } else if (   !wantExportToUserDefinedFormat
                               && (cellmlFile->version() == CellMLSupport::CellmlFile::Cellml_1_0)) {
                        errorMessage = "The file is already a CellML 1.0 file.";
                    } else {
                        // Everything seems to be fine, so attempt the export
                        // itself

                        if (   ( wantExportToUserDefinedFormat && !cellmlFile->exportTo(QString(), predefinedFormatOrUserDefinedFormatFileName))
                            || (!wantExportToUserDefinedFormat && !cellmlFile->exportTo(QString(), CellMLSupport::CellmlFile::Cellml_1_0))) {
                            errorMessage = "The file could not be exported";

                            if (cellmlFile->issues().count()) {
                                errorMessage += " ("+cellmlFile->issues().first().message()+")";
                                // Note: if there are 'cellmlFile->issues()',
                                //       then there can be only one of them
                                //       following a CellML export...
                            }

                            errorMessage += '.';
                        }
                    }
                }

                // We are done (whether the export was successful or not), so
                // delete our CellML file object and unmanage our input file

                delete cellmlFile;

                fileManagerInstance->unmanage(fileName);
            }
        }
    }

    // Delete the temporary file, if any, i.e. we are dealing with a remote file
    // and it has a temporay file associated with it

    if (!isLocalFile && QFile::exists(fileName))
        QFile::remove(fileName);

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

    QString xmlFilter = tr("User-Defined Format File")+" (*.xml)";
    QString userDefinedFormatFileName = Core::getOpenFileName(tr("Select User-Defined Format File"),
                                                              QStringList() << xmlFilter,
                                                              &xmlFilter);

    if (userDefinedFormatFileName.isEmpty())
        return;

    // Ask for the name of the file that will contain the export

    QString fileName = Core::getSaveFileName(tr("Export CellML File To User-Defined Format"),
                                             Core::newFileName(mFileName, tr("Exported"), false));

    if (fileName.isEmpty())
        return;

    // Now that we have both a user-defined format file and output file, we can
    // do the export itself

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);

    if (!cellmlFile->exportTo(fileName, userDefinedFormatFileName, true)) {
        QString errorMessage = QString();

        if (cellmlFile->issues().count()) {
            errorMessage = " ("+cellmlFile->issues().first().message()+")";
            // Note: if there are 'cellmlFile->issues()', then there can be only
            //       one of them following a CellML export...
        }

        Core::warningMessageBox(tr("Export CellML File To User-Defined Format"),
                                tr("<strong>%1</strong> could not be exported to the user-defined format described in <strong>%2</strong>%3.").arg(fileName, userDefinedFormatFileName, errorMessage));
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

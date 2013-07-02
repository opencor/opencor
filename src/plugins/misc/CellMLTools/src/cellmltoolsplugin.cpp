//==============================================================================
// CellMLTools plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "cellmltoolsplugin.h"
#include "coreutils.h"

//==============================================================================

#include <QAction>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

//==============================================================================

namespace OpenCOR {
namespace CellMLTools {

//==============================================================================

PLUGININFO_FUNC CellMLToolsPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access various <a href=\"http://www.cellml.org/\">CellML</a>-related tools."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder à divers outils en rapport avec <a href=\"http://www.cellml.org/\">CellML</a>."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Miscellaneous,
                          true,
                          true,
                          QStringList() << "CellMLSupport",
                          descriptions);
}

//==============================================================================

void CellMLToolsPlugin::initialize()
{
    // Create our Tools | Export To menu

    mCellmlFileExportToMenu = newMenu(mMainWindow, "CellmlFileExportTo");

    // Create our different Tools | Export To actions, and add them to our
    // Tools | Export To menu

    mExportToCellml10Action = newAction(mMainWindow);
    mExportToCellml11Action = newAction(mMainWindow);

    mCellmlFileExportToMenu->addAction(mExportToCellml10Action);
    mCellmlFileExportToMenu->addAction(mExportToCellml11Action);

    // Some connections to handle our different Tools | Export To actions

    connect(mExportToCellml10Action, SIGNAL(triggered()),
            this, SLOT(exportToCellml10()));
    connect(mExportToCellml11Action, SIGNAL(triggered()),
            this, SLOT(exportToCellml11()));

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::Tools, mCellmlFileExportToMenu->menuAction());
    mGuiSettings->addMenuAction(GuiMenuActionSettings::Tools);
}

//==============================================================================

void CellMLToolsPlugin::initializationsDone(const Plugins &pLoadedPlugins)
{
    // Retrieve the file types supported by the CellMLSupport plugin

    mCellmlFileTypes = FileTypes();

    foreach (Plugin *loadedPlugin, pLoadedPlugins) {
        FileInterface *fileInterface = qobject_cast<FileInterface *>(loadedPlugin->instance());

        if (!loadedPlugin->name().compare("CellMLSupport") && fileInterface) {
            // This is the CellMLSupport plugin and, as expected, it implements
            // the file interface, so retrieve the file types it supports

            mCellmlFileTypes = fileInterface->fileTypes();

            break;
        }
    }
}

//==============================================================================

void CellMLToolsPlugin::runHelpCommand()
{
    std::cout << "Commands supported by CellMLTools:" << std::endl;
    std::cout << " * Display the commands supported by CellMLTools:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Export <in_file> to <out_file> using <format> as the destination format:" << std::endl;
    std::cout << "      export <in_file> <out_file> <format>" << std::endl;
    std::cout << "   <format> can take one of the following values:" << std::endl;
    std::cout << "      cellml_1_0: to export a CellML 1.1 file to CellML 1.0" << std::endl;
}

//==============================================================================

void CellMLToolsPlugin::runExportCommand(const QStringList &pArguments,
                                         int *pRes)
{
    // Export an existing file to another file using a given format as the
    // destination format

    // Make sure that we have three arguments and that the given format is
    // valid

    bool validArguments = true;

    if (pArguments.count() != 3) {
        validArguments = false;
    } else {
        QString format = pArguments.at(2);

        if (format.compare("cellml_1_0"))
            validArguments = false;
    }

    // Confirm that we have valid arguments

    if (!validArguments) {
        runHelpCommand();

        *pRes = -1;

        return;
    }

    // Make sure that the input file exists and that it is a valid non CellML
    // 1.0 file

    QString errorMessage = QString();
    QString inFileName = pArguments.at(0);
    CellMLSupport::CellmlFile *inCellmlFile = new CellMLSupport::CellmlFile(inFileName);

    if (!QFileInfo(inFileName).exists())
        errorMessage = "Sorry, but the input file could not be found.";
    else if (!CellMLSupport::isCellmlFile(inFileName))
        errorMessage = "Sorry, but the input file is not a CellML file.";
    else if (!inCellmlFile->load())
        errorMessage = "Sorry, but a problem occurred while loading the input file.";
    else if (!QString::fromStdWString(inCellmlFile->model()->cellmlVersion()).compare(CellMLSupport::Cellml_1_0))
        errorMessage = "Sorry, but the input file is already a CellML 1.0 file.";

    // Our input file is a valid non CellML 1.0 file, so we can export it to
    // CellML 1.0

    if (    errorMessage.isEmpty()
        && !inCellmlFile->exportTo(pArguments.at(1), CellMLSupport::CellmlFile::Cellml_1_0))
        errorMessage = "Sorry, but a problem occurred while exporting the input file.";

    delete inCellmlFile;

    // Check whether something wrong happened at some point

    if (!errorMessage.isEmpty()) {
        std::cout << qPrintable(errorMessage) << std::endl;

        *pRes = -1;
    }
}

//==============================================================================

void CellMLToolsPlugin::runCliCommand(const QString &pCommand,
                                      const QStringList &pArguments, int *pRes)
{
    // Run the given CLI command

    *pRes = 0;

    if (!pCommand.compare("help")) {
        // Display the commands we support

        runHelpCommand();
    } else if (!pCommand.compare("export")) {
        // Export a file from one format to another

        runExportCommand(pArguments, pRes);
    } else {
        // Not a CLI command that we can run, so...

        runHelpCommand();

        *pRes = -1;
    }
}

//==============================================================================

void CellMLToolsPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    // Enable/disable and show/hide our tools in case we are dealing with a
    // CellML-based view plugin

    bool toolsVisible = pViewPlugin?
                            pViewPlugin->info()->fullDependencies().contains("CellMLSupport"):
                            false;
    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mExportToCellml10Action->setEnabled(   toolsVisible && cellmlFile
                                        && QString::fromStdWString(cellmlFile->model()->cellmlVersion()).compare(CellMLSupport::Cellml_1_0));
    mExportToCellml10Action->setVisible(toolsVisible);

/*---GRY---
    mExportToCellml11Action->setEnabled(   toolsVisible && cellmlFile
                                        && QString::fromStdWString(cellmlFile->model()->cellmlVersion()).compare(CellMLSupport::Cellml_1_1));
    mExportToCellml11Action->setVisible(toolsVisible);
*/
//---GRY--- BEGIN
// THIS IS UNTIL EXPORTING TO CellML 1.1 IS FULLY SUPPORTED...
mExportToCellml11Action->setEnabled(false);
mExportToCellml11Action->setVisible(false);
//---GRY--- END

    bool menuEnabled = true;

    foreach (QAction *action, mCellmlFileExportToMenu->actions())
        if (!action->isEnabled()) {
            menuEnabled = false;

            break;
        }

    mCellmlFileExportToMenu->menuAction()->setEnabled(toolsVisible && menuEnabled);
    mCellmlFileExportToMenu->menuAction()->setVisible(toolsVisible);

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

void CellMLToolsPlugin::retranslateUi()
{
    // Retranslate our different Tools actions

    retranslateMenu(mCellmlFileExportToMenu, tr("CellML File Export To"));

    retranslateAction(mExportToCellml10Action, tr("CellML 1.0..."), tr("Export the CellML file to CellML 1.0"));
    retranslateAction(mExportToCellml11Action, tr("CellML 1.1..."), tr("Export the CellML file to CellML 1.1"));
}

//==============================================================================

void CellMLToolsPlugin::exportTo(const CellMLSupport::CellmlFile::Format &pFormat)
{
    // Ask for the name of the file which will contain the export

    QString format;
    QString fileTypes;

    switch (pFormat) {
    case CellMLSupport::CellmlFile::Cellml_1_1:
    default:   // CellMLSupport::CellmlFile::Cellml_1_0
        if (pFormat == CellMLSupport::CellmlFile::Cellml_1_0)
            format = "CellML 1.0";
        else
            format = "CellML 1.1";

        foreach (const FileType &fileType, mCellmlFileTypes) {
            if (!fileTypes.isEmpty())
                fileTypes += ";;";

            fileTypes +=  fileType.description()+" (*."+fileType.fileExtension()+")";
        }
    }

    QString fileName = Core::getSaveFileName(tr("CellML file export to %1").arg(format), mFileName, fileTypes);

    // Make sure that we have a file name or leave, if not

    if (fileName.isEmpty())
        return;

    // Now that we have a file name, we can do the eport itself

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);

    if (!cellmlFile->exportTo(fileName, pFormat)) {
        CellMLSupport::CellmlFileIssues issues = cellmlFile->issues();
        QString errorMessage = QString();

        if (issues.count())
            errorMessage = " ("+issues.first().message()+")";
            // Note: if there are 'issues', then there can be only one of them
            //       following a CellML export...

        QMessageBox::warning(mMainWindow, tr("CellML file export to %1").arg(format),
                             tr("Sorry, but <strong>%1</strong> could not be exported to <strong>%2</strong>%3.").arg(fileName, format, errorMessage));
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

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML Text view plugin
//==============================================================================

#include "cellmleditingviewwidget.h"
#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "cellmltextviewplugin.h"
#include "cellmltextviewwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"

//==============================================================================

#include <QFile>
#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

PLUGININFO_FUNC CellMLTextViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit <a href=\"http://www.cellml.org/\">CellML</a> files using the CellML Text format."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers <a href=\"http://www.cellml.org/\">CellML</a> à l'aide du format CellML Text."));

    return new PluginInfo(PluginInfo::Editing, true, true,
                          QStringList() << "CellMLEditingView",
                          descriptions);
}

//==============================================================================
// CellML editing view interface
//==============================================================================

void CellMLTextViewPlugin::reformat(const QString &pFileName) const
{
    // Reformat the contents of the given file's editor

    mViewWidget->reformat(pFileName);
}

//==============================================================================

bool CellMLTextViewPlugin::validCellml(const QString &pFileName,
                                       QString &pExtra) const
{
    // Validate the given file

//---GRY--- THE BELOW EXTRA INFORMATION SHOULD BE REMOVED ONCE WE USE libCellML
//          AND ONCE WE CAN TRULY DO CellML VALIDATION...
    pExtra = tr("the <a href=\"http://cellml-api.sourceforge.net/\">CellML validation service</a> cannot be used in this view, so only validation against the <a href=\"http://opencor.ws/user/plugins/editing/CellMLTextView.html#CellML Text format\">CellML Text format</a> was performed. For full CellML validation, you might want to use the Raw CellML view instead.");

    return mViewWidget->validate(pFileName);
}

//==============================================================================
// CLI interface
//==============================================================================

int CellMLTextViewPlugin::executeCommand(const QString &pCommand,
                                         const QStringList &pArguments)
{
    // Run the given CLI command

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return 0;
    } else if (!pCommand.compare("export")) {
        // Export a CellML file to our CellML Text view format

        return runExportCommand(pArguments);
    } else if (!pCommand.compare("import")) {
        // Import a CellML Text view file to CellML

        return runImportCommand(pArguments);
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return -1;
    }
}

//==============================================================================
// Editing interface
//==============================================================================

EditorWidget::EditorWidget * CellMLTextViewPlugin::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    return mViewWidget->editorWidget(pFileName);
}

//==============================================================================

bool CellMLTextViewPlugin::isEditorWidgetUseable(const QString &pFileName) const
{
    // Return whether the requested editor widget is useable

    return mViewWidget->isEditorWidgetUseable(pFileName);
}

//==============================================================================

bool CellMLTextViewPlugin::isEditorWidgetContentsModified(const QString &pFileName) const
{
    // Return whether the contents of the requested editor widget has been
    // modified

    return mViewWidget->isEditorWidgetContentsModified(pFileName);
}

//==============================================================================
// File handling interface
//==============================================================================

bool CellMLTextViewPlugin::isIndirectRemoteFile(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLTextViewPlugin::saveFile(const QString &pOldFileName,
                                    const QString &pNewFileName,
                                    bool &pNeedFeedback)
{
    // Save the given file

    return mViewWidget->saveFile(pOldFileName, pNewFileName, pNeedFeedback);
}

//==============================================================================

void CellMLTextViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::fileReloaded(const QString &pFileName,
                                        const bool &pFileChanged)
{
    // The given file has been reloaded, so let our view widget know about it

    if (pFileChanged)
        mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void CellMLTextViewPlugin::fileRenamed(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void CellMLTextViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLTextViewPlugin::retranslateUi()
{
    // Retranslate our view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool CellMLTextViewPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLTextViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                              QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CellMLTextViewPlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (!Core::mainWindow())
        return;

    // Create our CellML text view widget

    mViewWidget = new CellmlTextViewWidget(Core::mainWindow());

    mViewWidget->setObjectName("CellmlTextViewWidget");

    // Hide our CellML text view widget since it may not initially be shown in
    // our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void CellMLTextViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::loadSettings(QSettings *pSettings)
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (!Core::mainWindow())
        return;

    // Retrieve our CellML text view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellMLTextViewPlugin::saveSettings(QSettings *pSettings) const
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (!Core::mainWindow())
        return;

    // Keep track of our generic CellML text view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellMLTextViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode CellMLTextViewPlugin::viewMode() const
{
    // Return our mode

    return EditingMode;
}

//==============================================================================

QStringList CellMLTextViewPlugin::viewMimeTypes(const MimeTypeMode &pMimeTypeMode) const
{
    Q_UNUSED(pMimeTypeMode);

    // Return the MIME types we support

    return QStringList() << CellMLSupport::CellmlMimeType;
}

//==============================================================================

QString CellMLTextViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return CellMLSupport::CellmlFileExtension;
}

//==============================================================================

QWidget * CellMLTextViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // Update and return our CellML text view widget using the given CellML
    // file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void CellMLTextViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our CellML text view widget to finalise the given CellML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString CellMLTextViewPlugin::viewName() const
{
    // Return our CellML text view's name

    return tr("CellML Text");
}

//==============================================================================

QIcon CellMLTextViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    static const QIcon NoIcon = QIcon();

    return NoIcon;
}

//==============================================================================
// Plugin specific
//==============================================================================

int CellMLTextViewPlugin::importExport(const QStringList &pArguments,
                                       const bool &pImport)
{
    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 1) {
        runHelpCommand();

        return -1;
    }

    // Check whether we are dealing with a local or a remote file, and retrieve
    // its contents

    QString errorMessage = QString();
    bool isLocalFile;
    QString fileNameOrUrl;

    Core::checkFileNameOrUrl(pArguments[0], isLocalFile, fileNameOrUrl);

    QString fileContents;

    if (isLocalFile) {
        if (!QFile::exists(fileNameOrUrl))
            errorMessage = "The file could not be found.";
        else if (!Core::readFileContentsFromFile(fileNameOrUrl, fileContents))
            errorMessage = "The file could not be opened.";
    } else {
        if (!Core::readFileContentsFromUrl(fileNameOrUrl, fileContents, &errorMessage))
            errorMessage = QString("The file could not be opened (%1).").arg(Core::formatMessage(errorMessage));
    }

    // At this stage, we have the contents of the file, so we can do the
    // import/export

    if (errorMessage.isEmpty()) {
        if (pImport) {
            CellMLTextView::CellMLTextViewConverter converter;

            if (!converter.execute(fileContents)) {
                errorMessage = QString("The file could not be imported:\n [%1:%2] %3.").arg(QString::number(converter.errorLine()),
                                                                                            QString::number(converter.errorColumn()),
                                                                                            Core::formatMessage(converter.errorMessage(), false));
            } else {
                std::cout << converter.output().toUtf8().constData();
            }
        } else {
            CellMLTextView::CellmlTextViewParser parser;

            if (!parser.execute(fileContents, CellMLSupport::CellmlFile::Cellml_1_1)) {
                errorMessage = "The file could not be exported:";

                foreach (const CellmlTextViewParserMessage &message, parser.messages()) {
                    if (message.type() == CellmlTextViewParserMessage::Error) {
                        errorMessage += QString("\n [%1:%2] %3").arg(QString::number(message.line()),
                                                                     QString::number(message.column()),
                                                                     message.message());
                    }
                }
            } else {
                std::cout << Core::serialiseDomDocument(parser.domDocument()).constData();
            }
        }
    }

    // Let the user know if something went wrong at some point and then leave

    if (errorMessage.isEmpty()) {
        return 0;
    } else {
        std::cout << errorMessage.toStdString() << std::endl;

        return -1;
    }
}

//==============================================================================

void CellMLTextViewPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by CellMLTextView:" << std::endl;
    std::cout << " * Display the commands supported by CellMLTextView:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Export a CellML Text <file> to CellML:" << std::endl;
    std::cout << "      export <file>" << std::endl;
    std::cout << " * Import a CellML <file> to the CellML Text format:" << std::endl;
    std::cout << "      import <file>" << std::endl;
}

//==============================================================================

int CellMLTextViewPlugin::runImportCommand(const QStringList &pArguments)
{
    // Export an existing CellML Text file to CellML on the console

    return importExport(pArguments, true);
}

//==============================================================================

int CellMLTextViewPlugin::runExportCommand(const QStringList &pArguments)
{
    // Export an existing CellML file to CellML Text format on the console

    return importExport(pArguments, false);
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

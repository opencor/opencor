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
// CellML Text view plugin
//==============================================================================

#include "cellmlsupportplugin.h"
#include "cellmltextviewplugin.h"
#include "cellmltextviewwidget.h"
#include "coreguiutils.h"
#include "filemanager.h"

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

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to edit <a href="https://cellml.org/">CellML</a> files using the CellML Text format.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour éditer des fichiers <a href="https://cellml.org/">CellML</a> à l'aide du format CellML Text.)"));

    return new PluginInfo(PluginInfo::Category::Editing, true, true,
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

    return mViewWidget->validate(pFileName, pExtra);
}

//==============================================================================
// CLI interface
//==============================================================================

bool CellMLTextViewPlugin::executeCommand(const QString &pCommand,
                                          const QStringList &pArguments,
                                          int &pRes)
{
    Q_UNUSED(pRes)

    // Run the given CLI command

    static const QString Help = "help";
    static const QString Export = "export";
    static const QString Import = "import";

    if (pCommand == Help) {
        // Display the commands that we support

        runHelpCommand();

        return true;
    }

    if (pCommand == Export) {
        // Export a CellML file to our CellML Text view format

        return runExportCommand(pArguments);
    }

    if (pCommand == Import) {
        // Import a CellML Text view file to CellML

        return runImportCommand(pArguments);
    }

    // Not a CLI command that we support

    runHelpCommand();

    return false;
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

bool CellMLTextViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

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
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so let our view widget know about it

    mViewWidget->fileSaved(pFileName);
}

//==============================================================================

void CellMLTextViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

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
    Q_UNUSED(pFileName)

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
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CellMLTextViewPlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (Core::mainWindow() == nullptr) {
        return;
    }

    // Create our CellML Text view widget

    mViewWidget = new CellmlTextViewWidget(Core::mainWindow());

    mViewWidget->setObjectName("CellmlTextViewWidget");

    // Hide our CellML Text view widget since it may not initially be shown in
    // our central widget

    mViewWidget->hide();
}

//==============================================================================

void CellMLTextViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::loadSettings(QSettings &pSettings)
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (Core::mainWindow() == nullptr) {
        return;
    }

    // Retrieve our CellML Text view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CellMLTextViewPlugin::saveSettings(QSettings &pSettings) const
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (Core::mainWindow() == nullptr) {
        return;
    }

    // Keep track of our generic CellML Text view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CellMLTextViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode CellMLTextViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Mode::Editing;
}

//==============================================================================

QStringList CellMLTextViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return QStringList() << CellMLSupport::CellmlMimeType;
}

//==============================================================================

QString CellMLTextViewPlugin::viewMimeType(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // Return the MIME type for the given CellML file

    return CellMLSupport::CellmlMimeType;
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
    // Make sure that we are dealing with either a new file or a CellML 1.0/1.1
    // file

    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::fileVersion(pFileName);

    if (   !Core::FileManager::instance()->isNew(pFileName)
        &&  (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_0)
        &&  (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_1)) {
        return nullptr;
    }

    // Update and return our CellML Text view widget using the given CellML
    // file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void CellMLTextViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our CellML Text view widget to finalise the given CellML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString CellMLTextViewPlugin::viewName() const
{
    // Return our CellML Text view's name

    return tr("CellML Text");
}

//==============================================================================

QIcon CellMLTextViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    static const QIcon NoIcon = QIcon();

    return NoIcon;
}

//==============================================================================
// Plugin specific
//==============================================================================

bool CellMLTextViewPlugin::importExport(const QStringList &pArguments,
                                        bool pImport)
{
    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 1) {
        runHelpCommand();

        return false;
    }

    // Check whether we are dealing with a local or a remote file, and retrieve
    // its contents

    QByteArray fileContents;
    QString errorMessage = QString();

    if (!Core::readFile(pArguments[0], fileContents, &errorMessage)) {
        if (errorMessage.isEmpty()) {
            if (QFile::exists(pArguments[0])) {
                errorMessage = "The file could not be opened.";
            } else {
                errorMessage = "The file could not be found.";
            }
        } else {
            errorMessage = QString("The file could not be opened (%1).").arg(Core::formatMessage(errorMessage));
        }
    }

    // At this stage, we have the contents of the file, so we need to check that
    // it is a CellML 1.0/1.1 file if we want to import it

    if (errorMessage.isEmpty() && pImport) {
        CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::fileContentsVersion(fileContents);

        if (   (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_0)
            && (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_1)) {
            errorMessage = QString("Only CellML 1.0/1.1 files can be imported.");
        }
    }

    // We know that the file is a CellML 1.0/1.1 file, so we can do the
    // import/export

    if (errorMessage.isEmpty()) {
        if (pImport) {
            CellMLTextViewConverter converter;

            if (!converter.execute(fileContents)) {
                errorMessage = QString("The file could not be imported:\n [%1:%2] %3.").arg(converter.errorLine())
                                                                                       .arg(converter.errorColumn())
                                                                                       .arg(Core::formatMessage(converter.errorMessage(), false));
            } else {
                std::cout << converter.output().toUtf8().constData();
            }
        } else {
            CellmlTextViewParser parser;

            if (!parser.execute(fileContents, CellMLSupport::CellmlFile::Version::Cellml_1_1)) {
                errorMessage = "The file could not be exported:";

                for (const auto &message : parser.messages()) {
                    if (message.type() == CellmlTextViewParserMessage::Type::Error) {
                        errorMessage += QString("\n [%1:%2] %3").arg(message.line())
                                                                .arg(message.column())
                                                                .arg(message.message());
                    }
                }
            } else {
                std::cout << Core::serialiseDomDocument(parser.domDocument()).constData();
            }
        }
    }

    // Let the user know if something went wrong at some point and then leave

    if (errorMessage.isEmpty()) {
        return true;
    }

    std::cout << errorMessage.toStdString() << std::endl;

    return false;
}

//==============================================================================

void CellMLTextViewPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the CellMLTextView plugin:" << std::endl;
    std::cout << " * Display the commands supported by the CellMLTextView plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Export a CellML Text <file> to CellML:" << std::endl;
    std::cout << "      export <file>" << std::endl;
    std::cout << " * Import a CellML <file> to the CellML Text format:" << std::endl;
    std::cout << "      import <file>" << std::endl;
}

//==============================================================================

bool CellMLTextViewPlugin::runImportCommand(const QStringList &pArguments)
{
    // Export an existing CellML Text file to CellML on the console

    return importExport(pArguments, true);
}

//==============================================================================

bool CellMLTextViewPlugin::runExportCommand(const QStringList &pArguments)
{
    // Export an existing CellML file to CellML Text format on the console

    return importExport(pArguments, false);
}

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

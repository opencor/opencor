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
// CoreCellMLEditing plugin
//==============================================================================

#include "cellmleditinginterface.h"
#include "corecellmleditingplugin.h"
#include "coreguiutils.h"
#include "editinginterface.h"
#include "filemanager.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

//==============================================================================

namespace OpenCOR {
namespace CoreCellMLEditing {

//==============================================================================

PLUGININFO_FUNC CoreCellMLEditingPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core CellML editing plugin."));
    descriptions.insert("fr", QString::fromUtf8("l'extension d'édition CellML de base."));

    return new PluginInfo("Editing", false, false,
                          QStringList() << "CellMLSupport" << "CoreEditing" << "EditorList" << "MathMLViewer",
                          descriptions);
}

//==============================================================================

CoreCellMLEditingPlugin::CoreCellMLEditingPlugin() :
    mFileName(QString()),
    mCellmlEditingInterface(0)
{
}

//==============================================================================
// File handling interface
//==============================================================================

bool CoreCellMLEditingPlugin::isIndirectRemoteFile(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CoreCellMLEditingPlugin::saveFile(const QString &pOldFileName,
                                       const QString &pNewFileName,
                                       bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);
    Q_UNUSED(pNeedFeedback);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CoreCellMLEditingPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so show/enable or hide/disable our
    // reformat action, if needed

    if (!pFileName.compare(mFileName)) {
        Core::showEnableAction(mEditReformatAction, mCellmlEditingInterface,
                               !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName));
    }
}

//==============================================================================

void CoreCellMLEditingPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::fileReloaded(const QString &pFileName,
                                           const bool &pFileChanged)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pFileChanged);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::fileRenamed(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void CoreCellMLEditingPlugin::updateGui(Plugin *pViewPlugin,
                                        const QString &pFileName)
{
    // Show/enable or hide/disable various actions, depending on whether the
    // view plugin handles the CellML editing interface

    mCellmlEditingInterface = pViewPlugin?qobject_cast<CellmlEditingInterface *>(pViewPlugin->instance()):0;

    Core::showEnableAction(mEditReformatAction, mCellmlEditingInterface,
                           !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName));

    Core::showEnableAction(mToolsCellmlValidationAction, mCellmlEditingInterface, !pFileName.isEmpty());

    // Update our editor's context menu
    // Note: our editor's original context menu is set in
    //       CoreEditingPlugin::updateGui()...

    EditingInterface *editingInterface = pViewPlugin?qobject_cast<EditingInterface *>(pViewPlugin->instance()):0;

    if (editingInterface) {
        Editor::EditorWidget *editor = editingInterface->editor(pFileName);

        if (editor) {
            QList<QAction *> contextMenuActions = editor->contextMenu()->actions();

            QAction *separatorAction = Core::newAction(Core::mainWindow());

            separatorAction->setSeparator(true);

            contextMenuActions << separatorAction;
            contextMenuActions << mEditReformatAction;

            separatorAction = Core::newAction(Core::mainWindow());

            separatorAction->setSeparator(true);

            contextMenuActions << separatorAction;
            contextMenuActions << mToolsCellmlValidationAction;

            editor->setContextMenu(contextMenuActions);
        }
    }

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

Gui::Menus CoreCellMLEditingPlugin::guiMenus() const
{
    // We don't handle this interface...

    return Gui::Menus();
}

//==============================================================================

Gui::MenuActions CoreCellMLEditingPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::Tools, mEditReformatAction)
                              << Gui::MenuAction(Gui::MenuAction::Tools)
                              << Gui::MenuAction(Gui::MenuAction::Tools, mToolsCellmlValidationAction)
                              << Gui::MenuAction(Gui::MenuAction::Tools);
}

//==============================================================================
// I18n interface
//==============================================================================

void CoreCellMLEditingPlugin::retranslateUi()
{
    // Retranslate our different actions

    retranslateAction(mEditReformatAction, tr("Reformat"),
                      tr("Reformat the contents of the editor"));

    retranslateAction(mToolsCellmlValidationAction, tr("CellML Validation"),
                      tr("Validate the CellML file"));
}

//==============================================================================
// Plugin interface
//==============================================================================

void CoreCellMLEditingPlugin::initializePlugin()
{
    // Create our different actions

    mEditReformatAction = Core::newAction(QKeySequence(Qt::CTRL|Qt::Key_R),
                                          Core::mainWindow());
    mToolsCellmlValidationAction = Core::newAction(QKeySequence(Qt::CTRL|Qt::Key_T),
                                                   Core::mainWindow());

    // Some connections to handle our different actions

    connect(mEditReformatAction, SIGNAL(triggered(bool)),
            this, SLOT(reformat()));

    connect(mToolsCellmlValidationAction, SIGNAL(triggered(bool)),
            this, SLOT(cellmlValidation()));
}

//==============================================================================

void CoreCellMLEditingPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CoreCellMLEditingPlugin::reformat()
{
    // Reformat the contents of the editor

    if (mCellmlEditingInterface)
        mCellmlEditingInterface->reformat(mFileName);
}

//==============================================================================

void CoreCellMLEditingPlugin::cellmlValidation()
{
    // Validate the current CellML file

    if (mCellmlEditingInterface) {
        QString extra = QString();

        if (mCellmlEditingInterface->validCellml(mFileName, extra)) {
            // There are no CellML issues, so the CellML file is valid

            QMessageBox::information( Core::mainWindow(),
                                      tr("CellML Validation"),
                                      tr("The CellML file is valid.")
                                     +(extra.isEmpty()?
                                          QString():
                                          "<br/><br/>"+tr("<strong>Note:</strong> %1.").arg(Core::formatMessage(extra))));
        }
    }
}

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// CoreSEDMLEditing plugin
//==============================================================================

#include "coreguiutils.h"
#include "coresedmleditingplugin.h"
#include "editinginterface.h"
#include "filemanager.h"
#include "sedmleditinginterface.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

//==============================================================================

namespace OpenCOR {
namespace CoreSEDMLEditing {

//==============================================================================

PLUGININFO_FUNC CoreSEDMLEditingPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core SED-ML editing plugin."));
    descriptions.insert("fr", QString::fromUtf8("l'extension d'édition SED-ML de base."));

    return new PluginInfo("Editing", false, false,
                          QStringList() << "CoreEditing" << "SEDMLSupport" << "EditorList",
                          descriptions);
}

//==============================================================================

CoreSEDMLEditingPlugin::CoreSEDMLEditingPlugin() :
    mFileName(QString()),
    mSedmlEditingInterface(0)
{
}

//==============================================================================
// File handling interface
//==============================================================================

bool CoreSEDMLEditingPlugin::isIndirectRemoteFile(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CoreSEDMLEditingPlugin::saveFile(const QString &pOldFileName,
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

void CoreSEDMLEditingPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreSEDMLEditingPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so show/enable or hide/disable our
    // reformat action, if needed

    if (!pFileName.compare(mFileName)) {
        Core::showEnableAction(mEditReformatAction, mSedmlEditingInterface,
                               !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName));
    }
}

//==============================================================================

void CoreSEDMLEditingPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreSEDMLEditingPlugin::fileReloaded(const QString &pFileName,
                                          const bool &pFileChanged)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pFileChanged);

    // We don't handle this interface...
}

//==============================================================================

void CoreSEDMLEditingPlugin::fileRenamed(const QString &pOldFileName,
                                         const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreSEDMLEditingPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void CoreSEDMLEditingPlugin::updateGui(Plugin *pViewPlugin,
                                       const QString &pFileName)
{
    // Show/enable or hide/disable various actions, depending on whether the
    // view plugin handles the SED-ML editing interface

    mSedmlEditingInterface = pViewPlugin?qobject_cast<SedmlEditingInterface *>(pViewPlugin->instance()):0;

    Core::showEnableAction(mEditReformatAction, mSedmlEditingInterface,
                           !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName));

    Core::showEnableAction(mToolsSedmlValidationAction, mSedmlEditingInterface, !pFileName.isEmpty());

    // Update our editor's context menu
    // Note: our editor's original context menu is set in
    //       CoreEditingPlugin::updateGui()...

    EditingInterface *editingInterface = pViewPlugin?qobject_cast<EditingInterface *>(pViewPlugin->instance()):0;

    if (editingInterface) {
        Editor::EditorWidget *editor = editingInterface->editor(pFileName);

        if (editor) {
            QList<QAction *> contextMenuActions = editor->contextMenu()->actions();

            QAction *separatorAction = new QAction(Core::mainWindow());

            separatorAction->setSeparator(true);

            contextMenuActions << separatorAction;
            contextMenuActions << mEditReformatAction;

            separatorAction = new QAction(Core::mainWindow());

            separatorAction->setSeparator(true);

            contextMenuActions << separatorAction;
            contextMenuActions << mToolsSedmlValidationAction;

            editor->setContextMenu(contextMenuActions);
        }
    }

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

Gui::Menus CoreSEDMLEditingPlugin::guiMenus() const
{
    // We don't handle this interface...

    return Gui::Menus();
}

//==============================================================================

Gui::MenuActions CoreSEDMLEditingPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::Tools, mEditReformatAction)
                              << Gui::MenuAction(Gui::MenuAction::Tools)
                              << Gui::MenuAction(Gui::MenuAction::Tools, mToolsSedmlValidationAction)
                              << Gui::MenuAction(Gui::MenuAction::Tools);
}

//==============================================================================
// I18n interface
//==============================================================================

void CoreSEDMLEditingPlugin::retranslateUi()
{
    // Retranslate our different actions

    retranslateAction(mEditReformatAction, tr("Reformat"),
                      tr("Reformat the contents of the editor"));

    retranslateAction(mToolsSedmlValidationAction, tr("SED-ML Validation"),
                      tr("Validate the SED-ML file"));
}

//==============================================================================
// Plugin interface
//==============================================================================

void CoreSEDMLEditingPlugin::initializePlugin()
{
    // Create our different actions

    mEditReformatAction = new QAction(Core::mainWindow());

    mEditReformatAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_R));

    mToolsSedmlValidationAction = new QAction(Core::mainWindow());

    mToolsSedmlValidationAction->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_T));

    // Some connections to handle our different actions

    connect(mEditReformatAction, SIGNAL(triggered(bool)),
            this, SLOT(reformat()));

    connect(mToolsSedmlValidationAction, SIGNAL(triggered(bool)),
            this, SLOT(sedmlValidation()));
}

//==============================================================================

void CoreSEDMLEditingPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CoreSEDMLEditingPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CoreSEDMLEditingPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CoreSEDMLEditingPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CoreSEDMLEditingPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CoreSEDMLEditingPlugin::reformat()
{
    // Reformat the contents of the editor

    if (mSedmlEditingInterface)
        mSedmlEditingInterface->reformat(mFileName);
}

//==============================================================================

void CoreSEDMLEditingPlugin::sedmlValidation()
{
    // Validate the current SED-ML file

    if (mSedmlEditingInterface) {
        QString extra = QString();

        if (mSedmlEditingInterface->validSedml(mFileName, extra)) {
            // There are no SED-ML issues, so the SED-ML file is valid

            QMessageBox::information( Core::mainWindow(),
                                      tr("SED-ML Validation"),
                                      tr("The SED-ML file is valid.")
                                     +(extra.isEmpty()?
                                          QString():
                                          "<br/><br/>"+tr("<strong>Note:</strong> %1.").arg(Core::formatMessage(extra))),
                                      QMessageBox::Ok);
        }
    }
}

//==============================================================================

}   // namespace CoreSEDMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

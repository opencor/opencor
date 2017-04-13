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
// SED-ML Editing view plugin
//==============================================================================

#include "coreguiutils.h"
#include "sedmleditingviewplugin.h"
#include "editingviewinterface.h"
#include "filemanager.h"
#include "sedmleditingviewinterface.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>
#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace SEDMLEditingView {

//==============================================================================

PLUGININFO_FUNC SEDMLEditingViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides core <a href=\"http://www.sed-ml.org/\">SED-ML</a> editing view facilities."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit les fonctionalités de base d'une vue d'édition <a href=\"http://www.sed-ml.org/\">SED-ML</a>."));

    return new PluginInfo(PluginInfo::Editing, false, false,
                          QStringList() << "EditingView" << "SEDMLSupport",
                          descriptions);
}

//==============================================================================

SEDMLEditingViewPlugin::SEDMLEditingViewPlugin() :
    mFileName(QString()),
    mSedmlEditingViewInterface(0)
{
}

//==============================================================================
// File handling interface
//==============================================================================

bool SEDMLEditingViewPlugin::saveFile(const QString &pOldFileName,
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

void SEDMLEditingViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so show/enable or hide/disable our
    // reformat action, if needed

    if (!pFileName.compare(mFileName)) {
        Core::showEnableAction(mEditReformatAction, mSedmlEditingViewInterface,
                               !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName));
    }
}

//==============================================================================

void SEDMLEditingViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::fileReloaded(const QString &pFileName,
                                          const bool &pFileChanged)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pFileChanged);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::fileRenamed(const QString &pOldFileName,
                                         const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void SEDMLEditingViewPlugin::updateGui(Plugin *pViewPlugin,
                                       const QString &pFileName)
{
    // Show/enable or hide/disable various actions, depending on whether the
    // view plugin handles the SED-ML editing interface

    mSedmlEditingViewInterface = pViewPlugin?qobject_cast<SedmlEditingViewInterface *>(pViewPlugin->instance()):0;

    Core::showEnableAction(mEditReformatAction, mSedmlEditingViewInterface,
                           !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName));

    Core::showEnableAction(mToolsSedmlValidationAction, mSedmlEditingViewInterface, !pFileName.isEmpty());

    // Update our editor's context menu
    // Note: our editor's original context menu is set in
    //       EditingViewPlugin::updateGui()...

    EditingViewInterface *editingViewInterface = pViewPlugin?qobject_cast<EditingViewInterface *>(pViewPlugin->instance()):0;

    if (editingViewInterface) {
        EditorWidget::EditorWidget *editorWidget = editingViewInterface->editorWidget(pFileName);

        if (editorWidget) {
            QList<QAction *> contextMenuActions = editorWidget->contextMenu()->actions();

            QAction *separatorAction = Core::newAction(Core::mainWindow());

            separatorAction->setSeparator(true);

            contextMenuActions << separatorAction;
            contextMenuActions << mEditReformatAction;

            separatorAction = Core::newAction(Core::mainWindow());

            separatorAction->setSeparator(true);

            contextMenuActions << separatorAction;
            contextMenuActions << mToolsSedmlValidationAction;

            editorWidget->setContextMenu(contextMenuActions);
        }
    }

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

Gui::Menus SEDMLEditingViewPlugin::guiMenus() const
{
    // We don't handle this interface...

    return Gui::Menus();
}

//==============================================================================

Gui::MenuActions SEDMLEditingViewPlugin::guiMenuActions() const
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

void SEDMLEditingViewPlugin::retranslateUi()
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

bool SEDMLEditingViewPlugin::definesPluginInterfaces()
{
    // We define the SED-ML editing view interface

    return true;
}

//==============================================================================

bool SEDMLEditingViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                                QObject *pInstance)
{
    // Make sure that the given plugin instance uses the right version of the
    // SED-ML editing view interface, if it supports it

    return !(   qobject_cast<SedmlEditingViewInterface *>(pInstance)
             && (Plugin::interfaceVersion(pFileName, "sedmlEditingViewInterfaceVersion") != sedmlEditingViewInterfaceVersion()));
}

//==============================================================================

void SEDMLEditingViewPlugin::initializePlugin()
{
    // Create our different actions

    mEditReformatAction = Core::newAction(QKeySequence(Qt::CTRL|Qt::Key_R),
                                          Core::mainWindow());
    mToolsSedmlValidationAction = Core::newAction(QKeySequence(Qt::CTRL|Qt::Key_T),
                                                  Core::mainWindow());

    // Some connections to handle our different actions

    connect(mEditReformatAction, SIGNAL(triggered(bool)),
            this, SLOT(reformat()));

    connect(mToolsSedmlValidationAction, SIGNAL(triggered(bool)),
            this, SLOT(sedmlValidation()));
}

//==============================================================================

void SEDMLEditingViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void SEDMLEditingViewPlugin::reformat()
{
    // Reformat the contents of the editor

    if (mSedmlEditingViewInterface)
        mSedmlEditingViewInterface->reformat(mFileName);
}

//==============================================================================

void SEDMLEditingViewPlugin::sedmlValidation()
{
    // Validate the current SED-ML file

    if (mSedmlEditingViewInterface) {
        QString extra = QString();

        if (mSedmlEditingViewInterface->validSedml(mFileName, extra)) {
            // There are no SED-ML issues, so the SED-ML file is valid

            Core::informationMessageBox( tr("SED-ML Validation"),
                                         tr("The SED-ML file is valid.")
                                        +(extra.isEmpty()?
                                             QString():
                                             "<br/><br/>"+tr("<strong>Note:</strong> %1.").arg(Core::formatMessage(extra))));
        }
    }
}

//==============================================================================

}   // namespace SEDMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// CoreCellMLEditing plugin
//==============================================================================

#include "cellmleditinginterface.h"
#include "cellmlfilemanager.h"
#include "corecellmleditingplugin.h"
#include "coreguiutils.h"
#include "editinginterface.h"
#include "filemanager.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>
#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace CoreCellMLEditing {

//==============================================================================

PLUGININFO_FUNC CoreCellMLEditingPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core CellML editing plugin."));
    descriptions.insert("fr", QString::fromUtf8("l'extension d'édition CellML de base."));

    return new PluginInfo(PluginInfo::Editing, false, false,
                          QStringList() << "CoreEditing" << "CellMLSupport" << "EditorList" << "Viewer",
                          descriptions);
}

//==============================================================================

CoreCellMLEditingPlugin::CoreCellMLEditingPlugin() :
    mFileName(QString()),
    mCellmlEditingInterface(0)
{
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

    Core::showEnableAction(mFileNewCellml1_0FileAction, mCellmlEditingInterface);
    Core::showEnableAction(mFileNewCellml1_1FileAction, mCellmlEditingInterface);

    Core::showEnableAction(mToolsCellmlValidationAction, mCellmlEditingInterface, !pFileName.isEmpty());

    // Update our editor's context menu
    // Note: our editor's original context menu is set in
    //       CoreEditingPlugin::updateGui()...

    EditingInterface *editingInterface = pViewPlugin?qobject_cast<EditingInterface *>(pViewPlugin->instance()):0;

    if (editingInterface) {
        Editor::EditorWidget *editor = editingInterface->editor(pFileName);

        if (editor) {
            QList<QAction *> contextMenuActions = editor->contextMenu()->actions();

            QAction *separatorAction = new QAction(mMainWindow);

            separatorAction->setSeparator(true);

            contextMenuActions.append(separatorAction);
            contextMenuActions.append(mToolsCellmlValidationAction);

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

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::FileNew, mFileNewCellml1_0FileAction)
                              << Gui::MenuAction(Gui::MenuAction::FileNew, mFileNewCellml1_1FileAction)
                              << Gui::MenuAction(Gui::MenuAction::Tools, mToolsCellmlValidationAction)
                              << Gui::MenuAction(Gui::MenuAction::Tools);
}

//==============================================================================
// I18n interface
//==============================================================================

void CoreCellMLEditingPlugin::retranslateUi()
{
    // Retranslate our different actions

    retranslateAction(mFileNewCellml1_0FileAction, tr("CellML 1.0 File"),
                      tr("Create a new CellML 1.0 file"));
    retranslateAction(mFileNewCellml1_1FileAction, tr("CellML 1.1 File"),
                      tr("Create a new CellML 1.1 file"));

    retranslateAction(mToolsCellmlValidationAction, tr("CellML Validation"),
                      tr("Validate the CellML file"));
}

//==============================================================================
// Plugin interface
//==============================================================================

void CoreCellMLEditingPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Keep track of our main window

    mMainWindow = pMainWindow;

    // Create our different actions

    mFileNewCellml1_0FileAction = new QAction(pMainWindow);
    mFileNewCellml1_1FileAction = new QAction(pMainWindow);

    mToolsCellmlValidationAction = new QAction(pMainWindow);

    // Some connections to handle our different actions

    connect(mFileNewCellml1_0FileAction, SIGNAL(triggered(bool)),
            this, SLOT(newCellml1_0File()));
    connect(mFileNewCellml1_1FileAction, SIGNAL(triggered(bool)),
            this, SLOT(newCellml1_1File()));

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

void CoreCellMLEditingPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CoreCellMLEditingPlugin::newCellmlFile(const CellMLSupport::CellmlFile::Version &pVersion)
{
    // Determine some version-specific information

    QString version = QString();
    QString modelName = QString();

    switch (pVersion) {
    case CellMLSupport::CellmlFile::Cellml_1_1:
        version = "1.1";

        modelName = "new_cellml_1_1_model";

        break;
    default:   // CellMLSupport::CellmlFile::Cellml_1_0
        version = "1.0";

        modelName = "new_cellml_1_0_model";
    }

    // Ask our file manager to create a new file

    QString fileContents = "<?xml version=\"1.0\"?>\n"
                           "<model xmlns=\"http://www.cellml.org/cellml/%1#\" name=\"%2\">\n"
                           "    <!-- Your code goes here -->\n"
                           "</model>\n";
    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
#ifdef QT_DEBUG
    Core::FileManager::Status createStatus =
#endif
    fileManagerInstance->create(QString(), fileContents.arg(version, modelName));

#ifdef QT_DEBUG
    // Make sure that the file has indeed been created

    if (createStatus != Core::FileManager::Created)
        qFatal("FATAL ERROR | %s:%d: the file was not created", __FILE__, __LINE__);
#endif
}

//==============================================================================

void CoreCellMLEditingPlugin::newCellml1_0File()
{
    // Create a new CellML 1.0 file

    newCellmlFile(CellMLSupport::CellmlFile::Cellml_1_0);
}

//==============================================================================

void CoreCellMLEditingPlugin::newCellml1_1File()
{
    // Create a new CellML 1.1 file

    newCellmlFile(CellMLSupport::CellmlFile::Cellml_1_1);
}

//==============================================================================

void CoreCellMLEditingPlugin::cellmlValidation()
{
    // Validate the current CellML file

    if (mCellmlEditingInterface)
        mCellmlEditingInterface->validateCellml(mFileName);
}

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

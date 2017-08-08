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
// CellML Editing view plugin
//==============================================================================

#include "cellmleditingviewinterface.h"
#include "cellmleditingviewplugin.h"
#include "coreguiutils.h"
#include "editingviewinterface.h"
#include "filemanager.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>
#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace CellMLEditingView {

//==============================================================================

PLUGININFO_FUNC CellMLEditingViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides core <a href=\"http://www.cellml.org/\">CellML</a> editing view facilities."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit les fonctionalités de base d'une vue d'édition <a href=\"http://www.cellml.org/\">CellML</a>."));

    return new PluginInfo(PluginInfo::Editing, false, false,
                          QStringList() << "CellMLSupport" << "EditingView" << "MathMLViewerWidget",
                          descriptions);
}

//==============================================================================

CellMLEditingViewPlugin::CellMLEditingViewPlugin() :
    mFileName(QString()),
    mCellmlEditingViewInterface(0)
{
}

//==============================================================================
// File handling interface
//==============================================================================

bool CellMLEditingViewPlugin::saveFile(const QString &pOldFileName,
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

void CellMLEditingViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so show/enable or hide/disable our
    // reformat action, if needed

    if (!pFileName.compare(mFileName)) {
        Core::showEnableAction(mEditReformatAction, mCellmlEditingViewInterface,
                               !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName));
    }
}

//==============================================================================

void CellMLEditingViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::fileReloaded(const QString &pFileName,
                                           const bool &pFileChanged)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pFileChanged);

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::fileRenamed(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void CellMLEditingViewPlugin::updateGui(Plugin *pViewPlugin,
                                        const QString &pFileName)
{
    // Show/enable or hide/disable various actions, depending on whether the
    // view plugin handles the CellML editing interface

    mCellmlEditingViewInterface = pViewPlugin?qobject_cast<CellmlEditingViewInterface *>(pViewPlugin->instance()):0;

    Core::showEnableAction(mEditReformatAction, mCellmlEditingViewInterface,
                           !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName));

    Core::showEnableAction(mToolsCellmlValidationAction, mCellmlEditingViewInterface, !pFileName.isEmpty());

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
            contextMenuActions << mToolsCellmlValidationAction;

            editorWidget->setContextMenu(contextMenuActions);
        }
    }

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

Gui::Menus CellMLEditingViewPlugin::guiMenus() const
{
    // We don't handle this interface...

    return Gui::Menus();
}

//==============================================================================

Gui::MenuActions CellMLEditingViewPlugin::guiMenuActions() const
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

void CellMLEditingViewPlugin::retranslateUi()
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

bool CellMLEditingViewPlugin::definesPluginInterfaces()
{
    // We define the CellML editing view interface

    return true;
}

//==============================================================================

bool CellMLEditingViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                                 QObject *pInstance)
{
    // Make sure that the given plugin instance uses the right version of the
    // CellML editing view interface, if it supports it

    return !(   qobject_cast<CellmlEditingViewInterface *>(pInstance)
             && (Plugin::interfaceVersion(pFileName, "cellmlEditingViewInterfaceVersion") != cellmlEditingViewInterfaceVersion()));
}

//==============================================================================

void CellMLEditingViewPlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (!Core::mainWindow())
        return;

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

void CellMLEditingViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CellMLEditingViewPlugin::reformat()
{
    // Reformat the contents of the editor

    if (mCellmlEditingViewInterface)
        mCellmlEditingViewInterface->reformat(mFileName);
}

//==============================================================================

void CellMLEditingViewPlugin::cellmlValidation()
{
    // Validate the current CellML file

    if (mCellmlEditingViewInterface) {
        QString extra = QString();

        if (mCellmlEditingViewInterface->validCellml(mFileName, extra)) {
            // There are no CellML issues, so the CellML file is valid

            Core::informationMessageBox( tr("CellML Validation"),
                                         tr("The CellML file is valid.")
                                        +(extra.isEmpty()?
                                             QString():
                                             "<br/><br/>"+tr("<strong>Note:</strong> %1.").arg(Core::formatMessage(extra))));
        }
    }
}

//==============================================================================

}   // namespace CellMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

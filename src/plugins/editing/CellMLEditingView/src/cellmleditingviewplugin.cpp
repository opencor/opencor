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
#include "corecliutils.h"
#include "coreguiutils.h"
#include "editingviewinterface.h"
#include "editorwidget.h"
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

    descriptions.insert("en", QString::fromUtf8(R"(a plugin that provides core <a href="http://www.cellml.org/">CellML</a> editing view facilities.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension qui fournit les fonctionalités de base d'une vue d'édition <a href="http://www.cellml.org/">CellML</a>.)"));

    return new PluginInfo(PluginInfo::Category::Editing, false, false,
                          QStringList() << "CellMLSupport" << "EditingView" << "MathMLViewerWidget",
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool CellMLEditingViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLEditingViewPlugin::saveFile(const QString &pOldFileName,
                                       const QString &pNewFileName,
                                       bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)
    Q_UNUSED(pNeedFeedback)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CellMLEditingViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so show/enable or hide/disable our
    // reformat action, if needed

    if ((mEditor != nullptr) && (pFileName == mFileName)) {
        bool hasFileNameAndIsReadableAndWritable = !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName);

        Core::showEnableAction(mEditReformatAction, mCellmlEditingViewInterface != nullptr, hasFileNameAndIsReadableAndWritable);
        Core::showEnableAction(mEditReformatSeparator, mCellmlEditingViewInterface != nullptr, hasFileNameAndIsReadableAndWritable);
    }
}

//==============================================================================

void CellMLEditingViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::fileRenamed(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName)

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

    EditingViewInterface *editingViewInterface = (pViewPlugin != nullptr)?qobject_cast<EditingViewInterface *>(pViewPlugin->instance()):nullptr;

    mEditor = (editingViewInterface != nullptr)?editingViewInterface->editorWidget(pFileName):nullptr;
    mCellmlEditingViewInterface = (pViewPlugin != nullptr)?qobject_cast<CellmlEditingViewInterface *>(pViewPlugin->instance()):nullptr;

    bool hasFileName = !pFileName.isEmpty();
    bool hasFileNameAndIsReadWritable = hasFileName && Core::FileManager::instance()->isReadableAndWritable(pFileName);

    Core::showEnableAction(mEditReformatAction, mCellmlEditingViewInterface != nullptr, (mEditor != nullptr) && hasFileNameAndIsReadWritable);
    Core::showEnableAction(mEditReformatSeparator, mCellmlEditingViewInterface != nullptr, hasFileNameAndIsReadWritable);

    Core::showEnableAction(mToolsCellmlValidationAction, mCellmlEditingViewInterface != nullptr, (mEditor != nullptr) && hasFileName);
    Core::showEnableAction(mToolsCellmlValidationSeparator, mCellmlEditingViewInterface != nullptr, hasFileName);

    // Update our editor's context menu
    // Note: our editor's original context menu is set in
    //       EditingViewPlugin::updateGui()...

    if (mEditor != nullptr) {
        mEditor->setContextMenu(mEditor->contextMenu()->actions() << mEditReformatSeparator
                                                                  << mEditReformatAction
                                                                  << mToolsCellmlValidationSeparator
                                                                  << mToolsCellmlValidationAction);
    }

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

Gui::Menus CellMLEditingViewPlugin::guiMenus() const
{
    // We don't handle this interface...

    return {};
}

//==============================================================================

Gui::MenuActions CellMLEditingViewPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::Type::Tools, mEditReformatAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::Tools, mEditReformatSeparator)
                              << Gui::MenuAction(Gui::MenuAction::Type::Tools, mToolsCellmlValidationAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::Tools, mToolsCellmlValidationSeparator);
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

    return !(   (qobject_cast<CellmlEditingViewInterface *>(pInstance) != nullptr)
             && (Plugin::interfaceVersion(pFileName, "cellmlEditingViewInterfaceVersion") != cellmlEditingViewInterfaceVersion()));
}

//==============================================================================

void CellMLEditingViewPlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (Core::mainWindow() == nullptr) {
        return;
    }

    // Create our different actions

    mEditReformatAction = Core::newAction(QKeySequence(Qt::ControlModifier|Qt::Key_R),
                                          Core::mainWindow());
    mEditReformatSeparator = Core::newSeparator(Core::mainWindow());

    mToolsCellmlValidationAction = Core::newAction(QKeySequence(Qt::ControlModifier|Qt::Key_T),
                                                   Core::mainWindow());
    mToolsCellmlValidationSeparator = Core::newSeparator(Core::mainWindow());

    // Some connections to handle our different actions

    connect(mEditReformatAction, &QAction::triggered,
            this, &CellMLEditingViewPlugin::reformat);

    connect(mToolsCellmlValidationAction, &QAction::triggered,
            this, &CellMLEditingViewPlugin::cellmlValidation);
}

//==============================================================================

void CellMLEditingViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void CellMLEditingViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CellMLEditingViewPlugin::reformat()
{
    // Reformat the contents of the editor

    if (mCellmlEditingViewInterface != nullptr) {
        mCellmlEditingViewInterface->reformat(mFileName);
    }
}

//==============================================================================

void CellMLEditingViewPlugin::cellmlValidation()
{
    // Validate the current CellML file

    if (mCellmlEditingViewInterface != nullptr) {
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

} // namespace CellMLEditingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

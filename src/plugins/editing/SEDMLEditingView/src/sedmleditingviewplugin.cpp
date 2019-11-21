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
// SED-ML Editing view plugin
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "editingviewinterface.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "sedmleditingviewinterface.h"
#include "sedmleditingviewplugin.h"

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

    descriptions.insert("en", QString::fromUtf8(R"(a plugin that provides core <a href="http://www.sed-ml.org/">SED-ML</a> editing view facilities.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension qui fournit les fonctionalités de base d'une vue d'édition <a href="http://www.sed-ml.org/">SED-ML</a>.)"));

    return new PluginInfo(PluginInfo::Category::Editing, false, false,
                          QStringList() << "EditingView" << "SEDMLSupport",
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool SEDMLEditingViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool SEDMLEditingViewPlugin::saveFile(const QString &pOldFileName,
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

void SEDMLEditingViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so show/enable or hide/disable our
    // reformat action, if needed

    if ((mEditor != nullptr) && (pFileName == mFileName)) {
        bool hasFileNameAndIsReadableAndWritable = !pFileName.isEmpty() && Core::FileManager::instance()->isReadableAndWritable(pFileName);

        Core::showEnableAction(mEditReformatAction, mSedmlEditingViewInterface != nullptr, hasFileNameAndIsReadableAndWritable);
        Core::showEnableAction(mEditReformatSeparator, mSedmlEditingViewInterface != nullptr, hasFileNameAndIsReadableAndWritable);
    }
}

//==============================================================================

void SEDMLEditingViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::fileRenamed(const QString &pOldFileName,
                                         const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName)

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

    EditingViewInterface *editingViewInterface = (pViewPlugin != nullptr)?qobject_cast<EditingViewInterface *>(pViewPlugin->instance()):nullptr;

    mEditor = (editingViewInterface != nullptr)?editingViewInterface->editorWidget(pFileName):nullptr;
    mSedmlEditingViewInterface = (pViewPlugin != nullptr)?qobject_cast<SedmlEditingViewInterface *>(pViewPlugin->instance()):nullptr;

    bool hasFileName = !pFileName.isEmpty();
    bool hasFileNameAndIsReadWritable = hasFileName && Core::FileManager::instance()->isReadableAndWritable(pFileName);

    Core::showEnableAction(mEditReformatAction, mSedmlEditingViewInterface != nullptr, (mEditor != nullptr) && hasFileNameAndIsReadWritable);
    Core::showEnableAction(mEditReformatSeparator, mSedmlEditingViewInterface != nullptr, hasFileNameAndIsReadWritable);

    Core::showEnableAction(mToolsSedmlValidationAction, mSedmlEditingViewInterface != nullptr, (mEditor != nullptr) && hasFileName);
    Core::showEnableAction(mToolsSedmlValidationSeparator, mSedmlEditingViewInterface != nullptr, hasFileName);

    // Update our editor's context menu
    // Note: our editor's original context menu is set in
    //       EditingViewPlugin::updateGui()...

    if (mEditor != nullptr) {
        mEditor->setContextMenu(mEditor->contextMenu()->actions() << mEditReformatSeparator
                                                                  << mEditReformatAction
                                                                  << mToolsSedmlValidationSeparator
                                                                  << mToolsSedmlValidationAction);
    }

    // Keep track of the file name

    mFileName = pFileName;
}

//==============================================================================

Gui::Menus SEDMLEditingViewPlugin::guiMenus() const
{
    // We don't handle this interface...

    return {};
}

//==============================================================================

Gui::MenuActions SEDMLEditingViewPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::Type::Tools, mEditReformatAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::Tools, mEditReformatSeparator)
                              << Gui::MenuAction(Gui::MenuAction::Type::Tools, mToolsSedmlValidationAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::Tools, mToolsSedmlValidationSeparator);
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

    return !(   (qobject_cast<SedmlEditingViewInterface *>(pInstance) != nullptr)
             && (Plugin::interfaceVersion(pFileName, "sedmlEditingViewInterfaceVersion") != sedmlEditingViewInterfaceVersion()));
}

//==============================================================================

void SEDMLEditingViewPlugin::initializePlugin()
{
    // Create our different actions

    mEditReformatAction = Core::newAction(QKeySequence(Qt::ControlModifier|Qt::Key_R),
                                          Core::mainWindow());
    mEditReformatSeparator = Core::newSeparator(Core::mainWindow());

    mToolsSedmlValidationAction = Core::newAction(QKeySequence(Qt::ControlModifier|Qt::Key_T),
                                                  Core::mainWindow());
    mToolsSedmlValidationSeparator = Core::newSeparator(Core::mainWindow());

    // Some connections to handle our different actions

    connect(mEditReformatAction, &QAction::triggered,
            this, &SEDMLEditingViewPlugin::reformat);

    connect(mToolsSedmlValidationAction, &QAction::triggered,
            this, &SEDMLEditingViewPlugin::sedmlValidation);
}

//==============================================================================

void SEDMLEditingViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void SEDMLEditingViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void SEDMLEditingViewPlugin::reformat()
{
    // Reformat the contents of the editor

    if (mSedmlEditingViewInterface != nullptr) {
        mSedmlEditingViewInterface->reformat(mFileName);
    }
}

//==============================================================================

void SEDMLEditingViewPlugin::sedmlValidation()
{
    // Validate the current SED-ML file

    if (mSedmlEditingViewInterface != nullptr) {
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

} // namespace SEDMLEditingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
#include "corecellmleditingplugin.h"
#include "guiutils.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace CoreCellMLEditing {

//==============================================================================

PLUGININFO_FUNC CoreCellMLEditingPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core CellML editing plugin."));
    descriptions.insert("fr", QString::fromUtf8("l'extension d'édition CellML de base."));

    return new PluginInfo(PluginInfo::Editing, false,
                          QStringList() << "CoreEditing" << "CellMLSupport",
                          descriptions);
}

//==============================================================================
// Core interface
//==============================================================================

void CoreCellMLEditingPlugin::initialize()
{
    // Create our different File|New actions

    mFileNewCellml1_0FileAction = newAction(mMainWindow);
    mFileNewCellml1_1FileAction = newAction(mMainWindow);

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewCellml1_0FileAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewCellml1_1FileAction);
}

//==============================================================================

void CoreCellMLEditingPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::initialized(const Plugins &pLoadedPlugins)
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

void CoreCellMLEditingPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void CoreCellMLEditingPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::updateGui(Plugin *pViewPlugin,
                                        const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // Show/enable or hide/disable various actions, depending on whether the
    // view plugin handles the CellML editing interface

    CellmlEditingInterface *cellmlEditingInterface = qobject_cast<CellmlEditingInterface *>(pViewPlugin->instance());

    Core::showEnableAction(mFileNewCellml1_0FileAction, cellmlEditingInterface);
    Core::showEnableAction(mFileNewCellml1_1FileAction, cellmlEditingInterface);
}

//==============================================================================

void CoreCellMLEditingPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool CoreCellMLEditingPlugin::hasViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

QWidget * CoreCellMLEditingPlugin::viewWidget(const QString &pFileName,
                                              const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void CoreCellMLEditingPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString CoreCellMLEditingPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon CoreCellMLEditingPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool CoreCellMLEditingPlugin::saveFile(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

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

void CoreCellMLEditingPlugin::fileLocked(const QString &pFileName,
                                         const bool &pLocked)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pLocked);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::fileModified(const QString &pFileName,
                                           const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreCellMLEditingPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

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

bool CoreCellMLEditingPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void CoreCellMLEditingPlugin::retranslateUi()
{
    // Retranslate our different File|New actions

    retranslateAction(mFileNewCellml1_0FileAction, tr("CellML 1.0 File"),
                      tr("Create a new CellML 1.0 file"));
    retranslateAction(mFileNewCellml1_1FileAction, tr("CellML 1.1 File"),
                      tr("Create a new CellML 1.1 file"));
}

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

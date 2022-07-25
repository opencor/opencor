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
// BondGraphEditor window plugin
//==============================================================================

#include "coreguiutils.h"
#include "bondgrapheditorwindowplugin.h"
#include "bondgrapheditorwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace BondGraphEditorWindow {

//==============================================================================

PLUGININFO_FUNC BondGraphEditorWindowPluginInfo()
{
    static const Descriptions descriptions = {
                                                 { "en", QString::fromUtf8("a plugin that provides a BondGraph window.") },
                                                 { "fr", QString::fromUtf8("une extension qui fournit une BondGraph.") }
                                             };

    return new PluginInfo(PluginInfo::Category::Miscellaneous, true, false,
                          { "Core", "BondGraphAPI" },
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void BondGraphEditorWindowPlugin::retranslateUi()
{
    // Retranslate our Web Browser window action

    retranslateAction(mBondGraphEditorWindowAction,
                      tr("Graphical BondGraph Editor"),
                      tr("Show/hide the Graphical BondGraph Editor window"));

    // Retranslate our Web Browser window

    mBondGraphEditorWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool BondGraphEditorWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool BondGraphEditorWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                            QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void BondGraphEditorWindowPlugin::initializePlugin()
{

    // Create an action to show/hide our Web Browser window

    mBondGraphEditorWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our BondGraphEditorWindow

    mBondGraphEditorWindow = new BondGraphEditorWindow(Core::mainWindow());
}

//==============================================================================

void BondGraphEditorWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void BondGraphEditorWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void BondGraphEditorWindowPlugin::loadSettings(QSettings &pSettings)
{
   mBondGraphEditorWindow->loadSettings(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void BondGraphEditorWindowPlugin::saveSettings(QSettings &pSettings) const
{
    mBondGraphEditorWindow->saveSettings(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void BondGraphEditorWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea BondGraphEditorWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::RightDockWidgetArea;
}

//==============================================================================

QAction * BondGraphEditorWindowPlugin::windowAction() const
{
    // Return our window action

    return mBondGraphEditorWindowAction;
}

//==============================================================================

QDockWidget * BondGraphEditorWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mBondGraphEditorWindow;
}

//==============================================================================

} // namespace BondGraphEditorWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

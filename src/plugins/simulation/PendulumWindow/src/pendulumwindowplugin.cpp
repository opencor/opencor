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
// Pendulum window plugin
//==============================================================================

#include "coreguiutils.h"
#include "pendulumwindowplugin.h"
#include "pendulumwindowwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace PendulumWindow {

//==============================================================================

PLUGININFO_FUNC PendulumWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to test the pendulum model."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour tester le modèle du pendule."));

    return new PluginInfo(PluginInfo::Simulation, false, false,
                          QStringList() << "ZincWidget",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void PendulumWindowPlugin::retranslateUi()
{
    // Retranslate our Pendulum window action

    retranslateAction(mPendulumWindowAction,
                      tr("Pendulum"),
                      tr("Show/hide the Pendulum window"));

    // Retranslate our Pendulum window

    mPendulumWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PendulumWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PendulumWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                              QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PendulumWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our web browser window

    mPendulumWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our web browser window

    mPendulumWindowWindow = new PendulumWindowWindow(Core::mainWindow());
}

//==============================================================================

void PendulumWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PendulumWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PendulumWindowPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PendulumWindowPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PendulumWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PendulumWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::RightDockWidgetArea;
}

//==============================================================================

QAction * PendulumWindowPlugin::windowAction() const
{
    // Return our window action

    return mPendulumWindowAction;
}

//==============================================================================

QDockWidget * PendulumWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mPendulumWindowWindow;
}

//==============================================================================

}   // namespace PendulumWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

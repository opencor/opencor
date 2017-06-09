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
// An dockable iPython console window for Qt
//==============================================================================

#include "coreguiutils.h"
#include "ctkAbstractPythonManager.h"
#include "pythonqtconsolewindowplugin.h"
#include "pythonqtconsolewindow.h"
#include "pythonqtsupportplugin.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace PythonQtConsoleWindow {

//==============================================================================

PLUGININFO_FUNC PythonQtConsoleWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the Python Qt Console window plugin."));
    descriptions.insert("fr", QString::fromUtf8("the Python Qt Console window plugin."));

    return new PluginInfo(PluginInfo::Miscellaneous, true, false,
                          QStringList() << "Core" << "PythonQtSupport" << "PythonWrapper",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void PythonQtConsoleWindowPlugin::retranslateUi()
{
  // Retranslate our window action
  retranslateAction(mPythonQtConsoleWindowAction,
                    tr("PythonQtConsole"), tr("Show/hide the Python Qt Console window"));
  }

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonQtConsoleWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonQtConsoleWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                                     QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonQtConsoleWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our PythonQt window

    mPythonQtConsoleWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our PythonQt window

    mPythonQtConsoleWindow = new PythonQtConsoleWindow(Core::mainWindow());
}

//==============================================================================

void PythonQtConsoleWindowPlugin::finalizePlugin()
{
    // Delete our PythonQt window

    delete mPythonQtConsoleWindow;
}

//==============================================================================

void PythonQtConsoleWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtConsoleWindowPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtConsoleWindowPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtConsoleWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PythonQtConsoleWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::RightDockWidgetArea;
}

//==============================================================================

QAction * PythonQtConsoleWindowPlugin::windowAction() const
{
    // Return our window action

    return mPythonQtConsoleWindowAction;
}

//==============================================================================

QDockWidget * PythonQtConsoleWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mPythonQtConsoleWindow;
}

//==============================================================================

}   // namespace PythonQtConsoleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

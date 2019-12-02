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
// Python Console window plugin
//==============================================================================

#include "coreguiutils.h"
#include "pythonconsolewindow.h"
#include "pythonconsolewindowplugin.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace PythonConsoleWindow {

//==============================================================================

PLUGININFO_FUNC PythonConsoleWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to interact with <a href=\"https://python.org/\">Python</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour interagir avec <a href=\"https://python.org/\">Python</a>."));

    return new PluginInfo(PluginInfo::Category::Miscellaneous, true, false,
                          QStringList() << "Core" << "PythonQtSupport",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void PythonConsoleWindowPlugin::retranslateUi()
{
    // Retranslate our Python Console window action

    retranslateAction(mPythonConsoleWindowAction,
                      tr("Python Console"),
                      tr("Show/hide the Python Console window"));

    // Retranslate our Python Console window

    mPythonConsoleWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonConsoleWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonConsoleWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                                   QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonConsoleWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our Python console window

    mPythonConsoleWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our Python Console window

    mPythonConsoleWindowWindow = new PythonConsoleWindow(Core::mainWindow());
}

//==============================================================================

void PythonConsoleWindowPlugin::finalizePlugin()
{
    // Delete our Python Console window

    delete mPythonConsoleWindowWindow;
}

//==============================================================================

void PythonConsoleWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void PythonConsoleWindowPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void PythonConsoleWindowPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void PythonConsoleWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PythonConsoleWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::RightDockWidgetArea;
}

//==============================================================================

QAction * PythonConsoleWindowPlugin::windowAction() const
{
    // Return our Python Console window action

    return mPythonConsoleWindowAction;
}

//==============================================================================

QDockWidget * PythonConsoleWindowPlugin::windowWidget() const
{
    // Return our Python Console window widget

    return mPythonConsoleWindowWindow;
}

//==============================================================================

} // namespace PythonConsoleWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

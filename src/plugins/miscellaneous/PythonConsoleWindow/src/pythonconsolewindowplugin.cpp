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
// Python Console Window plugin
//==============================================================================

#include "coreguiutils.h"
#include "pythonconsolewindowplugin.h"
#include "pythonconsolewindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace PythonConsoleWindow {

//==============================================================================

PLUGININFO_FUNC PythonConsoleWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the Python Console window plugin."));
    descriptions.insert("fr", QString::fromUtf8("the Python Console window plugin."));

    return new PluginInfo(PluginInfo::Miscellaneous, true, false,
                          QStringList() << "Core" << "PythonQtSupport" << "PythonWrapper",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void PythonConsoleWindowPlugin::retranslateUi()
{
  // Retranslate our window action
  retranslateAction(mPythonConsoleWindowAction,
                    tr("Python"), tr("Show/hide the Python Console window"));
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
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonConsoleWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our Python window

    mPythonConsoleWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our Python window

    mPythonConsoleWindow = new PythonConsoleWindow(Core::mainWindow());


//    matplotlib.use('PythonQt')
}

//==============================================================================

void PythonConsoleWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonConsoleWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

static const auto SettingsCommandHistory = QStringLiteral("CommandHistory");

//==============================================================================

void PythonConsoleWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our Python window settings

    pSettings->beginGroup(mPythonConsoleWindow->objectName());
        auto commandHistory = pSettings->value(SettingsCommandHistory, QVariant(QStringList() << "")).toStringList();
        mPythonConsoleWindow->setCommandHistory(commandHistory);
    pSettings->endGroup();
}

//==============================================================================

void PythonConsoleWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our Python window settings

    pSettings->beginGroup(mPythonConsoleWindow->objectName());
        pSettings->setValue(SettingsCommandHistory, QVariant(mPythonConsoleWindow->getCommandHistory()));
    pSettings->endGroup();
}

//==============================================================================

void PythonConsoleWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PythonConsoleWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::TopDockWidgetArea;
}

//==============================================================================

QAction * PythonConsoleWindowPlugin::windowAction() const
{
    // Return our window action

    return mPythonConsoleWindowAction;
}

//==============================================================================

QDockWidget * PythonConsoleWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mPythonConsoleWindow;
}

//==============================================================================

}   // namespace PythonConsoleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

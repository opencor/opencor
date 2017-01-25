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
// Python Console Window plugin
//==============================================================================

#include "coreguiutils.h"
#include "pythonconsolewindowplugin.h"
#include "pythonconsolewindow.h"

//==============================================================================

//#include "guiutils.h"

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
                          QStringList() << "Core" << "PythonQtSupport",
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

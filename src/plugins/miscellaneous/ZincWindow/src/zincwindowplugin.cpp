/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Zinc window plugin
//==============================================================================

#include "coreguiutils.h"
#include "zincwindowplugin.h"
#include "zincwindowwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace ZincWindow {

//==============================================================================

PLUGININFO_FUNC ZincWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to Zinc."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour Zinc."));

    return new PluginInfo(PluginInfo::Miscellaneous, true, false,
                          QStringList() << "ZincWidget",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void ZincWindowPlugin::retranslateUi()
{
    // Retranslate our Zinc window action

    retranslateAction(mZincWindowAction,
                      tr("Zinc"),
                      tr("Show/hide the Zinc window"));

    // Retranslate our Zinc window

    mZincWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool ZincWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool ZincWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                          QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void ZincWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our web browser window

    mZincWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our web browser window

    mZincWindowWindow = new ZincWindowWindow(Core::mainWindow());
}

//==============================================================================

void ZincWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void ZincWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void ZincWindowPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void ZincWindowPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void ZincWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea ZincWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::RightDockWidgetArea;
}

//==============================================================================

QAction * ZincWindowPlugin::windowAction() const
{
    // Return our window action

    return mZincWindowAction;
}

//==============================================================================

QDockWidget * ZincWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mZincWindowWindow;
}

//==============================================================================

}   // namespace ZincWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// Web Browser window plugin
//==============================================================================

#include "coreguiutils.h"
#include "webbrowserwindowplugin.h"
#include "webbrowserwindowwindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

PLUGININFO_FUNC WebBrowserWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to browse the Web."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour naviguer sur le Web."));

    return new PluginInfo(PluginInfo::Miscellaneous, true, false,
                          QStringList() << "Core" << "WebViewerWidget",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void WebBrowserWindowPlugin::retranslateUi()
{
    // Retranslate our web browser window action

    retranslateAction(mWebBrowserWindowAction,
                      tr("Web Browser"),
                      tr("Show/hide the Web Browser window"));

    // Retranslate our web browser window

    mWebBrowserWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void WebBrowserWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our web browser window

    mWebBrowserWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our web browser window

    mWebBrowserWindowWindow = new WebBrowserWindowWindow(Core::mainWindow());
}

//==============================================================================

void WebBrowserWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void WebBrowserWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void WebBrowserWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our Web browser window settings

    pSettings->beginGroup(mWebBrowserWindowWindow->objectName());
        mWebBrowserWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void WebBrowserWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our Web browser window settings

    pSettings->beginGroup(mWebBrowserWindowWindow->objectName());
        mWebBrowserWindowWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void WebBrowserWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea WebBrowserWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::RightDockWidgetArea;
}

//==============================================================================

QAction * WebBrowserWindowPlugin::windowAction() const
{
    // Return our window action

    return mWebBrowserWindowAction;
}

//==============================================================================

QDockWidget * WebBrowserWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mWebBrowserWindowWindow;
}

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

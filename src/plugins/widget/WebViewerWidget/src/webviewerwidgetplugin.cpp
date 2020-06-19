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
// Web Viewer widget plugin
//==============================================================================

#include "webviewerwidgetplugin.h"

//==============================================================================

#include <QApplication>
#include <QWindow>

//==============================================================================

namespace OpenCOR {
namespace WebViewerWidget {

//==============================================================================

PLUGININFO_FUNC WebViewerWidgetPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to visualise Web documents."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour visualiser des documents Web."));

    return new PluginInfo(PluginInfo::Category::Widget, false, false,
                          { "Core" },
                          descriptions);
}

//==============================================================================
// Plugin interface
//==============================================================================

bool WebViewerWidgetPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool WebViewerWidgetPlugin::pluginInterfacesOk(const QString &pFileName,
                                               QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void WebViewerWidgetPlugin::initializePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void WebViewerWidgetPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void WebViewerWidgetPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void WebViewerWidgetPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void WebViewerWidgetPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // Delete any Web inspector window, if any

    static const QString QWebInspectorClassWindow = "QWebInspectorClassWindow";

    for (auto window : QGuiApplication::topLevelWindows()) {
        if (window->objectName() == QWebInspectorClassWindow) {
            window->close();
        }
    }
}

//==============================================================================

void WebViewerWidgetPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================

} // namespace WebViewerWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

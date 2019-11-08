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

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to test the <a href="https://github.com/OpenCMISS/zinc">Zinc</a> library.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour tester la librairie <a href="https://github.com/OpenCMISS/zinc">Zinc</a>.)"));

    return new PluginInfo(PluginInfo::Category::Test, true, false,
                          QStringList() << "ZincWidget",
#if !defined(QT_DEBUG) || !defined(Q_OS_WIN)
                          descriptions,
                          QStringList() << "PythonConsoleWindow");
#else
                          descriptions);
#endif
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
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void ZincWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our Zinc window

    mZincWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our Zinc window

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
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void ZincWindowPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void ZincWindowPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void ZincWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

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

} // namespace ZincWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

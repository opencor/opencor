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
// Help window plugin
//==============================================================================

#include "coreguiutils.h"
#include "helpwindowplugin.h"
#include "helpwindowwindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

PLUGININFO_FUNC HelpWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to provide help."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour fournir de l'aide."));

    return new PluginInfo(PluginInfo::Miscellaneous, true, false,
                          QStringList() << "Core" << "WebViewerWidget",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void HelpWindowPlugin::retranslateUi()
{
    // Retranslate our help action

    retranslateAction(mHelpAction, tr("Help"),
                      tr("Show/hide the OpenCOR help"));

    // Retranslate our Help window

    mHelpWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool HelpWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool HelpWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                          QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void HelpWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our Help window

    mHelpAction = Core::newAction(true, QIcon(":/oxygen/apps/help-browser.png"),
                                  Qt::Key_F1, Core::mainWindow());

    // Create our Help window

    mHelpWindow = new HelpWindowWindow(Core::mainWindow());
}

//==============================================================================

void HelpWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our Help window settings

    pSettings->beginGroup(mHelpWindow->objectName());
        mHelpWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our Help window settings

    pSettings->beginGroup(mHelpWindow->objectName());
        mHelpWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea HelpWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::RightDockWidgetArea;
}

//==============================================================================

QAction * HelpWindowPlugin::windowAction() const
{
    // Return our Help window action

    return mHelpAction;
}

//==============================================================================

QDockWidget * HelpWindowPlugin::windowWidget() const
{
    // Return our Help window widget

    return mHelpWindow;
}

//==============================================================================

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// File Browser window plugin
//==============================================================================

#include "coreguiutils.h"
#include "filebrowserwindowplugin.h"
#include "filebrowserwindowwindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileBrowserWindow {

//==============================================================================

PLUGININFO_FUNC FileBrowserWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access local files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder aux fichiers locaux."));

    return new PluginInfo(PluginInfo::Organisation, true, false,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void FileBrowserWindowPlugin::retranslateUi()
{
    // Retranslate our File Browser window action

    retranslateAction(mFileBrowserWindowAction, tr("File Browser"),
                      tr("Show/hide the File Browser window"));

    // Retranslate our File Browser window

    mFileBrowserWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool FileBrowserWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool FileBrowserWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                                 QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void FileBrowserWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our File Browser window

    mFileBrowserWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our File Browser window

    mFileBrowserWindowWindow = new FileBrowserWindowWindow(Core::mainWindow());
}

//==============================================================================

void FileBrowserWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our File Browser window settings

    pSettings->beginGroup(mFileBrowserWindowWindow->objectName());
        mFileBrowserWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileBrowserWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our File Browser window settings

    pSettings->beginGroup(mFileBrowserWindowWindow->objectName());
        mFileBrowserWindowWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileBrowserWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea FileBrowserWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::LeftDockWidgetArea;
}

//==============================================================================

QAction * FileBrowserWindowPlugin::windowAction() const
{
    // Return our File Browser window action

    return mFileBrowserWindowAction;
}

//==============================================================================

QDockWidget * FileBrowserWindowPlugin::windowWidget() const
{
    // Return our File Browser window widget

    return mFileBrowserWindowWindow;
}

//==============================================================================

}   // namespace FileBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// File Organiser window plugin
//==============================================================================

#include "coreguiutils.h"
#include "fileorganiserwindowplugin.h"
#include "fileorganiserwindowwindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

//==============================================================================

PLUGININFO_FUNC FileOrganiserWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to virtually organise files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour organiser virtuellement des fichiers."));

    return new PluginInfo(PluginInfo::Organisation, true, false,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void FileOrganiserWindowPlugin::retranslateUi()
{
    // Retranslate our File Organiser window action

    retranslateAction(mFileOrganiserWindowAction, tr("File Organiser"),
                      tr("Show/hide the File Organiser window"));

    // Retranslate our File Organiser window

    mFileOrganiserWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool FileOrganiserWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool FileOrganiserWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                                   QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void FileOrganiserWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our File Organiser window

    mFileOrganiserWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our File Organiser window

    mFileOrganiserWindowWindow = new FileOrganiserWindowWindow(Core::mainWindow());
}

//==============================================================================

void FileOrganiserWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our File Organiser window settings

    pSettings->beginGroup(mFileOrganiserWindowWindow->objectName());
        mFileOrganiserWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our File Organiser window settings

    pSettings->beginGroup(mFileOrganiserWindowWindow->objectName());
        mFileOrganiserWindowWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea FileOrganiserWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::LeftDockWidgetArea;
}

//==============================================================================

QAction * FileOrganiserWindowPlugin::windowAction() const
{
    // Return our File Organiser window action

    return mFileOrganiserWindowAction;
}

//==============================================================================

QDockWidget * FileOrganiserWindowPlugin::windowWidget() const
{
    // Return our File Organiser window widget

    return mFileOrganiserWindowWindow;
}

//==============================================================================

}   // namespace FileOrganiserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

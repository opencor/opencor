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
// PMR Workspaces window plugin
//==============================================================================

#include "coreguiutils.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrworkspaceswindowplugin.h"
#include "pmrworkspaceswindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PLUGININFO_FUNC PMRWorkspacesWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to manage a user's <a href=\"https://models.physiomeproject.org/\">PMR</a> workspaces."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder les espaces de travail <a href=\"https://models.physiomeproject.org/\">PMR</a> d'un utilisateur."));

    return new PluginInfo(PluginInfo::Organisation, true, false,
                          QStringList() << "CellMLSupport" << "LibXDiff" << "PMRSupport",
                          descriptions,
                          QStringList() << "FileBrowserWindow" << "FileOrganiserWindow");
}

//==============================================================================
// I18n interface
//==============================================================================

void PMRWorkspacesWindowPlugin::retranslateUi()
{
    // Retranslate our PMR Workspaces window action

    retranslateAction(mPmrWorkspacesWindowAction, tr("PMR Workspaces"),
                      tr("Show/hide the PMR Workspaces window"));

    // Retranslate our PMR Workspaces window

    mPmrWorkspacesWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PMRWorkspacesWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PMRWorkspacesWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                                   QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PMRWorkspacesWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our PMR Workspaces window

    mPmrWorkspacesWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our PMR Workspaces window

    mPmrWorkspacesWindowWindow = new PmrWorkspacesWindowWindow(Core::mainWindow());
}

//==============================================================================

void PMRWorkspacesWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PMRWorkspacesWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PMRWorkspacesWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our PMR Workspaces window settings

    pSettings->beginGroup(mPmrWorkspacesWindowWindow->objectName());
        mPmrWorkspacesWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWorkspacesWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our PMR Workspaces window settings

    pSettings->beginGroup(mPmrWorkspacesWindowWindow->objectName());
        mPmrWorkspacesWindowWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWorkspacesWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Preferences interface
//==============================================================================

Preferences::PreferencesWidget * PMRWorkspacesWindowPlugin::preferencesWidget()
{
    // We don't handle this interface...

    return 0;
}

//==============================================================================

void PMRWorkspacesWindowPlugin::preferencesChanged(const QStringList &pPluginNames)
{
    // Check whether it's the PMR Support preferences that have changed and, if
    // so, then update ourselves accordingly

    if (pPluginNames.contains(PMRSupport::PluginName)) {
        mPmrWorkspacesWindowWindow->update(PreferencesInterface::preference(PMRSupport::PluginName,
                                                                            PMRSupport::SettingsPreferencesPmrUrl,
                                                                            PMRSupport::SettingsPreferencesPmrUrlDefault).toString());
    }
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PMRWorkspacesWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::LeftDockWidgetArea;
}

//==============================================================================

QAction * PMRWorkspacesWindowPlugin::windowAction() const
{
    // Return our PMR Workspaces window action

    return mPmrWorkspacesWindowAction;
}

//==============================================================================

QDockWidget * PMRWorkspacesWindowPlugin::windowWidget() const
{
    // Return our PMR Workspaces window widget

    return mPmrWorkspacesWindowWindow;
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

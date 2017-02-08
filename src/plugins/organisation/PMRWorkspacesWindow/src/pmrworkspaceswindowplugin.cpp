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
                          QStringList() << "PMRSupport",
                          descriptions,
                          QStringList() << "FileBrowserWindow" << "FileOrganiserWindow");
}

//==============================================================================
// I18n interface
//==============================================================================

void PMRWorkspacesWindowPlugin::retranslateUi()
{
    // Retranslate our workspaces window action

    retranslateAction(mPmrWorkspacesWindowAction, tr("PMR Workspaces"),
                      tr("Show/hide the PMR Workspaces window"));

    // Retranslate our workspaces window

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
    // Create an action to show/hide our workspaces window

    mPmrWorkspacesWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our workspaces window

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
    // Retrieve our workspaces window settings

    pSettings->beginGroup(mPmrWorkspacesWindowWindow->objectName());
        mPmrWorkspacesWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWorkspacesWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our workspaces window settings

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

    if (pPluginNames.contains(PMRSupport::PluginName))
        mPmrWorkspacesWindowWindow->update(PreferencesInterface::preference(PMRSupport::PluginName, PMRSupport::SettingsPreferencesPmrUrl).toString());
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
    // Return our window action

    return mPmrWorkspacesWindowAction;
}

//==============================================================================

QDockWidget * PMRWorkspacesWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mPmrWorkspacesWindowWindow;
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

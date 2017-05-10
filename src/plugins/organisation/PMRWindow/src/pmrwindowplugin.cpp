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
// PMR window plugin
//==============================================================================

#include "coreguiutils.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrwindowplugin.h"
#include "pmrwindowwindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PMRWindow {

//==============================================================================

PLUGININFO_FUNC PMRWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access <a href=\"https://models.physiomeproject.org/\">PMR</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder <a href=\"https://models.physiomeproject.org/\">PMR</a>."));

    return new PluginInfo(PluginInfo::Organisation, true, false,
                          QStringList() << "PMRSupport",
                          descriptions,
                          QStringList() << "FileBrowserWindow" << "FileOrganiserWindow");
}

//==============================================================================
// I18n interface
//==============================================================================

void PMRWindowPlugin::retranslateUi()
{
    // Retranslate our PMR window action

    retranslateAction(mPmrWindowAction, tr("PMR"),
                      tr("Show/hide the PMR window"));

    // Retranslate our PMR window

    mPmrWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PMRWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PMRWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                         QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PMRWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our PMR window

    mPmrWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our PMR window

    mPmrWindowWindow = new PmrWindowWindow(Core::mainWindow());
}

//==============================================================================

void PMRWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PMRWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PMRWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our PMR window settings

    pSettings->beginGroup(mPmrWindowWindow->objectName());
        mPmrWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our PMR window settings

    pSettings->beginGroup(mPmrWindowWindow->objectName());
        mPmrWindowWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Preferences interface
//==============================================================================

Preferences::PreferencesWidget * PMRWindowPlugin::preferencesWidget()
{
    // We don't handle this interface...

    return 0;
}

//==============================================================================

void PMRWindowPlugin::preferencesChanged(const QStringList &pPluginNames)
{
    // Check whether it's the PMR Support preferences that have changed and, if
    // so, then update ourselves accordingly

    if (pPluginNames.contains(PMRSupport::PluginName)) {
        mPmrWindowWindow->update(PreferencesInterface::preference(PMRSupport::PluginName,
                                                                  PMRSupport::SettingsPreferencesPmrUrl,
                                                                  PMRSupport::SettingsPreferencesPmrUrlDefault).toString());
    }
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PMRWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::LeftDockWidgetArea;
}

//==============================================================================

QAction * PMRWindowPlugin::windowAction() const
{
    // Return our PMR window action

    return mPmrWindowAction;
}

//==============================================================================

QDockWidget * PMRWindowPlugin::windowWidget() const
{
    // Return our PMR window widget

    return mPmrWindowWindow;
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

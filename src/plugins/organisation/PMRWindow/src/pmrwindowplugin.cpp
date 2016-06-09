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
// PMRWindow plugin
//==============================================================================

#include "coreguiutils.h"
#include "pmrwindowplugin.h"
#include "pmrwindowwindow.h"

//==============================================================================

#include <Qt>

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

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"https://models.physiomeproject.org/\">Physiome Model Repository</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder le <a href=\"https://models.physiomeproject.org/\">Répertoire de Modèles Physiome</a>."));

    return new PluginInfo("Organisation", true, false,
                          QStringList() << "PMRSupport" << "WebViewer",
                          descriptions,
                          QStringList() << "FileBrowserWindow" << "FileOrganiserWindow");
}

//==============================================================================
// I18n interface
//==============================================================================

void PMRWindowPlugin::retranslateUi()
{
    // Retranslate our PMR window action

    retranslateAction(mPmrWindowAction,
                      tr("Physiome Model Repository"),
                      tr("Show/hide the Physiome Model Repository window"));

    // Retranslate our PMR window

    mPmrWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
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
    // Return our window action

    return mPmrWindowAction;
}

//==============================================================================

QDockWidget * PMRWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mPmrWindowWindow;
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

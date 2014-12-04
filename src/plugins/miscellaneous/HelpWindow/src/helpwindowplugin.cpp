/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// HelpWindow plugin
//==============================================================================

#include "coreguiutils.h"
#include "helpwindowplugin.h"
#include "helpwindowwindow.h"

//==============================================================================

#include <Qt>

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
                          QStringList() << "Core",
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

    // Retranslate our help window

    mHelpWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void HelpWindowPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Create an action to show/hide our help window

    mHelpAction = Core::newAction(true, QIcon(":/oxygen/apps/help-browser.png"),
                                  Qt::Key_F1, pMainWindow);

    // Create our help window

    mHelpWindow = new HelpWindowWindow(pMainWindow);
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
    // Retrieve our help window settings

    pSettings->beginGroup(mHelpWindow->objectName());
        mHelpWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our help window settings

    pSettings->beginGroup(mHelpWindow->objectName());
        mHelpWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindowPlugin::handleAction(const QUrl &pUrl)
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
    // Return our window action

    return mHelpAction;
}

//==============================================================================

QDockWidget * HelpWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mHelpWindow;
}

//==============================================================================

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

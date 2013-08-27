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
// Help plugin
//==============================================================================

#include "helpplugin.h"
#include "helpwindow.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace Help {

//==============================================================================

PLUGININFO_FUNC HelpPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to provide help."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour fournir de l'aide."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Miscellaneous,
                          false,
                          true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================

void HelpPlugin::initialize()
{
    // Create an action to show/hide our help window

    mHelpAction = newAction(mMainWindow, true,
                            ":/oxygen/apps/help-browser.png",
                            Qt::Key_F1);

    // Create our help window

    mHelpWindow = new HelpWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::RightDockWidgetArea, mHelpWindow,
                            GuiWindowSettings::Help, mHelpAction);
}

//==============================================================================

void HelpPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our help window settings

    loadWindowSettings(pSettings, mHelpWindow);
}

//==============================================================================

void HelpPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our help window settings

    saveWindowSettings(pSettings, mHelpWindow);
}

//==============================================================================

void HelpPlugin::retranslateUi()
{
    // Retranslate our help action

    retranslateAction(mHelpAction, tr("Help"),
                      tr("Show/hide the OpenCOR help"));

    // Retranslate our help window

    mHelpWindow->retranslateUi();
}

//==============================================================================

}   // namespace Help
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// FileOrganiser plugin
//==============================================================================

#include "fileorganiserplugin.h"
#include "fileorganiserwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

//==============================================================================

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to virtually organise your files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour organiser virtuellement vos fichiers."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Organisation,
                          false,
                          true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================

void FileOrganiserPlugin::initialize()
{
    // Create an action to show/hide our file organiser window

    mFileOrganiserAction = newAction(mMainWindow, true);

    // Create our file organiser window

    mFileOrganiserWindow = new FileOrganiserWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::LeftDockWidgetArea, mFileOrganiserWindow,
                            GuiWindowSettings::Organisation,
                            mFileOrganiserAction);
}

//==============================================================================

void FileOrganiserPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our file organiser window settings

    loadWindowSettings(pSettings, mFileOrganiserWindow);
}

//==============================================================================

void FileOrganiserPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our file organiser window settings

    saveWindowSettings(pSettings, mFileOrganiserWindow);
}

//==============================================================================

void FileOrganiserPlugin::retranslateUi()
{
    // Retranslate our file organiser action

    retranslateAction(mFileOrganiserAction, tr("File Organiser"),
                      tr("Show/hide the File Organiser window"));

    // Retranslate our file organiser window

    mFileOrganiserWindow->retranslateUi();
}

//==============================================================================

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

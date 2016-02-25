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
// PhysiomeModelRepositoryWindow plugin
//==============================================================================

#include "coreguiutils.h"
#include "physiomemodelrepositorywindowplugin.h"
#include "physiomemodelrepositorywindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PhysiomeModelRepositoryWindow {

//==============================================================================

PLUGININFO_FUNC PhysiomeModelRepositoryWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"https://models.physiomeproject.org/\">Physiome Model Repository</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder au <a href=\"https://models.physiomeproject.org/\">Répertoire de Modèles Physiome</a>."));

    return new PluginInfo("Organisation", true, false,
                          QStringList() << "Core" << "libgit2" << "zlib",
                          descriptions,
                          QStringList() << "FileBrowserWindow" << "FileOrganiserWindow");
}

//==============================================================================
// I18n interface
//==============================================================================

void PhysiomeModelRepositoryWindowPlugin::retranslateUi()
{
    // Retranslate our PMR window action

    retranslateAction(mPhysiomeModelRepositoryWindowAction,
                      tr("Physiome Model Repository"),
                      tr("Show/hide the Physiome Model Repository window"));

    // Retranslate our PMR window

    mPhysiomeModelRepositoryWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void PhysiomeModelRepositoryWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our PMR window

    mPhysiomeModelRepositoryWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our PMR window

    mPhysiomeModelRepositoryWindowWindow = new PhysiomeModelRepositoryWindowWindow(Core::mainWindow());
}

//==============================================================================

void PhysiomeModelRepositoryWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PhysiomeModelRepositoryWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PhysiomeModelRepositoryWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our PMR window settings

    pSettings->beginGroup(mPhysiomeModelRepositoryWindowWindow->objectName());
        mPhysiomeModelRepositoryWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PhysiomeModelRepositoryWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our PMR window settings

    pSettings->beginGroup(mPhysiomeModelRepositoryWindowWindow->objectName());
        mPhysiomeModelRepositoryWindowWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PhysiomeModelRepositoryWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PhysiomeModelRepositoryWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::LeftDockWidgetArea;
}

//==============================================================================

QAction * PhysiomeModelRepositoryWindowPlugin::windowAction() const
{
    // Return our window action

    return mPhysiomeModelRepositoryWindowAction;
}

//==============================================================================

QDockWidget * PhysiomeModelRepositoryWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mPhysiomeModelRepositoryWindowWindow;
}

//==============================================================================

}   // namespace PhysiomeModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

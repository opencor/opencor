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
// CellMLModelRepository plugin
//==============================================================================

#include "cellmlmodelrepositoryplugin.h"
#include "cellmlmodelrepositorywindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepository {

//==============================================================================

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"http://models.physiomeproject.org/cellml/\">CellML Model Repository</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder au <a href=\"http://models.physiomeproject.org/cellml/\">Répertoire de Modèles CellML</a>."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Organisation,
                          false,
                          true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================

void CellMLModelRepositoryPlugin::initialize()
{
    // Create an action to show/hide our CellML Model Repository window

    mCellmlModelRepositoryAction = newAction(mMainWindow, true);

    // Create our CellML Model Repository window

    mCellmlModelRepositoryWindow = new CellmlModelRepositoryWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::LeftDockWidgetArea,
                            mCellmlModelRepositoryWindow,
                            GuiWindowSettings::Organisation,
                            mCellmlModelRepositoryAction);
}

//==============================================================================

void CellMLModelRepositoryPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our CellML Model Repository window settings

    loadWindowSettings(pSettings, mCellmlModelRepositoryWindow);
}

//==============================================================================

void CellMLModelRepositoryPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our CellML Model Repository window settings

    saveWindowSettings(pSettings, mCellmlModelRepositoryWindow);
}

//==============================================================================

void CellMLModelRepositoryPlugin::retranslateUi()
{
    // Retranslate our CellML Model Repository action

    retranslateAction(mCellmlModelRepositoryAction,
                      tr("CellML Model Repository"),
                      tr("Show/hide the CellML Model Repository window"));

    // Retranslate our CellML Model Repository window

    mCellmlModelRepositoryWindow->retranslateUi();
}

//==============================================================================

}   // namespace CellMLModelRepository
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

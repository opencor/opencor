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
// CellMLModelRepositoryWindow plugin
//==============================================================================

#include "cellmlmodelrepositorywindowplugin.h"
#include "cellmlmodelrepositorywindowwindow.h"
#include "guiutils.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepositoryWindow {

//==============================================================================

PLUGININFO_FUNC CellMLModelRepositoryWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"http://models.physiomeproject.org/cellml/\">CellML Model Repository</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder au <a href=\"http://models.physiomeproject.org/cellml/\">Répertoire de Modèles CellML</a>."));

    return new PluginInfo(PluginInfo::Organisation, true, false,
                          QStringList() << "Core" << "Spinner",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLModelRepositoryWindowPlugin::retranslateUi()
{
    // Retranslate our CellML Model Repository window action

    retranslateAction(mCellmlModelRepositoryWindowAction,
                      tr("CellML Model Repository"),
                      tr("Show/hide the CellML Model Repository window"));

    // Retranslate our CellML Model Repository window

    mCellmlModelRepositoryWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void CellMLModelRepositoryWindowPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Create an action to show/hide our CellML Model Repository window

    mCellmlModelRepositoryWindowAction = Core::newAction(true, pMainWindow);

    // Create our CellML Model Repository window

    mCellmlModelRepositoryWindowWindow = new CellmlModelRepositoryWindowWindow(pMainWindow);
}

//==============================================================================

void CellMLModelRepositoryWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our CellML Model Repository window settings

    pSettings->beginGroup(mCellmlModelRepositoryWindowWindow->objectName());
        mCellmlModelRepositoryWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellMLModelRepositoryWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our CellML Model Repository window settings

    pSettings->beginGroup(mCellmlModelRepositoryWindowWindow->objectName());
        mCellmlModelRepositoryWindowWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellMLModelRepositoryWindowPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea CellMLModelRepositoryWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::LeftDockWidgetArea;
}

//==============================================================================

QAction * CellMLModelRepositoryWindowPlugin::windowAction() const
{
    // Return our window action

    return mCellmlModelRepositoryWindowAction;
}

//==============================================================================

QDockWidget * CellMLModelRepositoryWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mCellmlModelRepositoryWindowWindow;
}

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

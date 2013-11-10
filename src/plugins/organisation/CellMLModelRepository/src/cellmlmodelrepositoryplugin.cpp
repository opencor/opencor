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

    return new PluginInfo(PluginInfo::Organisation, true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// Core interface
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

void CellMLModelRepositoryPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
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

void CellMLModelRepositoryPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::updateGui(Plugin *pViewPlugin,
                                            const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

QWidget * CellMLModelRepositoryPlugin::viewWidget(const QString &pFileName,
                                                  const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void CellMLModelRepositoryPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString CellMLModelRepositoryPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon CellMLModelRepositoryPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool CellMLModelRepositoryPlugin::saveFile(const QString &pOldFileName,
                                           const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CellMLModelRepositoryPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::fileRenamed(const QString &pOldFileName,
                                              const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLModelRepositoryPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool CellMLModelRepositoryPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
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

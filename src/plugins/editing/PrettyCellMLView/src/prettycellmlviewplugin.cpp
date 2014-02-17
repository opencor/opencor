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
// PrettyCellMLView plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "prettycellmlviewplugin.h"
#include "prettycellmlviewwidget.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PLUGININFO_FUNC PrettyCellMLViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit <a href=\"http://www.cellml.org/\">CellML</a> files using an XML editor."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers <a href=\"http://www.cellml.org/\">CellML</a> à l'aide d'un éditeur XML."));

    return new PluginInfo(PluginInfo::Editing, true,
                          QStringList() << "CoreCellMLEditing" << "Viewer",
                          descriptions);
}

//==============================================================================

PrettyCellMLViewPlugin::PrettyCellMLViewPlugin()
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing,
                          QStringList() << CellMLSupport::CellmlMimeType);
}

//==============================================================================
// Core interface
//==============================================================================

void PrettyCellMLViewPlugin::initialize()
{
    // Create our generic pretty CellML view widget

    mViewWidget = new PrettyCellmlViewWidget(mMainWindow);

    // Hide our generic pretty CellML view widget since it may not initially be
    // shown in our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void PrettyCellMLViewPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our generic pretty CellML view widget settings

    loadViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void PrettyCellMLViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Retrieve our generic pretty CellML view widget settings

    saveViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void PrettyCellMLViewPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Editing interface
//==============================================================================

QScintillaSupport::QScintillaWidget * PrettyCellMLViewPlugin::editor(const QString &pFileName) const
{
    // Return the requested editor

    return mViewWidget->editor(pFileName);
}

//==============================================================================
// GUI interface
//==============================================================================

void PrettyCellMLViewPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::updateGui(Plugin *pViewPlugin,
                                       const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool PrettyCellMLViewPlugin::hasViewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return false;

    // Return whether we know about the given CellML file

    return mViewWidget->contains(pFileName);;
}

//==============================================================================

QWidget * PrettyCellMLViewPlugin::viewWidget(const QString &pFileName,
                                             const bool &pCreate)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // We are dealing with a CellML file, so update our generic pretty CellML
    // view widget using the given CellML file

    if (pCreate) {
        mViewWidget->initialize(pFileName);

        return mViewWidget;
    } else {
        return 0;
    }
}

//==============================================================================

void PrettyCellMLViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return;

    // Ask our generic view widget to finalise the given file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString PrettyCellMLViewPlugin::viewName() const
{
    // Return our pretty CellML view's name

    return tr("Pretty CellML");
}

//==============================================================================

QIcon PrettyCellMLViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool PrettyCellMLViewPlugin::saveFile(const QString &pOldFileName,
                                      const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PrettyCellMLViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::fileModified(const QString &pFileName,
                                          const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void PrettyCellMLViewPlugin::fileRenamed(const QString &pOldFileName,
                                         const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void PrettyCellMLViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool PrettyCellMLViewPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void PrettyCellMLViewPlugin::retranslateUi()
{
    // We don't handle this interface...
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

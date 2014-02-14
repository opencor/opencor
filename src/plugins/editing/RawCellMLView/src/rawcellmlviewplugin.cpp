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
// RawCellMLView plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "cliutils.h"
#include "rawcellmlviewplugin.h"
#include "rawcellmlviewwidget.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

PLUGININFO_FUNC RawCellMLViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit <a href=\"http://www.cellml.org/\">CellML</a> files using an XML editor."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers <a href=\"http://www.cellml.org/\">CellML</a> à l'aide d'un éditeur XML."));

    return new PluginInfo(PluginInfo::Editing, true,
                          QStringList() << "CoreCellMLEditing" << "Viewer",
                          descriptions);
}

//==============================================================================

RawCellMLViewPlugin::RawCellMLViewPlugin()
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing,
                          QStringList() << CellMLSupport::CellmlMimeType);
}

//==============================================================================
// Core interface
//==============================================================================

void RawCellMLViewPlugin::initialize()
{
    // Create our generic raw CellML view widget

    mViewWidget = new RawCellmlViewWidget(mMainWindow);

    // Hide our generic raw CellML view widget since it may not initially be
    // shown in our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void RawCellMLViewPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our generic raw CellML view widget settings

    loadViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void RawCellMLViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Retrieve our generic raw CellML view widget settings

    saveViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void RawCellMLViewPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Editing interface
//==============================================================================

QScintillaSupport::QScintillaWidget * RawCellMLViewPlugin::editor(const QString &pFileName) const
{
    // Return the requested editor

    return mViewWidget->editor(pFileName);
}

//==============================================================================
// GUI interface
//==============================================================================

void RawCellMLViewPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::updateGui(Plugin *pViewPlugin,
                                    const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool RawCellMLViewPlugin::hasViewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return false;

    // Return whether we know about the given CellML file

    return mViewWidget->contains(pFileName);;
}

//==============================================================================

QWidget * RawCellMLViewPlugin::viewWidget(const QString &pFileName,
                                          const bool &pCreate)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // We are dealing with a CellML file, so update our generic raw CellML view
    // widget using the given CellML file

    if (pCreate) {
        mViewWidget->initialize(pFileName);

        return mViewWidget;
    } else {
        return 0;
    }
}

//==============================================================================

void RawCellMLViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return;

    // Ask our generic view widget to finalise the given file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString RawCellMLViewPlugin::viewName() const
{
    // Return our raw CellML view's name

    return tr("Raw CellML");
}

//==============================================================================

QIcon RawCellMLViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool RawCellMLViewPlugin::saveFile(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    // Ask the given file's corresponding view widget to save its contents

    if (viewWidget(pOldFileName)) {
        QScintillaSupport::QScintillaWidget *editor = mViewWidget->editor(pOldFileName);
        bool res = Core::writeTextToFile(pNewFileName, editor->contents());

        if (res)
            editor->resetUndoHistory();

        return res;
    } else {
        return false;
    }
}

//==============================================================================

void RawCellMLViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::fileModified(const QString &pFileName,
                                       const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let its corresponding view widget
    // know about it

    if (viewWidget(pFileName))
        mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void RawCellMLViewPlugin::fileRenamed(const QString &pOldFileName,
                                      const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool RawCellMLViewPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void RawCellMLViewPlugin::retranslateUi()
{
    // We don't handle this interface...
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

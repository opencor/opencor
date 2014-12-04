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
#include "corecliutils.h"
#include "filemanager.h"
#include "rawcellmlviewplugin.h"
#include "rawcellmlviewwidget.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

PLUGININFO_FUNC RawCellMLViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit <a href=\"http://www.cellml.org/\">CellML</a> files using an XML editor."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers <a href=\"http://www.cellml.org/\">CellML</a> à l'aide d'un éditeur XML."));

    return new PluginInfo(PluginInfo::Editing, true, false,
                          QStringList() << "CoreCellMLEditing",
                          descriptions);
}

//==============================================================================
// CellML editing interface
//==============================================================================

void RawCellMLViewPlugin::validateCellml(const QString &pFileName) const
{
    // Validate the given file

    mViewWidget->validate(pFileName);
}

//==============================================================================
// Editing interface
//==============================================================================

Editor::EditorWidget * RawCellMLViewPlugin::editor(const QString &pFileName) const
{
    // Return the requested editor

    return mViewWidget->editor(pFileName);
}

//==============================================================================

bool RawCellMLViewPlugin::isEditorUseable(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return true;
}

//==============================================================================

bool RawCellMLViewPlugin::isEditorContentsModified(const QString &pFileName) const
{
    // Return whether the requested editor has been modified, which here is done
    // by comparing its contents to that of the given file

    Editor::EditorWidget *currentEditor = editor(pFileName);

    return currentEditor?
               Core::FileManager::instance()->isDifferent(pFileName, currentEditor->contents()):
               false;
}

//==============================================================================
// File handling interface
//==============================================================================

bool RawCellMLViewPlugin::saveFile(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    // Save the given file

    return Core::writeTextToFile(pNewFileName, editor(pOldFileName)->contents());
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

void RawCellMLViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void RawCellMLViewPlugin::fileRenamed(const QString &pOldFileName,
                                      const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void RawCellMLViewPlugin::fileSaved(const QString &pFileName)
{
    // The given file has been saved, but because it was done directly by
    // manipulating the file, we need to ask our file manager to unmanage it and
    // manage it back, so that anyone that relies on an internal representation
    // of the file (e.g. the CellML Annotation view plugin) will get properly
    // updated

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    fileManagerInstance->unmanage(pFileName);
    fileManagerInstance->manage(pFileName);
}

//==============================================================================

void RawCellMLViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void RawCellMLViewPlugin::retranslateUi()
{
    // Retranslate our view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void RawCellMLViewPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Create our raw CellML view widget

    mViewWidget = new RawCellmlViewWidget(pMainWindow);

    // Hide our raw CellML view widget since it may not initially be shown in
    // our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void RawCellMLViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our raw CellML view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void RawCellMLViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our raw CellML view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void RawCellMLViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode RawCellMLViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Editing;
}

//==============================================================================

QStringList RawCellMLViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return QStringList() << CellMLSupport::CellmlMimeType;
}

//==============================================================================

QWidget * RawCellMLViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // Update and return our raw CellML view widget using the given CellML file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void RawCellMLViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our raw CellML view widget to finalise the given CellML file

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

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// RawView plugin
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "rawviewplugin.h"
#include "rawviewwidget.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

PLUGININFO_FUNC RawViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit files using a text editor."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers à l'aide d'un éditeur de texte."));

    return new PluginInfo(PluginInfo::Editing, true, false,
                          QStringList() << "CoreEditing",
                          descriptions);
}

//==============================================================================
// Editing interface
//==============================================================================

Editor::EditorWidget * RawViewPlugin::editor(const QString &pFileName) const
{
    // Return the requested editor

    return mViewWidget->editor(pFileName);
}

//==============================================================================

bool RawViewPlugin::isEditorUseable(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return true;
}

//==============================================================================

bool RawViewPlugin::isEditorContentsModified(const QString &pFileName) const
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

bool RawViewPlugin::saveFile(const QString &pOldFileName,
                             const QString &pNewFileName)
{
    // Save the given file

    return Core::writeTextToFile(pNewFileName, editor(pOldFileName)->contents());
}

//==============================================================================

void RawViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void RawViewPlugin::fileRenamed(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void RawViewPlugin::fileSaved(const QString &pFileName)
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

void RawViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void RawViewPlugin::retranslateUi()
{
    // Retranslate our view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void RawViewPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Create our raw view widget

    mViewWidget = new RawViewWidget(pMainWindow);

    // Hide our raw view widget since it may not initially be shown in our
    // central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void RawViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our raw view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void RawViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our raw view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void RawViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode RawViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Editing;
}

//==============================================================================

QStringList RawViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support, i.e. any in our case

    return QStringList();
}

//==============================================================================

QWidget * RawViewPlugin::viewWidget(const QString &pFileName)
{
    // Update and return our raw view widget using the given file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void RawViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our raw view widget to finalise the given file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString RawViewPlugin::viewName() const
{
    // Return our raw view's name

    return tr("Raw");
}

//==============================================================================

QIcon RawViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

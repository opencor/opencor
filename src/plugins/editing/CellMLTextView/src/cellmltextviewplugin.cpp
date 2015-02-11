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
// CellMLTextView plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "cellmltextviewplugin.h"
#include "cellmltextviewwidget.h"
#include "filemanager.h"

//==============================================================================

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

PLUGININFO_FUNC CellMLTextViewPluginInfo()
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

void CellMLTextViewPlugin::validateCellml(const QString &pFileName) const
{
//---GRY--- TO BE IMPLEMENTED...

Q_UNUSED(pFileName);

QMessageBox::information(qApp->activeWindow(),
                         tr("CellML Validation"),
                         "Validation is not yet available for the <strong>CellML Text</strong> view.",
                         QMessageBox::Ok);
}

//==============================================================================
// Editing interface
//==============================================================================

Editor::EditorWidget * CellMLTextViewPlugin::editor(const QString &pFileName) const
{
    // Return the requested editor

    return mViewWidget->editor(pFileName);
}

//==============================================================================

bool CellMLTextViewPlugin::isEditorUseable(const QString &pFileName) const
{
    // Return whether the requested editor is useable

    return mViewWidget->isEditorUseable(pFileName);
}

//==============================================================================

bool CellMLTextViewPlugin::isEditorContentsModified(const QString &pFileName) const
{
    // Return whether the contents of the requested editor has been modified

    return mViewWidget->isEditorContentsModified(pFileName);
}

//==============================================================================
// File handling interface
//==============================================================================

bool CellMLTextViewPlugin::saveFile(const QString &pOldFileName,
                                      const QString &pNewFileName)
{
    // Save the given file

    return mViewWidget->saveFile(pOldFileName, pNewFileName);
}

//==============================================================================

void CellMLTextViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void CellMLTextViewPlugin::fileRenamed(const QString &pOldFileName,
                                         const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void CellMLTextViewPlugin::fileSaved(const QString &pFileName)
{
    // The given file has been saved, but because it was done directly by
    // manipulating the file, we need to ask our file manager to reload it, so
    // that anyone that relies on an internal representation of the file (e.g.
    // the CellML Annotation view plugin) will get properly updated

    Core::FileManager::instance()->reload(pFileName);
}

//==============================================================================

void CellMLTextViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLTextViewPlugin::retranslateUi()
{
    // Retranslate our view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void CellMLTextViewPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Create our CellML text view widget

    mViewWidget = new CellmlTextViewWidget(pMainWindow);

    // Hide our CellML text view widget since it may not initially be shown in
    // our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void CellMLTextViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLTextViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our CellML text view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellMLTextViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our generic CellML text view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellMLTextViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode CellMLTextViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Editing;
}

//==============================================================================

QStringList CellMLTextViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return QStringList() << CellMLSupport::CellmlMimeType;
}

//==============================================================================

QWidget * CellMLTextViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // Update and return our CellML text view widget using the given CellML
    // file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void CellMLTextViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our CellML text view widget to finalise the given CellML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString CellMLTextViewPlugin::viewName() const
{
    // Return our CellML text view's name

    return tr("CellML Text");
}

//==============================================================================

QIcon CellMLTextViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Raw SED-ML view plugin
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "rawsedmlviewplugin.h"
#include "rawsedmlviewwidget.h"
#include "sedmlfilemanager.h"
#include "sedmlsupportplugin.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace RawSEDMLView {

//==============================================================================

PLUGININFO_FUNC RawSEDMLViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit <a href=\"http://www.sed-ml.org/\">SED-ML</a> files using an XML editor."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers <a href=\"http://www.sed-ml.org/\">SED-ML</a> à l'aide d'un éditeur XML."));

    return new PluginInfo("Editing", true, false,
                          QStringList() << "SEDMLEditingView",
                          descriptions);
}

//==============================================================================
// SED-ML editing interface
//==============================================================================

void RawSEDMLViewPlugin::reformat(const QString &pFileName) const
{
    // Reformat the contents of the given file's editor

    mViewWidget->reformat(pFileName);
}

//==============================================================================

bool RawSEDMLViewPlugin::validSedml(const QString &pFileName,
                                    QString &pExtra) const
{
    Q_UNUSED(pExtra);

    // Validate the given file

    return mViewWidget->validate(pFileName);
}

//==============================================================================
// Editing interface
//==============================================================================

EditorWidget::EditorWidget * RawSEDMLViewPlugin::editor(const QString &pFileName) const
{
    // Return the requested editor

    return mViewWidget->editor(pFileName);
}

//==============================================================================

bool RawSEDMLViewPlugin::isEditorUseable(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return true;
}

//==============================================================================

bool RawSEDMLViewPlugin::isEditorContentsModified(const QString &pFileName) const
{
    // Return whether the requested editor has been modified, which here is done
    // by comparing its contents to that of the given file

    EditorWidget::EditorWidget *crtEditor = editor(pFileName);

    return crtEditor?
               Core::FileManager::instance()->isDifferent(pFileName, crtEditor->contents().toUtf8()):
               false;
}

//==============================================================================
// File handling interface
//==============================================================================

bool RawSEDMLViewPlugin::isIndirectRemoteFile(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool RawSEDMLViewPlugin::saveFile(const QString &pOldFileName,
                                  const QString &pNewFileName,
                                  bool &pNeedFeedback)
{
    Q_UNUSED(pNeedFeedback);

    // Save the given file

    EditorWidget::EditorWidget *crtEditor = editor(pOldFileName);

    return crtEditor?
               Core::writeFileContentsToFile(pNewFileName, crtEditor->contents().toUtf8()):
               false;
}

//==============================================================================

void RawSEDMLViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::fileReloaded(const QString &pFileName,
                                      const bool &pFileChanged)
{
    // The given file has been reloaded, so let our view widget know about it

    if (pFileChanged)
        mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void RawSEDMLViewPlugin::fileRenamed(const QString &pOldFileName,
                                     const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void RawSEDMLViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void RawSEDMLViewPlugin::retranslateUi()
{
    // Retranslate our view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void RawSEDMLViewPlugin::initializePlugin()
{
    // Create our raw SED-ML view widget

    mViewWidget = new RawSedmlViewWidget(Core::mainWindow());

    // Hide our raw SED-ML view widget since it may not initially be shown in
    // our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void RawSEDMLViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our raw SED-ML view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void RawSEDMLViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our raw SED-ML view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void RawSEDMLViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode RawSEDMLViewPlugin::viewMode() const
{
    // Return our mode

    return EditingMode;
}

//==============================================================================

QStringList RawSEDMLViewPlugin::viewMimeTypes(const MimeTypeMode &pMimeTypeMode) const
{
    Q_UNUSED(pMimeTypeMode);

    // Return the MIME types we support

    return QStringList() << SEDMLSupport::SedmlMimeType;
}

//==============================================================================

QString RawSEDMLViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return SEDMLSupport::SedmlFileExtension;
}

//==============================================================================

bool RawSEDMLViewPlugin::hasViewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a SED-ML file

    if (!SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName))
        return false;

    // Return whether we have a view widget for the given SED-ML file

    return mViewWidget->contains(pFileName);
}

//==============================================================================

QWidget * RawSEDMLViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a SED-ML file

    if (!SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName))
        return 0;

    // Update and return our raw SED-ML view widget using the given SED-ML file
    // Note: we temporarily disable updates for our raw SED-ML view widget, so
    //       as to avoid any risk of known/unknown/potential flickering...

    mViewWidget->setUpdatesEnabled(false);
        mViewWidget->initialize(pFileName);
    mViewWidget->setUpdatesEnabled(true);

    return mViewWidget;
}

//==============================================================================

void RawSEDMLViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our raw SED-ML view widget to finalise the given SED-ML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString RawSEDMLViewPlugin::viewName() const
{
    // Return our raw SED-ML view's name

    return tr("Raw SED-ML");
}

//==============================================================================

QIcon RawSEDMLViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    static const QIcon NoIcon = QIcon();

    return NoIcon;
}

//==============================================================================

}   // namespace RawSEDMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

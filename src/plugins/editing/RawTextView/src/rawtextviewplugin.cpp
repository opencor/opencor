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
// Raw Text view plugin
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "rawtextviewplugin.h"
#include "rawtextviewwidget.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace RawTextView {

//==============================================================================

PLUGININFO_FUNC RawTextViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit text-based files using a text editor."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers textes à l'aide d'un éditeur de texte."));

    return new PluginInfo(PluginInfo::Editing, true, false,
                          QStringList() << "EditingView",
                          descriptions);
}

//==============================================================================
// Editing interface
//==============================================================================

EditorWidget::EditorWidget * RawTextViewPlugin::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    return mViewWidget->editorWidget(pFileName);
}

//==============================================================================

bool RawTextViewPlugin::isEditorWidgetUseable(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return true;
}

//==============================================================================

bool RawTextViewPlugin::isEditorWidgetContentsModified(const QString &pFileName) const
{
    // Return whether the requested editor widget has been modified, which here
    // is done by comparing its contents to that of the given file

    EditorWidget::EditorWidget *crtEditorWidget = editorWidget(pFileName);

    return crtEditorWidget?
               Core::FileManager::instance()->isDifferent(pFileName, crtEditorWidget->contents()):
               false;
}

//==============================================================================
// File handling interface
//==============================================================================

bool RawTextViewPlugin::saveFile(const QString &pOldFileName,
                                 const QString &pNewFileName,
                                 bool &pNeedFeedback)
{
    Q_UNUSED(pNeedFeedback);

    // Save the given file

    EditorWidget::EditorWidget *crtEditorWidget = editorWidget(pOldFileName);

    return crtEditorWidget?
               Core::writeFileContentsToFile(pNewFileName, crtEditorWidget->contents()):
               false;
}

//==============================================================================

void RawTextViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::fileReloaded(const QString &pFileName,
                                     const bool &pFileChanged)
{
    // The given file has been reloaded, so let our view widget know about it

    if (pFileChanged)
        mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void RawTextViewPlugin::fileRenamed(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void RawTextViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void RawTextViewPlugin::retranslateUi()
{
    // Retranslate our view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool RawTextViewPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool RawTextViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                           QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void RawTextViewPlugin::initializePlugin()
{
    // Create our Raw Text view widget

    mViewWidget = new RawTextViewWidget(Core::mainWindow());

    mViewWidget->setObjectName("RawTextViewWidget");

    // Hide our Raw Text view widget since it may not initially be shown in our
    // central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void RawTextViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our Raw Text view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void RawTextViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our Raw Text view widget settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void RawTextViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode RawTextViewPlugin::viewMode() const
{
    // Return our mode

    return EditingMode;
}

//==============================================================================

QStringList RawTextViewPlugin::viewMimeTypes(const MimeTypeMode &pMimeTypeMode) const
{
    Q_UNUSED(pMimeTypeMode);

    // Return the MIME types we support, i.e. any in our case

    return QStringList();
}

//==============================================================================

QString RawTextViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return "txt";
}

//==============================================================================

QWidget * RawTextViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a text file

    if (!Core::isTextFile(pFileName))
        return 0;

    // Update and return our Raw Text view widget using the given file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void RawTextViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our Raw Text view widget to finalise the given file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString RawTextViewPlugin::viewName() const
{
    // Return our Raw Text view's name

    return tr("Raw Text");
}

//==============================================================================

QIcon RawTextViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    static const QIcon NoIcon = QIcon();

    return NoIcon;
}

//==============================================================================

}   // namespace RawTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

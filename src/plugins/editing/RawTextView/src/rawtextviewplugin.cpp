/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Raw Text view plugin
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "editorwidget.h"
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

    return new PluginInfo(PluginInfo::Category::Editing, true, false,
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
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return true;
}

//==============================================================================

bool RawTextViewPlugin::isEditorWidgetContentsModified(const QString &pFileName) const
{
    // Return whether the requested editor widget has been modified, which here
    // is done by comparing its contents to that of the given file

    EditorWidget::EditorWidget *crtEditorWidget = editorWidget(pFileName);

    return (crtEditorWidget != nullptr)?
               Core::FileManager::instance()->isDifferent(pFileName, crtEditorWidget->contents()):
               false;
}

//==============================================================================
// File handling interface
//==============================================================================

bool RawTextViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool RawTextViewPlugin::saveFile(const QString &pOldFileName,
                                 const QString &pNewFileName,
                                 bool &pNeedFeedback)
{
    Q_UNUSED(pNeedFeedback)

    // Save the given file

    EditorWidget::EditorWidget *crtEditorWidget = editorWidget(pOldFileName);

    return (crtEditorWidget != nullptr)?
               Core::writeFile(pNewFileName, crtEditorWidget->contents()):
               false;
}

//==============================================================================

void RawTextViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so let our view widget know about it

    mViewWidget->fileSaved(pFileName);
}

//==============================================================================

void RawTextViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

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
    Q_UNUSED(pFileName)

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
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

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

    mViewWidget->hide();
}

//==============================================================================

void RawTextViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void RawTextViewPlugin::loadSettings(QSettings &pSettings)
{
    // Retrieve our Raw Text view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void RawTextViewPlugin::saveSettings(QSettings &pSettings) const
{
    // Keep track of our Raw Text view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void RawTextViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode RawTextViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Mode::Editing;
}

//==============================================================================

QStringList RawTextViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support, i.e. any in our case

    return {};
}

//==============================================================================

QString RawTextViewPlugin::viewMimeType(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // Return the MIME type for the given file

    return {};
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
    // Make sure that we are dealing with a text file (be it new or not)

    if (!Core::isTextFile(pFileName)) {
        return nullptr;
    }

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
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    static const QIcon NoIcon = QIcon();

    return NoIcon;
}

//==============================================================================

} // namespace RawTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

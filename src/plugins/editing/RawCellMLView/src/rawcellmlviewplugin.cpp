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
// Raw CellML view plugin
//==============================================================================

#include "cellmlfile.h"
#include "cellmlsupportplugin.h"
#include "coreguiutils.h"
#include "editorwidget.h"
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

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to edit <a href="http://www.cellml.org/">CellML</a> files using an <a href="https://www.w3.org/XML/">XML</a> editor.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour éditer des fichiers <a href="http://www.cellml.org/">CellML</a> à l'aide d'un éditeur <a href="https://www.w3.org/XML/">XML</a>.)"));

    return new PluginInfo(PluginInfo::Category::Editing, true, false,
                          QStringList() << "CellMLEditingView",
                          descriptions);
}

//==============================================================================
// CellML editing view interface
//==============================================================================

void RawCellMLViewPlugin::reformat(const QString &pFileName) const
{
    // Reformat the contents of the given file's editor

    mViewWidget->reformat(pFileName);
}

//==============================================================================

bool RawCellMLViewPlugin::validCellml(const QString &pFileName,
                                      QString &pExtra) const
{
    // Validate the given file

    return mViewWidget->validate(pFileName, pExtra);
}

//==============================================================================
// Editing interface
//==============================================================================

EditorWidget::EditorWidget * RawCellMLViewPlugin::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    return mViewWidget->editorWidget(pFileName);
}

//==============================================================================

bool RawCellMLViewPlugin::isEditorWidgetUseable(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return true;
}

//==============================================================================

bool RawCellMLViewPlugin::isEditorWidgetContentsModified(const QString &pFileName) const
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

bool RawCellMLViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool RawCellMLViewPlugin::saveFile(const QString &pOldFileName,
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

void RawCellMLViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so let our view widget know about it

    mViewWidget->fileSaved(pFileName);
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

void RawCellMLViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName)

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

bool RawCellMLViewPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool RawCellMLViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                             QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void RawCellMLViewPlugin::initializePlugin()
{
    // Create our Raw CellML view widget

    mViewWidget = new RawCellmlViewWidget(Core::mainWindow());

    mViewWidget->setObjectName("RawCellmlViewWidget");

    // Hide our Raw CellML view widget since it may not initially be shown in
    // our central widget

    mViewWidget->hide();
}

//==============================================================================

void RawCellMLViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void RawCellMLViewPlugin::loadSettings(QSettings &pSettings)
{
    // Retrieve our Raw CellML view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void RawCellMLViewPlugin::saveSettings(QSettings &pSettings) const
{
    // Keep track of our Raw CellML view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void RawCellMLViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode RawCellMLViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Mode::Editing;
}

//==============================================================================

QStringList RawCellMLViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return QStringList() << CellMLSupport::CellmlMimeType;
}

//==============================================================================

QString RawCellMLViewPlugin::viewMimeType(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // Return the MIME type for the given CellML file

    return CellMLSupport::CellmlMimeType;
}

//==============================================================================

QString RawCellMLViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return CellMLSupport::CellmlFileExtension;
}

//==============================================================================

QWidget * RawCellMLViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with either a new file or a CellML 1.0/1.1
    // file

    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::fileVersion(pFileName);

    if (   !Core::FileManager::instance()->isNew(pFileName)
        &&  (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_0)
        &&  (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_1)) {
        return nullptr;
    }

    // Update and return our Raw CellML view widget using the given CellML file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void RawCellMLViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our Raw CellML view widget to finalise the given CellML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString RawCellMLViewPlugin::viewName() const
{
    // Return our Raw CellML view's name

    return tr("Raw CellML");
}

//==============================================================================

QIcon RawCellMLViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    static const QIcon NoIcon = QIcon();

    return NoIcon;
}

//==============================================================================

} // namespace RawCellMLView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

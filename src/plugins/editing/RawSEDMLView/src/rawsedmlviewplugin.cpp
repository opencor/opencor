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
// Raw SED-ML view plugin
//==============================================================================

#include "coreguiutils.h"
#include "editorwidget.h"
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

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to edit <a href="http://www.sed-ml.org/">SED-ML</a> files using an <a href="https://www.w3.org/XML/">XML</a> editor.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour éditer des fichiers <a href="http://www.sed-ml.org/">SED-ML</a> à l'aide d'un éditeur <a href="https://www.w3.org/XML/">XML</a>.)"));

    return new PluginInfo(PluginInfo::Category::Editing, true, false,
                          QStringList() << "SEDMLEditingView",
                          descriptions);
}

//==============================================================================
// Editing interface
//==============================================================================

EditorWidget::EditorWidget * RawSEDMLViewPlugin::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    return mViewWidget->editorWidget(pFileName);
}

//==============================================================================

bool RawSEDMLViewPlugin::isEditorWidgetUseable(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return true;
}

//==============================================================================

bool RawSEDMLViewPlugin::isEditorWidgetContentsModified(const QString &pFileName) const
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

bool RawSEDMLViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool RawSEDMLViewPlugin::saveFile(const QString &pOldFileName,
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

void RawSEDMLViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so let our view widget know about it

    mViewWidget->fileSaved(pFileName);
}

//==============================================================================

void RawSEDMLViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

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
    Q_UNUSED(pFileName)

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

bool RawSEDMLViewPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool RawSEDMLViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                            QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void RawSEDMLViewPlugin::initializePlugin()
{
    // Create our Raw SED-ML view widget

    mViewWidget = new RawSedmlViewWidget(Core::mainWindow());

    mViewWidget->setObjectName("RawSedmlViewWidget");

    // Hide our Raw SED-ML view widget since it may not initially be shown in
    // our central widget

    mViewWidget->hide();
}

//==============================================================================

void RawSEDMLViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void RawSEDMLViewPlugin::loadSettings(QSettings &pSettings)
{
    // Retrieve our Raw SED-ML view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void RawSEDMLViewPlugin::saveSettings(QSettings &pSettings) const
{
    // Keep track of our Raw SED-ML view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void RawSEDMLViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// SED-ML editing view interface
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
    Q_UNUSED(pExtra)

    // Validate the given file

    return mViewWidget->validate(pFileName);
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode RawSEDMLViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Mode::Editing;
}

//==============================================================================

QStringList RawSEDMLViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return QStringList() << SEDMLSupport::SedmlMimeType;
}

//==============================================================================

QString RawSEDMLViewPlugin::viewMimeType(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // Return the MIME type for the given SED-ML file

    return SEDMLSupport::SedmlMimeType;
}

//==============================================================================

QString RawSEDMLViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return SEDMLSupport::SedmlFileExtension;
}

//==============================================================================

QWidget * RawSEDMLViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a SED-ML file (be it new or not)

    if (SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName) == nullptr) {
        return nullptr;
    }

    // Update and return our Raw SED-ML view widget using the given SED-ML file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void RawSEDMLViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our Raw SED-ML view widget to finalise the given SED-ML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString RawSEDMLViewPlugin::viewName() const
{
    // Return our Raw SED-ML view's name

    return tr("Raw SED-ML");
}

//==============================================================================

QIcon RawSEDMLViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    static const QIcon NoIcon = QIcon();

    return NoIcon;
}

//==============================================================================

} // namespace RawSEDMLView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// CellML Annotation view plugin
//==============================================================================

#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfile.h"
#include "cellmlsupportplugin.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to annotate <a href="https://cellml.org/">CellML</a> files.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour annoter des fichiers <a href="https://cellml.org/">CellML</a>.)"));

    return new PluginInfo(PluginInfo::Category::Editing, true, false,
                          { "CellMLSupport", "WebViewerWidget" },
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool CellMLAnnotationViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLAnnotationViewPlugin::saveFile(const QString &pOldFileName,
                                          const QString &pNewFileName,
                                          bool &pNeedFeedback)
{
    Q_UNUSED(pNeedFeedback)

    // Save the given file

    return mViewWidget->saveFile(pOldFileName, pNewFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so let our view widget know about it

    mViewWidget->filePermissionsChanged(pFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so let our view widget know about it

    mViewWidget->fileSaved(pFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileRenamed(const QString &pOldFileName,
                                             const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLAnnotationViewPlugin::retranslateUi()
{
    // Retranslate our CellML Annotation view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool CellMLAnnotationViewPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLAnnotationViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                                    QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CellMLAnnotationViewPlugin::initializePlugin()
{
    // Create our CellML Annotation view widget

    mViewWidget = new CellmlAnnotationViewWidget(this, Core::mainWindow());

    mViewWidget->setObjectName("CellmlAnnotationViewWidget");

    // Hide our CellML Annotation view widget since it may not initially be
    // shown in our central widget

    mViewWidget->hide();
}

//==============================================================================

void CellMLAnnotationViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::loadSettings(QSettings &pSettings)
{
    // Retrieve our CellML Annotation view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::saveSettings(QSettings &pSettings) const
{
    // Keep track of our CellML Annotation view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode CellMLAnnotationViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Mode::Editing;
}

//==============================================================================

QStringList CellMLAnnotationViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return { CellMLSupport::CellmlMimeType };
}

//==============================================================================

QString CellMLAnnotationViewPlugin::viewMimeType(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // Return the MIME type for the given CellML file

    return CellMLSupport::CellmlMimeType;
}

//==============================================================================

QString CellMLAnnotationViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return CellMLSupport::CellmlFileExtension;
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are not dealing with a new file, but a CellML 1.0/1.1
    // file

    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::fileVersion(pFileName);

    if (   Core::FileManager::instance()->isNew(pFileName)
        || (   (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_0)
            && (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_1))) {
        return nullptr;
    }

    // Update and return our CellML Annotation view widget using the given
    // CellML file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void CellMLAnnotationViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our CellML Annotation view widget to finalise the given CellML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString CellMLAnnotationViewPlugin::viewName() const
{
    // Return our CellML Annotation view's name

    return tr("CellML Annotation");
}

//==============================================================================

QIcon CellMLAnnotationViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    static const QIcon NoIcon;

    return NoIcon;
}

//==============================================================================

} // namespace CellMLAnnotationView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// Mapping view plugin
//==============================================================================

#include "coreguiutils.h"
#include "mappingviewplugin.h"
#include "mappingviewwidget.h"
#include "filemanager.h"

//==============================================================================

#include <QIcon>
#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

PLUGININFO_FUNC MappingViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides a test view."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit une vue de test."));

    return new PluginInfo(PluginInfo::Category::Editing, true, false,
                          { "Core" },
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool MappingViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool MappingViewPlugin::saveFile(const QString &pOldFileName,
                                const QString &pNewFileName,
                                bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)
    Q_UNUSED(pNeedFeedback)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void MappingViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void MappingViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has had its permissions changed, so update our view
    // widget, if needed

    if (pFileName == mFileName) {
        mViewWidget->filePermissionsChanged(pFileName);
    }
}

//==============================================================================

void MappingViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void MappingViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void MappingViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so update our view widget, if needed

    if (pFileName == mFileName) {
        mViewWidget->fileReloaded(pFileName);
    }
}

//==============================================================================

void MappingViewPlugin::fileRenamed(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName)

    // The given file has been renamed, so update our view widget, if needed

    if (pOldFileName == mFileName) {
        mFileName = pNewFileName;

        mViewWidget->fileRenamed(pOldFileName,pNewFileName);
    }
}

//==============================================================================

void MappingViewPlugin::fileClosed(const QString &pFileName)
{
    // The given file has been closed, so update our internals, if needed

    if (pFileName == mFileName) {
        mFileName = QString();
    }
}

//==============================================================================
// I18n interface
//==============================================================================

void MappingViewPlugin::retranslateUi()
{
    // Retranslate our view widget, if needed

    if (!mFileName.isEmpty()) {
        mViewWidget->retranslateUi();
    }
}

//==============================================================================
// Plugin interface
//==============================================================================

bool MappingViewPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool MappingViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                          QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void MappingViewPlugin::initializePlugin()
{
    // Create our Mapping view widget

    mViewWidget = new MappingViewWidget(Core::mainWindow());

    // Hide our Mapping view widget since it may not initially be shown in our
    // central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void MappingViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void MappingViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void MappingViewPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void MappingViewPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void MappingViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode MappingViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Mode::Editing;
}

//==============================================================================

QStringList MappingViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support, i.e. any in our case

    return {};
}

//==============================================================================

QString MappingViewPlugin::viewMimeType(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // Return the MIME type for the given file

    return {};
}

//==============================================================================

QString MappingViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return {};
}

//==============================================================================

QWidget * MappingViewPlugin::viewWidget(const QString &pFileName)
{
    // Update and return our Mapping view widget using the given file

    // Make sure that we are not dealing with a new file, but a CellML 1.0/1.1
    // file

    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::fileVersion(pFileName);

    if (   Core::FileManager::instance()->isNew(pFileName)
        || (   (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_0)
            && (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_1))) {
        return nullptr;
    }

    mFileName = pFileName;

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void MappingViewPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // Reset our internals

    mFileName = QString();
}

//==============================================================================

QString MappingViewPlugin::viewName() const
{
    // Return our Mapping view's name

    return tr("Mapping");
}

//==============================================================================

QIcon MappingViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return {};
}

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// CellML-Zinc Mapping view plugin
//==============================================================================

#include "cellmlfile.h"
#include "cellmlsupportplugin.h"
#include "cellmlzincmappingviewplugin.h"
#include "cellmlzincmappingviewwidget.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace CellMLZincMappingView {

//==============================================================================

PLUGININFO_FUNC CellMLZincMappingViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to map <a href="https://cellml.org/">CellML</a> variables to Zinc nodes.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour mapper des variables <a href="https://cellml.org/">CellML</a> à des noeuds Zinc.)"));

    return new PluginInfo(PluginInfo::Category::Editing, true, false,
                          { "Core" },
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool CellMLZincMappingViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLZincMappingViewPlugin::saveFile(const QString &pOldFileName,
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

void CellMLZincMappingViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLZincMappingViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has had its permissions changed, so update our view
    // widget, if needed

    mViewWidget->filePermissionsChanged(pFileName);
}

//==============================================================================

void CellMLZincMappingViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLZincMappingViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CellMLZincMappingViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void CellMLZincMappingViewPlugin::fileRenamed(const QString &pOldFileName,
                                              const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void CellMLZincMappingViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLZincMappingViewPlugin::retranslateUi()
{
    // Retranslate our CellML-Zinc Mapping view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool CellMLZincMappingViewPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CellMLZincMappingViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                                     QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CellMLZincMappingViewPlugin::initializePlugin()
{
    // Create our CellML-Zinc Mapping view widget

    mViewWidget = new CellMLZincMappingViewWidget(Core::mainWindow());

    mViewWidget->setObjectName("CellMLZincMappingViewWidget");

    // Hide our CellML-Zinc Mapping view widget since it may not initially be
    // shown in our central widget

    mViewWidget->hide();
}

//==============================================================================

void CellMLZincMappingViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLZincMappingViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void CellMLZincMappingViewPlugin::loadSettings(QSettings &pSettings)
{
    // Retrieve our CellML-Zinc Mapping view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CellMLZincMappingViewPlugin::saveSettings(QSettings &pSettings) const
{
    // Keep track of our CellML-Zinc Mapping view widget settings

    pSettings.beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CellMLZincMappingViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode CellMLZincMappingViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Mode::Editing;
}

//==============================================================================

QStringList CellMLZincMappingViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return { CellMLSupport::CellmlMimeType };
}

//==============================================================================

QString CellMLZincMappingViewPlugin::viewMimeType(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // Return the MIME type for the given CellML file

    return CellMLSupport::CellmlMimeType;
}

//==============================================================================

QString CellMLZincMappingViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return CellMLSupport::CellmlFileExtension;
}

//==============================================================================

QWidget * CellMLZincMappingViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are not dealing with a new file, but a CellML 1.0/1.1
    // file

    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::fileVersion(pFileName);

    if (   Core::FileManager::instance()->isNew(pFileName)
        || (   (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_0)
            && (cellmlVersion != CellMLSupport::CellmlFile::Version::Cellml_1_1))) {
        return nullptr;
    }

    // Update and return our CellML-Zinc Mapping view widget using the given
    // file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void CellMLZincMappingViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our CellML-Zinc Mapping view widget to finalise the given CellML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString CellMLZincMappingViewPlugin::viewName() const
{
    // Return our CellML-Zinc Mapping view's name

    return tr("CellML-Zinc Mapping");
}

//==============================================================================

QIcon CellMLZincMappingViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    static const QIcon NoIcon;

    return NoIcon;
}

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

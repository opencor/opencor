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
// COMBINE support plugin
//==============================================================================

#include "combinefilemanager.h"
#include "combineinterface.h"
#include "combinesupportplugin.h"
#include "corecliutils.h"

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

PLUGININFO_FUNC COMBINESupportPluginInfo()
{
    static const Descriptions descriptions = {
                                                 { "en", QString::fromUtf8(R"(a plugin to support <a href="https://co.mbine.org/">COMBINE</a>.)") },
                                                 { "fr", QString::fromUtf8(R"(une extension pour supporter <a href="https://co.mbine.org/">COMBINE</a>.)") }
                                             };

    return new PluginInfo(PluginInfo::Category::Support, false, false,
                          { "Core", "SEDMLSupport", "ZIPSupport" },
                          descriptions);
}

//==============================================================================

COMBINESupportPlugin::COMBINESupportPlugin()
{
    // Keep track of our file type interface

    static CombineInterfaceData data(qobject_cast<FileTypeInterface *>(this));

    Core::globalInstance(CombineInterfaceDataSignature, &data);
}

//==============================================================================
// File interface
//==============================================================================

bool COMBINESupportPlugin::isFile(const QString &pFileName) const
{
    // Return whether the given file is of the type that we support

    return CombineFileManager::instance()->isFile(pFileName);
}

//==============================================================================

QString COMBINESupportPlugin::mimeType() const
{
    // Return the MIME type we support

    return CombineMimeType;
}

//==============================================================================

QString COMBINESupportPlugin::fileExtension() const
{
    // Return the extension of the type of file we support

    return CombineFileExtension;
}

//==============================================================================

QString COMBINESupportPlugin::fileTypeDescription() const
{
    // Return the description of the type of file we support

    return tr("COMBINE Archive");
}

//==============================================================================

QStringList COMBINESupportPlugin::fileTypeDefaultViews() const
{
    // Return the default views to use for the type of file we support

    return { "SimulationExperimentView" };
}

//==============================================================================
// I18n interface
//==============================================================================

void COMBINESupportPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Plugin interface
//==============================================================================

bool COMBINESupportPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool COMBINESupportPlugin::pluginInterfacesOk(const QString &pFileName,
                                              QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void COMBINESupportPlugin::initializePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void COMBINESupportPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void COMBINESupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // Make a call to the instance of the COMBINE file manager so that it gets
    // properly set up (and therefore can start managing COMBINE archives)
    // before it actually gets used by other plugins
    // Note: we do it here rather than in initializePlugin() since we need the
    //       Core plugin to be initialised (so we can get access to our 'global'
    //       file manager)...

    CombineFileManager::instance();
}

//==============================================================================

void COMBINESupportPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void COMBINESupportPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void COMBINESupportPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================

} // namespace COMBINESupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

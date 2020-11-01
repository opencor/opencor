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
// Zinc support plugin
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "zincsupportplugin.h"
#include "zincinterface.h"

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/result.hpp"
    #include "opencmiss/zinc/streamregion.hpp"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace ZincSupport {

//==============================================================================

PLUGININFO_FUNC ZincSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to support <a href="http://opencmiss.org/documentation/zinc/zinc_data_model.html">Zinc</a>.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour supporter <a href="http://opencmiss.org/documentation/zinc/zinc_data_model.html">Zinc</a>.)"));

    return new PluginInfo(PluginInfo::Category::Support, false, false,
                          { "Zinc", "Compiler", "StandardSupport" },
                          descriptions);
}

//==============================================================================

ZincSupportPlugin::ZincSupportPlugin()
{
    // Keep track of our file type interface

    static ZincInterfaceData data(qobject_cast<FileTypeInterface *>(this));

    Core::globalInstance(ZincInterfaceDataSignature, &data);
}

//==============================================================================
// File interface
//==============================================================================

bool ZincSupportPlugin::isFile(const QString &pFileName) const
{
    // Return whether the given file is of the type that we support

    OpenCMISS::Zinc::Context zincContext = OpenCMISS::Zinc::Context("SimulationViewZincWidget");

    OpenCMISS::Zinc::Region region = zincContext.getDefaultRegion();
    OpenCMISS::Zinc::StreaminformationRegion sir = region.createStreaminformationRegion();

    sir.createStreamresourceFile(pFileName.toUtf8().constData());

    return OpenCMISS::Zinc::Result::RESULT_OK == region.read(sir);
}

//==============================================================================

QString ZincSupportPlugin::mimeType() const
{
    // Return the MIME type we support

    return ZincMeshMimeType;
}

//==============================================================================

QString ZincSupportPlugin::fileExtension() const
{
    // Return the extension of the type of file we support

    return ZincMeshFileExtension;
}

//==============================================================================

QString ZincSupportPlugin::fileTypeDescription() const
{
    // Return the description of the type of file we support

    return tr("Zinc File");
}

//==============================================================================

QStringList ZincSupportPlugin::fileTypeDefaultViews() const
{
    // Return the default views to use for the type of file we support

    return {};
}

//==============================================================================
// Plugin interface
//==============================================================================

bool ZincSupportPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool ZincSupportPlugin::pluginInterfacesOk(const QString &pFileName,
                                             QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void ZincSupportPlugin::initializePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void ZincSupportPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void ZincSupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    initializePlugin();
}

//==============================================================================

void ZincSupportPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void ZincSupportPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void ZincSupportPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================

} // namespace ZincSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

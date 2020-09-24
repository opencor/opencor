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

#pragma once

//==============================================================================

#include "filetypeinterface.h"
#include "meshinterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {
namespace ZincSupport {

//==============================================================================

PLUGININFO_FUNC ZincSupportPluginInfo();

//==============================================================================

static const auto ZincMeshMimeType      = QStringLiteral("application/zinc");
static const auto ZincMeshFileExtension = QStringLiteral("exfile");

//==============================================================================

class ZincSupportPlugin : public QObject, public FileTypeInterface,
                            public PluginInterface, public MeshInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.ZincSupportPlugin" FILE "zincsupportplugin.json")

    Q_INTERFACES(OpenCOR::FileTypeInterface)
    Q_INTERFACES(OpenCOR::MeshInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit ZincSupportPlugin();

#include "filetypeinterface.inl"
#include "plugininterface.inl"

};

//==============================================================================

} // namespace ZincSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

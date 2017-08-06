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
// COMBINE support plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filetypeinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

PLUGININFO_FUNC COMBINESupportPluginInfo();

//==============================================================================

static const auto CombineMimeType      = QStringLiteral("application/combine+zip");
static const auto CombineFileExtension = QStringLiteral("omex");

//==============================================================================

class COMBINESupportPlugin : public QObject, public FileTypeInterface,
                             public I18nInterface, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.COMBINESupportPlugin" FILE "combinesupportplugin.json")

    Q_INTERFACES(OpenCOR::FileTypeInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit COMBINESupportPlugin();

#include "filetypeinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"
};

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

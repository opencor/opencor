/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

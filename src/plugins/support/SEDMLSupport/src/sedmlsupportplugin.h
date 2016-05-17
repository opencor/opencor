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
// SEDMLSupport plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filetypeinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

PLUGININFO_FUNC SEDMLSupportPluginInfo();

//==============================================================================

static const auto SedmlMimeType      = QStringLiteral("application/sedml+xml");
static const auto SedmlFileExtension = QStringLiteral("sedml");

//==============================================================================

class SEDMLSupportPlugin : public QObject, public FileTypeInterface,
                           public GuiInterface, public I18nInterface,
                           public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.SEDMLSupportPlugin" FILE "sedmlsupportplugin.json")

    Q_INTERFACES(OpenCOR::FileTypeInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit SEDMLSupportPlugin();

#include "filetypeinterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    QAction *mFileNewSedmlFileAction;

private Q_SLOTS:
    void newSedmlFile();
};

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

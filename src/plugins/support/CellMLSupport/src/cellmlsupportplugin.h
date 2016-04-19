/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CellMLSupport plugin
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
namespace CellMLSupport {

//==============================================================================

PLUGININFO_FUNC CellMLSupportPluginInfo();

//==============================================================================

static const auto CellmlMimeType      = QStringLiteral("application/cellml+xml");
static const auto CellmlFileExtension = QStringLiteral("cellml");

//==============================================================================

class CellMLSupportPlugin : public QObject, public FileTypeInterface,
                            public GuiInterface, public I18nInterface,
                            public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CellMLSupportPlugin" FILE "cellmlsupportplugin.json")

    Q_INTERFACES(OpenCOR::FileTypeInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit CellMLSupportPlugin();

#include "filetypeinterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    QAction *mFileNewCellmlFileAction;

private Q_SLOTS:
    void newCellmlFile();
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

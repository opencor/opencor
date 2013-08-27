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

#ifndef CELLMLSUPPORTPLUGIN_H
#define CELLMLSUPPORTPLUGIN_H

//==============================================================================

#include "cellmlsupportglobal.h"
#include "coreinterface.h"
#include "fileinterface.h"
#include "i18ninterface.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

PLUGININFO_FUNC CellMLSupportPluginInfo();

//==============================================================================

static const QString CellmlMimeType = "application/cellml+xml";
static const QString CellmlFileExtension = "cellml";

//==============================================================================

class CellMLSupportPlugin : public QObject, public CoreInterface,
                            public FileInterface, public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CellMLSupportPlugin" FILE "cellmlsupportplugin.json")

    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::FileInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initializationsDone(const Plugins & pLoadedPlugins);

    virtual FileTypes fileTypes() const;
    virtual QString fileTypeDescription(const QString &pMimeType) const;
};

//==============================================================================

bool CELLMLSUPPORT_EXPORT isCellmlFile(const QString &pFileName);

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

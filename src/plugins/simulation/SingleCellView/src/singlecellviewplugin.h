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
// SingleCellView plugin
//==============================================================================

#pragma once

//==============================================================================

#include "datastoreinterface.h"
#include "filehandlinginterface.h"
#include "filetypeinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"
#include "solverinterface.h"
#include "viewinterface.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

PLUGININFO_FUNC SingleCellViewPluginInfo();

//==============================================================================

class SingleCellViewWidget;

//==============================================================================

class SingleCellViewPlugin : public QObject, public FileHandlingInterface,
                             public I18nInterface, public PluginInterface,
                             public ViewInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.SingleCellViewPlugin" FILE "singlecellviewplugin.json")

    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::ViewInterface)

public:
    explicit SingleCellViewPlugin();

#include "filehandlinginterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"
#include "viewinterface.inl"

    SingleCellViewWidget * viewWidget() const;

    SolverInterfaces solverInterfaces() const;
    DataStoreInterfaces dataStoreInterfaces() const;
    Plugins cellmlEditingViewPlugins() const;

    FileTypes sedmlFileTypes() const;
    FileTypes combineFileTypes() const;

private:
    SingleCellViewWidget *mViewWidget;

    SolverInterfaces mSolverInterfaces;
    DataStoreInterfaces mDataStoreInterfaces;
    Plugins mCellmlEditingViewPlugins;

    FileTypes mSedmlFileTypes;
    FileTypes mCombineFileTypes;
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

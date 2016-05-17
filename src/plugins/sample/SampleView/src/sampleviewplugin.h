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
// SampleView plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filehandlinginterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"
#include "viewinterface.h"

//==============================================================================

namespace OpenCOR {
namespace SampleView {

//==============================================================================

PLUGININFO_FUNC SampleViewPluginInfo();

//==============================================================================

class SampleViewWidget;

//==============================================================================

class SampleViewPlugin : public QObject, public FileHandlingInterface,
                         public I18nInterface, public PluginInterface,
                         public ViewInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.SampleViewPlugin" FILE "sampleviewplugin.json")

    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::ViewInterface)

public:
    explicit SampleViewPlugin();

#include "filehandlinginterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"
#include "viewinterface.inl"

private:
    SampleViewWidget *mViewWidget;

    QString mFileName;
};

//==============================================================================

}   // namespace SampleView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

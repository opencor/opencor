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
// Python plugin
//==============================================================================

#pragma once

//==============================================================================

#include "cliinterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {
namespace Python {

//==============================================================================

PLUGININFO_FUNC PythonPluginInfo();

//==============================================================================

class PythonPlugin : public QObject, public CliInterface,
                     public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.PythonPlugin" FILE "pythonplugin.json")

    Q_INTERFACES(OpenCOR::CliInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
#include "cliinterface.inl"
#include "plugininterface.inl"

private:
    void runHelpCommand();
    int runPythonShell(const QStringList &pArguments);
};

//==============================================================================

}   // namespace Python
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

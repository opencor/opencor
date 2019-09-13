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
// Run Python script plugin
//==============================================================================

#pragma once

//==============================================================================

#include "cliinterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace PythonRunScript {

//==============================================================================

PLUGININFO_FUNC PythonRunScriptPluginInfo();

//==============================================================================

class PythonRunScriptPlugin : public QObject, public CliInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.PythonRunScriptPlugin" FILE "pythonrunscriptplugin.json")

    Q_INTERFACES(OpenCOR::CliInterface)

public:
#include "cliinterface.inl"

private:
    void runHelpCommand();
    bool runScript(const QStringList &pArguments, int &pRes);
    bool runShell(const QStringList &pArguments, int &pRes);
};

//==============================================================================

}   // namespace PythonRunScript
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

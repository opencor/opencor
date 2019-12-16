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
// Python shell plugin
//==============================================================================

#pragma once

//==============================================================================

#include "cliinterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace PythonShell {

//==============================================================================

PLUGININFO_FUNC PythonShellPluginInfo();

//==============================================================================

class PythonShellPlugin : public QObject, public CliInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.PythonShellPlugin" FILE "pythonshellplugin.json")

    Q_INTERFACES(OpenCOR::CliInterface)

public:
#include "cliinterface.inl"

private:
    void runHelpCommand();
    bool runPython(const QStringList &pArguments, int &pRes);
};

//==============================================================================

} // namespace PythonShell
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
// Jupyter kernel plugin
//==============================================================================

#include "jupyterkernelplugin.h"
#include "pythonqtsupport.h"

//==============================================================================

#include <QCoreApplication>
#include <QSettings>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace JupyterKernel {

//==============================================================================

PLUGININFO_FUNC JupyterKernelPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the Jupyter kernel plugin."));
    descriptions.insert("fr", QString::fromUtf8("the Jupyter kernel plugin."));

    return new PluginInfo(PluginInfo::Miscellaneous, true, true,
                          QStringList() << "Core" << "DataStore"
                                        << "SimulationSupport"
                                        << "PythonQtSupport",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

int JupyterKernelPlugin::executeCommand(const QString &pCommand,
                                        const QStringList &pArguments)
{
    // Run the given CLI command

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return 0;
    } else if (!pCommand.compare("kernel")) {
        // Run the Jupyter kernel with the specified connection file

        return runKernel(pArguments);
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return -1;
    }
}

//==============================================================================
// Plugin specific
//==============================================================================

void JupyterKernelPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the Jupyter kernel plugin:" << std::endl;
    std::cout << " * Display the commands supported by the Jupyter kernel plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Start the OpenCOR Jupyter kernel:" << std::endl;
    std::cout << "      kernel <connection_file>" << std::endl;
}

//==============================================================================

// The OpenCOR Jupyter kernel

static QString jupyterKernel = R"PYTHON(
from ipykernel.ipkernel import IPythonKernel
import matplotlib

class OpenCORKernel(IPythonKernel):
    implementation = 'OpenCOR'
    implementation_version = '0.6'
    banner = "Jupyter kernel for OpenCOR"

if __name__ == '__main__':
    from ipykernel.kernelapp import IPKernelApp
    IPKernelApp.connection_file = '%1'
    IPKernelApp.launch_instance(kernel_class=OpenCORKernel)
    )PYTHON";

//==============================================================================

int JupyterKernelPlugin::runKernel(const QStringList &pArguments)
{
    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 1) {
        runHelpCommand();

        return -1;
    }

    // Run the the kernel using our connection file

    PythonQtSupport::evalScript(jupyterKernel.arg(pArguments[0]));

    return 0;
}

//==============================================================================

}   // namespace JupyterKernel
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

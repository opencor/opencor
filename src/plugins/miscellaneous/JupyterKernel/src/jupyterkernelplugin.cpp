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
// Jupyter kernel plugin
//==============================================================================

#include "jupyterkernelplugin.h"
#include "pythonqtsupport.h"

//==============================================================================

#include <QCoreApplication>

//==============================================================================

namespace OpenCOR {
namespace JupyterKernel {

//==============================================================================

PLUGININFO_FUNC JupyterKernelPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(the <a href="https://jupyter.org/">Jupyter</a> kernel plugin.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(le plugin du noyau <a href="https://jupyter.org/">Jupyter</a>.)"));

    return new PluginInfo(PluginInfo::Category::Miscellaneous, false, true,
                          QStringList() << "Core" << "SimulationSupport",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

bool JupyterKernelPlugin::executeCommand(const QString &pCommand,
                                         const QStringList &pArguments,
                                         int &pRes)
{
    // Run the given CLI command

    static const QString Help = "help";
    static const QString Kernel = "kernel";

    if (pCommand == Help) {
        // Display the commands that we support

        runHelpCommand();

        return true;
    }

    if (pCommand == Kernel) {
        // Run the Jupyter kernel with the specified connection file

        return runKernel(pArguments, pRes);
    }

    // Not a CLI command that we support

    runHelpCommand();

    return false;
}

//==============================================================================
// Plugin specific
//==============================================================================

void JupyterKernelPlugin::runHelpCommand()
{
    // Output the commands we support

    std::cout << "Commands supported by the JupyterKernel plugin:" << std::endl;
    std::cout << " * Display the commands supported by the JupyterKernel plugin:" << std::endl;
    std::cout << "      help" << std::endl;
    std::cout << " * Start the OpenCOR Jupyter kernel:" << std::endl;
    std::cout << "      kernel <connectionFile>" << std::endl;
}

//==============================================================================

bool JupyterKernelPlugin::runKernel(const QStringList &pArguments, int &pRes)
{
    Q_UNUSED(pRes)

    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 1) {
        runHelpCommand();

        return false;
    }

    // Run the the kernel using our connection file
    // Note: any backslashes in the filename need to be escaped for Python...

    static const QString JupyterKernel = R"PYTHON(
import matplotlib

from IPython.core.pylabtools import activate_matplotlib
from ipykernel.ipkernel import IPythonKernel

# Use the Jupyter notebook for Matplotlib plots

MATPLOTLIB_BACKEND = 'nbAgg'

matplotlib.use(MATPLOTLIB_BACKEND)

# Enable interactive plots

activate_matplotlib(MATPLOTLIB_BACKEND)

# Make sure that Jupyter %matplotlib magic doesn't try to use OS-specific
# backend

matplotlib.rcParamsOrig['backend'] = matplotlib.rcParams['backend']

# Minimal customisation of the standard IPython kernel

class OpenCORKernel(IPythonKernel):
    implementation = '%1'
    implementation_version = '%2'
    banner = "Jupyter kernel for OpenCOR"

if __name__ == '__main__':
    from ipykernel.kernelapp import IPKernelApp

    IPKernelApp.connection_file = '%3'
    IPKernelApp.launch_instance(kernel_class=OpenCORKernel)
)PYTHON";

    PythonQtSupport::evaluateScript(JupyterKernel.arg(qApp->applicationName(),
                                                      qApp->applicationVersion(),
                                                      QString(pArguments[0]).replace("\\", "\\\\")));

    return true;
}

//==============================================================================

} // namespace JupyterKernel
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

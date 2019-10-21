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

//==============================================================================

#include <QCoreApplication>
#include <QSettings>
#include <QTimer>

//==============================================================================

#include "pythonbegin.h"
    #include "pythonqtsupport.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {
namespace JupyterKernel {

//==============================================================================

PLUGININFO_FUNC JupyterKernelPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the Jupyter kernel plugin."));
    descriptions.insert("fr", QString::fromUtf8("the Jupyter kernel plugin."));

    return new PluginInfo(PluginInfo::Category::Miscellaneous, true, true,
                          QStringList() << "Core" << "DataStore"
                                        << "SimulationSupport"
                                        << "PythonQtSupport",
                          descriptions);
}

//==============================================================================
// CLI interface
//==============================================================================

bool JupyterKernelPlugin::executeCommand(const QString &pCommand,
                                         const QStringList &pArguments, int &pRes)
{
    // Run the given CLI command

    if (!pCommand.compare("help")) {
        // Display the commands that we support

        runHelpCommand();

        return true;
    } else if (!pCommand.compare("kernel")) {
        // Run the Jupyter kernel with the specified connection file

        return runKernel(pArguments, pRes);
    } else {
        // Not a CLI command that we support

        runHelpCommand();

        return false;
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
import matplotlib
from IPython.core.pylabtools import activate_matplotlib
from ipykernel.ipkernel import IPythonKernel

# Use the Jupyter notebook for matplotlib plots

MATPLOTLIB_BACKEND = 'nbAgg'
matplotlib.use(MATPLOTLIB_BACKEND)

# Enable interactive plots

activate_matplotlib(MATPLOTLIB_BACKEND)

# Make sure Jupyter %matplotlib magic won't try to use OS-specific backend

matplotlib.rcParamsOrig['backend'] = matplotlib.rcParams['backend']

# Minimal customisation of the standard IPython kernel

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

bool JupyterKernelPlugin::runKernel(const QStringList &pArguments, int &pRes)
{
    Q_UNUSED(pRes)

    // Make sure that we have the correct number of arguments

    if (pArguments.count() != 1) {
        runHelpCommand();

        return false;
    }

    // Run the the kernel using our connection file
    // Note: any backslashes in the filename need to be escaped for Python

    QString connectionFile = pArguments[0];

    PythonQtSupport::evalScript(jupyterKernel.arg(connectionFile.replace("\\", "\\\\")));

    return true;
}

//==============================================================================

}   // namespace JupyterKernel
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

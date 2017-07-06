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

#include "CTK/ctkAbstractPythonManager.h"
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

    return new PluginInfo(PluginInfo::Miscellaneous, true, false,
                          QStringList() << "Core" << "PythonQtSupport",
                          descriptions);
}

//==============================================================================
// Event loop interface
//==============================================================================

bool JupyterKernelPlugin::useExec()
{
    return !mConnectionFile.isEmpty();
}

//==============================================================================

int JupyterKernelPlugin::exec()
{
    if (!mConnectionFile.isEmpty())
        return runKernel();

    return 0;
}

//==============================================================================
// Plugin interface
//==============================================================================

bool JupyterKernelPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool JupyterKernelPlugin::pluginInterfacesOk(const QString &pFileName,
                                                   QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void JupyterKernelPlugin::initializePlugin()
{
    mConnectionFile = QString();
}

//==============================================================================

void JupyterKernelPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void JupyterKernelPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void JupyterKernelPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void JupyterKernelPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void JupyterKernelPlugin::handleUrl(const QUrl &pUrl)
{
    // Save the path of the connection file

    mConnectionFile = pUrl.path().mid(1);
}

//==============================================================================
// Plugin specific
//==============================================================================

// The OpenCOR Jupyter kernel

static QString jupyterKernel = R"PYTHON(
import os

from ipykernel.eventloops import register_integration, enable_gui, loop_qt4
from ipykernel.ipkernel import IPythonKernel

import matplotlib


@register_integration('opencor')
def loop_opencor(kernel):
    """Start a kernel with PyQt5 event loop integration."""
    os.environ['QT_API'] = 'opencor'
    return loop_qt4(kernel)


class OpenCORKernel(IPythonKernel):
    implementation = 'OpenCOR'
    implementation_version = '0.1'
    banner = "Jupyter kernel for OpenCOR"

    def __init__(self, *args, **kwds):
        super().__init__(*args, **kwds)
        # Use the Jupyter notebook backend for matplotlib
        matplotlib.use('nbagg')
        # Work nicely with OpenCOR's Qt exec() loop
        enable_gui('opencor', self)

    def do_shutdown(self, restart):
        from PythonQt import QtGui
        QtGui.QApplication.instance().exit(0)
        return super().do_shutdown(restart)

if __name__ == '__main__':
    import sys
    sys.argv.append('--debug')

    from ipykernel.kernelapp import IPKernelApp
    IPKernelApp.connection_file = '%1'
    IPKernelApp.launch_instance(kernel_class=OpenCORKernel)
    )PYTHON";

//==============================================================================

int JupyterKernelPlugin::runKernel()
{
    // Run the the kernel using our connection file

    PythonQtSupport::PythonQtSupportPlugin::pythonManager()->executeString(jupyterKernel.arg(mConnectionFile));

    // TODO: return gui->exec()
    return 0;
}

//==============================================================================

}   // namespace JupyterKernel
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

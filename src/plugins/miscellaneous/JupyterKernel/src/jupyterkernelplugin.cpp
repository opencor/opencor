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
// Jupyter kernel plugin
//==============================================================================

#include "ctkAbstractPythonManager.h"
#include "jupyterkernelplugin.h"
#include "pythonqtsupportplugin.h"

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
                          QStringList() << "Core" << "PythonQtSupport" << "PythonWrapper",
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
import logging

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
        logging.debug("Kernel inited...")


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)-15s %(message)s')

    from ipykernel.kernelapp import IPKernelApp
    IPKernelApp.connection_file = '%1'
    logging.debug("Launching kernel...")
    IPKernelApp.launch_instance(kernel_class=OpenCORKernel)
    logging.debug("Running python...")
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

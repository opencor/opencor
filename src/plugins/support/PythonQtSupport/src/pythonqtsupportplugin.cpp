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
// Python Qt Support plugin
//==============================================================================

#include "CTK/ctkAbstractPythonManager.h"
#include "corecliutils.h"
#include "pythonqtsupportplugin.h"
#include "pythoninterface.h"
#include "solverinterface.h"

//==============================================================================

#include <PythonQt/PythonQt.h>
#include <PythonQt/PythonQt_QtAll.h>

//==============================================================================

#include <Qt>

//==============================================================================

namespace OpenCOR {
namespace PythonQtSupport {

//==============================================================================

PLUGININFO_FUNC PythonQtSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the Python Qt support plugin."));
    descriptions.insert("fr", QString::fromUtf8("the Python Qt support plugin."));

    return new PluginInfo(PluginInfo::Support, false, false,
                          QStringList() << "PythonQtAPI",
                          descriptions);
}

//==============================================================================

PythonQtSupportPlugin::PythonQtSupportPlugin() :
    mPythonManager(0),
    mOpenCORModule(0)
{
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonQtSupportPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonQtSupportPlugin::pluginInterfacesOk(const QString &pFileName,
                                               QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonQtSupportPlugin::initializePlugin()
{
    // Use our (patched) matplotlib backend for PythonQt

    qputenv("MPLBACKEND", "module://matplotlib.backends.backend_qt5agg");

    // Create and initialise a new CTK Python manager

    auto pythonManager = new ctkAbstractPythonManager(this);

    // This also initialises Python Qt

    pythonManager->initialize();

    // Enable the Qt bindings for Python

    PythonQt_QtAll::init();

    // Save the manager in our instance

    instance()->mPythonManager = pythonManager;
}

//==============================================================================

void PythonQtSupportPlugin::finalizePlugin()
{
    delete instance()->mPythonManager;
}

//==============================================================================

void PythonQtSupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    // We need to register the Solver::Properties class with Qt so it gets automatically
    // wrapped to Python

    qRegisterMetaType<OpenCOR::Solver::Solver::Properties>("Solver::Solver::Properties");

    // Register wrappers for every plugin that has a Python interface

    foreach (Plugin *plugin, pLoadedPlugins) {
        PythonInterface *pythonInterface = qobject_cast<PythonInterface *>(plugin->instance());

        if (pythonInterface) {
            pythonInterface->registerPythonClasses(mOpenCORModule);
        }
    }
}

//==============================================================================

void PythonQtSupportPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtSupportPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtSupportPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
//==============================================================================

ctkAbstractPythonManager *PythonQtSupportPlugin::pythonManager(void)
{
    return instance()->mPythonManager;
}

//==============================================================================

PythonQtSupportPlugin *PythonQtSupportPlugin::instance(void)
{
    // Return the 'global' instance of our Python Qt plugin

    static PythonQtSupportPlugin pluginInstance;
    return static_cast<PythonQtSupportPlugin *>(Core::globalInstance("OpenCOR::PythonQtSupport::PythonQtSupportPlugin",
                                                                     &pluginInstance));
}

//==============================================================================

}   // namespace PythonQtSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

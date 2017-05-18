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
// Python wrapper plugin
//==============================================================================

#include "corecliutils.h"
#include "pythonwrappercore.h"
#include "pythonwrapperdatastore.h"
#include "pythonwrapperplugin.h"
#include "pythonwrappersimulationexperimentview.h"
#include "pythonwrappersolver.h"
#include "simulationexperimentviewplugin.h"

//==============================================================================

namespace OpenCOR {
namespace PythonWrapper {

//==============================================================================

PLUGININFO_FUNC PythonWrapperPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access OpenCOR data from <a href=\"http://www.python.org/\">Python</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder... <a href=\"http://www.python.org/\">Python</a>."));

    return new PluginInfo(PluginInfo::Tools, true, false,
                          QStringList() << "PythonQtSupport" << "SimulationExperimentView",
                          descriptions);
}

//==============================================================================

PythonWrapperPlugin::PythonWrapperPlugin()
{
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonWrapperPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonWrapperPlugin::pluginInterfacesOk(const QString &pFileName,
                                             QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonWrapperPlugin::initializePlugin()
{
    // Create a Python module to access OpenCOR's objects

    mOpenCORModule = PythonQt::self()->createModuleFromScript("OpenCOR");

    mPythonWrapperCore = new PythonWrapperCore(mOpenCORModule);
    mPythonWrapperDataStore = new PythonWrapperDataStore(mOpenCORModule);
    mPythonWrapperSimulationExperimentView = new PythonWrapperSimulationExperimentView(mOpenCORModule);
    mPythonWrapperSolver = new PythonWrapperSolver(mOpenCORModule);
}

//==============================================================================

void PythonWrapperPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonWrapperPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    // Look for simulation views

    foreach (Plugin *plugin, pLoadedPlugins) {

        if (!plugin->name().compare("SimulationExperimentView")) {

            auto *simulationExperimentViewPlugin = qobject_cast<SimulationExperimentView::SimulationExperimentViewPlugin *>(plugin->instance());
            instance()->mSimulationExperimentViewWidget = simulationExperimentViewPlugin->viewWidget();

            break;
        }
    }
}

//==============================================================================

void PythonWrapperPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonWrapperPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonWrapperPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Our own methods
//==============================================================================

PythonWrapperPlugin *PythonWrapperPlugin::instance(void)
{
    // Return the 'global' instance of our Python wrapper plugin

    static PythonWrapperPlugin pluginInstance;
    return static_cast<PythonWrapperPlugin *>(Core::globalInstance("OpenCOR::PythonQtSupport::PythonWrapperPlugin",
                                                                   &pluginInstance));
}

//==============================================================================

SimulationExperimentView::SimulationExperimentViewWidget *PythonWrapperPlugin::simulationExperimentViewWidget()
{
    return mSimulationExperimentViewWidget;
}

//==============================================================================

}   // namespace PythonSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

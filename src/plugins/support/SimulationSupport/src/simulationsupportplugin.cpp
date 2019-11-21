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
// Simulation support plugin
//==============================================================================

#include "simulation.h"
#include "simulationmanager.h"
#include "simulationsupportplugin.h"
#include "simulationsupportpythonwrapper.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

PLUGININFO_FUNC SimulationSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to support simulations."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter des simulations."));

    return new PluginInfo(PluginInfo::Category::Support, false, false,
                          QStringList() << "COMBINESupport" << "CVODESolver" << "DataStore" << "ForwardEulerSolver" << "FourthOrderRungeKuttaSolver" << "HeunSolver" << "KINSOLSolver" << "SecondOrderRungeKuttaSolver",
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool SimulationSupportPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool SimulationSupportPlugin::saveFile(const QString &pOldFileName,
                                       const QString &pNewFileName,
                                       bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)
    Q_UNUSED(pNeedFeedback)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void SimulationSupportPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SimulationSupportPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SimulationSupportPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SimulationSupportPlugin::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so let our simulation manager know about
    // it

    SimulationManager::instance()->save(pFileName);
}

//==============================================================================

void SimulationSupportPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our simulation manager know
    // about it

    SimulationManager::instance()->reload(pFileName);
}

//==============================================================================

void SimulationSupportPlugin::fileRenamed(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    // The given file has been renamed, so let our simulation manager know
    // about it

    SimulationManager::instance()->rename(pOldFileName, pNewFileName);
}

//==============================================================================

void SimulationSupportPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void SimulationSupportPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Python interface
//==============================================================================

void SimulationSupportPlugin::registerPythonClasses(void *pModule)
{
    // Register our Python classes

    new SimulationSupportPythonWrapper(pModule, this);
}

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

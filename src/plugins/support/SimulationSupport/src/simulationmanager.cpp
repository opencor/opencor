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
// Simulation manager
//==============================================================================

#include "corecliutils.h"
#include "simulation.h"
#include "simulationmanager.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

SimulationManager * SimulationManager::instance()
{
    // Return the 'global' instance of our simulation manager class

    static SimulationManager instance;

    return static_cast<SimulationManager *>(Core::globalInstance("OpenCOR::SimulationSupport::SimulationManager::instance()",
                                                                 &instance));
}

//==============================================================================

void SimulationManager::manage(const QString &pFileName)
{
    // Check whether we are already managing the corresponding simulation and,
    // if not, add it to our list of managed simulations

    QString fileName = Core::canonicalFileName(pFileName);

    if (simulation(fileName) == nullptr) {
        mSimulations.insert(fileName, new Simulation(fileName));
    }
}

//==============================================================================

void SimulationManager::unmanage(const QString &pFileName)
{
    // Check whether we are already managing the corresponding simulation and,
    // if so, remove it from our list of managed simulations after having
    // deleted it

    Simulation *crtSimulation = simulation(pFileName);

    if (crtSimulation != nullptr) {
        delete crtSimulation;

        mSimulations.remove(Core::canonicalFileName(pFileName));
    }
}

//==============================================================================

void SimulationManager::save(const QString &pFileName)
{
    // Check whether we are already managing the corresponding simulation and,
    // if so, save it

    Simulation *crtSimulation = simulation(pFileName);

    if (crtSimulation != nullptr) {
        crtSimulation->save();
    }
}

//==============================================================================

void SimulationManager::reload(const QString &pFileName)
{
    // Check whether we are already managing the corresponding simulation and,
    // if so, reload it

    Simulation *crtSimulation = simulation(pFileName);

    if (crtSimulation != nullptr) {
        crtSimulation->reload();
    }
}

//==============================================================================

void SimulationManager::rename(const QString &pOldFileName,
                               const QString &pNewFileName)
{
    // Check whether we are already managing the corresponding simulation and,
    // if so, rename it

    Simulation *crtSimulation = simulation(pOldFileName);

    if (crtSimulation != nullptr) {
        QString newFileName = Core::canonicalFileName(pNewFileName);

        mSimulations.insert(newFileName, crtSimulation);
        mSimulations.remove(Core::canonicalFileName(pOldFileName));

        crtSimulation->rename(newFileName);
    }
}

//==============================================================================

Simulation * SimulationManager::simulation(const QString &pFileName) const
{
    // Return the Simulation object, if any, associated with the given file

    return mSimulations.value(Core::canonicalFileName(pFileName));
}

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

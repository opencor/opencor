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
// Simulation manager
//==============================================================================

#include "corecliutils.h"
#include "simulation.h"
#include "simulationmanager.h"

//==============================================================================

#include "sbmlapibegin.h"
#include "sbmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

SimulationManager::SimulationManager() :
    mSimulations(QMap<QString, Simulation *>())
{
}

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

    QString nativeFileName = Core::nativeCanonicalFileName(pFileName);

    if (!simulation(nativeFileName))
        mSimulations.insert(nativeFileName, new Simulation(nativeFileName));
}

//==============================================================================

void SimulationManager::unmanage(const QString &pFileName)
{
    // Check whether we are already managing the corresponding simulation and,
    // if so, remove it from our list of managed simulations after having
    // deleted it

    Simulation *crtSimulation = simulation(pFileName);

    if (crtSimulation) {
        delete crtSimulation;

        mSimulations.remove(Core::nativeCanonicalFileName(pFileName));
    }
}

//==============================================================================

void SimulationManager::reload(const QString &pFileName)
{
    // Check whether we are already managing the corresponding simulation and,
    // if so, reload it

    Simulation *crtSimulation = simulation(pFileName);

    if (crtSimulation)
        crtSimulation->reload();
}

//==============================================================================

void SimulationManager::rename(const QString &pOldFileName,
                               const QString &pNewFileName)
{
    // Check whether we are already managing the corresponding simulation and,
    // if so, rename it

    Simulation *crtSimulation = simulation(pOldFileName);

    if (crtSimulation) {
        QString newNativeFileName = Core::nativeCanonicalFileName(pNewFileName);

        mSimulations.insert(newNativeFileName, crtSimulation);
        mSimulations.remove(Core::nativeCanonicalFileName(pOldFileName));

        crtSimulation->rename(newNativeFileName);
    }
}

//==============================================================================

Simulation * SimulationManager::simulation(const QString &pFileName) const
{
    // Return the Simulation object, if any, associated with the given file

    return mSimulations.value(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

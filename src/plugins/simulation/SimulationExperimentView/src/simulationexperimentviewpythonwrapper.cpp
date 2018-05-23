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
// Python wrapper for SimulationExperimentView classes
//==============================================================================

#include "centralwidget.h"
#include "coreguiutils.h"
#include "file.h"
#include "pythonqtsupport.h"
#include "simulation.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewpythonwrapper.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include <stdexcept>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SimulationSupport {
    class Simulation;
}   // namespace Simulationsupport

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

static SimulationSupport::Simulation *getSimulation(const QString &pFileName,
                                                    SimulationExperimentViewWidget *pSimulationExperimentViewWidget)
{
    SimulationSupport::Simulation *simulation = pSimulationExperimentViewWidget->simulation(pFileName);

    if (simulation) {
        // Let the simulation's widget know when we start running
        // Note: connect is also in the PythonQt namespace

        QObject::connect(simulation, &SimulationSupport::Simulation::runStarting,
                         pSimulationExperimentViewWidget, &SimulationExperimentViewWidget::startingRun);

        // Use the simulation's widget to clear our data

        QObject::connect(simulation, &SimulationSupport::Simulation::clearResults,
                         pSimulationExperimentViewWidget, &SimulationExperimentViewWidget::clearResults);
    }

    return simulation;
}

//==============================================================================

static PyObject *OpenCOR_simulation(PyObject *self,  PyObject *args)
{
    Q_UNUSED(self);
    Q_UNUSED(args);

    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();
    if (simulationExperimentViewWidget) {
        auto simulation = getSimulation(Core::centralWidget()->currentFileName(), simulationExperimentViewWidget);

        // Return the simulation as a Python object

        if (simulation)
            return PythonQt::priv()->wrapQObject(simulation);

    }

    Py_RETURN_NONE;
}

//==============================================================================

static PyMethodDef pythonSimulationExperimentViewMethods[] = {
    {"simulation",  OpenCOR_simulation, METH_VARARGS, "Current simulation."},
    {NULL, NULL, 0, NULL}
};

//==============================================================================

SimulationExperimentViewPythonWrapper::SimulationExperimentViewPythonWrapper(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    PyModule_AddFunctions(pModule, pythonSimulationExperimentViewMethods);
}

//==============================================================================
}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

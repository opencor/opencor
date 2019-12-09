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
// Simulation Experiment view Python wrapper
//==============================================================================

#include "centralwidget.h"
#include "corecliutils.h"
#include "filemanager.h"
#include "pythonqtsupport.h"
#include "simulation.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewpythonwrapper.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include <array>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

static PyObject * simulation(const QString &pFileName,
                             SimulationExperimentViewWidget *pSimulationExperimentViewWidget)
{
    // Return the simualtion, should there be one, for the given file using the
    // given Simulation Experiment view widget

    SimulationSupport::Simulation *simulation = pSimulationExperimentViewWidget->simulation(pFileName);

    if (simulation != nullptr) {
        // Check if something is wrong with our simulation and, if so, raise a
        // Python exception
        // Note: there may be several issues, but only the first one needs to be
        //       raised since Python obviously gives control back to the user as
        //       soon as an exception is raised...

        SimulationSupport::SimulationIssues simulationIssues = simulation->issues();

        if (!simulationIssues.isEmpty()) {
            auto simulationIssue = simulationIssues.first();

            if ((simulationIssue.line() != 0) && (simulationIssue.column() != 0)) {
                PyErr_SetString(PyExc_ValueError, qPrintable(QObject::tr("[%1:%2] %3: %4.").arg(simulationIssue.line())
                                                                                           .arg(simulationIssue.column())
                                                                                           .arg(simulationIssue.typeAsString(),
                                                                                                Core::formatMessage(simulationIssue.message().toHtmlEscaped()))));
            } else {
                PyErr_SetString(PyExc_ValueError, qPrintable(QObject::tr("%1: %2.").arg(simulationIssue.typeAsString(),
                                                                                        Core::formatMessage(simulationIssue.message().toHtmlEscaped()))));
            }

            return nullptr;
        }

        // Return our simulation wrapped into a Python object

        return PythonQt::priv()->wrapQObject(simulation);
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

static PyObject * simulation(PyObject *pSelf,  PyObject *pArgs)
{
    Q_UNUSED(pSelf)
    Q_UNUSED(pArgs)

    // Return the current simulation

    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();

    if (simulationExperimentViewWidget != nullptr) {
        return simulation(Core::currentFileName(), simulationExperimentViewWidget);
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

static PyObject * initializeSimulation(const QString &pFileName)
{
    // Initialise a simulation for the given file name

    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();

    if (simulationExperimentViewWidget != nullptr) {
        simulationExperimentViewWidget->initialize(pFileName);

        return simulation(pFileName, simulationExperimentViewWidget);
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

#define GUI_SUPPORT
    #include "opensimulation.cpp.inl"
#undef GUI_SUPPORT

//==============================================================================

#define GUI_SUPPORT
    #include "closesimulation.cpp.inl"
#undef GUI_SUPPORT

//==============================================================================

SimulationExperimentViewPythonWrapper::SimulationExperimentViewPythonWrapper(void *pModule,
                                                                             QObject *pParent) :
    QObject(pParent)
{
    // Add some Python wrappers

    static std::array<PyMethodDef, 4> PythonSimulationExperimentViewMethods = {{
                                                                                  { "open_simulation", openSimulation, METH_VARARGS, "Open a simulation." },
                                                                                  { "close_simulation", closeSimulation, METH_VARARGS, "Close a simulation." },
                                                                                  { "simulation",  simulation, METH_VARARGS, "The current simulation." },
                                                                                  { nullptr, nullptr, 0, nullptr }
                                                                              }};

    PyModule_AddFunctions(static_cast<PyObject *>(pModule),
                          PythonSimulationExperimentViewMethods.data());
}

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
    #pragma GCC diagnostic ignored "-Wold-style-cast"
    #pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#endif

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SimulationSupport {
    class Simulation;
}   // namespace Simulationsupport

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

static PyObject *getSimulation(const QString &pFileName,
                                                    SimulationExperimentViewWidget *pSimulationExperimentViewWidget)
{
    SimulationSupport::Simulation *simulation = pSimulationExperimentViewWidget->simulation(pFileName);

    if (simulation) {
        if (!simulation->runtime()) {
            // The simulation is missing a runtime so raise a Python exception

            PyErr_SetString(PyExc_ValueError, QObject::tr("unable to get simulations's runtime").toStdString().c_str());

            return NULL;
        }
        // Let the simulation's widget know when we start running
        // Note: connect is also in the PythonQt namespace

        QObject::connect(simulation, &SimulationSupport::Simulation::runStarting,
                         pSimulationExperimentViewWidget, &SimulationExperimentViewWidget::startingRun);

        // Use the simulation's widget to clear our data

        QObject::connect(simulation, &SimulationSupport::Simulation::clearResults,
                         pSimulationExperimentViewWidget, &SimulationExperimentViewWidget::clearResults);

        return PythonQt::priv()->wrapQObject(simulation);
    }

    Py_RETURN_NONE;
}

//==============================================================================

static PyObject *initializeSimulation(const QString &pFileName)
{
    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();

    if (simulationExperimentViewWidget) {
        simulationExperimentViewWidget->initialize(pFileName);

        return getSimulation(pFileName, simulationExperimentViewWidget);
    }

    Py_RETURN_NONE;
}

//==============================================================================

static PyObject *openSimulation(PyObject *self, PyObject *args)
{
    Q_UNUSED(self);

    PyObject *bytes;
    char *name;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "O&", PyUnicode_FSConverter, &bytes)) {
        Py_RETURN_NONE;
    }
    PyBytes_AsStringAndSize(bytes, &name, &len);
    QString fileName = QString::fromUtf8(name, len);
    Py_DECREF(bytes);

    QString ioError = Core::centralWidget()->openFile(fileName, Core::File::Type::Local,
                                                      QString(), false);

    if (!ioError.isEmpty()) {
        PyErr_SetString(PyExc_IOError, ioError.toStdString().c_str());

        return NULL;
    }

    return initializeSimulation(QFileInfo(fileName).canonicalFilePath());
}

//==============================================================================

static PyObject *openRemoteSimulation(PyObject *self, PyObject *args)
{
    Q_UNUSED(self);

    PyObject *bytes;
    char *name;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "O&", PyUnicode_FSConverter, &bytes)) {
        Py_RETURN_NONE;
    }
    PyBytes_AsStringAndSize(bytes, &name, &len);
    QString url = QString::fromUtf8(name, len);
    Py_DECREF(bytes);

    QString ioError = Core::centralWidget()->openRemoteFile(url, false);

    if (!ioError.isEmpty()) {
        PyErr_SetString(PyExc_IOError, ioError.toStdString().c_str());

        return NULL;
    }

    return initializeSimulation(Core::localFileName(url));
}

//==============================================================================

static PyObject *closeSimulation(PyObject *self, PyObject *args)
{
    Q_UNUSED(self);

    Py_ssize_t argc = PyTuple_Size(args);
    if (argc >  0) {
        PyObject *self = PyTuple_GET_ITEM(args, 0);

        if (PyObject_TypeCheck(self, &PythonQtInstanceWrapper_Type)) {
            PythonQtInstanceWrapper* wrap = (PythonQtInstanceWrapper *)self;

            // Get the wrapped simulation

            SimulationSupport::Simulation *simulation = (SimulationSupport::Simulation *)wrap->_objPointerCopy;

            // Close it by closing its file, raising an exception if we
            // are unable to do so

            if (!Core::centralWidget()->closeFile(simulation->fileName())) {
                PyErr_SetString(PyExc_IOError, "unable to close file");

                return NULL;
            }
        }
    }

    Py_RETURN_NONE;
}

//==============================================================================

static PyObject *OpenCOR_simulation(PyObject *self,  PyObject *args)
{
    Q_UNUSED(self);
    Q_UNUSED(args);

    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();
    if (simulationExperimentViewWidget) {

        return getSimulation(Core::centralWidget()->currentFileName(), simulationExperimentViewWidget);
    }

    Py_RETURN_NONE;
}

//==============================================================================

static PyMethodDef pythonSimulationExperimentViewMethods[] = {
    {"simulation",  OpenCOR_simulation, METH_VARARGS, "Current simulation."},
    {"openSimulation", openSimulation, METH_VARARGS, "Open a simulation."},
    {"openRemoteSimulation", openRemoteSimulation, METH_VARARGS, "Open a remote simulation."},
    {"closeSimulation", closeSimulation, METH_VARARGS, "Close a simulation."},
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

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

//==============================================================================
// End of file
//==============================================================================

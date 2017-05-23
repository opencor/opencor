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
// Python wrapper for Core plugin
//==============================================================================

#include "centralwidget.h"
#include "coreguiutils.h"
#include "pythonwrappercore.h"
#include "pythonwrapperplugin.h"
#include "simulationexperimentviewsimulation.h"
#include "simulationexperimentviewsimulationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace PythonWrapper {

//==============================================================================

static PyObject *initializeSimulation(const QString &pFileName)
{
    SimulationExperimentView::SimulationExperimentViewWidget *simulationExperimentViewWidget = PythonWrapperPlugin::instance()->simulationExperimentViewWidget();
    if (simulationExperimentViewWidget) {
        simulationExperimentViewWidget->initialize(pFileName);
        auto simulation = simulationExperimentViewWidget->simulation(pFileName);
        if (simulation) return PythonQt::priv()->wrapQObject(simulation);
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

    Core::centralWidget()->openFile(fileName);
    return initializeSimulation(fileName);
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

    Core::centralWidget()->openRemoteFile(url);
    return initializeSimulation(Core::centralWidget()->localFileName(url));
}

//==============================================================================

static PyMethodDef wrappedCoreMethods[] = {
    {"openSimulation",  openSimulation, METH_VARARGS, "Open a simulation."},
    {"openRemoteSimulation",  openRemoteSimulation, METH_VARARGS, "Open a remote simulation."},
    {NULL, NULL, 0, NULL}
};

//==============================================================================

PythonWrapperCore::PythonWrapperCore(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    PyModule_AddFunctions(pModule, wrappedCoreMethods);
}

//==============================================================================

}   // namespace PythonWrapper
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

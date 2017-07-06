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
#include "datastoreinterface.h"
#include "datastorepythonwrapper.h"
#include "pythonqtsupport.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewpythonwrapper.h"
#include "simulationexperimentviewsimulation.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

static PyObject *initializeSimulation(const QString &pFileName)
{
    SimulationExperimentView::SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();

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

static PyObject *OpenCOR_simulations(PyObject *self,  PyObject *args)
{
    Q_UNUSED(self);
    Q_UNUSED(args);

    PyObject *simulationDict = PyDict_New();
    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();

    if (simulationExperimentViewWidget) {
        foreach (const QString &fileName, simulationExperimentViewWidget->fileNames()) {
            auto simulation = simulationExperimentViewWidget->simulation(fileName);
            PythonQtSupport::addObject(simulationDict, fileName, simulation);
        }
    }

    return simulationDict;
}

//==============================================================================

static PyObject *OpenCOR_simulation(PyObject *self,  PyObject *args)
{
    Q_UNUSED(self);
    Q_UNUSED(args);

    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();
    if (simulationExperimentViewWidget) {
        auto simulation = simulationExperimentViewWidget->simulation(Core::centralWidget()->currentFileName());
        return PythonQtSupport::wrapQObject(simulation);
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

//==============================================================================

static PyMethodDef pythonSimulationExperimentViewMethods[] = {
    {"openSimulation",  openSimulation, METH_VARARGS, "Open a simulation."},
    {"openRemoteSimulation",  openRemoteSimulation, METH_VARARGS, "Open a remote simulation."},
    {"simulation",  OpenCOR_simulation, METH_VARARGS, "Current simulation."},
    {"simulations",  OpenCOR_simulations, METH_VARARGS, "Dictionary of simulations."},
    {NULL, NULL, 0, NULL}
};

//==============================================================================

SimulationExperimentViewPythonWrapper::SimulationExperimentViewPythonWrapper(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    PythonQtSupport::registerClass(&OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulation::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationData::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationResults::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationWidget::staticMetaObject);
    PythonQtSupport::addInstanceDecorators(this);

    PyModule_AddFunctions(pModule, pythonSimulationExperimentViewMethods);
}

//==============================================================================

SimulationExperimentViewSimulationWidget * SimulationExperimentViewPythonWrapper::widget(
    SimulationExperimentViewSimulation *pSimulationExperimentViewSimulation) const
{
    SimulationExperimentViewWidget *simulationExperimentViewWidget = SimulationExperimentViewPlugin::instance()->viewWidget();
    return simulationExperimentViewWidget->simulationWidget(pSimulationExperimentViewSimulation->fileName());
}

//==============================================================================

bool SimulationExperimentViewPythonWrapper::run(SimulationExperimentViewSimulation *pSimulationExperimentViewSimulation)
{
    // Check that we have enough memory to run our simulation

    bool runSimulation = true;

    double freeMemory = Core::freeMemory();
    double requiredMemory = pSimulationExperimentViewSimulation->requiredMemory();

    if (requiredMemory > freeMemory) {
        throw std::runtime_error(
            tr("The simulation requires %1 of memory and you have only %2 left.")
                .arg(Core::sizeAsString(requiredMemory), Core::sizeAsString(freeMemory)).toStdString());
    } else {
        // Theoretically speaking, we have enough memory to run the
        // simulation, so try to allocate all the memory we need for the
        // simulation by resetting its settings

        runSimulation = pSimulationExperimentViewSimulation->results()->reset();

        SimulationExperimentViewPlugin::instance()->viewWidget()->checkSimulationResults(pSimulationExperimentViewSimulation->fileName(), true);
        // Note: this will, among other things, clear our plots...

        // Effectively run our simulation in case we were able to
        // allocate all the memory we need to run the simulation

        if (runSimulation) {
            return pSimulationExperimentViewSimulation->run();
        } else {
            throw std::runtime_error(
                tr("We could not allocate the %1 of memory required for the simulation.")
                    .arg(Core::sizeAsString(requiredMemory)).toStdString());
        }
    }
    return false;
}

//==============================================================================

PyObject * SimulationExperimentViewPythonWrapper::constants(
    SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationExperimentViewSimulationData->mConstantVariables);
}

//==============================================================================

PyObject * SimulationExperimentViewPythonWrapper::rates(
    SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationExperimentViewSimulationData->mRateVariables);
}

//==============================================================================

PyObject * SimulationExperimentViewPythonWrapper::states(
    SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationExperimentViewSimulationData->mStateVariables);
}

//==============================================================================

PyObject * SimulationExperimentViewPythonWrapper::algebraic(
    SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationExperimentViewSimulationData->mAlgebraicVariables);
}

//==============================================================================

const OpenCOR::DataStore::DataStoreVariable * SimulationExperimentViewPythonWrapper::points(
    SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return pSimulationExperimentViewSimulationResults->mPointVariable;
}

//==============================================================================

PyObject * SimulationExperimentViewPythonWrapper::constants(
    SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationExperimentViewSimulationResults->mConstantVariables);
}

//==============================================================================

PyObject * SimulationExperimentViewPythonWrapper::rates(
    SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationExperimentViewSimulationResults->mRateVariables);
}

//==============================================================================

PyObject * SimulationExperimentViewPythonWrapper::states(
    SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationExperimentViewSimulationResults->mStateVariables);
}

//==============================================================================

PyObject * SimulationExperimentViewPythonWrapper::algebraic(
    SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationExperimentViewSimulationResults->mAlgebraicVariables);
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

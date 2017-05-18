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
#include "pythonwrapperplugin.h"
#include "pythonwrapperdatastore.h"
#include "pythonwrappersimulationexperimentview.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewsimulation.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include "PythonQt.h"

//==============================================================================

namespace OpenCOR {
namespace PythonWrapper {

//==============================================================================

static PyObject *OpenCOR_simulations(PyObject *self,  PyObject *args)
{
    Q_UNUSED(self);
    Q_UNUSED(args);

    PyObject *simulationDict = PyDict_New();
    SimulationExperimentView::SimulationExperimentViewWidget *simulationExperimentViewWidget = PythonWrapperPlugin::instance()->simulationExperimentViewWidget();

    if (simulationExperimentViewWidget) {
        foreach (const QString &fileName, simulationExperimentViewWidget->fileNames()) {
            auto simulation = simulationExperimentViewWidget->simulation(fileName);
            PythonQt::self()->addObject(simulationDict, fileName, simulation);
        }
    }

    return simulationDict;
}

//==============================================================================

PyObject *OpenCOR_simulation(PyObject *self,  PyObject *args)
{
    Q_UNUSED(self);
    Q_UNUSED(args);

    SimulationExperimentView::SimulationExperimentViewWidget *simulationExperimentViewWidget = PythonWrapperPlugin::instance()->simulationExperimentViewWidget();
    if (simulationExperimentViewWidget) {
        auto simulation = simulationExperimentViewWidget->simulation(Core::centralWidget()->currentFileName());
        return PythonQt::priv()->wrapQObject(simulation);
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

//==============================================================================

static PyMethodDef wrappedSimulationExperimentViewMethods[] = {
    {"simulation",  OpenCOR_simulation, METH_VARARGS, "Current simulation."},
    {"simulations",  OpenCOR_simulations, METH_VARARGS, "Dictionary of simulations."},
    {NULL, NULL, 0, NULL}
};

//==============================================================================

PythonWrapperSimulationExperimentView::PythonWrapperSimulationExperimentView(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    PythonQt::self()->registerClass(&OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulation::staticMetaObject);
    PythonQt::self()->registerClass(&OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationData::staticMetaObject);
    PythonQt::self()->registerClass(&OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationResults::staticMetaObject);
    PythonQt::self()->registerClass(&OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationWidget::staticMetaObject);
    PythonQt::self()->addInstanceDecorators(this);

    PyModule_AddFunctions(pModule, wrappedSimulationExperimentViewMethods);
}

//==============================================================================

SimulationExperimentView::SimulationExperimentViewSimulationWidget * PythonWrapperSimulationExperimentView::widget(SimulationExperimentView::SimulationExperimentViewSimulation *pSimulationExperimentViewSimulation) const
{
    auto simulationExperimentViewWidget = PythonWrapperPlugin::instance()->simulationExperimentViewWidget();
    return simulationExperimentViewWidget->simulationWidget(pSimulationExperimentViewSimulation->fileName());
}

//==============================================================================

bool PythonWrapperSimulationExperimentView::run(SimulationExperimentView::SimulationExperimentViewSimulation *pSimulationExperimentViewSimulation)
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

        PythonWrapperPlugin::instance()->simulationExperimentViewWidget()->checkSimulationResults(pSimulationExperimentViewSimulation->fileName(), true);
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

PyObject * PythonWrapperSimulationExperimentView::constants(
    SimulationExperimentView::SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const
{
    return PythonWrapperDataStore::dataStoreValuesDict(pSimulationExperimentViewSimulationData->mConstantVariables);
}

//==============================================================================

PyObject * PythonWrapperSimulationExperimentView::rates(
    SimulationExperimentView::SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const
{
    return PythonWrapperDataStore::dataStoreValuesDict(pSimulationExperimentViewSimulationData->mRateVariables);
}

//==============================================================================

PyObject * PythonWrapperSimulationExperimentView::states(
    SimulationExperimentView::SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const
{
    return PythonWrapperDataStore::dataStoreValuesDict(pSimulationExperimentViewSimulationData->mStateVariables);
}

//==============================================================================

PyObject * PythonWrapperSimulationExperimentView::algebraic(
    SimulationExperimentView::SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const
{
    return PythonWrapperDataStore::dataStoreValuesDict(pSimulationExperimentViewSimulationData->mAlgebraicVariables);
}

//==============================================================================

const OpenCOR::DataStore::DataStoreVariable * PythonWrapperSimulationExperimentView::points(
    SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return pSimulationExperimentViewSimulationResults->mPointVariable;
}

//==============================================================================

PyObject * PythonWrapperSimulationExperimentView::constants(
    SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return PythonWrapperDataStore::dataStoreVariablesDict(pSimulationExperimentViewSimulationResults->mConstantVariables);
}

//==============================================================================

PyObject * PythonWrapperSimulationExperimentView::rates(
    SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return PythonWrapperDataStore::dataStoreVariablesDict(pSimulationExperimentViewSimulationResults->mRateVariables);
}

//==============================================================================

PyObject * PythonWrapperSimulationExperimentView::states(
    SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return PythonWrapperDataStore::dataStoreVariablesDict(pSimulationExperimentViewSimulationResults->mStateVariables);
}

//==============================================================================

PyObject * PythonWrapperSimulationExperimentView::algebraic(
    SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const
{
    return PythonWrapperDataStore::dataStoreVariablesDict(pSimulationExperimentViewSimulationResults->mAlgebraicVariables);
}

//==============================================================================

}   // namespace PythonWrapper
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Python wrapper for SingleCellView classes
//==============================================================================

#include "centralwidget.h"
#include "coreguiutils.h"
#include "pythonwrapperplugin.h"
#include "pythonwrapperdatastore.h"
#include "pythonwrappersinglecellview.h"
#include "singlecellviewplugin.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewsimulationwidget.h"
#include "singlecellviewwidget.h"

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
    SingleCellView::SingleCellViewWidget *singleCellViewWidget = PythonWrapperPlugin::instance()->singleCellViewWidget();

    if (singleCellViewWidget) {
        foreach (const QString &fileName, singleCellViewWidget->fileNames()) {
            auto simulation = singleCellViewWidget->simulation(fileName);
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

    SingleCellView::SingleCellViewWidget *singleCellViewWidget = PythonWrapperPlugin::instance()->singleCellViewWidget();
    if (singleCellViewWidget) {
        auto simulation = singleCellViewWidget->simulation(Core::centralWidget()->currentFileName());
        return PythonQt::priv()->wrapQObject(simulation);
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

//==============================================================================

static PyMethodDef wrappedSingleCellViewMethods[] = {
    {"simulation",  OpenCOR_simulation, METH_VARARGS, "Current simulation."},
    {"simulations",  OpenCOR_simulations, METH_VARARGS, "Dictionary of simulations."},
    {NULL, NULL, 0, NULL}
};

//==============================================================================

PythonWrapperSingleCellView::PythonWrapperSingleCellView(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    PythonQt::self()->registerClass(&OpenCOR::SingleCellView::SingleCellViewSimulation::staticMetaObject);
    PythonQt::self()->registerClass(&OpenCOR::SingleCellView::SingleCellViewSimulationData::staticMetaObject);
    PythonQt::self()->registerClass(&OpenCOR::SingleCellView::SingleCellViewSimulationResults::staticMetaObject);
    PythonQt::self()->registerClass(&OpenCOR::SingleCellView::SingleCellViewSimulationWidget::staticMetaObject);
    PythonQt::self()->addInstanceDecorators(this);

    PyModule_AddFunctions(pModule, wrappedSingleCellViewMethods);
}

//==============================================================================

SingleCellView::SingleCellViewSimulationWidget * PythonWrapperSingleCellView::widget(SingleCellView::SingleCellViewSimulation *pSingleCellViewSimulation) const
{
    auto singleCellViewWidget = PythonWrapperPlugin::instance()->singleCellViewWidget();
    return singleCellViewWidget->simulationWidget(pSingleCellViewSimulation->fileName());
}

//==============================================================================

bool PythonWrapperSingleCellView::run(SingleCellView::SingleCellViewSimulation *pSingleCellViewSimulation)
{
    // Check that we have enough memory to run our simulation

    bool runSimulation = true;

    double freeMemory = Core::freeMemory();
    double requiredMemory = pSingleCellViewSimulation->requiredMemory();

    if (requiredMemory > freeMemory) {
        Core::warningMessageBox(tr("Run Simulation"),
                                tr("The simulation requires %1 of memory and you have only %2 left.").arg(Core::sizeAsString(requiredMemory), Core::sizeAsString(freeMemory)));
    } else {
        // Theoretically speaking, we have enough memory to run the
        // simulation, so try to allocate all the memory we need for the
        // simulation by resetting its settings

        runSimulation = pSingleCellViewSimulation->results()->reset();

        PythonWrapperPlugin::instance()->singleCellViewWidget()->checkSimulationResults(pSingleCellViewSimulation->fileName(), true);
        // Note: this will, among other things, clear our plots...

        // Effectively run our simulation in case we were able to
        // allocate all the memory we need to run the simulation

        if (runSimulation) {
            return pSingleCellViewSimulation->run();
        } else {
            Core::warningMessageBox(tr("Run Simulation"),
                                    tr("We could not allocate the %1 of memory required for the simulation.").arg(Core::sizeAsString(requiredMemory)));
        }
    }
    return false;
}

//==============================================================================

const OpenCOR::DataStore::DataStoreVariable * PythonWrapperSingleCellView::points(
    SingleCellView::SingleCellViewSimulationResults *pSingleCellViewSimulationResults) const
{
    return pSingleCellViewSimulationResults->mPointVariable;
}

//==============================================================================

PyObject * PythonWrapperSingleCellView::constants(
    SingleCellView::SingleCellViewSimulationResults *pSingleCellViewSimulationResults) const
{
    return PythonWrapperDataStore::dataStoreVariablesDict(pSingleCellViewSimulationResults->mConstantVariables);
}

//==============================================================================

PyObject * PythonWrapperSingleCellView::rates(
    SingleCellView::SingleCellViewSimulationResults *pSingleCellViewSimulationResults) const
{
    return PythonWrapperDataStore::dataStoreVariablesDict(pSingleCellViewSimulationResults->mRateVariables);
}

//==============================================================================

PyObject * PythonWrapperSingleCellView::states(
    SingleCellView::SingleCellViewSimulationResults *pSingleCellViewSimulationResults) const
{
    return PythonWrapperDataStore::dataStoreVariablesDict(pSingleCellViewSimulationResults->mStateVariables);
}

//==============================================================================

PyObject * PythonWrapperSingleCellView::algebraic(
    SingleCellView::SingleCellViewSimulationResults *pSingleCellViewSimulationResults) const
{
    return PythonWrapperDataStore::dataStoreVariablesDict(pSingleCellViewSimulationResults->mAlgebraicVariables);
}

//==============================================================================

}   // namespace PythonWrapper
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

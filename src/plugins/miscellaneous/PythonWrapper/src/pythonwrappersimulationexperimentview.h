
/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Python wrapper for SimulationExperimentView classes
//==============================================================================

#pragma once

//==============================================================================

#include "PythonQtPythonInclude.h"

//==============================================================================

#include <QList>
#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace DataStore {
    class DataStoreVariable;
    class DataStoreVariables;
}

namespace SimulationExperimentView {
    class SimulationExperimentViewSimulation;
    class SimulationExperimentViewSimulationData;
    class SimulationExperimentViewSimulationResults;
    class SimulationExperimentViewSimulationWidget;
};

//==============================================================================

namespace PythonWrapper {

//==============================================================================

class PythonWrapperSimulationExperimentView : public QObject
{
    Q_OBJECT

public:
    explicit PythonWrapperSimulationExperimentView(PyObject *pModule, QObject *pParent=0);

public slots:
    bool run(OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulation *pSimulationExperimentViewSimulation);

    OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationWidget * widget(OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulation *pSimulationExperimentViewSimulation) const;

    // Access a simulation's parameter data

    PyObject * constants(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const;
    PyObject * rates(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const;
    PyObject * states(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const;
    PyObject * algebraic(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationData *pSimulationExperimentViewSimulationData) const;

    // Access a simulation's result data

    const OpenCOR::DataStore::DataStoreVariable * points(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const;
    PyObject * constants(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const;
    PyObject * rates(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const;
    PyObject * states(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const;
    PyObject * algebraic(
        OpenCOR::SimulationExperimentView::SimulationExperimentViewSimulationResults *pSimulationExperimentViewSimulationResults) const;
};

//==============================================================================

}   // namespace PythonWrapper
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================


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

#pragma once

//==============================================================================

#include <PythonQt/PythonQtPythonInclude.h>

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


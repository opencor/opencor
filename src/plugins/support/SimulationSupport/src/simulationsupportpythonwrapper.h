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
// Simulation support Python wrapper
//==============================================================================

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

#include "pythonbegin.h"
    #include "PythonQt/PythonQtPythonInclude.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace DataStore {
    class DataStore;
    class DataStoreValue;
    class DataStoreVariable;
} // namespace DataStore

//==============================================================================

namespace SimulationSupport {

//==============================================================================

class Simulation;
class SimulationData;
class SimulationResults;

//==============================================================================

class SimulationSupportPythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit SimulationSupportPythonWrapper(void *pModule, QObject *pParent);

private:
    qint64 mElapsedTime = -1;
    QString mErrorMessage;

public slots:
    bool valid(OpenCOR::SimulationSupport::Simulation *pSimulation);

    bool run(OpenCOR::SimulationSupport::Simulation *pSimulation);

    void reset(OpenCOR::SimulationSupport::Simulation *pSimulation,
               bool pAll = true);
    void clear_results(OpenCOR::SimulationSupport::Simulation *pSimulation);

    PyObject * issues(OpenCOR::SimulationSupport::Simulation *pSimulation) const;

    double starting_point(OpenCOR::SimulationSupport::SimulationData *pSimulationData);
    void set_starting_point(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                            double pStartingPoint);

    double ending_point(OpenCOR::SimulationSupport::SimulationData *pSimulationData);
    void set_ending_point(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                          double pEndingPoint);

    double point_interval(OpenCOR::SimulationSupport::SimulationData *pSimulationData);
    void set_point_interval(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                            double pPointInterval);

    QString ode_solver_name(OpenCOR::SimulationSupport::SimulationData *pSimulationData);
    void set_ode_solver(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                        const QString &pName);

    QVariant ode_solver_property(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                                 const QString &pName);
    void set_ode_solver_property(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                                 const QString &pName, const QVariant &pValue);

    QString nla_solver_name(OpenCOR::SimulationSupport::SimulationData *pSimulationData);
    void set_nla_solver(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                        const QString &pName);

    QVariant nla_solver_property(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                                 const QString &pName);
    void set_nla_solver_property(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                                 const QString &pName, const QVariant &pValue);

    PyObject * constants(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * rates(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * states(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * algebraic(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;

    OpenCOR::DataStore::DataStore * data_store(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;

    OpenCOR::DataStore::DataStoreVariable * voi(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;

    PyObject * constants(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * states(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * rates(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * algebraic(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;

    void set_value(OpenCOR::DataStore::DataStoreValue *pDataStoreValue,
                   double pValue);

    int runs_count(OpenCOR::DataStore::DataStoreVariable *pDataStoreVariable) const;
    quint64 values_count(OpenCOR::DataStore::DataStoreVariable *pDataStoreVariable,
                         int pRun = -1) const;

private slots:
    void simulationError(const QString &pErrorMessage);
    void simulationDone(qint64 pElapsedTime);
};

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================


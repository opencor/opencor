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
    void clearResults(OpenCOR::SimulationSupport::Simulation *pSimulation);

    PyObject * issues(OpenCOR::SimulationSupport::Simulation *pSimulation) const;

    void setStartingPoint(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                          double pStartingPoint, bool pRecompute = true);
    void setEndingPoint(OpenCOR::SimulationSupport::SimulationData *pSimulationData, double pEndingPoint);
    void setPointInterval(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                          double pPointInterval);

    void setOdeSolver(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                      const QString &pOdeSolverName);
    void setNlaSolver(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                      const QString &pNlaSolverName);

    PyObject * constants(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * rates(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * states(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * algebraic(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;

    OpenCOR::DataStore::DataStoreVariable * points(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;

    PyObject * constants(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * states(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * rates(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * algebraic(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;

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


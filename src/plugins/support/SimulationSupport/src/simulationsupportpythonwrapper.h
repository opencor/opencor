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
// Python wrapper for SimulationSupport classes
//==============================================================================

#pragma once

//==============================================================================

#include "pythoninclude.h"

//==============================================================================

#include <QEventLoop>
#include <QList>
#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace DataStore {
    class DataStoreVariable;
}

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
    explicit SimulationSupportPythonWrapper(PyObject *pModule, QObject *pParent=0);

private:
    qint64 mElapsedTime;
    QString mErrorMessage;
    QEventLoop *mSimulationRunEventLoop;

public slots:
    bool reset(OpenCOR::SimulationSupport::Simulation *pSimulationSupportSimulation);

    bool run(OpenCOR::SimulationSupport::Simulation *pSimulationSupportSimulation);

    // Set a simulation's point data

    void setStartingPoint(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                          const double &pStartingPoint,
                          const bool &pRecompute = true);
    void setEndingPoint(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                        const double &pEndingPoint);
    void setPointInterval(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                          const double &pPointInterval);

    // Access a simulation's parameter data

    PyObject * algebraic(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * constants(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * rates(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;
    PyObject * states(OpenCOR::SimulationSupport::SimulationData *pSimulationData) const;

    // Access a simulation's result data

    const OpenCOR::DataStore::DataStoreVariable * points(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;

    PyObject * algebraic(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * constants(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * states(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;
    PyObject * rates(OpenCOR::SimulationSupport::SimulationResults *pSimulationResults) const;

    // Access a simulation's sensitivity gradients

    PyObject * gradients(OpenCOR::SimulationSupport::SimulationResults *pSimulationData) const;

private slots:
    void error(const QString &pErrorMessage);
    void simulationFinished(const qint64 &pElapsedTime);
};

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================


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

#include <QList>
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
}

//==============================================================================

namespace SimulationSupport {

//==============================================================================

class Simulation;
class SimulationData;
class SimulationResults;

//==============================================================================

// We bind the SimulationData object to the the first parameter of `updateParameters()`
// to create a function object to be called when simulation parameters are updated

#if defined(_MSC_VER)
    #if _MSC_VER > 1900
        // Visual Studio 2017
        typedef std::_Binder<std::_Unforced, void (*)(SimulationData *), SimulationData *> SimulationDataUpdatedFunction;
    #else
        // Visual Studio 2015
        typedef std::_Binder<std::_Unforced, void (*)(SimulationData *), SimulationData * const> SimulationDataUpdatedFunction;
    #endif
#elif defined(__APPLE__)
    typedef std::__bind<void (*)(SimulationData *), SimulationData *> SimulationDataUpdatedFunction;
#else
    typedef std::_Bind_helper<false, void (*)(SimulationData *), SimulationData *>::type SimulationDataUpdatedFunction;
#endif

//==============================================================================

class SimulationSupportPythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit SimulationSupportPythonWrapper(PyObject *pModule,
                                            QObject *pParent);

private:
    qint64 mElapsedTime;
    QString mErrorMessage;

public slots:
    bool valid(OpenCOR::SimulationSupport::Simulation *pSimulation);

    PyObject * issues(OpenCOR::SimulationSupport::Simulation *pSimulation) const;

    void clearResults(OpenCOR::SimulationSupport::Simulation *pSimulation);

    void resetParameters(OpenCOR::SimulationSupport::Simulation *pSimulation);

    bool run(OpenCOR::SimulationSupport::Simulation *pSimulation);

    // Set a simulation's point data

    void setStartingPoint(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                          const double &pStartingPoint,
                          const bool &pRecompute = true);
    void setEndingPoint(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                        const double &pEndingPoint);
    void setPointInterval(OpenCOR::SimulationSupport::SimulationData *pSimulationData,
                          const double &pPointInterval);

    // Assign a solver

    void setOdeSolver(OpenCOR::SimulationSupport::SimulationData *pSimulationData, const QString &pOdeSolverName);
    void setNlaSolver(OpenCOR::SimulationSupport::SimulationData *pSimulationData, const QString &pNlaSolverName);

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

signals:
    void gotElapsedTime();
};

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================


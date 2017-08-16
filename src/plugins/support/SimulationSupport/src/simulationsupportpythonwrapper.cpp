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

#include "corecliutils.h"
#include "datastoreinterface.h"
#include "datastorepythonwrapper.h"
#include "pythonsupport.h"
#include "simulation.h"
#include "simulationsupportplugin.h"
#include "simulationsupportpythonwrapper.h"

//==============================================================================

#include <stdexcept>

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

SimulationSupportPythonWrapper::SimulationSupportPythonWrapper(PyObject *pModule, QObject *pParent) :
    QObject(pParent),
    mElapsedTime(-1),
    mErrorMessage(QString()),
    mSimulationRunEventLoop(new QEventLoop())
{
    Q_UNUSED(pModule);

    PythonSupport::registerClass(&OpenCOR::SimulationSupport::Simulation::staticMetaObject);
    PythonSupport::registerClass(&OpenCOR::SimulationSupport::SimulationData::staticMetaObject);
    PythonSupport::registerClass(&OpenCOR::SimulationSupport::SimulationResults::staticMetaObject);
    PythonSupport::addInstanceDecorators(this);
}

//==============================================================================

void SimulationSupportPythonWrapper::error(const QString &pErrorMessage)
{
    mErrorMessage = pErrorMessage;
}

//==============================================================================

void SimulationSupportPythonWrapper::simulationFinished(const qint64 &pElapsedTime)
{
    mElapsedTime = pElapsedTime;

    QMetaObject::invokeMethod(mSimulationRunEventLoop, "quit", Qt::QueuedConnection);
}

//==============================================================================

bool SimulationSupportPythonWrapper::run(Simulation *pSimulation)
{
    // A successful run will set elapsed time

    mElapsedTime = -1;

    // Clear error message

    mErrorMessage = QString();

    // Try to allocate all the memory we need for the simulation by
    // resetting its settings

    bool runSimulation = pSimulation->results()->reset();

    // Run our simulation in case we were able to allocate all the
    // memory we need to run the simulation

    if (runSimulation) {
        // Signal our event loop when the simulation has finished

        connect(pSimulation, SIGNAL(stopped(const qint64 &)), this, SLOT(simulationFinished(const qint64 &)));

        // Get error messages from the simulation

        connect(pSimulation, SIGNAL(error(const QString &)), this, SLOT(error(const QString &)));

        // Start the simulation and wait for it to complete

        if (pSimulation->run())
            mSimulationRunEventLoop->exec();

        if (!mErrorMessage.isEmpty())
            throw std::runtime_error(mErrorMessage.toStdString());
    } else {
        throw std::runtime_error(
            tr("We could not allocate the memory required for the simulation.").toStdString());
    }

    return mElapsedTime >= 0;
}

//==============================================================================

void SimulationSupportPythonWrapper::setStartingPoint(SimulationData *pSimulationData,
    const double &pStartingPoint, const bool &pRecompute)
{
    pSimulationData->setStartingPoint(pStartingPoint, pRecompute);

    emit pSimulationData->updatedSimulation();
}

//==============================================================================

void SimulationSupportPythonWrapper::setEndingPoint(SimulationData *pSimulationData, const double &pEndingPoint)
{
    pSimulationData->setEndingPoint(pEndingPoint);

    emit pSimulationData->updatedSimulation();
}

//==============================================================================

void SimulationSupportPythonWrapper::setPointInterval(SimulationData *pSimulationData, const double &pPointInterval)
{
    pSimulationData->setPointInterval(pPointInterval);

    emit pSimulationData->updatedSimulation();
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::algebraic(SimulationData *pSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationData->algebraicVariables(),
        &(pSimulationData->mSimulationDataUpdatedFunction));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::constants(SimulationData *pSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationData->constantVariables(),
        &(pSimulationData->mSimulationDataUpdatedFunction));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::rates(SimulationData *pSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationData->rateVariables(),
        &(pSimulationData->mSimulationDataUpdatedFunction));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::states(SimulationData *pSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationData->stateVariables(),
        &(pSimulationData->mSimulationDataUpdatedFunction));
}

//==============================================================================

const OpenCOR::DataStore::DataStoreVariable * SimulationSupportPythonWrapper::points(SimulationResults *pSimulationResults) const
{
    return pSimulationResults->mPointVariable;
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::algebraic(SimulationResults *pSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->mAlgebraicVariables);
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::constants(SimulationResults *pSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->mConstantVariables);
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::rates(SimulationResults *pSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->mRateVariables);
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::states(SimulationResults *pSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->mStateVariables);
}

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
#include "cellmlfileruntime.h"
#include "datastoreinterface.h"
#include "datastorepythonwrapper.h"
#include "pythonqtsupport.h"
#include "simulation.h"
#include "simulationmanager.h"
#include "simulationsupportplugin.h"
#include "simulationsupportpythonwrapper.h"

//==============================================================================

#include <QApplication>
#include <QEventLoop>
#include <QMap>
#include <QWidget>

//==============================================================================

#include <stdexcept>

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

static PyObject *initializeSimulation(const QString &pFileName)
{
    // Ask our simulation manager to manage our file and then retrieve the
    // corresponding simulation from it

    SimulationManager *simulationManager = SimulationManager::instance();

    simulationManager->manage(pFileName);

    Simulation *simulation = simulationManager->simulation(pFileName);

    if (simulation) {
        // TODO: we need to check that the simulation is valid and
        //       raise an exception if not

        // Reset both the simulation's data and results (well, initialise in the
        // case of its data)

        simulation->data()->reset();
        simulation->results()->reset();

        return PythonQt::priv()->wrapQObject(simulation);
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

    QString ioError = Core::centralWidget()->openFile(fileName, Core::File::Local,
                                                      QString(), false);
    if (!ioError.isEmpty()) {
        PyErr_SetString(PyExc_IOError, ioError.toStdString().c_str());
        return NULL;
    } else {
        return initializeSimulation(QFileInfo(fileName).canonicalFilePath());
    }
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

    QString ioError = Core::centralWidget()->openRemoteFile(url, false); // No warning...
    if (!ioError.isEmpty()) {
        PyErr_SetString(PyExc_IOError, ioError.toStdString().c_str());
        return NULL;
    }

    return initializeSimulation(Core::centralWidget()->localFileName(url));
}

//==============================================================================

SimulationSupportPythonWrapper::SimulationSupportPythonWrapper(PyObject *pModule, QObject *pParent) :
    QObject(pParent),
    mElapsedTime(-1),
    mErrorMessage(QString())
{
    Q_UNUSED(pModule);

    PythonQtSupport::registerClass(&OpenCOR::SimulationSupport::Simulation::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::SimulationSupport::SimulationData::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::SimulationSupport::SimulationResults::staticMetaObject);
    PythonQtSupport::addInstanceDecorators(this);
}

//==============================================================================

void SimulationSupportPythonWrapper::error(const QString &pErrorMessage)
{
    mErrorMessage = pErrorMessage;
}

//==============================================================================

void SimulationSupportPythonWrapper::simulationFinished(const qint64 &pElapsedTime)
{
    // Save the elapsed time of the simulation

    mElapsedTime = pElapsedTime;

    emit gotElapsedTime();
}

//==============================================================================

void SimulationSupportPythonWrapper::clearResults(Simulation *pSimulation)
{
    // Ask our widget to clear our results

    // Note: we get the widget to do this as it needs to clear all
    // associated graphs...

    emit pSimulation->clearResults(pSimulation->fileName());
}

//==============================================================================

void SimulationSupportPythonWrapper::resetParameters(Simulation *pSimulation)
{
    // Reset our model parameters

    pSimulation->reset();
}

//==============================================================================

bool SimulationSupportPythonWrapper::run(Simulation *pSimulation)
{
    QWidget *focusWidget = 0;

    // A successful run will set elapsed time

    mElapsedTime = -1;

    // Clear error message

    mErrorMessage = QString();

    // Try to allocate all the memory we need by adding a run to our
    // simulation

    bool runSimulation = pSimulation->addRun();

    // Run our simulation (after having added a run to our graphs), in
    // case we were able to allocate all the memory we need

    if (runSimulation) {
        // Save the keyboard focus, which will be to our IPython console

        focusWidget = QApplication::focusWidget();

        // Let the simulation widget know we are starting

        emit pSimulation->runStarting(pSimulation->fileName());

        // Get the elapsed time when the simulation has finished

        connect(pSimulation, &Simulation::stopped,
                this, &SimulationSupportPythonWrapper::simulationFinished, Qt::QueuedConnection);

        // Get error messages from the simulation

        connect(pSimulation, &Simulation::error,
                this, &SimulationSupportPythonWrapper::error);

        // Use an event loop so we don't busy wait

        QEventLoop waitForCompletion;

        connect(this, &SimulationSupportPythonWrapper::gotElapsedTime,
                &waitForCompletion, &QEventLoop::quit);

        // Start the simulation and wait for it to complete

        if (pSimulation->run())
            waitForCompletion.exec();

        // Disconnect our signal handlers now that the simulation has finished

        disconnect(pSimulation, 0, this, 0);

        if (!mErrorMessage.isEmpty())
            throw std::runtime_error(mErrorMessage.toStdString());
    } else {
        throw std::runtime_error(
            tr("We could not allocate the memory required for the simulation.").toStdString());
    }

    // Restore the keyboard focus back to IPython

    if (focusWidget)
        focusWidget->setFocus();

    return mElapsedTime >= 0;
}

//==============================================================================

void SimulationSupportPythonWrapper::setStartingPoint(SimulationData *pSimulationData,
    const double &pStartingPoint, const bool &pRecompute)
{
    pSimulationData->setStartingPoint(pStartingPoint, pRecompute);

    emit pSimulationData->updatedPointData();
}

//==============================================================================

void SimulationSupportPythonWrapper::setEndingPoint(SimulationData *pSimulationData, const double &pEndingPoint)
{
    pSimulationData->setEndingPoint(pEndingPoint);

    emit pSimulationData->updatedPointData();
}

//==============================================================================

void SimulationSupportPythonWrapper::setPointInterval(SimulationData *pSimulationData, const double &pPointInterval)
{
    pSimulationData->setPointInterval(pPointInterval);

    emit pSimulationData->updatedPointData();
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::algebraic(SimulationData *pSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(
        pSimulationData->algebraicValues(),
        &(pSimulationData->mSimulationDataUpdatedFunction));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::constants(SimulationData *pSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(
        pSimulationData->constantsValues(),
        &(pSimulationData->mSimulationDataUpdatedFunction));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::rates(SimulationData *pSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(
        pSimulationData->ratesValues(),
        &(pSimulationData->mSimulationDataUpdatedFunction));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::states(SimulationData *pSimulationData) const
{
    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(
        pSimulationData->statesValues(),
        &(pSimulationData->mSimulationDataUpdatedFunction));
}

//==============================================================================

const OpenCOR::DataStore::DataStoreVariable * SimulationSupportPythonWrapper::points(
    SimulationResults *pSimulationResults) const
{
    return pSimulationResults->mPoints;
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::algebraic(
    SimulationResults *pSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->mAlgebraic);
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::constants(
    SimulationResults *pSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->mConstants);
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::rates(
    SimulationResults *pSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->mRates);
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::states(
    SimulationResults *pSimulationResults) const
{
    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->mStates);
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::gradients(SimulationResults *pSimulationResults) const
{
    SimulationData *simulationData = pSimulationResults->mSimulation->data();

    const DataStore::DataStoreVariables constantVariables = pSimulationResults->mConstants;

    const DataStore::DataStoreVariables stateVariables = pSimulationResults->mStates;
    int statesCount = stateVariables.size();

    const DataStore::DataStoreVariables gradientVariables = pSimulationResults->mGradients;
    int gradientsCount = gradientVariables.size()/statesCount;

    int *indices = simulationData->gradientIndices();

    PyObject *gradientsDict = PyDict_New();
    QMap<QString, PyObject *> stateGradientsDictionaries;

    // We need to transpose gradients when building dictionary

    for (int i = 0; i < gradientsCount; ++i) {
        DataStore::DataStoreVariable *constant = constantVariables[indices[i]];

        for (int j = 0; j < statesCount; ++j) {
            DataStore::DataStoreVariable *state = stateVariables[j];
            DataStore::DataStoreVariable *gradient = gradientVariables[i*statesCount + j];

            // Each state variable has a dictionary containing gradients wrt each constant

            PyObject *stateGradientsDict = stateGradientsDictionaries[state->uri()];
            if (stateGradientsDict == 0) {
                stateGradientsDict = PyDict_New();
                PyDict_SetItemString(gradientsDict, state->uri().toLatin1().data(), stateGradientsDict);
                stateGradientsDictionaries[state->uri()] = stateGradientsDict;
            }

            PythonQtSupport::addObject(stateGradientsDict, constant->uri(), gradient);
        }
    }
    return gradientsDict;
}

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

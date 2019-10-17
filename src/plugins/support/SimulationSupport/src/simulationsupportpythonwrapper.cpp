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
#include "filemanager.h"
#include "interfaces.h"
#include "simulation.h"
#include "simulationmanager.h"
#include "simulationsupportplugin.h"
#include "solverinterface.h"

//==============================================================================

#include <QApplication>
#include <QEventLoop>
#include <QFileInfo>
#include <QMap>
#include <QWidget>

//==============================================================================

#include "pythonbegin.h"

#include "datastorepythonwrapper.h"
#include "pythonqtsupport.h"
#include "simulationsupportpythonwrapper.h"

//==============================================================================

#include <stdexcept>

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

static void setOdeSolver_(SimulationData *pSimulationData, const QString &pOdeSolverName)
{
    foreach (SolverInterface *solverInterface, Core::solverInterfaces()) {
        if (!pOdeSolverName.compare(solverInterface->solverName())) {
            // Set the ODE solver's name

            pSimulationData->setOdeSolverName(pOdeSolverName);

            foreach (const Solver::Property &solverInterfaceProperty,
                     solverInterface->solverProperties()) {
                // Set each ODE solver property's default value

                pSimulationData->addOdeSolverProperty(solverInterfaceProperty.id(), solverInterfaceProperty.defaultValue());
            }

            return;
        }
    }
    throw std::runtime_error(QObject::tr("Unknown ODE solver.").toStdString());
}

//==============================================================================

static void setNlaSolver_(SimulationData *pSimulationData, const QString &pNlaSolverName)
{
    foreach (SolverInterface *solverInterface, Core::solverInterfaces()) {
        if (!pNlaSolverName.compare(solverInterface->solverName())) {
            // Set the NLA solver's name

            pSimulationData->setNlaSolverName(pNlaSolverName);

            foreach (const Solver::Property &solverInterfaceProperty,
                     solverInterface->solverProperties()) {
                // Set each NLA solver property's default value

                pSimulationData->addNlaSolverProperty(solverInterfaceProperty.id(), solverInterfaceProperty.defaultValue());
            }

            return;
        }
    }
    throw std::runtime_error(QObject::tr("Unknown NLA solver.").toStdString());
}

//==============================================================================

static PyObject *initializeSimulation(const QString &pFileName)
{
    // Ask our simulation manager to manage our file and then retrieve the
    // corresponding simulation from it

    SimulationManager *simulationManager = SimulationManager::instance();

    simulationManager->manage(pFileName);

    Simulation *simulation = simulationManager->simulation(pFileName);

    if (simulation) {

        // Check for issues with the simulation

        simulation->checkIssues();

        if (simulation->hasBlockingIssues()) {
            // We return the simulation to allow the user to view its issues

            return PythonQt::priv()->wrapQObject(simulation);
        }

        // Get our runtime

        CellMLSupport::CellmlFileRuntime *runtime = simulation->runtime();

        // Find the solver whose name is first in alphabetical order, as this
        // is the simulation's solver

        QString odeSolverName = QString();
        QString nlaSolverName = QString();

        foreach (SolverInterface *solverInterface, Core::solverInterfaces()) {
            QString solverName = solverInterface->solverName();
            if (solverInterface->solverType() == Solver::Type::Ode) {
                if (odeSolverName.isEmpty()
                 || odeSolverName.compare(solverName, Qt::CaseInsensitive) > 0) {
                    odeSolverName = solverName;
                }
            } else if (solverInterface->solverType() == Solver::Type::Nla) {
                if (nlaSolverName.isEmpty()
                 || nlaSolverName.compare(solverName, Qt::CaseInsensitive) > 0) {
                    nlaSolverName = solverName;
                }
            }
        }

        // Set our solver and its default properties

        setOdeSolver_(simulation->data(), odeSolverName);

        // Set our NLA solver if we need one

        if ((runtime != nullptr) && runtime->needNlaSolver()) {
            setNlaSolver_(simulation->data(), nlaSolverName);
        }

        // Complete initialisation by loading any SED-ML properties

        if (simulation->fileType() == SimulationSupport::Simulation::FileType::SedmlFile
         || simulation->fileType() == SimulationSupport::Simulation::FileType::CombineArchive) {

            const QString initialisationError = simulation->furtherInitialize();

            if (!initialisationError.isEmpty()) {
                // We couldn't complete initialisation so no longer manage the simulation

                simulationManager->unmanage(pFileName);

                // And raise a Python exception

                PyErr_SetString(PyExc_ValueError, initialisationError.toStdString().c_str());

                return nullptr;
            }
        }

        // Do we have a valid simulation?

        if ((runtime != nullptr) && runtime->isValid()) {
            // Reset both the simulation's data and results (well, initialise in the
            // case of its data)

            simulation->data()->reset();
            simulation->results()->reset();
        }

        // Return the simulation as a Python object

        return PythonQt::priv()->wrapQObject(simulation);
    }

    Py_RETURN_NONE;
}

//==============================================================================

static PyObject *openSimulation(PyObject *self, PyObject *args)
{
    Q_UNUSED(self)

    PyObject *bytes;
    char *name;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "O&", PyUnicode_FSConverter, &bytes)) {
        Py_RETURN_NONE;
    }
    PyBytes_AsStringAndSize(bytes, &name, &len);
    QString fileName = QString::fromUtf8(name, len);
    Py_DECREF(bytes);

    QString ioError = Core::openFile(fileName);

    if (!ioError.isEmpty()) {
        PyErr_SetString(PyExc_IOError, ioError.toStdString().c_str());

        return nullptr;
    }

    return initializeSimulation(QFileInfo(fileName).canonicalFilePath());
}

//==============================================================================

static PyObject *openRemoteSimulation(PyObject *self, PyObject *args)
{
    Q_UNUSED(self)

    PyObject *bytes;
    char *name;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "O&", PyUnicode_FSConverter, &bytes)) {
        Py_RETURN_NONE;
    }
    PyBytes_AsStringAndSize(bytes, &name, &len);
    QString url = QString::fromUtf8(name, len);
    Py_DECREF(bytes);

    QString ioError = Core::openRemoteFile(url);

    if (!ioError.isEmpty()) {
        PyErr_SetString(PyExc_IOError, ioError.toStdString().c_str());

        return nullptr;
    }

    return initializeSimulation(Core::localFileName(url));
}

//==============================================================================

static PyObject *closeSimulation(PyObject *self, PyObject *args)
{
    Q_UNUSED(self)

    if (PyTuple_Size(args) > 0) {
        PythonQtInstanceWrapper *wrappedSimulation = PythonQtSupport::getInstanceWrapper(PyTuple_GET_ITEM(args, 0));

        if (wrappedSimulation) {
            SimulationSupport::Simulation *simulation = (SimulationSupport::Simulation *)wrappedSimulation->_objPointerCopy;

            QString fileName = simulation->fileName();

            // Close the simulation by asking our manager to no longer manage it

            SimulationManager::instance()->unmanage(simulation->fileName());

            // And tell the file manager that we no longer are using the file

            Core::FileManager::instance()->unmanage(fileName);
        }
    }

    Py_RETURN_NONE;
}

//==============================================================================

static PyMethodDef pythonSimulationSupportMethods[] = {
    {"openSimulation", openSimulation, METH_VARARGS, "Open a simulation."},
    {"openRemoteSimulation", openRemoteSimulation, METH_VARARGS, "Open a remote simulation."},
    {"closeSimulation", closeSimulation, METH_VARARGS, "Close a simulation."},
    {nullptr, nullptr, 0, nullptr}
};

//==============================================================================

SimulationSupportPythonWrapper::SimulationSupportPythonWrapper(PyObject *pModule, QObject *pParent) :
    QObject(pParent),
    mElapsedTime(-1),
    mErrorMessage(QString())
{
    Q_UNUSED(pModule)

    PythonQtSupport::registerClass(&OpenCOR::SimulationSupport::Simulation::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::SimulationSupport::SimulationData::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::SimulationSupport::SimulationResults::staticMetaObject);
    PythonQtSupport::addInstanceDecorators(this);

    PyModule_AddFunctions(pModule, pythonSimulationSupportMethods);
}

//==============================================================================

void SimulationSupportPythonWrapper::error(const QString &pErrorMessage)
{
    mErrorMessage = pErrorMessage;
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

void SimulationSupportPythonWrapper::simulationFinished(const qint64 &pElapsedTime)
{
    // Save the elapsed time of the simulation

    mElapsedTime = pElapsedTime;

    emit gotElapsedTime();
}

//==============================================================================

bool SimulationSupportPythonWrapper::valid(Simulation *pSimulation)
{
    // Is the simulation's model valid?

    if (!pSimulation->hasBlockingIssues()) {
        CellMLSupport::CellmlFileRuntime *runtime = pSimulation->runtime();
        return (runtime != nullptr) && runtime->isValid();
    }

    return false;
}

//==============================================================================

PyObject *SimulationSupportPythonWrapper::issues(Simulation *pSimulation) const
{
    // Return a list of any issues the simulation has

    PyObject *issuesList = PyList_New(0);

    auto simulationIssues = pSimulation->issues();

    for (const auto &simulationIssue : simulationIssues) {
        QString issueType;
        QString information;

        switch (simulationIssue.type()) {
        case SimulationSupport::SimulationIssue::Type::Information:
            issueType = tr("Information");

            break;
        case SimulationSupport::SimulationIssue::Type::Error:
            issueType = tr("Error");

            break;
        case SimulationSupport::SimulationIssue::Type::Warning:
            issueType = tr("Warning");

            break;
        case SimulationSupport::SimulationIssue::Type::Fatal:
            issueType = tr("Fatal");

            break;
        }

        if ((simulationIssue.line() != 0) && (simulationIssue.column() != 0)) {
            information = QString("[%1:%2] %3: %4.").arg(simulationIssue.line())
                                                     .arg(simulationIssue.column())
                                                     .arg(issueType)
                                                     .arg(Core::formatMessage(simulationIssue.message()));
        } else {
            information = QString("%1: %2.").arg(issueType)
                                             .arg(Core::formatMessage(simulationIssue.message()));
        }

        PyList_Append(issuesList, PyUnicode_FromString(information.toUtf8().constData()));
    }

    return issuesList;
}

//==============================================================================

bool SimulationSupportPythonWrapper::run(Simulation *pSimulation)
{
    if (pSimulation->hasBlockingIssues()) {
        throw std::runtime_error(
            tr("Cannot run because simulation has blocking issues.").toStdString());
    } else if (!valid(pSimulation)) {
        throw std::runtime_error(
            tr("Cannot run because simulation has an invalid runtime.").toStdString());
    }

    QWidget *focusWidget = nullptr;

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

        connect(pSimulation, &Simulation::done,
                this, &SimulationSupportPythonWrapper::simulationFinished);

        // Get error messages from the simulation

        connect(pSimulation, &Simulation::error,
                this, &SimulationSupportPythonWrapper::error);

        // Use an event loop so we don't busy wait

        QEventLoop waitForCompletion;

        // We use a queued connection because the event is in our thread

        connect(this, &SimulationSupportPythonWrapper::gotElapsedTime,
                &waitForCompletion, &QEventLoop::quit, Qt::QueuedConnection);

        // Start the simulation and wait for it to complete

        pSimulation->run();

        waitForCompletion.exec();

        // Disconnect our signal handlers now that the simulation has finished

        disconnect(pSimulation, nullptr, this, nullptr);

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

void SimulationSupportPythonWrapper::setOdeSolver(SimulationData *pSimulationData, const QString &pOdeSolverName)
{
    setOdeSolver_(pSimulationData, pOdeSolverName);
}

//==============================================================================

void SimulationSupportPythonWrapper::setNlaSolver(SimulationData *pSimulationData, const QString &pNlaSolverName)
{
    setNlaSolver_(pSimulationData, pNlaSolverName);
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
            if (stateGradientsDict == nullptr) {
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

#include "pythonend.h"

//==============================================================================
// End of file
//==============================================================================

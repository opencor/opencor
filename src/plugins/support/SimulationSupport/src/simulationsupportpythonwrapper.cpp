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

#include "corecliutils.h"
#include "cellmlfileruntime.h"
#include "datastorepythonwrapper.h"
#include "filemanager.h"
#include "interfaces.h"
#include "pythonqtsupport.h"
#include "simulation.h"
#include "simulationmanager.h"
#include "simulationsupportpythonwrapper.h"

//==============================================================================

#include <QApplication>
#include <QFileInfo>
#include <QWidget>

//==============================================================================

#include <array>

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

static void setOdeSolver(SimulationData *pSimulationData,
                         const QString &pOdeSolverName)
{
    // Set the given ODE solver for the given simulation data

    for (auto solverInterface : Core::solverInterfaces()) {
        if (pOdeSolverName == solverInterface->solverName()) {
            // Set the ODE solver's name

            pSimulationData->setOdeSolverName(pOdeSolverName);

            for (const auto &solverInterfaceProperty : solverInterface->solverProperties()) {
                // Set each ODE solver property to their default value

                pSimulationData->addOdeSolverProperty(solverInterfaceProperty.id(), solverInterfaceProperty.defaultValue());
            }

            return;
        }
    }

    throw std::runtime_error(QObject::tr("The requested solver (%1) could not be found.").arg(pOdeSolverName).toStdString());
}

//==============================================================================

static void setNlaSolver(SimulationData *pSimulationData,
                         const QString &pNlaSolverName)
{
    // Set the given NLA solver for the given simulation data

    for (auto solverInterface : Core::solverInterfaces()) {
        if (pNlaSolverName == solverInterface->solverName()) {
            // Set the NLA solver's name

            pSimulationData->setNlaSolverName(pNlaSolverName);

            for (const auto &solverInterfaceProperty : solverInterface->solverProperties()) {
                // Set each NLA solver property to their default value

                pSimulationData->addNlaSolverProperty(solverInterfaceProperty.id(), solverInterfaceProperty.defaultValue());
            }

            return;
        }
    }

    throw std::runtime_error(QObject::tr("The requested solver (%1) could not be found.").arg(pNlaSolverName).toStdString());
}

//==============================================================================

static PyObject * initializeSimulation(const QString &pFileName)
{
    // Ask our simulation manager to manage our file and then retrieve the
    // corresponding simulation from it

    SimulationManager *simulationManager = SimulationManager::instance();

    simulationManager->manage(pFileName);

    Simulation *simulation = simulationManager->simulation(pFileName);

    if (simulation != nullptr) {
        // Check for issues with the simulation

        if (simulation->hasBlockingIssues()) {
            // We return the simulation to allow the user to view its issues

            return PythonQt::priv()->wrapQObject(simulation);
        }

        // Retrieve a default ODE and NLA solver
        // Note: this is useful in case our simulation is solely based on a
        //       CellML file...

        QString odeSolverName = QString();
        QString nlaSolverName = QString();

        for (auto solverInterface : Core::solverInterfaces()) {
            QString solverName = solverInterface->solverName();

            if (solverInterface->solverType() == Solver::Type::Ode) {
                if (    odeSolverName.isEmpty()
                    || (odeSolverName.compare(solverName, Qt::CaseInsensitive) > 0)) {
                    odeSolverName = solverName;
                }
            } else if (solverInterface->solverType() == Solver::Type::Nla) {
                if (    nlaSolverName.isEmpty()
                    || (nlaSolverName.compare(solverName, Qt::CaseInsensitive) > 0)) {
                    nlaSolverName = solverName;
                }
            }
        }

        // Set our default ODE and NLA, if needed, solvers

        CellMLSupport::CellmlFileRuntime *runtime = simulation->runtime();

        setOdeSolver(simulation->data(), odeSolverName);

        if ((runtime != nullptr) && runtime->needNlaSolver()) {
            setNlaSolver(simulation->data(), nlaSolverName);
        }

        // Further initialise our simulation, should we be dealing with either
        // a SED-ML file or a COMBINE archive
        // Note: this will overwrite the default ODE and NLA solvers that we set
        //       above...

        if (   (simulation->fileType() == SimulationSupport::Simulation::FileType::SedmlFile)
            || (simulation->fileType() == SimulationSupport::Simulation::FileType::CombineArchive)) {
            QString error = simulation->furtherInitialize();

            if (!error.isEmpty()) {
                // We couldn't complete initialisation, so no longer manage the
                // simulation and raise a Python exception

                simulationManager->unmanage(pFileName);

                PyErr_SetString(PyExc_ValueError, qPrintable(error));

                return nullptr;
            }
        }

        // Reset both the simulation's data and results (well, initialise in the
        // case of its data), should we have a valid runtime

        if ((runtime != nullptr) && runtime->isValid()) {
            simulation->data()->reset();
            simulation->results()->reset();
        }

        // Return our simulation object as a Python object

        return PythonQt::priv()->wrapQObject(simulation);
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

static PyObject * openSimulation(PyObject *pSelf, PyObject *pArgs)
{
    Q_UNUSED(pSelf)

    // Open a simulation

    PyObject *bytes;

    if (PyArg_ParseTuple(pArgs, "O&", PyUnicode_FSConverter, &bytes) == 0) { // NOLINT(cppcoreguidelines-pro-type-vararg)
#include "pythonbegin.h"
        Py_RETURN_NONE;
#include "pythonend.h"
    }

    char *string;
    Py_ssize_t len;

    PyBytes_AsStringAndSize(bytes, &string, &len);

    bool isLocalFile;
    QString fileNameOrUrl;

    Core::checkFileNameOrUrl(QString::fromUtf8(string, int(len)), isLocalFile, fileNameOrUrl);

#include "pythonbegin.h"
    Py_DECREF(bytes);
#include "pythonend.h"

    QString error = isLocalFile?
                        Core::openFile(fileNameOrUrl):
                        Core::openRemoteFile(fileNameOrUrl);

    if (!error.isEmpty()) {
        PyErr_SetString(PyExc_IOError, qPrintable(error));

        return nullptr;
    }

    return initializeSimulation(isLocalFile?
                                    fileNameOrUrl:
                                    Core::FileManager::instance()->fileName(fileNameOrUrl));
}

//==============================================================================

static PyObject * closeSimulation(PyObject *pSelf, PyObject *pArgs)
{
    Q_UNUSED(pSelf)

    // Close a simulation

    if (PyTuple_Size(pArgs) > 0) {
#include "pythonbegin.h"
        PythonQtInstanceWrapper *wrappedSimulation = PythonQtSupport::getInstanceWrapper(PyTuple_GET_ITEM(pArgs, 0)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
#include "pythonend.h"

        if (wrappedSimulation != nullptr) {
            // Close the simulation by asking our file and simulation managers
            // to umanage it

            auto simulation = static_cast<SimulationSupport::Simulation *>(wrappedSimulation->_objPointerCopy);
            QString fileName = simulation->fileName();

            Core::FileManager::instance()->unmanage(fileName);
            SimulationManager::instance()->unmanage(fileName);
        }
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

SimulationSupportPythonWrapper::SimulationSupportPythonWrapper(void *pModule,
                                                               QObject *pParent) :
    QObject(pParent)
{
    // Register some OpenCOR classes with Python and add some decorators to
    // ourselves

    PythonQtSupport::registerClass(&Simulation::staticMetaObject);
    PythonQtSupport::registerClass(&SimulationData::staticMetaObject);
    PythonQtSupport::registerClass(&SimulationResults::staticMetaObject);

    PythonQtSupport::addInstanceDecorators(this);

    // Add some Python wrappers

    static std::array<PyMethodDef, 4> PythonSimulationSupportMethods = {{
                                                                           { "openSimulation", openSimulation, METH_VARARGS, "Open a simulation." },
                                                                           { "closeSimulation", closeSimulation, METH_VARARGS, "Close a simulation." },
                                                                           { nullptr, nullptr, 0, nullptr }
                                                                       }};

    PyModule_AddFunctions(static_cast<PyObject *>(pModule),
                          PythonSimulationSupportMethods.data());
}

//==============================================================================

bool SimulationSupportPythonWrapper::valid(Simulation *pSimulation)
{
    // Return whether the given simulation is valid

    if (!pSimulation->hasBlockingIssues()) {
        CellMLSupport::CellmlFileRuntime *runtime = pSimulation->runtime();

        return (runtime != nullptr) && runtime->isValid();
    }

    return false;
}

//==============================================================================

bool SimulationSupportPythonWrapper::run(Simulation *pSimulation)
{
    // Run the given simulation, but only if it doesn't have blocking issues and
    // if it is valid

    if (pSimulation->hasBlockingIssues()) {
        throw std::runtime_error(tr("The simulation has blocking issues and cannot therefore be run.").toStdString());
    }

    if (!valid(pSimulation)) {
        throw std::runtime_error(tr("The simulation has an invalid runtime and cannot therefore be run.").toStdString());
    }

    // Reset our internals

    mElapsedTime = -1;
    mErrorMessage = QString();

    // Try to allocate all the memory we need by adding a run to our simulation
    // and, if successful, run our simulation
    // Note: we keep track of our focus widget (which might be our Python
    //       console window), so that we can give the focus back to it once we
    //       are done running our simulation...

    QWidget *focusWidget = QApplication::focusWidget();

    if (pSimulation->addRun()) {
        // Keep track of any simulation error and of when the simulation is done

        connect(pSimulation, &Simulation::error,
                this, &SimulationSupportPythonWrapper::simulationError);
        connect(pSimulation, &Simulation::done,
                this, &SimulationSupportPythonWrapper::simulationDone);

        // Run our simulation and wait for it to complete
        // Note: we use a queued connection because the event is in our
        //       thread...

        QEventLoop waitLoop;

        connect(pSimulation, &Simulation::done,
                &waitLoop, &QEventLoop::quit,
                Qt::QueuedConnection);

        pSimulation->run();

        waitLoop.exec();

        disconnect(pSimulation, nullptr, this, nullptr);

        // Throw any error message that has been generated

        if (!mErrorMessage.isEmpty()) {
            throw std::runtime_error(mErrorMessage.toStdString());
        }
    } else {
        throw std::runtime_error(tr("The memory required for the simulation could not be allocated.").toStdString());
    }

    // Restore the focus to the previous widget

    if (focusWidget != nullptr) {
        focusWidget->setFocus();
    }

    return mElapsedTime >= 0;
}

//==============================================================================

void SimulationSupportPythonWrapper::reset(Simulation *pSimulation, bool pAll)
{
    // Reset our simulation

    pSimulation->reset(pAll);
}

//==============================================================================

void SimulationSupportPythonWrapper::clearResults(Simulation *pSimulation)
{
    // Reset our simulation results

    pSimulation->results()->reset();
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::issues(Simulation *pSimulation) const
{
    // Return a list of issues the simulation has

    PyObject *issuesList = PyList_New(0);
    auto simulationIssues = pSimulation->issues();

    for (const auto &simulationIssue : simulationIssues) {
        QString information;

        if ((simulationIssue.line() != 0) && (simulationIssue.column() != 0)) {
            information = QString("[%1:%2] %3: %4.").arg(simulationIssue.line())
                                                    .arg(simulationIssue.column())
                                                    .arg(simulationIssue.typeAsString(),
                                                         Core::formatMessage(simulationIssue.message()));
        } else {
            information = QString("%1: %2.").arg(simulationIssue.typeAsString(),
                                                 Core::formatMessage(simulationIssue.message()));
        }

        PyList_Append(issuesList, PyUnicode_FromString(information.toUtf8().constData()));
    }

    return issuesList;
}

//==============================================================================

void SimulationSupportPythonWrapper::setStartingPoint(SimulationData *pSimulationData,
                                                      double pStartingPoint,
                                                      bool pRecompute)
{
    // Set the starting point of our simulation

    pSimulationData->setStartingPoint(pStartingPoint, pRecompute);
}

//==============================================================================

void SimulationSupportPythonWrapper::setEndingPoint(SimulationData *pSimulationData,
                                                    double pEndingPoint)
{
    // Set the ending point of our simulation

    pSimulationData->setEndingPoint(pEndingPoint);
}

//==============================================================================

void SimulationSupportPythonWrapper::setPointInterval(SimulationData *pSimulationData,
                                                      double pPointInterval)
{
    // Set the point interval for our simulation

    pSimulationData->setPointInterval(pPointInterval);
}

//==============================================================================

void SimulationSupportPythonWrapper::setOdeSolver(SimulationData *pSimulationData,
                                                  const QString &pOdeSolverName)
{
    // Set the given ODE solver for the given simulation data

    SimulationSupport::setOdeSolver(pSimulationData, pOdeSolverName);
}

//==============================================================================

void SimulationSupportPythonWrapper::setNlaSolver(SimulationData *pSimulationData,
                                                  const QString &pNlaSolverName)
{
    // Set the given NLA solver for the given simulation data

    SimulationSupport::setNlaSolver(pSimulationData, pNlaSolverName);
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::constants(SimulationData *pSimulationData) const
{
    // Return our constants values

    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationData->constantsValues(),
                                                                  &(pSimulationData->simulationDataUpdatedFunction()));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::rates(SimulationData *pSimulationData) const
{
    // Return our rates values

    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationData->ratesValues(),
                                                                  &(pSimulationData->simulationDataUpdatedFunction()));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::states(SimulationData *pSimulationData) const
{
    // Return our states values

    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationData->statesValues(),
                                                                  &(pSimulationData->simulationDataUpdatedFunction()));
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::algebraic(SimulationData *pSimulationData) const
{
    // Return our algebraic values

    return DataStore::DataStorePythonWrapper::dataStoreValuesDict(pSimulationData->algebraicValues(),
                                                                  &(pSimulationData->simulationDataUpdatedFunction()));
}

//==============================================================================

DataStore::DataStoreVariable * SimulationSupportPythonWrapper::points(SimulationResults *pSimulationResults) const
{
    // Return our points variable

    return pSimulationResults->pointsVariable();
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::constants(SimulationResults *pSimulationResults) const
{
    // Return our constants variables

    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->constantsVariables());
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::rates(SimulationResults *pSimulationResults) const
{
    // Return our rates variables

    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->ratesVariables());
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::states(SimulationResults *pSimulationResults) const
{
    // Return our states variables

    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->statesVariables());
}

//==============================================================================

PyObject * SimulationSupportPythonWrapper::algebraic(SimulationResults *pSimulationResults) const
{
    // Return our algebraic variables

    return DataStore::DataStorePythonWrapper::dataStoreVariablesDict(pSimulationResults->algebraicVariables());
}

//==============================================================================

void SimulationSupportPythonWrapper::simulationError(const QString &pErrorMessage)
{
    // Keep track of the given error message

    mErrorMessage = pErrorMessage;
}

//==============================================================================

void SimulationSupportPythonWrapper::simulationDone(qint64 pElapsedTime)
{
    // Save the given elapsed time and let people know that we have got it

    mElapsedTime = pElapsedTime;
}

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

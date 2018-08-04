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
// Simulation
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlfileruntime.h"
#include "combinefilemanager.h"
#include "interfaces.h"
#include "sedmlfilemanager.h"
#include "simulation.h"
#include "simulationworker.h"

//==============================================================================

#include <QtMath>

//==============================================================================

#include <QEventLoop>

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

SimulationData::SimulationData(Simulation *pSimulation) :
    mSimulation(pSimulation),
    mDelay(0),
    mStartingPoint(0.0),
    mEndingPoint(1000.0),
    mPointInterval(1.0),
    mOdeSolverName(QString()),
    mOdeSolverProperties(Solver::Solver::Properties()),
    mDaeSolverName(QString()),
    mDaeSolverProperties(Solver::Solver::Properties()),
    mNlaSolverName(QString()),
    mNlaSolverProperties(Solver::Solver::Properties())
{
    // Create our various arrays

    createArrays();
}

//==============================================================================

SimulationData::~SimulationData()
{
    // Delete some internal objects

    deleteArrays();
}

//==============================================================================

void SimulationData::reload()
{
    // Reload ourselves by deleting and recreating our arrays

    deleteArrays();
    createArrays();
}

//==============================================================================

Simulation * SimulationData::simulation() const
{
    // Return our simulation

    return mSimulation;
}

//==============================================================================

double * SimulationData::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double * SimulationData::rates() const
{
    // Return our rates array

    return mRates;
}

//==============================================================================

double * SimulationData::states() const
{
    // Return our states array

    return mStates;
}

//==============================================================================

double * SimulationData::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

int SimulationData::delay() const
{
    // Return our delay

    return mDelay;
}

//==============================================================================

void SimulationData::setDelay(int pDelay)
{
    // Set our delay

    mDelay = pDelay;
}

//==============================================================================

double SimulationData::startingPoint() const
{
    // Return our starting point

    return mStartingPoint;
}

//==============================================================================

void SimulationData::setStartingPoint(double pStartingPoint, bool pRecompute)
{
    // Set our starting point

    mStartingPoint = pStartingPoint;

    // Recompute our 'computed constants' and 'variables', i.e. reset ourselves
    // witout initialisation (hence passing false to reset())

    if (pRecompute)
        reset(false);
}

//==============================================================================

double SimulationData::endingPoint() const
{
    // Return our ending point

    return mEndingPoint;
}

//==============================================================================

void SimulationData::setEndingPoint(double pEndingPoint)
{
    // Set our ending point

    mEndingPoint = pEndingPoint;
}

//==============================================================================

double SimulationData::pointInterval() const
{
    // Return our point interval

    return mPointInterval;
}

//==============================================================================

void SimulationData::setPointInterval(double pPointInterval)
{
    // Set our point interval

    mPointInterval = pPointInterval;
}

//==============================================================================

SolverInterface * SimulationData::solverInterface(const QString &pSolverName) const
{
    // Return the named solver interface, if any

    foreach (SolverInterface *solverInterface, Core::solverInterfaces()) {
        if (!solverInterface->solverName().compare(pSolverName))
            return solverInterface;
    }

    return nullptr;
}

//==============================================================================

SolverInterface * SimulationData::odeSolverInterface() const
{
    // Return our ODE solver interface, if any

    return solverInterface(odeSolverName());
}

//==============================================================================

QString SimulationData::odeSolverName() const
{
    // Return our ODE solver name

    return mSimulation->runtime()?mOdeSolverName:QString();
}

//==============================================================================

void SimulationData::setOdeSolverName(const QString &pOdeSolverName)
{
    // Set our ODE solver name and reset its properties

    if (pOdeSolverName.compare(mOdeSolverName) && mSimulation->runtime()) {
        mOdeSolverName = pOdeSolverName;

        mOdeSolverProperties.clear();
    }
}

//==============================================================================

Solver::Solver::Properties SimulationData::odeSolverProperties() const
{
    // Return our ODE solver's properties

    return mSimulation->runtime()?mOdeSolverProperties:Solver::Solver::Properties();
}

//==============================================================================

void SimulationData::addOdeSolverProperty(const QString &pName,
                                          const QVariant &pValue)
{
    // Add an ODE solver property

    if (mSimulation->runtime())
        mOdeSolverProperties.insert(pName, pValue);
}

//==============================================================================

SolverInterface * SimulationData::nlaSolverInterface() const
{
    // Return our NLA solver interface, if any

    return solverInterface(nlaSolverName());
}

//==============================================================================

QString SimulationData::nlaSolverName() const
{
    // Return our NLA solver name

    return (   mSimulation->runtime()
            && mSimulation->runtime()->needNlaSolver())?mNlaSolverName:QString();
}

//==============================================================================

void SimulationData::setNlaSolverName(const QString &pNlaSolverName,
                                      bool pReset)
{
    // Set our NLA solver name and reset its properties

    if (   pNlaSolverName.compare(mNlaSolverName)
        && mSimulation->runtime() && mSimulation->runtime()->needNlaSolver()) {
        mNlaSolverName = pNlaSolverName;

        mNlaSolverProperties.clear();

        // Reset our parameter values, if required
        // Note: to only recompute our 'computed constants' and 'variables' is
        //       not sufficient since some constants may also need to be
        //       reinitialised...

        if (pReset)
            reset();
    }
}

//==============================================================================

Solver::Solver::Properties SimulationData::nlaSolverProperties() const
{
    // Return our NLA solver's properties

    return (   mSimulation->runtime()
            && mSimulation->runtime()->needNlaSolver())?mNlaSolverProperties:Solver::Solver::Properties();
}

//==============================================================================

void SimulationData::addNlaSolverProperty(const QString &pName,
                                          const QVariant &pValue, bool pReset)
{
    // Add an NLA solver property

    if (mSimulation->runtime() && mSimulation->runtime()->needNlaSolver()) {
        mNlaSolverProperties.insert(pName, pValue);

        // Reset our parameter values, if required
        // Note: to only recompute our 'computed constants' and 'variables' is
        //       not sufficient since some constants may also need to be
        //       reinitialised...

        if (pReset)
            reset();
    }
}

//==============================================================================

void SimulationData::reset(bool pInitialize)
{
    // Reset our parameter values which means both initialising our 'constants'
    // and computing our 'computed constants' and 'variables'
    // Note #1: we must check whether our runtime needs NLA solver and, if so,
    //          then retrieve an instance of our NLA solver since some of the
    //          resetting may require solving one or several NLA systems...
    // Note #2: recomputeComputedConstantsAndVariables() will let people know
    //          that our data has changed...

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();
    Solver::NlaSolver *nlaSolver = nullptr;

    if (runtime->needNlaSolver()) {
        // Set our NLA solver
        // Note: we unset it at the end of this method...

        nlaSolver = static_cast<Solver::NlaSolver *>(nlaSolverInterface()->solverInstance());

        Solver::setNlaSolver(runtime->address(), nlaSolver);

        // Keep track of any error that might be reported by our NLA solver

        connect(nlaSolver, &Solver::NlaSolver::error,
                this, &SimulationData::error);

        // Initialise our NLA solver

        nlaSolver->setProperties(mNlaSolverProperties);
    }

    // Reset our parameter values

    if (pInitialize) {
        memset(mConstants, 0, size_t(runtime->constantsCount()*Solver::SizeOfDouble));
        memset(mRates, 0, size_t(runtime->ratesCount()*Solver::SizeOfDouble));
        memset(mStates, 0, size_t(runtime->statesCount()*Solver::SizeOfDouble));
        memset(mAlgebraic, 0, size_t(runtime->algebraicCount()*Solver::SizeOfDouble));

        runtime->initializeConstants()(mConstants, mRates, mStates);
    }

    recomputeComputedConstantsAndVariables(mStartingPoint, pInitialize);

    // Delete our NLA solver, if any

    if (nlaSolver) {
        delete nlaSolver;

        Solver::unsetNlaSolver(runtime->address());
    }

    // Keep track of our various initial values

    if (pInitialize) {
        memcpy(mInitialConstants, mConstants, size_t(runtime->constantsCount()*Solver::SizeOfDouble));
        memcpy(mInitialStates, mStates, size_t(runtime->statesCount()*Solver::SizeOfDouble));
    }

    // Let people know whether our data is 'cleaned', i.e. not modified, and ask
    // our simulation worker to reset itself
    // Note: no point in checking if we are initialising...

    if (!pInitialize) {
        emit modified(isModified());

        if (mSimulation->worker())
            mSimulation->worker()->reset();
    }
}

//==============================================================================

void SimulationData::recomputeComputedConstantsAndVariables(double pCurrentPoint,
                                                            bool pInitialize)
{
    // Recompute our 'computed constants', some 'constant' algebraic variables
    // and our 'variables'

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    runtime->computeComputedConstants()(pCurrentPoint, mConstants, mRates, pInitialize?mStates:mDummyStates, mAlgebraic);
    runtime->computeRates()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);
    runtime->computeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);

    // Let people know that our data has been updated

    emit updated(pCurrentPoint);
}

//==============================================================================

void SimulationData::recomputeVariables(double pCurrentPoint)
{
    // Recompute our 'variables'

    mSimulation->runtime()->computeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);
}

//==============================================================================

bool SimulationData::isModified() const
{
    // Check whether any of our constants or states has been modified, if
    // possible
    // Note: we start with our states since they are more likely to be modified
    //       than our constants...

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    if (runtime) {
        for (int i = 0, iMax = runtime->statesCount(); i < iMax; ++i) {
            if (!qIsNull(mStates[i]-mInitialStates[i]))
                return true;
        }

        for (int i = 0, iMax = runtime->constantsCount(); i < iMax; ++i) {
            if (!qIsNull(mConstants[i]-mInitialConstants[i]))
                return true;
        }
    }

    return false;
}

//==============================================================================

void SimulationData::checkForModifications()
{
    // Let people know whether any of our constants or states has been modified

    emit modified(isModified());
}

//==============================================================================

void SimulationData::createArrays()
{
    // Create our various arrays, if possible

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    if (runtime) {
        // Create our various arrays to compute our model

        mConstants = new double[runtime->constantsCount()] {};
        mRates = new double[runtime->ratesCount()] {};
        mStates = new double[runtime->statesCount()] {};
        mDummyStates = new double[runtime->statesCount()] {};
        mAlgebraic = new double[runtime->algebraicCount()] {};

        // Create our various arrays to keep track of our various initial values

        mInitialConstants = new double[runtime->constantsCount()] {};
        mInitialStates = new double[runtime->statesCount()] {};
    } else {
        mConstants = mRates = mStates = mDummyStates = mAlgebraic = nullptr;
        mInitialConstants = mInitialStates = nullptr;
    }
}

//==============================================================================

void SimulationData::deleteArrays()
{
    // Delete our various arrays

    delete[] mConstants;
    delete[] mRates;
    delete[] mStates;
    delete[] mDummyStates;
    delete[] mAlgebraic;

    delete[] mInitialConstants;
    delete[] mInitialStates;
}

//==============================================================================

SimulationResults::SimulationResults(Simulation *pSimulation) :
    mSimulation(pSimulation),
    mDataStore(nullptr),
    mPoints(nullptr),
    mConstants(DataStore::DataStoreVariables()),
    mRates(DataStore::DataStoreVariables()),
    mStates(DataStore::DataStoreVariables()),
    mAlgebraic(DataStore::DataStoreVariables())
{
    // Create our data store

    createDataStore();
}

//==============================================================================

SimulationResults::~SimulationResults()
{
    // Delete some internal objects

    deleteDataStore();
}

//==============================================================================

QString SimulationResults::uri(const QStringList &pComponentHierarchy,
                               const QString &pName)
{
    // Generate an URI using the given component hierarchy and name

    QString res = pComponentHierarchy.join('/')+"/"+pName;

    return res.replace('\'', "/prime");
}

//==============================================================================

void SimulationResults::createDataStore()
{
    // Make sure that we have a runtime and a VOI

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    if (!runtime || !runtime->voi())
        return;

    // Create our data store

    SimulationData *data = mSimulation->data();

    mDataStore = new DataStore::DataStore(runtime->cellmlFile()->xmlBase());

    mPoints = mDataStore->voi();

    mConstants = mDataStore->addVariables(data->constants(), runtime->constantsCount());
    mRates = mDataStore->addVariables(data->rates(), runtime->ratesCount());
    mStates = mDataStore->addVariables(data->states(), runtime->statesCount());
    mAlgebraic = mDataStore->addVariables(data->algebraic(), runtime->algebraicCount());

    // Customise our VOI, as well as our constant, rate, state and algebraic
    // variables

    for (int i = 0, iMax = runtime->parameters().count(); i < iMax; ++i) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = runtime->parameters()[i];
        DataStore::DataStoreVariable *variable = nullptr;

        switch (parameter->type()) {
        case CellMLSupport::CellmlFileRuntimeParameter::Voi:
            mPoints->setType(parameter->type());
            mPoints->setUri(uri(runtime->voi()->componentHierarchy(),
                                runtime->voi()->name()));
            mPoints->setLabel(runtime->voi()->name());
            mPoints->setUnit(runtime->voi()->unit());

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Constant:
        case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
            variable = mConstants[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Rate:
            variable = mRates[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::State:
            variable = mStates[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
            variable = mAlgebraic[parameter->index()];

            break;
        default:
            // Not a relevant type, so do nothing

            ;
        }

        if (variable) {
            variable->setType(parameter->type());
            variable->setUri(uri(parameter->componentHierarchy(), parameter->formattedName()));
            variable->setLabel(parameter->formattedName());
            variable->setUnit(parameter->formattedUnit(runtime->voi()->unit()));
        }
    }
}

//==============================================================================

void SimulationResults::deleteDataStore()
{
    // Delete our data store

    delete mDataStore;

    // Reset our data store and our different data store variable/s
    // Note: this is in case we are not able to recreate a data store...

    mDataStore = nullptr;

    mPoints = nullptr;

    mConstants = DataStore::DataStoreVariables();
    mRates = DataStore::DataStoreVariables();
    mStates = DataStore::DataStoreVariables();
    mAlgebraic = DataStore::DataStoreVariables();
}

//==============================================================================

void SimulationResults::reload()
{
    // Reload ourselves by resetting ourselves

    reset();
}

//==============================================================================

void SimulationResults::reset()
{
    // Reset our data store by deleting it and then recreating it

    deleteDataStore();
    createDataStore();
}

//==============================================================================

int SimulationResults::runsCount() const
{
    // Return the number of runs held by our data store

    return mDataStore?mDataStore->runsCount():0;
}

//==============================================================================

bool SimulationResults::addRun()
{
    // Ask our data store to add a run to itself
    // Note: we consider things to be fine if our data store has had no problems
    //       adding a run to itself or if the simulation size is zero...

    quint64 simulationSize = mSimulation->size();

    if (simulationSize)
        return mDataStore->addRun(simulationSize);
    else
        return true;
}

//==============================================================================

void SimulationResults::addPoint(double pPoint)
{
    // Add the data to our data store

    mDataStore->addValues(pPoint);
}

//==============================================================================

quint64 SimulationResults::size(int pRun) const
{
    // Return the size of our data store for the given run

    return mDataStore?mDataStore->size(pRun):0;
}

//==============================================================================

DataStore::DataStore * SimulationResults::dataStore() const
{
    // Return our data store

    return mDataStore;
}

//==============================================================================

double * SimulationResults::points(int pRun) const
{
    // Return our points for the given run

    return mPoints?mPoints->values(pRun):nullptr;
}

//==============================================================================

double * SimulationResults::constants(int pIndex, int pRun) const
{
    // Return our constants data at the given index and for the given run

    return mConstants.isEmpty()?nullptr:mConstants[pIndex]->values(pRun);
}

//==============================================================================

double * SimulationResults::rates(int pIndex, int pRun) const
{
    // Return our rates data at the given index and for the given run

    return mRates.isEmpty()?nullptr:mRates[pIndex]->values(pRun);
}

//==============================================================================

double * SimulationResults::states(int pIndex, int pRun) const
{
    // Return our states data at the given index and for the given run

    return mStates.isEmpty()?nullptr:mStates[pIndex]->values(pRun);
}

//==============================================================================

double * SimulationResults::algebraic(int pIndex, int pRun) const
{
    // Return our algebraic data at the given index and for the given run

    return mAlgebraic.isEmpty()?nullptr:mAlgebraic[pIndex]->values(pRun);
}

//==============================================================================

Simulation::Simulation(const QString &pFileName) :
    mFileName(pFileName),
    mRuntime(nullptr),
    mWorker(nullptr),
    mWorkerFinishedEventLoop(new QEventLoop())
{
    // Retrieve our file details

    retrieveFileDetails();

    // Create our data and results objects, now that we are all set

    mData = new SimulationData(this);
    mResults = new SimulationResults(this);

    // Keep track of any error occurring in our data

    connect(mData, &SimulationData::error,
            this, &Simulation::error);
}

//==============================================================================

Simulation::~Simulation()
{
    // Stop our worker

    stop();

    // Delete some internal objects

    delete mRuntime;

    delete mResults;
    delete mData;
}

//==============================================================================

void Simulation::retrieveFileDetails(bool pRecreateRuntime)
{
    // Retrieve our CellML and SED-ML files, as well as COMBINE archive

    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);
    mSedmlFile = mCellmlFile?nullptr:SEDMLSupport::SedmlFileManager::instance()->sedmlFile(mFileName);
    mCombineArchive = mSedmlFile?nullptr:COMBINESupport::CombineFileManager::instance()->combineArchive(mFileName);

    // Determine the type of our file

    mFileType = mCellmlFile?CellmlFile:mSedmlFile?SedmlFile:CombineArchive;

    // We have a COMBINE archive, so we need to retrieve its corresponding
    // SED-ML file

    if (mCombineArchive)
        mSedmlFile = mCombineArchive->sedmlFile();

    // We have a SED-ML file (either a direct one or through a COMBINE archive),
    // so we need to retrieve its corresponding CellML file

    if (mSedmlFile)
        mCellmlFile = mSedmlFile->cellmlFile();

    // Get a (new) runtime, if possible

    if (pRecreateRuntime) {
        delete mRuntime;

        mRuntime = mCellmlFile?mCellmlFile->runtime(true):nullptr;
    }
}

//==============================================================================

QString Simulation::fileName() const
{
    // Return our file name

    return mFileName;
}

//==============================================================================

void Simulation::save()
{
    // Retrieve our file details

    bool needReloading = !mRuntime;

    retrieveFileDetails(false);

    // Ask our data and results to update themselves, if needed
    // Note: this is, for example, needed when we open an invalid file (in which
    //       case mRuntime is null), fix it (resulting in a valid mRuntime
    //       value) and then save it...

    if (needReloading) {
        mData->reload();
        mResults->reload();
    }
}

//==============================================================================

void Simulation::reload()
{
    // Stop our worker
    // Note: we don't need to delete mWorker since it will be done as part of
    //       its thread being stopped...

    stop();

    // Retrieve our file details

    retrieveFileDetails();

    // Ask our data and results to update themselves

    mData->reload();
    mResults->reload();
}

//==============================================================================

void Simulation::rename(const QString &pFileName)
{
    // Rename ourselves by simply updating our file name

    mFileName = pFileName;
}

//==============================================================================

CellMLSupport::CellmlFileRuntime * Simulation::runtime() const
{
    // Return our runtime

    return mRuntime;
}

//==============================================================================

SimulationWorker * Simulation::worker() const
{
    // Return our worker

    return mWorker;
}

//==============================================================================

Simulation::FileType Simulation::fileType() const
{
    // Return our file type

    return mFileType;
}

//==============================================================================

CellMLSupport::CellmlFile * Simulation::cellmlFile() const
{
    // Return our CellML file object

    return mCellmlFile;
}

//==============================================================================

SEDMLSupport::SedmlFile * Simulation::sedmlFile() const
{
    // Return our SED-ML file object

    return mSedmlFile;
}

//==============================================================================

COMBINESupport::CombineArchive * Simulation::combineArchive() const
{
    // Return our COBMINE archive object

    return mCombineArchive;
}

//==============================================================================

SimulationData * Simulation::data() const
{
    // Return our data

    return mData;
}

//==============================================================================

SimulationResults * Simulation::results() const
{
    // Return our results

    return mResults;
}

//==============================================================================

int Simulation::runsCount() const
{
    // Return the number of runs held by our results

    return mResults?mResults->runsCount():0;
}

//==============================================================================

bool Simulation::addRun()
{
    // Ask our results to add a run

    return mResults?mResults->addRun():false;
}

//==============================================================================

bool Simulation::isRunning() const
{
    // Return whether we are running

    return mWorker?mWorker->isRunning():false;
}

//==============================================================================

bool Simulation::isPaused() const
{
    // Return whether we are paused

    return mWorker?mWorker->isPaused():false;
}

//==============================================================================

double Simulation::currentPoint() const
{
    // Return our current point

    return mWorker?mWorker->currentPoint():data()->startingPoint();
}

//==============================================================================

int Simulation::delay() const
{
    // Return our delay

    return mData->delay();
}

//==============================================================================

void Simulation::setDelay(int pDelay)
{
    // Set our delay

    mData->setDelay(pDelay);
}

//==============================================================================

bool Simulation::simulationSettingsOk(bool pEmitSignal)
{
    // Check and return whether our simulation settings are sound

    if (qIsNull(mData->startingPoint()-mData->endingPoint())) {
        if (pEmitSignal)
            emit error(tr("the starting and ending points cannot have the same value"));

        return false;
    } else if (mData->startingPoint() > mData->endingPoint()) {
        if (pEmitSignal)
            emit error(tr("the starting point cannot be greater than the ending point"));

        return false;
    } else {
        return true;
    }
}

//==============================================================================

quint64 Simulation::size()
{
    // Return the size of our simulation (i.e. the number of data points that
    // should be generated), if possible

    if (simulationSettingsOk(false))
        return quint64(ceil((mData->endingPoint()-mData->startingPoint())/mData->pointInterval())+1.0);
    else
        return 0;
}

//==============================================================================

bool Simulation::run()
{
    if (!mRuntime)
        return false;

    // Initialise our worker, if not active

    if (mWorker) {
        return false;
    } else {
        // Make sure that the simulation settings we were given are sound

        if (!simulationSettingsOk())
            return false;

        // Create our worker

        mWorker = new SimulationWorker(this, mWorker);

        if (!mWorker) {
            emit error(tr("the simulation worker could not be created"));

            return false;
        }

        // Create a few connections

        connect(mWorker, &SimulationWorker::running,
                this, &Simulation::running);
        connect(mWorker, &SimulationWorker::paused,
                this, &Simulation::paused);

        connect(mWorker, &SimulationWorker::finished,
                this, &Simulation::stopped);

        connect(mWorker, &SimulationWorker::error,
                this, &Simulation::error);

        // Track of when our worker is finished

        connect(mWorker, &SimulationWorker::finished,
                mWorkerFinishedEventLoop, &QEventLoop::quit);

        // Start our worker

        return mWorker->run();
    }
}

//==============================================================================

bool Simulation::pause()
{
    // Pause our worker

    return mWorker?mWorker->pause():false;
}

//==============================================================================

bool Simulation::resume()
{
    // Resume our worker

    return mWorker?mWorker->resume():false;
}

//==============================================================================

bool Simulation::stop()
{
    // Stop our worker, if any, and wait for it to be done

    if (mWorker && mWorker->stop()) {
        mWorkerFinishedEventLoop->exec();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

bool Simulation::reset()
{
    // Reset our data

    mData->reset();

    // Reset our worker

    return mWorker?mWorker->reset():false;
}

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

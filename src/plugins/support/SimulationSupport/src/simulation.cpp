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
#include "combinefilemanager.h"
#include "interfaces.h"
#include "sedmlfilemanager.h"
#include "simulation.h"
#include "simulationworker.h"

//==============================================================================

#include <QtMath>

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

void SimulationData::setDelay(const int &pDelay)
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

void SimulationData::setStartingPoint(const double &pStartingPoint,
                                      const bool &pRecompute)
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

void SimulationData::setEndingPoint(const double &pEndingPoint)
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

void SimulationData::setPointInterval(const double &pPointInterval)
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

    return 0;
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
                                      const bool &pReset)
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
                                          const QVariant &pValue,
                                          const bool &pReset)
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

void SimulationData::reset(const bool &pInitialize)
{
    // Reset our parameter values which means both initialising our 'constants'
    // and computing our 'computed constants' and 'variables'
    // Note #1: we must check whether our runtime needs NLA solver and, if so,
    //          then retrieve an instance of our NLA solver since some of the
    //          resetting may require solving one or several NLA systems...
    // Note #2: recomputeComputedConstantsAndVariables() will let people know
    //          that our data has changed...

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();
    Solver::NlaSolver *nlaSolver = 0;

    if (runtime->needNlaSolver()) {
        // Set our NLA solver
        // Note: we unset it at the end of this method...

        nlaSolver = static_cast<Solver::NlaSolver *>(nlaSolverInterface()->solverInstance());

        Solver::setNlaSolver(runtime->address(), nlaSolver);

        // Keep track of any error that might be reported by our NLA solver

        connect(nlaSolver, SIGNAL(error(const QString &)),
                this, SIGNAL(error(const QString &)));

        // Initialise our NLA solver

        nlaSolver->setProperties(mNlaSolverProperties);
    }

    // Reset our parameter values

    if (pInitialize) {
        memset(mConstants, 0, runtime->constantsCount()*Solver::SizeOfDouble);
        memset(mRates, 0, runtime->ratesCount()*Solver::SizeOfDouble);
        memset(mStates, 0, runtime->statesCount()*Solver::SizeOfDouble);
        memset(mAlgebraic, 0, runtime->algebraicCount()*Solver::SizeOfDouble);

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
        memcpy(mInitialConstants, mConstants, runtime->constantsCount()*Solver::SizeOfDouble);
        memcpy(mInitialStates, mStates, runtime->statesCount()*Solver::SizeOfDouble);
    }

    // Let people know whether our data is 'cleaned', i.e. not modified
    // Note: no point in checking if we are initialising...

    if (!pInitialize)
        emit modified(isModified());
}

//==============================================================================

void SimulationData::recomputeComputedConstantsAndVariables(const double &pCurrentPoint,
                                                            const bool &pInitialize)
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

void SimulationData::recomputeVariables(const double &pCurrentPoint)
{
    // Recompute our 'variables'

    mSimulation->runtime()->computeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);
}

//==============================================================================

bool SimulationData::isModified() const
{
    // Check whether any of our constants or states has been modified
    // Note: we start with our states since they are more likely to be modified
    //       than our constants...

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    for (int i = 0, iMax = runtime->statesCount(); i < iMax; ++i) {
        if (!qIsFinite(mStates[i]) || (mStates[i] != mInitialStates[i]))
            return true;
    }

    for (int i = 0, iMax = runtime->constantsCount(); i < iMax; ++i) {
        if (!qIsFinite(mConstants[i]) || (mConstants[i] != mInitialConstants[i]))
            return true;
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

        mConstants = new double[runtime->constantsCount()];
        mRates = new double[runtime->ratesCount()];
        mStates = new double[runtime->statesCount()];
        mDummyStates = new double[runtime->statesCount()];
        mAlgebraic = new double[runtime->algebraicCount()];

        // Create our various arrays to keep track of our various initial values

        mInitialConstants = new double[runtime->constantsCount()];
        mInitialStates = new double[runtime->statesCount()];
    } else {
        mConstants = mRates = mStates = mDummyStates = mAlgebraic = 0;
        mInitialConstants = mInitialStates = 0;
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
    mDataStore(0),
    mPoints(0),
    mConstants(DataStore::DataStoreVariables()),
    mRates(DataStore::DataStoreVariables()),
    mStates(DataStore::DataStoreVariables()),
    mAlgebraic(DataStore::DataStoreVariables())
{
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

bool SimulationResults::createDataStore()
{
    // Note: the boolean value we return is true if we have had no problem
    //       creating our data store or if the simulation size is zero...

    // Delete the previous data store, if any

    deleteDataStore();

    // Retrieve the size of our data and make sure that it is valid

    qulonglong simulationSize = qulonglong(mSimulation->size());

    if (!simulationSize)
        return true;

    // Create our data store and populate it with a variable of integration, as
    // well as with constant, rate, state and algebraic variables

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    try {
        mDataStore = new DataStore::DataStore(runtime->cellmlFile()->xmlBase(), simulationSize);

        mPoints = mDataStore->addVoi();
        mConstants = mDataStore->addVariables(runtime->constantsCount(), mSimulation->data()->constants());
        mRates = mDataStore->addVariables(runtime->ratesCount(), mSimulation->data()->rates());
        mStates = mDataStore->addVariables(runtime->statesCount(), mSimulation->data()->states());
        mAlgebraic = mDataStore->addVariables(runtime->algebraicCount(), mSimulation->data()->algebraic());
    } catch (...) {
        deleteDataStore();

        return false;
    }

    // Customise our variable of integration, as well as our constant, rate,
    // state and algebraic variables

    for (int i = 0, iMax = runtime->parameters().count(); i < iMax; ++i) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = runtime->parameters()[i];
        DataStore::DataStoreVariable *variable = 0;

        switch (parameter->type()) {
        case CellMLSupport::CellmlFileRuntimeParameter::Voi:
            mPoints->setIcon(parameter->icon());
            mPoints->setUri(uri(runtime->variableOfIntegration()->componentHierarchy(),
                                runtime->variableOfIntegration()->name()));
            mPoints->setLabel(runtime->variableOfIntegration()->name());
            mPoints->setUnit(runtime->variableOfIntegration()->unit());

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
            variable->setIcon(parameter->icon());
            variable->setUri(uri(parameter->componentHierarchy(), parameter->formattedName()));
            variable->setLabel(parameter->formattedName());
            variable->setUnit(parameter->formattedUnit(runtime->variableOfIntegration()->unit()));
        }
    }

    return true;
}

//==============================================================================

void SimulationResults::deleteDataStore()
{
    // Delete our data store

    delete mDataStore;

    mDataStore = 0;
}

//==============================================================================

void SimulationResults::reload()
{
    // Reload ourselves by deleting our data store

    deleteDataStore();
}

//==============================================================================

bool SimulationResults::reset(const bool &pCreateDataStore)
{
    // Reset our data store

    if (pCreateDataStore) {
        return createDataStore();
    } else {
        deleteDataStore();

        return true;
    }
}

//==============================================================================

void SimulationResults::addPoint(const double &pPoint)
{
    // Add the data to our data store

    mDataStore->addValues(pPoint);
}

//==============================================================================

qulonglong SimulationResults::size() const
{
    // Return our size

    return mDataStore?mDataStore->size():0;
}

//==============================================================================

DataStore::DataStore * SimulationResults::dataStore() const
{
    // Return our data store

    return mDataStore;
}

//==============================================================================

double * SimulationResults::points() const
{
    // Return our points

    return mPoints?mPoints->values():0;
}

//==============================================================================

double * SimulationResults::constants(const int &pIndex) const
{
    // Return our constants data at the given index

    return mConstants.isEmpty()?0:mConstants[pIndex]->values();
}

//==============================================================================

double * SimulationResults::rates(const int &pIndex) const
{
    // Return our rates data at the given index

    return mRates.isEmpty()?0:mRates[pIndex]->values();
}

//==============================================================================

double * SimulationResults::states(const int &pIndex) const
{
    // Return our states data at the given index

    return mStates.isEmpty()?0:mStates[pIndex]->values();
}

//==============================================================================

double * SimulationResults::algebraic(const int &pIndex) const
{
    // Return our algebraic data at the given index

    return mAlgebraic.isEmpty()?0:mAlgebraic[pIndex]->values();
}

//==============================================================================

Simulation::Simulation(const QString &pFileName) :
    mFileName(pFileName),
    mWorker(0)
{
    // Retrieve our file details

    retrieveFileDetails();

    // Create our data and results objects, now that we are all set

    mData = new SimulationData(this);
    mResults = new SimulationResults(this);

    // Keep track of any error occurring in our data

    connect(mData, SIGNAL(error(const QString &)),
            this, SIGNAL(error(const QString &)));
}

//==============================================================================

Simulation::~Simulation()
{
    // Stop our worker

    stop();

    // Delete some internal objects

    delete mResults;
    delete mData;
}

//==============================================================================

void Simulation::retrieveFileDetails()
{
    // Retrieve our CellML and SED-ML files, as well as COMBINE archive

    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName);
    mSedmlFile = mCellmlFile?0:SEDMLSupport::SedmlFileManager::instance()->sedmlFile(mFileName);
    mCombineArchive = mSedmlFile?0:COMBINESupport::CombineFileManager::instance()->combineArchive(mFileName);

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

    // Keep track of our runtime, if any

    mRuntime = mCellmlFile?mCellmlFile->runtime(true):0;
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

    retrieveFileDetails();
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

void Simulation::setDelay(const int &pDelay)
{
    // Set our delay

    mData->setDelay(pDelay);
}

//==============================================================================

bool Simulation::simulationSettingsOk(const bool &pEmitSignal)
{
    // Check and return whether our simulation settings are sound

    if (mData->startingPoint() == mData->endingPoint()) {
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

double Simulation::size()
{
    // Return the size of our simulation (i.e. the number of data points that
    // should be generated), if possible
    // Note: we return a double rather than a qulonglong in case the simulation
    //       requires an insane amount of memory...

    if (simulationSettingsOk(false))
        return ceil((mData->endingPoint()-mData->startingPoint())/mData->pointInterval())+1.0;
    else
        return 0.0;
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

        connect(mWorker, SIGNAL(running(const bool &)),
                this, SIGNAL(running(const bool &)));
        connect(mWorker, SIGNAL(paused()),
                this, SIGNAL(paused()));

        connect(mWorker, SIGNAL(finished(const qint64 &)),
                this, SIGNAL(stopped(const qint64 &)));

        connect(mWorker, SIGNAL(error(const QString &)),
                this, SIGNAL(error(const QString &)));

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
    // Stop our worker

    return mWorker?mWorker->stop():false;
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

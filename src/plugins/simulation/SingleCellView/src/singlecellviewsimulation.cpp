/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view simulation
//==============================================================================

#include "cellmlfileruntime.h"
#include "coredatastore.h"
#include "corenlasolver.h"
#include "csvdatastore.h"
#include "singlecellviewcontentswidget.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewinformationwidget.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewwidget.h"
#include "solverinterface.h"

//==============================================================================

#include <QtMath>
#include <QtNumeric>

//==============================================================================

#include <QFile>
#include <QIODevice>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewSimulationData::SingleCellViewSimulationData(CellMLSupport::CellmlFileRuntime *pRuntime,
                                                           const SolverInterfaces &pSolverInterfaces) :
    mRuntime(pRuntime),
    mSolverInterfaces(pSolverInterfaces),
    mDelay(0),
    mStartingPoint(0.0),
    mEndingPoint(1000.0),
    mPointInterval(1.0),
    mOdeSolverName(QString()),
    mOdeSolverProperties(CoreSolver::Properties()),
    mDaeSolverName(QString()),
    mDaeSolverProperties(CoreSolver::Properties()),
    mNlaSolverName(QString()),
    mNlaSolverProperties(CoreSolver::Properties())
{
    // Create our various arrays, if possible

    if (pRuntime) {
        // Create our various arrays to compute our model

        mConstants = new double[pRuntime->constantsCount()];
        mRates     = new double[pRuntime->ratesCount()];
        mStates    = new double[pRuntime->statesCount()];
        mAlgebraic = new double[pRuntime->algebraicCount()];
        mCondVar   = new double[pRuntime->condVarCount()];

        // Create our various arrays to keep track of our various initial values

        mInitialConstants = new double[pRuntime->constantsCount()];
        mInitialStates    = new double[pRuntime->statesCount()];
    } else {
        mConstants = mRates = mStates = mAlgebraic = mCondVar = 0;
        mInitialConstants = mInitialStates = 0;
    }
}

//==============================================================================

SingleCellViewSimulationData::~SingleCellViewSimulationData()
{
    // Delete some internal objects

    delete[] mConstants;
    delete[] mRates;
    delete[] mStates;
    delete[] mAlgebraic;
    delete[] mCondVar;

    delete[] mInitialConstants;
    delete[] mInitialStates;
}

//==============================================================================

double * SingleCellViewSimulationData::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double * SingleCellViewSimulationData::rates() const
{
    // Return our rates array

    return mRates;
}

//==============================================================================

double * SingleCellViewSimulationData::states() const
{
    // Return our states array

    return mStates;
}

//==============================================================================

double * SingleCellViewSimulationData::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

double * SingleCellViewSimulationData::condVar() const
{
    // Return our condVar array

    return mCondVar;
}

//==============================================================================

int SingleCellViewSimulationData::delay() const
{
    // Return our delay

    return mDelay;
}

//==============================================================================

void SingleCellViewSimulationData::setDelay(const int &pDelay)
{
    // Set our delay

    mDelay = pDelay;
}

//==============================================================================

double SingleCellViewSimulationData::startingPoint() const
{
    // Return our starting point

    return mStartingPoint;
}

//==============================================================================

void SingleCellViewSimulationData::setStartingPoint(const double &pStartingPoint,
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

double SingleCellViewSimulationData::endingPoint() const
{
    // Return our ending point

    return mEndingPoint;
}

//==============================================================================

void SingleCellViewSimulationData::setEndingPoint(const double &pEndingPoint)
{
    // Set our ending point

    mEndingPoint = pEndingPoint;
}

//==============================================================================

double SingleCellViewSimulationData::pointInterval() const
{
    // Return our point interval

    return mPointInterval;
}

//==============================================================================

void SingleCellViewSimulationData::setPointInterval(const double &pPointInterval)
{
    // Set our point interval

    mPointInterval = pPointInterval;
}

//==============================================================================

QString SingleCellViewSimulationData::odeSolverName() const
{
    // Return our ODE solver name

    return (mRuntime && mRuntime->needOdeSolver())?mOdeSolverName:QString();
}

//==============================================================================

void SingleCellViewSimulationData::setOdeSolverName(const QString &pOdeSolverName)
{
    if (!mRuntime)
        return;

    // Set our ODE solver name and reset its properties

    if (mRuntime->needOdeSolver()) {
        mOdeSolverName = pOdeSolverName;

        mOdeSolverProperties.clear();
    }
}

//==============================================================================

CoreSolver::Properties SingleCellViewSimulationData::odeSolverProperties() const
{
    // Return our ODE solver's properties

    return (mRuntime && mRuntime->needOdeSolver())?mOdeSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellViewSimulationData::addOdeSolverProperty(const QString &pName,
                                                        const QVariant &pValue)
{
    if (!mRuntime)
        return;

    // Add an ODE solver property

    if (mRuntime->needOdeSolver())
        mOdeSolverProperties.insert(pName, pValue);
}

//==============================================================================

QString SingleCellViewSimulationData::daeSolverName() const
{
    // Return our DAE solver name

    return (mRuntime && mRuntime->needDaeSolver())?mDaeSolverName:QString();
}

//==============================================================================

void SingleCellViewSimulationData::setDaeSolverName(const QString &pDaeSolverName)
{
    if (!mRuntime)
        return;

    // Set our DAE solver name and reset its properties

    if (mRuntime->needDaeSolver()) {
        mDaeSolverName = pDaeSolverName;

        mDaeSolverProperties.clear();
    }
}

//==============================================================================

CoreSolver::Properties SingleCellViewSimulationData::daeSolverProperties() const
{
    // Return our DAE solver's properties

    return (mRuntime && mRuntime->needDaeSolver())?mDaeSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellViewSimulationData::addDaeSolverProperty(const QString &pName,
                                                        const QVariant &pValue)
{
    if (!mRuntime)
        return;

    // Add an DAE solver property

    if (mRuntime->needDaeSolver())
        mDaeSolverProperties.insert(pName, pValue);
}

//==============================================================================

QString SingleCellViewSimulationData::nlaSolverName() const
{
    // Return our NLA solver name

    return (mRuntime && mRuntime->needNlaSolver())?mNlaSolverName:QString();
}

//==============================================================================

void SingleCellViewSimulationData::setNlaSolverName(const QString &pNlaSolverName,
                                                    const bool &pReset)
{
    if (!mRuntime)
        return;

    // Set our NLA solver name and reset its properties

    if (mRuntime->needNlaSolver()) {
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

CoreSolver::Properties SingleCellViewSimulationData::nlaSolverProperties() const
{
    // Return our NLA solver's properties

    return (mRuntime && mRuntime->needNlaSolver())?mNlaSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellViewSimulationData::addNlaSolverProperty(const QString &pName,
                                                        const QVariant &pValue,
                                                        const bool &pReset)
{
    if (!mRuntime)
        return;

    // Add an NLA solver property

    if (mRuntime->needNlaSolver()) {
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

void SingleCellViewSimulationData::reset(const bool &pInitialize)
{
    if (!mRuntime)
        return;

    // Reset our parameter values which means both initialising our 'constants'
    // and computing our 'computed constants' and 'variables'
    // Note #1: we must check whether our runtime needs NLA solver and, if so,
    //          then retrieve an instance of our NLA solver since some of the
    //          resetting may require solving one or several NLA systems...
    // Note #2: recomputeComputedConstantsAndVariables() will let people know
    //          that our data has changed...

    CoreSolver::CoreNlaSolver *nlaSolver = 0;

    if (mRuntime->needNlaSolver()) {
        // Retrieve an instance of our NLA solver

        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->solverName().compare(mNlaSolverName)) {
                // The requested NLA solver was found, so retrieve an instance
                // of it

                nlaSolver = static_cast<CoreSolver::CoreNlaSolver *>(solverInterface->solverInstance());

                // Keep track of our NLA solver, so that doNonLinearSolve() can
                // work as expected

                CoreSolver::setNlaSolver(mRuntime->address(), nlaSolver);

                break;
            }

        // Make sure that we have found our NLA solver
        // Note: this should never happen, but we never know...

        if (!nlaSolver) {
            emit error(tr("the NLA solver could not be found"));

            return;
        }

        // Keep track of any error that might be reported by our NLA solver

        connect(nlaSolver, SIGNAL(error(const QString &)),
                this, SIGNAL(error(const QString &)));

        // Initialise our NLA solver

        nlaSolver->setProperties(mNlaSolverProperties);
    }

    // Reset our parameter values

    if (pInitialize) {
        memset(mConstants, 0, mRuntime->constantsCount()*CoreSolver::SizeOfDouble);
        memset(mRates, 0, mRuntime->ratesCount()*CoreSolver::SizeOfDouble);
        memset(mStates, 0, mRuntime->statesCount()*CoreSolver::SizeOfDouble);
        memset(mAlgebraic, 0, mRuntime->algebraicCount()*CoreSolver::SizeOfDouble);
        memset(mCondVar, 0, mRuntime->condVarCount()*CoreSolver::SizeOfDouble);

        mRuntime->initializeConstants()(mConstants, mRates, mStates);
    }

    recomputeComputedConstantsAndVariables(mStartingPoint, pInitialize);

    // Recompute our computed constants and variables in case our model needs an
    // NLA solver
    // Note: this is very much empiric. Indeed, we noticed that using
    //       https://gist.github.com/agarny/b051897560031a2591a2,
    //       x=3.0000006396753 after calling
    //       recomputeComputedConstantsAndVariables(), but then if we call
    //       recomputeComputedConstantsAndVariables() again, we get
    //       x=3.00000000000007...

    if (mRuntime->needNlaSolver())
        recomputeComputedConstantsAndVariables(mStartingPoint, pInitialize);

    // Delete our NLA solver, if any

    if (nlaSolver) {
        delete nlaSolver;

        CoreSolver::unsetNlaSolver(mRuntime->address());
    }

    // Keep track of our various initial values

    if (pInitialize) {
        memcpy(mInitialConstants, mConstants, mRuntime->constantsCount()*CoreSolver::SizeOfDouble);
        memcpy(mInitialStates, mStates, mRuntime->statesCount()*CoreSolver::SizeOfDouble);
    }

    // Let people know whether our data is 'cleaned', i.e. not modified

    emit modified(isModified());
}

//==============================================================================

void SingleCellViewSimulationData::recomputeComputedConstantsAndVariables(const double &pCurrentPoint,
                                                                          const bool &pFullComputeComputedConstants)
{
    if (!mRuntime)
        return;

    // Recompute our 'computed constants' and 'variables', if possible

    if (mRuntime->isValid()) {
        double *realStates = mStates;

        if (!pFullComputeComputedConstants)
            realStates = new double[mRuntime->statesCount()];

        mRuntime->computeComputedConstants()(mConstants, mRates, realStates);

        if (!pFullComputeComputedConstants)
            delete[] realStates;

        if (mRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)
            mRuntime->computeOdeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);
        else
            mRuntime->computeDaeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic, mCondVar);

        // Let people know that our data has been updated

        emit updated(pCurrentPoint);
    }
}

//==============================================================================

void SingleCellViewSimulationData::recomputeVariables(const double &pCurrentPoint)
{
    if (!mRuntime)
        return;

    // Recompute our 'variables'

    if (mRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)
        mRuntime->computeOdeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);
    else
        mRuntime->computeDaeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic, mCondVar);
}

//==============================================================================

bool SingleCellViewSimulationData::isModified() const
{
    if (!mRuntime)
        return false;

    // Check whether any of our constants or states has been modified
    // Note: we start with our states since they are more likely to be modified
    //       than our constants...

    for (int i = 0, iMax = mRuntime->statesCount(); i < iMax; ++i)
        if (!qIsFinite(mStates[i]) || !qFuzzyCompare(mStates[i], mInitialStates[i]))
            return true;

    for (int i = 0, iMax = mRuntime->constantsCount(); i < iMax; ++i)
        if (!qIsFinite(mConstants[i]) || !qFuzzyCompare(mConstants[i], mInitialConstants[i]))
            return true;

    return false;
}

//==============================================================================

void SingleCellViewSimulationData::checkForModifications()
{
    // Let people know whether any of our constants or states has been modified

    emit modified(isModified());
}

//==============================================================================

SingleCellViewSimulationResults::SingleCellViewSimulationResults(CellMLSupport::CellmlFileRuntime *pRuntime,
                                                                 SingleCellViewSimulation *pSimulation) :
    mRuntime(pRuntime),
    mSimulation(pSimulation),
    mSize(0),
    mDataStore(0),
    mPoints(0),
    mConstants(CoreDataStore::DataStoreVariables()),
    mRates(CoreDataStore::DataStoreVariables()),
    mStates(CoreDataStore::DataStoreVariables()),
    mAlgebraic(CoreDataStore::DataStoreVariables())
{
}

//==============================================================================

SingleCellViewSimulationResults::~SingleCellViewSimulationResults()
{
    // Delete some internal objects

    deleteDataStore();
}

//==============================================================================

QString SingleCellViewSimulationResults::uri(const QStringList &pComponentHierarchy,
                                             const QString &pName)
{
    // Generate an URI using the given component hierarchy and name

    QString res = pComponentHierarchy.join("/")+"/"+pName;

    return res.replace("'", "/prime");
}

//==============================================================================

bool SingleCellViewSimulationResults::createDataStore()
{
    // Note: the boolean value we return is true if we have had no problem
    //       creating our data store, false otherwise. This is the reason, for
    //       example, we return true when there is either no runtime or if the
    //       simulation size is zero...

    // Delete the previous data store, if any

    deleteDataStore();

    // Make sure that we have a runtime

    if (!mRuntime)
        return true;

    // Retrieve the size of our data and make sure that it is valid

    qulonglong simulationSize = qulonglong(mSimulation->size());

    if (!simulationSize)
        return true;

    // Create our data store and populate it with a variable of integration, as
    // well as with constant, rate, state and algebraic variables

    try {
        mDataStore = new CoreDataStore::CoreDataStore(simulationSize);

        mPoints = mDataStore->addVoi();
        mConstants = mDataStore->addVariables(mRuntime->constantsCount(), mSimulation->data()->constants());
        mRates = mDataStore->addVariables(mRuntime->ratesCount(), mSimulation->data()->rates());
        mStates = mDataStore->addVariables(mRuntime->statesCount(), mSimulation->data()->states());
        mAlgebraic = mDataStore->addVariables(mRuntime->algebraicCount(), mSimulation->data()->algebraic());
    } catch (...) {
        deleteDataStore();

        return false;
    }

    // Customise our variable of integration, as well as our constant, rate,
    // state and algebraic variables

    mPoints->setUri(uri(mRuntime->variableOfIntegration()->componentHierarchy(),
                        mRuntime->variableOfIntegration()->name()));
    mPoints->setLabel(mRuntime->variableOfIntegration()->name());
    mPoints->setUnit(mRuntime->variableOfIntegration()->unit());

    for (int i = 0, iMax = mRuntime->parameters().count(); i < iMax; ++i) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = mRuntime->parameters()[i];
        CoreDataStore::DataStoreVariable *variable = 0;

        switch (parameter->type()) {
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
            // Not a type in which we are interested, so do nothing

            ;
        }

        if (variable) {
            variable->setUri(uri(parameter->componentHierarchy(),
                                 parameter->formattedName()));
            variable->setLabel(parameter->formattedName());
            variable->setUnit(parameter->formattedUnit(mRuntime->variableOfIntegration()->unit()));
        }
    }

    return true;
}

//==============================================================================

void SingleCellViewSimulationResults::deleteDataStore()
{
    // Delete our data store

    delete mDataStore;

    mDataStore = 0;
}

//==============================================================================

bool SingleCellViewSimulationResults::reset(const bool &pCreateDataStore)
{
    // Reset our size

    mSize = 0;

    // Reset our data store

    if (pCreateDataStore) {
        return createDataStore();
    } else {
        deleteDataStore();

        return true;
    }
}

//==============================================================================

void SingleCellViewSimulationResults::addPoint(const double &pPoint)
{
    // Add the data to our data store

    mDataStore->setValues(mSize, pPoint);

    ++mSize;
}

//==============================================================================

qulonglong SingleCellViewSimulationResults::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

double * SingleCellViewSimulationResults::points() const
{
    // Return our points

    return mPoints?mPoints->values():0;
}

//==============================================================================

double * SingleCellViewSimulationResults::constants(const int &pIndex) const
{
    // Return our constants data at the given index

    return mConstants.isEmpty()?0:mConstants[pIndex]->values();
}

//==============================================================================

double * SingleCellViewSimulationResults::rates(const int &pIndex) const
{
    // Return our rates data at the given index

    return mRates.isEmpty()?0:mRates[pIndex]->values();
}

//==============================================================================

double * SingleCellViewSimulationResults::states(const int &pIndex) const
{
    // Return our states data at the given index

    return mStates.isEmpty()?0:mStates[pIndex]->values();
}

//==============================================================================

double * SingleCellViewSimulationResults::algebraic(const int &pIndex) const
{
    // Return our algebraic data at the given index

    return mAlgebraic.isEmpty()?0:mAlgebraic[pIndex]->values();
}

//==============================================================================

bool SingleCellViewSimulationResults::exportToCsv(const QString &pFileName) const
{
    // Export of all of our data to a CSV file

    return CSVDataStore::exportDataStore(mDataStore, pFileName);
}

//==============================================================================

SingleCellViewSimulation::SingleCellViewSimulation(const QString &pFileName,
                                                   CellMLSupport::CellmlFileRuntime *pRuntime,
                                                   const SolverInterfaces &pSolverInterfaces) :
    mWorker(0),
    mFileName(pFileName),
    mRuntime(pRuntime),
    mSolverInterfaces(pSolverInterfaces),
    mData(new SingleCellViewSimulationData(pRuntime, pSolverInterfaces)),
    mResults(new SingleCellViewSimulationResults(pRuntime, this))
{
    // Keep track of any error occurring in our data

    connect(mData, SIGNAL(error(const QString &)),
            this, SIGNAL(error(const QString &)));
}

//==============================================================================

SingleCellViewSimulation::~SingleCellViewSimulation()
{
    // Stop our worker
    // Note: we don't need to delete mWorker since it will be done as part of
    //       its thread being stopped...

    stop();

    // Delete some internal objects

    delete mResults;
    delete mData;
}

//==============================================================================

QString SingleCellViewSimulation::fileName() const
{
    // Retrieve and return our file name

    return mFileName;
}

//==============================================================================

SingleCellViewSimulationData * SingleCellViewSimulation::data() const
{
    // Retrieve and return our data

    return mData;
}

//==============================================================================

SingleCellViewSimulationResults * SingleCellViewSimulation::results() const
{
    // Return our results

    return mResults;
}

//==============================================================================

bool SingleCellViewSimulation::isRunning() const
{
    // Return whether we are running

    return mWorker?mWorker->isRunning():false;
}

//==============================================================================

bool SingleCellViewSimulation::isPaused() const
{
    // Return whether we are paused

    return mWorker?mWorker->isPaused():false;
}

//==============================================================================

double SingleCellViewSimulation::currentPoint() const
{
    // Return our current point

    return mWorker?mWorker->currentPoint():data()->startingPoint();
}

//==============================================================================

double SingleCellViewSimulation::progress() const
{
    // Return our progress

    return mWorker?mWorker->progress():0.0;
}

//==============================================================================

int SingleCellViewSimulation::delay() const
{
    // Return our delay

    return mData->delay();
}

//==============================================================================

void SingleCellViewSimulation::setDelay(const int &pDelay)
{
    // Set our delay

    mData->setDelay(pDelay);
}

//==============================================================================

double SingleCellViewSimulation::requiredMemory()
{
    // Determine and return the amount of required memory to run our simulation
    // Note #1: we return the amount as a double rather than a qulonglong (as we
    //          do when retrieving the total/free amount of memory available;
    //          see [OpenCOR]/src/plugins/miscellaneous/Core/src/guiutils.cpp)
    //          in case a simulation requires an insane amount of memory...
    // Note #2: the 1.0 is for mPoints in SingleCellViewSimulationResults...

    if (mRuntime)
        return  size()
               *( 1
                 +mRuntime->constantsCount()
                 +mRuntime->ratesCount()
                 +mRuntime->statesCount()
                 +mRuntime->algebraicCount())
               *CoreSolver::SizeOfDouble;
    else
        return 0.0;
}

//==============================================================================

bool SingleCellViewSimulation::simulationSettingsOk(const bool &pEmitError)
{
    // Check and return whether our simulation settings are sound

    if (mData->startingPoint() == mData->endingPoint()) {
        if (pEmitError)
            emit error(tr("the starting and ending points cannot have the same value"));

        return false;
    } else if (mData->pointInterval() == 0) {
        if (pEmitError)
            emit error(tr("the point interval cannot be equal to zero"));

        return false;
    } else if (   (mData->startingPoint() < mData->endingPoint())
             && (mData->pointInterval() < 0)) {
        if (pEmitError)
            emit error(tr("the ending point is greater than the starting point, so the point interval should be greater than zero"));

        return false;
    } else if (   (mData->startingPoint() > mData->endingPoint())
             && (mData->pointInterval() > 0)) {
        if (pEmitError)
            emit error(tr("the ending point is smaller than the starting point, so the point interval should be smaller than zero"));

        return false;
    } else {
        return true;
    }
}

//==============================================================================

double SingleCellViewSimulation::size()
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

bool SingleCellViewSimulation::run()
{
    if (!mRuntime)
        return false;

    // Initialise our worker, if not active

    if (mWorker) {
        return false;
    } else {
        // Make sure that that the simulation settings we were given are sound

        if (!simulationSettingsOk())
            return false;

        // Create our worker

        mWorker = new SingleCellViewSimulationWorker(mSolverInterfaces, mRuntime, this, &mWorker);

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

bool SingleCellViewSimulation::pause()
{
    // Pause our worker

    if (mWorker)
        return mWorker->pause();
    else
        return false;
}

//==============================================================================

bool SingleCellViewSimulation::resume()
{
    // Resume our worker

    if (mWorker)
        return mWorker->resume();
    else
        return false;
}

//==============================================================================

bool SingleCellViewSimulation::stop()
{
    // Stop our worker

    if (mWorker)
        return mWorker->stop();
    else
        return false;
}

//==============================================================================

bool SingleCellViewSimulation::reset()
{
    // Reset our data

    mData->reset();

    // Reset our worker

    if (mWorker)
        return mWorker->reset();
    else
        return false;
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

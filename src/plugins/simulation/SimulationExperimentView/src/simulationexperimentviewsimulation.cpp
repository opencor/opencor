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
// Simulation Experiment view simulation
//==============================================================================

#include "cellmlfile.h"
#include "cellmlfileruntime.h"
#include "interfaces.h"
#include "simulationexperimentviewsimulation.h"
#include "simulationexperimentviewsimulationwidget.h"

//==============================================================================

#include <QtMath>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewSimulationData::SimulationExperimentViewSimulationData(SimulationExperimentViewSimulation *pSimulation) :
    mSimulation(pSimulation),
    mRuntime(pSimulation->runtime()),
    mDelay(0),
    mStartingPoint(0.0),
    mEndingPoint(1000.0),
    mPointInterval(1.0),
    mOdeSolverName(QString()),
    mOdeSolverProperties(Solver::Solver::Properties()),
    mDaeSolverName(QString()),
    mDaeSolverProperties(Solver::Solver::Properties()),
    mNlaSolverName(QString()),
    mNlaSolverProperties(Solver::Solver::Properties()),
    mResultsDataStore(0)
{
    // Create our various arrays

    createArrays();

    createResultsDataStore();
}

//==============================================================================

SimulationExperimentViewSimulationData::~SimulationExperimentViewSimulationData()
{
    // Delete some internal objects

    deleteArrays();
}

//==============================================================================

void SimulationExperimentViewSimulationData::update()
{
    // Update ourselves by updating our runtime, and deleting and recreating our
    // arrays

    mRuntime = mSimulation->runtime();

    deleteArrays();
    createArrays();
}

//==============================================================================

SimulationExperimentViewSimulation * SimulationExperimentViewSimulationData::simulation() const
{
    // Return our simulation

    return mSimulation;
}

//==============================================================================

double * SimulationExperimentViewSimulationData::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double * SimulationExperimentViewSimulationData::rates() const
{
    // Return our rates array

    return mRates;
}

//==============================================================================

double * SimulationExperimentViewSimulationData::states() const
{
    // Return our states array

    return mStates;
}

//==============================================================================

double * SimulationExperimentViewSimulationData::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

double * SimulationExperimentViewSimulationData::condVar() const
{
    // Return our condVar array

    return mCondVar;
}

//==============================================================================

int SimulationExperimentViewSimulationData::delay() const
{
    // Return our delay

    return mDelay;
}

//==============================================================================

void SimulationExperimentViewSimulationData::setDelay(const int &pDelay)
{
    // Set our delay

    mDelay = pDelay;
}

//==============================================================================

double SimulationExperimentViewSimulationData::startingPoint() const
{
    // Return our starting point

    return mStartingPoint;
}

//==============================================================================

void SimulationExperimentViewSimulationData::setStartingPoint(const double &pStartingPoint,
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

double SimulationExperimentViewSimulationData::endingPoint() const
{
    // Return our ending point

    return mEndingPoint;
}

//==============================================================================

void SimulationExperimentViewSimulationData::setEndingPoint(const double &pEndingPoint)
{
    // Set our ending point

    mEndingPoint = pEndingPoint;
}

//==============================================================================

double SimulationExperimentViewSimulationData::pointInterval() const
{
    // Return our point interval

    return mPointInterval;
}

//==============================================================================

void SimulationExperimentViewSimulationData::setPointInterval(const double &pPointInterval)
{
    // Set our point interval

    mPointInterval = pPointInterval;
}

//==============================================================================

SolverInterface * SimulationExperimentViewSimulationData::solverInterface(const QString &pSolverName) const
{
    // Return the named solver interface, if any

    foreach (SolverInterface *solverInterface, Core::solverInterfaces()) {
        if (!solverInterface->solverName().compare(pSolverName))
            return solverInterface;
    }

    return 0;
}

//==============================================================================

SolverInterface * SimulationExperimentViewSimulationData::odeSolverInterface() const
{
    // Return our ODE solver interface, if any

    return solverInterface(odeSolverName());
}

//==============================================================================

QString SimulationExperimentViewSimulationData::odeSolverName() const
{
    // Return our ODE solver name

    return (mRuntime && mRuntime->needOdeSolver())?mOdeSolverName:QString();
}

//==============================================================================

void SimulationExperimentViewSimulationData::setOdeSolverName(const QString &pOdeSolverName)
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

Solver::Solver::Properties SimulationExperimentViewSimulationData::odeSolverProperties() const
{
    // Return our ODE solver's properties

    return (mRuntime && mRuntime->needOdeSolver())?mOdeSolverProperties:Solver::Solver::Properties();
}

//==============================================================================

void SimulationExperimentViewSimulationData::addOdeSolverProperty(const QString &pName,
                                                                  const QVariant &pValue)
{
    if (!mRuntime)
        return;

    // Add an ODE solver property

    if (mRuntime->needOdeSolver())
        mOdeSolverProperties.insert(pName, pValue);
}

//==============================================================================

SolverInterface * SimulationExperimentViewSimulationData::daeSolverInterface() const
{
    // Return our DAE solver interface, if any

    return solverInterface(daeSolverName());
}

//==============================================================================

QString SimulationExperimentViewSimulationData::daeSolverName() const
{
    // Return our DAE solver name

    return (mRuntime && mRuntime->needDaeSolver())?mDaeSolverName:QString();
}

//==============================================================================

void SimulationExperimentViewSimulationData::setDaeSolverName(const QString &pDaeSolverName)
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

Solver::Solver::Properties SimulationExperimentViewSimulationData::daeSolverProperties() const
{
    // Return our DAE solver's properties

    return (mRuntime && mRuntime->needDaeSolver())?mDaeSolverProperties:Solver::Solver::Properties();
}

//==============================================================================

void SimulationExperimentViewSimulationData::addDaeSolverProperty(const QString &pName,
                                                                  const QVariant &pValue)
{
    if (!mRuntime)
        return;

    // Add an DAE solver property

    if (mRuntime->needDaeSolver())
        mDaeSolverProperties.insert(pName, pValue);
}

//==============================================================================

SolverInterface * SimulationExperimentViewSimulationData::nlaSolverInterface() const
{
    // Return our NLA solver interface, if any

    return solverInterface(nlaSolverName());
}

//==============================================================================

QString SimulationExperimentViewSimulationData::nlaSolverName() const
{
    // Return our NLA solver name

    return (mRuntime && mRuntime->needNlaSolver())?mNlaSolverName:QString();
}

//==============================================================================

void SimulationExperimentViewSimulationData::setNlaSolverName(const QString &pNlaSolverName,
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

Solver::Solver::Properties SimulationExperimentViewSimulationData::nlaSolverProperties() const
{
    // Return our NLA solver's properties

    return (mRuntime && mRuntime->needNlaSolver())?mNlaSolverProperties:Solver::Solver::Properties();
}

//==============================================================================

void SimulationExperimentViewSimulationData::addNlaSolverProperty(const QString &pName,
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

void SimulationExperimentViewSimulationData::reset(const bool &pInitialize)
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

    Solver::NlaSolver *nlaSolver = 0;

    if (mRuntime->needNlaSolver()) {
        // Set our NLA solver
        // Note: we unset it at the end of this method...

        nlaSolver = static_cast<Solver::NlaSolver *>(nlaSolverInterface()->solverInstance());

        Solver::setNlaSolver(mRuntime->address(), nlaSolver);

        // Keep track of any error that might be reported by our NLA solver

        connect(nlaSolver, SIGNAL(error(const QString &)),
                this, SIGNAL(error(const QString &)));

        // Initialise our NLA solver

        nlaSolver->setProperties(mNlaSolverProperties);
    }

    // Reset our parameter values

    if (pInitialize) {
        memset(mConstants, 0, mRuntime->constantsCount()*Solver::SizeOfDouble);
        memset(mRates, 0, mRuntime->ratesCount()*Solver::SizeOfDouble);
        memset(mStates, 0, mRuntime->statesCount()*Solver::SizeOfDouble);
        memset(mAlgebraic, 0, mRuntime->algebraicCount()*Solver::SizeOfDouble);
        memset(mCondVar, 0, mRuntime->condVarCount()*Solver::SizeOfDouble);

        mRuntime->initializeConstants()(mConstants, mRates, mStates);
    }

    recomputeComputedConstantsAndVariables(mStartingPoint, pInitialize);

    // Delete our NLA solver, if any

    if (nlaSolver) {
        delete nlaSolver;

        Solver::unsetNlaSolver(mRuntime->address());
    }

    // Keep track of our various initial values

    if (pInitialize) {
        memcpy(mInitialConstants, mConstants, mRuntime->constantsCount()*Solver::SizeOfDouble);
        memcpy(mInitialStates, mStates, mRuntime->statesCount()*Solver::SizeOfDouble);
    }

    // Let people know whether our data is 'cleaned', i.e. not modified
    // Note: no point in checking if we are initialising...

    if (!pInitialize)
        emit modified(isModified());
}

//==============================================================================

void SimulationExperimentViewSimulationData::recomputeComputedConstantsAndVariables(const double &pCurrentPoint,
                                                                                    const bool &pInitialize)
{
    if (!mRuntime)
        return;

    // Make sure that our runtime is valid

    if (mRuntime->isValid()) {
        // Recompute our 'computed constants'

        mRuntime->computeComputedConstants()(mConstants, mRates,
                                             pInitialize?mStates:mDummyStates);

        // Recompute some 'constant' algebraic variables

        if (mRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)
            mRuntime->computeOdeRates()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);

        // Recompute our 'variables'

        recomputeVariables(pCurrentPoint);

        // Let people know that our data has been updated

        emit updated(pCurrentPoint);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationData::recomputeVariables(const double &pCurrentPoint)
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

bool SimulationExperimentViewSimulationData::isModified() const
{
    if (!mRuntime)
        return false;

    // Check whether any of our constants or states has been modified
    // Note: we start with our states since they are more likely to be modified
    //       than our constants...

    for (int i = 0, iMax = mRuntime->statesCount(); i < iMax; ++i) {
        if (!qIsFinite(mStates[i]) || !qFuzzyCompare(mStates[i], mInitialStates[i]))
            return true;
    }

    for (int i = 0, iMax = mRuntime->constantsCount(); i < iMax; ++i) {
        if (!qIsFinite(mConstants[i]) || !qFuzzyCompare(mConstants[i], mInitialConstants[i]))
            return true;
    }

    return false;
}

//==============================================================================

void SimulationExperimentViewSimulationData::checkForModifications()
{
    // Let people know whether any of our constants or states has been modified

    emit modified(isModified());
}

//==============================================================================

void SimulationExperimentViewSimulationData::createArrays()
{
    // Create our various arrays, if possible

    if (mRuntime) {
        // Create our various arrays to compute our model

        mConstants = new double[mRuntime->constantsCount()];
        mRates = new double[mRuntime->ratesCount()];
        mStates = new double[mRuntime->statesCount()];
        mDummyStates = new double[mRuntime->statesCount()];
        mAlgebraic = new double[mRuntime->algebraicCount()];
        mCondVar = new double[mRuntime->condVarCount()];

        // Create our various arrays to keep track of our various initial values

        mInitialConstants = new double[mRuntime->constantsCount()];
        mInitialStates = new double[mRuntime->statesCount()];

        if (mResultsDataStore) {
            mConstantVariables.setNextValuePtrs(mConstants);
            mRateVariables.setNextValuePtrs(mRates);
            mStateVariables.setNextValuePtrs(mStates);
            mAlgebraicVariables.setNextValuePtrs(mAlgebraic);
        }

    } else {
        mConstants = mRates = mStates = mDummyStates = mAlgebraic = mCondVar = 0;
        mInitialConstants = mInitialStates = 0;
    }
}

//==============================================================================

void SimulationExperimentViewSimulationData::deleteArrays()
{
    // Delete our various arrays

    delete[] mConstants;
    delete[] mRates;
    delete[] mStates;
    delete[] mDummyStates;
    delete[] mAlgebraic;
    delete[] mCondVar;

    delete[] mInitialConstants;
    delete[] mInitialStates;

    if (mResultsDataStore) {
        mConstantVariables.clearNextValuePtrs();
        mRateVariables.clearNextValuePtrs();
        mStateVariables.clearNextValuePtrs();
        mAlgebraicVariables.clearNextValuePtrs();
    }
}

//==============================================================================

QString SimulationExperimentViewSimulationData::uri(const QStringList &pComponentHierarchy,
                                                    const QString &pName)
{
    // Generate an URI using the given component hierarchy and name

    QString res = pComponentHierarchy.join('/')+"/"+pName;

    return res.replace('\'', "/prime");
}

//==============================================================================

void SimulationExperimentViewSimulationData::createResultsDataStore()
{
    // Create our data store and populate it with a variable of integration, as
    // well as with constant, rate, state and algebraic variables

    mResultsDataStore = new DataStore::DataStore(mRuntime->cellmlFile()->xmlBase());

    mPointVariable = mResultsDataStore->addVoi();
    mConstantVariables = mResultsDataStore->addVariables(mRuntime->constantsCount(), mConstants);
    mRateVariables = mResultsDataStore->addVariables(mRuntime->ratesCount(), mRates);
    mStateVariables = mResultsDataStore->addVariables(mRuntime->statesCount(), mStates);
    mAlgebraicVariables = mResultsDataStore->addVariables(mRuntime->algebraicCount(), mAlgebraic);

    // Customise our variable of integration, as well as our constant, rate,
    // state and algebraic variables

    for (int i = 0, iMax = mRuntime->parameters().count(); i < iMax; ++i) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = mRuntime->parameters()[i];
        DataStore::DataStoreVariable *variable = 0;

        switch (parameter->type()) {
        case CellMLSupport::CellmlFileRuntimeParameter::Voi:
            mPointVariable->setIcon(parameter->icon());
            mPointVariable->setUri(uri(mRuntime->variableOfIntegration()->componentHierarchy(),
                                   mRuntime->variableOfIntegration()->name()));
            mPointVariable->setLabel(mRuntime->variableOfIntegration()->name());
            mPointVariable->setUnit(mRuntime->variableOfIntegration()->unit());

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Constant:
        case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
            variable = mConstantVariables[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Rate:
            variable = mRateVariables[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::State:
            variable = mStateVariables[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
            variable = mAlgebraicVariables[parameter->index()];

            break;
        default:
            // Not a relevant type, so do nothing

            ;
        }

        if (variable) {
            variable->setIcon(parameter->icon());
            variable->setUri(uri(parameter->componentHierarchy(),
                                 parameter->formattedName()));
            variable->setLabel(parameter->formattedName());
            variable->setUnit(parameter->formattedUnit(mRuntime->variableOfIntegration()->unit()));
        }
    }
}

//==============================================================================

DataStore::DataStore * SimulationExperimentViewSimulationData::resultsDataStore() const
{
    return mResultsDataStore;
}

//==============================================================================

DataStore::DataStoreVariable * SimulationExperimentViewSimulationData::pointVariable() const
{
    return mPointVariable;
}

//==============================================================================

DataStore::DataStoreVariables SimulationExperimentViewSimulationData::constantVariables() const
{
    return mConstantVariables;
}

//==============================================================================

DataStore::DataStoreVariables SimulationExperimentViewSimulationData::rateVariables() const
{
    return mRateVariables;
}

//==============================================================================

DataStore::DataStoreVariables SimulationExperimentViewSimulationData::stateVariables() const
{
    return mStateVariables;
}

//==============================================================================

DataStore::DataStoreVariables SimulationExperimentViewSimulationData::algebraicVariables() const
{
    return mAlgebraicVariables;
}

//==============================================================================

SimulationExperimentViewSimulationResults::SimulationExperimentViewSimulationResults(SimulationExperimentViewSimulation *pSimulation) :
    mSimulation(pSimulation),
    mRuntime(pSimulation->runtime()),
    mDataStore(pSimulation->data()->resultsDataStore()),
    mPointVariable(pSimulation->data()->pointVariable()),
    mConstantVariables(pSimulation->data()->constantVariables()),
    mRateVariables(pSimulation->data()->rateVariables()),
    mStateVariables(pSimulation->data()->stateVariables()),
    mAlgebraicVariables(pSimulation->data()->algebraicVariables())
{
}

//==============================================================================

SimulationExperimentViewSimulationResults::~SimulationExperimentViewSimulationResults()
{
    // Delete some internal objects

    deleteArrays();
}

//==============================================================================

bool SimulationExperimentViewSimulationResults::createArrays()
{
    // Note: the boolean value we return is true if we have had no problem
    //       allocating our data store arrays, false otherwise. This is the
    //       reason, for example, we return true when there is either no
    //       runtime or if the simulation size is zero...

    // Delete the previous data store arrays, if any

    mDataStore->deleteArrays();

    // Make sure that we have a runtime

    if (!mRuntime)
        return true;

    // Retrieve the size of our data and make sure that it is valid

    qulonglong simulationSize = qulonglong(mSimulation->size());

    if (!simulationSize)
        return true;

    // Allocate our data store arrays

    try {
        mDataStore->createArrays(simulationSize);

    } catch (...) {
        mDataStore->deleteArrays();

        return false;
    }

    return true;
}

//==============================================================================

void SimulationExperimentViewSimulationResults::deleteArrays()
{
    // Deallocate our data store arrays

    mDataStore->deleteArrays();
}

//==============================================================================

void SimulationExperimentViewSimulationResults::update()
{
    // Update ourselves by updating our runtime and deleting our data store arrays

    mRuntime = mSimulation->runtime();

    deleteArrays();
}

//==============================================================================

bool SimulationExperimentViewSimulationResults::reset(const bool &pAllocateArrays)
{
    // Reset our data store arrays

    if (pAllocateArrays) {
        return createArrays();
    } else {
        deleteArrays();

        return true;
    }
}

//==============================================================================

void SimulationExperimentViewSimulationResults::addPoint(const double &pPoint)
{
    // Add the data to our data store

    mDataStore->addValues(pPoint);
}

//==============================================================================

qulonglong SimulationExperimentViewSimulationResults::size() const
{
    // Return our size

    return mDataStore?mDataStore->size():0;
}

//==============================================================================

DataStore::DataStore * SimulationExperimentViewSimulationResults::dataStore() const
{
    // Return our data store

    return mDataStore;
}

//==============================================================================

double * SimulationExperimentViewSimulationResults::points() const
{
    // Return our points

    return mPointVariable?mPointVariable->values():0;
}

//==============================================================================

double * SimulationExperimentViewSimulationResults::constants(const int &pIndex) const
{
    // Return our constants data at the given index

    return mConstantVariables.isEmpty()?0:mConstantVariables[pIndex]->values();
}

//==============================================================================

double * SimulationExperimentViewSimulationResults::rates(const int &pIndex) const
{
    // Return our rates data at the given index

    return mRateVariables.isEmpty()?0:mRateVariables[pIndex]->values();
}

//==============================================================================

double * SimulationExperimentViewSimulationResults::states(const int &pIndex) const
{
    // Return our states data at the given index

    return mStateVariables.isEmpty()?0:mStateVariables[pIndex]->values();
}

//==============================================================================

double * SimulationExperimentViewSimulationResults::algebraic(const int &pIndex) const
{
    // Return our algebraic data at the given index

    return mAlgebraicVariables.isEmpty()?0:mAlgebraicVariables[pIndex]->values();
}

//==============================================================================

SimulationExperimentViewSimulation::SimulationExperimentViewSimulation(CellMLSupport::CellmlFileRuntime *pRuntime) :
    mWorker(0),
    mRuntime(pRuntime),
    mData(new SimulationExperimentViewSimulationData(this)),
    mResults(new SimulationExperimentViewSimulationResults(this))
{
    // Keep track of any error occurring in our data

    connect(mData, SIGNAL(error(const QString &)),
            this, SIGNAL(error(const QString &)));
}

//==============================================================================

SimulationExperimentViewSimulation::~SimulationExperimentViewSimulation()
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

CellMLSupport::CellmlFileRuntime * SimulationExperimentViewSimulation::runtime() const
{
    // Return our runtime

    return mRuntime;
}

//==============================================================================

QString SimulationExperimentViewSimulation::fileName() const
{
    // Return the name of the CellML file

    return mRuntime?mRuntime->cellmlFile()->fileName():QString();
}

//==============================================================================

SimulationExperimentViewSimulationData * SimulationExperimentViewSimulation::data() const
{
    // Return our data

    return mData;
}

//==============================================================================

SimulationExperimentViewSimulationResults * SimulationExperimentViewSimulation::results() const
{
    // Return our results

    return mResults;
}

//==============================================================================

void SimulationExperimentViewSimulation::update(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Update ourselves by first stopping ourselves, then by by updating our
    // runtime, and asking our data and results to update themselves too

    stop();

    mRuntime = pRuntime;

    mData->update();
    mResults->update();
}

//==============================================================================

bool SimulationExperimentViewSimulation::isRunning() const
{
    // Return whether we are running

    return mWorker?mWorker->isRunning():false;
}

//==============================================================================

bool SimulationExperimentViewSimulation::isPaused() const
{
    // Return whether we are paused

    return mWorker?mWorker->isPaused():false;
}

//==============================================================================

double SimulationExperimentViewSimulation::currentPoint() const
{
    // Return our current point

    return mWorker?mWorker->currentPoint():data()->startingPoint();
}

//==============================================================================

int SimulationExperimentViewSimulation::delay() const
{
    // Return our delay

    return mData->delay();
}

//==============================================================================

void SimulationExperimentViewSimulation::setDelay(const int &pDelay)
{
    // Set our delay

    mData->setDelay(pDelay);
}

//==============================================================================

double SimulationExperimentViewSimulation::requiredMemory()
{
    // Determine and return the amount of required memory to run our simulation
    // Note #1: we return the amount as a double rather than a qulonglong (as we
    //          do when retrieving the total/free amount of memory available;
    //          see [OpenCOR]/src/plugins/miscellaneous/Core/src/guiutils.cpp)
    //          in case a simulation requires an insane amount of memory...
    // Note #2: the 1.0 is for mPoints in
    //          SimulationExperimentViewSimulationResults...

    if (mRuntime) {
        return  size()
               *( 1.0
                 +mRuntime->constantsCount()
                 +mRuntime->ratesCount()
                 +mRuntime->statesCount()
                 +mRuntime->algebraicCount())
               *Solver::SizeOfDouble;
    } else {
        return 0.0;
    }
}

//==============================================================================

bool SimulationExperimentViewSimulation::simulationSettingsOk(const bool &pEmitSignal)
{
    // Check and return whether our simulation settings are sound

    if (mData->startingPoint() == mData->endingPoint()) {
        if (pEmitSignal)
            emit error(tr("the starting and ending points cannot have the same value"));

        return false;
    } else if (mData->pointInterval() == 0.0) {
        if (pEmitSignal)
            emit error(tr("the point interval cannot be equal to zero"));

        return false;
    } else if (   (mData->startingPoint() < mData->endingPoint())
             && (mData->pointInterval() < 0.0)) {
        if (pEmitSignal)
            emit error(tr("the ending point is greater than the starting point, so the point interval should be greater than zero"));

        return false;
    } else if (   (mData->startingPoint() > mData->endingPoint())
             && (mData->pointInterval() > 0.0)) {
        if (pEmitSignal)
            emit error(tr("the ending point is smaller than the starting point, so the point interval should be smaller than zero"));

        return false;
    } else {
        return true;
    }
}

//==============================================================================

double SimulationExperimentViewSimulation::size()
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

bool SimulationExperimentViewSimulation::run()
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

        // Make sure we have a data store in which to store results

        if (!mResults->dataStore()) {
            emit error(tr("no datastore for results"));

            return false;
        }

        // Make sure the data store is big enough to store our results

        if (size() > (mResults->dataStore()->capacity() - mResults->dataStore()->size())) {
            emit error(tr("datastore doesn't have enough capacity"));

            return false;
        }

        // Make sure we have a valid solver

        if ((mRuntime->needOdeSolver() && !mData->odeSolverInterface())
         || (!mRuntime->needOdeSolver() && !mData->daeSolverInterface())
         || (mRuntime->needNlaSolver() && !mData->nlaSolverInterface())) {
            emit error(tr("no valid solvers"));

            return false;
        }

        // Create our worker

        mWorker = new SimulationExperimentViewSimulationWorker(this, mWorker);

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

bool SimulationExperimentViewSimulation::pause()
{
    // Pause our worker

    return mWorker?mWorker->pause():false;
}

//==============================================================================

bool SimulationExperimentViewSimulation::resume()
{
    // Resume our worker

    return mWorker?mWorker->resume():false;
}

//==============================================================================

bool SimulationExperimentViewSimulation::stop()
{
    // Stop our worker

    return mWorker?mWorker->stop():false;
}

//==============================================================================

bool SimulationExperimentViewSimulation::reset()
{
    // Reset our data

    mData->reset();

    // Reset our worker

    return mWorker?mWorker->reset():false;
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

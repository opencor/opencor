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
#include "solverinterface.h"

//==============================================================================

#include <QSet>
#include <QtMath>

//==============================================================================

#include "libsbmlbegin.h"
    #include "sbml/math/FormulaParser.h"
#include "libsbmlend.h"

//==============================================================================

#include "libsedmlbegin.h"
    #include "sedml/SedAlgorithm.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedOneStep.h"
    #include "sedml/SedPlot2D.h"
    #include "sedml/SedRepeatedTask.h"
    #include "sedml/SedSimulation.h"
    #include "sedml/SedUniformTimeCourse.h"
    #include "sedml/SedVectorRange.h"
#include "libsedmlend.h"

//==============================================================================

#include <QEventLoop>
#include <QThread>

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

SimulationObject::SimulationObject(Simulation *pSimulation) :
    mSimulation(pSimulation)
{
}

//==============================================================================

Simulation * SimulationObject::simulation() const
{
    // Return our simulation

    return mSimulation;
}

//==============================================================================

SimulationData::SimulationData(Simulation *pSimulation) :
    SimulationObject(pSimulation),
    mSimulationResults(nullptr),
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
    mGradientIndices(QVector<int>()),
    mGradientsArray(nullptr),
    mSimulationDataUpdatedFunction(std::bind(&SimulationData::updateParameters, this))
{
    // Create our various arrays

    createArrays();
}

//==============================================================================

SimulationData::~SimulationData()
{
    // Delete some internal objects

    deleteArrays();
    deleteGradientsArray();
}

//==============================================================================

void SimulationData::setSimulationResults(SimulationResults *pSimulationResults)
{
    mSimulationResults = pSimulationResults;
}

//==============================================================================

void SimulationData::reload()
{
    // Reload ourselves by deleting and recreating our arrays

    deleteArrays();
    createArrays();
}

//==============================================================================

double * SimulationData::constants() const
{
    // Return our constants array

    return mConstantsArray->data();
}

//==============================================================================

double * SimulationData::rates() const
{
    // Return our rates array

    return mRatesArray->data();
}

//==============================================================================

double * SimulationData::states() const
{
    // Return our states array

    return mStatesArray->data();
}

//==============================================================================

double * SimulationData::algebraic() const
{
    // Return our algebraic array

    return mAlgebraicArray->data();
}

//==============================================================================

DataStore::DataStoreValues * SimulationData::constantsValues() const
{
    // Return our constants DataStoreValues

    return mConstantsValues;
}

//==============================================================================

DataStore::DataStoreValues * SimulationData::ratesValues() const
{
    // Return our rates DataStoreValues

    return mRatesValues;
}

//==============================================================================

DataStore::DataStoreValues * SimulationData::statesValues() const
{
    // Return our states DataStoreValues

    return mStatesValues;
}

//==============================================================================

DataStore::DataStoreValues * SimulationData::algebraicValues() const
{
    // Return our algebraic DataStoreValues

    return mAlgebraicValues;
}

//==============================================================================

const quint64 * SimulationData::delay() const
{
    // Return our delay

    return &mDelay;
}

//==============================================================================

void SimulationData::setDelay(quint64 pDelay)
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

    for (auto solverInterface : Core::solverInterfaces()) {
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

void SimulationData::reset(bool pInitialize, bool pAll)
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

    // Keep track of our constants (in case we don't want to reset them)

    double *currentConstants = new double[mConstantsArray->size()] {};

    if (!pAll)
        memcpy(currentConstants, mConstantsArray->data(), size_t(mConstantsArray->size()*Solver::SizeOfDouble));

    // Reset our parameter values

    if (pInitialize) {
        mConstantsArray->clear();
        mRatesArray->clear();
        mStatesArray->clear();
        mAlgebraicArray->clear();

        runtime->initializeConstants()(mConstantsArray->data(), mRatesArray->data(), mStatesArray->data());
    }

    // Recompute our computed constants and variables

    recomputeComputedConstantsAndVariables(mStartingPoint, pInitialize);

    // Keep track of our various initial values

    if (pInitialize) {
        memcpy(mInitialConstants, mConstantsArray->data(), size_t(mConstantsArray->size()*Solver::SizeOfDouble));
        memcpy(mInitialStates, mStatesArray->data(), size_t(mStatesArray->size()*Solver::SizeOfDouble));
    }

    // Use our "current" constants, if needed

    if (!pAll)
        memcpy(mConstantsArray->data(), currentConstants, size_t(mConstantsArray->size()*Solver::SizeOfDouble));

    delete[] currentConstants;

    // Recompute our computed constants and variables, if we are using our
    // "current" constants

    if (!pAll)
        recomputeComputedConstantsAndVariables(mStartingPoint, pInitialize);

    // Delete our NLA solver, if any

    if (nlaSolver) {
        delete nlaSolver;

        Solver::unsetNlaSolver(runtime->address());
    }

    // Let people know whether our data is 'cleaned', i.e. not modified, and ask
    // our simulation worker to reset itself

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

    runtime->computeComputedConstants()(pCurrentPoint, mConstantsArray->data(), mRatesArray->data(),
                                        pInitialize?mStatesArray->data():mDummyStates, mAlgebraicArray->data());
    runtime->computeRates()(pCurrentPoint, mConstantsArray->data(), mRatesArray->data(),
                            mStatesArray->data(), mAlgebraicArray->data());
    runtime->computeVariables()(pCurrentPoint, mConstantsArray->data(), mRatesArray->data(),
                                mStatesArray->data(), mAlgebraicArray->data());

    // Let people know that our data has been updated

    emit updatedParameters(pCurrentPoint);
}

//==============================================================================

void SimulationData::recomputeVariables(double pCurrentPoint)
{
    // Recompute our 'variables'

    mSimulation->runtime()->computeVariables()(pCurrentPoint, mConstantsArray->data(), mRatesArray->data(),
                                               mStatesArray->data(), mAlgebraicArray->data());
}

//==============================================================================

bool SimulationData::doIsModified(bool pCheckConstants) const
{
    // Check whether any of our constants (if requested) or states has been
    // modified, if possible
    // Note: we start with our states since they are more likely to be modified
    //       than our constants...

    for (quint64 i = 0, iMax = mStatesArray->size(); i < iMax; ++i) {
        if (!qFuzzyCompare(mStatesArray->data(i), mInitialStates[i]))
            return true;
    }

    if (pCheckConstants) {
        for (quint64 i = 0, iMax = mConstantsArray->size(); i < iMax; ++i) {
            if (!qFuzzyCompare(mConstantsArray->data(i), mInitialConstants[i]))
                return true;
        }
    }

    return false;
}

//==============================================================================

bool SimulationData::isStatesModified() const
{
    // Check whether any of our states has been modified

    return doIsModified(false);
}

//==============================================================================

bool SimulationData::isModified() const
{
    // Check whether any of our constants or states has been modified

    return doIsModified(true);
}

//==============================================================================

void SimulationData::checkForModifications()
{
    // Let people know whether any of our constants or states has been modified

    emit modified(isModified());
}

//==============================================================================

void SimulationData::updateParameters(SimulationData *pSimulationData)
{
    // Recompute our 'computed constants' and 'variables'

    pSimulationData->reset(false);

    // Let the simultaion view know parameters have changed

    emit pSimulationData->updatedParameters(pSimulationData->mStartingPoint);
}

//==============================================================================

void SimulationData::createArrays()
{
    // Create our various arrays, if possible

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    if (runtime) {
        // Create our various arrays to compute our model

        mConstantsArray = new DataStore::DataStoreArray(quint64(runtime->constantsCount()));
        mRatesArray = new DataStore::DataStoreArray(quint64(runtime->ratesCount()));
        mStatesArray = new DataStore::DataStoreArray(quint64(runtime->statesCount()));
        mAlgebraicArray = new DataStore::DataStoreArray(quint64(runtime->algebraicCount()));

        // Create DataStoreValues to hold properties of our model's arrays

        mConstantsValues = new DataStore::DataStoreValues(mConstantsArray);
        mRatesValues = new DataStore::DataStoreValues(mRatesArray);
        mStatesValues = new DataStore::DataStoreValues(mStatesArray);
        mAlgebraicValues = new DataStore::DataStoreValues(mAlgebraicArray);

        // Create our various arrays to keep track of our various initial values

        mDummyStates = new double[mStatesArray->size()]{};
        mInitialConstants = new double[mConstantsArray->size()];
        mInitialStates = new double[mStatesArray->size()];
    } else {
        mConstantsArray = mRatesArray = mStatesArray = mAlgebraicArray = nullptr;
        mConstantsValues = mRatesValues = mStatesValues = mAlgebraicValues = nullptr;
        mDummyStates = mInitialConstants = mInitialStates = nullptr;
    }
}

//==============================================================================

void SimulationData::deleteArrays()
{
    // Delete our various arrays

    if (mConstantsArray)
        mConstantsArray->decRef();

    if (mRatesArray)
        mRatesArray->decRef();

    if (mStatesArray)
        mStatesArray->decRef();

    if (mAlgebraicArray)
        mAlgebraicArray->decRef();

    delete mConstantsValues;
    delete mRatesValues;
    delete mStatesValues;
    delete mAlgebraicValues;

    delete[] mDummyStates;
    delete[] mInitialConstants;
    delete[] mInitialStates;

    // Reset our various arrays
    // Note: this shouldn't be needed, but better be safe than sorry...

    mConstantsArray = mRatesArray = mStatesArray = mAlgebraicArray = nullptr;
    mConstantsValues = mRatesValues = mStatesValues = mAlgebraicValues = nullptr;
    mDummyStates = mInitialConstants = mInitialStates = nullptr;
}

//==============================================================================

void SimulationData::setGradientCalculationByIndex(const int &pIndex, bool pCalculate)
{
    // Keep track of the indices of constant parameters that will
    // have gradients calculated

    int pos = mGradientIndices.indexOf(pIndex);

    if (pCalculate) {
        if (pos == -1) mGradientIndices.append(pIndex);
    } else if (pos > -1) {
        mGradientIndices.remove(pos);
    }
}

//==============================================================================

void SimulationData::setGradientCalculation(const QString &pConstantUri, bool pCalculate)
{
    // Keep track of the indices of constant parameters that will
    // have gradients calculated

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    for (int i = 0, iMax = runtime->parameters().count(); i < iMax; ++i) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = runtime->parameters()[i];

        if (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Constant
              && pConstantUri == mSimulationResults->constantVariables()[parameter->index()]->uri()) {
            setGradientCalculationByIndex(parameter->index(), pCalculate);

            // Update icon in parameter information widget

            emit gradientCalculation(parameter, pCalculate);
        }
    }
}
//==============================================================================

bool SimulationData::createGradientsArray()
{
    // Create our gradients array. We can only do so when
    // starting a simulation as we need to know what constant
    // parameters are having their gradients calculated

    if (!mGradientIndices.isEmpty()) {
        // Allocate the array to hold sensitivity gradients at a single point

        mGradientsArray = new DataStore::DataStoreArray(quint64(mGradientIndices.size())*mStatesArray->size());

        return true;
    } else {
        mGradientsArray = nullptr;

        return false;
    }
}

//==============================================================================

void SimulationData::deleteGradientsArray()
{
    // Remove reference to gradients array
    // Note: this will delete the array if it now has no references

    if (mGradientsArray)
        mGradientsArray->decRef();
}

//==============================================================================

double * SimulationData::gradients() const
{
    // Return our gradients array if it has been allocated

    return mGradientsArray?mGradientsArray->data():nullptr;
}

//==============================================================================

int SimulationData::gradientsSize() const
{
    // Return the size of the gradients array

    return mGradientsArray?int(mGradientsArray->size()):0;
}

//==============================================================================

int * SimulationData::gradientIndices()
{
    // Return an integer array of our gradient indices

    return mGradientIndices.data();
}

//==============================================================================

int SimulationData::gradientsCount() const
{
    // Return the number of constant parameters having their gradients
    // calculated

    return mGradientIndices.size();
}

//==============================================================================

SimulationResults::SimulationResults(Simulation *pSimulation) :
    SimulationObject(pSimulation),
    mDataStore(nullptr),
    mPoints(nullptr),
    mConstants(DataStore::DataStoreVariables()),
    mRates(DataStore::DataStoreVariables()),
    mStates(DataStore::DataStoreVariables()),
    mAlgebraic(DataStore::DataStoreVariables()),
    mGradientsStore(nullptr),
    mGradients(DataStore::DataStoreVariables())
{
    // Create our data store

    createDataStore();
}

//==============================================================================

SimulationResults::~SimulationResults()
{
    // Delete some internal objects

    deleteDataStore();

    if (mGradientsStore)
        delete mGradientsStore;
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

    DataStore::DataStoreValues *constantsValues = data->constantsValues();
    DataStore::DataStoreValues *ratesValues = data->ratesValues();
    DataStore::DataStoreValues *statesValues = data->statesValues();
    DataStore::DataStoreValues *algebraicValues = data->algebraicValues();

    // Customise our VOI, as well as our constant, rate, state and algebraic
    // variables

    for (auto parameter : runtime->parameters()) {
        DataStore::DataStoreValue *value = nullptr;
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
            value = constantsValues->at(parameter->index());

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Rate:
            variable = mRates[parameter->index()];
            value = ratesValues->at(parameter->index());

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::State:
            variable = mStates[parameter->index()];
            value = statesValues->at(parameter->index());

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
            variable = mAlgebraic[parameter->index()];
            value = algebraicValues->at(parameter->index());

            break;
        default:
            // Not a relevant type, so do nothing

            ;
        }

        if (value) {
            value->setUri(uri(parameter->componentHierarchy(), parameter->formattedName()));
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

bool SimulationResults::initialiseGradientsStore()
{
    // Allocate additional memory for sensitivity analysis

    // This can only be done when starting a simulation as only then do we
    // know what constant parameters are having their gradients calculated

    // Delete the previous run's gradient store if it exists

    if (mGradientsStore)
        delete mGradientsStore;

    SimulationData *data = mSimulation->data();

    // Delete any gradients array

    data->deleteGradientsArray();

    // Create a new gradients array and data store

    if (data->createGradientsArray()) {
        mGradientsStore = new DataStore::DataStore(mDataStore->uri() + "/gradients");
        mGradients = mGradientsStore->addVariables(data->gradients(), data->gradientsSize());

        // Customise our gradient variables

        int *gradientIndices = data->gradientIndices();
        int statesCount = mStates.size();
        for (int i = 0; i < data->gradientsCount(); ++i) {
            DataStore::DataStoreVariable *constant = mConstants[gradientIndices[i]];

            for (int j = 0; j < statesCount; ++j) {
                DataStore::DataStoreVariable *state = mStates[j];
                DataStore::DataStoreVariable *gradient = mGradients[i*statesCount + j];

                // Gradient is of state variable wrt each constant

                gradient->setUri(state->uri() + "/gradient_with/" + constant->uri());
                gradient->setLabel("d(" + state->label() + ")/d(" + constant->label() + ")");
            }
        }

        // Allocate data for gradients

        quint64 simulationSize = mSimulation->size();

        if (simulationSize)
            return mGradientsStore->addRun(simulationSize);
    } else {
        mGradientsStore = nullptr;
        mGradients = DataStore::DataStoreVariables();
        return true;
    }
    return false;
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
    // Make sure that all our variables are up to date

    mSimulation->data()->recomputeVariables(pPoint);

    // Now that we are all set, we can add the data to our data store

    mDataStore->addValues(pPoint);

    // Add data to gradients store

    if (mGradientsStore)
       mGradientsStore->addValues(pPoint);
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

DataStore::DataStoreVariables SimulationResults::constantVariables() const
{
    // Return our constants data store variables

    return mConstants;
}

//==============================================================================

DataStore::DataStoreVariables SimulationResults::rateVariables() const
{
    // Return our rates data store variables

    return mRates;
}

//==============================================================================

DataStore::DataStoreVariables SimulationResults::stateVariables() const
{
    // Return our states data store variables

    return mStates;
}

//==============================================================================

DataStore::DataStoreVariables SimulationResults::algebraicVariables() const
{
    // Return our algebraic data store variables

    return mAlgebraic;
}

//==============================================================================

Simulation::Simulation(const QString &pFileName) :
    mFileName(pFileName),
    mRuntime(nullptr),
    mWorker(nullptr)
{
    // Retrieve our file details

    retrieveFileDetails();

    // Create our data and results objects, now that we are all set

    mData = new SimulationData(this);
    mResults = new SimulationResults(this);

    // Make sure we can access metadata about result variables

    mData->setSimulationResults(mResults);


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

QString Simulation::furtherInitialize() const
{
    // Initialize ourself from a SED-ML document

    libsedml::SedDocument *sedmlDocument = sedmlFile()->sedmlDocument();
    libsedml::SedUniformTimeCourse *sedmlUniformTimeCourse = static_cast<libsedml::SedUniformTimeCourse *>(sedmlDocument->getSimulation(0));
    libsedml::SedOneStep *sedmlOneStep = static_cast<libsedml::SedOneStep *>(sedmlDocument->getSimulation(1));

    double startingPoint = sedmlUniformTimeCourse->getOutputStartTime();
    double endingPoint = sedmlUniformTimeCourse->getOutputEndTime();
    double pointInterval = (endingPoint-startingPoint)/sedmlUniformTimeCourse->getNumberOfPoints();

    if (sedmlOneStep)
        endingPoint += sedmlOneStep->getStep();

    mData->setStartingPoint(startingPoint);
    mData->setEndingPoint(endingPoint);
    mData->setPointInterval(pointInterval);

    const libsedml::SedAlgorithm *sedmlAlgorithm = sedmlUniformTimeCourse->getAlgorithm();

    SolverInterface *odeSolverInterface = nullptr;
    SolverInterfaces solverInterfaces = Core::solverInterfaces();

    QString kisaoId = QString::fromStdString(sedmlAlgorithm->getKisaoID());

    foreach (SolverInterface *solverInterface, solverInterfaces) {
        if (!solverInterface->id(kisaoId).compare(solverInterface->solverName())) {
            odeSolverInterface = solverInterface;

            mData->setOdeSolverName(solverInterface->solverName());

            break;
        }
    }

    if (!odeSolverInterface) {
        return tr("the requested solver (%1) could not be found").arg(kisaoId);
    }

    for (unsigned int i = 0, iMax = sedmlAlgorithm->getNumAlgorithmParameters(); i < iMax; ++i) {
        const libsedml::SedAlgorithmParameter *sedmlAlgorithmParameter = sedmlAlgorithm->getAlgorithmParameter(i);
        QString kisaoId = QString::fromStdString(sedmlAlgorithmParameter->getKisaoID());
        QString id = odeSolverInterface->id(kisaoId);

        QVariant solverPropertyValue = QString::fromStdString(sedmlAlgorithmParameter->getValue());

        mData->addOdeSolverProperty(id, solverPropertyValue);
    }

    libsbml::XMLNode *annotation = sedmlAlgorithm->getAnnotation();

    if (annotation) {
        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const libsbml::XMLNode &solverPropertiesNode = annotation->getChild(i);

            if (   !QString::fromStdString(solverPropertiesNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                && !QString::fromStdString(solverPropertiesNode.getName()).compare(SEDMLSupport::SolverProperties)) {
                for (uint j = 0, jMax = solverPropertiesNode.getNumChildren(); j < jMax; ++j) {
                    const libsbml::XMLNode &solverPropertyNode = solverPropertiesNode.getChild(j);

                    if (   !QString::fromStdString(solverPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                        && !QString::fromStdString(solverPropertyNode.getName()).compare(SEDMLSupport::SolverProperty)) {
                        QString id = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Id.toStdString())));
                        QString value = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Value.toStdString())));

                        mData->addOdeSolverProperty(id, value);
                    }
                }
            }
        }
    }

    annotation = sedmlUniformTimeCourse->getAnnotation();

    if (annotation) {
        bool mustHaveNlaSolver = false;
        bool hasNlaSolver = false;
        QString nlaSolverName = QString();

        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const libsbml::XMLNode &nlaSolverNode = annotation->getChild(i);

            if (   !QString::fromStdString(nlaSolverNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                && !QString::fromStdString(nlaSolverNode.getName()).compare(SEDMLSupport::NlaSolver)) {

                mustHaveNlaSolver = true;
                nlaSolverName = QString::fromStdString(nlaSolverNode.getAttrValue(nlaSolverNode.getAttrIndex(SEDMLSupport::Name.toStdString())));

                foreach (SolverInterface *solverInterface, solverInterfaces) {
                    if (!nlaSolverName.compare(solverInterface->solverName())) {

                        mData->setNlaSolverName(nlaSolverName);

                        hasNlaSolver = true;

                        break;
                    }
                }

                if (hasNlaSolver) {
                    for (uint j = 0, jMax = nlaSolverNode.getNumChildren(); j < jMax; ++j) {
                        const libsbml::XMLNode &solverPropertyNode = nlaSolverNode.getChild(j);

                        if (   !QString::fromStdString(solverPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                            && !QString::fromStdString(solverPropertyNode.getName()).compare(SEDMLSupport::SolverProperty)) {
                            QString id = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Id.toStdString())));
                            QString value = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Value.toStdString())));

                            mData->addNlaSolverProperty(id, value);
                        }
                    }

                    break;
                }
            }
        }

        if (mustHaveNlaSolver && !hasNlaSolver) {
            return tr("the requested NLA solver (%1) could not be set").arg(nlaSolverName);
        }
    }

    return QString();
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

const quint64 * Simulation::delay() const
{
    // Return our delay

    return mData->delay();
}

//==============================================================================

void Simulation::setDelay(quint64 pDelay)
{
    // Set our delay

    mData->setDelay(pDelay);
}

//==============================================================================

bool Simulation::simulationSettingsOk(bool pEmitSignal)
{
    // Check and return whether our simulation settings are sound

    if (qFuzzyCompare(mData->startingPoint(), mData->endingPoint())) {
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

void Simulation::run()
{
    // Make sure that we have a runtime

    if (!mRuntime)
        return;

    // Initialise sensitivity gradients data store

    if (!mResults->initialiseGradientsStore())
        return;

    // Initialise our worker, if we don't already have one and if the
    // simulation settings we were given are sound

    if (!mWorker && simulationSettingsOk()) {
        // Create and move our worker to a thread

        QThread *thread = new QThread();
        mWorker = new SimulationWorker(this, thread, mWorker);

        mWorker->moveToThread(thread);

        connect(thread, &QThread::started,
                mWorker, &SimulationWorker::run);

        connect(mWorker, &SimulationWorker::running,
                this, &Simulation::running);
        connect(mWorker, &SimulationWorker::paused,
                this, &Simulation::paused);

        connect(mWorker, &SimulationWorker::done,
                this, &Simulation::done);
        connect(mWorker, &SimulationWorker::done,
                thread, &QThread::quit);
        connect(mWorker, &SimulationWorker::done,
                mWorker, &SimulationWorker::deleteLater);

        connect(mWorker, &SimulationWorker::error,
                this, &Simulation::error);

        connect(thread, &QThread::finished,
                thread, &QThread::deleteLater);

        // Start our worker by starting the thread in which it is

        thread->start();
    }
}

//==============================================================================

void Simulation::pause()
{
    // Pause our worker

    if (mWorker)
        mWorker->pause();
}

//==============================================================================

void Simulation::resume()
{
    // Resume our worker

    if (mWorker)
        mWorker->resume();
}

//==============================================================================

void Simulation::stop()
{
    // Stop our worker

    if (mWorker)
        mWorker->stop();
}

//==============================================================================

void Simulation::reset(bool pAll)
{
    // Reset our data

    mData->reset(true, pAll);

    // Reset our worker

    if (mWorker)
        mWorker->reset();
}

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

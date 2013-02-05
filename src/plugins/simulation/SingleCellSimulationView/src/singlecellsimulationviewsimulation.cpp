//==============================================================================
// Single cell simulation view simulation
//==============================================================================

#include "cellmlfileruntime.h"
#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewwidget.h"
#include "thread.h"

//==============================================================================

#include <qmath.h>

//==============================================================================

#include "qwt_slider.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationData::SingleCellSimulationViewSimulationData(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime) :
    mCellmlFileRuntime(pCellmlFileRuntime),
    mDelay(0),
    mStartingPoint(0.0),
    mEndingPoint(1000.0),
    mPointInterval(1.0),
    mOdeSolverName(QString()),
    mOdeSolverProperties(CoreSolver::Properties()),
    mDaeSolverName(QString()),
    mDaeSolverProperties(CoreSolver::Properties()),
    mNlaSolverName(QString()),
    mNlaSolverProperties(CoreSolver::Properties()),
    mResults(0),
    mResultsCount(0),
    mResultsFilled(0)
{
    // Create the various arrays needed to compute a model

    int constantsCount = pCellmlFileRuntime->constantsCount();
    int ratesCount = pCellmlFileRuntime->ratesCount();
    int statesCount = pCellmlFileRuntime->statesCount();
    int algebraicCount = pCellmlFileRuntime->algebraicCount();
    int condVarCount = pCellmlFileRuntime->condVarCount();

    mConstants = new double[constantsCount];
    mStates    = new double[statesCount];
    mRates     = new double[ratesCount];
    mAlgebraic = new double[algebraicCount];
    mCondVar   = new double[condVarCount];

    // Make sure that all our arrays are initialised with zeros

    static const int sizeOfDouble = sizeof(double);

    memset(mConstants, 0, constantsCount*sizeOfDouble);
    memset(mStates, 0, statesCount*sizeOfDouble);
    memset(mRates, 0, ratesCount*sizeOfDouble);
    memset(mAlgebraic, 0, algebraicCount*sizeOfDouble);
    memset(mCondVar, 0, condVarCount*sizeOfDouble);
}

//==============================================================================

SingleCellSimulationViewSimulationData::~SingleCellSimulationViewSimulationData()
{
    // Delete some internal objects

    delete[] mConstants;
    delete[] mStates;
    delete[] mRates;
    delete[] mAlgebraic;
    delete[] mCondVar;
}

//==============================================================================

double * SingleCellSimulationViewSimulationData::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double * SingleCellSimulationViewSimulationData::states() const
{
    // Return our states array

    return mStates;
}

//==============================================================================

double * SingleCellSimulationViewSimulationData::rates() const
{
    // Return our rates array

    return mRates;
}

//==============================================================================

double * SingleCellSimulationViewSimulationData::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

double * SingleCellSimulationViewSimulationData::condVar() const
{
    // Return our condVar array

    return mCondVar;
}

//==============================================================================

int SingleCellSimulationViewSimulationData::delay() const
{
    // Return our delay

    return mDelay;
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setDelay(const int &pDelay)
{
    // Set our delay

    mDelay = pDelay;
}

//==============================================================================

double SingleCellSimulationViewSimulationData::startingPoint() const
{
    // Return our starting point

    return mStartingPoint;
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setStartingPoint(const double &pStartingPoint)
{
    // Set our starting point

    mStartingPoint = pStartingPoint;

    // Recompute our 'computed constants' and 'variables'

    recomputeComputedConstantsAndVariables();
}

//==============================================================================

double SingleCellSimulationViewSimulationData::endingPoint() const
{
    // Return our ending point

    return mEndingPoint;
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setEndingPoint(const double &pEndingPoint)
{
    // Set our ending point

    mEndingPoint = pEndingPoint;
}

//==============================================================================

double SingleCellSimulationViewSimulationData::pointInterval() const
{
    // Return our point interval

    return mPointInterval;
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setPointInterval(const double &pPointInterval)
{
    // Set our point interval

    mPointInterval = pPointInterval;
}

//==============================================================================

QString SingleCellSimulationViewSimulationData::odeSolverName() const
{
    // Return our ODE solver name

    return mCellmlFileRuntime->needOdeSolver()?mOdeSolverName:QString();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setOdeSolverName(const QString &pOdeSolverName)
{
    // Set our ODE solver name

    if (mCellmlFileRuntime->needOdeSolver())
        mOdeSolverName = pOdeSolverName;
}

//==============================================================================

CoreSolver::Properties SingleCellSimulationViewSimulationData::odeSolverProperties() const
{
    // Return our ODE solver's properties

    return mCellmlFileRuntime->needOdeSolver()?mOdeSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::addOdeSolverProperty(const QString &pName,
                                                                  const QVariant &pValue)
{
    // Add an ODE solver property

    if (mCellmlFileRuntime->needOdeSolver())
        mOdeSolverProperties.insert(pName, pValue);
}

//==============================================================================

QString SingleCellSimulationViewSimulationData::daeSolverName() const
{
    // Return our DAE solver name

    return mCellmlFileRuntime->needDaeSolver()?mDaeSolverName:QString();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setDaeSolverName(const QString &pDaeSolverName)
{
    // Set our DAE solver name

    if (mCellmlFileRuntime->needDaeSolver())
        mDaeSolverName = pDaeSolverName;
}

//==============================================================================

CoreSolver::Properties SingleCellSimulationViewSimulationData::daeSolverProperties() const
{
    // Return our DAE solver's properties

    return mCellmlFileRuntime->needDaeSolver()?mDaeSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::addDaeSolverProperty(const QString &pName,
                                                                  const QVariant &pValue)
{
    // Add an DAE solver property

    if (mCellmlFileRuntime->needDaeSolver())
        mDaeSolverProperties.insert(pName, pValue);
}

//==============================================================================

QString SingleCellSimulationViewSimulationData::nlaSolverName() const
{
    // Return our NLA solver name

    return mCellmlFileRuntime->needNlaSolver()?mNlaSolverName:QString();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setNlaSolverName(const QString &pNlaSolverName)
{
    // Set our NLA solver name

    if (mCellmlFileRuntime->needNlaSolver())
        mNlaSolverName = pNlaSolverName;
}

//==============================================================================

CoreSolver::Properties SingleCellSimulationViewSimulationData::nlaSolverProperties() const
{
    // Return our NLA solver's properties

    return mCellmlFileRuntime->needNlaSolver()?mNlaSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::addNlaSolverProperty(const QString &pName,
                                                                  const QVariant &pValue)
{
    // Add an NLA solver property

    if (mCellmlFileRuntime->needNlaSolver())
        mNlaSolverProperties.insert(pName, pValue);
}

//==============================================================================

void SingleCellSimulationViewSimulationData::reset()
{
    // Reset our model parameter values which means both initialising our
    // 'constants' and computing our 'computed constants' and 'variables'
    // Note: recomputeComputedConstantsAndVariables() will let people know that
    //       our data changed...

    mCellmlFileRuntime->initializeConstants()(mConstants, mRates, mStates);
    recomputeComputedConstantsAndVariables();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::recomputeComputedConstantsAndVariables()
{
    // Recompute our 'computed constants' and 'variables'

    mCellmlFileRuntime->computeComputedConstants()(mConstants, mRates, mStates);
    mCellmlFileRuntime->computeVariables()(mStartingPoint, mConstants, mRates, mStates, mAlgebraic);

    // Let people know that our data has changed

    emit dataChanged();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::recomputeVariables(const double &pCurrentPoint)
{
    // Recompute our 'variables'

    mCellmlFileRuntime->computeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);

    // Let people know that our data has changed

    emit dataChanged();
}

//==============================================================================

SingleCellSimulationViewSimulationData::Results * SingleCellSimulationViewSimulationData::results() const
{
    // Return our results

    return mResults;
}

//==============================================================================

int SingleCellSimulationViewSimulationData::resultsCount() const
{
    // Return our results count

    return mResultsCount;
}

//==============================================================================

int SingleCellSimulationViewSimulationData::resultsFilled() const
{
    // Return our results filled

    return mResultsFilled;
}

//==============================================================================

void SingleCellSimulationViewSimulationData::resetResults()
{
    // Remove previous results

    for (int i = 0; i < mResultsCount; ++i) {
        Results *results = &mResults[i];

        delete[] results->constants;
        delete[] results->states;
        delete[] results->rates;
        delete[] results->algebraic;
    }

    delete[] mResults;

    // Create our arrays of results

    mResultsCount  = qCeil((mEndingPoint-mStartingPoint)/mPointInterval)+1;
    mResultsFilled = 0;

    mResults = new Results[mResultsCount];

    for (int i = 0; i < mResultsCount; ++i) {
        Results *results = &mResults[i];

        results->constants = new double[mCellmlFileRuntime->constantsCount()];
        results->states    = new double[mCellmlFileRuntime->statesCount()];
        results->rates     = new double[mCellmlFileRuntime->ratesCount()];
        results->algebraic = new double[mCellmlFileRuntime->algebraicCount()];
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationData::addResults(const double &pPoint)
{
    // Add the given results to our arrays of results

    static const int sizeOfDouble = sizeof(double);

    Results *results = &mResults[mResultsFilled];

    results->point = pPoint;

    memcpy(results->constants, mConstants, mCellmlFileRuntime->constantsCount()*sizeOfDouble);
    memcpy(results->states, mStates, mCellmlFileRuntime->statesCount()*sizeOfDouble);
    memcpy(results->rates, mRates, mCellmlFileRuntime->ratesCount()*sizeOfDouble);
    memcpy(results->algebraic, mAlgebraic, mCellmlFileRuntime->algebraicCount()*sizeOfDouble);

    ++mResultsFilled;
}

//==============================================================================

SingleCellSimulationViewSimulation::SingleCellSimulationViewSimulation(const QString &pFileName,
                                                                       CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime) :
    mWorker(0),
    mWorkerThread(0),
    mFileName(pFileName),
    mCellmlFileRuntime(pCellmlFileRuntime),
    mData(new SingleCellSimulationViewSimulationData(pCellmlFileRuntime))
{
}

//==============================================================================

SingleCellSimulationViewSimulation::~SingleCellSimulationViewSimulation()
{
    // Stop our worker (just in case...)

    stop();

    // Delete some internal objects

    delete mData;
}

//==============================================================================

QString SingleCellSimulationViewSimulation::fileName() const
{
    // Retrieve and return our file name

    return mFileName;
}

//==============================================================================

SingleCellSimulationViewSimulationData * SingleCellSimulationViewSimulation::data() const
{
    // Retrieve and return our data

    return mData;
}

//==============================================================================

SingleCellSimulationViewSimulationWorker::Status SingleCellSimulationViewSimulation::workerStatus() const
{
    // Return the status of our worker, if active

    return mWorker?mWorker->status():SingleCellSimulationViewSimulationWorker::Unknown;
}

//==============================================================================

double SingleCellSimulationViewSimulation::workerProgress() const
{
    // Return the progress of our worker, if active

    return mWorker?mWorker->progress():0.0;
}

//==============================================================================

void SingleCellSimulationViewSimulation::setDelay(const int &pDelay)
{
    // Set our delay

    mData->setDelay(pDelay);
}

//==============================================================================

void SingleCellSimulationViewSimulation::reset()
{
    // Reset our data

    mData->reset();
}

//==============================================================================

void SingleCellSimulationViewSimulation::recomputeComputedConstantsAndVariables()
{
    // Recompute our 'computed constants' and 'variables'

    mData->recomputeComputedConstantsAndVariables();
}

//==============================================================================

void SingleCellSimulationViewSimulation::run(const SolverInterfaces &pSolverInterfaces)
{
    // Initialise our worker, if not active

    if (!mWorker) {
        // First, check that our simulation settings we were given are sound

        bool simulationSettingsOk = false;

        if (mData->startingPoint() == mData->endingPoint())
            emit error(tr("the starting and ending points cannot have the same value"));
        else if (mData->pointInterval() == 0)
            emit error(tr("the point interval cannot be equal to zero"));
        else if (   (mData->startingPoint() < mData->endingPoint())
                 && (mData->pointInterval() < 0))
            emit error(tr("the ending point is greater than the starting point, so the point interval should be greater than zero"));
        else if (   (mData->startingPoint() > mData->endingPoint())
                 && (mData->pointInterval() > 0))
            emit error(tr("the ending point is smaller than the starting point, so the point interval should be smaller than zero"));
        else
            simulationSettingsOk = true;

        if (!simulationSettingsOk)
            // Something wrong with our simulation settings, so...

            return;

        // Create our worker

        mWorker = new SingleCellSimulationViewSimulationWorker(pSolverInterfaces, mCellmlFileRuntime, mData);

        if (!mWorker) {
            emit error(tr("the simulation worker could not be created"));

            return;
        }

        // Create the thread in which our worker will work

        mWorkerThread = new Core::Thread();

        if (!mWorkerThread) {
            delete mWorker;

            mWorker = 0;

            emit error(tr("the thread for the simulation worker could not be created"));

            return;
        }

        // Move our worker to its thread

        mWorker->moveToThread(mWorkerThread);

        // Create a few connections

        connect(mWorkerThread, SIGNAL(started()),
                mWorker, SLOT(run()));

        connect(mWorker, SIGNAL(running()),
                this, SIGNAL(running()));
        connect(mWorker, SIGNAL(pausing()),
                this, SIGNAL(pausing()));

        connect(mWorker, SIGNAL(progress(const double &)),
                this, SIGNAL(progress(const double &)));

        connect(mWorker, SIGNAL(finished(const int &)),
                this, SLOT(finished(const int &)));

        connect(mWorker, SIGNAL(error(const QString &)),
                this, SIGNAL(error(const QString &)));

        connect(mWorker, SIGNAL(finished(const int &)),
                mWorkerThread, SLOT(quit()));
        connect(mWorker, SIGNAL(finished(const int &)),
                mWorker, SLOT(deleteLater()));

        connect(mWorkerThread, SIGNAL(finished()),
                mWorkerThread, SLOT(deleteLater()));

        // Start our worker thread

        mWorkerThread->start();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulation::pause()
{
    // Ask our worker to pause, if active

    if (mWorker)
        mWorker->pause();
}

//==============================================================================

void SingleCellSimulationViewSimulation::resume()
{
    // Ask our worker to resume, if active

    if (mWorker)
        mWorker->resume();
}

//==============================================================================

void SingleCellSimulationViewSimulation::stop()
{
    // Ask our worker to stop, if active

    if (mWorker)
        mWorker->stop();
}

//==============================================================================

void SingleCellSimulationViewSimulation::finished(const int &pElapsedTime)
{
    // Our worker is done (and it will get deleted and everything), so...

    mWorker       = 0;
    mWorkerThread = 0;

    // Let people know that we have stopped

    emit stopped(pElapsedTime);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

#include <QFile>
#include <QTextStream>

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
    mNlaSolverProperties(CoreSolver::Properties())
{
    // Create the various arrays needed to compute our model

    mConstants = new double[pCellmlFileRuntime->constantsCount()];
    mStates    = new double[pCellmlFileRuntime->statesCount()];
    mRates     = new double[pCellmlFileRuntime->ratesCount()];
    mAlgebraic = new double[pCellmlFileRuntime->algebraicCount()];
    mCondVar   = new double[pCellmlFileRuntime->condVarCount()];

    // Make sure that all our arrays are initialised with zeros

    static const int SizeOfDouble = sizeof(double);

    memset(mConstants, 0, pCellmlFileRuntime->constantsCount()*SizeOfDouble);
    memset(mStates, 0, pCellmlFileRuntime->statesCount()*SizeOfDouble);
    memset(mRates, 0, pCellmlFileRuntime->ratesCount()*SizeOfDouble);
    memset(mAlgebraic, 0, pCellmlFileRuntime->algebraicCount()*SizeOfDouble);
    memset(mCondVar, 0, pCellmlFileRuntime->condVarCount()*SizeOfDouble);
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

double SingleCellSimulationViewSimulationData::size() const
{
    // Return the size of our simulation (i.e. the number of data points which
    // should be generated)
    // Note: we return a double rather than a size_t in case the simulation
    //       requires an insane amount of memory...

    return ceil((mEndingPoint-mStartingPoint)/mPointInterval)+1;
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

SingleCellSimulationViewSimulationResults::SingleCellSimulationViewSimulationResults(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                                                     SingleCellSimulationViewSimulationData *pData) :
    mCellmlFileRuntime(pCellmlFileRuntime),
    mData(pData),
    mSize(0),
    mPoints(0),
    mConstants(0),
    mStates(0),
    mRates(0),
    mAlgebraic(0)
{
}

//==============================================================================

SingleCellSimulationViewSimulationResults::~SingleCellSimulationViewSimulationResults()
{
    // Delete some internal objects

    deleteArrays();
}

//==============================================================================

void SingleCellSimulationViewSimulationResults::createArrays()
{
    // Determine the size of our arrays

    int dataSize = mData->size();

    // Create our points array

    mPoints = new double[dataSize];

    // Create our constants arrays

    mConstants = new double*[mCellmlFileRuntime->constantsCount()];

    for (int i = 0, iMax = mCellmlFileRuntime->constantsCount(); i < iMax; ++i)
        mConstants[i] = new double[dataSize];

    // Create our states arrays

    mStates = new double*[mCellmlFileRuntime->statesCount()];

    for (int i = 0, iMax = mCellmlFileRuntime->statesCount(); i < iMax; ++i)
        mStates[i] = new double[dataSize];

    // Create our rates arrays

    mRates = new double*[mCellmlFileRuntime->ratesCount()];

    for (int i = 0, iMax = mCellmlFileRuntime->ratesCount(); i < iMax; ++i)
        mRates[i] = new double[dataSize];

    // Create our algebraic arrays

    mAlgebraic = new double*[mCellmlFileRuntime->algebraicCount()];

    for (int i = 0, iMax = mCellmlFileRuntime->algebraicCount(); i < iMax; ++i)
        mAlgebraic[i] = new double[dataSize];
}

//==============================================================================

void SingleCellSimulationViewSimulationResults::deleteArrays()
{
    if (!mPoints)
        // There are no arrays to delete, so...

        return;

    // Delete our points array

    delete[] mPoints;

    // Delete our constants arrays

    for (int i = 0, iMax = mCellmlFileRuntime->constantsCount(); i < iMax; ++i)
        delete[] mConstants[i];

    delete mConstants;

    // Delete our states arrays

    for (int i = 0, iMax = mCellmlFileRuntime->statesCount(); i < iMax; ++i)
        delete[] mStates[i];

    delete mStates;

    // Delete our rates arrays

    for (int i = 0, iMax = mCellmlFileRuntime->ratesCount(); i < iMax; ++i)
        delete[] mRates[i];

    delete mRates;

    // Delete our algebraic arrays

    for (int i = 0, iMax = mCellmlFileRuntime->algebraicCount(); i < iMax; ++i)
        delete[] mAlgebraic[i];

    delete mAlgebraic;
}

//==============================================================================

void SingleCellSimulationViewSimulationResults::reset()
{
    // Reset our size

    mSize = 0;

    // Reset our arrays

    deleteArrays();
    createArrays();
}

//==============================================================================

void SingleCellSimulationViewSimulationResults::addPoint(const double &pPoint)
{
    // Add the data to our different arrays

    mPoints[mSize] = pPoint;

    for (int i = 0, iMax = mCellmlFileRuntime->constantsCount(); i < iMax; ++i)
        mConstants[i][mSize] = mData->constants()[i];

    for (int i = 0, iMax = mCellmlFileRuntime->statesCount(); i < iMax; ++i)
        mStates[i][mSize] = mData->states()[i];

    for (int i = 0, iMax = mCellmlFileRuntime->ratesCount(); i < iMax; ++i)
        mRates[i][mSize] = mData->rates()[i];

    for (int i = 0, iMax = mCellmlFileRuntime->algebraicCount(); i < iMax; ++i)
        mAlgebraic[i][mSize] = mData->algebraic()[i];

    // Increase our size

    ++mSize;

    // Let people know that new results have been added

    emit results(this);
}

//==============================================================================

int SingleCellSimulationViewSimulationResults::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

double * SingleCellSimulationViewSimulationResults::points() const
{
    // Return our points

    return mPoints;
}

//==============================================================================

double ** SingleCellSimulationViewSimulationResults::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double ** SingleCellSimulationViewSimulationResults::states() const
{
    // Return our states array

    return mStates;
}

//==============================================================================

double ** SingleCellSimulationViewSimulationResults::rates() const
{
    // Return our rates array

    return mRates;
}

//==============================================================================

double ** SingleCellSimulationViewSimulationResults::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

bool SingleCellSimulationViewSimulationResults::exportToCsv(const QString &pFileName) const
{
    // Export of all of our data to a CSV file

    QFile file(pFileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // The file can't be opened, so...

        file.remove();

        return false;
    }

    // Write out the contents of the CellML file to the file

    QTextStream out(&file);

    // Header

    int constantsCount = mCellmlFileRuntime->constantsCount();
    int statesCount = mCellmlFileRuntime->statesCount();
    int ratesCount = mCellmlFileRuntime->ratesCount();
    int algebraicCount = mCellmlFileRuntime->algebraicCount();

    out << "VOI";

    for (int i = 0; i < constantsCount; ++i)
        out << ",CONSTANTS[" << i << "]";

    for (int i = 0; i < statesCount; ++i)
        out << ",STATES[" << i << "]";

    for (int i = 0; i < ratesCount; ++i)
        out << ",RATES[" << i << "]";

    for (int i = 0; i < algebraicCount; ++i)
        out << ",ALGEBRAIC[" << i << "]";

    out << "\n";

    // Data itself

    for (int j = 0; j < mSize; ++j) {
        out << mPoints[j];

        for (int i = 0; i < constantsCount; ++i)
            out << "," << mConstants[i][j];

        for (int i = 0; i < statesCount; ++i)
            out << "," << mStates[i][j];

        for (int i = 0; i < ratesCount; ++i)
            out << "," << mRates[i][j];

        for (int i = 0; i < algebraicCount; ++i)
            out << "," << mAlgebraic[i][j];

        out << "\n";
    }

    // We are done, so close our file

    file.close();

    // Everything went fine, so...

    return true;
}

//==============================================================================

SingleCellSimulationViewSimulation::SingleCellSimulationViewSimulation(const QString &pFileName,
                                                                       CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime) :
    mWorker(0),
    mWorkerThread(0),
    mFileName(pFileName),
    mCellmlFileRuntime(pCellmlFileRuntime),
    mData(new SingleCellSimulationViewSimulationData(pCellmlFileRuntime)),
    mResults(new SingleCellSimulationViewSimulationResults(pCellmlFileRuntime, mData))
{
}

//==============================================================================

SingleCellSimulationViewSimulation::~SingleCellSimulationViewSimulation()
{
    // Stop our worker (just in case...)

    stop();

    // Delete some internal objects

    delete mData;
    delete mResults;
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

SingleCellSimulationViewSimulationResults * SingleCellSimulationViewSimulation::results() const
{
    // Return our results

    return mResults;
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
    // Reset both our data and results

    mData->reset();
    mResults->reset();
}

//==============================================================================

void SingleCellSimulationViewSimulation::recomputeComputedConstantsAndVariables()
{
    // Recompute our 'computed constants' and 'variables'

    mData->recomputeComputedConstantsAndVariables();
}

//==============================================================================

double SingleCellSimulationViewSimulation::neededMemory() const
{
    // Determine and return the amount of needed memory to run our simulation
    // Note: we return the amount as a double rather than a size_t (as we do
    //       when retrieving the total/free amount of memory available; see
    //       [OpenCOR]/src/plugins/misc/Core/src/coreutils.cpp) in case a
    //       simulation requires an insane amount of memory...

    static const int SizeOfDouble = sizeof(double);

    return  mData->size()
           *( 1
             +mCellmlFileRuntime->constantsCount()
             +mCellmlFileRuntime->statesCount()
             +mCellmlFileRuntime->ratesCount()
             +mCellmlFileRuntime->algebraicCount())
           *SizeOfDouble;
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

        mWorker = new SingleCellSimulationViewSimulationWorker(pSolverInterfaces, mCellmlFileRuntime, mData, mResults);

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

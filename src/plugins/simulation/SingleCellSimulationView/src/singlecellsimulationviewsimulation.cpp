//==============================================================================
// Single cell simulation view simulation
//==============================================================================

#include "cellmlfileruntime.h"
#include "corenlasolver.h"
#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewwidget.h"
#include "solverinterface.h"
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

SingleCellSimulationViewSimulationData::SingleCellSimulationViewSimulationData(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                                               const SolverInterfaces &pSolverInterfaces) :
    mCellmlFileRuntime(pCellmlFileRuntime),
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

void SingleCellSimulationViewSimulationData::setStartingPoint(const double &pStartingPoint,
                                                              const bool &pRecompute)
{
    // Set our starting point

    mStartingPoint = pStartingPoint;

    // Recompute our 'computed constants' and 'variables'

    if (pRecompute)
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
    // Note: we return a double rather than a qulonglong in case the simulation
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

void SingleCellSimulationViewSimulationData::setNlaSolverName(const QString &pNlaSolverName,
                                                              const bool &pReset)
{
    // Set our NLA solver name

    if (mCellmlFileRuntime->needNlaSolver()) {
        mNlaSolverName = pNlaSolverName;

        // Reset our model parameter values
        // Note: to only recompute our 'computed constants' and 'variables' is
        //       not sufficient since some constants may also need to be
        //       reinitialised...

        if (pReset)
            reset();
    }
}

//==============================================================================

CoreSolver::Properties SingleCellSimulationViewSimulationData::nlaSolverProperties() const
{
    // Return our NLA solver's properties

    return mCellmlFileRuntime->needNlaSolver()?mNlaSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::addNlaSolverProperty(const QString &pName,
                                                                  const QVariant &pValue,
                                                                  const bool &pReset)
{
    // Add an NLA solver property

    if (mCellmlFileRuntime->needNlaSolver()) {
        mNlaSolverProperties.insert(pName, pValue);

        // Reset our model parameter values
        // Note: to only recompute our 'computed constants' and 'variables' is
        //       not sufficient since some constants may also need to be
        //       reinitialised...

        if (pReset)
            reset();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationData::reset()
{
    // Reset our model parameter values which means both initialising our
    // 'constants' and computing our 'computed constants' and 'variables'
    // Note #1: we must check whether our runtime needs NLA solver and, if so,
    //          then retrieve an instance of our NLA solver since some of the
    //          resetting may require solving one or several NLA systems...
    // Note #2: recomputeComputedConstantsAndVariables() will let people know
    //          that our data has changed...

    CoreSolver::CoreNlaSolver *nlaSolver = 0;

    if (mCellmlFileRuntime->needNlaSolver()) {
        // Retrieve an instance of our NLA solver

        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare(mNlaSolverName)) {
                // The requested NLA solver was found, so retrieve an instance
                // of it

                nlaSolver = static_cast<CoreSolver::CoreNlaSolver *>(solverInterface->instance());

                // Keep track of our NLA solver, so that doNonLinearSolve() can
                // work as expected

                CoreSolver::setNlaSolver(mCellmlFileRuntime->address(),
                                         nlaSolver);

                break;
            }

        // Keep track of any error that might be reported by our NLA solver

        connect(nlaSolver, SIGNAL(error(const QString &)),
                this, SIGNAL(error(const QString &)));

        // Initialise our NLA solver

        nlaSolver->setProperties(mNlaSolverProperties);
    }

    // Reset our model parameter values

    mCellmlFileRuntime->initializeConstants()(mConstants, mRates, mStates);
    recomputeComputedConstantsAndVariables();

    // Delete our NLA solver, if any

    if (nlaSolver) {
        delete nlaSolver;

        CoreSolver::unsetNlaSolver(mCellmlFileRuntime->address());
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationData::recomputeComputedConstantsAndVariables()
{
    // Recompute our 'computed constants' and 'variables'

    mCellmlFileRuntime->computeComputedConstants()(mConstants, mRates, mStates);
    mCellmlFileRuntime->computeVariables()(mStartingPoint, mConstants, mRates, mStates, mAlgebraic);

    // Let people know that our data has been updated

    emit updated();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::recomputeVariables(const double &pCurrentPoint,
                                                                const bool &pEmitSignal)
{
    // Recompute our 'variables'

    mCellmlFileRuntime->computeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);

    // Let people know that our data has been updated

    if (pEmitSignal)
        emit updated();
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

bool SingleCellSimulationViewSimulationResults::createArrays()
{
    static const int SizeOfDoublePointer = sizeof(double *);

    // Create our points array

    qulonglong dataSize = qulonglong(mData->size());

    try {
        mPoints = new double[dataSize];
    } catch(...) {
        return false;
    }

    // Create our constants arrays

    try {
        mConstants = new double*[mCellmlFileRuntime->constantsCount()];

        memset(mConstants, 0, mCellmlFileRuntime->constantsCount()*SizeOfDoublePointer);
    } catch(...) {
        deleteArrays();

        return false;
    }

    for (int i = 0, iMax = mCellmlFileRuntime->constantsCount(); i < iMax; ++i)
        try {
            mConstants[i] = new double[dataSize];
        } catch(...) {
            deleteArrays();

            return false;
        }

    // Create our states arrays

    try {
        mStates = new double*[mCellmlFileRuntime->statesCount()];

        memset(mStates, 0, mCellmlFileRuntime->statesCount()*SizeOfDoublePointer);
    } catch(...) {
        deleteArrays();

        return false;
    }

    for (int i = 0, iMax = mCellmlFileRuntime->statesCount(); i < iMax; ++i)
        try {
            mStates[i] = new double[dataSize];
        } catch(...) {
            deleteArrays();

            return false;
        }

    // Create our rates arrays

    try {
        mRates = new double*[mCellmlFileRuntime->ratesCount()];

        memset(mRates, 0, mCellmlFileRuntime->ratesCount()*SizeOfDoublePointer);
    } catch(...) {
        deleteArrays();

        return false;
    }

    for (int i = 0, iMax = mCellmlFileRuntime->ratesCount(); i < iMax; ++i)
        try {
            mRates[i] = new double[dataSize];
        } catch(...) {
            deleteArrays();

            return false;
        }

    // Create our algebraic arrays

    try {
        mAlgebraic = new double*[mCellmlFileRuntime->algebraicCount()];

        memset(mAlgebraic, 0, mCellmlFileRuntime->algebraicCount()*SizeOfDoublePointer);
    } catch(...) {
        deleteArrays();

        return false;
    }

    for (int i = 0, iMax = mCellmlFileRuntime->algebraicCount(); i < iMax; ++i)
        try {
            mAlgebraic[i] = new double[dataSize];
        } catch(...) {
            deleteArrays();

            return false;
        }

    // We could allocate all of our required memory, so...

    return true;
}

//==============================================================================

void SingleCellSimulationViewSimulationResults::deleteArrays()
{
    // Delete our points array

    delete[] mPoints;

    mPoints = 0;

    // Delete our constants arrays

    if (mConstants)
        for (int i = 0, iMax = mCellmlFileRuntime->constantsCount(); i < iMax; ++i)
            delete[] mConstants[i];

    delete mConstants;

    mConstants = 0;

    // Delete our states arrays

    if (mStates)
        for (int i = 0, iMax = mCellmlFileRuntime->statesCount(); i < iMax; ++i)
            delete[] mStates[i];

    delete mStates;

    mStates = 0;

    // Delete our rates arrays

    if (mRates)
        for (int i = 0, iMax = mCellmlFileRuntime->ratesCount(); i < iMax; ++i)
            delete[] mRates[i];

    delete mRates;

    mRates = 0;

    // Delete our algebraic arrays

    if (mAlgebraic)
        for (int i = 0, iMax = mCellmlFileRuntime->algebraicCount(); i < iMax; ++i)
            delete[] mAlgebraic[i];

    delete mAlgebraic;

    mAlgebraic = 0;
}

//==============================================================================

bool SingleCellSimulationViewSimulationResults::reset(const bool &pCreateArrays)
{
    // Reset our size

    mSize = 0;

    // Reset our arrays

    deleteArrays();

    bool res = pCreateArrays?createArrays():true;

    // Let people know that our results have been updated

    if (pCreateArrays && res)
        emit updated();

    // We are done, so...

    return res;
}

//==============================================================================

void SingleCellSimulationViewSimulationResults::addPoint(const double &pPoint,
                                                         const bool &pEmitSignal)
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

    // Let people know that our results have been updated

    if (pEmitSignal)
        emit updated();
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

    static const QString Header = "%1 | %2 (%3)";

    out << Header.arg(mCellmlFileRuntime->variableOfIntegration()->component(),
                      mCellmlFileRuntime->variableOfIntegration()->name(),
                      mCellmlFileRuntime->variableOfIntegration()->unit());

    for (int i = 0, iMax = mCellmlFileRuntime->modelParameters().count(); i < iMax; ++i) {
        CellMLSupport::CellmlFileRuntimeModelParameter *modelParameter = mCellmlFileRuntime->modelParameters()[i];

        out << "," << Header.arg(modelParameter->component(),
                                 modelParameter->name()+QString(modelParameter->degree(), '\''),
                                 modelParameter->unit());
    }

    out << "\n";

    // Data itself

    for (int j = 0; j < mSize; ++j) {
        out << mPoints[j];

        for (int i = 0, iMax = mCellmlFileRuntime->modelParameters().count(); i < iMax; ++i) {
            CellMLSupport::CellmlFileRuntimeModelParameter *modelParameter = mCellmlFileRuntime->modelParameters()[i];

            switch (modelParameter->type()) {
            case CellMLSupport::CellmlFileRuntimeModelParameter::Constant:
            case CellMLSupport::CellmlFileRuntimeModelParameter::ComputedConstant:
                out << "," << mConstants[modelParameter->index()][j];

                break;
            case CellMLSupport::CellmlFileRuntimeModelParameter::State:
                out << "," << mStates[modelParameter->index()][j];

                break;
            case CellMLSupport::CellmlFileRuntimeModelParameter::Rate:
                out << "," << mRates[modelParameter->index()][j];

                break;
            case CellMLSupport::CellmlFileRuntimeModelParameter::Algebraic:
                out << "," << mAlgebraic[modelParameter->index()][j];

                break;
            default:
                // Either Voi or Undefined, so...

                ;
            }
        }

        out << "\n";
    }

    // We are done, so close our file

    file.close();

    // Everything went fine, so...

    return true;
}

//==============================================================================

SingleCellSimulationViewSimulation::SingleCellSimulationViewSimulation(const QString &pFileName,
                                                                       CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                                       const SolverInterfaces &pSolverInterfaces) :
    mWorker(0),
    mWorkerThread(0),
    mFileName(pFileName),
    mCellmlFileRuntime(pCellmlFileRuntime),
    mSolverInterfaces(pSolverInterfaces),
    mWorkerStatus(Unknown),
    mWorkerProgress(0.0),
    mData(new SingleCellSimulationViewSimulationData(pCellmlFileRuntime, pSolverInterfaces)),
    mResults(new SingleCellSimulationViewSimulationResults(pCellmlFileRuntime, mData))
{
    // Keep track of any error occurring in our data

    connect(mData, SIGNAL(error(const QString &)),
            this, SIGNAL(error(const QString &)));
}

//==============================================================================

SingleCellSimulationViewSimulation::~SingleCellSimulationViewSimulation()
{
    // Stop our worker (just in case...)

    stop();

    // Delete some internal objects

    delete mResults;
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

SingleCellSimulationViewSimulationResults * SingleCellSimulationViewSimulation::results() const
{
    // Return our results

    return mResults;
}

//==============================================================================

SingleCellSimulationViewSimulation::WorkerStatus SingleCellSimulationViewSimulation::workerStatus() const
{
    // Return our worker's status

    return mWorkerStatus;
}

//==============================================================================

double SingleCellSimulationViewSimulation::workerProgress() const
{
    // Return our worker's progress

    return mWorkerProgress;
}

//==============================================================================

void SingleCellSimulationViewSimulation::setDelay(const int &pDelay)
{
    // Set our delay

    mData->setDelay(pDelay);
}

//==============================================================================

bool SingleCellSimulationViewSimulation::reset(const bool &pResultsArrays)
{
    // Reset both our data and results

    mData->reset();

    return mResults->reset(pResultsArrays);
}

//==============================================================================

void SingleCellSimulationViewSimulation::recomputeComputedConstantsAndVariables()
{
    // Recompute our 'computed constants' and 'variables'

    mData->recomputeComputedConstantsAndVariables();
}

//==============================================================================

double SingleCellSimulationViewSimulation::requiredMemory() const
{
    // Determine and return the amount of required memory to run our simulation
    // Note: we return the amount as a double rather than a qulonglong (as we do
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

void SingleCellSimulationViewSimulation::run()
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

        mWorker = new SingleCellSimulationViewSimulationWorker(mSolverInterfaces, mCellmlFileRuntime, this);

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

        mWorkerStatus   = Idling;
        mWorkerProgress = 0.0;

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

    mWorkerStatus   = Unknown;
    mWorkerProgress = 0.0;

    // Let people know that we have stopped

    emit stopped(pElapsedTime);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

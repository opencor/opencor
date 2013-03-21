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

SingleCellSimulationViewSimulationData::SingleCellSimulationViewSimulationData(CellMLSupport::CellmlFileRuntime *pRuntime,
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
        mStates    = new double[pRuntime->statesCount()];
        mRates     = new double[pRuntime->ratesCount()];
        mAlgebraic = new double[pRuntime->algebraicCount()];
        mCondVar   = new double[pRuntime->condVarCount()];

        // Create our various arrays to keep track of our various initial values

        mInitialConstants = new double[pRuntime->constantsCount()];
        mInitialStates    = new double[pRuntime->statesCount()];
    } else {
        mConstants = mStates = mRates = mAlgebraic = mCondVar = 0;
        mInitialConstants = mInitialStates = 0;
    }
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

    delete[] mInitialConstants;
    delete[] mInitialStates;
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

QString SingleCellSimulationViewSimulationData::odeSolverName() const
{
    // Return our ODE solver name

    return mRuntime->needOdeSolver()?mOdeSolverName:QString();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setOdeSolverName(const QString &pOdeSolverName)
{
    // Set our ODE solver name

    if (mRuntime->needOdeSolver())
        mOdeSolverName = pOdeSolverName;
}

//==============================================================================

CoreSolver::Properties SingleCellSimulationViewSimulationData::odeSolverProperties() const
{
    // Return our ODE solver's properties

    return mRuntime->needOdeSolver()?mOdeSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::addOdeSolverProperty(const QString &pName,
                                                                  const QVariant &pValue)
{
    // Add an ODE solver property

    if (mRuntime->needOdeSolver())
        mOdeSolverProperties.insert(pName, pValue);
}

//==============================================================================

QString SingleCellSimulationViewSimulationData::daeSolverName() const
{
    // Return our DAE solver name

    return mRuntime->needDaeSolver()?mDaeSolverName:QString();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setDaeSolverName(const QString &pDaeSolverName)
{
    // Set our DAE solver name

    if (mRuntime->needDaeSolver())
        mDaeSolverName = pDaeSolverName;
}

//==============================================================================

CoreSolver::Properties SingleCellSimulationViewSimulationData::daeSolverProperties() const
{
    // Return our DAE solver's properties

    return mRuntime->needDaeSolver()?mDaeSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::addDaeSolverProperty(const QString &pName,
                                                                  const QVariant &pValue)
{
    // Add an DAE solver property

    if (mRuntime->needDaeSolver())
        mDaeSolverProperties.insert(pName, pValue);
}

//==============================================================================

QString SingleCellSimulationViewSimulationData::nlaSolverName() const
{
    // Return our NLA solver name

    return mRuntime->needNlaSolver()?mNlaSolverName:QString();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setNlaSolverName(const QString &pNlaSolverName,
                                                              const bool &pReset)
{
    // Set our NLA solver name

    if (mRuntime->needNlaSolver()) {
        mNlaSolverName = pNlaSolverName;

        // Reset our model parameter values, if required
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

    return mRuntime->needNlaSolver()?mNlaSolverProperties:CoreSolver::Properties();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::addNlaSolverProperty(const QString &pName,
                                                                  const QVariant &pValue,
                                                                  const bool &pReset)
{
    // Add an NLA solver property

    if (mRuntime->needNlaSolver()) {
        mNlaSolverProperties.insert(pName, pValue);

        // Reset our model parameter values, if required
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

    if (mRuntime->needNlaSolver()) {
        // Retrieve an instance of our NLA solver

        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare(mNlaSolverName)) {
                // The requested NLA solver was found, so retrieve an instance
                // of it

                nlaSolver = static_cast<CoreSolver::CoreNlaSolver *>(solverInterface->instance());

                // Keep track of our NLA solver, so that doNonLinearSolve() can
                // work as expected

                CoreSolver::setNlaSolver(mRuntime->address(), nlaSolver);

                break;
            }

        // Keep track of any error that might be reported by our NLA solver

        connect(nlaSolver, SIGNAL(error(const QString &)),
                this, SIGNAL(error(const QString &)));

        // Initialise our NLA solver

        nlaSolver->setProperties(mNlaSolverProperties);
    }

    // Reset our model parameter values

    static const int SizeOfDouble = sizeof(double);

    memset(mConstants, 0, mRuntime->constantsCount()*SizeOfDouble);
    memset(mStates, 0, mRuntime->statesCount()*SizeOfDouble);
    memset(mRates, 0, mRuntime->ratesCount()*SizeOfDouble);
    memset(mAlgebraic, 0, mRuntime->algebraicCount()*SizeOfDouble);
    memset(mCondVar, 0, mRuntime->condVarCount()*SizeOfDouble);

    mRuntime->initializeConstants()(mConstants, mRates, mStates);
    recomputeComputedConstantsAndVariables();

    // Delete our NLA solver, if any

    if (nlaSolver) {
        delete nlaSolver;

        CoreSolver::unsetNlaSolver(mRuntime->address());
    }

    // Keep track of our various initial values

    memcpy(mInitialConstants, mConstants, mRuntime->constantsCount()*SizeOfDouble);
    memcpy(mInitialStates, mStates, mRuntime->statesCount()*SizeOfDouble);

    // Let people know that our data is 'cleaned', i.e. not modified

    emit modified(false);
}

//==============================================================================

void SingleCellSimulationViewSimulationData::recomputeComputedConstantsAndVariables()
{
    // Recompute our 'computed constants' and 'variables', if possible

    if (mRuntime && mRuntime->isValid()) {
        mRuntime->computeComputedConstants()(mConstants, mRates, mStates);
        mRuntime->computeVariables()(mStartingPoint, mConstants, mRates, mStates, mAlgebraic);

        // Let people know that our data has been updated

        emit updated();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationData::recomputeVariables(const double &pCurrentPoint,
                                                                const bool &pEmitSignal)
{
    // Recompute our 'variables'

    mRuntime->computeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);

    // Let people know that our data has been updated, if requested
    // Note: recomputeVariables() will normally be called many times when
    //       running a simulation to ensure that all of our 'variables' are
    //       up-to-date and to emit loads of signals wouldn't be a good idea,
    //       hence the caller can decide whether to emit a signal or not...

    if (pEmitSignal)
        emit updated();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::checkForModifications()
{
    // Check whether any of our constants or states has been modified

    foreach (CellMLSupport::CellmlFileRuntimeModelParameter *modelParameter, mRuntime->modelParameters())
        switch (modelParameter->type()) {
        case CellMLSupport::CellmlFileRuntimeModelParameter::Constant:
            if (mConstants[modelParameter->index()] != mInitialConstants[modelParameter->index()]) {
                emit modified(true);

                return;
            }

            break;
        case CellMLSupport::CellmlFileRuntimeModelParameter::State:
            if (mStates[modelParameter->index()] != mInitialStates[modelParameter->index()]) {
                emit modified(true);

                return;
            }

            break;
        default:
            // Either Voi, ComputedConstant, Rate, Algebraic or Undefined, so...

            ;
        }

    // Let people know that no data has been modified

    emit modified(false);
}

//==============================================================================

SingleCellSimulationViewSimulationResults::SingleCellSimulationViewSimulationResults(CellMLSupport::CellmlFileRuntime *pRuntime,
                                                                                     SingleCellSimulationViewSimulation *pSimulation) :
    mRuntime(pRuntime),
    mSimulation(pSimulation),
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

    // Retrieve the size of our data and make sure that it is valid

    qulonglong simulationSize = qulonglong(mSimulation->size());

    if (!simulationSize)
        return true;

    // Create our points array

    try {
        mPoints = new double[simulationSize];
    } catch(...) {
        return false;
    }

    // Create our constants arrays

    try {
        mConstants = new double*[mRuntime->constantsCount()];

        memset(mConstants, 0, mRuntime->constantsCount()*SizeOfDoublePointer);
    } catch(...) {
        deleteArrays();

        return false;
    }

    for (int i = 0, iMax = mRuntime->constantsCount(); i < iMax; ++i)
        try {
            mConstants[i] = new double[simulationSize];
        } catch(...) {
            deleteArrays();

            return false;
        }

    // Create our states arrays

    try {
        mStates = new double*[mRuntime->statesCount()];

        memset(mStates, 0, mRuntime->statesCount()*SizeOfDoublePointer);
    } catch(...) {
        deleteArrays();

        return false;
    }

    for (int i = 0, iMax = mRuntime->statesCount(); i < iMax; ++i)
        try {
            mStates[i] = new double[simulationSize];
        } catch(...) {
            deleteArrays();

            return false;
        }

    // Create our rates arrays

    try {
        mRates = new double*[mRuntime->ratesCount()];

        memset(mRates, 0, mRuntime->ratesCount()*SizeOfDoublePointer);
    } catch(...) {
        deleteArrays();

        return false;
    }

    for (int i = 0, iMax = mRuntime->ratesCount(); i < iMax; ++i)
        try {
            mRates[i] = new double[simulationSize];
        } catch(...) {
            deleteArrays();

            return false;
        }

    // Create our algebraic arrays

    try {
        mAlgebraic = new double*[mRuntime->algebraicCount()];

        memset(mAlgebraic, 0, mRuntime->algebraicCount()*SizeOfDoublePointer);
    } catch(...) {
        deleteArrays();

        return false;
    }

    for (int i = 0, iMax = mRuntime->algebraicCount(); i < iMax; ++i)
        try {
            mAlgebraic[i] = new double[simulationSize];
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
        for (int i = 0, iMax = mRuntime->constantsCount(); i < iMax; ++i)
            delete[] mConstants[i];

    delete mConstants;

    mConstants = 0;

    // Delete our states arrays

    if (mStates)
        for (int i = 0, iMax = mRuntime->statesCount(); i < iMax; ++i)
            delete[] mStates[i];

    delete mStates;

    mStates = 0;

    // Delete our rates arrays

    if (mRates)
        for (int i = 0, iMax = mRuntime->ratesCount(); i < iMax; ++i)
            delete[] mRates[i];

    delete mRates;

    mRates = 0;

    // Delete our algebraic arrays

    if (mAlgebraic)
        for (int i = 0, iMax = mRuntime->algebraicCount(); i < iMax; ++i)
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

    return pCreateArrays?createArrays():true;
}

//==============================================================================

void SingleCellSimulationViewSimulationResults::addPoint(const double &pPoint)
{
    // Add the data to our different arrays

    mPoints[mSize] = pPoint;

    for (int i = 0, iMax = mRuntime->constantsCount(); i < iMax; ++i)
        mConstants[i][mSize] = mSimulation->data()->constants()[i];

    for (int i = 0, iMax = mRuntime->statesCount(); i < iMax; ++i)
        mStates[i][mSize] = mSimulation->data()->states()[i];

    for (int i = 0, iMax = mRuntime->ratesCount(); i < iMax; ++i)
        mRates[i][mSize] = mSimulation->data()->rates()[i];

    for (int i = 0, iMax = mRuntime->algebraicCount(); i < iMax; ++i)
        mAlgebraic[i][mSize] = mSimulation->data()->algebraic()[i];

    // Increase our size

    ++mSize;
}

//==============================================================================

qulonglong SingleCellSimulationViewSimulationResults::size() const
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

    out << Header.arg(mRuntime->variableOfIntegration()->component(),
                      mRuntime->variableOfIntegration()->name(),
                      mRuntime->variableOfIntegration()->unit());

    for (int i = 0, iMax = mRuntime->modelParameters().count(); i < iMax; ++i) {
        CellMLSupport::CellmlFileRuntimeModelParameter *modelParameter = mRuntime->modelParameters()[i];

        out << "," << Header.arg(modelParameter->component(),
                                 modelParameter->name()+QString(modelParameter->degree(), '\''),
                                 modelParameter->unit());
    }

    out << "\n";

    // Data itself

    for (qulonglong j = 0; j < mSize; ++j) {
        out << mPoints[j];

        for (int i = 0, iMax = mRuntime->modelParameters().count(); i < iMax; ++i) {
            CellMLSupport::CellmlFileRuntimeModelParameter *modelParameter = mRuntime->modelParameters()[i];

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
                                                                       CellMLSupport::CellmlFileRuntime *pRuntime,
                                                                       const SolverInterfaces &pSolverInterfaces) :
    mWorker(0),
    mFileName(pFileName),
    mRuntime(pRuntime),
    mSolverInterfaces(pSolverInterfaces),
    mData(new SingleCellSimulationViewSimulationData(pRuntime, pSolverInterfaces)),
    mResults(new SingleCellSimulationViewSimulationResults(pRuntime, this))
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

bool SingleCellSimulationViewSimulation::isRunning() const
{
    // Return whether we are running

    return mWorker?mWorker->isRunning():false;
}

//==============================================================================

bool SingleCellSimulationViewSimulation::isPaused() const
{
    // Return whether we are paused

    return mWorker?mWorker->isPaused():false;
}

//==============================================================================

double SingleCellSimulationViewSimulation::progress() const
{
    // Return our progress

    return mWorker?mWorker->progress():0.0;
}

//==============================================================================

void SingleCellSimulationViewSimulation::setDelay(const int &pDelay)
{
    // Set our delay

    mData->setDelay(pDelay);
}

//==============================================================================

double SingleCellSimulationViewSimulation::requiredMemory()
{
    // Determine and return the amount of required memory to run our simulation
    // Note: we return the amount as a double rather than a qulonglong (as we do
    //       when retrieving the total/free amount of memory available; see
    //       [OpenCOR]/src/plugins/misc/Core/src/coreutils.cpp) in case a
    //       simulation requires an insane amount of memory...

    static const int SizeOfDouble = sizeof(double);

    return  size()
           *( 1
             +mRuntime->constantsCount()
             +mRuntime->statesCount()
             +mRuntime->ratesCount()
             +mRuntime->algebraicCount())
           *SizeOfDouble;
}

//==============================================================================

bool SingleCellSimulationViewSimulation::simulationSettingsOk(const bool &pEmitError)
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

double SingleCellSimulationViewSimulation::size()
{
    // Return the size of our simulation (i.e. the number of data points which
    // should be generated)
    // Note: we return a double rather than a qulonglong in case the simulation
    //       requires an insane amount of memory...

    if (simulationSettingsOk(false))
        // Our simulation settings are fine, so...

        return ceil((mData->endingPoint()-mData->startingPoint())/mData->pointInterval())+1.0;
    else
        // Something wrong with our simulation settings, so...

        return 0.0;
}

//==============================================================================

void SingleCellSimulationViewSimulation::run()
{
    // Initialise our worker, if not active

    if (!mWorker) {
        // First, check that our simulation settings we were given are sound

        if (!simulationSettingsOk())
            // Something wrong with our simulation settings, so...

            return;

        // Create our worker

        mWorker = new SingleCellSimulationViewSimulationWorker(mSolverInterfaces, mRuntime, this, &mWorker);

        if (!mWorker) {
            emit error(tr("the simulation worker could not be created"));

            return;
        }

        // Create a few connections

        connect(mWorker, SIGNAL(running(const bool &)),
                this, SIGNAL(running(const bool &)));
        connect(mWorker, SIGNAL(paused()),
                this, SIGNAL(paused()));

        connect(mWorker, SIGNAL(finished(const int &)),
                this, SIGNAL(stopped(const int &)));

        connect(mWorker, SIGNAL(error(const QString &)),
                this, SIGNAL(error(const QString &)));

        // Start our worker thread

        mWorker->run();
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

void SingleCellSimulationViewSimulation::resetWorker()
{
    // Ask our worker to reset itself

    if (mWorker)
        mWorker->reset();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

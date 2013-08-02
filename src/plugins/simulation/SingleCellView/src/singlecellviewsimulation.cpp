//==============================================================================
// Single cell view simulation
//==============================================================================

#include "cellmlfileruntime.h"
#include "corenlasolver.h"
#include "singlecellviewcontentswidget.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewinformationwidget.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewwidget.h"
#include "solverinterface.h"

//==============================================================================

#include <QFile>
#include <QTextStream>

//==============================================================================

#include <QtNumeric>

//==============================================================================

#include <qmath.h>

//==============================================================================

#include "qwt_slider.h"

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

    // Recompute our 'computed constants' and 'variables'

    if (pRecompute)
        recomputeComputedConstantsAndVariables(mStartingPoint);
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

void SingleCellViewSimulationData::reset()
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

    // Reset our parameter values

    memset(mConstants, 0, mRuntime->constantsCount()*CoreSolver::SizeOfDouble);
    memset(mRates, 0, mRuntime->ratesCount()*CoreSolver::SizeOfDouble);
    memset(mStates, 0, mRuntime->statesCount()*CoreSolver::SizeOfDouble);
    memset(mAlgebraic, 0, mRuntime->algebraicCount()*CoreSolver::SizeOfDouble);
    memset(mCondVar, 0, mRuntime->condVarCount()*CoreSolver::SizeOfDouble);

    mRuntime->initializeConstants()(mConstants, mRates, mStates);
    recomputeComputedConstantsAndVariables(mStartingPoint);

    // Delete our NLA solver, if any

    if (nlaSolver) {
        delete nlaSolver;

        CoreSolver::unsetNlaSolver(mRuntime->address());
    }

    // Keep track of our various initial values

    memcpy(mInitialConstants, mConstants, mRuntime->constantsCount()*CoreSolver::SizeOfDouble);
    memcpy(mInitialStates, mStates, mRuntime->statesCount()*CoreSolver::SizeOfDouble);

    // Let people know that our data is 'cleaned', i.e. not modified

    emit modified(false);
}

//==============================================================================

void SingleCellViewSimulationData::recomputeComputedConstantsAndVariables(const double &pCurrentPoint)
{
    if (!mRuntime)
        return;

    // Recompute our 'computed constants' and 'variables', if possible

    if (mRuntime->isValid()) {
        mRuntime->computeComputedConstants()(mConstants, mRates, mStates);

        if (mRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)
            mRuntime->computeOdeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);
        else
            mRuntime->computeDaeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic, mCondVar);

        // Let people know that our data has been updated

        emit updated(pCurrentPoint);
    }
}

//==============================================================================

void SingleCellViewSimulationData::recomputeVariables(const double &pCurrentPoint,
                                                      const bool &pEmitSignal)
{
    if (!mRuntime)
        return;

    // Recompute our 'variables'

    if (mRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)
        mRuntime->computeOdeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic);
    else
        mRuntime->computeDaeVariables()(pCurrentPoint, mConstants, mRates, mStates, mAlgebraic, mCondVar);

    // Let people know that our data has been updated, if requested
    // Note: recomputeVariables() will normally be called many times when
    //       running a simulation to ensure that all of our 'variables' are
    //       up-to-date and to emit loads of signals wouldn't be a good idea,
    //       hence the caller can decide whether to emit a signal or not...

    if (pEmitSignal)
        emit updated(pCurrentPoint);
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
        if (!qIsFinite(mStates[i]) || (mStates[i] != mInitialStates[i]))
            return true;

    for (int i = 0, iMax = mRuntime->constantsCount(); i < iMax; ++i)
        if (!qIsFinite(mConstants[i]) || (mConstants[i] != mInitialConstants[i]))
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
    mPoints(0),
    mConstants(0),
    mRates(0),
    mStates(0),
    mAlgebraic(0)
{
}

//==============================================================================

SingleCellViewSimulationResults::~SingleCellViewSimulationResults()
{
    // Delete some internal objects

    deleteArrays();
}

//==============================================================================

bool SingleCellViewSimulationResults::createArrays()
{
    // Note: the boolean value we return is true if we have had no problem
    //       allocating memory, false otherwise. This is the reason, for
    //       example, we return true when there is either no runtime or the
    //       simulation size is zero...

    // Clean things up

    deleteArrays();

    // Make sure that we have a runtime

    if (!mRuntime)
        return true;

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

        memset(mConstants, 0, mRuntime->constantsCount()*CoreSolver::SizeOfDoublePointer);
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

    // Create our rates arrays

    try {
        mRates = new double*[mRuntime->ratesCount()];

        memset(mRates, 0, mRuntime->ratesCount()*CoreSolver::SizeOfDoublePointer);
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

    // Create our states arrays

    try {
        mStates = new double*[mRuntime->statesCount()];

        memset(mStates, 0, mRuntime->statesCount()*CoreSolver::SizeOfDoublePointer);
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

    // Create our algebraic arrays

    try {
        mAlgebraic = new double*[mRuntime->algebraicCount()];

        memset(mAlgebraic, 0, mRuntime->algebraicCount()*CoreSolver::SizeOfDoublePointer);
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

void SingleCellViewSimulationResults::deleteArrays()
{
    // Delete our points array

    delete[] mPoints;

    mPoints = 0;

    // Delete our constants arrays

    if (mRuntime && mConstants)
        for (int i = 0, iMax = mRuntime->constantsCount(); i < iMax; ++i)
            delete[] mConstants[i];

    delete mConstants;

    mConstants = 0;

    // Delete our rates arrays

    if (mRuntime && mRates)
        for (int i = 0, iMax = mRuntime->ratesCount(); i < iMax; ++i)
            delete[] mRates[i];

    delete mRates;

    mRates = 0;

    // Delete our states arrays

    if (mRuntime && mStates)
        for (int i = 0, iMax = mRuntime->statesCount(); i < iMax; ++i)
            delete[] mStates[i];

    delete mStates;

    mStates = 0;

    // Delete our algebraic arrays

    if (mRuntime && mAlgebraic)
        for (int i = 0, iMax = mRuntime->algebraicCount(); i < iMax; ++i)
            delete[] mAlgebraic[i];

    delete mAlgebraic;

    mAlgebraic = 0;
}

//==============================================================================

bool SingleCellViewSimulationResults::reset(const bool &pCreateArrays)
{
    // Reset our size

    mSize = 0;

    // Reset our arrays

    if (pCreateArrays) {
        return createArrays();
    } else {
        deleteArrays();

        return true;
    }
}

//==============================================================================

void SingleCellViewSimulationResults::addPoint(const double &pPoint)
{
    if (!mRuntime)
        return;

    // Add the data to our different arrays

    mPoints[mSize] = pPoint;

    for (int i = 0, iMax = mRuntime->constantsCount(); i < iMax; ++i)
        mConstants[i][mSize] = mSimulation->data()->constants()[i];

    for (int i = 0, iMax = mRuntime->ratesCount(); i < iMax; ++i)
        mRates[i][mSize] = mSimulation->data()->rates()[i];

    for (int i = 0, iMax = mRuntime->statesCount(); i < iMax; ++i)
        mStates[i][mSize] = mSimulation->data()->states()[i];

    for (int i = 0, iMax = mRuntime->algebraicCount(); i < iMax; ++i)
        mAlgebraic[i][mSize] = mSimulation->data()->algebraic()[i];

    // Increase our size

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

    return mPoints;
}

//==============================================================================

double ** SingleCellViewSimulationResults::constants() const
{
    // Return our constants array

    return mConstants;
}

//==============================================================================

double ** SingleCellViewSimulationResults::rates() const
{
    // Return our rates array

    return mRates;
}

//==============================================================================

double ** SingleCellViewSimulationResults::states() const
{
    // Return our states array

    return mStates;
}

//==============================================================================

double ** SingleCellViewSimulationResults::algebraic() const
{
    // Return our algebraic array

    return mAlgebraic;
}

//==============================================================================

bool SingleCellViewSimulationResults::exportToCsv(const QString &pFileName) const
{
    if (!mRuntime)
        return false;

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

    for (int i = 0, iMax = mRuntime->parameters().count(); i < iMax; ++i) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = mRuntime->parameters()[i];

        if (parameter != mRuntime->variableOfIntegration())
            out << "," << Header.arg(parameter->component(),
                                     parameter->formattedName(),
                                     parameter->formattedUnit(mRuntime->variableOfIntegration()->name()));
    }

    out << "\n";

    // Data itself

    for (qulonglong j = 0; j < mSize; ++j) {
        out << mPoints[j];

        for (int i = 0, iMax = mRuntime->parameters().count(); i < iMax; ++i) {
            CellMLSupport::CellmlFileRuntimeParameter *parameter = mRuntime->parameters()[i];

            switch (parameter->type()) {
            case CellMLSupport::CellmlFileRuntimeParameter::Constant:
            case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
                out << "," << mConstants[parameter->index()][j];

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Rate:
                out << "," << mRates[parameter->index()][j];

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::State:
                out << "," << mStates[parameter->index()][j];

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
                out << "," << mAlgebraic[parameter->index()][j];

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
    // Stop our worker (just in case...)

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
    //          see [OpenCOR]/src/plugins/misc/Core/src/coreutils.cpp) in case a
    //          simulation requires an insane amount of memory...
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

void SingleCellViewSimulation::run()
{
    if (!mRuntime)
        return;

    // Initialise our worker, if not active

    if (!mWorker) {
        // First, check that our simulation settings we were given are sound

        if (!simulationSettingsOk())
            // Something wrong with our simulation settings, so...

            return;

        // Create our worker

        mWorker = new SingleCellViewSimulationWorker(mSolverInterfaces, mRuntime, this, &mWorker);

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

        // Start our worker

        mWorker->run();
    }
}

//==============================================================================

void SingleCellViewSimulation::pause()
{
    // Pause our worker

    if (mWorker)
        mWorker->pause();
}

//==============================================================================

void SingleCellViewSimulation::resume()
{
    // Resume our worker

    if (mWorker)
        mWorker->resume();
}

//==============================================================================

void SingleCellViewSimulation::stop()
{
    // Stop our worker

    if (mWorker)
        mWorker->stop();
}

//==============================================================================

void SingleCellViewSimulation::reset()
{
    // Reset our data

    mData->reset();

    // Reset our worker

    if (mWorker)
        mWorker->reset();
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Single cell view simulation
//==============================================================================

#include "cellmlfileruntime.h"
#include "singlecellviewcontentswidget.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewinformationwidget.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewwidget.h"
#include "CISBootstrap.hpp"

//==============================================================================

#include <QFile>
#include <QTextStream>
#include <QAtomicInt>

//==============================================================================

#include <QtNumeric>

//==============================================================================

#include <qmath.h>

//==============================================================================

#include "qwt_slider.h"

//==============================================================================

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif


namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

GrabInitialValueListener::GrabInitialValueListener(iface::cellml_services::CellMLIntegrationRun* pRun)
    : mHadResults(false), mRun(pRun)
{
}

void
GrabInitialValueListener::results(const std::vector<double>& pState) throw() {
    if (mHadResults)
        return;
    mHadResults = true;
    mRun->stop();
    mState = pState;
    emit resultsReady();
}

void
GrabInitialValueListener::failed(const std::string& aFailWhy) throw() {
    solveFailure(QString::fromStdString(aFailWhy));
}

//==============================================================================

ResultListener::ResultListener(iface::cellml_services::CellMLIntegrationRun* pRun,
                               int pNStates, int pNAlgebraic)
    : mRun(pRun), mNStates(pNStates), mNAlgebraic(pNAlgebraic), mSuspended(false)
{
}

void
ResultListener::suspend()
{
    if (mSuspended)
        return;
    mSuspended = true;
    mResultMutex.lock();
}

void
ResultListener::unsuspend()
{
    if (!mSuspended)
        return;
    mSuspended = false;
    mResultMutex.unlock();
}

void
ResultListener::results(const std::vector<double>& pState)
    throw()
{
    std::vector<double>::const_iterator i = pState.begin();
    while (i != pState.end()) {
        // Briefly acquire the result mutex. This will block while we are
        // suspended due to a pause.
        mResultMutex.lock();
        mResultMutex.unlock();
        double bvar = *i++;
        QList<double> states, rates, algebraic;
        states.reserve(mNStates);
        rates.reserve(mNStates);
        algebraic.reserve(mNAlgebraic);
        for (int j = 0; j < mNStates; j++)
            states << *i++;
        for (int j = 0; j < mNStates; j++)
            rates << *i++;
        for (int j = 0; j < mNAlgebraic; j++)
            algebraic << *i++;
        solvePointAvailable(bvar, states, rates, algebraic);

        int delay = mDelay.load();
        if (delay) {
#ifdef Q_OS_WIN
            Sleep(uint(delay));
#else
            usleep(delay * 1000);
#endif
        }
    }
}

void
ResultListener::done()
    throw()
{
    solveDone();
}

void
ResultListener::failed(const std::string& pFailWhy)
    throw()
{
    solveFailure(QString::fromStdString(pFailWhy));
}

//==============================================================================

SingleCellViewSimulationData::SingleCellViewSimulationData(CellMLSupport::CellMLFileRuntime *pRuntime) :
    mRuntime(pRuntime),
    mState(SIMSTATE_INITIAL),
    mDebug(false),
    mDelay(0),
    mStartingPoint(0.0),
    mEndingPoint(1000.0),
    mPointInterval(1.0),
    mSolverName(QString()),
    mSolverProperties(Properties())
{
    ensureCodeCompiled();
}

//==============================================================================

SingleCellViewSimulationData::~SingleCellViewSimulationData()
{
    stopAllSimulations();
}

//==============================================================================

void SingleCellViewSimulationData::ensureCodeCompiled()
{
    if (mRuntime) {
        bool isDAEType = isDAETypeSolver();
        if (isDAEType)
            mRuntime->ensureDAECompiledModel(mDebug);
        else
            mRuntime->ensureODECompiledModel(mDebug);
    }
}

void SingleCellViewSimulationData::setDebug(bool pDebug)
{
    if (mDebug == pDebug)
        return;

    mDebug = pDebug;
    stopAllSimulations();

    // Debug mode uses a completely different compiled model.
    if (mRuntime)
        ensureCodeCompiled();

    // We need to recalculate everything, as debug mode might pick up problems
    // during the recalculation.
    reset();
}

//==============================================================================

void SingleCellViewSimulationData::setPoint
(
 const QList<double>& pStates,
 const QList<double>& pRates,
 const QList<double>& pAlgebraic
)
{
    mStates = pStates;
    mRates = pRates;
    mAlgebraic = pAlgebraic;
}

//==============================================================================

void SingleCellViewSimulationData::pause()
{
    if (mState != SingleCellViewSimulationData::SIMSTATE_WAITING_RESULTS)
        return;

    if (mResultReceiver)
        mResultReceiver->suspend();

    mState = SingleCellViewSimulationData::SIMSTATE_PAUSED;
    if (mIntegrationRun)
        mIntegrationRun->pause();
}

void SingleCellViewSimulationData::resume()
{
    if (mResultReceiver)
        mResultReceiver->unsuspend();

    if (mIntegrationRun &&
        mState == SingleCellViewSimulationData::SIMSTATE_PAUSED)
    {
        mState = SingleCellViewSimulationData::SIMSTATE_WAITING_RESULTS;
        mIntegrationRun->resume();
    }
}


//==============================================================================

int SingleCellViewSimulationData::delay() const
{
    return mDelay;
}

//==============================================================================

void SingleCellViewSimulationData::setDelay(const int &pDelay)
{
    mDelay = pDelay;
    if (mResultReceiver)
        mResultReceiver->delay(mDelay);
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
        recomputeComputedConstantsAndVariables();
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

QString SingleCellViewSimulationData::solverName() const
{
    // Return our ODE solver name
    return mSolverName;
}

//==============================================================================

void SingleCellViewSimulationData::setSolverName(const QString &pSolverName)
{
    if (mSolverName == pSolverName)
        return;

    // Set our ODE solver name
    mSolverName = pSolverName;

    stopAllSimulations();

    ensureCodeCompiled();
}

//==============================================================================

bool SingleCellViewSimulationData::isDAETypeSolver() const
{
    return (mSolverName == "IDA");
}

//==============================================================================

Properties SingleCellViewSimulationData::solverProperties() const
{
    // Return our ODE solver's properties
    return mSolverProperties;
}

//==============================================================================

void SingleCellViewSimulationData::addSolverProperty(const QString &pName,
                                                     const QVariant &pValue)
{
    // Add an ODE solver property
    mSolverProperties.insert(pName, pValue);
}

//==============================================================================

// Asks all solver threads that might be returning data to stop. It is possible
// that solvers will not immediately notice the change. All signals are also
// disconnected, so no new data will be received after this is called.
void SingleCellViewSimulationData::stopAllSimulations()
{
    if (mIntegrationRun) {
        mIntegrationRun->stop();
        mIntegrationRun = NULL;
    }

    if (mIVGrabber) {
        mIVGrabber->disconnect();
        mIVGrabber = NULL;
    }

    if (mResultReceiver)
    {
        mResultReceiver->disconnect();
        // So it doesn't hang around for a long time processing cached data from
        // before the stop takes effect.
        mResultReceiver->delay(0);
        mResultReceiver->unsuspend();
        mResultReceiver = NULL;
    }

    if (mState == SIMSTATE_WAITING_IV)
        mState = SIMSTATE_INITIAL;
    else if (mState == SIMSTATE_WAITING_RESULTS ||
             mState == SIMSTATE_PAUSED)
        mState = SIMSTATE_GOT_IV;
}

void SingleCellViewSimulationData::newIntegrationRun()
{
    if (!mRuntime || !mRuntime->isValid())
        return;
    if (isDAETypeSolver() ? !mRuntime->daeCompiledModel() :
        !mRuntime->odeCompiledModel())
        return;

    ObjRef<iface::cellml_services::CellMLIntegrationService> cis(CreateIntegrationService());
    if (isDAETypeSolver())
        mIntegrationRun = cis->createDAEIntegrationRun(mRuntime->daeCompiledModel());
    else
    {
        mIntegrationRun = cis->createODEIntegrationRun(mRuntime->odeCompiledModel());
        ObjRef<iface::cellml_services::ODESolverRun>
            odeRun(QueryInterface(mIntegrationRun));

        // TODO have more options than just 'CVODE', allowing user to specify
        // which CVODE solver.
        odeRun->stepType(iface::cellml_services::BDF_IMPLICIT_1_5_SOLVE);
    }
}

void SingleCellViewSimulationData::reset()
{
    stopAllSimulations();
    newIntegrationRun();

    if (!mIntegrationRun)
        return;

    // Reset our model parameter values which means both initialising our
    // 'constants' and computing our 'computed constants' and 'variables'
    mConstants.clear();
    mStates.clear();
    mRates.clear();
    mAlgebraic.clear();
    mCondVar.clear();

    mState = SingleCellViewSimulationData::SIMSTATE_WAITING_IV;
    mIVGrabber = new GrabInitialValueListener(mIntegrationRun);
    mDidReset = true;
    QObject::connect(mIVGrabber, SIGNAL(resultsReady()), this, SLOT(initialValuesIn()));
    QObject::connect(mIVGrabber, SIGNAL(solveFailure(QString)), this, SLOT(initialValuesFailed(QString)));
    mIntegrationRun->setProgressObserver(mIVGrabber);
    mIntegrationRun->setResultRange(mStartingPoint, mStartingPoint, 1);
    
    mIntegrationRun->start();
}

void SingleCellViewSimulationData::initialValuesIn()
{
    iface::cellml_services::CellMLCompiledModel*
        compModel(isDAETypeSolver() ?
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->daeCompiledModel()) :
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->odeCompiledModel()));
    ObjRef<iface::cellml_services::CodeInformation> codeInfo
        (compModel->codeInformation());

    mState = SingleCellViewSimulationData::SIMSTATE_GOT_IV;

    if (mDidReset)
    {
      mInitialConstants.clear();
      mInitialConstants.reserve(codeInfo->constantIndexCount());
      mInitialStates.clear();
      mInitialStates.reserve(codeInfo->rateIndexCount());
    }

    mConstants.clear();
    mConstants.reserve(codeInfo->constantIndexCount());

    std::cout << "Computed " << mIVGrabber->consts().size() << " constants and "
              << mIVGrabber->states().size() << " initial values." << std::endl;
        

    for (std::vector<double>::iterator i = mIVGrabber->consts().begin();
         i != mIVGrabber->consts().end(); i++) {
        if (mDidReset)
            mInitialConstants << *i;
        mConstants << *i;
    }

    std::vector<double>& computedData = mIVGrabber->states();
    mStates.clear();
    mStates.reserve(codeInfo->rateIndexCount());
    for (unsigned int i = 0; i < codeInfo->rateIndexCount(); i++) {
        double v = computedData[1 + i];
        if (mDidReset)
            mInitialStates << v;
        mStates << v;
    }

    mRates.clear();
    mRates.reserve(codeInfo->rateIndexCount());
    const unsigned int rateOffset = codeInfo->rateIndexCount() + 1;
    for (unsigned int i = 0; i < codeInfo->rateIndexCount(); i++) {
      mRates << computedData[rateOffset + i];
    }
    
    mAlgebraic.clear();
    mAlgebraic.reserve(codeInfo->algebraicIndexCount());
    const unsigned int algebraicOffset = codeInfo->rateIndexCount() * 2 + 1;
    for (unsigned int i = 0; i < codeInfo->algebraicIndexCount(); i++) {
      mAlgebraic << computedData[algebraicOffset + i];
    }

    emit updated();
    emit modified(this, !mDidReset);
}

void SingleCellViewSimulationData::initialValuesFailed(QString pError)
{
    // Let people know that our data is 'cleaned', i.e. not modified
    emit modified(this, false);

    emit error("Problem computing initial values: " + pError);
}

//==============================================================================

// Recompute our 'computed constants' and 'variables', if possible
void SingleCellViewSimulationData::recomputeComputedConstantsAndVariables()
{
    stopAllSimulations();
    newIntegrationRun();
    
    if (!mIntegrationRun)
        return;

    mState = SingleCellViewSimulationData::SIMSTATE_WAITING_IV;
    mIVGrabber = new GrabInitialValueListener(mIntegrationRun);
    mDidReset = false;
    QObject::connect(mIVGrabber, SIGNAL(resultsReady()), this, SLOT(initialValuesIn()));
    QObject::connect(mIVGrabber, SIGNAL(solveFailure(QString)), this, SLOT(initialValuesFailed(QString)));
    mIntegrationRun->setProgressObserver(mIVGrabber);
    mIntegrationRun->setResultRange(mStartingPoint, mStartingPoint, 1);
    setupOverrides();

    mIntegrationRun->start();
}

void SingleCellViewSimulationData::setupOverrides()
{
    iface::cellml_services::CellMLCompiledModel*
        compModel(isDAETypeSolver() ?
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->daeCompiledModel()) :
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->odeCompiledModel()));
    ObjRef<iface::cellml_services::CodeInformation> codeInfo
        (compModel->codeInformation());
    
    for (unsigned int i = 0; i < codeInfo->constantIndexCount(); i++)
        if (mInitialConstants[i] != mConstants[i])
            mIntegrationRun->setOverride(iface::cellml_services::CONSTANT,
                                         i, mConstants[i]);
    for (unsigned int i = 0; i < codeInfo->rateIndexCount(); i++)
        if (mInitialStates[i] != mStates[i])
            mIntegrationRun->setOverride(iface::cellml_services::STATE_VARIABLE,
                                         i, mStates[i]);
}

//==============================================================================

/**
 * Sends a modified signal if and only if any data has been changed since the
 * last time reset() was called.
 */
void SingleCellViewSimulationData::checkForModifications()
{
    iface::cellml_services::CellMLCompiledModel*
        compModel(isDAETypeSolver() ?
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->daeCompiledModel()) :
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->odeCompiledModel()));
    ObjRef<iface::cellml_services::CodeInformation> codeInfo
        (compModel->codeInformation());
    
    bool foundChange = false;
    for (unsigned int i = 0; i < codeInfo->constantIndexCount(); i++)
        if (!qIsFinite(mInitialConstants[i]) ||
            mInitialConstants[i] != mConstants[i]) {
            foundChange = true;
            break;
        }
    if (!foundChange)
        for (unsigned int i = 0; i < codeInfo->rateIndexCount(); i++)
            if (!qIsFinite(mInitialStates[i]) ||
                mInitialStates[i] != mStates[i]) {
                foundChange = true;
                break;
            }
    if (foundChange) {
        emit modified(this, true);
        return;
    }
}

void SingleCellViewSimulationData::startMainSimulation(SingleCellViewSimulation* pSignalsTo)
{
    stopAllSimulations();
    newIntegrationRun();

    if (!mIntegrationRun)
        return;

    mState = SingleCellViewSimulationData::SIMSTATE_WAITING_RESULTS;

    iface::cellml_services::CellMLCompiledModel*
        compModel(isDAETypeSolver() ?
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->daeCompiledModel()) :
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->odeCompiledModel()));
    ObjRef<iface::cellml_services::CodeInformation> codeInfo
        (compModel->codeInformation());

    mResultReceiver = new ResultListener(mIntegrationRun, codeInfo->rateIndexCount(),
                                         codeInfo->algebraicIndexCount());
    mResultReceiver->delay(mDelay);

    mIntegrationRun->setStepSizeControl(mSolverProperties["absTol"].toDouble(),
                                        mSolverProperties["relTol"].toDouble(),
                                        1.0, // Scaling factor: states
                                        1.0, // Scaling factor: rates
                                        mSolverProperties["maxStep"].toDouble());
    mIntegrationRun->setResultRange(mStartingPoint, mEndingPoint,
                                    10000.0 // Maximum density of points in bvar, as an upper bound on the number
                                            // of points returned.
                                   );
    mIntegrationRun->setProgressObserver(mResultReceiver);

    QObject::connect(mResultReceiver, SIGNAL(solveDone()), pSignalsTo, SLOT(simulationComplete()));
    QObject::connect(mResultReceiver, SIGNAL(solveFailure(QString)), pSignalsTo, SLOT(simulationFailed(QString)));
    QObject::connect(mResultReceiver, SIGNAL(solvePointAvailable(double,QList<double>,QList<double>,QList<double>)), pSignalsTo, SLOT(simulationDataAvailable(double,QList<double>,QList<double>,QList<double>)));

    setupOverrides();
    mIntegrationRun->start();
}

//==============================================================================

SingleCellViewSimulationResults::SingleCellViewSimulationResults
(
 CellMLSupport::CellMLFileRuntime *pRuntime,
 SingleCellViewSimulation *pSimulation
) :
    mRuntime(pRuntime),
    mSimulation(pSimulation)
{
}

//==============================================================================

SingleCellViewSimulationResults::~SingleCellViewSimulationResults()
{
}

//==============================================================================

void SingleCellViewSimulationResults::reset()
{
    mPoints.clear();
    mStates.clear();
    mRates.clear();
    mAlgebraic.clear();
}

//==============================================================================

void SingleCellViewSimulationResults::addPoint
(
 const double &pPoint,
 const QList<double>& pStates,
 const QList<double>& pRates,
 const QList<double>& pAlgebraic
)
{
    // Add the data to our different arrays

    mPoints    << pPoint;
    mStates    << pStates;
    mRates     << pRates;
    mAlgebraic << pAlgebraic;
}

//==============================================================================

qulonglong SingleCellViewSimulationResults::size() const
{
    // Return our size
    return mPoints.size();
}

//==============================================================================

bool SingleCellViewSimulationResults::exportToCsv(const QString &pFileName) const
{
    // Export of all of our data to a CSV file
    QFile file(pFileName);

    bool isDAEType = mSimulation->data()->isDAETypeSolver();

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
        QSharedPointer<CellMLSupport::CellMLFileRuntimeModelParameter> modelParameter
            (mRuntime->modelParameters()[i]);

        out << "," << Header.arg(modelParameter->component(),
                                 modelParameter->name()+QString(modelParameter->degree(), '\''),
                                 modelParameter->unit());
    }

    out << "\n";

    // Data itself

    for (int j = 0; j < mPoints.size(); ++j) {
        out << mPoints[j];

        for (int i = 0, iMax = mRuntime->modelParameters().count(); i < iMax; ++i) {
            QSharedPointer<CellMLSupport::CellMLFileRuntimeModelParameter> modelParameter =
                mRuntime->modelParameters()[i];
            QSharedPointer<CellMLSupport::CellMLFileRuntimeCompiledModelParameter> compiledParameter =
                isDAEType ? modelParameter->DAEData() : modelParameter->ODEData();

            switch (compiledParameter->type()) {
            case CellMLSupport::CellMLFileRuntimeCompiledModelParameter::Constant:
            case CellMLSupport::CellMLFileRuntimeCompiledModelParameter::ComputedConstant:
                out << "," << mSimulation->data()->constants()[compiledParameter->index()];

                break;
            case CellMLSupport::CellMLFileRuntimeCompiledModelParameter::State:
                out << "," << mStates[j][compiledParameter->index()];

                break;
            case CellMLSupport::CellMLFileRuntimeCompiledModelParameter::Rate:
                out << "," << mRates[j][compiledParameter->index()];

                break;
            case CellMLSupport::CellMLFileRuntimeCompiledModelParameter::Algebraic:
                out << "," << mAlgebraic[j][compiledParameter->index()];

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
                                                   CellMLSupport::CellMLFileRuntime *pRuntime) :
    mFileName(pFileName),
    mRuntime(pRuntime),
    mData(new SingleCellViewSimulationData(pRuntime)),
    mResults(new SingleCellViewSimulationResults(pRuntime, this))
{
    // Keep track of any error occurring in our data

    connect(mData, SIGNAL(error(const QString &)),
            this, SLOT(reemitError(const QString &)));
}

//==============================================================================

SingleCellViewSimulation::~SingleCellViewSimulation()
{
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
    return (mData->state() == SingleCellViewSimulationData::SIMSTATE_WAITING_RESULTS);
}

//==============================================================================

bool SingleCellViewSimulation::isPaused() const
{
    // Return whether we are paused
    return (mData->state() == SingleCellViewSimulationData::SIMSTATE_PAUSED);
}

//==============================================================================

double SingleCellViewSimulation::progress() const
{
    double p0 = mData->startingPoint(), pn = mData->endingPoint();
    if (p0 == pn)
        return 1.0;
    double m = pn - p0;

    QList<double> p = mResults->points();
    if (p.isEmpty())
        return 0.0;

    double pLastSoFar = p.last();

    return (pLastSoFar - p0) / m;
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
    // Note #2: the 1 is for mPoints in SingleCellViewSimulationResults...

    iface::cellml_services::CellMLCompiledModel*
        compModel(mData->isDAETypeSolver() ?
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->daeCompiledModel()) :
                  static_cast<iface::cellml_services::CellMLCompiledModel*>
                  (mRuntime->odeCompiledModel()));
    ObjRef<iface::cellml_services::CodeInformation> codeInfo
        (compModel->codeInformation());

    // This is not very accurate at all, because the solver caches a lot more
    // information about the problem being solved, some of it bigger than any
    // of the below (e.g. Jacobian matricies. Given the solver dependence of
    // this size, I'm not sure this function is that useful.
    return 
        size() *
        (1 + codeInfo->constantIndexCount() + codeInfo->rateIndexCount() * 3 +
         codeInfo->algebraicIndexCount())
        * sizeof(double);
}

//==============================================================================

bool SingleCellViewSimulation::simulationSettingsOk(const bool &pEmitError)
{
    // Check and return whether our simulation settings are sound

    if (mData->startingPoint() == mData->endingPoint()) {
        if (pEmitError)
            emit error(this,
                       tr("the starting and ending points cannot have the same value"));

        return false;
    } else if (mData->pointInterval() == 0) {
        if (pEmitError)
            emit error(this,
                       tr("the point interval cannot be equal to zero"));

        return false;
    } else if (   (mData->startingPoint() < mData->endingPoint())
             && (mData->pointInterval() < 0)) {
        if (pEmitError)
            emit error(this,
                       tr("the ending point is greater than the starting point, so the point interval should be greater than zero"));

        return false;
    } else if (   (mData->startingPoint() > mData->endingPoint())
             && (mData->pointInterval() > 0)) {
        if (pEmitError)
            emit error(this,
                       tr("the ending point is smaller than the starting point, so the point interval should be smaller than zero"));

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
    // It is only valid to run a simulation is the simulation is ready, and we
    // are in the 'got initial value' state (i.e. we are ready to simulate, but
    // no simulation is already running).
    if (!simulationSettingsOk() ||
        data()->state() != SingleCellViewSimulationData::SIMSTATE_GOT_IV)
        return;

    mLastUpdate = QTime::currentTime();
    mRunTime = QTime::currentTime();

    emit running(this, true);

    data()->startMainSimulation(this);
}

//==============================================================================

void SingleCellViewSimulation::pause()
{
    data()->pause();
    emit paused(this);
    mData->checkForModifications();
}

//==============================================================================

void SingleCellViewSimulation::resume()
{
    data()->resume();
    emit running(this, true);
}

//==============================================================================

void SingleCellViewSimulation::stop()
{
    data()->stopAllSimulations();
    emit stopped(this,
                 mRunTime.elapsed());
    emit mData->updated();
}

void SingleCellViewSimulation::simulationComplete()
{
    // This catches what would otherwise be a race condition if the simulation
    // is paused before the simulation thread sends a complete.
    mData->resume();

    // Ensure graphs are up to date by informing them there might be more data.
    emit running(this, false);

    mData->state(SingleCellViewSimulationData::SIMSTATE_GOT_IV);

    emit stopped(this,
                 mRunTime.elapsed());
    emit mData->updated();
}

void SingleCellViewSimulation::simulationFailed(QString pError)
{
    // This catches what would otherwise be a race condition if the simulation
    // is paused before the simulation thread sends a failed.
    mData->resume();

    mData->state(SingleCellViewSimulationData::SIMSTATE_GOT_IV);
    emit error(this,
               "Problem solving model: " + pError);
}

void SingleCellViewSimulation::simulationDataAvailable
(
 double pPoint,
 QList<double> pStates,
 QList<double> pRates,
 QList<double> pAlgebraic
)
{
    mData->setPoint(pStates, pRates, pAlgebraic);
    mResults->addPoint(pPoint, pStates, pRates, pAlgebraic);
    // Things seem to run slowly if we send every point this way, so rate limit
    // it a bit.
    if (mLastUpdate.elapsed() > 500) {
        mLastUpdate = QTime::currentTime();
        emit running(this, false);
    }
}

//==============================================================================

void SingleCellViewSimulation::reemitError(const QString& pMsg)
{
    emit error(this, pMsg);
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

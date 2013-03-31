//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#include "cellmlfileruntime.h"
#include "coredaesolver.h"
#include "corenlasolver.h"
#include "coreodesolver.h"
#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewsimulationworker.h"
#include "thread.h"

//==============================================================================

#include <QMutex>
#include <QTime>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationWorker::SingleCellSimulationViewSimulationWorker(const SolverInterfaces &pSolverInterfaces,
                                                                                   CellMLSupport::CellmlFileRuntime *pRuntime,
                                                                                   SingleCellSimulationViewSimulation *pSimulation,
                                                                                   SingleCellSimulationViewSimulationWorker **pSelf) :
    mSolverInterfaces(pSolverInterfaces),
    mRuntime(pRuntime),
    mSimulation(pSimulation),
    mProgress(0.0),
    mPaused(false),
    mStopped(false),
    mReset(false),
    mError(false),
    mSelf(pSelf)
{
    // Create our thread

    mThread = new Core::Thread();

    // Move ourselves to our thread

    moveToThread(mThread);

    // Create a few connections

    connect(mThread, SIGNAL(started()),
            this, SLOT(started()));

    connect(this, SIGNAL(finished(const int &)),
            mThread, SLOT(quit()));

    connect(mThread, SIGNAL(finished()),
            mThread, SLOT(deleteLater()));
    connect(mThread, SIGNAL(finished()),
            this, SLOT(deleteLater()));
}

//==============================================================================

bool SingleCellSimulationViewSimulationWorker::isRunning() const
{
    // Return whether our thread is running

    return mThread?
               mThread->isRunning() && !mPaused:
               false;
}

//==============================================================================

bool SingleCellSimulationViewSimulationWorker::isPaused() const
{
    // Return whether our thread is paused

    return mThread?
               mThread->isRunning() && mPaused:
               false;
}

//==============================================================================

double SingleCellSimulationViewSimulationWorker::progress() const
{
    // Return our progress

    return mThread?
               mThread->isRunning()?mProgress:0.0:
               0.0;
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::run()
{
    // Start our thread

    if (mThread)
        mThread->start();
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::started()
{
    // Reset our progress

    mProgress = 0.0;

    // Let people know that we are running

    emit running(false);

    // Set up our ODE/DAE solver

    CoreSolver::CoreVoiSolver *voiSolver = 0;
    CoreSolver::CoreOdeSolver *odeSolver = 0;
    CoreSolver::CoreDaeSolver *daeSolver = 0;

    if (mRuntime->needOdeSolver()) {
        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare(mSimulation->data()->odeSolverName())) {
                // The requested ODE solver was found, so retrieve an instance
                // of it

                voiSolver = odeSolver = static_cast<CoreSolver::CoreOdeSolver *>(solverInterface->instance());

                break;
            }
    } else {
        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare("IDA")) {
                // The requested DAE solver was found, so retrieve an instance
                // of it

                voiSolver = daeSolver = static_cast<CoreSolver::CoreDaeSolver *>(solverInterface->instance());

                break;
            }
    }

    // Set up our NLA solver, if needed

    CoreSolver::CoreNlaSolver *nlaSolver = 0;

    if (mRuntime->needNlaSolver())
        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare(mSimulation->data()->nlaSolverName())) {
                // The requested NLA solver was found, so retrieve an instance
                // of it

                nlaSolver = static_cast<CoreSolver::CoreNlaSolver *>(solverInterface->instance());

                // Keep track of our NLA solver, so that doNonLinearSolve() can
                // work as expected

                CoreSolver::setNlaSolver(mRuntime->address(), nlaSolver);

                break;
            }

    // Keep track of any error that might be reported by any of our solvers

    mStopped = false;
    mError = false;

    if (odeSolver)
        connect(odeSolver, SIGNAL(error(const QString &)),
                this, SLOT(emitError(const QString &)));
    else
        connect(daeSolver, SIGNAL(error(const QString &)),
                this, SLOT(emitError(const QString &)));

    if (nlaSolver)
        connect(nlaSolver, SIGNAL(error(const QString &)),
                this, SLOT(emitError(const QString &)));

    // Retrieve our simulation properties

    double startingPoint = mSimulation->data()->startingPoint();
    double endingPoint   = mSimulation->data()->endingPoint();
    double pointInterval = mSimulation->data()->pointInterval();

    bool increasingPoints = endingPoint > startingPoint;
    const double oneOverPointsRange = 1.0/(endingPoint-startingPoint);
    int pointCounter = 0;
    double currentPoint = startingPoint;

    // Initialise our ODE/DAE solver

    if (odeSolver) {
        odeSolver->setProperties(mSimulation->data()->odeSolverProperties());

        odeSolver->initialize(currentPoint,
                              mRuntime->statesCount(),
                              mSimulation->data()->constants(),
                              mSimulation->data()->states(),
                              mSimulation->data()->rates(),
                              mSimulation->data()->algebraic(),
                              mRuntime->computeRates());
    } else {
        daeSolver->setProperties(mSimulation->data()->daeSolverProperties());

        daeSolver->initialize(currentPoint, endingPoint,
                              mRuntime->statesCount(),
                              mRuntime->condVarCount(),
                              mSimulation->data()->constants(),
                              mSimulation->data()->states(),
                              mSimulation->data()->rates(),
                              mSimulation->data()->algebraic(),
                              mSimulation->data()->condVar(),
                              mRuntime->computeEssentialVariables(),
                              mRuntime->computeResiduals(),
                              mRuntime->computeRootInformation(),
                              mRuntime->computeStateInformation());
    }

    // Initialise our NLA solver

    if (nlaSolver)
        nlaSolver->setProperties(mSimulation->data()->nlaSolverProperties());

    // Now, we are ready to compute our model, but only if no error has occurred
    // so far

    int elapsedTime;

    if (!mError) {
        // Start our timer

        QTime timer;

        elapsedTime = 0;

        timer.start();

        // Add our first point after making sure that all the variables have
        // been computed

        mSimulation->data()->recomputeVariables(currentPoint, false);

        mSimulation->results()->addPoint(currentPoint);

        // Our main work loop

        while ((currentPoint != endingPoint) && !mStopped && !mError) {
            // Determine our next point and compute our model up to it

            ++pointCounter;

            voiSolver->solve(currentPoint,
                             increasingPoints?
                                 qMin(endingPoint, startingPoint+pointCounter*pointInterval):
                                 qMax(endingPoint, startingPoint+pointCounter*pointInterval));

            // Update our progress

            mProgress = (currentPoint-startingPoint)*oneOverPointsRange;

            // Add our new point after making sure that all the variables have
            // been computed

            mSimulation->data()->recomputeVariables(currentPoint, false);

            mSimulation->results()->addPoint(currentPoint);

            // Check whether some or even all of our data has changed

            mSimulation->data()->checkForModifications();

            // Delay things a bit, if (really) needed

            if (mSimulation->data()->delay() && !mStopped && !mError) {
                elapsedTime += timer.elapsed();

                static_cast<Core::Thread *>(thread())->msleep(mSimulation->data()->delay());

                timer.restart();
            }

            // Check whether we should be paused

            if (mPaused) {
                // We should be paused, so stop our timer

                elapsedTime += timer.elapsed();

                // Let people know that we are paused

                emit paused();

                // Actually pause ourselves

                QMutex pausedMutex;

                pausedMutex.lock();
                    mPausedCondition.wait(&pausedMutex);
                pausedMutex.unlock();

                // We are not paused anymore

                mPaused = false;

                // Let people know that we are running again

                emit running(true);

                // Restart our timer

                timer.restart();
            }

            // Reinitialise our solver, if needed

            if (mReset) {
                if (odeSolver)
                    odeSolver->initialize(currentPoint,
                                          mRuntime->statesCount(),
                                          mSimulation->data()->constants(),
                                          mSimulation->data()->states(),
                                          mSimulation->data()->rates(),
                                          mSimulation->data()->algebraic(),
                                          mRuntime->computeRates());
                else
                    daeSolver->initialize(currentPoint, endingPoint,
                                          mRuntime->statesCount(),
                                          mRuntime->condVarCount(),
                                          mSimulation->data()->constants(),
                                          mSimulation->data()->states(),
                                          mSimulation->data()->rates(),
                                          mSimulation->data()->algebraic(),
                                          mSimulation->data()->condVar(),
                                          mRuntime->computeEssentialVariables(),
                                          mRuntime->computeResiduals(),
                                          mRuntime->computeRootInformation(),
                                          mRuntime->computeStateInformation());

                mReset = false;
            }
        }

        // Retrieve the total elapsed time, should no error have occurred

        if (mError)
            // An error occurred, so...

            elapsedTime = -1;
            // Note: we use -1 as a way to indicate that something went wrong...
        else
            elapsedTime += timer.elapsed();
    } else {
        // An error occurred, so...

        elapsedTime = -1;
        // Note: we use -1 as a way to indicate that something went wrong...
    }

    // Delete our solver(s)

    delete voiSolver;

    if (nlaSolver) {
        delete nlaSolver;

        CoreSolver::unsetNlaSolver(mRuntime->address());
    }

    // Reset our simulation owner's knowledge of us
    // Note: if we were to do it the Qt way, our simulation owner would have a
    //       slot for our finished() signal, but we want it to know as quickly
    //       as possible that we are done, so...

    *mSelf = 0;

    // Let people know that we are done and give them the elapsed time

    emit finished(elapsedTime);
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::pause()
{
    // Pause ourselves, but only if we are currently running

    if (isRunning())
        // Ask ourselves to pause

        mPaused = true;
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::resume()
{
    // Resume ourselves, but only if are currently paused

    if (isPaused())
        // Ask ourselves to resume

        mPausedCondition.wakeAll();
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::stop()
{
    // Check that we are either running or paused

    if (isRunning() || isPaused()) {
        // Resume ourselves, if needed

        if (isPaused())
            mPausedCondition.wakeAll();

        // Ask ourselves to stop

        mStopped = true;

        // Ask our thread to quit and wait for it to do so

        mThread->quit();
        mThread->wait();

        mThread = 0;
        // Note: this is in case we, for example, want to retrieve our
        //       progress...
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::reset()
{
    // Check that we are either running or paused

    if (isRunning() || isPaused())
        // Ask ourselves to reinitialise our solver

        mReset = true;
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::emitError(const QString &pMessage)
{
    // A solver error occurred, so keep track of it and let people know about it

    mError = true;

    emit error(pMessage);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

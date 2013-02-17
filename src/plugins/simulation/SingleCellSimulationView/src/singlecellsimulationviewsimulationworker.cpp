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

#include <QTime>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationWorker::SingleCellSimulationViewSimulationWorker(const SolverInterfaces &pSolverInterfaces,
                                                                                   CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                                                   SingleCellSimulationViewSimulation *pSimulation) :
    mSolverInterfaces(pSolverInterfaces),
    mCellmlFileRuntime(pCellmlFileRuntime),
    mSimulation(pSimulation),
    mError(false)
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

void SingleCellSimulationViewSimulationWorker::run()
{
    // Start our thread

    mThread->start();
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::started()
{
    // Start ourselves, but only if we are currently idling

    if (mSimulation->mWorkerStatus == SingleCellSimulationViewSimulation::Idling) {
        // We are now running

        mSimulation->mWorkerStatus = SingleCellSimulationViewSimulation::Running;

        // Let people know that we are running

        emit running();

        // Set up our ODE/DAE solver

        CoreSolver::CoreVoiSolver *voiSolver = 0;
        CoreSolver::CoreOdeSolver *odeSolver = 0;
        CoreSolver::CoreDaeSolver *daeSolver = 0;

        if (mCellmlFileRuntime->needOdeSolver()) {
            foreach (SolverInterface *solverInterface, mSolverInterfaces)
                if (!solverInterface->name().compare(mSimulation->data()->odeSolverName())) {
                    // The requested ODE solver was found, so retrieve an
                    // instance of it

                    voiSolver = odeSolver = static_cast<CoreSolver::CoreOdeSolver *>(solverInterface->instance());

                    break;
                }
        } else {
            foreach (SolverInterface *solverInterface, mSolverInterfaces)
                if (!solverInterface->name().compare("IDA")) {
                    // The requested DAE solver was found, so retrieve an
                    // instance of it

                    voiSolver = daeSolver = static_cast<CoreSolver::CoreDaeSolver *>(solverInterface->instance());

                    break;
                }
        }

        // Set up our NLA solver, if needed

        CoreSolver::CoreNlaSolver *nlaSolver = 0;

        if (mCellmlFileRuntime->needNlaSolver())
            foreach (SolverInterface *solverInterface, mSolverInterfaces)
                if (!solverInterface->name().compare(mSimulation->data()->nlaSolverName())) {
                    // The requested NLA solver was found, so retrieve an
                    // instance of it

                    nlaSolver = static_cast<CoreSolver::CoreNlaSolver *>(solverInterface->instance());

                    // Keep track of our NLA solver, so that doNonLinearSolve()
                    // can work as expected

                    CoreSolver::setNlaSolver(mCellmlFileRuntime->address(),
                                             nlaSolver);

                    break;
                }

        // Keep track of any error that might be reported by any of our solvers

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
                                  mCellmlFileRuntime->statesCount(),
                                  mSimulation->data()->constants(),
                                  mSimulation->data()->states(),
                                  mSimulation->data()->rates(),
                                  mSimulation->data()->algebraic(),
                                  mCellmlFileRuntime->computeRates());
        } else {
            daeSolver->setProperties(mSimulation->data()->daeSolverProperties());

            daeSolver->initialize(currentPoint, endingPoint,
                                  mCellmlFileRuntime->statesCount(),
                                  mCellmlFileRuntime->condVarCount(),
                                  mSimulation->data()->constants(),
                                  mSimulation->data()->states(),
                                  mSimulation->data()->rates(),
                                  mSimulation->data()->algebraic(),
                                  mSimulation->data()->condVar(),
                                  mCellmlFileRuntime->computeEssentialVariables(),
                                  mCellmlFileRuntime->computeResiduals(),
                                  mCellmlFileRuntime->computeRootInformation(),
                                  mCellmlFileRuntime->computeStateInformation());
        }

        // Initialise our NLA solver

        if (nlaSolver)
            nlaSolver->setProperties(mSimulation->data()->nlaSolverProperties());

        // Now, we are ready to compute our model, but only if no error has
        // occurred so far

        int elapsedTime;

        if (!mError) {
            // Signals timer

            QTime signalsTimer;
            bool canEmitSignals;

            // Start our timer

            QTime timer;

            elapsedTime = 0;

            timer.start();

            // Our main work loop

            while (   (currentPoint != endingPoint) && !mError
                   && (mSimulation->mWorkerStatus != SingleCellSimulationViewSimulation::Stopped)) {
                // Check whether we came here more than 10 ms ago (i.e. 100 Hz)
                // and, if so, then allow for signals to be emitted
                // Note: if a simulation is quick to run, then many signals
                //       would get emitted and their handling could make OpenCOR
                //       unresponsive, so by waiting for 10 ms (i.e. 100 Hz
                //       which is more than what human eyes can handle) we
                //       reduce the number of signals being emitted while still
                //       allowing for their smooth handling...

                if (currentPoint == startingPoint) {
                    // This our first point, so start our signals timer and
                    // allow signals to be emitted

                    signalsTimer.start();

                    canEmitSignals = true;
                } else if (signalsTimer.elapsed() < 10) {
                    // This is not our first point and it has been less than 10
                    // ms since we were last here, so prevent signals from being
                    // emitted

                    canEmitSignals = false;
                } else {
                    // This is not our first point and it has been more than 10
                    // ms since we were last here, so retart our timer and allow
                    // signals to be emitted

                    signalsTimer.restart();

                    canEmitSignals = true;
                }

                // Update our progress

                mSimulation->mWorkerProgress = (currentPoint-startingPoint)*oneOverPointsRange;

                // Add our new point after making sure that all the variables
                // have been computed

                mSimulation->data()->recomputeVariables(currentPoint, canEmitSignals);

                mSimulation->results()->addPoint(currentPoint, canEmitSignals);

                // Check whether we should be pausing

                if(mSimulation->mWorkerStatus == SingleCellSimulationViewSimulation::Pausing) {
                    // We have been asked to pause, so do just that after
                    // stopping our timer

                    elapsedTime += timer.elapsed();

                    mStatusMutex.lock();
                        mStatusCondition.wait(&mStatusMutex);
                    mStatusMutex.unlock();

                    // We are now running again

                    mSimulation->mWorkerStatus = SingleCellSimulationViewSimulation::Running;

                    // Let people know that we are running again

                    emit running();

                    // Restart our timer

                    timer.restart();
                }

                // Determine our next point and compute our model up to it

                ++pointCounter;

                voiSolver->solve(currentPoint,
                                 increasingPoints?
                                     qMin(endingPoint, startingPoint+pointCounter*pointInterval):
                                     qMax(endingPoint, startingPoint+pointCounter*pointInterval));

                // Delay things a bit, if (really) needed

                if (   mSimulation->data()->delay()
                    && (mSimulation->mWorkerStatus != SingleCellSimulationViewSimulation::Stopped)) {
                    elapsedTime += timer.elapsed();

                    static_cast<Core::Thread *>(thread())->msleep(mSimulation->data()->delay());

                    timer.restart();
                }
            }

            // Update our progress and add our last point, but only if we didn't
            // stop the simulation

            if (   !mError
                && (mSimulation->mWorkerStatus != SingleCellSimulationViewSimulation::Stopped)) {
                // Update our progress

                mSimulation->mWorkerProgress = 1.0;

                // Add our last point after making sure that all the variables
                // have been computed

                mSimulation->data()->recomputeVariables(currentPoint);

                mSimulation->results()->addPoint(currentPoint);
            }

            // Retrieve the total elapsed time, should no error have occurred

            if (mError)
                // An error occurred, so...

                elapsedTime = -1;
                // Note: we use -1 as a way to indicate that something went
                //       wrong...
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

            CoreSolver::unsetNlaSolver(mCellmlFileRuntime->address());
        }

        // We are done, so...

        mSimulation->mWorkerStatus = SingleCellSimulationViewSimulation::Finished;

        // Let people know that we are done and give them the elapsed time

        emit finished(elapsedTime);
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::pause()
{
    // Pause ourselves, but only if we are currently running

    if (mSimulation->mWorkerStatus == SingleCellSimulationViewSimulation::Running) {
        // We are now pausing

        mSimulation->mWorkerStatus = SingleCellSimulationViewSimulation::Pausing;

        // Let people know that we are pausing

        emit pausing();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::resume()
{
    // Resume ourselves, but only if are currently pausing

    if (mSimulation->mWorkerStatus == SingleCellSimulationViewSimulation::Pausing) {
        // Actually resume ourselves

        mStatusCondition.wakeAll();

        // We are now running again

        mSimulation->mWorkerStatus = SingleCellSimulationViewSimulation::Running;

        // Let people know that we are running again

        emit running();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::stop()
{
    // Check that we are either running or pausing

    if (   (mSimulation->mWorkerStatus == SingleCellSimulationViewSimulation::Running)
        || (mSimulation->mWorkerStatus == SingleCellSimulationViewSimulation::Pausing)) {
        // Resume ourselves, if needed

        if (mSimulation->mWorkerStatus == SingleCellSimulationViewSimulation::Pausing)
            mStatusCondition.wakeAll();

        // Stop ourselves

        mSimulation->mWorkerStatus = SingleCellSimulationViewSimulation::Stopped;
    }
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

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
                                                                                   SingleCellSimulationViewSimulationData *pData,
                                                                                   SingleCellSimulationViewSimulationResults *pResults) :
    mStatus(Idling),
    mSolverInterfaces(pSolverInterfaces),
    mCellmlFileRuntime(pCellmlFileRuntime),
    mData(pData),
    mResults(pResults),
    mError(false)
{
    // Initialise our progress and let people know about it

    updateAndEmitProgress(0.0);
}

//==============================================================================

SingleCellSimulationViewSimulationWorker::Status SingleCellSimulationViewSimulationWorker::status() const
{
    // Return our status

    return mStatus;
}

//==============================================================================

double SingleCellSimulationViewSimulationWorker::progress() const
{
    // Return our progress

    return mProgress;
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::updateAndEmitProgress(const double &pProgress)
{
    // Set our progress

    mProgress = pProgress;

    // Let people know about our progress

    emit progress(pProgress);
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::run()
{
    // Run ourselves, but only if we are currently idling

    if (mStatus == Idling) {
        // We are running

        mStatus = Running;

        // Let people know that we are running

        emit running();

        // Set up our ODE/DAE solver

        CoreSolver::CoreVoiSolver *voiSolver = 0;
        CoreSolver::CoreOdeSolver *odeSolver = 0;
        CoreSolver::CoreDaeSolver *daeSolver = 0;

        if (mCellmlFileRuntime->needOdeSolver()) {
            foreach (SolverInterface *solverInterface, mSolverInterfaces)
                if (!solverInterface->name().compare(mData->odeSolverName())) {
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
                if (!solverInterface->name().compare(mData->nlaSolverName())) {
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

        double startingPoint = mData->startingPoint();
        double endingPoint   = mData->endingPoint();
        double pointInterval = mData->pointInterval();

        bool increasingPoints = endingPoint > startingPoint;
        const double oneOverPointRange = 1.0/(endingPoint-startingPoint);
        int pointCounter = 0;
        double currentPoint = startingPoint;

        // Initialise our ODE/DAE solver

        if (odeSolver) {
            odeSolver->setProperties(mData->odeSolverProperties());

            odeSolver->initialize(currentPoint,
                                  mCellmlFileRuntime->statesCount(),
                                  mData->constants(), mData->states(),
                                  mData->rates(), mData->algebraic(),
                                  mCellmlFileRuntime->computeRates());
        } else {
            daeSolver->setProperties(mData->daeSolverProperties());

            daeSolver->initialize(currentPoint, endingPoint,
                                  mCellmlFileRuntime->statesCount(),
                                  mCellmlFileRuntime->condVarCount(),
                                  mData->constants(), mData->states(),
                                  mData->rates(), mData->algebraic(),
                                  mData->condVar(),
                                  mCellmlFileRuntime->computeEssentialVariables(),
                                  mCellmlFileRuntime->computeResiduals(),
                                  mCellmlFileRuntime->computeRootInformation(),
                                  mCellmlFileRuntime->computeStateInformation());
        }

        // Initialise our NLA solver

        if (nlaSolver)
            nlaSolver->setProperties(mData->nlaSolverProperties());

        // Now, we are ready to compute our model, but only if no error has
        // occurred so far

        int elapsedTime;

        if (!mError) {
            // Start our timer

            QTime timer;

            elapsedTime = 0;

            timer.start();

            // Our main work loop

            while (   (currentPoint != endingPoint) && !mError
                   && (mStatus != Stopped)) {
                // Handle our current point after making sure that all the
                // variables have been computed

                mData->recomputeVariables(currentPoint);

                mResults->addPoint(currentPoint);

                // Let people know about our progress

                updateAndEmitProgress((currentPoint-startingPoint)*oneOverPointRange);

                // Check whether we should be pausing

                if(mStatus == Pausing) {
                    // We have been asked to pause, so do just that after
                    // stopping our timer

                    elapsedTime += timer.elapsed();

                    mStatusMutex.lock();
                        mStatusCondition.wait(&mStatusMutex);
                    mStatusMutex.unlock();

                    // We are running again

                    mStatus = Running;

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

                if (mData->delay() && (mStatus != Stopped)) {
                    elapsedTime += timer.elapsed();

                    static_cast<Core::Thread *>(thread())->msleep(mData->delay());

                    timer.restart();
                }
            }

            // Handle our last point and let people know about our final
            // progress, but only if we didn't stop the simulation

            if (!mError && (mStatus != Stopped)) {
                // Handle our last point after making sure that all the
                // variables have been computed

                mData->recomputeVariables(currentPoint);

                mResults->addPoint(currentPoint);

                // Let people know about our final progress

                updateAndEmitProgress(1.0);
            }

            // Reset our progress
            // Note: we would normally use updateAndEmitProgress(), but we don't
            //       want to emit the progress, so...

            mProgress = 0.0;

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

        mStatus = Finished;

        // Let people know that we are done and give them the elapsed time

        emit finished(elapsedTime);
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::pause()
{
    // Pause ourselves, but only if we are currently running

    if (mStatus == Running) {
        // We are pausing

        mStatus = Pausing;

        // Let people know that we are pausing

        emit pausing();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::resume()
{
    // Resume ourselves, but only if are currently pausing

    if (mStatus == Pausing) {
        // Actually resume ourselves

        mStatusCondition.wakeAll();

        // We are running again

        mStatus = Running;

        // Let people know that we are running again

        emit running();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::stop()
{
    // Check that we are either running or pausing

    if ((mStatus == Running) || (mStatus == Pausing)) {
        // Resume ourselves, if needed

        if (mStatus == Pausing)
            mStatusCondition.wakeAll();

        // Stop ourselves

        mStatus = Stopped;
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

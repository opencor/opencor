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
// Simulation worker
//==============================================================================

#include "cellmlfileruntime.h"
#include "corecliutils.h"
#include "simulation.h"
#include "simulationworker.h"

//==============================================================================

#include <QElapsedTimer>
#include <QMutex>
#include <QThread>

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

SimulationWorker::SimulationWorker(Simulation *pSimulation, QThread *pThread,
                                   SimulationWorker *&pSelf) :
    mSimulation(pSimulation),
    mThread(pThread),
    mRuntime(pSimulation->runtime()),
    mSelf(pSelf)
{
}

//==============================================================================

bool SimulationWorker::isRunning() const
{
    // Return whether our thread is running

    return mThread->isRunning() && !mPaused;
}

//==============================================================================

bool SimulationWorker::isPaused() const
{
    // Return whether our thread is paused

    return mThread->isRunning() && mPaused;
}

//==============================================================================

double SimulationWorker::currentPoint() const
{
    // Return our current point

    return mThread->isRunning()?
               mCurrentPoint:
               mSimulation->data()->startingPoint();
}

//==============================================================================

void SimulationWorker::run()
{
    // Let people know that we are running

    emit running(false);

    // Set up our ODE solver

    auto odeSolver = static_cast<Solver::OdeSolver *>(mSimulation->data()->odeSolverInterface()->solverInstance());

    // Set up our NLA solver, if needed
    // Note: we unset it at the end of this method...

    Solver::NlaSolver *nlaSolver = nullptr;

    if (mRuntime->needNlaSolver()) {
        nlaSolver = static_cast<Solver::NlaSolver *>(mSimulation->data()->nlaSolverInterface()->solverInstance());

        Solver::setNlaSolver(mRuntime->address(), nlaSolver);
    }

    // Keep track of any error that might be reported by any of our solvers

    mStopped = false;
    mError = false;

    connect(odeSolver, &Solver::OdeSolver::error,
            this, &SimulationWorker::emitError);

    if (nlaSolver != nullptr) {
        connect(nlaSolver, &Solver::NlaSolver::error,
                this, &SimulationWorker::emitError);
    }

    // Retrieve our simulation properties

    double startingPoint = mSimulation->data()->startingPoint();
    double endingPoint = mSimulation->data()->endingPoint();
    double pointInterval = mSimulation->data()->pointInterval();
    quint64 pointCounter = 0;

    mCurrentPoint = startingPoint;

    // Initialise our ODE solver

    odeSolver->setProperties(mSimulation->data()->odeSolverProperties());

    odeSolver->initialize(mCurrentPoint, mRuntime->statesCount(),
                          mSimulation->data()->constants(),
                          mSimulation->data()->rates(),
                          mSimulation->data()->states(),
                          mSimulation->data()->algebraic(),
                          mRuntime->computeRates());

    // Initialise our NLA solver, if any

    if (nlaSolver != nullptr) {
        nlaSolver->setProperties(mSimulation->data()->nlaSolverProperties());
    }

    // Now, we are ready to compute our model, but only if no error has occurred
    // so far
    // Note: we use -1 as a way to indicate that something went wrong...

    qint64 elapsedTime = 0;

    if (!mError) {
        // Start our timer

        QElapsedTimer timer;

        timer.start();

        // Add our first point

        mSimulation->results()->addPoint(mCurrentPoint);

        // Our main work loop
        // Note: for performance reasons, it is essential that the following
        //       loop doesn't emit any signal, be it directly or indirectly,
        //       unless it is to let people know that we are pausing or running.
        //       Indeed, the signal/slot mechanism adds a certain level of
        //       overhead and, here, we want things to be as fast as possible...

        QMutex pausedMutex;

        forever {
            // Reinitialise our solver, if we have an NLA solver or if the model
            // got reset
            // Note: indeed, with a solver such as CVODE, we need to update our
            //       internals...

            if ((nlaSolver != nullptr) || mReset) {
                odeSolver->reinitialize(mCurrentPoint);

                mReset = false;
            }

            // Determine our next point and compute our model up to it

            ++pointCounter;

            odeSolver->solve(mCurrentPoint,
                             qMin(endingPoint,
                                  startingPoint+pointCounter*pointInterval));

            // Make sure that no error occurred

            if (mError) {
                break;
            }

            // Add our new point

            mSimulation->results()->addPoint(mCurrentPoint);

            // Some post-processing, if needed

            if (qFuzzyCompare(mCurrentPoint, endingPoint) || mStopped) {
                // We have reached our ending point or we have been asked to
                // stop, so leave our main work loop

                break;
            }

            // Delay things a bit, if needed

            if (mSimulation->delay() != nullptr) {
                Core::doNothing(mSimulation->delay(), &mStopped);
            }

            // Pause ourselves, if needed

            if (mPaused) {
                // We should be paused, so stop our timer

                elapsedTime += timer.elapsed();

                // Let people know that we are paused

                emit paused();

                // Actually pause ourselves

                pausedMutex.lock();
                    mPausedCondition.wait(&pausedMutex);
                pausedMutex.unlock();

                // We are not paused anymore

                mPaused = false;

                // Let people know that we are running again

                emit running(true);

                // (Re)start our timer

                timer.start();

            }
        }

        // Retrieve the total elapsed time, should no error have occurred

        if (!mError) {
            elapsedTime += timer.elapsed();
        }
    }

    // Delete our solver(s)

    delete odeSolver;

    if (nlaSolver != nullptr) {
        delete nlaSolver;

        Solver::unsetNlaSolver(mRuntime->address());
    }

    // Reset our simulation owner's knowledge of us
    // Note: if we were to do it the Qt way, our simulation owner would have a
    //       slot for our done() signal, but we want our simulation owner to
    //       know as quickly as possible that we are done...

    mSelf = nullptr;

    // Let people know that we are done and give them the elapsed time

    emit done(mError?-1:elapsedTime);
}

//==============================================================================

void SimulationWorker::pause()
{
    // Pause ourselves, if we are currently running

    if (isRunning()) {
        mPaused = true;
    }
}

//==============================================================================

void SimulationWorker::resume()
{
    // Resume ourselves, if we are currently paused

    if (isPaused()) {
        mPausedCondition.wakeOne();
    }
}

//==============================================================================

void SimulationWorker::stop()
{
    // Stop ourselves, if we are currently running or paused

    if (isRunning() || isPaused()) {
        mStopped = true;

        if (isPaused()) {
            mPausedCondition.wakeOne();
        }
    }
}

//==============================================================================

void SimulationWorker::reset()
{
    // Stop ourselves, if we are currently running or paused

    if (isRunning() || isPaused()) {
        mReset = true;
    }
}

//==============================================================================

void SimulationWorker::emitError(const QString &pMessage)
{
    // A solver error occurred, so keep track of it and let people know about
    // it, but only if another error hasn't already been received

    if (!mError) {
        mError = true;

        emit error(pMessage);
    }
}

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

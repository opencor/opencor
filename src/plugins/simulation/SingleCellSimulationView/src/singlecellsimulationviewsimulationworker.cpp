//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewsimulationworker.h"
#include "thread.h"

//==============================================================================

#include <QTime>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationWorker::SingleCellSimulationViewSimulationWorker(SingleCellSimulationViewSimulationData *pData) :
    mStatus(Idling),
    mData(pData)
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
    // Check our status

    if (mStatus == Idling) {
        // We are currently idling which means that we can be run

        mStatus = Running;

        // Let people know that we are running

        emit running();

        // Start our timer

        QTime timer;
        int totalElapsedTime = 0;

        timer.start();

        // Our main work loop

        double startingPoint = mData->startingPoint();
        double endingPoint   = mData->endingPoint();
        double pointInterval = mData->pointInterval();

        bool increasingPoints = endingPoint > startingPoint;
        const double oneOverPointRange = 1.0/(endingPoint-startingPoint);
        int voiCounter = 0;
        double currentPoint = startingPoint;

        while ((currentPoint != endingPoint) && (mStatus != Stopped)) {
            // Handle our current point

//---GRY--- TO BE DONE...

            // Let people know about our progress

            updateAndEmitProgress((currentPoint-startingPoint)*oneOverPointRange);

            // Check whether we should be pausing

            if(mStatus == Pausing) {
                // We have been asked to pause, so do just that after stopping
                // our timer

                totalElapsedTime += timer.elapsed();

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

            // Go to the next point, if needed

            ++voiCounter;

            currentPoint = increasingPoints?
                               qMin(endingPoint, startingPoint+voiCounter*pointInterval):
                               qMax(endingPoint, startingPoint+voiCounter*pointInterval);

            // Delay things a bit, if needed
            // Note: unlike for the starting/ending points and point interval,
            //       we always retrieve the delay from our data structure since
            //       it can be changed ay any time (through the GUI) unlike
            //       those other properties...

            if (mData->delay()) {
                totalElapsedTime += timer.elapsed();

                static_cast<Core::Thread *>(thread())->msleep(mData->delay());

                timer.restart();
            }
        }

        // Handle our last point

//---GRY--- TO BE DONE...

        // Let people know about our final progress, but only if we didn't stop
        // the simulation

        if (mStatus != Stopped)
            updateAndEmitProgress(1.0);

        // We are done, so...

        mStatus = Finished;

        // Reset our progress
        // Note: we would normally use updateAndEmitProgress(), but we don't
        //       want to emit the progress, so...

        mProgress = 0.0;

        // Retrieve the total elapsed time

        totalElapsedTime += timer.elapsed();

        // Let people know that we are done and give them the total elapsed time too

        emit finished(totalElapsedTime);
    } else if (mStatus == Pausing) {
        // We are currently pausing, so resume ourselves

        mStatusCondition.wakeAll();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::pause()
{
    // Pause ourselves, but only if we are currently running

    if (mStatus == Running) {
        // Pause ourselves

        mStatus = Pausing;

        // Let people know that we are pausing

        emit pausing();
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

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

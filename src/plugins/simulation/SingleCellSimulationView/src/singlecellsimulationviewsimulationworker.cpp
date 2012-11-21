//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#include "singlecellsimulationviewsimulationworker.h"

//==============================================================================

#include <QTime>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationWorker::SingleCellSimulationViewSimulationWorker(const double &pStartingPoint,
                                                                                   const double &pEndingPoint,
                                                                                   const double &pPointInterval) :
    mStatus(Idling),
    mStartingPoint(pStartingPoint),
    mEndingPoint(pEndingPoint),
    mPointInterval(pPointInterval)
{
    // Initialise our progress
    // Note: we use setProgress() since it will let people know about our
    //       progress...

    setProgress(0.0);
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

void SingleCellSimulationViewSimulationWorker::setProgress(const double &pProgress)
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

        bool increasingPoints = mEndingPoint > mStartingPoint;
        const double oneOverPointRange = 1.0/(mEndingPoint-mStartingPoint);
        int voiCounter = 0;
        double currentPoint = mStartingPoint;

        while ((currentPoint != mEndingPoint) && (mStatus != Stopped)) {
            // Handle our current point

//---GRY--- TO BE DONE...

            // Let people know about our progress

            setProgress((currentPoint-mStartingPoint)*oneOverPointRange);

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
                               qMin(mEndingPoint, mStartingPoint+voiCounter*mPointInterval):
                               qMax(mEndingPoint, mStartingPoint+voiCounter*mPointInterval);
        }

        // Handle our last point

//---GRY--- TO BE DONE...

        // Let people know about our final progress

        setProgress(1.0);

        // Retrieve the total elapsed time

        totalElapsedTime += timer.elapsed();

        // We are done, so...

        mStatus = Finished;

        // Reset our progress

        setProgress(0.0);

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

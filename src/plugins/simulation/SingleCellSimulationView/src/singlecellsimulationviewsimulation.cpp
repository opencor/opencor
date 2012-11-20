//==============================================================================
// Single cell simulation view simulation
//==============================================================================

#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewsimulationinformationwidget.h"
#include "singlecellsimulationviewsimulationworker.h"

//==============================================================================

#include <QThread>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulation::SingleCellSimulationViewSimulation() :
    mWorkerThread(0),
    mWorker(0),
    mStartingPoint(0.0),
    mEndingPoint(1000.0),
    mPointInterval(1.0)
{
}

//==============================================================================

SingleCellSimulationViewSimulation::~SingleCellSimulationViewSimulation()
{
    // Stop our worker (just in case...)

    stop();
}

//==============================================================================

void SingleCellSimulationViewSimulation::updateFromGui(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings)
{
    // Update our settings from our simulation information widget

    mStartingPoint = pSimulationSettings->startingPoint();
    mEndingPoint   = pSimulationSettings->endingPoint();
    mPointInterval = pSimulationSettings->pointInterval();
}

//==============================================================================

void SingleCellSimulationViewSimulation::updateGui(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings)
{
    // Update our simulation information widget using our settings

    pSimulationSettings->setStartingPoint(mStartingPoint);
    pSimulationSettings->setEndingPoint(mEndingPoint);
    pSimulationSettings->setPointInterval(mPointInterval);
}

//==============================================================================

void SingleCellSimulationViewSimulation::run()
{
    // Initialize our worker, if necessary

    if (!mWorkerThread && !mWorker) {
        // First, check that our simulation settings we were given are sound

        bool simulationSettingsOk = false;

        if (mStartingPoint == mEndingPoint)
            emit error(tr("the starting and ending points cannot have the same value"));
        else if (mPointInterval == 0)
            emit error(tr("the point interval cannot be equal to zero"));
        else if ((mStartingPoint < mEndingPoint) && (mPointInterval < 0))
            emit error(tr("the ending point is greater than the starting point, so the point interval should be greater than zero"));
        else if ((mStartingPoint > mEndingPoint) && (mPointInterval > 0))
            emit error(tr("the ending point is smaller than the starting point, so the point interval should be smaller than zero"));
        else
            simulationSettingsOk = true;

        if (!simulationSettingsOk)
            // Something wrong with our simulation settings, so...

            return;

        // Create our worker and the thread in which it will work

        mWorkerThread = new QThread();
        mWorker       = new SingleCellSimulationViewSimulationWorker(mStartingPoint, mEndingPoint, mPointInterval);

        // Check that the worker and its thread have been properly created

        if (!mWorkerThread || !mWorker) {
            delete mWorkerThread;
            delete mWorker;

            mWorkerThread = 0;
            mWorker       = 0;

            emit error(tr("the simulation worker and/or its thread could not be initialised"));

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

        connect(mWorker, SIGNAL(progress(const double &)),
                this, SIGNAL(progress(const double &)));

        connect(mWorker, SIGNAL(finished(const int &)),
                this, SLOT(finished(const int &)));

        connect(mWorker, SIGNAL(finished(const int &)),
                mWorkerThread, SLOT(quit()));
        connect(mWorker, SIGNAL(finished(const int &)),
                mWorker, SLOT(deleteLater()));

        connect(mWorkerThread, SIGNAL(finished()),
                mWorkerThread, SLOT(deleteLater()));

        // Start our worker thread

        mWorkerThread->start();
    } else {
        // Our worker (incl. its thread) has already been initialized, so just
        // run it
        // Note: it might have been paused in between in which case it will
        //       automatically resume itself...

        mWorker->run();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulation::pause()
{
    // Ask our worker to pause, but only if it exists

    if (mWorkerThread && mWorker)
        mWorker->pause();
}

//==============================================================================

void SingleCellSimulationViewSimulation::stop()
{
    // Ask our worker to stop, but only if it exists

    if (mWorkerThread && mWorker)
        mWorker->stop();
}

//==============================================================================

void SingleCellSimulationViewSimulation::finished(const int &pElapsedTime)
{
    // Our worker is done (and it will get deleted and everything), so...

    mWorkerThread = 0;
    mWorker       = 0;

    // Let people know that we have stopped

    emit stopped(pElapsedTime);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

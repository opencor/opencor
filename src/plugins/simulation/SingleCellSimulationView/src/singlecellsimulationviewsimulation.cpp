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

void SingleCellSimulationViewSimulation::fromGui(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings)
{
    // Load our settings from our simulation information widget

    mStartingPoint = pSimulationSettings->startingPoint();
    mEndingPoint   = pSimulationSettings->endingPoint();
    mPointInterval = pSimulationSettings->pointInterval();
}

//==============================================================================

void SingleCellSimulationViewSimulation::toGui(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings)
{
    // Save our settings to our simulation information widget

    pSimulationSettings->setStartingPoint(mStartingPoint);
    pSimulationSettings->setEndingPoint(mEndingPoint);
    pSimulationSettings->setPointInterval(mPointInterval);
}

//==============================================================================

void SingleCellSimulationViewSimulation::run()
{
    // Initialize our worker, if necessary

    if (!mWorkerThread && !mWorker) {
        // Create our worker and the thread in which it will work

        mWorkerThread = new QThread();
        mWorker       = new SingleCellSimulationViewSimulationWorker();

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

        connect(mWorker, SIGNAL(finished()),
                this, SLOT(finished()));

        connect(mWorker, SIGNAL(finished()),
                mWorkerThread, SLOT(quit()));
        connect(mWorker, SIGNAL(finished()),
                mWorker, SLOT(deleteLater()));

        connect(mWorkerThread, SIGNAL(finished()),
                mWorkerThread, SLOT(deleteLater()));
    }

    // Start our worker thread or resume our worker, depending on the case

    if (mWorker->isPausing())
        // Our worker is pausing, so resume it

        mWorker->resume();
    else
        // Our worker is not paused, meaning that ou worker thread needs to be
        // started

        mWorkerThread->start();
}

//==============================================================================

void SingleCellSimulationViewSimulation::pause()
{
    // Check that our worker exists

    if (!mWorkerThread && !mWorker)
        return;

    // Ask our worker to pause

    mWorker->pause();
}

//==============================================================================

void SingleCellSimulationViewSimulation::stop()
{
    // Check that our worker exists

    if (!mWorkerThread && !mWorker)
        return;

    // Ask our worker to stop

    mWorker->stop();
}

//==============================================================================

void SingleCellSimulationViewSimulation::finished()
{
    // Our worker is done (and it will get deleted and everything), so...

    mWorkerThread = 0;
    mWorker       = 0;

    // Let people know that we have stopped

    emit stopped();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

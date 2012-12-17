//==============================================================================
// Single cell simulation view simulation
//==============================================================================

#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewwidget.h"
#include "thread.h"

//==============================================================================

#include "qwt_slider.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulation::SingleCellSimulationViewSimulation(const QString &pFileName) :
    mWorkerThread(0),
    mWorker(0),
    mFileName(pFileName),
    mData(SingleCellSimulationViewSimulationData())
{
}

//==============================================================================

SingleCellSimulationViewSimulation::~SingleCellSimulationViewSimulation()
{
    // Stop our worker (just in case...)

    stop();
}

//==============================================================================

QString SingleCellSimulationViewSimulation::fileName() const
{
    // Retrieve and return our file name

    return mFileName;
}

//==============================================================================

SingleCellSimulationViewSimulationWorker::Status SingleCellSimulationViewSimulation::workerStatus() const
{
    // Return the status of our worker, if active

    return (mWorkerThread && mWorker)?
               mWorker->status():
               SingleCellSimulationViewSimulationWorker::Unknown;
}

//==============================================================================

double SingleCellSimulationViewSimulation::workerProgress() const
{
    // Return the progress of our worker, if active

    return (mWorkerThread && mWorker)?
               mWorker->progress():
               0.0;
}

//==============================================================================

void SingleCellSimulationViewSimulation::setData(SingleCellSimulationViewWidget *pGui)
{
    // Set our data using information from the GUI

    // Set our delay

    setDelay(pGui->delayValue());

    // Update some of our data from our GUI's simulation widget

    SingleCellSimulationViewInformationSimulationWidget *simulationWidget = pGui->contentsWidget()->informationWidget()->simulationWidget();

    mData.startingPoint = simulationWidget->startingPoint();
    mData.endingPoint   = simulationWidget->endingPoint();
    mData.pointInterval = simulationWidget->pointInterval();
}

//==============================================================================

void SingleCellSimulationViewSimulation::setDelay(const int &pDelay)
{
    // Set our delay

    mData.delay = pDelay;
}

//==============================================================================

void SingleCellSimulationViewSimulation::run()
{
    // Initialise our worker, if not active

    if (!mWorkerThread && !mWorker) {
        // First, check that our simulation settings we were given are sound

        bool simulationSettingsOk = false;

        if (mData.startingPoint == mData.endingPoint)
            emit error(tr("the starting and ending points cannot have the same value"));
        else if (mData.pointInterval == 0)
            emit error(tr("the point interval cannot be equal to zero"));
        else if (   (mData.startingPoint < mData.endingPoint)
                 && (mData.pointInterval < 0))
            emit error(tr("the ending point is greater than the starting point, so the point interval should be greater than zero"));
        else if (   (mData.startingPoint > mData.endingPoint)
                 && (mData.pointInterval > 0))
            emit error(tr("the ending point is smaller than the starting point, so the point interval should be smaller than zero"));
        else
            simulationSettingsOk = true;

        if (!simulationSettingsOk)
            // Something wrong with our simulation settings, so...

            return;

        // Create our worker and the thread in which it will work

        mWorkerThread = new Core::Thread();
        mWorker       = new SingleCellSimulationViewSimulationWorker(&mData);

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
        // Our worker is already active, so just run it
        // Note: it might have been paused in between, in which case it will
        //       automatically resume itself...

        mWorker->run();
    }
}

//==============================================================================

void SingleCellSimulationViewSimulation::pause()
{
    // Ask our worker to pause, if active

    if (mWorkerThread && mWorker)
        mWorker->pause();
}

//==============================================================================

void SingleCellSimulationViewSimulation::stop()
{
    // Ask our worker to stop, if active

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

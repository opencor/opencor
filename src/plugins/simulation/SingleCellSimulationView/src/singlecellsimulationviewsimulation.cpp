//==============================================================================
// Single cell simulation view simulation
//==============================================================================

#include "cellmlfilemanager.h"
#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewsimulationinformationwidget.h"

//==============================================================================

#include <QThread>

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
    mDelay(0),
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

void SingleCellSimulationViewSimulation::updateFromGui(QwtSlider *pSlider,
                                                       SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings)
{
    // Update our delay from our slider

    mDelay = pSlider->value();

    // Update our settings from our simulation information widget

    mStartingPoint = pSimulationSettings->startingPoint();
    mEndingPoint   = pSimulationSettings->endingPoint();
    mPointInterval = pSimulationSettings->pointInterval();
}

//==============================================================================

void SingleCellSimulationViewSimulation::updateGui(QwtSlider *pSlider,
                                                   SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings)
{
    // Update our slider using our delay

    pSlider->setValue(mDelay);

    // Update our simulation information widget using our settings

    pSimulationSettings->setStartingPoint(mStartingPoint);
    pSimulationSettings->setEndingPoint(mEndingPoint);
    pSimulationSettings->setPointInterval(mPointInterval);
}

//==============================================================================

QString SingleCellSimulationViewSimulation::fileName() const
{
    // Retrieve and return our file name

    return mFileName;
}

//==============================================================================

CellMLSupport::CellmlFileRuntime * SingleCellSimulationViewSimulation::cellmlFileRuntime() const
{
    // Retrieve and return our runtime

    return CellMLSupport::CellmlFileManager::instance()->cellmlFile(mFileName)->runtime();
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

void SingleCellSimulationViewSimulation::run()
{
    // Initialise our worker, if not active

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
        mWorker       = new SingleCellSimulationViewSimulationWorker(mDelay,
                                                                     mStartingPoint, mEndingPoint, mPointInterval);

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

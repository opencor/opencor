//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONWORKER_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONWORKER_H

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include <QObject>
#include <QWaitCondition>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace Core {
    class Thread;
}   // namespace Core

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulation;

//==============================================================================

class SingleCellSimulationViewSimulationWorker : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulationWorker(const SolverInterfaces &pSolverInterfaces,
                                                      CellMLSupport::CellmlFileRuntime *pRuntime,
                                                      SingleCellSimulationViewSimulation *pSimulation,
                                                      SingleCellSimulationViewSimulationWorker **pSelf);

    bool isRunning() const;
    bool isPaused() const;

    double progress() const;

    void run();
    void pause();
    void resume();
    void stop();

    void reset();

private:
    Core::Thread *mThread;

    SolverInterfaces mSolverInterfaces;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    SingleCellSimulationViewSimulation *mSimulation;

    double mProgress;

    bool mPaused;
    bool mStopped;

    bool mReset;

    QWaitCondition mPausedCondition;

    bool mError;

    SingleCellSimulationViewSimulationWorker **mSelf;

Q_SIGNALS:
    void running(const bool &pIsResuming);
    void paused();

    void finished(const int &pElapsedTime);

    void error(const QString &pMessage);

private Q_SLOTS:
    void started();

    void emitError(const QString &pMessage);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

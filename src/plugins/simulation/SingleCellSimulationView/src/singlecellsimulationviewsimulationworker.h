//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONWORKER_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONWORKER_H

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include <QMutex>
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
                                                      CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                      SingleCellSimulationViewSimulation *pSimulation);

    bool isRunning() const;
    bool isPaused() const;

    double progress() const;

    void run();
    void pause();
    void resume();
    void stop();

private:
    Core::Thread *mThread;

    SolverInterfaces mSolverInterfaces;

    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    SingleCellSimulationViewSimulation *mSimulation;

    double mProgress;

    bool mPaused;
    bool mStopped;

    QWaitCondition mPausedCondition;

    bool mError;

Q_SIGNALS:
    void running();
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

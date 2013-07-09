//==============================================================================
// Single cell view simulation worker
//==============================================================================

#ifndef SINGLECELLVIEWSIMULATIONWORKER_H
#define SINGLECELLVIEWSIMULATIONWORKER_H

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

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewSimulationWorker : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationWorker(const SolverInterfaces &pSolverInterfaces,
                                            CellMLSupport::CellmlFileRuntime *pRuntime,
                                            SingleCellViewSimulation *pSimulation,
                                            SingleCellViewSimulationWorker **pSelf);

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    double progress() const;

    void run();
    void pause();
    void resume();
    void stop();

    void reset();

private:
    QThread *mThread;

    SolverInterfaces mSolverInterfaces;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    SingleCellViewSimulation *mSimulation;

    double mCurrentPoint;

    double mProgress;

    bool mPaused;
    bool mStopped;

    bool mReset;

    QWaitCondition mPausedCondition;

    bool mError;

    SingleCellViewSimulationWorker **mSelf;

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

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

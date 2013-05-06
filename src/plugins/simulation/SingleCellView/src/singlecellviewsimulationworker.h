//==============================================================================
// Single cell view simulation worker
//==============================================================================

#ifndef SINGLECELLVIEWSIMULATIONWORKER_H
#define SINGLECELLVIEWSIMULATIONWORKER_H

//==============================================================================

#include <QObject>
#include <QWaitCondition>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellMLFileRuntime;
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
    explicit SingleCellViewSimulationWorker(CellMLSupport::CellMLFileRuntime *pRuntime,
                                            SingleCellViewSimulation *pSimulation,
                                            SingleCellViewSimulationWorker **pSelf);

    bool isRunning() const;
    bool isPaused() const;

    double progress() const;

    void run();
    void pause();
    void resume();
    void stop();

    void reset();

private:
    QThread *mThread;

    CellMLSupport::CellMLFileRuntime *mRuntime;

    SingleCellViewSimulation *mSimulation;

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

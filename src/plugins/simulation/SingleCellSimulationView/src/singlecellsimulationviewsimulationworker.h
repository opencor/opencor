//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONWORKER_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONWORKER_H

//==============================================================================

#include <QMutex>
#include <QObject>
#include <QWaitCondition>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationWorker : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulationWorker();

    bool isPausing() const;

    void pause();
    void resume();
    void stop();

private:
    bool mActive;
    bool mPausing;

    QMutex mPauseMutex;
    QWaitCondition mPauseCondition;

Q_SIGNALS:
    void running();
    void pausing();

    void progress(const double &pProgress);

    void finished();

private Q_SLOTS:
    void run();
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

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

class SingleCellSimulationViewSimulationData;

//==============================================================================

class SingleCellSimulationViewSimulationWorker : public QObject
{
    Q_OBJECT

public:
    enum Status {
        Unknown,
        Idling,
        Running,
        Pausing,
        Stopped,
        Finished
    };

    explicit SingleCellSimulationViewSimulationWorker(SingleCellSimulationViewSimulationData *pData);

    Status status() const;
    double progress() const;

    void pause();
    void stop();

private:
    Status mStatus;

    bool mActive;
    bool mPausing;

    QMutex mStatusMutex;
    QWaitCondition mStatusCondition;

    SingleCellSimulationViewSimulationData *mData;

    double mProgress;

    void setProgress(const double &pProgress);

Q_SIGNALS:
    void running();
    void pausing();

    void progress(const double &pProgress);

    void finished(const int &pElapsedTime);

public Q_SLOTS:
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

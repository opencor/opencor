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

    explicit SingleCellSimulationViewSimulationWorker(const SolverInterfaces &pSolverInterfaces,
                                                      CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                      SingleCellSimulationViewSimulationData *pData);

    Status status() const;
    double progress() const;

    void pause();
    void resume();
    void stop();

private:
    Status mStatus;

    bool mActive;
    bool mPausing;

    QMutex mStatusMutex;
    QWaitCondition mStatusCondition;

    SolverInterfaces mSolverInterfaces;

    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    SingleCellSimulationViewSimulationData *mData;

    double mProgress;

    void updateAndEmitProgress(const double &pProgress);

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

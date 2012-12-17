//==============================================================================
// Single cell simulation view simulation
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATION_H
#define SINGLECELLSIMULATIONVIEWSIMULATION_H

//==============================================================================

#include "singlecellsimulationviewsimulationworker.h"

//==============================================================================

#include <QObject>

//==============================================================================

class QwtSlider;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class Thread;
}   // namespace Core

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationData;

//==============================================================================

class SingleCellSimulationViewSimulation : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulation(const QString &pFileName);
    ~SingleCellSimulationViewSimulation();

    QString fileName() const;

    SingleCellSimulationViewSimulationData * data() const;

    SingleCellSimulationViewSimulationWorker::Status workerStatus() const;
    double workerProgress() const;

    void run();
    void pause();
    void stop();

private:
    Core::Thread *mWorkerThread;
    SingleCellSimulationViewSimulationWorker *mWorker;

    QString mFileName;

    SingleCellSimulationViewSimulationData *mData;

Q_SIGNALS:
    void running();
    void pausing();
    void stopped(const int &pElapsedTime);

    void progress(const double &pProgress);

    void error(const QString &pMessage);

private Q_SLOTS:
    void finished(const int &pElapsedTime);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

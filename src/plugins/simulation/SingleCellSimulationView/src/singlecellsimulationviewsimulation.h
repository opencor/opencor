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

class SingleCellSimulationViewWidget;

//==============================================================================

class SingleCellSimulationViewSimulationData
{
public:
    explicit SingleCellSimulationViewSimulationData();

    int delay() const;
    void setDelay(const int &pDelay);

    double startingPoint() const;
    void setStartingPoint(const double &pStartingPoint);

    double endingPoint() const;
    void setEndingPoint(const double &pEndingPoint);

    double pointInterval() const;
    void setPointInterval(const double &pPointInterval);

private:
    int mDelay;

    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;
};

//==============================================================================

class SingleCellSimulationViewSimulation : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulation(const QString &pFileName,
                                                CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);
    ~SingleCellSimulationViewSimulation();

    QString fileName() const;

    SingleCellSimulationViewSimulationWorker::Status workerStatus() const;
    double workerProgress() const;

    void setData(SingleCellSimulationViewWidget *pGui);
    void setDelay(const int &pDelay);

    void run();
    void pause();
    void stop();

private:
    SingleCellSimulationViewSimulationWorker *mWorker;
    Core::Thread *mWorkerThread;

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

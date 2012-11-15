//==============================================================================
// Single cell simulation view simulation
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEW_H
#define SINGLECELLSIMULATIONVIEW_H

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationInformationWidget;
class SingleCellSimulationViewSimulationWorker;

//==============================================================================

class SingleCellSimulationViewSimulation : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulation();
    ~SingleCellSimulationViewSimulation();

    void updateFromGui(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings);
    void updateGui(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings);

    void run();
    void pause();
    void stop();

private:
    QThread *mWorkerThread;
    SingleCellSimulationViewSimulationWorker *mWorker;

    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;

Q_SIGNALS:
    void running();
    void pausing();
    void stopped(const int &pElapsedTime);

    void progress(const double &pProgress);

    void elapsedTime(const int &pElapsedTime);

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

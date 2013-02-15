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

class SingleCellSimulationViewSimulation;

//==============================================================================

class SingleCellSimulationViewSimulationWorker : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulationWorker(const SolverInterfaces &pSolverInterfaces,
                                                      CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                      SingleCellSimulationViewSimulation *pSimulation);

    void pause();
    void resume();
    void stop();

private:
    bool mActive;
    bool mPausing;

    QMutex mStatusMutex;
    QWaitCondition mStatusCondition;

    SolverInterfaces mSolverInterfaces;

    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    SingleCellSimulationViewSimulation *mSimulation;

    bool mError;

Q_SIGNALS:
    void running();
    void pausing();

    void finished(const int &pElapsedTime);

    void error(const QString &pMessage);

public Q_SLOTS:
    void run();

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

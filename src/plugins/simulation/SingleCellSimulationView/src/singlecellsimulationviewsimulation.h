//==============================================================================
// Single cell simulation view simulation
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATION_H
#define SINGLECELLSIMULATIONVIEWSIMULATION_H

//==============================================================================

#include "coresolver.h"
#include "singlecellsimulationviewsimulationworker.h"
#include "solverinterface.h"

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

class SingleCellSimulationViewSimulationDataResults
{
public:
    explicit SingleCellSimulationViewSimulationDataResults(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                           const int &pSize);
    ~SingleCellSimulationViewSimulationDataResults();

    void addData(const double &pPoint, double *pConstants, double *pStates,
                 double *pRates, double *pAlgebraic);

    int lastResultIndex() const;

    double * points() const;

    double **constants() const;
    double **states() const;
    double **rates() const;
    double **algebraic() const;

private:
    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    int mLastResultIndex;

    double *mPoints;

    double **mConstants;
    double **mStates;
    double **mRates;
    double **mAlgebraic;
};

//==============================================================================

class SingleCellSimulationViewSimulationData : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulationData(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);
    ~SingleCellSimulationViewSimulationData();

    double * constants() const;
    double * states() const;
    double * rates() const;
    double * algebraic() const;
    double * condVar() const;

    int delay() const;
    void setDelay(const int &pDelay);

    double startingPoint() const;
    void setStartingPoint(const double &pStartingPoint);

    double endingPoint() const;
    void setEndingPoint(const double &pEndingPoint);

    double pointInterval() const;
    void setPointInterval(const double &pPointInterval);

    QString odeSolverName() const;
    void setOdeSolverName(const QString &pOdeSolverName);

    CoreSolver::Properties odeSolverProperties() const;
    void addOdeSolverProperty(const QString &pName, const QVariant &pValue);

    QString daeSolverName() const;
    void setDaeSolverName(const QString &pDaeSolverName);

    CoreSolver::Properties daeSolverProperties() const;
    void addDaeSolverProperty(const QString &pName, const QVariant &pValue);

    QString nlaSolverName() const;
    void setNlaSolverName(const QString &pNlaSolverName);

    CoreSolver::Properties nlaSolverProperties() const;
    void addNlaSolverProperty(const QString &pName, const QVariant &pValue);

    void reset();
    void recomputeComputedConstantsAndVariables();
    void recomputeVariables(const double &pCurrentPoint);

    SingleCellSimulationViewSimulationDataResults * results() const;

    void resetResults();
    void addResults(const double &pPoint);

private:
    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    int mDelay;

    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;

    QString mOdeSolverName;
    CoreSolver::Properties mOdeSolverProperties;

    QString mDaeSolverName;
    CoreSolver::Properties mDaeSolverProperties;

    QString mNlaSolverName;
    CoreSolver::Properties mNlaSolverProperties;

    double *mConstants;
    double *mStates;
    double *mRates;
    double *mAlgebraic;
    double *mCondVar;

    SingleCellSimulationViewSimulationDataResults *mResults;

Q_SIGNALS:
    void dataChanged();

    void results(SingleCellSimulationViewSimulationDataResults *pResults);
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

    SingleCellSimulationViewSimulationData * data() const;

    SingleCellSimulationViewSimulationWorker::Status workerStatus() const;
    double workerProgress() const;

    void setDelay(const int &pDelay);

    void reset();
    void recomputeComputedConstantsAndVariables();

    void run(const SolverInterfaces &pSolverInterfaces);
    void pause();
    void resume();
    void stop();

private:
    SingleCellSimulationViewSimulationWorker *mWorker;
    Core::Thread *mWorkerThread;

    QString mFileName;

    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

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

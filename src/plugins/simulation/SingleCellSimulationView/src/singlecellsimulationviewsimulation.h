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

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewWidget;

//==============================================================================

class SingleCellSimulationViewSimulationData : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulationData(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                    const SolverInterfaces &pSolverInterfaces);
    ~SingleCellSimulationViewSimulationData();

    double * constants() const;
    double * states() const;
    double * rates() const;
    double * algebraic() const;
    double * condVar() const;

    int delay() const;
    void setDelay(const int &pDelay);

    double startingPoint() const;
    void setStartingPoint(const double &pStartingPoint,
                          const bool &pRecompute = true);

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
    void setNlaSolverName(const QString &pNlaSolverName,
                          const bool &pReset = true);

    CoreSolver::Properties nlaSolverProperties() const;
    void addNlaSolverProperty(const QString &pName, const QVariant &pValue,
                              const bool &pReset = true);

    void reset();

    void recomputeComputedConstantsAndVariables();
    void recomputeVariables(const double &pCurrentPoint,
                            const bool &pEmitSignal = true);

    void checkForModifications();

private:
    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    SolverInterfaces mSolverInterfaces;

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

    double *mInitialConstants;
    double *mInitialStates;

Q_SIGNALS:
    void updated();
    void modified(const bool &pIsModified);

    void error(const QString &pMessage);
};

//==============================================================================

class SingleCellSimulationViewSimulationResults : public QObject
{
public:
    explicit SingleCellSimulationViewSimulationResults(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                       SingleCellSimulationViewSimulation *pSimulation);
    ~SingleCellSimulationViewSimulationResults();

    bool reset(const bool &pCreateArrays = true);

    void addPoint(const double &pPoint);

    qulonglong size() const;

    double * points() const;

    double **constants() const;
    double **states() const;
    double **rates() const;
    double **algebraic() const;

    bool exportToCsv(const QString &pFileName) const;

private:
    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    SingleCellSimulationViewSimulation *mSimulation;

    qulonglong mSize;

    double *mPoints;

    double **mConstants;
    double **mStates;
    double **mRates;
    double **mAlgebraic;

    bool createArrays();
    void deleteArrays();
};

//==============================================================================

class SingleCellSimulationViewSimulation : public QObject
{
    Q_OBJECT

    friend class SingleCellSimulationViewSimulationWorker;

public:
    explicit SingleCellSimulationViewSimulation(const QString &pFileName,
                                                CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                const SolverInterfaces &pSolverInterfaces);
    ~SingleCellSimulationViewSimulation();

    QString fileName() const;

    SingleCellSimulationViewSimulationData * data() const;
    SingleCellSimulationViewSimulationResults * results() const;

    bool isRunning() const;
    bool isPaused() const;

    double progress() const;

    void setDelay(const int &pDelay);

    double requiredMemory();

    double size();

    void run();
    void pause();
    void resume();
    void stop();

    void resetWorker();

private:
    SingleCellSimulationViewSimulationWorker *mWorker;

    QString mFileName;

    CellMLSupport::CellmlFileRuntime *mCellmlFileRuntime;

    SolverInterfaces mSolverInterfaces;

    SingleCellSimulationViewSimulationData *mData;
    SingleCellSimulationViewSimulationResults *mResults;

    bool simulationSettingsOk(const bool &pEmitError = true);

Q_SIGNALS:
    void running();
    void paused();
    void stopped(const int &pElapsedTime);

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

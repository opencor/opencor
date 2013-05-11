//==============================================================================
// Single cell view simulation
//==============================================================================

#ifndef SINGLECELLVIEWSIMULATION_H
#define SINGLECELLVIEWSIMULATION_H

//==============================================================================

#include "coresolver.h"
#include "singlecellviewsimulationworker.h"
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

namespace SingleCellView {

//==============================================================================

class SingleCellViewWidget;

//==============================================================================

class SingleCellViewSimulationData : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationData(CellMLSupport::CellmlFileRuntime *pRuntime,
                                          const SolverInterfaces &pSolverInterfaces);
    ~SingleCellViewSimulationData();

    double * constants() const;
    double * rates() const;
    double * states() const;
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
    CellMLSupport::CellmlFileRuntime *mRuntime;

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
    double *mRates;
    double *mStates;
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

class SingleCellViewSimulationResults : public QObject
{
public:
    explicit SingleCellViewSimulationResults(CellMLSupport::CellmlFileRuntime *pRuntime,
                                             SingleCellViewSimulation *pSimulation);
    ~SingleCellViewSimulationResults();

    bool reset(const bool &pCreateArrays = true);

    void addPoint(const double &pPoint);

    qulonglong size() const;

    double * points() const;

    double **constants() const;
    double **rates() const;
    double **states() const;
    double **algebraic() const;

    bool exportToCsv(const QString &pFileName) const;

private:
    CellMLSupport::CellmlFileRuntime *mRuntime;

    SingleCellViewSimulation *mSimulation;

    qulonglong mSize;

    double *mPoints;

    double **mConstants;
    double **mRates;
    double **mStates;
    double **mAlgebraic;

    bool createArrays();
    void deleteArrays();
};

//==============================================================================

class SingleCellViewSimulation : public QObject
{
    Q_OBJECT

    friend class SingleCellViewSimulationWorker;

public:
    explicit SingleCellViewSimulation(const QString &pFileName,
                                      CellMLSupport::CellmlFileRuntime *pRuntime,
                                      const SolverInterfaces &pSolverInterfaces);
    ~SingleCellViewSimulation();

    QString fileName() const;

    SingleCellViewSimulationData * data() const;
    SingleCellViewSimulationResults * results() const;

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
    SingleCellViewSimulationWorker *mWorker;

    QString mFileName;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    SolverInterfaces mSolverInterfaces;

    SingleCellViewSimulationData *mData;
    SingleCellViewSimulationResults *mResults;

    bool simulationSettingsOk(const bool &pEmitError = true);

Q_SIGNALS:
    void running(const bool &pIsResuming);
    void paused();
    void stopped(const int &pElapsedTime);

    void error(const QString &pMessage);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

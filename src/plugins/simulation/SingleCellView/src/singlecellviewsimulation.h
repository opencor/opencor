//==============================================================================
// Single cell view simulation
//==============================================================================

#ifndef SINGLECELLVIEWSIMULATION_H
#define SINGLECELLVIEWSIMULATION_H

//==============================================================================

#include "singlecellviewsimulationworker.h"

//==============================================================================

#include <QObject>

//==============================================================================

class QwtSlider;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellMLFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

typedef QMap<QString, QVariant> Properties;

//==============================================================================

class SingleCellViewWidget;

//==============================================================================

class SingleCellViewSimulationData : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationData(CellMLSupport::CellMLFileRuntime *pRuntime);
    ~SingleCellViewSimulationData();

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

    QString solverName() const;
    void setSolverName(const QString &pSolverName);

    bool isDAETypeSolver() const;

    Properties solverProperties() const;
    void addSolverProperty(const QString &pName, const QVariant &pValue);

    void reset();

    void recomputeComputedConstantsAndVariables();
    void recomputeVariables(const double &pCurrentPoint,
                            const bool &pEmitSignal = true);

    void checkForModifications();

private:
    CellMLSupport::CellMLFileRuntime *mRuntime;

    int mDelay;

    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;

    QString mSolverName;
    Properties mSolverProperties;

    double *mConstants;
    double *mStates;
    double *mRates;
    double *mAlgebraic;
    double *mCondVar;

    double *mInitialConstants;
    double *mInitialStates;

    void allocateStorageArrays();
    void freeStorageArrays();

Q_SIGNALS:
    void updated();
    void modified(const bool &pIsModified);

    void error(const QString &pMessage);
};

//==============================================================================

class SingleCellViewSimulationResults : public QObject
{
public:
    explicit SingleCellViewSimulationResults(CellMLSupport::CellMLFileRuntime *pRuntime,
                                             SingleCellViewSimulation *pSimulation);
    ~SingleCellViewSimulationResults();

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
    CellMLSupport::CellMLFileRuntime *mRuntime;

    SingleCellViewSimulation *mSimulation;

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

class SingleCellViewSimulation : public QObject
{
    Q_OBJECT

    friend class SingleCellViewSimulationWorker;

public:
    explicit SingleCellViewSimulation(const QString &pFileName,
                                                CellMLSupport::CellMLFileRuntime *pRuntime);
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

    CellMLSupport::CellMLFileRuntime *mRuntime;

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

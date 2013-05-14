//==============================================================================
// Single cell view simulation
//==============================================================================

#ifndef SINGLECELLVIEWSIMULATION_H
#define SINGLECELLVIEWSIMULATION_H

//==============================================================================

#include <QObject>
#include <QMap>
#include <QTime>
#include <QPointer>
#include <QMutex>
#include "cellml-api-cxx-support.hpp"
#include "IfaceCIS.hxx"

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

class GrabInitialValueListener 
    : public QObject, public iface::cellml_services::IntegrationProgressObserver
{
public:
    Q_OBJECT

    CDA_IMPL_REFCOUNT;
    CDA_IMPL_QI1(cellml_services::IntegrationProgressObserver);

    GrabInitialValueListener(iface::cellml_services::CellMLIntegrationRun* pRun);

    std::string objid() throw() {
        return "initialvaluelistener";
    }

    void computedConstants(const std::vector<double>& pConstValues) throw() {
        mConstValues = pConstValues;
    }
    void results(const std::vector<double>& pState) throw();

    void done() throw() {}
    void failed(const std::string& pFailWhy) throw();

    std::vector<double>& consts() { return mConstValues; }
    std::vector<double>& states() { return mState; }

private:
    bool mHadResults;
    ObjRef<iface::cellml_services::CellMLIntegrationRun> mRun;
    std::vector<double> mConstValues;
    std::vector<double> mState;

Q_SIGNALS:
    void resultsReady();
    void solveFailure(QString pFailWhy);
};

class ResultListener
    : public QObject, public iface::cellml_services::IntegrationProgressObserver
{
public:
    Q_OBJECT
    CDA_IMPL_REFCOUNT;
    CDA_IMPL_QI1(cellml_services::IntegrationProgressObserver);

    ResultListener(iface::cellml_services::CellMLIntegrationRun* pRun,
                   int pNStates, int pNAlgebraic);

    std::string objid() throw() {
        return "resultlistener";
    }

    void computedConstants(const std::vector<double>&) throw() {
    }
    void results(const std::vector<double>& pState) throw();

    void done() throw();
    void failed(const std::string& pFailWhy) throw();

    void delay(int pDelay) { mDelay = pDelay; }
    void suspend();
    void unsuspend();
private:
    QAtomicInt mDelay;
    ObjRef<iface::cellml_services::CellMLIntegrationRun> mRun;
    int mNStates, mNAlgebraic;
    bool mSuspended;
    QMutex mResultMutex;

Q_SIGNALS:
    void solveDone();
    void solveFailure(QString pFailWhy);
    void solvePointAvailable(double bvar, const QList<double> states,
                             const QList<double> rates,
                             const QList<double> algebraic);
};


class SingleCellViewSimulation;

class SingleCellViewSimulationData : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationData(CellMLSupport::CellMLFileRuntime *pRuntime);
    ~SingleCellViewSimulationData();

    enum SimulatorState {
      SIMSTATE_INITIAL,
      SIMSTATE_WAITING_IV,
      SIMSTATE_GOT_IV,
      SIMSTATE_WAITING_RESULTS,
      SIMSTATE_PAUSED
    };

    QList<double>& constants() { return mConstants; }
    const QList<double>& constants() const { return mConstants; }

    QList<double>& states() { return mStates; }
    const QList<double>& states() const { return mStates; }

    QList<double>& rates() { return mRates; }
    const QList<double>& rates() const { return mRates; }

    QList<double>& algebraic() { return mAlgebraic; }
    const QList<double>& algebraic() const { return mAlgebraic; }

    QList<double>& condVar() { return mCondVar; }
    const QList<double>& condVar() const { return mCondVar; }

    void pause();
    void resume();

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

    void checkForModifications();

    SimulatorState state() const { return mState; }
    void state(SimulatorState pState) { mState = pState; }
    void startMainSimulation(SingleCellViewSimulation* pSignalsTo);
    void stopAllSimulations();

    void ensureCodeCompiled();

    void setPoint(const QList<double>& pStates,
                  const QList<double>& pRates,
                  const QList<double>& pAlgebraic);
    void didReset(bool pDid) { mDidReset = pDid; }
    bool didReset() const { return mDidReset; }

private:
    CellMLSupport::CellMLFileRuntime *mRuntime;

    SimulatorState mState;
    ObjRef<GrabInitialValueListener> mIVGrabber;
    ObjRef<ResultListener> mResultReceiver;
    ObjRef<iface::cellml_services::CellMLIntegrationRun> mIntegrationRun;

    bool mDidReset;
    int mDelay;

    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;

    QString mSolverName;
    Properties mSolverProperties;

    QList<double> mConstants;
    QList<double> mStates;
    QList<double> mRates;
    QList<double> mAlgebraic;
    QList<double> mCondVar;

    QList<double> mInitialConstants;
    QList<double> mInitialStates;

    void newIntegrationRun();
    void setupOverrides();

Q_SIGNALS:
    void updated();
    void modified(QPointer<OpenCOR::SingleCellView::SingleCellViewSimulationData> pSimulationData,
                  const bool &pIsModified);

    void error(const QString &pMessage);

private Q_SLOTS:
    void initialValuesIn();
    void initialValuesFailed(QString pFailWhy);
};

//==============================================================================

class SingleCellViewSimulationResults : public QObject
{
public:
    typedef QList<QList<double> > Matrix;

    explicit SingleCellViewSimulationResults(CellMLSupport::CellMLFileRuntime *pRuntime,
                                             SingleCellViewSimulation *pSimulation);
    ~SingleCellViewSimulationResults();

    void reset();

    void addPoint(const double &pPoint,
                  const QList<double>& pStates, const QList<double>& pRates,
                  const QList<double>& pAlgebraic);

    qulonglong size() const;

    QList<double>& points() { return mPoints; }
    const QList<double>& points() const { return mPoints; }

    Matrix& states() { return mStates; };
    const Matrix& states() const { return mStates; };
    Matrix& rates() { return mRates; };
    const Matrix& rates() const { return mRates; };
    Matrix& algebraic() { return mAlgebraic; };
    const Matrix& algebraic() const { return mAlgebraic; };

    bool exportToCsv(const QString &pFileName) const;

private:
    CellMLSupport::CellMLFileRuntime *mRuntime;
    SingleCellViewSimulation *mSimulation;
    QList<double> mPoints;
    // Note that the outer list index is timestep, the inner is variable offset.
    // Doing it this way allows us to leverage the implicit COW sharing supported
    // by QList to avoid unneeded data copying.
    Matrix mStates, mRates, mAlgebraic;
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

private:
    QString mFileName;
    QTime mRunTime;
    QTime mLastUpdate;

    CellMLSupport::CellMLFileRuntime *mRuntime;

    SingleCellViewSimulationData *mData;
    SingleCellViewSimulationResults *mResults;

    bool simulationSettingsOk(const bool &pEmitError = true);

public Q_SLOTS:
    void simulationComplete();
    void simulationFailed(QString pError);
    void simulationDataAvailable(double pPoint, QList<double> pStates,
                                 QList<double> pRates, QList<double> pAlgebraic);
    void reemitError(const QString& pMsg);

Q_SIGNALS:
    void running(QPointer<OpenCOR::SingleCellView::SingleCellViewSimulation> pSimulation, bool pIsResuming);
    void paused(QPointer<OpenCOR::SingleCellView::SingleCellViewSimulation> pSimulation);
    void stopped(QPointer<OpenCOR::SingleCellView::SingleCellViewSimulation>,
                 const int &pElapsedTime);

    void error(QPointer<OpenCOR::SingleCellView::SingleCellViewSimulation> pSimulation,
               const QString &pMessage);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

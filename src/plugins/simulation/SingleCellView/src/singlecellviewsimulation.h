/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Single Cell view simulation
//==============================================================================

#pragma once

//==============================================================================

#include "datastoreinterface.h"
#include "singlecellviewglobal.h"
#include "singlecellviewsimulationworker.h"
#include "solverinterface.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace PythonWrapper {
    class PythonWrapperSingleCellView;
}   // namespace PythonWrapper

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulation;

//==============================================================================

class SINGLECELLVIEW_EXPORT SingleCellViewSimulationData : public QObject
{
    Q_OBJECT

    friend class PythonWrapper::PythonWrapperSingleCellView;

public:
    explicit SingleCellViewSimulationData(SingleCellViewSimulation *pSimulation,
                                          const SolverInterfaces &pSolverInterfaces);
    ~SingleCellViewSimulationData();

    void update();

    double * constants() const;
    double * rates() const;
    double * states() const;
    double * algebraic() const;
    double * condVar() const;

    SolverInterface * odeSolverInterface() const;

    QString odeSolverName() const;
    void setOdeSolverName(const QString &pOdeSolverName);

    Solver::Solver::Properties odeSolverProperties() const;
    void addOdeSolverProperty(const QString &pName, const QVariant &pValue);

    SolverInterface * daeSolverInterface() const;

    QString daeSolverName() const;
    void setDaeSolverName(const QString &pDaeSolverName);

    Solver::Solver::Properties daeSolverProperties() const;
    void addDaeSolverProperty(const QString &pName, const QVariant &pValue);

    SolverInterface * nlaSolverInterface() const;

    QString nlaSolverName() const;
    void setNlaSolverName(const QString &pNlaSolverName,
                          const bool &pReset = true);

    Solver::Solver::Properties nlaSolverProperties() const;
    void addNlaSolverProperty(const QString &pName, const QVariant &pValue,
                              const bool &pReset = true);

    DataStore::DataStore * resultsDataStore() const;

    DataStore::DataStoreVariable * pointVariable() const;

    DataStore::DataStoreVariables constantVariables() const;
    DataStore::DataStoreVariables rateVariables() const;
    DataStore::DataStoreVariables stateVariables() const;
    DataStore::DataStoreVariables algebraicVariables() const;

public slots:
    OpenCOR::SingleCellView::SingleCellViewSimulation * simulation() const;

    int delay() const;
    void setDelay(const int &pDelay);

    double startingPoint() const;
    void setStartingPoint(const double &pStartingPoint,
                          const bool &pRecompute = true);

    double endingPoint() const;
    void setEndingPoint(const double &pEndingPoint);

    double pointInterval() const;
    void setPointInterval(const double &pPointInterval);

    void reset(const bool &pInitialize = true);

    void recomputeComputedConstantsAndVariables(const double &pCurrentPoint,
                                                const bool &pInitialize = true);
    void recomputeVariables(const double &pCurrentPoint);

    bool isModified() const;
    void checkForModifications();

private:
    SingleCellViewSimulation *mSimulation;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    SolverInterfaces mSolverInterfaces;

    int mDelay;

    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;

    QString mOdeSolverName;
    Solver::Solver::Properties mOdeSolverProperties;

    QString mDaeSolverName;
    Solver::Solver::Properties mDaeSolverProperties;

    QString mNlaSolverName;
    Solver::Solver::Properties mNlaSolverProperties;

    DataStore::DataStore *mResultsDataStore;

    DataStore::DataStoreVariable *mPointVariable;

    DataStore::DataStoreVariables mConstantVariables;
    DataStore::DataStoreVariables mRateVariables;
    DataStore::DataStoreVariables mStateVariables;
    DataStore::DataStoreVariables mAlgebraicVariables;

    double *mConstants;
    double *mRates;
    double *mStates;
    double *mDummyStates;
    double *mAlgebraic;
    double *mCondVar;

    double *mInitialConstants;
    double *mInitialStates;

    void createArrays();
    void deleteArrays();

    void createResultsDataStore();

    QString uri(const QStringList &pComponentHierarchy, const QString &pName);

    SolverInterface * solverInterface(const QString &pSolverName) const;

signals:
    void updated(const double &pCurrentPoint);
    void modified(const bool &pIsModified);

    void error(const QString &pMessage);
};

//==============================================================================

class SINGLECELLVIEW_EXPORT SingleCellViewSimulationResults : public QObject
{
    Q_OBJECT

    friend class PythonWrapper::PythonWrapperSingleCellView;

public:
    explicit SingleCellViewSimulationResults(SingleCellViewSimulation *pSimulation);
    ~SingleCellViewSimulationResults();

    void update();

    void addPoint(const double &pPoint);

    double * points() const;

    double * constants(const int &pIndex) const;
    double * rates(const int &pIndex) const;
    double * states(const int &pIndex) const;
    double * algebraic(const int &pIndex) const;

public slots:
    bool reset(const bool &pAllocateArrays = true);

    qulonglong size() const;

    OpenCOR::DataStore::DataStore * dataStore() const;

private:
    SingleCellViewSimulation *mSimulation;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    DataStore::DataStore *mDataStore;

    const DataStore::DataStoreVariable *mPointVariable;

    const DataStore::DataStoreVariables mConstantVariables;
    const DataStore::DataStoreVariables mRateVariables;
    const DataStore::DataStoreVariables mStateVariables;
    const DataStore::DataStoreVariables mAlgebraicVariables;

    bool createArrays();
    void deleteArrays();
};

//==============================================================================

class SINGLECELLVIEW_EXPORT SingleCellViewSimulation : public QObject
{
    Q_OBJECT

    friend class SingleCellViewSimulationWorker;

public:
    explicit SingleCellViewSimulation(CellMLSupport::CellmlFileRuntime *pRuntime,
                                      const SolverInterfaces &pSolverInterfaces);
    ~SingleCellViewSimulation();

    CellMLSupport::CellmlFileRuntime * runtime() const;

    void update(CellMLSupport::CellmlFileRuntime *pRuntime);

    bool run();
    bool pause();
    bool resume();
    bool stop();

public slots:
    QString fileName() const;

    OpenCOR::SingleCellView::SingleCellViewSimulationData * data() const;
    OpenCOR::SingleCellView::SingleCellViewSimulationResults * results() const;

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    int delay() const;
    void setDelay(const int &pDelay);

    double requiredMemory();

    double size();

    bool reset();

private:
    SingleCellViewSimulationWorker *mWorker;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    SolverInterfaces mSolverInterfaces;

    SingleCellViewSimulationData *mData;
    SingleCellViewSimulationResults *mResults;

    bool simulationSettingsOk(const bool &pEmitSignal = true);

signals:
    void running(const bool &pIsResuming);
    void paused();
    void stopped(const qint64 &pElapsedTime);

    void error(const QString &pMessage);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

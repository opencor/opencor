/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view simulation
//==============================================================================

#pragma once

//==============================================================================

#include "datastoreinterface.h"
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

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewSimulationData : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationData(SingleCellViewSimulation *pSimulation,
                                          const SolverInterfaces &pSolverInterfaces);
    ~SingleCellViewSimulationData();

    void update();

    SingleCellViewSimulation * simulation() const;

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

Q_SIGNALS:
    void updated(const double &pCurrentPoint);
    void modified(const bool &pIsModified);

    void error(const QString &pMessage);
};

//==============================================================================

class SingleCellViewSimulationResults : public QObject
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationResults(SingleCellViewSimulation *pSimulation);
    ~SingleCellViewSimulationResults();

    void update();

    bool reset(const bool &pCreateDataStore = true);

    void addPoint(const double &pPoint);

    qulonglong size() const;

    DataStore::DataStore * dataStore() const;

    double * points() const;

    double * constants(const int &pIndex) const;
    double * rates(const int &pIndex) const;
    double * states(const int &pIndex) const;
    double * algebraic(const int &pIndex) const;

private:
    SingleCellViewSimulation *mSimulation;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    qulonglong mSize;

    DataStore::DataStore *mDataStore;

    DataStore::DataStoreVariable *mPoints;

    DataStore::DataStoreVariables mConstants;
    DataStore::DataStoreVariables mRates;
    DataStore::DataStoreVariables mStates;
    DataStore::DataStoreVariables mAlgebraic;

    bool createDataStore();
    void deleteDataStore();

    QString uri(const QStringList &pComponentHierarchy, const QString &pName);
};

//==============================================================================

class SingleCellViewSimulation : public QObject
{
    Q_OBJECT

    friend class SingleCellViewSimulationWorker;

public:
    explicit SingleCellViewSimulation(CellMLSupport::CellmlFileRuntime *pRuntime,
                                      const SolverInterfaces &pSolverInterfaces);
    ~SingleCellViewSimulation();

    CellMLSupport::CellmlFileRuntime * runtime() const;

    SingleCellViewSimulationData * data() const;
    SingleCellViewSimulationResults * results() const;

    void update(CellMLSupport::CellmlFileRuntime *pRuntime);

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    int delay() const;
    void setDelay(const int &pDelay);

    double requiredMemory();

    double size();

    bool run();
    bool pause();
    bool resume();
    bool stop();

    bool reset();

private:
    SingleCellViewSimulationWorker *mWorker;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    SolverInterfaces mSolverInterfaces;

    SingleCellViewSimulationData *mData;
    SingleCellViewSimulationResults *mResults;

    bool simulationSettingsOk(const bool &pEmitError = true);

Q_SIGNALS:
    void running(const bool &pIsResuming);
    void paused();
    void stopped(const qint64 &pElapsedTime);

    void error(const QString &pMessage);
};

//==============================================================================

typedef QList<SingleCellViewSimulation *> SingleCellViewSimulations;

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Simulation
//==============================================================================

#pragma once

//==============================================================================

#include "datastoreinterface.h"
#include "simulationsupportglobal.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFile;
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace COMBINESupport {
    class CombineArchive;
}   // namespace COMBINESupport

//==============================================================================

namespace SEDMLSupport {
    class SedmlFile;
}   // namespace SEDMLSupport

//==============================================================================

namespace SimulationSupport {

//==============================================================================

class SimulationSupportPythonWrapper;

//==============================================================================

class Simulation;
class SimulationWorker;

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationData : public QObject
{
    Q_OBJECT

    friend class SimulationSupportPythonWrapper;

public:
    explicit SimulationData(Simulation *pSimulation);
    ~SimulationData();

    double * constants() const;
    double * rates() const;
    double * states() const;
    double * algebraic() const;
    double * condVar() const;

    SolverInterface * odeSolverInterface() const;

    Solver::Solver::Properties odeSolverProperties() const;
    void addOdeSolverProperty(const QString &pName, const QVariant &pValue);

    SolverInterface * daeSolverInterface() const;

    Solver::Solver::Properties daeSolverProperties() const;
    void addDaeSolverProperty(const QString &pName, const QVariant &pValue);

    SolverInterface * nlaSolverInterface() const;

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

    void reload();

    OpenCOR::SimulationSupport::Simulation * simulation() const;

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

    QString daeSolverName() const;
    void setDaeSolverName(const QString &pDaeSolverName);

    QString nlaSolverName() const;
    void setNlaSolverName(const QString &pNlaSolverName,
                          const bool &pReset = true);

    void reset(const bool &pInitialize = true);

    void recomputeComputedConstantsAndVariables(const double &pCurrentPoint,
                                                const bool &pInitialize = true);
    void recomputeVariables(const double &pCurrentPoint);

    bool isModified() const;
    void checkForModifications();

private:
    Simulation *mSimulation;

    CellMLSupport::CellmlFileRuntime *mRuntime;

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

class SIMULATIONSUPPORT_EXPORT SimulationResults : public QObject
{
    Q_OBJECT

    friend class SimulationSupportPythonWrapper;

public:
    explicit SimulationResults(Simulation *pSimulation);
    ~SimulationResults();

    void reload();

    void addPoint(const double &pPoint);

    double * points() const;

    double * algebraic(const int &pIndex) const;
    double * constants(const int &pIndex) const;
    double * rates(const int &pIndex) const;
    double * states(const int &pIndex) const;

public slots:
    bool reset(const bool &pAllocateArrays = true);

    qulonglong size() const;

    OpenCOR::DataStore::DataStore * dataStore() const;

private:
    Simulation *mSimulation;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    DataStore::DataStore *mDataStore;

    const DataStore::DataStoreVariable *mPointVariable;

    const DataStore::DataStoreVariables mConstantVariables;
    const DataStore::DataStoreVariables mRateVariables;
    const DataStore::DataStoreVariables mStateVariables;
    const DataStore::DataStoreVariables mAlgebraicVariables;

    bool createDataStoreArrays();
    void deleteDataStoreArrays();
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT Simulation : public QObject
{
    Q_OBJECT

public:
    enum FileType {
        CellmlFile,
        SedmlFile,
        CombineArchive
    };

    explicit Simulation(const QString &pFileName);
    ~Simulation();

    CellMLSupport::CellmlFileRuntime * runtime() const;

    Simulation::FileType fileType() const;

    CellMLSupport::CellmlFile * cellmlFile() const;
    SEDMLSupport::SedmlFile * sedmlFile() const;
    COMBINESupport::CombineArchive * combineArchive() const;

    bool run();
    bool pause();
    bool resume();
    bool stop();

public slots:
    QString fileName() const;

    OpenCOR::SimulationSupport::SimulationData * data() const;
    OpenCOR::SimulationSupport::SimulationResults * results() const;

    void reload();
    void rename(const QString &pFileName);

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    int delay() const;
    void setDelay(const int &pDelay);

    double requiredMemory();

    double size();

    bool reset();

private:
    QString mFileName;

    FileType mFileType;

    CellMLSupport::CellmlFile *mCellmlFile;
    SEDMLSupport::SedmlFile *mSedmlFile;
    COMBINESupport::CombineArchive *mCombineArchive;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    SimulationWorker *mWorker;

    SimulationData *mData;
    SimulationResults *mResults;

    void retrieveFileDetails();

    bool simulationSettingsOk(const bool &pEmitSignal = true);

signals:
    void running(const bool &pIsResuming);
    void paused();
    void stopped(const qint64 &pElapsedTime);

    void error(const QString &pMessage);
};

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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

#include <functional>

//==============================================================================

namespace libsedml {
    class SedListOfAlgorithmParameters;
} // namespace libsedml

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFile;
    class CellmlFileRuntime;
} // namespace CellMLSupport

//==============================================================================

namespace COMBINESupport {
    class CombineArchive;
} // namespace COMBINESupport

//==============================================================================

namespace SEDMLSupport {
    class SedmlFile;
} // namespace SEDMLSupport

//==============================================================================

namespace SimulationSupport {

//==============================================================================

class Simulation;
class SimulationData;
class SimulationWorker;

//==============================================================================
// Note: we bind the SimulationData object to the the first parameter of
//       updateParameters() to create a function object to be called when
//       simulation parameters are updated...

#if defined(Q_OS_WIN)
    using SimulationDataUpdatedFunction = std::_Binder<std::_Unforced, void (*)(SimulationData *), SimulationData *>;
#elif defined(Q_OS_LINUX)
    using SimulationDataUpdatedFunction = std::_Bind_helper<false, void (*)(SimulationData *), SimulationData *>::type;
#else
    using SimulationDataUpdatedFunction = std::__bind<void (*)(SimulationData *), SimulationData *>;
#endif

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationIssue
{
public:
    enum class Type {
        Information,
        Error,
        Warning,
        Fatal
    };

    explicit SimulationIssue(Type pType, int pLine, int pColumn,
                             const QString &pMessage);
    explicit SimulationIssue(Type pType, const QString &pMessage);

    Type type() const;
    QString typeAsString() const;

    int line() const;
    int column() const;
    QString message() const;

private:
    Type mType;
    int mLine;
    int mColumn;
    QString mMessage;
};

//==============================================================================

using SimulationIssues = QList<SimulationIssue>;

//==============================================================================

class SimulationObject : public QObject
{
    Q_OBJECT

public:
    explicit SimulationObject(Simulation *pSimulation);

protected:
    Simulation *mSimulation;
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationData : public SimulationObject
{
    Q_OBJECT

public:
    explicit SimulationData(Simulation *pSimulation);
    ~SimulationData() override;

    double * constants() const;
    double * rates() const;
    double * states() const;
    double * algebraic() const;
    double * data(DataStore::DataStore *pDataStore) const;

    void importData(DataStore::DataStoreImportData *pImportData);

    DataStore::DataStoreValues * constantsValues() const;
    DataStore::DataStoreValues * ratesValues() const;
    DataStore::DataStoreValues * statesValues() const;
    DataStore::DataStoreValues * algebraicValues() const;

    void setStartingPoint(double pStartingPoint, bool pRecompute = true);
    void setEndingPoint(double pEndingPoint);
    void setPointInterval(double pPointInterval);

    SolverInterface * odeSolverInterface() const;
    SolverInterface * nlaSolverInterface() const;

    void setOdeSolverName(const QString &pOdeSolverName);
    void setNlaSolverName(const QString &pNlaSolverName, bool pReset = true);

    SimulationDataUpdatedFunction & simulationDataUpdatedFunction();

    static void updateParameters(SimulationData *pSimulationData);

private:
    quint64 mDelay = 0;

    double mStartingPoint = 0.0;
    double mEndingPoint = 1000.0;
    double mPointInterval = 1.0;

    QString mOdeSolverName;
    Solver::Solver::Properties mOdeSolverProperties;

    QString mDaeSolverName;
    Solver::Solver::Properties mDaeSolverProperties;

    QString mNlaSolverName;
    Solver::Solver::Properties mNlaSolverProperties;

    DataStore::DataStoreArray *mConstantsArray = nullptr;
    DataStore::DataStoreArray *mRatesArray = nullptr;
    DataStore::DataStoreArray *mStatesArray = nullptr;
    DataStore::DataStoreArray *mAlgebraicArray = nullptr;

    DataStore::DataStoreValues *mConstantsValues = nullptr;
    DataStore::DataStoreValues *mRatesValues = nullptr;
    DataStore::DataStoreValues *mStatesValues = nullptr;
    DataStore::DataStoreValues *mAlgebraicValues = nullptr;

    double *mInitialConstants = nullptr;
    double *mInitialStates = nullptr;
    double *mDummyStates = nullptr;

    QMap<DataStore::DataStore *, double *> mData;

    SimulationDataUpdatedFunction mSimulationDataUpdatedFunction;

    void createArrays();
    void deleteArrays();

    SolverInterface * solverInterface(const QString &pSolverName) const;

    bool doIsModified(bool pCheckConstants) const;

signals:
    void dataUpdated(double pCurrentPoint);
    void dataModified(bool pIsModified);

    void pointUpdated();

    void error(const QString &pMessage);

public slots:
    void reload();

    const quint64 * delay() const;
    void setDelay(quint64 pDelay);

    double startingPoint() const;
    double endingPoint() const;
    double pointInterval() const;

    QString odeSolverName() const;
    QString nlaSolverName() const;

    Solver::Solver::Properties odeSolverProperties() const;
    Solver::Solver::Properties nlaSolverProperties() const;

    void addOdeSolverProperty(const QString &pName, const QVariant &pValue);
    void addNlaSolverProperty(const QString &pName, const QVariant &pValue,
                              bool pReset = true);

    void reset(bool pInitialize = true, bool pAll = true);

    void recomputeComputedConstantsAndVariables(double pCurrentPoint,
                                                bool pInitialize);
    void recomputeVariables(double pCurrentPoint);

    bool isStatesModified() const;
    bool isModified() const;

    void checkForModifications();

    void updateInitialValues();
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationResults : public SimulationObject
{
    Q_OBJECT

public:
    explicit SimulationResults(Simulation *pSimulation);
    ~SimulationResults() override;

    void importData(DataStore::DataStoreImportData *pImportData);

    bool addRun();

    void addPoint(double pPoint);

    double * points(int pRun = -1) const;

    double * constants(int pIndex, int pRun = -1) const;
    double * rates(int pIndex, int pRun = -1) const;
    double * states(int pIndex, int pRun = -1) const;
    double * algebraic(int pIndex, int pRun = -1) const;

    double * data(double *pData, int pIndex, int pRun = -1) const;

    DataStore::DataStoreVariable * pointsVariable() const;

    DataStore::DataStoreVariables constantsVariables() const;
    DataStore::DataStoreVariables ratesVariables() const;
    DataStore::DataStoreVariables statesVariables() const;
    DataStore::DataStoreVariables algebraicVariables() const;

private:
    DataStore::DataStore *mDataStore = nullptr;

    DataStore::DataStoreVariable *mPointsVariable = nullptr;

    DataStore::DataStoreVariables mConstantsVariables;
    DataStore::DataStoreVariables mRatesVariables;
    DataStore::DataStoreVariables mStatesVariables;
    DataStore::DataStoreVariables mAlgebraicVariables;

    QMap<double *, DataStore::DataStoreVariables> mData;
    QMap<double *, DataStore::DataStore *> mDataDataStores;

    void createDataStore();
    void deleteDataStore();

    QString uri(const QStringList &pComponentHierarchy, const QString &pName);

    double realPoint(double pPoint, int pRun = -1) const;

    double realValue(double pPoint, DataStore::DataStoreVariable *pVoi,
                     DataStore::DataStoreVariable *pVariable) const;

signals:
    void resultsReset();
    void runAdded();

public slots:
    void reload();

    void reset();

    int runsCount() const;

    quint64 size(int pRun = -1) const;

    OpenCOR::DataStore::DataStore * dataStore() const;
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationImportData : public SimulationObject
{
    Q_OBJECT

public:
    explicit SimulationImportData(Simulation *pSimulation);
    ~SimulationImportData() override;

    DataStore::DataStore * addDataStore();

private:
    QList<DataStore::DataStore *> mDataStores;
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT Simulation : public QObject
{
    Q_OBJECT

public:
    enum class FileType {
        Unknown,
        CellmlFile,
        SedmlFile,
        CombineArchive
    };

    explicit Simulation(const QString &pFileName);
    ~Simulation() override;

    SimulationIssues issues();

    QString furtherInitialize() const;

    CellMLSupport::CellmlFileRuntime * runtime() const;

    SimulationWorker * worker() const;

    Simulation::FileType fileType() const;

    CellMLSupport::CellmlFile * cellmlFile() const;
    SEDMLSupport::SedmlFile * sedmlFile() const;
    COMBINESupport::CombineArchive * combineArchive() const;

    SimulationImportData * importData() const;

    void importData(DataStore::DataStoreImportData *pImportData);

    bool addRun();

    void run();
    void pause();
    void resume();
    void stop();

    void reset(bool pAll = true);

private:
    QString mFileName;

    FileType mFileType = FileType::Unknown;

    CellMLSupport::CellmlFile *mCellmlFile = nullptr;
    SEDMLSupport::SedmlFile *mSedmlFile = nullptr;
    COMBINESupport::CombineArchive *mCombineArchive = nullptr;

    bool mNeedCheckIssues = true;
    SimulationIssues mIssues;
    bool mHasBlockingIssues = false;

    CellMLSupport::CellmlFileRuntime *mRuntime = nullptr;

    SimulationWorker *mWorker = nullptr;

    SimulationData *mData = nullptr;
    SimulationResults *mResults = nullptr;
    SimulationImportData *mImportData = nullptr;

    void checkIssues();

    void retrieveFileDetails(bool pRecreateRuntime = true);

    bool simulationSettingsOk(bool pEmitSignal = true);

    QString initializeSolver(const libsedml::SedListOfAlgorithmParameters *pSedmlAlgorithmParameters,
                             const QString &pKisaoId) const;

signals:
    void running(bool pIsResuming);
    void paused();

    void done(qint64 pElapsedTime);

    void error(const QString &pMessage);

public slots:
    QString fileName() const;

    bool hasBlockingIssues();

    void save();
    void reload();
    void rename(const QString &pFileName);

    OpenCOR::SimulationSupport::SimulationData * data() const;
    OpenCOR::SimulationSupport::SimulationResults * results() const;

    int runsCount() const;
    quint64 runSize(int pRun = -1) const;

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    const quint64 * delay() const;
    void setDelay(quint64 pDelay);

    quint64 size();

private slots:
    void fileManaged(const QString &pFileName);
};

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

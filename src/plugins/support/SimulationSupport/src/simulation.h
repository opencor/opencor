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
class SimulationWorker;

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
public:
    explicit SimulationObject(Simulation *pSimulation);
    ~SimulationObject() override;

    Simulation * simulation() const;

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

    void reload();

    double * constants() const;
    double * rates() const;
    double * states() const;
    double * algebraic() const;
    double * data(DataStore::DataStore *pDataStore) const;

    void importData(DataStore::DataStoreImportData *pImportData);

    const quint64 * delay() const;
    void setDelay(quint64 pDelay);

    double startingPoint() const;
    void setStartingPoint(double pStartingPoint, bool pRecompute = true);

    double endingPoint() const;
    void setEndingPoint(double pEndingPoint);

    double pointInterval() const;
    void setPointInterval(double pPointInterval);

    SolverInterface * odeSolverInterface() const;

    QString odeSolverName() const;
    void setOdeSolverName(const QString &pOdeSolverName);

    Solver::Solver::Properties odeSolverProperties() const;
    void addOdeSolverProperty(const QString &pName, const QVariant &pValue);

    SolverInterface * nlaSolverInterface() const;

    QString nlaSolverName() const;
    void setNlaSolverName(const QString &pNlaSolverName, bool pReset = true);

    Solver::Solver::Properties nlaSolverProperties() const;
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

    double *mConstants = nullptr;
    double *mRates = nullptr;
    double *mStates = nullptr;
    double *mDummyStates = nullptr;
    double *mAlgebraic = nullptr;

    double *mInitialConstants = nullptr;
    double *mInitialStates = nullptr;

    QMap<DataStore::DataStore *, double *> mData;

    void createArrays();
    void deleteArrays();

    SolverInterface * solverInterface(const QString &pSolverName) const;

    bool doIsModified(bool pCheckConstants) const;

signals:
    void updated(double pCurrentPoint);
    void modified(bool pIsModified);

    void error(const QString &pMessage);
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationResults : public SimulationObject
{
    Q_OBJECT

public:
    explicit SimulationResults(Simulation *pSimulation);
    ~SimulationResults() override;

    void reload();

    void reset();

    void importData(DataStore::DataStoreImportData *pImportData);

    int runsCount() const;

    bool addRun();

    void addPoint(double pPoint);

    quint64 size(int pRun = -1) const;

    DataStore::DataStore * dataStore() const;

    double * points(int pRun = -1) const;

    double * constants(int pIndex, int pRun = -1) const;
    double * rates(int pIndex, int pRun = -1) const;
    double * states(int pIndex, int pRun = -1) const;
    double * algebraic(int pIndex, int pRun = -1) const;
    double * data(double *pData, int pIndex, int pRun = -1) const;

private:
    DataStore::DataStore *mDataStore = nullptr;

    DataStore::DataStoreVariable *mPoints = nullptr;

    DataStore::DataStoreVariables mConstants;
    DataStore::DataStoreVariables mRates;
    DataStore::DataStoreVariables mStates;
    DataStore::DataStoreVariables mAlgebraic;

    QMap<double *, DataStore::DataStoreVariables> mData;
    QMap<double *, DataStore::DataStore *> mDataDataStores;

    void createDataStore();
    void deleteDataStore();

    QString uri(const QStringList &pComponentHierarchy, const QString &pName);

    double realPoint(double pPoint, int pRun = -1) const;

    double realValue(double pPoint, DataStore::DataStoreVariable *pVoi,
                     DataStore::DataStoreVariable *pVariable) const;
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

    QString fileName() const;

    void save();
    void reload();
    void rename(const QString &pFileName);

    SimulationIssues issues();
    bool hasBlockingIssues();

    CellMLSupport::CellmlFileRuntime * runtime() const;

    SimulationWorker * worker() const;

    Simulation::FileType fileType() const;

    CellMLSupport::CellmlFile * cellmlFile() const;
    SEDMLSupport::SedmlFile * sedmlFile() const;
    COMBINESupport::CombineArchive * combineArchive() const;

    SimulationData * data() const;
    SimulationResults * results() const;
    SimulationImportData * importData() const;

    void importData(DataStore::DataStoreImportData *pImportData);

    int runsCount() const;
    quint64 runSize(int pRun = -1) const;

    bool addRun();

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    const quint64 * delay() const;
    void setDelay(quint64 pDelay);

    quint64 size();

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

    SimulationData *mData;
    SimulationResults *mResults;
    SimulationImportData *mImportData;

    void checkIssues();

    void retrieveFileDetails(bool pRecreateRuntime = true);

    bool simulationSettingsOk(bool pEmitSignal = true);

signals:
    void running(bool pIsResuming);
    void paused();

    void done(qint64 pElapsedTime);

    void error(const QString &pMessage);

private slots:
    void fileManaged(const QString &pFileName);
};

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

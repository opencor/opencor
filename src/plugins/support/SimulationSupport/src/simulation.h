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

#include <QEventLoop>
#include <QVector>

//==============================================================================

#include <functional>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFile;
    class CellmlFileRuntime;
    class CellmlFileRuntimeParameter;
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

class SimulationSupportPythonWrapper;

//==============================================================================

class Simulation;
class SimulationData;
class SimulationResults;
class SimulationWorker;

//==============================================================================

// We bind the SimulationData object to the the first parameter of `updateParameters()`
// to create a function object to be called when simulation parameters are updated

#if defined(_MSC_VER)
    typedef std::_Binder<std::_Unforced, void (*)(SimulationData *), SimulationData *> SimulationDataUpdatedFunction;
#elif defined(__APPLE__)
    typedef std::__bind<void (*)(SimulationData *), SimulationData *> SimulationDataUpdatedFunction;
#else
    typedef std::_Bind_helper<false, void (*)(SimulationData *), SimulationData *>::type SimulationDataUpdatedFunction;
#endif

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationIssue
{
public:
    enum class Type {
        Unknown,
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

typedef QList<SimulationIssue> SimulationIssues;

//==============================================================================

class SimulationObject : public QObject
{
    Q_OBJECT

public:
    explicit SimulationObject(Simulation *pSimulation);
    ~SimulationObject() override;

public slots:
    Simulation * simulation() const;

protected:
    Simulation *mSimulation;
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationData : public SimulationObject
{
    Q_OBJECT

    friend class SimulationSupportPythonWrapper;

public:
    explicit SimulationData(Simulation *pSimulation);
    ~SimulationData() override;

    void setSimulationResults(SimulationResults *pSimulationResults);

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

    Solver::Solver::Properties odeSolverProperties() const;
    void addOdeSolverProperty(const QString &pName, const QVariant &pValue);

    SolverInterface * nlaSolverInterface() const;

    Solver::Solver::Properties nlaSolverProperties() const;
    void addNlaSolverProperty(const QString &pName, const QVariant &pValue,
                              bool pReset = true);

    int * gradientIndices();

    bool createGradientsArray();
    void deleteGradientsArray();

    double * gradients() const;

    int gradientsCount() const;
    int gradientsSize() const;

    static void updateParameters(SimulationData *pSimulationData);

public slots:
    void reload();

    const quint64 * delay() const;
    void setDelay(quint64 pDelay);

    double startingPoint() const;

    double endingPoint() const;

    double pointInterval() const;

    QString odeSolverName() const;
    void setOdeSolverName(const QString &pOdeSolverName);

    QString nlaSolverName() const;
    void setNlaSolverName(const QString &pNlaSolverName, bool pReset = true);

    void reset(bool pInitialize = true, bool pAll = true);

    void recomputeComputedConstantsAndVariables(double pCurrentPoint,
                                                bool pInitialize);
    void recomputeVariables(double pCurrentPoint);

    bool isStatesModified() const;
    bool isModified() const;
    void checkForModifications();

    void setGradientCalculation(const QString &pConstantUri, bool pCalculate = true);
    void setGradientCalculationByIndex(const int &pIndex, bool pCalculate);

    void updateInitialValues();

private:
    SimulationResults *mSimulationResults;

    quint64 mDelay;

    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;

    QString mOdeSolverName;
    Solver::Solver::Properties mOdeSolverProperties;

    QString mDaeSolverName;
    Solver::Solver::Properties mDaeSolverProperties;

    QString mNlaSolverName;
    Solver::Solver::Properties mNlaSolverProperties;

    DataStore::DataStoreArray *mConstantsArray;
    DataStore::DataStoreArray *mRatesArray;
    DataStore::DataStoreArray *mStatesArray;
    DataStore::DataStoreArray *mAlgebraicArray;

    DataStore::DataStoreValues *mConstantsValues;
    DataStore::DataStoreValues *mRatesValues;
    DataStore::DataStoreValues *mStatesValues;
    DataStore::DataStoreValues *mAlgebraicValues;

    double *mDummyStates;
    double *mInitialConstants;
    double *mInitialStates;

    QVector<int> mGradientIndices;
    DataStore::DataStoreArray *mGradientsArray;

    SimulationDataUpdatedFunction mSimulationDataUpdatedFunction;

    QMap<DataStore::DataStore *, double *> mData;

    void createArrays();
    void deleteArrays();

    SolverInterface * solverInterface(const QString &pSolverName) const;

    bool doIsModified(bool pCheckConstants) const;

signals:
    void updatedParameters(double pCurrentPoint);
    void updatedPointData();

    void modified(bool pIsModified);

    void gradientCalculation(CellMLSupport::CellmlFileRuntimeParameter *pParameter, const bool &pCalculate=true);

    void error(const QString &pMessage);
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationResults : public SimulationObject
{
    Q_OBJECT

    friend class SimulationSupportPythonWrapper;

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

    DataStore::DataStoreVariables constantVariables() const;
    DataStore::DataStoreVariables rateVariables() const;
    DataStore::DataStoreVariables stateVariables() const;
    DataStore::DataStoreVariables algebraicVariables() const;

    bool initialiseGradientsStore();

public slots:
    void reload();

    void reset();

    int runsCount() const;

    quint64 size(int pRun = -1) const;

    OpenCOR::DataStore::DataStore * dataStore() const;

private:
    DataStore::DataStore *mDataStore;

    DataStore::DataStoreVariable *mPoints;

    DataStore::DataStoreVariables mConstants;
    DataStore::DataStoreVariables mRates;
    DataStore::DataStoreVariables mStates;
    DataStore::DataStoreVariables mAlgebraic;

    DataStore::DataStore *mGradientsStore;
    DataStore::DataStoreVariables mGradients;

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
    ~SimulationImportData();

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

    void checkForIssues();
    bool hasBlockingIssues();
    SimulationIssues issues() const;

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

public slots:
    QString fileName() const;

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

private:
    QString mFileName;

    FileType mFileType;

    CellMLSupport::CellmlFile *mCellmlFile;
    SEDMLSupport::SedmlFile *mSedmlFile;
    COMBINESupport::CombineArchive *mCombineArchive;

    bool mHasBlockingIssues;
    SimulationIssues mIssues;

    CellMLSupport::CellmlFileRuntime *mRuntime;

    SimulationWorker *mWorker;

    SimulationData *mData;
    SimulationResults *mResults;
    SimulationImportData *mImportData;

    void retrieveFileDetails(bool pRecreateRuntime = true);

    bool simulationSettingsOk(bool pEmitSignal = true);

signals:
    void clearResults(const QString &pFileName);
    void runStarting(const QString &pFileName);

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

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

class QEventLoop;

//==============================================================================

#include <functional>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFile;
    class CellmlFileRuntime;
    class CellmlFileRuntimeParameter;
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
class SimulationData;
class SimulationResults;
class SimulationWorker;

//==============================================================================

// We bind the SimulationData object to the the first parameter of `updateParameters()`
// to create a function object to be called when simulation parameters are updated

#if defined(_MSC_VER)
    typedef std::_Binder<std::_Unforced, void (*)(SimulationData *), SimulationData * const> SimulationDataUpdatedFunction;
#elif defined(__APPLE__)
    typedef std::__bind<void (*)(SimulationData *), SimulationData *> SimulationDataUpdatedFunction;
#else
    typedef std::_Bind_helper<false, void (*)(SimulationData *), SimulationData *>::type SimulationDataUpdatedFunction;
#endif

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationData : public QObject
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

    OpenCOR::SimulationSupport::Simulation * simulation() const;

    int delay() const;
    void setDelay(int pDelay);

    double startingPoint() const;

    double endingPoint() const;

    double pointInterval() const;

    QString odeSolverName() const;
    void setOdeSolverName(const QString &pOdeSolverName);

    QString nlaSolverName() const;
    void setNlaSolverName(const QString &pNlaSolverName, bool pReset = true);

    void reset(bool pInitialize = true);

    void recomputeComputedConstantsAndVariables(double pCurrentPoint,
                                                bool pInitialize);
    void recomputeVariables(double pCurrentPoint);

    bool isModified() const;
    void checkForModifications();

    void setGradientCalculation(const QString &pConstantUri, bool pCalculate = true);
    void setGradientCalculationByIndex(const int &pIndex, bool pCalculate);

private:
    Simulation *mSimulation;
    SimulationResults *mSimulationResults;

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

    void createArrays();
    void deleteArrays();

    SolverInterface * solverInterface(const QString &pSolverName) const;

signals:
    void updatedParameters(double pCurrentPoint);
    void updatedPointData();

    void modified(bool pIsModified);

    void gradientCalculation(CellMLSupport::CellmlFileRuntimeParameter *pParameter, const bool &pCalculate=true);

    void error(const QString &pMessage);
};

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationResults : public QObject
{
    Q_OBJECT

    friend class SimulationSupportPythonWrapper;

public:
    explicit SimulationResults(Simulation *pSimulation);
    ~SimulationResults() override;

    bool addRun();

    void addPoint(double pPoint);

    double * points(int pRun = -1) const;

    double * constants(int pIndex, int pRun = -1) const;
    double * rates(int pIndex, int pRun = -1) const;
    double * states(int pIndex, int pRun = -1) const;
    double * algebraic(int pIndex, int pRun = -1) const;

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
    Simulation *mSimulation;

    DataStore::DataStore *mDataStore;

    DataStore::DataStoreVariable *mPoints;

    DataStore::DataStoreVariables mConstants;
    DataStore::DataStoreVariables mRates;
    DataStore::DataStoreVariables mStates;
    DataStore::DataStoreVariables mAlgebraic;

    DataStore::DataStore *mGradientsStore;
    DataStore::DataStoreVariables mGradients;

    void createDataStore();
    void deleteDataStore();

    QString uri(const QStringList &pComponentHierarchy, const QString &pName);
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
    ~Simulation() override;

    CellMLSupport::CellmlFileRuntime * runtime() const;

    SimulationWorker * worker() const;

    Simulation::FileType fileType() const;

    CellMLSupport::CellmlFile * cellmlFile() const;
    SEDMLSupport::SedmlFile * sedmlFile() const;
    COMBINESupport::CombineArchive * combineArchive() const;

    bool addRun();

    bool run();
    bool pause();
    bool resume();
    bool stop();

    bool reset();

public slots:
    QString fileName() const;

    void save();
    void reload();
    void rename(const QString &pFileName);

    OpenCOR::SimulationSupport::SimulationData * data() const;
    OpenCOR::SimulationSupport::SimulationResults * results() const;

    int runsCount() const;

    bool isRunning() const;
    bool isPaused() const;

    double currentPoint() const;

    int delay() const;
    void setDelay(int pDelay);

    double size();

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

    QEventLoop *mWorkerFinishedEventLoop;

    void retrieveFileDetails(bool pRecreateRuntime = true);

    bool simulationSettingsOk(bool pEmitSignal = true);

signals:
    void clearResults(const QString &pFileName);
    void runStarting(const QString &pFileName);

    void running(bool pIsResuming);
    void paused();
    void stopped(qint64 pElapsedTime);

    void error(const QString &pMessage);
};

//==============================================================================

}   // namespace SimulationSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

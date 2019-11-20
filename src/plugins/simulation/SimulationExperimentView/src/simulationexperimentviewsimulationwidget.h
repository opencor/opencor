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
// Simulation Experiment view simulation widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "graphpanelplotwidget.h"
#include "graphpanelwidget.h"
#include "solverinterface.h"
#include "widget.h"

//==============================================================================

class QFrame;
class QLabel;
class QMenu;
class QTextEdit;

//==============================================================================

class QwtWheel;

//==============================================================================

namespace libsedml {
    class SedAlgorithm;
    class SedAlgorithmParameter;
    class SedDocument;
    class SedListOfAlgorithmParameters;
    class SedModel;
    class SedRepeatedTask;
    class SedSimulation;
    class SedVariable;
} // namespace libsedml

//==============================================================================

namespace OpenCOR {

//==============================================================================

class DataStoreInterface;

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntimeParameter;
} // namespace CellMLSupport

//==============================================================================

namespace Core {
    class ProgressBarWidget;
    class Property;
    class PropertyEditorWidget;
    class SplitterWidget;
    class ToolBarWidget;
    class UserMessageWidget;
} // namespace Core

//==============================================================================

namespace DataStore {
    class DataStoreExportData;
    class DataStoreExporter;
    class DataStoreImportData;
    class DataStoreImporter;
} // namespace DataStore

//==============================================================================

namespace GraphPanelWidget {
    class GraphPanelWidget;
} // namespace GraphPanelWidget

//==============================================================================

namespace SEDMLSupport {
    class SedmlFile;
} // namespace SEDMLSupport

//==============================================================================

namespace SimulationSupport {
    class Simulation;
} // namespace SimulationSupport

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewContentsWidget;
class SimulationExperimentViewInformationSolversWidgetData;
class SimulationExperimentViewPlugin;
class SimulationExperimentViewWidget;

//==============================================================================

class SimulationExperimentViewSimulationWidget : public Core::Widget
{
    Q_OBJECT

public:
    enum class Task {
        None,
        ResetRuns,
        AddRun
    };

    explicit SimulationExperimentViewSimulationWidget(SimulationExperimentViewPlugin *pPlugin,
                                                      SimulationExperimentViewWidget *pViewWidget,
                                                      const QString &pFileName,
                                                      QWidget *pParent);
    ~SimulationExperimentViewSimulationWidget() override;

    void retranslateUi() override;

    void initialize(bool pReloadingView = false);
    void finalize();

    void setSizes(const QIntList &pSizes);

    SimulationExperimentViewContentsWidget * contentsWidget() const;

    QIcon fileTabIcon() const;

    bool import(const QString &pFileName, bool pShowWarning = true);
    bool save(const QString &pFileName);

    void filePermissionsChanged();
    void fileReloaded();

    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    SimulationSupport::Simulation * simulation() const;

    GraphPanelWidget::GraphPanelWidgetProperties defaultGraphPanelProperties() const;

    GraphPanelWidget::GraphPanelPlotGraphProperties defaultGraphProperties(const QString &pTitle,
                                                                           const QColor &pColor) const;
    GraphPanelWidget::GraphPanelPlotGraphProperties defaultGraphProperties(const QColor &pColor) const;

    void updateGui(bool pCheckVisibility = false);
    void updateSimulationResults(SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                 quint64 pSimulationResultsSize,
                                 int pSimulationRun, Task pTask);
    void updateSimulationResults(SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                 quint64 pSimulationResultsSize, Task pTask);

    void resetSimulationProgress();

protected:
    void changeEvent(QEvent *pEvent) override;
    void dragEnterEvent(QDragEnterEvent *pEvent) override;
    void dragMoveEvent(QDragMoveEvent *pEvent) override;
    void dropEvent(QDropEvent *pEvent) override;

private:
    enum class Error {
        General,
        InvalidCellmlFile,
        InvalidSimulationEnvironment
    };

    SimulationExperimentViewPlugin *mPlugin;

    SimulationExperimentViewWidget *mViewWidget;

    QMap<QAction *, DataStoreInterface *> mDataStoreInterfaces;

    QMap<QAction *, Plugin *> mCellmlBasedViewPlugins;

    SimulationSupport::Simulation *mSimulation;

    Core::ProgressBarWidget *mProgressBarWidget;

    int mProgress = -1;
    bool mLockedDevelopmentMode = false;

    Core::ToolBarWidget *mToolBarWidget;

    QMenu *mSimulationResultsExportDropDownMenu;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QAction *mRunPauseResumeSimulationAction;
    QAction *mStopSimulationAction;
    QAction *mResetStateModelParametersAction;
    QAction *mResetAllModelParametersAction;
    QAction *mClearSimulationResultsAction;
    QAction *mDevelopmentModeAction;
    QAction *mAddGraphPanelAction;
    QAction *mRemoveGraphPanelAction;
    QAction *mRemoveCurrentGraphPanelAction;
    QAction *mRemoveAllGraphPanelsAction;
    QAction *mCellmlOpenAction;
    QAction *mSedmlExportAction;
    QAction *mSedmlExportSedmlFileAction;
    QAction *mSedmlExportCombineArchiveAction;
    QAction *mDataImportAction;
    QAction *mLocalDataImportAction;
    QAction *mRemoteDataImportAction;
    QAction *mSimulationResultsExportAction;
    QAction *mPreferencesAction;

    QwtWheel *mDelayWidget;
    QLabel *mDelayValueWidget;

    Core::SplitterWidget *mSplitterWidget;

    SimulationExperimentViewContentsWidget *mContentsWidget;

    bool mRunActionEnabled = true;

    Core::UserMessageWidget *mInvalidModelMessageWidget;

    QTextEdit *mOutputWidget;
    QString mOutputMessage;

    Error mError = Error::General;

    bool mValidSimulationEnvironment = false;

    GraphPanelWidget::GraphPanelPlotWidgets mPlots;

    QVariantList mSimulationProperties;
    QVariantList mSolversProperties;
    QMap<Core::PropertyEditorWidget *, QVariantList> mGraphPanelProperties;
    QMap<Core::PropertyEditorWidget *, QVariantList> mGraphsProperties;

    bool mSimulationPropertiesModified = false;
    bool mSolversPropertiesModified = false;
    QMap<Core::PropertyEditorWidget *, bool>  mGraphPanelPropertiesModified;
    QMap<Core::PropertyEditorWidget *, bool>  mGraphsPropertiesModified;

    QIntList mGraphPanelsWidgetSizes;
    bool mGraphPanelsWidgetSizesModified = false;

    bool mCanUpdatePlotsForUpdatedGraphs = true;

    bool mNeedUpdatePlots = false;

    QMap<GraphPanelWidget::GraphPanelPlotGraph *, quint64> mOldDataSizes;

    QMap<QString, FileTypeInterface *> mFileTypeInterfaces;

    QString styledOutput();
    void output(const QString &pMessage);

    void updateSimulationMode();

    int tabBarPixmapSize() const;

    void updateRunPauseAction(bool pRunActionEnabled);

    void updateDataStoreActions();

    void updateInvalidModelMessageWidget();

    bool updatePlot(GraphPanelWidget::GraphPanelPlotWidget *pPlot,
                    bool pCanSetAxes, bool pForceReplot);

    double * data(SimulationSupport::Simulation *pSimulation,
                  CellMLSupport::CellmlFileRuntimeParameter *pParameter,
                  int pRun) const;

    void updateGraphData(GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                         quint64 pSize, int pRun = -1);

    void updateSimulationProperties(Core::Property *pProperty = nullptr);
    void updateSolversProperties(Core::Property *pProperty,
                                 bool pResetNlaSolver);
    void updateSolversProperties(Core::Property *pProperty);
    void updateSolversProperties(bool pResetNlaSolver);
    void updateSolversProperties();

    CellMLSupport::CellmlFileRuntimeParameter * runtimeParameter(libsedml::SedVariable *pSedmlVariable,
                                                                 QString &pCellmlComponent,
                                                                 QString &pCellmlVariable);
    bool isRuntimeDataParameter(const QString &pComponent,
                                const QString &pVariable);

    bool initializeSolver(const libsedml::SedListOfAlgorithmParameters *pSedmlAlgorithmParameters,
                          const QString &pKisaoId,
                          SimulationExperimentViewInformationSolversWidgetData *pSolverData);

    bool furtherInitialize();
    void initializeGui(bool pValidSimulationEnvironment);
    void initializeSimulation();

    void initializeTrackers(bool pInitialzeGraphPanelsWidgetSizes = false);

    QString fileName(const QString &pFileName, const QString &pBaseFileName,
                     const QString &pFileExtension, const QString &pCaption,
                     const QStringList &pFileFilters);

    void addSedmlSimulationAlgorithm(libsedml::SedAlgorithm *pAlgorithm,
                                     SolverInterface *pSolverInterface,
                                     const Solver::Solver::Properties &pSolverProperties);
    void addSedmlSimulation(libsedml::SedDocument *pSedmlDocument,
                            libsedml::SedModel *pSedmlModel,
                            libsedml::SedRepeatedTask *pSedmlRepeatedTask,
                            libsedml::SedSimulation *pSedmlSimulation,
                            int pOrder);
    void addSedmlVariableTarget(libsedml::SedVariable *pSedmlVariable,
                                const QString &pComponent,
                                const QString &pVariable);
    bool createSedmlFile(SEDMLSupport::SedmlFile *pSedmlFile,
                         const QString &pFileName, const QString &pModelSource);

    QVariantList allPropertyValues(Core::PropertyEditorWidget *pPropertyEditor) const;

    void updateSedmlFileOrCombineArchiveModifiedStatus();

    void simulationError(const QString &pMessage, Error pError);

    void sedmlExportSedmlFile(const QString &pFileName);
    void sedmlExportCombineArchive(const QString &pFileName);

    void dataImport(const QStringList &pFileNames);

signals:
    void splitterMoved(const QIntList &pSizes);

    void graphPanelSettingsRequested();
    void graphsSettingsRequested();

    void importDone(DataStore::DataStoreImporter *pDataStoreImporter);
    void exportDone(DataStore::DataStoreExporter *pDataStoreExporter);

public slots:
    void clearSimulationResults();

private slots:
    void runPauseResumeSimulation();
    void stopSimulation();
    void developmentMode();
    void addGraphPanel();
    void removeGraphPanel();
    void removeCurrentGraphPanel();
    void removeAllGraphPanels();
    void resetStateModelParameters();
    void resetAllModelParameters();
    void sedmlExportSedmlFile();
    void sedmlExportCombineArchive();
    void preferences();

    void emitSplitterMoved();

    void localDataImport();
    void remoteDataImport();

    void simulationResultsExport();

    void updateDelayValue(double pDelayValue);

    void simulationRunning(bool pIsResuming);
    void simulationPaused();

    void simulationDone(qint64 pElapsedTime);

    void resetProgressBar();
    void resetFileTabIcon();

    void simulationError(const QString &pMessage);

    void simulationDataModified(bool pIsModified);

    void simulationResultsReset();
    void simulationResultsRunAdded();

    void simulationPropertyChanged(Core::Property *pProperty);
    void solversPropertyChanged(Core::Property *pProperty);

    void graphPanelAdded(GraphPanelWidget::GraphPanelWidget *pGraphPanel);
    void graphPanelRemoved(GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                  CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

    void graphAdded(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    GraphPanelWidget::GraphPanelPlotGraph *pGraph);
    void graphsRemoved(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                       const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

    void graphsUpdated(const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);
    void graphUpdated(GraphPanelWidget::GraphPanelPlotGraph *pGraph);

    void openCellmlFile();

    void dataStoreImportProgress(DataStore::DataStoreImportData *pImportData,
                                 double pProgress);
    void dataStoreImportDone(DataStore::DataStoreImportData *pImportData,
                             const QString &pErrorMessage);

    void dataStoreExportProgress(DataStore::DataStoreExportData *pDataStoreData,
                                 double pProgress);
    void dataStoreExportDone(DataStore::DataStoreExportData *pDataStoreData,
                             const QString &pErrorMessage);

    void checkSimulationProperties();
    void checkSolversProperties();
    void checkGraphPanelsAndGraphs();

    void finalFurtherInitialize();

    void resetDataStoreImporterConnections(DataStore::DataStoreImporter *pDataStoreImporter);
    void resetDataStoreExporterConnections(DataStore::DataStoreExporter *pDataStoreExporter);
};

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

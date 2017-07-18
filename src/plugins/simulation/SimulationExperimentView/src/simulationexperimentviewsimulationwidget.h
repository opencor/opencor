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

#include "cellmlfileruntime.h"
#include "corecliutils.h"
#include "graphpanelplotwidget.h"
#include "sedmlfileissue.h"
#include "simulationexperimentviewglobal.h"
#include "simulationexperimentviewwidget.h"
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
    class SedDocument;
    class SedModel;
    class SedRepeatedTask;
    class SedSimulation;
    class SedVariable;
}   // namespace libsedml

//==============================================================================

namespace OpenCOR {

//==============================================================================

class DataStoreInterface;
class ViewInterface;

//==============================================================================

namespace COMBINESupport {
    class CombineArchive;
}   // namespace COMBINESupport

//==============================================================================

namespace Core {
    class Property;
    class ProgressBarWidget;
    class SplitterWidget;
    class ToolBarWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace GraphPanelWidget {
    class GraphPanelWidget;
}   // namespace GraphPanelWidget

//==============================================================================

namespace SEDMLSupport {
    class SedmlFile;
}   // namespace SEDMLSupport

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewContentsWidget;
class SimulationExperimentViewPlugin;
class SimulationExperimentViewSimulation;

//==============================================================================

class SIMULATIONEXPERIMENTVIEW_EXPORT SimulationExperimentViewSimulationWidget : public Core::Widget
{
    Q_OBJECT

public:
    enum FileType {
        CellmlFile,
        SedmlFile,
        CombineArchive
    };

    explicit SimulationExperimentViewSimulationWidget(SimulationExperimentViewPlugin *pPlugin,
                                                      SimulationExperimentViewWidget *pViewWidget,
                                                      const QString &pFileName,
                                                      QWidget *pParent);
    ~SimulationExperimentViewSimulationWidget();

    virtual void retranslateUi();

    void initialize(const bool &pReloadingView = false);
    void finalize();

    void setSizes(const QIntList &pSizes);

    SimulationExperimentViewContentsWidget * contentsWidget() const;

    QIcon fileTabIcon() const;

    bool save(const QString &pFileName);

    void filePermissionsChanged();
    void fileModified();
    void fileReloaded();

    QString fileName() const;
    void setFileName(const QString &pFileName);

    SEDMLSupport::SedmlFile * sedmlFile() const;

    FileType fileType() const;

    SimulationExperimentViewSimulation * simulation() const;

    void updateGui(const bool &pCheckVisibility = false);
    void updateSimulationResults(SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                 const qulonglong &pSimulationResultsSize,
                                 const bool &pClearGraphs);

    void resetSimulationProgress();

private:
    enum ErrorType {
        General,
        InvalidCellmlFile,
        InvalidSimulationEnvironment
    };

    SimulationExperimentViewPlugin *mPlugin;

    SimulationExperimentViewWidget *mViewWidget;

    QString mFileName;

    QMap<QAction *, DataStoreInterface *> mDataStoreInterfaces;

    QMap<QAction *, Plugin *> mCellmlBasedViewPlugins;

    SimulationExperimentViewSimulation *mSimulation;

    Core::ProgressBarWidget *mProgressBarWidget;

    int mProgress;
    bool mLockedDevelopmentMode;

    Core::ToolBarWidget *mToolBarWidget;

    QMenu *mSimulationDataExportDropDownMenu;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QAction *mRunPauseResumeSimulationAction;
    QAction *mStopSimulationAction;
    QAction *mResetModelParametersAction;
    QAction *mClearSimulationDataAction;
    QAction *mDevelopmentModeAction;
    QAction *mAddGraphPanelAction;
    QAction *mRemoveGraphPanelAction;
    QAction *mRemoveCurrentGraphPanelAction;
    QAction *mRemoveAllGraphPanelsAction;
    QAction *mCellmlOpenAction;
    QAction *mSedmlExportAction;
    QAction *mSedmlExportSedmlFileAction;
    QAction *mSedmlExportCombineArchiveAction;
    QAction *mSimulationDataExportAction;

    QwtWheel *mDelayWidget;
    QLabel *mDelayValueWidget;

    Core::SplitterWidget *mSplitterWidget;

    SimulationExperimentViewContentsWidget *mContentsWidget;

    bool mRunActionEnabled;

    Core::UserMessageWidget *mInvalidModelMessageWidget;

    QTextEdit *mOutputWidget;

    CellMLSupport::CellmlFile *mCellmlFile;
    SEDMLSupport::SedmlFile *mSedmlFile;
    COMBINESupport::CombineArchive *mCombineArchive;

    FileType mFileType;

    SEDMLSupport::SedmlFileIssues mSedmlFileIssues;
    COMBINESupport::CombineArchiveIssues mCombineArchiveIssues;

    ErrorType mErrorType;

    GraphPanelWidget::GraphPanelPlotWidgets mPlots;
    QMap<GraphPanelWidget::GraphPanelPlotWidget *, bool> mUpdatablePlotViewports;

    bool mCanUpdatePlotsForUpdatedGraphs;

    bool mNeedReloadView;

    bool mNeedUpdatePlots;

    QMap<GraphPanelWidget::GraphPanelPlotGraph *, qulonglong> mOldDataSizes;

    QMap<QString, QString> mLocallyManagedCellmlFiles;

    void reloadView();

    void output(const QString &pMessage);

    void updateSimulationMode();

    int tabBarPixmapSize() const;

    void updateRunPauseAction(const bool &pRunActionEnabled);

    void updateDataStoreActions();

    void updateInvalidModelMessageWidget();

    void checkAxisValue(double &pValue, const double &pOrigValue,
                        const QList<double> &pTestValues);

    bool updatePlot(GraphPanelWidget::GraphPanelPlotWidget *pPlot,
                    const bool &pForceReplot = false);

    double * dataPoints(SimulationExperimentViewSimulation *pSimulation,
                        CellMLSupport::CellmlFileRuntimeParameter *pParameter) const;

    void updateGraphData(GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                         const qulonglong &pSize);

    QVariant value(Core::Property *pProperty) const;

    void updateSimulationProperties(Core::Property *pProperty = 0);
    void updateSolversProperties(Core::Property *pProperty = 0);

    CellMLSupport::CellmlFileRuntimeParameter * runtimeParameter(libsedml::SedVariable *pSedmlVariable);

    bool doFurtherInitialize();
    void initializeGui(const bool &pValidSimulationEnvironment);
    void initializeSimulation();

    void addSedmlSimulation(libsedml::SedDocument *pSedmlDocument,
                            libsedml::SedModel *pSedmlModel,
                            libsedml::SedRepeatedTask *pSedmlRepeatedTask,
                            libsedml::SedSimulation *pSedmlSimulation,
                            const int &pOrder);
    void addSedmlVariableTarget(libsedml::SedVariable *pSedmlVariable,
                                const QString &pComponent,
                                const QString &pVariable);
    bool createSedmlFile(const QString &pFileName, const QString &pModelSource);

    void checkSimulationDataModified(const bool &pIsModified);

    bool sedmlAlgorithmSupported(const libsedml::SedAlgorithm *pSedmlAlgorithm,
                                 SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const;
    bool sedmlFileSupported(SEDMLSupport::SedmlFile *pSedmlFile,
                            SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const;

    bool combineArchiveSupported(COMBINESupport::CombineArchive *pCombineArchive,
                                 COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues) const;

    void retrieveCellmlFile(const QString &pFileName,
                            CellMLSupport::CellmlFile *&pCellmlFile,
                            SEDMLSupport::SedmlFile *pSedmlFile,
                            COMBINESupport::CombineArchive *pCombineArchive,
                            const FileType &pFileType,
                            SEDMLSupport::SedmlFileIssues &pSedmlFileIssues);
    void retrieveSedmlFile(SEDMLSupport::SedmlFile *&pSedmlFile,
                           COMBINESupport::CombineArchive *pCombineArchive,
                           COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues);

    void retrieveFileDetails(const QString &pFileName,
                             CellMLSupport::CellmlFile *&pCellmlFile,
                             SEDMLSupport::SedmlFile *&pSedmlFile,
                             COMBINESupport::CombineArchive *&pCombineArchive,
                             FileType &pFileType,
                             SEDMLSupport::SedmlFileIssues &pSedmlFileIssues,
                             COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues);

signals:
    void splitterMoved(const QIntList &pSizes);

private slots:
    void runPauseResumeSimulation();
    void stopSimulation();
    void developmentMode();
    void addGraphPanel();
    void removeGraphPanel();
    void removeCurrentGraphPanel();
    void removeAllGraphPanels();
    void resetModelParameters();
    void clearSimulationData();
    void sedmlExportSedmlFile();
    void sedmlExportCombineArchive();

    void furtherInitialize();

    void emitSplitterMoved();

    void simulationDataExport();

    void updateDelayValue(const double &pDelayValue);

    void simulationRunning(const bool &pIsResuming);
    void simulationPaused();
    void simulationStopped(const qint64 &pElapsedTime);

    void resetProgressBar();
    void resetFileTabIcon();

    void simulationError(const QString &pMessage,
                         const ErrorType &pErrorType = General);

    void simulationDataModified(const bool &pIsModified);

    void simulationPropertyChanged(Core::Property *pProperty);
    void solversPropertyChanged(Core::Property *pProperty);

    void graphPanelAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                         const bool &pActive);
    void graphPanelRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                  CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

    void graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph);
    void graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                       const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

    void graphsUpdated(OpenCOR::GraphPanelWidget::GraphPanelPlotWidget *pPlot,
                       const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

    void openCellmlFile();

    void plotAxesChanged();

    void dataStoreExportDone(const QString &pErrorMessage);
    void dataStoreExportProgress(const double &pProgress);
};

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

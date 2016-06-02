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
// Single cell view simulation widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfileruntime.h"
#include "corecliutils.h"
#include "sedmlfileissue.h"
#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewwidget.h"
#include "widget.h"

//==============================================================================

class QFrame;
class QLabel;
class QMenu;
class QSplitter;
class QTextEdit;

//==============================================================================

class QwtWheel;

//==============================================================================

namespace Ui {
    class SingleCellViewSimulationWidget;
}

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
    class ToolBarWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace SEDMLSupport {
    class SedmlFile;
}   // namespace SEDMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewContentsWidget;
class SingleCellViewGraphPanelWidget;
class SingleCellViewPlugin;
class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewSimulationWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationWidget(SingleCellViewPlugin *pPlugin,
                                            const QString &pFileName,
                                            QWidget *pParent);
    ~SingleCellViewSimulationWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const bool &pReloadingView = false);
    void finalize();

    void setSizes(const QIntList &pSizes);

    SingleCellViewContentsWidget * contentsWidget() const;

    QIcon fileTabIcon() const;

    bool save(const QString &pFileName);

    void filePermissionsChanged();
    void fileModified();
    void fileReloaded();

    QString fileName() const;
    void setFileName(const QString &pFileName);

    SEDMLSupport::SedmlFile * sedmlFile() const;

    SingleCellViewWidget::FileType fileType() const;

    SingleCellViewSimulation *simulation() const;

    void updateGui();
    void updateSimulationResults(SingleCellViewSimulationWidget *pSimulationWidget,
                                 const qulonglong &pSimulationResultsSize,
                                 const bool &pClearGraphs);

    void resetSimulationProgress();

    static QIcon parameterIcon(const CellMLSupport::CellmlFileRuntimeParameter::ParameterType &pParameterType);

private:
    enum ErrorType {
        General,
        InvalidCellmlFile,
        InvalidSimulationEnvironment
    };

    Ui::SingleCellViewSimulationWidget *mGui;

    SingleCellViewPlugin *mPlugin;

    QString mFileName;

    QMap<QAction *, DataStoreInterface *> mDataStoreInterfaces;

    QMap<QAction *, Plugin *> mCellmlEditingViewPlugins;

    SingleCellViewSimulation *mSimulation;

    Core::ProgressBarWidget *mProgressBarWidget;

    int mProgress;
    bool mLockedDevelopmentMode;

    Core::ToolBarWidget *mToolBarWidget;

    QMenu *mSimulationDataExportDropDownMenu;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QwtWheel *mDelayWidget;
    QLabel *mDelayValueWidget;

    QSplitter *mSplitterWidget;

    SingleCellViewContentsWidget *mContentsWidget;

    bool mRunActionEnabled;

    Core::UserMessageWidget *mInvalidModelMessageWidget;

    QTextEdit *mOutputWidget;

    CellMLSupport::CellmlFile *mCellmlFile;
    SEDMLSupport::SedmlFile *mSedmlFile;
    COMBINESupport::CombineArchive *mCombineArchive;

    SingleCellViewWidget::FileType mFileType;

    SEDMLSupport::SedmlFileIssues mSedmlFileIssues;
    COMBINESupport::CombineArchiveIssues mCombineArchiveIssues;

    ErrorType mErrorType;

    QMap<SingleCellViewGraphPanelWidget *, SingleCellViewGraphPanelPlotWidget *> mGraphPanelsPlots;
    SingleCellViewGraphPanelPlotWidgets mPlots;

    bool mCanUpdatePlotsForUpdatedGraphs;

    bool mNeedReloadView;

    bool mNeedUpdatePlots;

    QMap<SingleCellViewGraphPanelPlotGraph *, qulonglong> mOldDataSizes;

    void reloadView();

    void output(const QString &pMessage);

    void updateSimulationMode();

    int tabBarPixmapSize() const;

    void updateRunPauseAction(const bool &pRunActionEnabled);

    void updateDataStoreActions();

    void updateInvalidModelMessageWidget();

    void checkAxisValue(double &pValue, const double &pOrigValue,
                        const QList<double> &pTestValues);

    bool updatePlot(SingleCellViewGraphPanelPlotWidget *pPlot,
                    const bool &pForceReplot = false);

    double * dataPoints(SingleCellViewSimulation *pSimulation,
                        CellMLSupport::CellmlFileRuntimeParameter *pParameter) const;

    void updateGraphData(SingleCellViewGraphPanelPlotGraph *pGraph,
                         const qulonglong &pSize);

    QVariant value(Core::Property *pProperty) const;

    void updateSimulationProperties(OpenCOR::Core::Property *pProperty = 0);
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

Q_SIGNALS:
    void splitterMoved(const QIntList &pSizes);

private Q_SLOTS:
    void on_actionRunPauseResumeSimulation_triggered();
    void on_actionStopSimulation_triggered();

    void on_actionResetModelParameters_triggered();
    void on_actionClearSimulationData_triggered();

    void on_actionDevelopmentMode_triggered();

    void on_actionAddGraphPanel_triggered();

    void on_actionRemoveGraphPanel_triggered();
    void on_actionRemoveCurrentGraphPanel_triggered();
    void on_actionRemoveAllGraphPanels_triggered();

    void on_actionSedmlExportSedmlFile_triggered();
    void on_actionSedmlExportCombineArchive_triggered();

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

    void graphPanelAdded(SingleCellViewGraphPanelWidget *pGraphPanel,
                         const bool &pActive);
    void graphPanelRemoved(SingleCellViewGraphPanelWidget *pGraphPanel);

    void addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                  CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

    void graphAdded(SingleCellViewGraphPanelWidget *pGraphPanel,
                    SingleCellViewGraphPanelPlotGraph *pGraph);
    void graphsRemoved(SingleCellViewGraphPanelWidget *pGraphPanel,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);

    void graphsUpdated(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);

    void openCellmlFile();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

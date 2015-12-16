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
// Single cell view widget
//==============================================================================

#ifndef SINGLECELLVIEWWIDGET_H
#define SINGLECELLVIEWWIDGET_H

//==============================================================================

#include "cellmlfileruntime.h"
#include "corecliutils.h"
#include "datastoreinterface.h"
#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewsimulation.h"
#include "solverinterface.h"
#include "viewwidget.h"

//==============================================================================

class QFrame;
class QLabel;
class QMenu;
class QSettings;
class QSplitter;
class QTextEdit;

//==============================================================================

class QwtWheel;

//==============================================================================

namespace Ui {
    class SingleCellViewWidget;
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

namespace Core {
    class Property;
    class ProgressBarWidget;
    class ToolBarWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntimeParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewContentsWidget;
class SingleCellViewGraphPanelWidget;
class SingleCellViewInformationSolversWidgetData;
class SingleCellViewPlugin;

//==============================================================================

class SingleCellViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewWidget(SingleCellViewPlugin *pPlugin,
                                  QWidget *pParent);
    ~SingleCellViewWidget();

    virtual void retranslateUi();

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);
    void setDataStoreInterfaces(const DataStoreInterfaces &pDataStoreInterfaces);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName,
                    const bool &pReloadingView = false);
    void finalize(const QString &pFileName, const bool &pReloadingView = false);

    QIcon fileTabIcon(const QString &pFileName) const;

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName);

    void fileOpened(const QString &pFileName);
    void filePermissionsChanged(const QString &pFileName);
    void fileModified(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileClosed(const QString &pFileName);

    static QIcon parameterIcon(const CellMLSupport::CellmlFileRuntimeParameter::ParameterType &pParameterType);

private:
    enum ErrorType {
        General,
        InvalidCellmlFile,
        InvalidSimulationEnvironment
    };

    Ui::SingleCellViewWidget *mGui;

    SingleCellViewPlugin *mPlugin;

    SolverInterfaces mSolverInterfaces;

    QMap<QAction *, DataStoreInterface *> mDataStoreInterfaces;

    SingleCellViewSimulation *mSimulation;
    QMap<QString, SingleCellViewSimulation *> mSimulations;

    SingleCellViewSimulations mStoppedSimulations;

    Core::ProgressBarWidget *mProgressBarWidget;

    QMap<QString, int> mProgresses;
    QMap<QString, bool> mResets;
    QMap<QString, int> mDelays;
    QMap<QString, bool> mDevelopmentModes;
    QMap<QString, bool> mLockedDevelopmentModes;

    Core::ToolBarWidget *mToolBarWidget;

    QMenu *mSimulationDataExportDropDownMenu;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QwtWheel *mDelayWidget;
    QLabel *mDelayValueWidget;

    QSplitter *mSplitterWidget;
    QIntList mSplitterWidgetSizes;

    SingleCellViewContentsWidget *mContentsWidget;

    bool mRunActionEnabled;

    Core::UserMessageWidget *mInvalidModelMessageWidget;

    QTextEdit *mOutputWidget;

    ErrorType mErrorType;

    QMap<SingleCellViewSimulation *, qulonglong> mOldSimulationResultsSizes;
    SingleCellViewSimulations mCheckResultsSimulations;

    SingleCellViewSimulations mResetSimulations;

    QMap<SingleCellViewGraphPanelWidget *, SingleCellViewGraphPanelPlotWidget *> mGraphPanelsPlots;
    SingleCellViewGraphPanelPlotWidgets mPlots;
    QMap<SingleCellViewGraphPanelPlotWidget *, QRectF> mPlotsViewports;

    bool mCanUpdatePlotsForUpdatedGraphs;

    QList<QString> mNeedReloadViews;

    void reloadView(const QString &pFileName);

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

    void updateResults(SingleCellViewSimulation *pSimulation,
                       const qulonglong &pSize);
    void checkResults(SingleCellViewSimulation *pSimulation,
                      const bool &pForceUpdateResults = false);

    QVariant value(Core::Property *pProperty) const;

    void updateSimulationProperties(OpenCOR::Core::Property *pProperty = 0);
    void updateSolversProperties(Core::Property *pProperty = 0);

    void addSedmlSimulation(libsedml::SedDocument *pSedmlDocument,
                            libsedml::SedModel *pSedmlModel,
                            libsedml::SedRepeatedTask *pSedmlRepeatedTask,
                            libsedml::SedSimulation *pSedmlSimulation,
                            const int &pOrder);
    void addSedmlVariableTarget(libsedml::SedVariable *pSedmlVariable,
                                const QString &pComponent,
                                const QString &pVariable);
    void createSedmlFile(const QString &pFileName, const QString &pModelSource);

    void checkSimulationDataModified(const bool &pCurrentSimulation,
                                     const QString &pFileName,
                                     const bool &pIsModified);

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

    void simulationDataExport();

    void updateDelayValue(const double &pDelayValue);

    void simulationRunning(const bool &pIsResuming);
    void simulationPaused();
    void simulationStopped(const qint64 &pElapsedTime);

    void resetProgressBar(SingleCellViewSimulation *pSimulation = 0);
    void resetFileTabIcon(SingleCellViewSimulation *pSimulation = 0);

    void simulationError(const QString &pMessage,
                         const ErrorType &pErrorType = General);

    void simulationDataModified(const bool &pIsModified);

    void splitterWidgetMoved();

    void simulationPropertyChanged(Core::Property *pProperty);
    void solversPropertyChanged(Core::Property *pProperty);

    void graphPanelAdded(SingleCellViewGraphPanelWidget *pGraphPanel);
    void graphPanelRemoved(SingleCellViewGraphPanelWidget *pGraphPanel);

    void addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                  CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

    void graphAdded(SingleCellViewGraphPanelPlotWidget *pPlot,
                    SingleCellViewGraphPanelPlotGraph *pGraph);
    void graphsRemoved(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);

    void graphsUpdated(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);

    void callCheckResults();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

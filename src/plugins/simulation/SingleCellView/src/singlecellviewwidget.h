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
#include "solverinterface.h"
#include "viewwidget.h"

//==============================================================================

class QFrame;
class QLabel;
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
class SingleCellViewGraphPanelPlotGraph;
class SingleCellViewGraphPanelPlotWidget;
class SingleCellViewGraphPanelWidget;
class SingleCellViewPlugin;
class SingleCellViewSimulation;

//==============================================================================

class SingleCellViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewWidget(SingleCellViewPlugin *pPluginParent,
                                  QWidget *pParent);
    ~SingleCellViewWidget();

    virtual void retranslateUi();

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName,
                    const bool &pReloadingView = false);
    void finalize(const QString &pFileName, const bool &pReloadingView = false);

    QIcon fileTabIcon(const QString &pFileName) const;

    void fileOpened(const QString &pFileName);
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

    SingleCellViewPlugin *mPluginParent;

    SolverInterfaces mSolverInterfaces;

    SingleCellViewSimulation *mSimulation;
    QMap<QString, SingleCellViewSimulation *> mSimulations;

    QList<SingleCellViewSimulation *> mStoppedSimulations;

    Core::ProgressBarWidget *mProgressBarWidget;

    QMap<QString, int> mProgresses;

    QMap<QString, bool> mResets;
    QMap<QString, int> mDelays;

    Core::ToolBarWidget *mToolBarWidget;

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
    QList<SingleCellViewSimulation *> mCheckResultsSimulations;

    QMap<SingleCellViewGraphPanelWidget *, SingleCellViewGraphPanelPlotWidget *> mGraphPanelsPlots;
    QList<SingleCellViewGraphPanelPlotWidget *> mPlots;
    QMap<SingleCellViewGraphPanelPlotWidget *, QRectF> mPlotsViewports;

    bool mCanUpdatePlotsForUpdatedGraphs;

    QList<QString> mNeedReloadViews;

    void reloadView(const QString &pFileName);

    void output(const QString &pMessage);

    void updateSimulationMode();

    int tabBarPixmapSize() const;

    void updateRunPauseAction(const bool &pRunActionEnabled);

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
    void checkResults(SingleCellViewSimulation *pSimulation);

    void resetFileTabIcon(const QString &pFileName,
                          const bool &pRemoveProgress = true);

    QVariant value(Core::Property *pProperty) const;

private Q_SLOTS:
    void on_actionRunPauseResumeSimulation_triggered();
    void on_actionStopSimulation_triggered();

    void on_actionResetModelParameters_triggered();
    void on_actionClearSimulationData_triggered();

    void on_actionDebugMode_triggered();

    void on_actionAddGraphPanel_triggered();

    void on_actionRemoveGraphPanel_triggered();
    void on_actionRemoveCurrentGraphPanel_triggered();
    void on_actionRemoveAllGraphPanels_triggered();

    void on_actionSimulationDataCsvExport_triggered();

    void updateDelayValue(const double &pDelayValue);

    void simulationRunning(const bool &pIsResuming);
    void simulationPaused();
    void simulationStopped(const qint64 &pElapsedTime);

    void resetProgressBar();
    void resetFileTabIcon();

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
                       const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs);

    void graphsUpdated(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs);

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

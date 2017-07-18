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

#include "cellmlfilemanager.h"
#include "centralwidget.h"
#include "combinefilemanager.h"
#include "combineinterface.h"
#include "combinesupportplugin.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "graphpanelswidget.h"
#include "interfaces.h"
#include "progressbarwidget.h"
#include "sedmlfilemanager.h"
#include "sedmlinterface.h"
#include "sedmlsupportplugin.h"
#include "simulationexperimentviewcontentswidget.h"
#include "simulationexperimentviewinformationgraphswidget.h"
#include "simulationexperimentviewinformationparameterswidget.h"
#include "simulationexperimentviewinformationsimulationwidget.h"
#include "simulationexperimentviewinformationsolverswidget.h"
#include "simulationexperimentviewinformationwidget.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewsimulation.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"
#include "toolbarwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QTextEdit>
#include <QTimer>
#include <QToolButton>

//==============================================================================

#include "qwt_wheel.h"

//==============================================================================

#include "sbmlapibegin.h"
    #include "sbml/math/FormulaParser.h"
#include "sbmlapiend.h"

//==============================================================================

#include "sedmlapibegin.h"
    #include "sedml/SedAlgorithm.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedOneStep.h"
    #include "sedml/SedPlot2D.h"
    #include "sedml/SedRepeatedTask.h"
    #include "sedml/SedSimulation.h"
    #include "sedml/SedUniformTimeCourse.h"
    #include "sedml/SedVectorRange.h"
#include "sedmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewSimulationWidget::SimulationExperimentViewSimulationWidget(SimulationExperimentViewPlugin *pPlugin,
                                                                                   SimulationExperimentViewWidget *pViewWidget,
                                                                                   const QString &pFileName,
                                                                                   QWidget *pParent) :
    Widget(pParent),
    mPlugin(pPlugin),
    mViewWidget(pViewWidget),
    mFileName(pFileName),
    mDataStoreInterfaces(QMap<QAction *, DataStoreInterface *>()),
    mCellmlBasedViewPlugins(QMap<QAction *, Plugin *>()),
    mProgress(-1),
    mLockedDevelopmentMode(false),
    mRunActionEnabled(true),
    mCellmlFile(0),
    mSedmlFile(0),
    mCombineArchive(0),
    mFileType(CellmlFile),
    mSedmlFileIssues(SEDMLSupport::SedmlFileIssues()),
    mCombineArchiveIssues(COMBINESupport::CombineArchiveIssues()),
    mErrorType(General),
    mPlots(GraphPanelWidget::GraphPanelPlotWidgets()),
    mUpdatablePlotViewports(QMap<GraphPanelWidget::GraphPanelPlotWidget *, bool>()),
    mCanUpdatePlotsForUpdatedGraphs(true),
    mNeedReloadView(false),
    mNeedUpdatePlots(false),
    mOldDataSizes(QMap<GraphPanelWidget::GraphPanelPlotGraph *, qulonglong>()),
    mLocallyManagedCellmlFiles(QMap<QString, QString>())
{
    // Create a tool bar

    mToolBarWidget = new Core::ToolBarWidget(this);

    // Create and handle various actions

    mRunPauseResumeSimulationAction = Core::newAction(QIcon(":/oxygen/actions/media-playback-start.png"),
                                                      Qt::Key_F9, mToolBarWidget);
    mStopSimulationAction = Core::newAction(QIcon(":/oxygen/actions/media-playback-stop.png"),
                                            QKeySequence(Qt::CTRL|Qt::Key_F2), mToolBarWidget);
    mResetModelParametersAction = Core::newAction(QIcon(":/oxygen/actions/view-refresh.png"),
                                                  mToolBarWidget);
    mClearSimulationDataAction = Core::newAction(QIcon(":/oxygen/actions/trash-empty.png"),
                                                 mToolBarWidget);
    mDevelopmentModeAction = Core::newAction(true, QIcon(":/oxygen/actions/run-build-configure.png"),
                                             mToolBarWidget);
    mAddGraphPanelAction = Core::newAction(QIcon(":/oxygen/actions/list-add.png"),
                                           mToolBarWidget);
    mRemoveGraphPanelAction = Core::newAction(QIcon(":/oxygen/actions/list-remove.png"),
                                              mToolBarWidget);
    mRemoveCurrentGraphPanelAction = Core::newAction(mToolBarWidget);
    mRemoveAllGraphPanelsAction = Core::newAction(mToolBarWidget);
    mCellmlOpenAction = Core::newAction(QIcon(":/CellMLSupport/logo.png"),
                                        mToolBarWidget);
    mSedmlExportAction = Core::newAction(QIcon(":/SEDMLSupport/logo.png"),
                                         mToolBarWidget);
    mSedmlExportSedmlFileAction = Core::newAction(mToolBarWidget);
    mSedmlExportCombineArchiveAction = Core::newAction(mToolBarWidget);
    mSimulationDataExportAction = Core::newAction(QIcon(":/oxygen/actions/document-export.png"),
                                                  mToolBarWidget);

    connect(mRunPauseResumeSimulationAction, SIGNAL(triggered(bool)),
            this, SLOT(runPauseResumeSimulation()));
    connect(mStopSimulationAction, SIGNAL(triggered(bool)),
            this, SLOT(stopSimulation()));
    connect(mResetModelParametersAction, SIGNAL(triggered(bool)),
            this, SLOT(resetModelParameters()));
    connect(mClearSimulationDataAction, SIGNAL(triggered(bool)),
            this, SLOT(clearSimulationData()));
    connect(mDevelopmentModeAction, SIGNAL(triggered(bool)),
            this, SLOT(developmentMode()));
    connect(mAddGraphPanelAction, SIGNAL(triggered(bool)),
            this, SLOT(addGraphPanel()));
    connect(mRemoveGraphPanelAction, SIGNAL(triggered(bool)),
            this, SLOT(removeGraphPanel()));
    connect(mRemoveCurrentGraphPanelAction, SIGNAL(triggered(bool)),
            this, SLOT(removeCurrentGraphPanel()));
    connect(mRemoveAllGraphPanelsAction, SIGNAL(triggered(bool)),
            this, SLOT(removeAllGraphPanels()));
    connect(mCellmlOpenAction, SIGNAL(triggered(bool)),
            this, SLOT(openCellmlFile()));
    connect(mSedmlExportAction, SIGNAL(triggered(bool)),
            this, SLOT(sedmlExportSedmlFile()));
    connect(mSedmlExportSedmlFileAction, SIGNAL(triggered(bool)),
            this, SLOT(sedmlExportSedmlFile()));
    connect(mSedmlExportCombineArchiveAction, SIGNAL(triggered(bool)),
            this, SLOT(sedmlExportCombineArchive()));

    // Create a wheel (and a label to show its value) to specify the delay (in
    // milliseconds) between the output of two data points

    mDelayWidget = new QwtWheel(mToolBarWidget);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    QWidget *delaySpaceWidget = new QWidget(mToolBarWidget);
#endif
    mDelayValueWidget = new QLabel(mToolBarWidget);

    mDelayWidget->setBorderWidth(0);
    mDelayWidget->setFixedSize(0.07*qApp->desktop()->screenGeometry().width(),
                               0.5*mDelayWidget->height());
    mDelayWidget->setFocusPolicy(Qt::NoFocus);
    mDelayWidget->setRange(0.0, 55.0);
    mDelayWidget->setWheelBorderWidth(0);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    delaySpaceWidget->setFixedWidth(4);
#endif

    connect(mDelayWidget, SIGNAL(valueChanged(double)),
            this, SLOT(updateDelayValue(const double &)));

    mDelayWidget->setValue(0.0);

    // Create various tool buttons

    QToolButton *removeGraphPanelToolButton = new QToolButton(mToolBarWidget);
    QMenu *removeGraphPanelDropDownMenu = new QMenu(removeGraphPanelToolButton);

    removeGraphPanelDropDownMenu->addAction(mRemoveCurrentGraphPanelAction);
    removeGraphPanelDropDownMenu->addAction(mRemoveAllGraphPanelsAction);

    removeGraphPanelToolButton->setDefaultAction(mRemoveGraphPanelAction);
    removeGraphPanelToolButton->setMenu(removeGraphPanelDropDownMenu);
    removeGraphPanelToolButton->setPopupMode(QToolButton::MenuButtonPopup);

    QToolButton *cellmlOpenToolButton = new QToolButton(mToolBarWidget);
    QMenu *cellmlOpenDropDownMenu = new QMenu(cellmlOpenToolButton);

    cellmlOpenToolButton->setDefaultAction(mCellmlOpenAction);
    cellmlOpenToolButton->setMenu(cellmlOpenDropDownMenu);
    cellmlOpenToolButton->setPopupMode(QToolButton::MenuButtonPopup);

    foreach (Plugin *cellmlEditingViewPlugin, pViewWidget->cellmlEditingViewPlugins()) {
        QAction *action = Core::newAction(Core::mainWindow());

        cellmlOpenDropDownMenu->addAction(action);

        mCellmlBasedViewPlugins.insert(action, cellmlEditingViewPlugin);

        connect(action, SIGNAL(triggered(bool)),
                this, SLOT(openCellmlFile()));
    }

    cellmlOpenDropDownMenu->addSeparator();

    foreach (Plugin *cellmlSimulationViewPlugin, pViewWidget->cellmlSimulationViewPlugins()) {
        QAction *action = Core::newAction(Core::mainWindow());

        cellmlOpenDropDownMenu->addAction(action);

        mCellmlBasedViewPlugins.insert(action, cellmlSimulationViewPlugin);

        connect(action, SIGNAL(triggered(bool)),
                this, SLOT(openCellmlFile()));
    }

    QToolButton *sedmlExportToolButton = new QToolButton(mToolBarWidget);
    QMenu *sedmlExportDropDownMenu = new QMenu(sedmlExportToolButton);

    sedmlExportToolButton->setDefaultAction(mSedmlExportAction);
    sedmlExportToolButton->setMenu(sedmlExportDropDownMenu);
    sedmlExportToolButton->setPopupMode(QToolButton::MenuButtonPopup);

    sedmlExportDropDownMenu->addAction(mSedmlExportSedmlFileAction);
    sedmlExportDropDownMenu->addAction(mSedmlExportCombineArchiveAction);

    QToolButton *simulationDataExportToolButton = new QToolButton(mToolBarWidget);

    mSimulationDataExportDropDownMenu = new QMenu(simulationDataExportToolButton);

    simulationDataExportToolButton->setDefaultAction(mSimulationDataExportAction);
    simulationDataExportToolButton->setMenu(mSimulationDataExportDropDownMenu);
    simulationDataExportToolButton->setPopupMode(QToolButton::InstantPopup);

    foreach (DataStoreInterface *dataStoreInterface, Core::dataStoreInterfaces()) {
        QString dataStoreName = dataStoreInterface->dataStoreName();
        QAction *action = mSimulationDataExportDropDownMenu->addAction(dataStoreName+"...");

        mDataStoreInterfaces.insert(action, dataStoreInterface);

        connect(action, SIGNAL(triggered()),
                this, SLOT(simulationDataExport()));
    }

    updateDataStoreActions();

    // Add the various actions, wheel and tool buttons to our tool bar

    mToolBarWidget->addAction(mRunPauseResumeSimulationAction);
    mToolBarWidget->addAction(mStopSimulationAction);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mResetModelParametersAction);
    mToolBarWidget->addAction(mClearSimulationDataAction);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addWidget(mDelayWidget);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mToolBarWidget->addWidget(delaySpaceWidget);
#endif
    mToolBarWidget->addWidget(mDelayValueWidget);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mDevelopmentModeAction);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mAddGraphPanelAction);
    mToolBarWidget->addWidget(removeGraphPanelToolButton);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addWidget(cellmlOpenToolButton);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addWidget(sedmlExportToolButton);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addWidget(simulationDataExportToolButton);

    mTopSeparator = Core::newLineWidget(this);

    QLayout *layout = createLayout();

    layout->addWidget(mToolBarWidget);
    layout->addWidget(mTopSeparator);

    // Create and add our invalid simulation message widget

    mInvalidModelMessageWidget = new Core::UserMessageWidget(":/oxygen/actions/help-about.png", this);

    layout->addWidget(mInvalidModelMessageWidget);

    // Create our splitter widget and keep track of its movement
    // Note: we need to keep track of its movement so that saveSettings() can
    //       work fine even when mContentsWidget is not visible (which happens
    //       when a CellML file cannot be run for some reason or another)...

    mSplitterWidget = new Core::SplitterWidget(Qt::Vertical, this);

    connect(mSplitterWidget, SIGNAL(splitterMoved(int, int)),
            this, SLOT(emitSplitterMoved()));

    // Create our contents widget

    mContentsWidget = new SimulationExperimentViewContentsWidget(pViewWidget, this, this);

    mContentsWidget->setObjectName("Contents");

    // Keep track of changes to some of our simulation and solvers properties

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();

    connect(informationWidget->simulationWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(simulationPropertyChanged(Core::Property *)));
    connect(informationWidget->solversWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(solversPropertyChanged(Core::Property *)));

    // Keep track of whether we can remove graph panels

    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();

    connect(graphPanelsWidget, SIGNAL(removeGraphPanelsEnabled(const bool &)),
            mRemoveGraphPanelAction, SLOT(setEnabled(bool)));

    // Keep track of the addition and removal of a graph panel

    SimulationExperimentViewInformationGraphsWidget *graphsWidget = informationWidget->graphsWidget();

    connect(graphPanelsWidget, SIGNAL(graphPanelAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const bool &)),
            graphsWidget, SLOT(initialize(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const bool &)));
    connect(graphPanelsWidget, SIGNAL(graphPanelRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *)),
            graphsWidget, SLOT(finalize(OpenCOR::GraphPanelWidget::GraphPanelWidget *)));

    connect(graphPanelsWidget, SIGNAL(graphPanelAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const bool &)),
            this, SLOT(graphPanelAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const bool &)));
    connect(graphPanelsWidget, SIGNAL(graphPanelRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *)),
            this, SLOT(graphPanelRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *)));

    // Keep track of whether a graph panel has been activated

    connect(graphPanelsWidget, SIGNAL(graphPanelActivated(OpenCOR::GraphPanelWidget::GraphPanelWidget *)),
            graphsWidget, SLOT(initialize(OpenCOR::GraphPanelWidget::GraphPanelWidget *)));

    // Keep track of a graph being required

    connect(informationWidget->parametersWidget(), SIGNAL(graphRequired(CellMLSupport::CellmlFileRuntimeParameter *, CellMLSupport::CellmlFileRuntimeParameter *)),
            this, SLOT(addGraph(CellMLSupport::CellmlFileRuntimeParameter *, CellMLSupport::CellmlFileRuntimeParameter *)));

    // Keep track of the addition and removal of a graph

    connect(graphPanelsWidget, SIGNAL(graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *, OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *)),
            graphsWidget, SLOT(addGraph(OpenCOR::GraphPanelWidget::GraphPanelWidget *, OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *)));
    connect(graphPanelsWidget, SIGNAL(graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &)),
            graphsWidget, SLOT(removeGraphs(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &)));

    connect(graphPanelsWidget, SIGNAL(graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *, OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *)),
            this, SLOT(graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *, OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *)));
    connect(graphPanelsWidget, SIGNAL(graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &)),
            this, SLOT(graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &)));

    // Keep track of the updating of a graph
    // Note: ideally, this would, as for the addition and removal of a graph
    //       (see above), be done through graphPanelsWidget (i.e. a graph would
    //       let people know that it has been updated and the graph panel with
    //       which it is associated would forward the signal to
    //       graphPanelsWidget), but this may result in too many graphsUpdated()
    //       signals being emitted. For example, say that you change the model
    //       with which a graph is associated, then both the X and Y parameters
    //       will get updated, and for each of those updates a graphsUpdated()
    //       signal would be emitted by the graph, hence we would end up with
    //       two signals when only one would have sufficed. Even worse is that
    //       after having updated the X parameter, the graph would have its X
    //       parameter coming from the 'new' model while its Y parameter from
    //       the 'old' model, which could mess things up quite a bit from a
    //       plotting viewpoint. So, instead, the updating is done through our
    //       graphs property editor...

    connect(graphsWidget, SIGNAL(graphsUpdated(OpenCOR::GraphPanelWidget::GraphPanelPlotWidget *, const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &)),
            this, SLOT(graphsUpdated(OpenCOR::GraphPanelWidget::GraphPanelPlotWidget *, const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &)));

    // Create our simulation output widget with a layout on which we put a
    // separating line and our simulation output list view
    // Note: the separating line is because we remove, for aesthetical reasons,
    //       the border of our simulation output list view...

    QWidget *simulationOutputWidget = new QWidget(this);
    QVBoxLayout *simulationOutputLayout= new QVBoxLayout(simulationOutputWidget);

    simulationOutputLayout->setContentsMargins(QMargins());
    simulationOutputLayout->setSpacing(0);

    simulationOutputWidget->setLayout(simulationOutputLayout);

    mOutputWidget = new QTextEdit(this);

    mOutputWidget->setAcceptDrops(false);
    mOutputWidget->setFrameStyle(QFrame::NoFrame);
    mOutputWidget->setReadOnly(true);

    simulationOutputLayout->addWidget(Core::newLineWidget(this));
    simulationOutputLayout->addWidget(mOutputWidget);

    // Populate our splitter and use as much space as possible for it by asking
    // for its height to be that of the desktop's, and then add our splitter to
    // our Simulation Experiment view widget

    mSplitterWidget->addWidget(mContentsWidget);
    mSplitterWidget->addWidget(simulationOutputWidget);

    mSplitterWidget->setSizes(QIntList() << qApp->desktop()->screenGeometry().height() << 1);

    layout->addWidget(mSplitterWidget);

    // Create our (thin) simulation progress widget

    mBottomSeparator = Core::newLineWidget(this);
    mProgressBarWidget = new Core::ProgressBarWidget(this);

    mProgressBarWidget->setFixedHeight(3);
    mProgressBarWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    layout->addWidget(mBottomSeparator);
    layout->addWidget(mProgressBarWidget);

    // Make our contents widget our focus proxy

    setFocusProxy(mContentsWidget);

    // Create our simulation object and a few connections for it, after having
    // retrieved our file details

    retrieveFileDetails(pFileName, mCellmlFile, mSedmlFile, mCombineArchive,
                        mFileType, mSedmlFileIssues, mCombineArchiveIssues);

    mSimulation = new SimulationExperimentViewSimulation(mCellmlFile?mCellmlFile->runtime(true):0);

    connect(mSimulation, SIGNAL(running(const bool &)),
            this, SLOT(simulationRunning(const bool &)));
    connect(mSimulation, SIGNAL(paused()),
            this, SLOT(simulationPaused()));
    connect(mSimulation, SIGNAL(stopped(const qint64 &)),
            this, SLOT(simulationStopped(const qint64 &)));

    connect(mSimulation, SIGNAL(error(const QString &)),
            this, SLOT(simulationError(const QString &)));

    connect(mSimulation->data(), SIGNAL(modified(const bool &)),
            this, SLOT(simulationDataModified(const bool &)));

    // Enable/disable our development mode action depending on whether our file
    // is readable/writable and of CellML type

    mDevelopmentModeAction->setEnabled(   Core::FileManager::instance()->isReadableAndWritable(pFileName)
                                       && (mFileType == CellmlFile));

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

SimulationExperimentViewSimulationWidget::~SimulationExperimentViewSimulationWidget()
{
    // Delete some internal objects

    delete mSimulation;

    if (mFileType != CellmlFile)
        delete mCellmlFile;

    if (mFileType != SedmlFile)
        delete mSedmlFile;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mRunPauseResumeSimulationAction, tr("Run Simulation"),
                                     tr("Run the simulation"));
    I18nInterface::retranslateAction(mStopSimulationAction, tr("Stop Simulation"),
                                     tr("Stop the simulation"));
    I18nInterface::retranslateAction(mResetModelParametersAction, tr("Reset Model Parameters"),
                                     tr("Reset all the model parameters"));
    I18nInterface::retranslateAction(mClearSimulationDataAction, tr("Clear Simulation Data"),
                                     tr("Clear the simulation data"));
    I18nInterface::retranslateAction(mDevelopmentModeAction, tr("Development Mode"),
                                     tr("Enable/disable the development mode"));
    I18nInterface::retranslateAction(mAddGraphPanelAction, tr("Add Graph Panel"),
                                     tr("Add a graph panel"));
    I18nInterface::retranslateAction(mRemoveGraphPanelAction, tr("Remove Graph Panel"),
                                     tr("Remove the current graph panel or all the graph panels"));
    I18nInterface::retranslateAction(mRemoveCurrentGraphPanelAction, tr("Current"),
                                     tr("Remove the current graph panel"));
    I18nInterface::retranslateAction(mRemoveAllGraphPanelsAction, tr("All"),
                                     tr("Remove all the graph panels"));
    I18nInterface::retranslateAction(mCellmlOpenAction, tr("Referenced CellML File"),
                                     tr("Open the referenced CellML file"));
    I18nInterface::retranslateAction(mSedmlExportAction, tr("SED-ML Export"),
                                     tr("Export the simulation to SED-ML"));
    I18nInterface::retranslateAction(mSedmlExportSedmlFileAction, tr("SED-ML File..."),
                                     tr("Export the simulation to SED-ML using a SED-ML file"));
    I18nInterface::retranslateAction(mSedmlExportCombineArchiveAction, tr("COMBINE Archive..."),
                                     tr("Export the simulation to SED-ML using a COMBINE archive"));
    I18nInterface::retranslateAction(mSimulationDataExportAction, tr("Simulation Data Export"),
                                     tr("Export the simulation data"));

    // Retranslate our delay and delay value widgets

    updateDelayValue(mDelayWidget->value());
    // Note: we do this because we want to display the delay using digit
    //       grouping, this respecting the current locale...

    mDelayWidget->setToolTip(tr("Simulation Delay"));
    mDelayValueWidget->setToolTip(mDelayWidget->toolTip());

    mDelayWidget->setStatusTip(tr("Delay between two data points"));
    mDelayValueWidget->setStatusTip(mDelayWidget->statusTip());

    // Retranslate our run/pause action

    updateRunPauseAction(mRunActionEnabled);

    // Retranslate our data store actions

    updateDataStoreActions();

    // Retranslate our CellML editing view actions

    foreach (QAction *cellmlEditingViewAction, mCellmlBasedViewPlugins.keys()) {
        Plugin *plugin = mCellmlBasedViewPlugins.value(cellmlEditingViewAction);
        QString viewName = qobject_cast<ViewInterface *>(plugin->instance())->viewName();

        I18nInterface::retranslateAction(cellmlEditingViewAction,
                                         tr("%1 View").arg(viewName),
                                         tr("Open the referenced CellML file using the %1 view").arg(viewName));
    }

    // Retranslate our invalid model message

    updateInvalidModelMessageWidget();

    // Retranslate our contents widget

    mContentsWidget->retranslateUi();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateDataStoreActions()
{
    // Update our data store actions

    foreach (QAction *action, mDataStoreInterfaces.keys()) {
        I18nInterface::retranslateAction(action,
                                         action->text(),
                                         tr("Export the simulation data to %1").arg(mDataStoreInterfaces.value(action)->dataStoreName()));
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::output(const QString &pMessage)
{
    // Move to the end of the output
    // Note: this is just in case the user clicked somewhere in the output and
    //       we are therefore not at the end of it anymore...

    mOutputWidget->moveCursor(QTextCursor::End);

    // Output the message and make sure that it's visible

    mOutputWidget->insertHtml(pMessage);
    mOutputWidget->ensureCursorVisible();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSimulationMode()
{
    // Update our run/pause action

    updateRunPauseAction(!mSimulation->isRunning() || mSimulation->isPaused());

    // Enable/disable our stop action

    bool simulationModeEnabled = mSimulation->isRunning() || mSimulation->isPaused();

    mStopSimulationAction->setEnabled(simulationModeEnabled);

    // Enable or disable our simulation and solvers widgets

    mContentsWidget->informationWidget()->simulationWidget()->setEnabled(!simulationModeEnabled);
    mContentsWidget->informationWidget()->solversWidget()->setEnabled(!simulationModeEnabled);

    // Enable/disable some actions

    mClearSimulationDataAction->setEnabled(    mSimulation->results()->size()
                                           && !simulationModeEnabled);
    mSimulationDataExportAction->setEnabled(    mSimulationDataExportDropDownMenu->actions().count()
                                            &&  mSimulation->results()->size()
                                            && !simulationModeEnabled);
    mCellmlOpenAction->setEnabled(mFileType != CellmlFile);
    mSedmlExportAction->setEnabled(    (mFileType == CellmlFile)
                                   &&  mSimulation->results()->size()
                                   && !simulationModeEnabled);

    // Give the focus to our focus proxy, in case we leave our simulation mode
    // (so that the user can modify simulation data, etc.)

    if (!simulationModeEnabled)
        focusProxy()->setFocus();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateRunPauseAction(const bool &pRunActionEnabled)
{
    // Update our various actions

    static const QIcon StartIcon = QIcon(":/oxygen/actions/media-playback-start.png");
    static const QIcon PauseIcon = QIcon(":/oxygen/actions/media-playback-pause.png");

    mRunActionEnabled = pRunActionEnabled;

    mRunPauseResumeSimulationAction->setIcon(pRunActionEnabled?StartIcon:PauseIcon);

    bool simulationPaused = mSimulation && mSimulation->isPaused();

    I18nInterface::retranslateAction(mRunPauseResumeSimulationAction,
                                     pRunActionEnabled?
                                         simulationPaused?
                                             tr("Resume Simulation"):
                                             tr("Run Simulation"):
                                         tr("Pause Simulation"),
                                     pRunActionEnabled?
                                         simulationPaused?
                                             tr("Resume the simulation"):
                                             tr("Run the simulation"):
                                         tr("Pause the simulation"));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateInvalidModelMessageWidget()
{
    // Update our invalid model message

    mInvalidModelMessageWidget->setMessage((mErrorType == InvalidCellmlFile)?
                                               tr("The <strong>%1</strong> view requires a valid CellML file to work...").arg(mPlugin->viewName()):
                                               tr("The <strong>%1</strong> view requires a valid simulation environment to work...").arg(mPlugin->viewName()),
                                           tr("See below for more information."));
}

//==============================================================================

static const auto OutputTab  = QStringLiteral("&nbsp;&nbsp;&nbsp;&nbsp;");
static const auto OutputGood = QStringLiteral(" style=\"color: green;\"");
static const auto OutputInfo = QStringLiteral(" style=\"color: navy;\"");
static const auto OutputBad  = QStringLiteral(" style=\"color: maroon;\"");
static const auto OutputBrLn = QStringLiteral("<br/>\n");

//==============================================================================

void SimulationExperimentViewSimulationWidget::initialize(const bool &pReloadingView)
{
    // Stop keeping track of certain things (so that updatePlot() doesn't get
    // called unnecessarily)
    // Note: see the corresponding code towards the end of this method...

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
    SimulationExperimentViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();

    disconnect(simulationWidget, SIGNAL(propertyChanged(Core::Property *)),
               this, SLOT(simulationPropertyChanged(Core::Property *)));

    // Reset our progress

    mProgress = -1;

    // Retrieve our file details and update our simulation object, if needed

    if (pReloadingView) {
        retrieveFileDetails(mFileName, mCellmlFile, mSedmlFile, mCombineArchive,
                            mFileType, mSedmlFileIssues, mCombineArchiveIssues);
    }

    CellMLSupport::CellmlFileRuntime *runtime = mCellmlFile?mCellmlFile->runtime(pReloadingView):0;

    if (pReloadingView)
        mSimulation->update(runtime);

    // Retrieve our variable of integration, if possible

    bool validRuntime = runtime && runtime->isValid();

    CellMLSupport::CellmlFileRuntimeParameter *variableOfIntegration = validRuntime?runtime->variableOfIntegration():0;

    // Clean up our output, if needed

    if (pReloadingView)
        mOutputWidget->document()->clear();

    // Output some information about our CellML file

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QString fileName = fileManagerInstance->isNew(mFileName)?
                           tr("File")+" #"+QString::number(fileManagerInstance->newIndex(mFileName)):
                           fileManagerInstance->isRemote(mFileName)?
                               fileManagerInstance->url(mFileName):
                               mFileName;
    QString information =  "<strong>"+fileName+"</strong>"+OutputBrLn;

    if (!mCombineArchiveIssues.isEmpty()) {
        // There is one or several issues with our COMBINE archive, so list
        // it/them

        foreach (const COMBINESupport::CombineArchiveIssue &combineArchiveIssue, mCombineArchiveIssues) {
            QString issueType;

            switch (combineArchiveIssue.type()) {
            case COMBINESupport::CombineArchiveIssue::Information:
                issueType = tr("Information:");

                break;
            case COMBINESupport::CombineArchiveIssue::Error:
                issueType = tr("Error:");

                break;
            case COMBINESupport::CombineArchiveIssue::Warning:
                issueType = tr("Warning:");

                break;
            case COMBINESupport::CombineArchiveIssue::Fatal:
                issueType = tr("Fatal:");

                break;
            }

            information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg(issueType, combineArchiveIssue.message());
        }
    } else if (!mSedmlFileIssues.isEmpty()) {
        // There is one or several issues with our SED-ML file, so list it/them

        foreach (const SEDMLSupport::SedmlFileIssue &sedmlFileIssue, mSedmlFileIssues) {
            QString issueType;

            switch (sedmlFileIssue.type()) {
            case SEDMLSupport::SedmlFileIssue::Information:
                issueType = tr("Information:");

                break;
            case SEDMLSupport::SedmlFileIssue::Error:
                issueType = tr("Error:");

                break;
            case SEDMLSupport::SedmlFileIssue::Warning:
                issueType = tr("Warning:");

                break;
            case SEDMLSupport::SedmlFileIssue::Fatal:
                issueType = tr("Fatal:");

                break;
            }

            information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg(issueType, Core::formatMessage(sedmlFileIssue.message()));
        }
    } else {
        information += OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

        if (variableOfIntegration) {
            // A variable of integration could be retrieved for our CellML file,
            // so we can also output the model type

            QString additionalInformation = QString();

            if (runtime->needNlaSolver())
                additionalInformation = "+"+tr("NLA system(s)");

            information += "<span"+OutputGood+">"+tr("valid")+"</span>."+OutputBrLn;
            information += QString(OutputTab+"<strong>"+tr("Model type:")+"</strong> <span"+OutputInfo+">%1%2</span>."+OutputBrLn).arg((runtime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?tr("ODE"):tr("DAE"),
                                                                                                                                       additionalInformation);
        } else {
            // We couldn't retrieve a variable of integration, which means that
            // we either don't have a runtime or we have one, but it's not valid
            // or it's valid but we really don't have a variable of integration
            // Note: in the case of a valid runtime and no variable of
            //       integration, we really shouldn't consider the runtime to be
            //       valid, hence we handle this case here...

            mErrorType = InvalidCellmlFile;

            updateInvalidModelMessageWidget();

            information += "<span"+OutputBad+">"+(runtime?tr("invalid"):tr("none"))+"</span>."+OutputBrLn;

            if (validRuntime) {
                // We have a valid runtime, but no variable of integration,
                // which means that the model doesn't contain any ODE or DAE

                information += OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+tr("the model must have at least one ODE or DAE")+".</span>"+OutputBrLn;
            } else {
                // We don't have a valid runtime, so either there are some
                // problems with the CellML file or its runtime

                foreach (const CellMLSupport::CellmlFileIssue &issue,
                         runtime?runtime->issues():mCellmlFile->issues()) {
                    information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?tr("Error:"):tr("Warning:"),
                                                                                                                         issue.message());
                }
            }
        }
    }

    output(information);

    // Enable/disable our run/pause action depending on whether we have a
    // variable of integration

    mRunPauseResumeSimulationAction->setEnabled(variableOfIntegration);

    // Update our simulation mode or clear our simulation data (should there be
    // some) in case we are reloading ourselves
    // Note: to clear our simualtion data will also update our simulation
    //       mode, so we are fine...

    if (pReloadingView)
        clearSimulationData();
    else
        updateSimulationMode();

    // Initialise our contents widget and make sure that we have the required
    // type(s) of solvers

    bool validSimulationEnvironment = false;
    SimulationExperimentViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();

    if (variableOfIntegration) {
        // Show our contents widget in case it got previously hidden
        // Note: indeed, if it was to remain hidden then some initialisations
        //       wouldn't work (e.g. the solvers widget has a property editor,
        //       which all properties need to be removed and if the contents
        //       widget is not visible, then upon repopulating the property
        //       editor, scrollbars will be shown even though they are not
        //       needed)...

        mContentsWidget->setVisible(true);

        // Check whether we have at least one ODE or DAE solver and, if needed,
        // at least one NLA solver

        if (runtime->needNlaSolver()) {
            if (solversWidget->nlaSolvers().isEmpty()) {
                if (runtime->needOdeSolver()) {
                    if (solversWidget->odeSolvers().isEmpty()) {
                        simulationError(tr("the model needs both an ODE and an NLA solver, but none are available"),
                                        InvalidSimulationEnvironment);
                    } else {
                        simulationError(tr("the model needs both an ODE and an NLA solver, but no NLA solver is available"),
                                        InvalidSimulationEnvironment);
                    }
                } else {
                    if (solversWidget->daeSolvers().isEmpty()) {
                        simulationError(tr("the model needs both a DAE and an NLA solver, but none are available"),
                                        InvalidSimulationEnvironment);
                    } else {
                        simulationError(tr("the model needs both a DAE and an NLA solver, but no NLA solver is available"),
                                        InvalidSimulationEnvironment);
                    }
                }
            } else if (   runtime->needOdeSolver()
                       && solversWidget->odeSolvers().isEmpty()) {
                simulationError(tr("the model needs both an ODE and an NLA solver, but no ODE solver is available"),
                                InvalidSimulationEnvironment);
            } else if (   runtime->needDaeSolver()
                       && solversWidget->daeSolvers().isEmpty()) {
                    simulationError(tr("the model needs both a DAE and an NLA solver, but no DAE solver is available"),
                                    InvalidSimulationEnvironment);
            } else {
                validSimulationEnvironment = true;
            }
        } else if (   runtime->needOdeSolver()
                   && solversWidget->odeSolvers().isEmpty()) {
            simulationError(tr("the model needs an ODE solver, but none is available"),
                            InvalidSimulationEnvironment);
        } else if (   runtime->needDaeSolver()
                   && solversWidget->daeSolvers().isEmpty()) {
            simulationError(tr("the model needs a DAE solver, but none is available"),
                            InvalidSimulationEnvironment);
        } else {
            validSimulationEnvironment = true;
        }
    }

    // Initialise our GUI based on whether we have a valid simulation
    // environment

    initializeGui(validSimulationEnvironment);

    // Some additional initialisations in case we have a valid simulation
    // environment

    if (validSimulationEnvironment) {
        // Initialise our GUI's simulation, solvers, graphs and graph panels
        // widgets, but not parameters widget
        // Note #1: this will also initialise some of our simulation data (i.e.
        //          our simulation's starting point and simulation's NLA
        //          solver's properties), which is needed since we want to be
        //          able to reset our simulation below...
        // Note #2: to initialise our graphs widget will result in some graphs
        //          being shown/hidden and, therefore, in graphsUpdated() being
        //          called. Yet, we don't want graphsUpdated() to update our
        //          plots. Indeed, if it did, then all of our plots' axes'
        //          values would be reset while we want to keep the ones we just
        //          retrieved (thus making it possible for the user to have
        //          different views for different files). So, for this to work
        //          we use mCanUpdatePlotsForUpdatedGraphs, and then 'manually'
        //          replot our plots, once we know which graphs are to be
        //          shown/hidden. We could do the initialisation before the
        //          setting of the plots' axes' values, but then we could see
        //          the graphs being plotted twice. Once after the plots' axes'
        //          values have been reset following the call to graphsUpdated()
        //          and another after we update our plots' axes' values. This is
        //          clearly not neat, hence the current solution...
        // Note #3: to initialise our parameters widget now would result in some
        //          default (in the computer science sense, i.e. wrong) values
        //          being visible for a split second before they get properly
        //          updated. So, instead, we initialise whatever needs
        //          initialising (e.g. NLA solver) so that we can safely compute
        //          our model parameters before showing their values...

        simulationWidget->initialize(mSimulation);
        solversWidget->initialize(mSimulation);

        mCanUpdatePlotsForUpdatedGraphs = false;
            informationWidget->graphsWidget()->initialize(mSimulation);
        mCanUpdatePlotsForUpdatedGraphs = true;

        mContentsWidget->graphPanelsWidget()->initialize();

        // Initialise our simulation

        initializeSimulation();

        // Now, we can safely update our parameters widget since our model
        // parameters have been computed

        mContentsWidget->informationWidget()->parametersWidget()->initialize(mSimulation, pReloadingView);
    }

    // Resume the tracking of certain things
    // Note: see the corresponding code at the beginning of this method...

    connect(mContentsWidget->informationWidget()->simulationWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(simulationPropertyChanged(Core::Property *)));

    // Further initialise ourselves, if we have a valid environment and we are
    // not dealing with a CellML file
    // Note: to further initialise ourselves involves, among other things,
    //       removing/adding graph panels. However, to do those things directly
    //       will result in the GUI flashing because of various events still
    //       having to be handled (e.g. see the above call to
    //       mContentsWidget->graphPanelsWidget()->initialize()). So, instead,
    //       we do the further initialisation through a single shot, ensuring
    //       that all the other events have been properly handled...

    if (    validSimulationEnvironment
        && (mFileType != CellmlFile)) {
        QTimer::singleShot(0, this, SLOT(furtherInitialize()));
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::finalize()
{
    // Finalize/backup a few things in our GUI's solvers, graphs, parameters and
    // graph panels widgets

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();

    informationWidget->graphsWidget()->finalize();
    informationWidget->parametersWidget()->finalize();

    // Then, we ask our file manager to stop managing our locally managed CellML
    // file, if any

    QString cellmlFileName = mLocallyManagedCellmlFiles.value(mFileName);

    if (!cellmlFileName.isEmpty()) {
        Core::FileManager::instance()->unmanage(cellmlFileName);

        mLocallyManagedCellmlFiles.remove(mFileName);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::setSizes(const QIntList &pSizes)
{
    // Set the sizes of our spliter widget, but only if there effectively are
    // some

    if (pSizes.count())
        mSplitterWidget->setSizes(pSizes);
}

//==============================================================================

SimulationExperimentViewContentsWidget * SimulationExperimentViewSimulationWidget::contentsWidget() const
{
    // Return our contents widget

    return mContentsWidget;
}

//==============================================================================

int SimulationExperimentViewSimulationWidget::tabBarPixmapSize() const
{
    // Return the size of a file tab icon

    return style()->pixelMetric(QStyle::PM_TabBarIconSize, 0, this);
}

//==============================================================================

QIcon SimulationExperimentViewSimulationWidget::fileTabIcon() const
{
    // Return a file tab icon that shows the given file's simulation progress

    if (!isVisible() && (mProgress != -1)) {
        // Create an image that shows the progress of our simulation

        QPixmap tabBarPixmap = QPixmap(tabBarPixmapSize(),
                                       mProgressBarWidget->height()+2);
        QPainter tabBarPixmapPainter(&tabBarPixmap);

        tabBarPixmapPainter.setBrush(QBrush(Core::windowColor()));
        tabBarPixmapPainter.setPen(QPen(Core::borderColor()));

        tabBarPixmapPainter.drawRect(0, 0, tabBarPixmap.width()-1, tabBarPixmap.height()-1);
        tabBarPixmapPainter.fillRect(1, 1, mProgress, tabBarPixmap.height()-2,
                                    Core::highlightColor());

        return QIcon(tabBarPixmap);
    } else {
        // No simulation object currently exists for the model, so return a null
        // icon

        static const QIcon NoIcon = QIcon();

        return NoIcon;
    }
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::save(const QString &pFileName)
{
    // In case of a CellML file that is in development mode, retrieve all the
    // state and constant parameters which value has changed and update our
    // CellML object with their 'new' values, unless they are imported, in which
    // case we let the user know that their 'new' values cannot be saved

    QString importedParameters = QString();

    if (   (mFileType == CellmlFile)
        && mDevelopmentModeAction->isChecked()) {
        ObjRef<iface::cellml_api::CellMLComponentSet> components = mCellmlFile->model()->localComponents();
        QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> parameters = mContentsWidget->informationWidget()->parametersWidget()->parameters();

        foreach (Core::Property *property, parameters.keys()) {
            CellMLSupport::CellmlFileRuntimeParameter *parameter = parameters.value(property);

            if (   (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::State)
                || (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Constant)) {
                ObjRef<iface::cellml_api::CellMLComponent> component = components->getComponent(parameter->componentHierarchy().last().toStdWString());
                ObjRef<iface::cellml_api::CellMLVariableSet>  variables = component->variables();
                ObjRef<iface::cellml_api::CellMLVariable> variable = variables->getVariable(property->name().toStdWString());
                ObjRef<iface::cellml_api::CellMLVariable> sourceVariable = variable->sourceVariable();

                if (variable == sourceVariable)
                    variable->initialValue(property->value().toStdWString());
                else
                    importedParameters += "\n - "+QString::fromStdWString(component->name())+" | "+QString::fromStdWString(variable->name());
            }
        }
    }

    // Now, we can effectively save our given file and let the user know if some
    // parameter values couldn't be saved

    bool res = (mFileType == CellmlFile)?
                   mCellmlFile->save(pFileName):
                   (mFileType == SedmlFile)?
                       mSedmlFile->save(pFileName):
                       mCombineArchive->save(pFileName);

    if (res) {
        mFileName = (mFileType == CellmlFile)?
                        mCellmlFile->fileName():
                        (mFileType == SedmlFile)?
                            mSedmlFile->fileName():
                            mCombineArchive->fileName();

        if (!importedParameters.isEmpty()) {
            Core::informationMessageBox(tr("Save File"),
                                        tr("The following parameters are imported and cannot therefore be saved:")+importedParameters);
        }
    }

    return res;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::filePermissionsChanged()
{
    // We have been un/locked, so enable/disable the development mode and keep
    // track of its checked status or recheck it, as necessary

     if (Core::FileManager::instance()->isReadableAndWritable(mFileName)) {
         mDevelopmentModeAction->setEnabled(mFileType == CellmlFile);
         mDevelopmentModeAction->setChecked(mLockedDevelopmentMode);
     } else {
         mLockedDevelopmentMode = mDevelopmentModeAction->isChecked();

         mDevelopmentModeAction->setChecked(false);
         mDevelopmentModeAction->setEnabled(false);
     }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::fileModified()
{
    // Update our reset action

    mResetModelParametersAction->setEnabled(Core::FileManager::instance()->isModified(mFileName));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::reloadView()
{
    // Reload ourselves, i.e. finalise and (re)initialise ourselves, meaning
    // that we have effectively been closed and (re)opened
    // Note: we don't want to call fileClosed() between finalize() and
    //       initialize() since it will trigger the description of existing
    //       graphs to be updated, which will result in them being temporarily
    //       shown as invalid even though they may actually be valid (since we
    //       have finalised the simulation)...

    finalize();
    initialize(true);

    mNeedReloadView = false;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::fileReloaded()
{
    // The given file has been reloaded, so stop its current simulation

    bool needReloadView = true;

    mNeedReloadView = true;

    if (mSimulation->stop()) {
        needReloadView = false;
        // Note: we don't need to reload ourselves since stopping the simulation
        //       will result in the stopped() signal being received and,
        //       therefore, the simulationStopped() slot being called, which is
        //       where we should reload ourselves since we cannot tell how long
        //       the signal/slot mechanism is going to take...
    }

    // Reload ourselves, if needed

    if (needReloadView)
        reloadView();
}

//==============================================================================

QString SimulationExperimentViewSimulationWidget::fileName() const
{
    // Return our file name

    return mFileName;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::setFileName(const QString &pFileName)
{
    // Set our file name

    mFileName = pFileName;
}

//==============================================================================

SEDMLSupport::SedmlFile * SimulationExperimentViewSimulationWidget::sedmlFile() const
{
    // Return our SED-ML file

    return mSedmlFile;
}

//==============================================================================

SimulationExperimentViewSimulationWidget::FileType SimulationExperimentViewSimulationWidget::fileType() const
{
    // Return our file type

    return mFileType;
}

//==============================================================================

SimulationExperimentViewSimulation * SimulationExperimentViewSimulationWidget::simulation() const
{
    // Return our simulation

    return mSimulation;
}

//==============================================================================

QVariant SimulationExperimentViewSimulationWidget::value(Core::Property *pProperty) const
{
    switch (pProperty->type()) {
    case Core::Property::Integer:
        return pProperty->integerValue();
    case Core::Property::Double:
        return pProperty->doubleValue();
    case Core::Property::List:
        return pProperty->listValue();
    case Core::Property::Boolean:
        return pProperty->booleanValue();
    default:
        // Not a relevant property, so return nothing
        // Note: we should never reach this point...

        return QVariant();
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::runPauseResumeSimulation()
{
    // Run/resume our simulation or pause it

    if (mRunActionEnabled) {
        // Protect ourselves against two successive (and very) quick attempts at
        // trying to run a simulation

        static bool handlingAction = false;

        if (!mSimulation->isPaused()) {
            if (handlingAction || mSimulation->isRunning())
                return;

            handlingAction = true;
        }

        // Finish any editing of our simulation information, and update our
        // simulation and solvers properties before running/resuming it

        mContentsWidget->informationWidget()->finishEditing(mSimulation->isPaused());

        if (!mSimulation->isPaused()) {
            updateSimulationProperties();
            updateSolversProperties();
        }

        // Run or resume our simulation

        if (mSimulation->isPaused()) {
            mSimulation->resume();
        } else {
            // Check that we have enough memory to run our simulation

            bool runSimulation = true;

            double freeMemory = Core::freeMemory();
            double requiredMemory = mSimulation->requiredMemory();

            if (requiredMemory > freeMemory) {
                Core::warningMessageBox(tr("Run Simulation"),
                                        tr("The simulation requires %1 of memory and you have only %2 left.").arg(Core::sizeAsString(requiredMemory), Core::sizeAsString(freeMemory)));
            } else {
                // Theoretically speaking, we have enough memory to run the
                // simulation, so try to allocate all the memory we need for the
                // simulation by resetting its settings

                runSimulation = mSimulation->results()->reset();

                mViewWidget->checkSimulationResults(mFileName, true);
                // Note: this will, among other things, clear our plots...

                // Effectively run our simulation in case we were able to
                // allocate all the memory we need to run the simulation

                if (runSimulation) {
                    mSimulation->run();
                } else {
                    Core::warningMessageBox(tr("Run Simulation"),
                                            tr("We could not allocate the %1 of memory required for the simulation.").arg(Core::sizeAsString(requiredMemory)));
                }
            }

            handlingAction = false;
        }
    } else {
        mSimulation->pause();
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::stopSimulation()
{
    // Stop our simulation

    mSimulation->stop();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::resetModelParameters()
{
    // Reset our model parameters

    mSimulation->reset();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::clearSimulationData()
{
    // Clear our simulation data

    mSimulation->results()->reset(false);

    // Update our simulation mode and check for results

    updateSimulationMode();

    mViewWidget->checkSimulationResults(mFileName, true);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::developmentMode()
{
    // The development mode has just been enabled/disabled, so update the
    // modified state of our current file accordingly

    if (!mDevelopmentModeAction->isChecked())
        Core::FileManager::instance()->setModified(mFileName, false);

    checkSimulationDataModified(mSimulation->data()->isModified());
    // Note: to call checkSimulationDataModified() will, in the case the
    //       development mode has just been disabled, ensure that the reset
    //       button is properly enabled/disabled...
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addGraphPanel()
{
    // Ask our graph panels widget to add a new graph panel

    mContentsWidget->graphPanelsWidget()->addGraphPanel();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::removeGraphPanel()
{
    // Default action for our removing of graph panel, i.e. remove the current
    // graph panel

    removeCurrentGraphPanel();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::removeCurrentGraphPanel()
{
    // Ask our graph panels widget to remove the current graph panel

    if (mContentsWidget->graphPanelsWidget()->removeCurrentGraphPanel()) {
        QCoreApplication::processEvents();
        // Note: this ensures that our remaining graph panels get realigned at
        //       once...
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::removeAllGraphPanels()
{
    // Ask our graph panels widget to remove the current graph panel

    mContentsWidget->graphPanelsWidget()->removeAllGraphPanels();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addSedmlSimulation(libsedml::SedDocument *pSedmlDocument,
                                                                  libsedml::SedModel *pSedmlModel,
                                                                  libsedml::SedRepeatedTask *pSedmlRepeatedTask,
                                                                  libsedml::SedSimulation *pSedmlSimulation,
                                                                  const int &pOrder)
{
    // Create, customise and add an algorithm (i.e. an ODE or DAE solver) to our
    // given SED-ML simulation
    // Note: the algorithm parameters require the use of KiSAO ids, so if none
    //       exists for an algorithm parameter then we set the algorithm
    //       parameter using an annotation...

    libsedml::SedAlgorithm *sedmlAlgorithm = pSedmlSimulation->createAlgorithm();
    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();
    SolverInterface *solverInterface = runtime->needOdeSolver()?
                                           mSimulation->data()->odeSolverInterface():
                                           mSimulation->data()->daeSolverInterface();
    QString solverName = runtime->needOdeSolver()?
                             mSimulation->data()->odeSolverName():
                             mSimulation->data()->daeSolverName();
    Solver::Solver::Properties solverProperties = runtime->needOdeSolver()?
                                                      mSimulation->data()->odeSolverProperties():
                                                      mSimulation->data()->daeSolverProperties();
    QString voiSolverProperties = QString();

    sedmlAlgorithm->setKisaoID(solverInterface->kisaoId(solverName).toStdString());

    foreach (const QString &solverProperty, solverProperties.keys()) {
        QString kisaoId = solverInterface->kisaoId(solverProperty);

        if (kisaoId.isEmpty()) {
            voiSolverProperties += QString("<solverProperty id=\"%1\" value=\"%2\"/>").arg(solverProperty,
                                                                                           solverProperties.value(solverProperty).toString());
        } else {
            libsedml::SedAlgorithmParameter *sedmlAlgorithmParameter = sedmlAlgorithm->createAlgorithmParameter();

            sedmlAlgorithmParameter->setKisaoID(kisaoId.toStdString());
            sedmlAlgorithmParameter->setValue(solverProperties.value(solverProperty).toString().toStdString());
        }
    }

    if (!voiSolverProperties.isEmpty()) {
        sedmlAlgorithm->appendAnnotation(QString("<%1 xmlns=\"%2\">%3</%1>").arg(SEDMLSupport::SolverProperties,
                                                                                 SEDMLSupport::OpencorNamespace,
                                                                                 voiSolverProperties).toStdString());
    }

    // Check whether the simulation required an NLA solver and, if so, let our
    // SED-ML simulation known about it through an annotation (since we cannot
    // have more than one SED-ML algorithm per SED-ML simulation)

    if (runtime->needNlaSolver()) {
        QString nlaSolverProperties = QString();

        foreach (const QString &solverProperty, mSimulation->data()->nlaSolverProperties().keys()) {
            nlaSolverProperties += QString("<%1 %2=\"%3\" %4=\"%5\"/>").arg(SEDMLSupport::SolverProperty,
                                                                            SEDMLSupport::SolverPropertyId,
                                                                            solverProperty,
                                                                            SEDMLSupport::SolverPropertyValue,
                                                                            solverProperties.value(solverProperty).toString());
        }

        pSedmlSimulation->appendAnnotation(QString("<%1 xmlns=\"%2\" name=\"%3\">%4</%1>").arg(SEDMLSupport::NlaSolver,
                                                                                               SEDMLSupport::OpencorNamespace,
                                                                                               mSimulation->data()->nlaSolverName(),
                                                                                               nlaSolverProperties).toStdString());
    }

    // Create and customise a task for our given SED-ML simulation

    libsedml::SedTask *sedmlTask = pSedmlDocument->createTask();

    sedmlTask->setId(QString("task%1").arg(pOrder).toStdString());
    sedmlTask->setModelReference(pSedmlModel->getId());
    sedmlTask->setSimulationReference(pSedmlSimulation->getId());

    // Create and customise the corresponding sub-task for our given SED-ML
    // simulation

    libsedml::SedSubTask *sedmlSubTask = pSedmlRepeatedTask->createSubTask();

    sedmlSubTask->setTask(sedmlTask->getId());
    sedmlSubTask->setOrder(pOrder);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addSedmlVariableTarget(libsedml::SedVariable *pSedmlVariable,
                                                                      const QString &pComponent,
                                                                      const QString &pVariable)
{
    // Set the target for the given SED-ML variable

    static const QString Target = "/cellml:model/cellml:component[@name='%1']/cellml:variable[@name='%2']";

    // Determine the degree of our variable, if any

    QString variable = pVariable;
    int variableDegree = variable.length();

    variable.replace('\'', QString());

    variableDegree -= variable.length();

    // Set the target itself, as well as its degree, if any

    pSedmlVariable->setTarget(Target.arg(pComponent, variable).toStdString());

    if (variableDegree) {
        pSedmlVariable->appendAnnotation(QString("<%1 xmlns=\"%2\">%3</%1>").arg(SEDMLSupport::VariableDegree,
                                                                                 SEDMLSupport::OpencorNamespace,
                                                                                 QString::number(variableDegree)).toStdString());
    }
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::createSedmlFile(const QString &pFileName,
                                                               const QString &pModelSource)
{
    // Create a SED-ML document and add the CellML namespace to it

    SEDMLSupport::SedmlFile sedmlFile(pFileName, true);
    libsedml::SedDocument *sedmlDocument = sedmlFile.sedmlDocument();
    XMLNamespaces *namespaces = sedmlDocument->getNamespaces();
    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::version(mFileName);

    namespaces->add((cellmlVersion == CellMLSupport::CellmlFile::Cellml_1_1)?
                        CellMLSupport::Cellml_1_1_Namespace.toStdString():
                        CellMLSupport::Cellml_1_0_Namespace.toStdString(),
                    "cellml");

    // Create and customise a model

    libsedml::SedModel *sedmlModel = sedmlDocument->createModel();

    sedmlModel->setId("model");
    sedmlModel->setLanguage((cellmlVersion == CellMLSupport::CellmlFile::Cellml_1_1)?
                                SEDMLSupport::Language::Cellml_1_1.toStdString():
                                SEDMLSupport::Language::Cellml_1_0.toStdString());
    sedmlModel->setSource(pModelSource.toStdString());

    // Create and customise a repeated task containing a uniform time course
    // simulation followed by a one-step simulation, if needed
    // Note: a uniform time course simulation would be enough if we can
    //       retrieve our point interval from the number of points and if we
    //       run a simulation without modifying any of the model parameters,
    //       but this would be a special case while here, using a repeated
    //       task, we can handle everything that we can do using the GUI...

    libsedml::SedRepeatedTask *sedmlRepeatedTask = sedmlDocument->createRepeatedTask();

    sedmlRepeatedTask->setId("repeatedTask");
    sedmlRepeatedTask->setRangeId("once");
    sedmlRepeatedTask->setResetModel(true);

    // Make our SED-ML repeated task non repeatable

    libsedml::SedVectorRange *sedmlVectorRange = sedmlRepeatedTask->createVectorRange();

    sedmlVectorRange->setId("once");
    sedmlVectorRange->addValue(1);

    // Create and customise a uniform time course simulation

    int simulationNumber = 0;
    double startingPoint = mSimulation->data()->startingPoint();
    double endingPoint = mSimulation->data()->endingPoint();
    double pointInterval = mSimulation->data()->pointInterval();
    int nbOfPoints = ceil((endingPoint-startingPoint)/pointInterval);
    bool needOneStepTask = (endingPoint-startingPoint)/nbOfPoints != pointInterval;

    libsedml::SedUniformTimeCourse *sedmlUniformTimeCourse = sedmlDocument->createUniformTimeCourse();

    ++simulationNumber;

    if (needOneStepTask)
        --nbOfPoints;

    sedmlUniformTimeCourse->setId(QString("simulation%1").arg(simulationNumber).toStdString());
    sedmlUniformTimeCourse->setInitialTime(startingPoint);
    sedmlUniformTimeCourse->setOutputStartTime(startingPoint);
    sedmlUniformTimeCourse->setOutputEndTime(startingPoint+nbOfPoints*pointInterval);
    sedmlUniformTimeCourse->setNumberOfPoints(nbOfPoints);

    addSedmlSimulation(sedmlDocument, sedmlModel, sedmlRepeatedTask,
                       sedmlUniformTimeCourse, simulationNumber);

    // Complete our simulation with a one-step simulation, if needed

    if (needOneStepTask) {
        libsedml::SedOneStep *sedmlOneStep = sedmlDocument->createOneStep();

        ++simulationNumber;

        sedmlOneStep->setId(QString("simulation%1").arg(simulationNumber).toStdString());
        sedmlOneStep->setStep(endingPoint-sedmlUniformTimeCourse->getOutputEndTime());

        addSedmlSimulation(sedmlDocument, sedmlModel, sedmlRepeatedTask,
                           sedmlOneStep, simulationNumber);
    }

    // Retrieve all the graphs that are to be plotted, if any

    QList<Core::Properties> graphsList = QList<Core::Properties>();
    SimulationExperimentViewInformationGraphsWidget *graphsWidget = mContentsWidget->informationWidget()->graphsWidget();

    foreach (GraphPanelWidget::GraphPanelWidget *graphPanel,
             mContentsWidget->graphPanelsWidget()->graphPanels()) {
        Core::Properties graphs = graphsWidget->graphProperties(graphPanel, mFileName);

        if (!graphs.isEmpty())
            graphsList << graphs;
    }

    // Create and customise 2D plot outputs and data generators for all the
    // graphs that are to be plotted, if any

    if (!graphsList.isEmpty()) {
        int graphPlotCounter = 0;

        foreach (Core::Properties graphs, graphsList) {
            ++graphPlotCounter;

            int graphCounter = 0;
            libsedml::SedPlot2D *sedmlPlot2d = sedmlDocument->createPlot2D();

            sedmlPlot2d->setId(QString("plot%1").arg(graphPlotCounter).toStdString());

            foreach (Core::Property *property, graphs) {
                ++graphCounter;

                // Create two data generators for the X and Y parameters of
                // our current graph

                libsedml::SedDataGenerator *sedmlDataGeneratorX = sedmlDocument->createDataGenerator();
                libsedml::SedDataGenerator *sedmlDataGeneratorY = sedmlDocument->createDataGenerator();
                std::string sedmlDataGeneratorIdX = QString("xDataGenerator%1_%2").arg(QString::number(graphPlotCounter),
                                                                                       QString::number(graphCounter)).toStdString();
                std::string sedmlDataGeneratorIdY = QString("yDataGenerator%1_%2").arg(QString::number(graphPlotCounter),
                                                                                       QString::number(graphCounter)).toStdString();

                sedmlDataGeneratorX->setId(sedmlDataGeneratorIdX);
                sedmlDataGeneratorY->setId(sedmlDataGeneratorIdY);

                libsedml::SedVariable *sedmlVariableX = sedmlDataGeneratorX->createVariable();
                libsedml::SedVariable *sedmlVariableY = sedmlDataGeneratorY->createVariable();
                QStringList propertyX = property->properties()[1]->value().split('.');
                QStringList propertyY = property->properties()[2]->value().split('.');

                sedmlVariableX->setId(QString("xVariable%1_%2").arg(QString::number(graphPlotCounter),
                                                                    QString::number(graphCounter)).toStdString());
                sedmlVariableX->setTaskReference(sedmlRepeatedTask->getId());
                addSedmlVariableTarget(sedmlVariableX, propertyX[propertyX.count()-2], propertyX.last());

                sedmlVariableY->setId(QString("yVariable%1_%2").arg(QString::number(graphPlotCounter),
                                                                    QString::number(graphCounter)).toStdString());
                sedmlVariableY->setTaskReference(sedmlRepeatedTask->getId());
                addSedmlVariableTarget(sedmlVariableY, propertyY[propertyY.count()-2], propertyY.last());

                sedmlDataGeneratorX->setMath(SBML_parseFormula(sedmlVariableX->getId().c_str()));
                sedmlDataGeneratorY->setMath(SBML_parseFormula(sedmlVariableY->getId().c_str()));

                // Create a curve for our current graph

                libsedml::SedCurve *sedmlCurve = sedmlPlot2d->createCurve();

                sedmlCurve->setId(QString("curve%1_%2").arg(QString::number(graphPlotCounter),
                                                            QString::number(graphCounter)).toStdString());

                sedmlCurve->setXDataReference(sedmlDataGeneratorIdX);
                sedmlCurve->setLogX(false);

                sedmlCurve->setYDataReference(sedmlDataGeneratorIdY);
                sedmlCurve->setLogY(false);
            }
        }
    }

    // Our SED-ML document is ready, so save it

    return sedmlFile.save(pFileName);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::sedmlExportSedmlFile()
{
    // Export ourselves to SED-ML using a SED-ML file, but first get a file name

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    bool remoteFile = fileManagerInstance->isRemote(mFileName);
    QString cellmlFileName = remoteFile?fileManagerInstance->url(mFileName):mFileName;
    QString cellmlFileCompleteSuffix = QFileInfo(cellmlFileName).completeSuffix();
    QString sedmlFileName = cellmlFileName;
    QStringList sedmlFilters = Core::filters(FileTypeInterfaces() << SEDMLSupport::fileTypeInterface());
    QString firstSedmlFilter = sedmlFilters.first();

    if (!cellmlFileCompleteSuffix.isEmpty()) {
        sedmlFileName.replace(QRegularExpression(QRegularExpression::escape(cellmlFileCompleteSuffix)+"$"),
                              SEDMLSupport::SedmlFileExtension);
    } else {
        sedmlFileName += "."+SEDMLSupport::SedmlFileExtension;
    }

    sedmlFileName = Core::getSaveFileName(tr("Export To SED-ML File"),
                                          sedmlFileName,
                                          sedmlFilters, &firstSedmlFilter);

    // Create a SED-ML file using the SED-ML file name that has been provided

    if (!sedmlFileName.isEmpty()) {
        QString modelSource = cellmlFileName;

        if (   !remoteFile
#ifdef Q_OS_WIN
            && !modelSource.left(3).compare(sedmlFileName.left(3))
#endif
           ) {
            // We are dealing with a local CellML file that is on the same drive
            // as our SED-ML file, so refer to it relatively to it
            // Note: normally, we would use QFileInfo::canonicalPath(), but this
            //       requires an existing file, so we use QFileInfo::path()
            //       instead...

            QDir sedmlFileDir = QFileInfo(sedmlFileName).path();

            modelSource = sedmlFileDir.relativeFilePath(modelSource);
        }

        if (!createSedmlFile(sedmlFileName, modelSource)) {
            Core::warningMessageBox(tr("Export To SED-ML File"),
                                    tr("The simulation could not be exported to <strong>%1</strong>.").arg(sedmlFileName));
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::sedmlExportCombineArchive()
{
    // Export ourselves to SED-ML using a COMBINE archive, but first get a file
    // name

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    bool remoteFile = fileManagerInstance->isRemote(mFileName);
    QString cellmlFileName = remoteFile?fileManagerInstance->url(mFileName):mFileName;
    QString cellmlFileCompleteSuffix = QFileInfo(cellmlFileName).completeSuffix();
    QString combineArchiveName = cellmlFileName;
    QStringList combineFilters = Core::filters(FileTypeInterfaces() << COMBINESupport::fileTypeInterface());
    QString firstCombineFilter = combineFilters.first();

    if (!cellmlFileCompleteSuffix.isEmpty()) {
        combineArchiveName.replace(QRegularExpression(QRegularExpression::escape(cellmlFileCompleteSuffix)+"$"),
                                   COMBINESupport::CombineFileExtension);
    } else {
        combineArchiveName += "."+COMBINESupport::CombineFileExtension;
    }

    combineArchiveName = Core::getSaveFileName(tr("Export To COMBINE Archive"),
                                               combineArchiveName,
                                               combineFilters, &firstCombineFilter);

    // Effectively export ourselves to a COMBINE archive, if a COMBINE archive
    // name has been provided

    if (!combineArchiveName.isEmpty()) {
        // Determine the path that is common to our main and, if any, imported
        // CellML files, as well as get a copy of our imported CellML files,
        // should they be remote ones

        static const QRegularExpression FileNameRegEx = QRegularExpression("/[^/]*$");

        QString commonPath = remoteFile?
                                 QString(cellmlFileName).remove(FileNameRegEx)+"/":
                                 QFileInfo(mFileName).canonicalPath()+QDir::separator();
        QMap<QString, QString> remoteImportedFileNames = QMap<QString, QString>();

        foreach (const QString &importedFileName, mCellmlFile->importedFileNames()) {
            // Check for the common path

            QString importedFilePath = remoteFile?
                                           QString(importedFileName).remove(FileNameRegEx)+"/":
                                           QFileInfo(importedFileName).canonicalPath()+QDir::separator();

            for (int i = 0, iMax = qMin(commonPath.length(), importedFilePath.length()); i < iMax; ++i) {
                if (commonPath[i] != importedFilePath[i]) {
                    commonPath = commonPath.left(i);

                    break;
                }
            }

            commonPath = commonPath.left(qMin(commonPath.length(), importedFilePath.length()));

            // Get a copy of the imported CellML file, if it is a remote one,
            // and keep track of it

            if (remoteFile) {
                QString localImportedFileName = Core::temporaryFileName();

                Core::writeFileContentsToFile(localImportedFileName,
                                              mCellmlFile->importedFileContents(importedFileName));

                remoteImportedFileNames.insert(importedFileName, localImportedFileName);
            }
        }

        // Determine the location of our main CellML file

        QString modelSource = remoteFile?
                                  QString(cellmlFileName).remove(commonPath):
                                  QString(mFileName).remove(Core::nativeCanonicalDirName(commonPath)+QDir::separator());

        // Create a copy of the SED-ML file that will be the master file in our
        // COMBINE archive

        QString errorMessage = QString();
        QString temporaryCombineArchiveName = Core::temporaryFileName();
        QString sedmlFileName = Core::temporaryFileName();

        createSedmlFile(sedmlFileName, modelSource);

        // Create our COMBINE archive after having added all our files to it

        COMBINESupport::CombineArchive combineArchive(temporaryCombineArchiveName, true);
        QFileInfo combineArchiveInfo = QFileInfo(combineArchiveName);
        QString sedmlFileLocation = combineArchiveInfo.fileName();

        sedmlFileLocation.replace(QRegularExpression(QRegularExpression::escape(combineArchiveInfo.completeSuffix())+"$"),
                                  SEDMLSupport::SedmlFileExtension);

        if (combineArchive.addFile(sedmlFileName, sedmlFileLocation,
                                   COMBINESupport::CombineArchiveFile::Sedml, true)) {
            if (combineArchive.addFile(mFileName, modelSource,
                                       (mCellmlFile->version() == CellMLSupport::CellmlFile::Cellml_1_1)?
                                           COMBINESupport::CombineArchiveFile::Cellml_1_1:
                                           COMBINESupport::CombineArchiveFile::Cellml_1_0)) {
                foreach (const QString &importedFileName, mCellmlFile->importedFileNames()) {
                    QString realImportedFileName = remoteFile?
                                                       remoteImportedFileNames.value(importedFileName):
                                                       importedFileName;

                    if (!combineArchive.addFile(realImportedFileName,
                                                QString(importedFileName).remove(commonPath),
                                                COMBINESupport::CombineArchiveFile::Cellml)) {
                        errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(combineArchiveName, " ("+tr("<strong>%1</strong> could not be added").arg(realImportedFileName)+").");

                        break;
                    }
                }

                if (errorMessage.isEmpty()) {
                    if (combineArchive.save()) {
                        QFile::remove(combineArchiveName);

                        if (!QFile::rename(temporaryCombineArchiveName, combineArchiveName))
                            errorMessage = tr("The simulation could not be exported to <strong>%1</strong>.").arg(combineArchiveName);
                    } else {
                        errorMessage = tr("The simulation could not be exported to <strong>%1</strong>.").arg(combineArchiveName);
                    }
                }
            } else {
                errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(combineArchiveName, " ("+tr("<strong>%1</strong> could not be added").arg(mFileName)+").");
            }
        } else {
            errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(combineArchiveName, " ("+tr("the master SED-ML file could not be added")+").");
        }

        // Remove our temporary COMBINE archive and the local copy of our remote
        // imported CellML files, if any

        QFile::remove(temporaryCombineArchiveName);

        foreach (const QString &localImportedFileName, remoteImportedFileNames.values())
            QFile::remove(localImportedFileName);

        // Let the user know about any error that may have occurred

        if (!errorMessage.isEmpty()) {
            Core::warningMessageBox(tr("Export To COMBINE Archive"),
                                    errorMessage);
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSimulationProperties(Core::Property *pProperty)
{
    // Update all the properties, or a particular property (if it exists), of
    // our simulation

    SimulationExperimentViewInformationSimulationWidget *simulationWidget = mContentsWidget->informationWidget()->simulationWidget();

    if (!pProperty || (pProperty == simulationWidget->startingPointProperty())) {
        mSimulation->data()->setStartingPoint(simulationWidget->startingPointProperty()->doubleValue());

        if (pProperty)
            return;
    }

    if (!pProperty || (pProperty == simulationWidget->endingPointProperty())) {
        mSimulation->data()->setEndingPoint(simulationWidget->endingPointProperty()->doubleValue());

        if (pProperty)
            return;
    }

    if (!pProperty || (pProperty == simulationWidget->pointIntervalProperty())) {
        mSimulation->data()->setPointInterval(simulationWidget->pointIntervalProperty()->doubleValue());

        if (pProperty)
            return;
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSolversProperties(Core::Property *pProperty)
{
    // Update all of our solver(s) properties (and solvers widget) or a
    // particular solver property (and the corresponding GUI for that solver)

    SimulationExperimentViewInformationSolversWidget *solversWidget = mContentsWidget->informationWidget()->solversWidget();

    // ODE solver properties

    bool needOdeSolverGuiUpdate = false;

    if (solversWidget->odeSolverData()) {
        if (!pProperty || (pProperty == solversWidget->odeSolverData()->solversListProperty())) {
            mSimulation->data()->setOdeSolverName(solversWidget->odeSolverData()->solversListProperty()->value());

            needOdeSolverGuiUpdate = true;
        }

        if (!pProperty || !needOdeSolverGuiUpdate) {
            foreach (Core::Property *property, solversWidget->odeSolverData()->solversProperties().value(mSimulation->data()->odeSolverName())) {
                if (!pProperty || (pProperty == property)) {
                    mSimulation->data()->addOdeSolverProperty(property->id(), value(property));

                    needOdeSolverGuiUpdate = true;

                    if (pProperty)
                        break;
                }
            }
        }
    }

    if (needOdeSolverGuiUpdate) {
        mContentsWidget->informationWidget()->solversWidget()->updateGui(solversWidget->odeSolverData());

        if (pProperty)
            return;
    }

    // DAE solver properties

    bool needDaeSolverGuiUpdate = false;

    if (solversWidget->daeSolverData()) {
        if (!pProperty || (pProperty == solversWidget->daeSolverData()->solversListProperty())) {
            mSimulation->data()->setDaeSolverName(solversWidget->daeSolverData()->solversListProperty()->value());

            needDaeSolverGuiUpdate = true;
        }

        if (!pProperty || !needDaeSolverGuiUpdate) {
            foreach (Core::Property *property, solversWidget->daeSolverData()->solversProperties().value(mSimulation->data()->daeSolverName())) {
                if (!pProperty || (pProperty == property)) {
                    mSimulation->data()->addDaeSolverProperty(property->id(), value(property));

                    needDaeSolverGuiUpdate = true;

                    if (pProperty)
                        break;
                }
            }
        }
    }

    if (needDaeSolverGuiUpdate) {
        mContentsWidget->informationWidget()->solversWidget()->updateGui(solversWidget->daeSolverData());

        if (pProperty)
            return;
    }

    // NLA solver properties

    bool needNlaSolverGuiUpdate = false;

    if (solversWidget->nlaSolverData()) {
        if (!pProperty || (pProperty == solversWidget->nlaSolverData()->solversListProperty())) {
            mSimulation->data()->setNlaSolverName(solversWidget->nlaSolverData()->solversListProperty()->value());

            needNlaSolverGuiUpdate = true;
        }

        if (!pProperty || !needNlaSolverGuiUpdate) {
            foreach (Core::Property *property, solversWidget->nlaSolverData()->solversProperties().value(mSimulation->data()->nlaSolverName())) {
                if (!pProperty || (pProperty == property)) {
                    mSimulation->data()->addNlaSolverProperty(property->id(), value(property));

                    needNlaSolverGuiUpdate = true;

                    if (pProperty)
                        break;
                }
            }
        }
    }

    if (needNlaSolverGuiUpdate) {
        mContentsWidget->informationWidget()->solversWidget()->updateGui(solversWidget->nlaSolverData());

        if (pProperty)
            return;
    }
}

//==============================================================================

CellMLSupport::CellmlFileRuntimeParameter * SimulationExperimentViewSimulationWidget::runtimeParameter(libsedml::SedVariable *pSedmlVariable)
{
    // Retrieve the CellML runtime parameter corresponding to the given SED-ML
    // variable

    static const QRegularExpression TargetStartRegEx  = QRegularExpression("^\\/cellml:model\\/cellml:component\\[@name='");
    static const QRegularExpression TargetMiddleRegEx = QRegularExpression("']\\/cellml:variable\\[@name='");
    static const QRegularExpression TargetEndRegEx    = QRegularExpression("'\\]$");
    static const QString Separator = "|";

    // Retrieve the component and name of the parameter

    QString target = QString::fromStdString(pSedmlVariable->getTarget());

    target.remove(TargetStartRegEx);
    target.replace(TargetMiddleRegEx, Separator);
    target.remove(TargetEndRegEx);

    QStringList identifiers = target.split(Separator);
    QString componentName = identifiers.first();
    QString variableName = identifiers.last();

    // Check whether the parameter has a degree

    libsbml::XMLNode *annotation = pSedmlVariable->getAnnotation();
    int variableDegree = 0;

    if (annotation) {
        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const XMLNode &node = annotation->getChild(i);

            if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                || QString::fromStdString(node.getName()).compare(SEDMLSupport::VariableDegree)) {
                continue;
            }

            variableDegree = QString::fromStdString(node.getChild(0).getCharacters()).toInt();
        }
    }

    // Go through the runtime parameters to see one of them correspond to our
    // given SED-ML variable

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, mSimulation->runtime()->parameters()) {
        if (   !componentName.compare(parameter->componentHierarchy().last())
            && !variableName.compare(parameter->name())
            &&  (variableDegree == parameter->degree())) {
            return parameter;
        }
    }

    return 0;
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::doFurtherInitialize()
{
    // Customise our simulation widget

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
    SimulationExperimentViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();

    libsedml::SedDocument *sedmlDocument = mSedmlFile->sedmlDocument();
    libsedml::SedUniformTimeCourse *uniformTimeCourseSimulation = static_cast<libsedml::SedUniformTimeCourse *>(sedmlDocument->getSimulation(0));
    libsedml::SedOneStep *oneStepSimulation = static_cast<libsedml::SedOneStep *>(sedmlDocument->getSimulation(1));

    double startingPoint = uniformTimeCourseSimulation->getOutputStartTime();
    double endingPoint = uniformTimeCourseSimulation->getOutputEndTime();
    double pointInterval = (endingPoint-startingPoint)/uniformTimeCourseSimulation->getNumberOfPoints();

    if (oneStepSimulation)
        endingPoint += oneStepSimulation->getStep();

    simulationWidget->startingPointProperty()->setDoubleValue(startingPoint);
    simulationWidget->endingPointProperty()->setDoubleValue(endingPoint);
    simulationWidget->pointIntervalProperty()->setDoubleValue(pointInterval);

    // Customise our solvers widget by:
    //  - Specifying the ODE/DAE solver to use
    //  - Customising the solver's properties for which we have a KiSAO id
    //  - Customising the solver's properties for which we don't have a KiSAO id
    //    (this shouldn't happen, but better be safe than sorry)
    //  - Specifying the NLA solver, if any

    SimulationExperimentViewInformationSolversWidgetData *solverData = (mCellmlFile->runtime()->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?
                                                                           informationWidget->solversWidget()->odeSolverData():
                                                                           informationWidget->solversWidget()->daeSolverData();
    const libsedml::SedAlgorithm *algorithm = uniformTimeCourseSimulation->getAlgorithm();
    SolverInterface *usedSolverInterface = 0;
    SolverInterfaces solverInterfaces = Core::solverInterfaces();
    Core::Properties solverProperties = Core::Properties();
    QString kisaoId = QString::fromStdString(algorithm->getKisaoID());

    foreach (SolverInterface *solverInterface, solverInterfaces) {
        if (!solverInterface->id(kisaoId).compare(solverInterface->solverName())) {
            usedSolverInterface = solverInterface;
            solverProperties = solverData->solversProperties().value(solverInterface->solverName());

            solverData->solversListProperty()->setValue(solverInterface->solverName());

            break;
        }
    }

    if (!usedSolverInterface) {
        simulationError(tr("the requested solver (%1) could not be found").arg(kisaoId),
                        InvalidSimulationEnvironment);

        return false;
    }

    for (int i = 0, iMax = algorithm->getNumAlgorithmParameters(); i < iMax; ++i) {
        const libsedml::SedAlgorithmParameter *algorithmParameter = algorithm->getAlgorithmParameter(i);
        QString kisaoId = QString::fromStdString(algorithmParameter->getKisaoID());
        QString id = usedSolverInterface->id(kisaoId);
        bool propertySet = false;

        foreach (Core::Property *solverProperty, solverProperties) {
            if (!solverProperty->id().compare(id)) {
                QVariant solverPropertyValue = QString::fromStdString(algorithmParameter->getValue());

                switch (solverProperty->type()) {
                case Core::Property::Section:
#ifdef QT_DEBUG
                    // We should never come here...

                    qFatal("FATAL ERROR | %s:%d: the solver property cannot be of section type.", __FILE__, __LINE__);
#endif

                    break;
                case Core::Property::String:
                    solverProperty->setValue(solverPropertyValue.toString());

                    break;
                case Core::Property::Integer:
                    solverProperty->setIntegerValue(solverPropertyValue.toInt());

                    break;
                case Core::Property::Double:
                    solverProperty->setDoubleValue(solverPropertyValue.toDouble());

                    break;
                case Core::Property::List:
                    solverProperty->setListValue(solverPropertyValue.toString());

                    break;
                case Core::Property::Boolean:
                    solverProperty->setBooleanValue(solverPropertyValue.toBool());

                    break;
                }

                propertySet = solverProperty->type() != Core::Property::Section;

                break;
            }
        }

        if (!propertySet) {
            simulationError(tr("the requested property (%1) could not be set").arg(kisaoId),
                            InvalidSimulationEnvironment);

            return false;
        }
    }

    libsbml::XMLNode *annotation = algorithm->getAnnotation();

    if (annotation) {
        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const XMLNode &node = annotation->getChild(i);

            if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                || QString::fromStdString(node.getName()).compare(SEDMLSupport::SolverProperties)) {
                continue;
            }

            for (uint j = 0, jMax = node.getNumChildren(); j < jMax; ++j) {
                const XMLNode &solverPropertyNode = node.getChild(j);

                if (   QString::fromStdString(solverPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                    || QString::fromStdString(solverPropertyNode.getName()).compare(SEDMLSupport::SolverProperty)) {
                    continue;
                }

                QString id = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::SolverPropertyId.toStdString())));
                QString value = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::SolverPropertyValue.toStdString())));
                bool propertySet = false;

                foreach (Core::Property *solverProperty, solverProperties) {
                    if (!solverProperty->id().compare(id)) {
                        solverProperty->setValue(value);

                        propertySet = true;

                        break;
                    }
                }

                if (!propertySet) {
                    simulationError(tr("the requested property (%1) could not be set").arg(id),
                                    InvalidSimulationEnvironment);

                    return false;
                }
            }
        }
    }

    annotation = uniformTimeCourseSimulation->getAnnotation();

    if (annotation) {
        bool mustHaveNlaSolver = false;
        bool hasNlaSolver = false;
        QString nlaSolverName = QString();

        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const libsbml::XMLNode &node = annotation->getChild(i);

            if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                || QString::fromStdString(node.getName()).compare(SEDMLSupport::NlaSolver)) {
                continue;
            }

            mustHaveNlaSolver = true;
            nlaSolverName = QString::fromStdString(node.getAttrValue(node.getAttrIndex(SEDMLSupport::NlaSolverName.toStdString())));

            foreach (SolverInterface *solverInterface, solverInterfaces) {
                if (!nlaSolverName.compare(solverInterface->solverName())) {
                    informationWidget->solversWidget()->nlaSolverData()->solversListProperty()->setValue(nlaSolverName);

                    hasNlaSolver = true;

                    break;
                }
            }

            if (hasNlaSolver)
                break;
        }

        if (mustHaveNlaSolver && !hasNlaSolver) {
            simulationError(tr("the requested NLA solver (%1) could not be set").arg(nlaSolverName),
                            InvalidSimulationEnvironment);

            return false;
        }
    }

    // Add/remove some graph panels, so that the end number of them corresponds
    // to the number of 2D outputs mentioned in the SED-ML file, this after
    // having made sure that the current graph panels are all of the same size
    // and that the first one of them is selected

    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();
    int oldNbOfGraphPanels = graphPanelsWidget->graphPanels().count();
    int newNbOfGraphPanels = sedmlDocument->getNumOutputs();

    graphPanelsWidget->setSizes(QIntList());
    graphPanelsWidget->setActiveGraphPanel(graphPanelsWidget->graphPanels().first());

    if (newNbOfGraphPanels > oldNbOfGraphPanels) {
        for (uint i = 0, iMax = newNbOfGraphPanels-oldNbOfGraphPanels; i < iMax; ++i)
            graphPanelsWidget->addGraphPanel(false);
    } else if (newNbOfGraphPanels < oldNbOfGraphPanels) {
        for (uint i = 0, iMax = oldNbOfGraphPanels-newNbOfGraphPanels; i < iMax; ++i)
            graphPanelsWidget->removeCurrentGraphPanel();
    }

    // Customise our graphs widget

    for (int i = 0; i < newNbOfGraphPanels; ++i) {
        libsedml::SedPlot2D *plot = static_cast<libsedml::SedPlot2D *>(sedmlDocument->getOutput(i));
        GraphPanelWidget::GraphPanelWidget *graphPanel = graphPanelsWidget->graphPanels()[i];

        graphPanel->removeAllGraphs();

        for (uint j = 0, jMax = plot->getNumCurves(); j < jMax; ++j) {
            libsedml::SedCurve *curve = plot->getCurve(j);
            CellMLSupport::CellmlFileRuntimeParameter *xParameter = runtimeParameter(sedmlDocument->getDataGenerator(curve->getXDataReference())->getVariable(0));
            CellMLSupport::CellmlFileRuntimeParameter *yParameter = runtimeParameter(sedmlDocument->getDataGenerator(curve->getYDataReference())->getVariable(0));

            if (!xParameter || !yParameter) {
                simulationError(tr("the requested curve (%1) could not be set").arg(QString::fromStdString(curve->getId())),
                                InvalidSimulationEnvironment);

                return false;
            }

            graphPanel->addGraph(new GraphPanelWidget::GraphPanelPlotGraph(xParameter, yParameter));
        }
    }

    return true;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::initializeGui(const bool &pValidSimulationEnvironment)
{
    // Show/hide some widgets based on whether we have a valid simulation
    // environment

    mToolBarWidget->setVisible(pValidSimulationEnvironment);
    mTopSeparator->setVisible(pValidSimulationEnvironment);

    mContentsWidget->setVisible(pValidSimulationEnvironment);
    mInvalidModelMessageWidget->setVisible(!pValidSimulationEnvironment);

    mBottomSeparator->setVisible(pValidSimulationEnvironment);
    mProgressBarWidget->setVisible(pValidSimulationEnvironment);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::furtherInitialize()
{
    // Further initialise ourselves, update our GUI (by reinitialising it) and
    // initialise our simulation, if we still have a valid simulation
    // environment

    bool validSimulationEnvironment = doFurtherInitialize();

    initializeGui(validSimulationEnvironment);

    if (validSimulationEnvironment)
        initializeSimulation();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::initializeSimulation()
{
    // Reset both the simulation's data and results (well, initialise in the
    // case of its data)

    mSimulation->data()->reset();
    mSimulation->results()->reset(false);

    // Retrieve our simulation and solvers properties since they may have an
    // effect on our parameter values (as well as result in some solver
    // properties being shown/hidden)

    updateSimulationProperties();
    updateSolversProperties();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(mSplitterWidget->sizes());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationDataExport()
{
    // Retrieve some data so that we can effectively export our simulation data
    // results

    DataStoreInterface *dataStoreInterface = mDataStoreInterfaces.value(qobject_cast<QAction *>(sender()));
    DataStore::DataStore *dataStore = mSimulation->results()->dataStore();
    DataStore::DataStoreData *dataStoreData = dataStoreInterface->getData(mFileName, dataStore);

    if (dataStoreData) {
        // We have got the data we need, so do the actual export

        Core::centralWidget()->showProgressBusyWidget();

        DataStore::DataStoreExporter *dataStoreExporter = dataStoreInterface->dataStoreExporterInstance(mFileName, dataStore, dataStoreData);

        connect(dataStoreExporter, SIGNAL(done(const QString &)),
                this, SLOT(dataStoreExportDone(const QString &)));
        connect(dataStoreExporter, SIGNAL(progress(const double &)),
                this, SLOT(dataStoreExportProgress(const double &)));

        dataStoreExporter->start();
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateDelayValue(const double &pDelayValue)
{
    // Update our delay value widget

    int delay = 0;
    int increment = 1;
    int multiple = 10;

    for (int i = 0, iMax = pDelayValue; i < iMax; ++i) {
        delay += increment;

        if (!(delay % multiple)) {
            increment *= 10;
            multiple *= 10;
        }
    }

    mDelayValueWidget->setText(QLocale().toString(delay));

    // Also update our simulation object

    mSimulation->setDelay(delay);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationRunning(const bool &pIsResuming)
{
    Q_UNUSED(pIsResuming);

    // Our simulation is running, so update our simulation mode and check for
    // results

    updateSimulationMode();

    mViewWidget->checkSimulationResults(mFileName);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationPaused()
{
    // Our simulation is paused, so update our simulation mode and parameters,
    // and check for results

    updateSimulationMode();

    mContentsWidget->informationWidget()->parametersWidget()->updateParameters(mSimulation->currentPoint());

    mViewWidget->checkSimulationResults(mFileName);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationStopped(const qint64 &pElapsedTime)
{
    // Output the given elapsed time, if valid

    if (pElapsedTime != -1) {
        QString solversInformation = QString();

        if (!mSimulation->data()->odeSolverName().isEmpty())
            solversInformation += mSimulation->data()->odeSolverName();
        else
            solversInformation += mSimulation->data()->daeSolverName();

        if (!mSimulation->data()->nlaSolverName().isEmpty())
            solversInformation += "+"+mSimulation->data()->nlaSolverName();

        output(QString(OutputTab+"<strong>"+tr("Simulation time:")+"</strong> <span"+OutputInfo+">"+tr("%1 s using %2").arg(QString::number(0.001*pElapsedTime, 'g', 3), solversInformation)+"</span>."+OutputBrLn));
    }

    // Update our parameters and simulation mode

    updateSimulationMode();

    mContentsWidget->informationWidget()->parametersWidget()->updateParameters(mSimulation->currentPoint());

    // Stop keeping track of our simulation progress

    mProgress = -1;

    // Reload ourselves, if needed (see fileReloaded())

    if (mNeedReloadView)
        reloadView();

    // Note: our simulation progress gets reset in resetSimulationProgress(),
    //       which is called by
    //       SimulationExperimentViewWidget::checkSimulationResults(). To reset
    //       our simulation progress here might not always work since our
    //       simulation is run in a different thread, meaning that a call to
    //       updateSimulationResults() might occur after we have reset our
    //       simulation progress...
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::resetProgressBar()
{
    // Reset our progress bar

    mProgressBarWidget->setValue(0.0);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::resetFileTabIcon()
{
    // Stop tracking our simulation progress and let people know that our file
    // tab icon should be reset

    static const QIcon NoIcon = QIcon();

    emit mViewWidget->updateFileTabIcon(mPlugin->viewName(), mFileName, NoIcon);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::resetSimulationProgress()
{
    // Reset our progress bar or tab icon, in case we are not visible, and this
    // with a short delay
    // Note #1: we check that we are not visible in case the user has selected a
    //          file that cannot be handled by us, meaning that our central
    //          widget would show a message rather than us...
    // Note #2: we want a short delay before resetting our progress bar or tab
    //          icon, so that the user can really see when our simulation has
    //          completed, but this is only is we don't need to reload
    //          ourselves. Indeed, if we need to reload ourselves (see
    //          fileReloaded()), we want things to be done as quickly as
    //          possible. Not only that, but we don't want to risk problems with
    //          our simulation being used while it has already been deleted due
    //          to threading issues...

    enum {
        ResetDelay = 169
    };

    if (isVisible()) {
        if (mNeedReloadView)
            resetProgressBar();
        else
            QTimer::singleShot(ResetDelay, this, SLOT(resetProgressBar()));
    } else {
        if (mNeedReloadView)
            resetFileTabIcon();
        else
            QTimer::singleShot(ResetDelay, this, SLOT(resetFileTabIcon()));
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationError(const QString &pMessage,
                                                               const ErrorType &pErrorType)
{
    // Output the simulation error

    mErrorType = pErrorType;

    updateInvalidModelMessageWidget();

    output(OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+pMessage+".</span>"+OutputBrLn);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::checkSimulationDataModified(const bool &pIsModified)
{
    // We are dealing with the current simulation

    if (mDevelopmentModeAction->isChecked())
        Core::FileManager::instance()->setModified(mFileName, pIsModified);
    else
        mResetModelParametersAction->setEnabled(pIsModified);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationDataModified(const bool &pIsModified)
{
    // Update our modified state

    checkSimulationDataModified(pIsModified);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationPropertyChanged(Core::Property *pProperty)
{
    // Update our simulation properties, as well as our plots, if it's not the
    // point interval property that has been updated

    updateSimulationProperties(pProperty);

    SimulationExperimentViewInformationSimulationWidget *simulationWidget = mContentsWidget->informationWidget()->simulationWidget();

    if (pProperty != simulationWidget->pointIntervalProperty()) {
        bool needProcessingEvents = false;
        // Note: needProcessingEvents is used to ensure that our plots are all
        //       updated at once...

        foreach (GraphPanelWidget::GraphPanelPlotWidget *plot, mPlots) {
            if (updatePlot(plot))
                needProcessingEvents = true;
        }

        if (needProcessingEvents)
            QCoreApplication::processEvents();
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::solversPropertyChanged(Core::Property *pProperty)
{
    // Update our solvers properties

    updateSolversProperties(pProperty);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphPanelAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                               const bool &pActive)
{
    Q_UNUSED(pActive);

    // Keep track of the fact that we want to know if a graph panel's plot's
    // axes have been changed
    // Note: we don't need to keep track of the graph panel's plot (in mPlots)
    //       since we only want to do this if the plot actually has graphs
    //       associated with it (see graphAdded())...

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    mUpdatablePlotViewports.insert(plot, true);

    connect(plot, SIGNAL(axesChanged(const double &, const double &, const double &, const double &)),
            this, SLOT(plotAxesChanged()));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphPanelRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel)
{
    // A graph panel has been removed, so stop tracking its plot and the fact
    // that we wanted to know if its axes had been changed

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    disconnect(plot, SIGNAL(axesChanged(const double &, const double &, const double &, const double &)),
               this, SLOT(plotAxesChanged()));

    mPlots.removeOne(plot);
    mUpdatablePlotViewports.remove(plot);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                                                        CellMLSupport::CellmlFileRuntimeParameter *pParameterY)
{
    // Ask the current graph panel to add a new graph for the given parameters

    mContentsWidget->graphPanelsWidget()->activeGraphPanel()->addGraph(new GraphPanelWidget::GraphPanelPlotGraph(pParameterX, pParameterY));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                          OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph)
{
    // A new graph has been added, so keep track of it and update its plot
    // Note: updating the plot will, if needed, update the plot's axes and, as
    //       a result, replot the graphs including our new one. On the other
    //       hand, if the plot's axes don't get updated, we need to draw our new
    //       graph...

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    updateGraphData(pGraph, mSimulation->results()->size());

    if (updatePlot(plot) || plot->drawGraphFrom(pGraph, 0)) {
        QCoreApplication::processEvents();
        // Note: needProcessingEvents is used to ensure that our plot is updated
        //       at once...
    }

    // Keep track of the plot itself, if needed

    if (!mPlots.contains(plot))
        mPlots << plot;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                             const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs)
{
    Q_UNUSED(pGraphs);

    // One or several graphs have been removed, so update and stop tracking (if
    // needed) their corresponding plot
    // Note: even if the axes' values of the plot haven't changed, we still want
    //       to replot the plot since at least one of its graphs has been
    //       removed...

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    updatePlot(plot, true);

    QCoreApplication::processEvents();
    // Note: this ensures that our plot is updated at once...

    if (plot->graphs().isEmpty())
        mPlots.removeOne(plot);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphsUpdated(OpenCOR::GraphPanelWidget::GraphPanelPlotWidget *pPlot,
                                                             const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs)
{
    Q_UNUSED(pPlot);

    // One or several graphs have been updated, so make sure that their
    // corresponding plots are up to date

    GraphPanelWidget::GraphPanelPlotWidgets plots = GraphPanelWidget::GraphPanelPlotWidgets();

    foreach (GraphPanelWidget::GraphPanelPlotGraph *graph, pGraphs) {
        // Show/hide the graph

        graph->setVisible(graph->isValid() && graph->isSelected());

        // Update the graph's data
        // Note: we need to check that we have a simulation since our graph may
        //       indeed refer to a file that has not yet been activated and
        //       therefore doesn't yet have a simulation associated with it...

        SimulationExperimentViewSimulation *simulation = mViewWidget->simulation(graph->fileName());

        updateGraphData(graph, simulation?simulation->results()->size():0);

        // Keep track of the plot that needs to be updated and replotted

        plots << qobject_cast<GraphPanelWidget::GraphPanelPlotWidget *>(graph->plot());
    }

    // Update and replot our various plots, if allowed

    if (mCanUpdatePlotsForUpdatedGraphs) {
        foreach (GraphPanelWidget::GraphPanelPlotWidget *plot, plots) {
            updatePlot(plot, true);
            // Note: even if the axes' values of the plot haven't changed, we
            //       still want to replot the plot since at least one of its
            //       graphs has been updated...
        }

        QCoreApplication::processEvents();
        // Note: this ensures that our plots are all updated at once...
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::checkAxisValue(double &pValue,
                                                              const double &pOrigValue,
                                                              const QList<double> &pTestValues)
{
    // Check whether pOrigValue is equal to one of the values in pTestValues and
    // if so then update pValue with pOrigValue

    foreach (const double &testValue, pTestValues) {
        if (pOrigValue == testValue) {
            pValue = pOrigValue;

            break;
        }
    }
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::updatePlot(GraphPanelWidget::GraphPanelPlotWidget *pPlot,
                                                          const bool &pForceReplot)
{
    // Retrieve the current axes' values or use some default ones, if none are
    // available

    bool hasAxesValues = false;

    double minX = GraphPanelWidget::DefMinAxis;
    double maxX = GraphPanelWidget::DefMaxAxis;
    double minY = GraphPanelWidget::DefMinAxis;
    double maxY = GraphPanelWidget::DefMaxAxis;

    QRectF dataRect = pPlot->dataRect();

    if (dataRect != QRectF()) {
        minX = dataRect.left();
        maxX = minX+dataRect.width();
        minY = dataRect.top();
        maxY = minY+dataRect.height();

        hasAxesValues = true;
    }

    // Check all the graphs associated with the given plot and see whether any
    // of them uses the variable of integration as parameter X and/or Y, and if
    // so then asks the plot to use the starting/ending points as the
    // minimum/maximum values for the X and/or Y axes

    bool needInitialisationX = true;
    bool needInitialisationY = true;

    bool canOptimiseAxisX = true;
    bool canOptimiseAxisY = true;

    QList<double> startingPoints = QList<double>();
    QList<double> endingPoints = QList<double>();

    foreach (GraphPanelWidget::GraphPanelPlotGraph *graph, pPlot->graphs()) {
        if (graph->isValid() && graph->isSelected()) {
            SimulationExperimentViewSimulation *simulation = mViewWidget->simulation(graph->fileName());
            double startingPoint = simulation->data()->startingPoint();
            double endingPoint = simulation->data()->endingPoint();

            startingPoints << startingPoint;
            endingPoints << endingPoint;

            if (startingPoint > endingPoint) {
                // The starting point is greater than the ending point, so swap
                // the two of them

                startingPoint = simulation->data()->endingPoint();
                endingPoint = simulation->data()->startingPoint();
            }

            if (static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterX())->type() == CellMLSupport::CellmlFileRuntimeParameter::Voi) {
                if (!hasAxesValues && needInitialisationX) {
                    minX = startingPoint;
                    maxX = endingPoint;

                    needInitialisationX = false;
                } else {
                    minX = qMin(minX, startingPoint);
                    maxX = qMax(maxX, endingPoint);
                }

                canOptimiseAxisX = false;
            }

            if (static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterY())->type() == CellMLSupport::CellmlFileRuntimeParameter::Voi)
            {
                if (!hasAxesValues && needInitialisationY) {
                    minY = startingPoint;
                    maxY = endingPoint;

                    needInitialisationY = false;
                } else {
                    minY = qMin(minY, startingPoint);
                    maxY = qMax(maxY, endingPoint);
                }

                canOptimiseAxisY = false;
            }
        }
    }

    // Optimise our axes' values, if possible

    double origMinX = minX;
    double origMaxX = maxX;
    double origMinY = minY;
    double origMaxY = maxY;

    pPlot->optimiseAxisX(minX, maxX);
    pPlot->optimiseAxisY(minY, maxY);

    if (!canOptimiseAxisX) {
        checkAxisValue(minX, origMinX, startingPoints);
        checkAxisValue(maxX, origMaxX, endingPoints);
    }

    if (!canOptimiseAxisY) {
        checkAxisValue(minY, origMinY, startingPoints);
        checkAxisValue(maxY, origMaxY, endingPoints);
    }

    // Set our axes' values and replot our plot, if needed

    if (pPlot->setAxes(minX, maxX, minY, maxY, true, true, false)) {
        return true;
    } else if (pForceReplot) {
        pPlot->replot();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

double * SimulationExperimentViewSimulationWidget::dataPoints(SimulationExperimentViewSimulation *pSimulation,
                                                              CellMLSupport::CellmlFileRuntimeParameter *pParameter) const
{
    // Return the array of data points associated with the given parameter

    switch (pParameter->type()) {
    case CellMLSupport::CellmlFileRuntimeParameter::Voi:
        return pSimulation->results()->points();
    case CellMLSupport::CellmlFileRuntimeParameter::Constant:
    case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
        return pSimulation->results()->constants(pParameter->index());
    case CellMLSupport::CellmlFileRuntimeParameter::Rate:
        return pSimulation->results()->rates(pParameter->index());
    case CellMLSupport::CellmlFileRuntimeParameter::State:
        return pSimulation->results()->states(pParameter->index());
    case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
        return pSimulation->results()->algebraic(pParameter->index());
    default:
        // Not a relevant type, so return null
        // Note: we should never reach this point...

        return 0;
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateGraphData(GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                                                               const qulonglong &pSize)
{
    // Update our graph's data

    if (pGraph->isValid()) {
        SimulationExperimentViewSimulation *simulation = mViewWidget->simulation(pGraph->fileName());

        pGraph->setRawSamples(dataPoints(simulation, static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(pGraph->parameterX())),
                              dataPoints(simulation, static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(pGraph->parameterY())),
                              pSize);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateGui(const bool &pCheckVisibility)
{
    // Make sure that we are visible, if requested

    if (pCheckVisibility && !isVisible())
        return;

    // Make sure that our graphs widget's GUI is up to date

    mContentsWidget->informationWidget()->graphsWidget()->updateGui();

    // Make sure that our plots are up to date
    // Note: indeed, say that we start a simulation with some graphs and switch
    //       to another file before the simulation is complete. Now, if we
    //       switch back to the file after the simulation has completed, the
    //       plots won't be up to date (since the simulation widget was not
    //       visible and therefore didn't update its plots), so we need to do it
    //       here...

    if (mNeedUpdatePlots) {
        mNeedUpdatePlots = false;

        foreach (GraphPanelWidget::GraphPanelPlotWidget *plot, mPlots)
            updatePlot(plot, true);

        QCoreApplication::processEvents();
        // Note: this ensures that our plots are all updated at once...
    }

    // Make sure that our progress bar is up to date

    mProgressBarWidget->setValue(mViewWidget->simulationResultsSize(mFileName)/mSimulation->size());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSimulationResults(SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                       const qulonglong &pSimulationResultsSize,
                                                                       const bool &pClearGraphs)
{
    // Update the modified state of our simulation's corresponding file, if
    // needed
    // Note: normally, our simulation worker would, for each point interval,
    //       call
    //       SimulationExperimentViewSimulationData::checkForModifications(),
    //       but this would result in a signal being emitted (and then handled
    //       by
    //       SimulationExperimentViewSimulationWidget::simulationDataModified()),
    //       resulting in some time overhead, so we check things here instead...

    SimulationExperimentViewSimulation *simulation = pSimulationWidget->simulation();

    if (simulation == mSimulation)
        checkSimulationDataModified(simulation->data()->isModified());

    // Update all the graphs of all our plots, but only if we are visible
    // Note: needProcessingEvents is used to ensure that our plots are all
    //       updated at once...

    bool visible = isVisible();
    bool needProcessingEvents = false;

    foreach (GraphPanelWidget::GraphPanelPlotWidget *plot, mPlots) {
        // If our graphs are to be cleared (i.e. our plot's viewport are going
        // to be reset), then we want to be able to update our plot's viewport
        // if needed (i.e. a graph segment doesn't fit within our plot's current
        // viewport anymore)

        if (pClearGraphs)
            mUpdatablePlotViewports.insert(plot, true);

        // Now we are ready to actually update all the graphs of all our plots

        bool needUpdatePlot = false;

        double plotMinX = plot->minX();
        double plotMaxX = plot->maxX();
        double plotMinY = plot->minY();
        double plotMaxY = plot->maxY();

        foreach (GraphPanelWidget::GraphPanelPlotGraph *graph, plot->graphs()) {
            if (!graph->fileName().compare(pSimulationWidget->fileName())) {
                if (pClearGraphs)
                    mOldDataSizes.remove(graph);

                // Update our graph's data and keep track of our new old data
                // size, if we are visible
                // Note: indeed, to update our graph's old data size if we are
                //       not visible means that when we come back to this file,
                //       part of the graphs will be missing...

                qulonglong oldDataSize = graph->dataSize();

                if (visible)
                    mOldDataSizes.insert(graph, oldDataSize);

                updateGraphData(graph, pSimulationResultsSize);

                // We need to update our plot, if we are drawing this graph's
                // first segment or if we were invisible at some point during
                // the simulation

                qulonglong realOldDataSize = mOldDataSizes.value(graph);

                needUpdatePlot =    needUpdatePlot || !realOldDataSize
                                 || (oldDataSize != realOldDataSize);

                // Draw the graph's new segment, but only if we and our graph
                // are visible, and that there is no need to update the plot and
                // that there is some data to plot

                if (    visible && graph->isVisible()
                    && !needUpdatePlot && pSimulationResultsSize) {
                    // Check that our graph segment can fit within our plot's
                    // current viewport, but only if the user hasn't changed the
                    // plot's viewport since we last came here (e.g. by panning
                    // the plot's contents)

                    if (mUpdatablePlotViewports.value(plot)) {
                        double minX = plotMinX;
                        double maxX = plotMaxX;
                        double minY = plotMinY;
                        double maxY = plotMaxY;

                        for (qulonglong i = oldDataSize?oldDataSize-1:0;
                             i < pSimulationResultsSize; ++i) {
                            double valX = graph->data()->sample(i).x();
                            double valY = graph->data()->sample(i).y();

                            minX = qMin(minX, valX);
                            maxX = qMax(maxX, valX);
                            minY = qMin(minY, valY);
                            maxY = qMax(maxY, valY);
                        }

                        // Update our plot, if our graph segment cannot fit
                        // within our plot's current viewport

                        needUpdatePlot =    (minX < plotMinX) || (maxX > plotMaxX)
                                         || (minY < plotMinY) || (maxY > plotMaxY);
                    }

                    if (!needUpdatePlot) {
                        if (plot->drawGraphFrom(graph, realOldDataSize-1))
                            needProcessingEvents = true;
                    }
                }
            }
        }

        // Check whether we need to update/replot our plot, but only if we are
        // visible

        if (visible) {
            if (needUpdatePlot) {
                // We are either drawing a graph's first segment or its new
                // segment doesn't fit within the plot's current viewport, in
                // which case we need to update our plot

                updatePlot(plot, true);

                needProcessingEvents = true;
            } else if (!pSimulationResultsSize) {
                // We came here as a result of starting a simulation or clearing
                // our plot, so simply replot it (rather than update it)
                // Note: we don't want to update our plot since this is going to
                //       reset its axes' values and therefore result in some
                //       (expected) flickering, if some data is to be drawn
                //       straightaway (e.g. when we start a simulation)...

                plot->replot();

                needProcessingEvents = true;
            }
        } else if (needUpdatePlot || !pSimulationResultsSize) {
            // We would normally update our plot, but we are not visible, so no
            // point in doing it, so instead we keep track of the fact that we
            // will need to update our plots the next time we become visible

            mNeedUpdatePlots = true;
        }
    }

    // Process events, if needed

    if (needProcessingEvents)
        QCoreApplication::processEvents();

    // Update our progress bar or our tab icon, if needed

    if (simulation == mSimulation) {
        double simulationProgress = mViewWidget->simulationResultsSize(mFileName)/simulation->size();

        if (pClearGraphs || visible) {
            mProgressBarWidget->setValue(simulationProgress);
        } else {
            // We are not visible, so create an icon that shows our simulation's
            // progress and let people know about it

            int newProgress = (tabBarPixmapSize()-2)*simulationProgress;
            // Note: tabBarPixmapSize()-2 because we want a one-pixel wide
            //       border...

            if (newProgress != mProgress) {
                // The progress has changed, so keep track of its new value and
                // update our file tab icon

                mProgress = newProgress;

                // Let people know about the file tab icon to be used for the
                // model

                emit mViewWidget->updateFileTabIcon(mPlugin->viewName(), mFileName, fileTabIcon());
            }
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::openCellmlFile()
{
    // Ask OpenCOR to open our referenced CellML file

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QString cellmlFileName = mCellmlFile->fileName();

    QDesktopServices::openUrl(QString("opencor://openFile/%1").arg(fileManagerInstance->isRemote(cellmlFileName)?
                                                                       fileManagerInstance->url(cellmlFileName):
                                                                       cellmlFileName));

    // Ask OpenCOR to switch to the requested CellML editing view

    Plugin *plugin = mCellmlBasedViewPlugins.value(qobject_cast<QAction *>(sender()));

    if (plugin)
        QDesktopServices::openUrl("opencor://Core.selectView/"+plugin->name());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::plotAxesChanged()
{
    // A plot has had its axes changed (e.g. its contents was panned), which
    // means that we don't want to allow its viewport to be updated anymore

    GraphPanelWidget::GraphPanelPlotWidget *plot = qobject_cast<GraphPanelWidget::GraphPanelPlotWidget *>(sender());

    mUpdatablePlotViewports.insert(plot, false);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreExportDone(const QString &pErrorMessage)
{
    // We are done with the export, so hide our busy widget

    Core::centralWidget()->hideBusyWidget();

    // Display the given error message, if any

    if (!pErrorMessage.isEmpty()) {
        Core::warningMessageBox(tr("Simulation Data Export"),
                                pErrorMessage);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreExportProgress(const double &pProgress)
{
    // There has been some progress with our export, so update our busy widget

    Core::centralWidget()->setBusyWidgetProgress(pProgress);
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::sedmlAlgorithmSupported(const libsedml::SedAlgorithm *pSedmlAlgorithm,
                                                                       SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const
{
    // Make sure that we have an algorithm

    if (!pSedmlAlgorithm) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with one or two simulations with an algorithm are supported"));

        return false;
    }

    // Make sure that the given algorithm relies on an algorithm that we support

    SolverInterface *usedSolverInterface = 0;
    QString kisaoId = QString::fromStdString(pSedmlAlgorithm->getKisaoID());

    foreach (SolverInterface *solverInterface, Core::solverInterfaces()) {
        if (!solverInterface->id(kisaoId).compare(solverInterface->solverName())) {
            usedSolverInterface = solverInterface;

            break;
        }
    }

    if (!usedSolverInterface) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("unsupported algorithm (%1)").arg(kisaoId));

        return false;
    }

    // Make sure that the algorithm parameters are also supported

    for (int i = 0, iMax = pSedmlAlgorithm->getNumAlgorithmParameters(); i < iMax; ++i) {
        QString kisaoId = QString::fromStdString(pSedmlAlgorithm->getAlgorithmParameter(i)->getKisaoID());
        QString id = usedSolverInterface->id(kisaoId);

        if (id.isEmpty() || !id.compare(usedSolverInterface->solverName())) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("unsupported algorithm parameter (%1)").arg(kisaoId));

            return false;
        }
    }

    // Make sure that the annotation, if any, contains at least the kind of
    // information we would expect

    libsbml::XMLNode *annotation = pSedmlAlgorithm->getAnnotation();

    if (annotation) {
        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const XMLNode &node = annotation->getChild(i);

            if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                || QString::fromStdString(node.getName()).compare(SEDMLSupport::SolverProperties)) {
                continue;
            }

            bool validSolverProperties = true;

            for (uint j = 0, jMax = node.getNumChildren(); j < jMax; ++j) {
                const XMLNode &solverPropertyNode = node.getChild(j);

                if (   QString::fromStdString(solverPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                    || QString::fromStdString(solverPropertyNode.getName()).compare(SEDMLSupport::SolverProperty)) {
                    continue;
                }

                int idIndex = solverPropertyNode.getAttrIndex(SEDMLSupport::SolverPropertyId.toStdString());
                int valueIndex = solverPropertyNode.getAttrIndex(SEDMLSupport::SolverPropertyValue.toStdString());

                if (   (idIndex == -1) || (valueIndex == -1)
                    || solverPropertyNode.getAttrValue(idIndex).empty()
                    || solverPropertyNode.getAttrValue(valueIndex).empty()) {
                    validSolverProperties = false;

                    break;
                }
            }

            if (!validSolverProperties) {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                 tr("incomplete algorithm annotation (missing algorithm property information)"));

                return false;
            }
        }
    }

    return true;
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::sedmlFileSupported(SEDMLSupport::SedmlFile *pSedmlFile,
                                                                  SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const
{
    // Make sure that there is only one model

    libsedml::SedDocument *sedmlDocument = pSedmlFile->sedmlDocument();

    if (sedmlDocument->getNumModels() != 1) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with one model are supported"));

        return false;
    }

    // Make sure that the model is of CellML type

    libsedml::SedModel *model = sedmlDocument->getModel(0);
    QString language = QString::fromStdString(model->getLanguage());

    if (   language.compare(SEDMLSupport::Language::Cellml)
        && language.compare(SEDMLSupport::Language::Cellml_1_0)
        && language.compare(SEDMLSupport::Language::Cellml_1_1)) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with a CellML file are supported"));

        return false;
    }

    // Make sure that there is either one or two simulations

    int nbOfSimulations = sedmlDocument->getNumSimulations();

    if ((nbOfSimulations != 1) && (nbOfSimulations != 2)) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with one or two simulations are supported"));

        return false;
    }

    // Make sure that the first simulation is a uniform time course simulation

    libsedml::SedSimulation *firstSimulation = sedmlDocument->getSimulation(0);

    if (firstSimulation->getTypeCode() != libsedml::SEDML_SIMULATION_UNIFORMTIMECOURSE) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with a uniform time course as a (first) simulation are supported"));

        return false;
    }

    // Make sure that the initial time and output start time are the same, that
    // the output start time and output end time are different, and that the
    // number of points is greater than zero

    libsedml::SedUniformTimeCourse *uniformTimeCourse = static_cast<libsedml::SedUniformTimeCourse *>(firstSimulation);
    double initialTime = uniformTimeCourse->getInitialTime();
    double outputStartTime = uniformTimeCourse->getOutputStartTime();
    double outputEndTime = uniformTimeCourse->getOutputEndTime();
    int nbOfPoints = uniformTimeCourse->getNumberOfPoints();

    if (initialTime != outputStartTime) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with the same values for initialTime and outputStartTime are supported"));

        return false;
    }

    if (outputStartTime == outputEndTime) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                         tr("the values for outputStartTime and outputEndTime must be different"));

        return false;
    }

    if (nbOfPoints <= 0) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                         tr("the value for numberOfPoints must be greater than zero"));

        return false;
    }

    // Make sure that the algorithm used for the first simulation is supported

    if (!sedmlAlgorithmSupported(firstSimulation->getAlgorithm(), pSedmlFileIssues))
        return false;

    // Make sure that the annotation, if any, contains at least the kind of
    // information we would expect

    libsbml::XMLNode *firstSimulationAnnotation = firstSimulation->getAnnotation();

    if (firstSimulationAnnotation) {
        bool hasNlaSolver = false;

        for (uint i = 0, iMax = firstSimulationAnnotation->getNumChildren(); i < iMax; ++i) {
            const libsbml::XMLNode &node = firstSimulationAnnotation->getChild(i);

            if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                || QString::fromStdString(node.getName()).compare(SEDMLSupport::NlaSolver)) {
                continue;
            }

            int nameIndex = node.getAttrIndex(SEDMLSupport::NlaSolverName.toStdString());

            if ((nameIndex != -1) && !node.getAttrValue(nameIndex).empty()) {
                if (hasNlaSolver) {
                    pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                     tr("only one NLA solver is allowed"));

                    return false;
                } else {
                    hasNlaSolver = true;
                }
            } else {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                 tr("incomplete simulation annotation (missing NLA solver name)"));

                return false;
            }
        }
    }

    // Check whether there is a second simulation

    libsedml::SedSimulation *secondSimulation = sedmlDocument->getSimulation(1);

    if (secondSimulation) {
        // Make sure that the second simulation is a one-step simulation

        if (secondSimulation->getTypeCode() != libsedml::SEDML_SIMULATION_ONESTEP) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with a one-step as a second simulation are supported"));

            return false;
        }

        // Make sure that its step is greater than zero

        if (static_cast<libsedml::SedOneStep *>(secondSimulation)->getStep() <= 0) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                             tr("the value for step must be greater than zero"));

            return false;
        }

        // Make sure that its algorithm and annotation, if any, is the same as
        // for the first simulation

        std::stringstream firstStream;
        std::stringstream secondStream;
        libsbml::XMLOutputStream firstXmlStream(firstStream);
        libsbml::XMLOutputStream secondXmlStream(secondStream);

        firstSimulation->getAlgorithm()->write(firstXmlStream);

        if (secondSimulation->getAlgorithm())
            secondSimulation->getAlgorithm()->write(secondXmlStream);

        libsbml::XMLNode *secondSimulationAnnotation = secondSimulation->getAnnotation();

        if (firstSimulationAnnotation)
            firstSimulationAnnotation->write(firstXmlStream);

        if (secondSimulationAnnotation)
            secondSimulationAnnotation->write(secondXmlStream);

        if (firstStream.str().compare(secondStream.str())) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with two simulations with the same algorithm are supported"));

            return false;
        }
    }

    // Make sure that we have only one repeated task, which aim is to execute
    // each simulation (using a sub-task) once

    uint totalNbOfTasks = secondSimulation?3:2;

    if (sedmlDocument->getNumTasks() != totalNbOfTasks) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files that execute one or two simulations once are supported"));

        return false;
    }

    libsedml::SedRepeatedTask *repeatedTask = 0;

    bool repeatedTaskOk = false;
    std::string repeatedTaskFirstSubTaskId = std::string();
    std::string repeatedTaskSecondSubTaskId = std::string();

    bool firstSubTaskOk = false;
    std::string firstSubTaskId = std::string();

    bool secondSubTaskOk = false;
    std::string secondSubTaskId = std::string();

    for (uint i = 0; i < totalNbOfTasks; ++i) {
        libsedml::SedTask *task = sedmlDocument->getTask(i);

        if (task->getTypeCode() == libsedml::SEDML_TASK_REPEATEDTASK) {
            // Make sure that the repeated task asks for the model to be reset,
            // that it has one range, no task change and one/two sub-task/s

            repeatedTask = static_cast<libsedml::SedRepeatedTask *>(task);

            if (    repeatedTask->getResetModel()
                &&  (repeatedTask->getNumRanges() == 1)
                && !repeatedTask->getNumTaskChanges()
                &&  (repeatedTask->getNumSubTasks() == totalNbOfTasks-1)) {
                // Make sure that the range is a vector range and that it's the
                // one referenced in the repeated task

                libsedml::SedRange *range = repeatedTask->getRange(0);

                if (    (range->getTypeCode() == libsedml::SEDML_RANGE_VECTORRANGE)
                    && !repeatedTask->getRangeId().compare(range->getId())) {
                    // Make sure that the vector range has one value that is
                    // equal to 1

                    libsedml::SedVectorRange *vectorRange = static_cast<libsedml::SedVectorRange *>(range);

                    if (   (vectorRange->getNumValues() == 1)
                        && (vectorRange->getValues().front() == 1)) {
                        // Make sure that the one/two sub-tasks have the correct
                        // order and retrieve their id

                        for (uint i = 0, iMax = totalNbOfTasks-1; i < iMax; ++i) {
                            libsedml::SedSubTask *subTask = repeatedTask->getSubTask(i);

                            if (subTask->getOrder() == 1)
                                repeatedTaskFirstSubTaskId = subTask->getTask();
                            else if (subTask->getOrder() == 2)
                                repeatedTaskSecondSubTaskId = subTask->getTask();
                        }

                        repeatedTaskOk = true;
                    }
                }
            }
        } else if (task->getTypeCode() == libsedml::SEDML_TASK) {
            // Make sure the sub-task references the correct model and
            // simulation

            if (   !task->getModelReference().compare(model->getId())
                && !task->getSimulationReference().compare(firstSimulation->getId())) {
                firstSubTaskOk = true;
                firstSubTaskId = task->getId();
            } else if (   secondSimulation
                       && !task->getModelReference().compare(model->getId())
                       && !task->getSimulationReference().compare(secondSimulation->getId())) {
                secondSubTaskOk = true;
                secondSubTaskId = task->getId();
            }
        }
    }

    if (   !repeatedTaskOk
        || !firstSubTaskOk || repeatedTaskFirstSubTaskId.compare(firstSubTaskId)
        || (     secondSimulation
            && (!secondSubTaskOk || repeatedTaskSecondSubTaskId.compare(secondSubTaskId)))) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files that execute one or two simulations once are supported"));

        return false;
    }

    // Make sure that all the data generators have one variable that references
    // the repeated task, that follows the correct CellML format for their
    // target (and OpenCOR format for their degree, if any), and that is not
    // modified

    for (uint i = 0, iMax = sedmlDocument->getNumDataGenerators(); i < iMax; ++i) {
        libsedml::SedDataGenerator *dataGenerator = sedmlDocument->getDataGenerator(i);

        if ((dataGenerator->getNumVariables() != 1) || dataGenerator->getNumParameters()) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable are supported"));

            return false;
        }

        libsedml::SedVariable *variable = dataGenerator->getVariable(0);

        if (variable->getSymbol().size() || variable->getModelReference().size()) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable with a target and a task reference are supported"));

            return false;
        }

        if (variable->getTaskReference().compare(repeatedTask->getId())) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable with a reference to a repeated task are supported"));

            return false;
        }

        static const QRegularExpression TargetStartRegEx  = QRegularExpression("^\\/cellml:model\\/cellml:component\\[@name='");
        static const QRegularExpression TargetMiddleRegEx = QRegularExpression("']\\/cellml:variable\\[@name='");
        static const QRegularExpression TargetEndRegEx    = QRegularExpression("'\\]$");

        bool referencingCellmlVariable = false;
        QString target = QString::fromStdString(variable->getTarget());

        if (target.contains(TargetStartRegEx) && target.contains(TargetEndRegEx)) {
            static const QString Separator = "|";

            target.remove(TargetStartRegEx);
            target.replace(TargetMiddleRegEx, Separator);
            target.remove(TargetEndRegEx);

            QStringList identifiers = target.split(Separator);

            if (identifiers.count() == 2) {
                static const QRegularExpression IdentifierRegEx = QRegularExpression("^[[:alpha:]_][[:alnum:]_]*$");

                QString componentName = identifiers.first();
                QString variableName = identifiers.last();

                referencingCellmlVariable =    IdentifierRegEx.match(componentName).hasMatch()
                                            && IdentifierRegEx.match(variableName).hasMatch();
            }
        }

        if (!referencingCellmlVariable) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable with a reference to a CellML variable are supported"));

            return false;
        }

        libsbml::XMLNode *annotation = variable->getAnnotation();

        if (annotation) {
            for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
                const XMLNode &node = annotation->getChild(i);

                if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                    || QString::fromStdString(node.getName()).compare(SEDMLSupport::VariableDegree)) {
                    continue;
                }

                bool validVariableDegree = false;

                if (node.getNumChildren() == 1) {
                    bool conversionOk;
                    int variableDegree = QString::fromStdString(node.getChild(0).getCharacters()).toInt(&conversionOk);

                    validVariableDegree = conversionOk && (variableDegree >= 0);
                }

                if (!validVariableDegree) {
                    pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                     tr("only SED-ML files with data generators for one variable that is derived or not are supported"));

                    return false;
                }
            }
        }

        const libsbml::ASTNode *mathNode = dataGenerator->getMath();

        if (   (mathNode->getType() != libsbml::AST_NAME)
            || variable->getId().compare(mathNode->getName())) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable that is not modified are supported"));

            return false;
        }
    }

    // Make sure that all the outputs are 2D outputs

    for (uint i = 0, iMax = sedmlDocument->getNumOutputs(); i < iMax; ++i) {
        libsedml::SedOutput *output = sedmlDocument->getOutput(i);

        if (output->getTypeCode() != libsedml::SEDML_OUTPUT_PLOT2D) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with 2D outputs are supported"));

            return false;
        }

        // Make sure that the curves reference listed data generators and don't
        // use logarithmic axes

        libsedml::SedPlot2D *plot = static_cast<libsedml::SedPlot2D *>(output);

        for (uint j = 0, jMax = plot->getNumCurves(); j < jMax; ++j) {
            libsedml::SedCurve *curve = plot->getCurve(j);

            if (curve->getLogX() || curve->getLogY()) {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                 tr("only SED-ML files with linear 2D outputs are supported"));

                return false;
            }

            if (   !sedmlDocument->getDataGenerator(curve->getXDataReference())
                || !sedmlDocument->getDataGenerator(curve->getYDataReference())) {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                 tr("a curve must reference existing data generators"));

                return false;
            }
        }
    }

    return true;
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::combineArchiveSupported(COMBINESupport::CombineArchive *pCombineArchive,
                                                                       COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues) const
{
    // Load and make sure that our COMBINE archive is valid

    if (   !pCombineArchive->load()
        || !pCombineArchive->isValid(pCombineArchiveIssues)) {
        return false;
    }

    // Make sure that there is only one master file in our COMBINE archive

    if (pCombineArchive->masterFiles().count() != 1) {
        pCombineArchiveIssues << COMBINESupport::CombineArchiveIssue(COMBINESupport::CombineArchiveIssue::Information,
                                                                     tr("only COMBINE archives with one master file are supported"));

        return false;
    }

    return true;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::retrieveCellmlFile(const QString &pFileName,
                                                                  CellMLSupport::CellmlFile *&pCellmlFile,
                                                                  SEDMLSupport::SedmlFile *pSedmlFile,
                                                                  COMBINESupport::CombineArchive *pCombineArchive,
                                                                  const FileType &pFileType,
                                                                  SEDMLSupport::SedmlFileIssues &pSedmlFileIssues)
{
    // Make sure that we support our SED-ML file

    if (!sedmlFileSupported(pSedmlFile, pSedmlFileIssues))
        return;

    // Retrieve the source of the CellML file, if any, referenced in our SED-ML
    // file

    QString modelSource = QString::fromStdString(pSedmlFile->sedmlDocument()->getModel(0)->getSource());

    // Check whether we are dealing with a local file (which location is
    // relative to that of our SED-ML file) or a remote file

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QString url = fileManagerInstance->file(pFileName)->url();
    bool isLocalFile;
    QString dummy;

    Core::checkFileNameOrUrl(modelSource, isLocalFile, dummy);

    if (isLocalFile && (url.isEmpty() || pCombineArchive)) {
        // By default, our model source refers to a file name relative to our
        // SED-ML file

        QString cellmlFileName = Core::nativeCanonicalFileName(QFileInfo(pSedmlFile->fileName()).path()+QDir::separator()+modelSource);

#ifdef Q_OS_WIN
        // On Windows, if our model source exists, it means that it refers to a
        // file on a different drive rather than to a file name relative to our
        // SED-ML file

        if (QFile::exists(modelSource))
            cellmlFileName = modelSource;
#endif

        if (QFile::exists(cellmlFileName)) {
            pCellmlFile = new CellMLSupport::CellmlFile(cellmlFileName);

            // If possible, set our CellML file and its dependencies, if any, as
            // dependencies for our SED-ML file

            if (pFileType == SedmlFile) {
                Core::FileManager::instance()->setDependencies(pFileName,
                                                               QStringList() << pCellmlFile->fileName()
                                                                             << pCellmlFile->dependencies(true));
            }
        } else {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                             tr("%1 could not be found").arg(modelSource));
        }
    } else {
        // Handle the case where our model source is a relative remote file

        static const QRegularExpression FileNameRegEx = QRegularExpression("/[^/]*$");

        if (isLocalFile)
            modelSource = url.remove(FileNameRegEx)+"/"+modelSource;

        // Retrieve the contents of our model source

        QString fileContents;
        QString errorMessage;

        if (Core::readFileContentsFromUrlWithBusyWidget(modelSource, fileContents, &errorMessage)) {
            // Save the contents of our model source to a local file and use
            // that to create a CellML file object after having asked our file
            // manager to manage it (so that CellML 1.1 files can be properly
            // instantiated)
            // Note: we also keep track of our model source's local file since
            //       we will need to unmanage it when closing this file...

            QString cellmlFileName = Core::temporaryFileName();

            if (Core::writeFileContentsToFile(cellmlFileName, fileContents)) {
                fileManagerInstance->manage(cellmlFileName, Core::File::Remote, modelSource);

                pCellmlFile = new CellMLSupport::CellmlFile(cellmlFileName);

                mLocallyManagedCellmlFiles.insert(pFileName, cellmlFileName);
            } else {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                 tr("%1 could not be saved").arg(modelSource));
            }
        } else {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                             tr("%1 could not be retrieved (%2)").arg(modelSource, Core::formatMessage(errorMessage)));
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::retrieveSedmlFile(SEDMLSupport::SedmlFile *&pSedmlFile,
                                                                 COMBINESupport::CombineArchive *pCombineArchive,
                                                                 COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues)
{
    // Make sure that we support our COMBINE archive

    if (!combineArchiveSupported(pCombineArchive, pCombineArchiveIssues))
        return;

    // Create a SED-ML file object for our COMBINE archive's master file

    pSedmlFile = new SEDMLSupport::SedmlFile(pCombineArchive->masterFiles().first().fileName());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::retrieveFileDetails(const QString &pFileName,
                                                                   CellMLSupport::CellmlFile *&pCellmlFile,
                                                                   SEDMLSupport::SedmlFile *&pSedmlFile,
                                                                   COMBINESupport::CombineArchive *&pCombineArchive,
                                                                   FileType &pFileType,
                                                                   SEDMLSupport::SedmlFileIssues &pSedmlFileIssues,
                                                                   COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues)
{
    // Determine the type of file we are dealing with

    pCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);
    pSedmlFile = pCellmlFile?0:SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName);
    pCombineArchive = pSedmlFile?0:COMBINESupport::CombineFileManager::instance()->combineArchive(pFileName);

    pFileType = pCellmlFile?CellmlFile:pSedmlFile?SedmlFile:CombineArchive;

    // In the case of a COMBINE archive, we need to retrieve the corresponding
    // SED-ML file while, in the case of a SED-ML file, we need to retrieve the
    // corresponding CellML file

    pSedmlFileIssues.clear();
    pCombineArchiveIssues.clear();

    if (pCombineArchive)
        retrieveSedmlFile(pSedmlFile, pCombineArchive, pCombineArchiveIssues);

    if (pSedmlFile) {
        retrieveCellmlFile(pFileName, pCellmlFile, pSedmlFile, pCombineArchive,
                           pFileType, pSedmlFileIssues);
    }
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

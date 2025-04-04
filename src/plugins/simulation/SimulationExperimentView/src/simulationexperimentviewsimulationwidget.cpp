/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Simulation Experiment view simulation widget
//==============================================================================

#include "cellmlfile.h"
#include "combinearchive.h"
#include "combineinterface.h"
#include "combinesupportplugin.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "graphpanelswidget.h"
#include "interfaces.h"
#include "progressbarwidget.h"
#include "remotefiledialog.h"
#include "sedmlfile.h"
#include "sedmlinterface.h"
#include "sedmlsupport.h"
#include "sedmlsupportplugin.h"
#include "simulation.h"
#include "simulationexperimentviewcontentswidget.h"
#include "simulationexperimentviewinformationgraphpanelandgraphswidget.h"
#include "simulationexperimentviewinformationparameterswidget.h"
#include "simulationexperimentviewinformationsimulationwidget.h"
#include "simulationexperimentviewinformationsolverswidget.h"
#include "simulationexperimentviewinformationwidget.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewpreferenceswidget.h"
#include "simulationexperimentviewsedmlsupport.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"
#include "simulationmanager.h"
#include "toolbarwidget.h"
#include "toolbarwidgetdropdownlistwidgetaction.h"
#include "toolbarwidgetlabelwidgetaction.h"
#include "toolbarwidgetwheelwidgetaction.h"
#include "usermessagewidget.h"

//==============================================================================

#include <QDesktopServices>
#include <QDir>
#include <QDragEnterEvent>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMimeData>
#include <QPainter>
#include <QScrollBar>
#include <QTextEdit>
#include <QTimer>
#include <QToolButton>

//==============================================================================

#include <memory>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_wheel.h"
#include "qwtend.h"

//==============================================================================

#include "libsedmlbegin.h"
    #include "sedml/SedAlgorithm.h"
    #include "sedml/SedCurve.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedOneStep.h"
    #include "sedml/SedPlot2D.h"
    #include "sedml/SedRepeatedTask.h"
    #include "sedml/SedTask.h"
    #include "sedml/SedUniformTimeCourse.h"
    #include "sedml/SedVectorRange.h"
#include "libsedmlend.h"

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
    mViewWidget(pViewWidget)
{
    // Ask our simulation manager to manage our file and then retrieve the
    // corresponding simulation from it

    SimulationSupport::SimulationManager *simulationManager = SimulationSupport::SimulationManager::instance();

    simulationManager->manage(pFileName);

    mSimulation = simulationManager->simulation(pFileName);

    connect(mSimulation, &SimulationSupport::Simulation::running,
            this, &SimulationExperimentViewSimulationWidget::simulationRunning);
    connect(mSimulation, &SimulationSupport::Simulation::paused,
            this, &SimulationExperimentViewSimulationWidget::simulationPaused);

    connect(mSimulation, &SimulationSupport::Simulation::done,
            this, &SimulationExperimentViewSimulationWidget::simulationDone);

    connect(mSimulation, &SimulationSupport::Simulation::error,
            this, QOverload<const QString &>::of(&SimulationExperimentViewSimulationWidget::simulationError));

    connect(mSimulation->data(), &SimulationSupport::SimulationData::dataModified,
            this, &SimulationExperimentViewSimulationWidget::simulationDataModified);

    connect(mSimulation->results(), &SimulationSupport::SimulationResults::resultsReset,
            this, &SimulationExperimentViewSimulationWidget::simulationResultsReset);
    connect(mSimulation->results(), &SimulationSupport::SimulationResults::runAdded,
            this, &SimulationExperimentViewSimulationWidget::simulationResultsRunAdded);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create a tool bar

    mToolBarWidget = new ToolBarWidget::ToolBarWidget(this);

    // Create, customise and handle various actions

    static const QIcon ResetIcon = QIcon(":/oxygen/actions/view-refresh.png");

    mRunPauseResumeSimulationAction = Core::newAction(QIcon(":/oxygen/actions/media-playback-start.png"),
                                                      Qt::Key_F9, mToolBarWidget);
    mStopSimulationAction = Core::newAction(QIcon(":/oxygen/actions/media-playback-stop.png"),
                                            QKeySequence(Qt::ControlModifier|Qt::Key_F2), mToolBarWidget);
    mResetStateModelParametersAction = Core::newAction(Core::tintedIcon(ResetIcon, Qt::darkBlue),
                                                        mToolBarWidget);
    mResetAllModelParametersAction = Core::newAction(ResetIcon, mToolBarWidget);
    mClearSimulationResultsAction = Core::newAction(QIcon(":/oxygen/actions/trash-empty.png"),
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
    mSedmlExportSedmlFileAction = (mSimulation->fileType() == SimulationSupport::Simulation::FileType::CellmlFile)?
                                      Core::newAction(mToolBarWidget):
                                      nullptr;
    mSedmlExportCombineArchiveAction = (mSimulation->fileType() != SimulationSupport::Simulation::FileType::CombineArchive)?
                                           Core::newAction(mToolBarWidget):
                                           nullptr;
    mDataImportAction = Core::newAction(QIcon(":/oxygen/actions/document-import.png"),
                                        mToolBarWidget);
    mLocalDataImportAction = Core::newAction(mToolBarWidget);
    mRemoteDataImportAction = Core::newAction(mToolBarWidget);
    mSimulationResultsExportAction = Core::newAction(QIcon(":/oxygen/actions/document-export.png"),
                                                     mToolBarWidget);
    mPreferencesAction = Core::newAction(QIcon(":/oxygen/categories/preferences-system.png"),
                                         mToolBarWidget);

    mCellmlOpenAction->setEnabled(mSimulation->fileType() != SimulationSupport::Simulation::FileType::CellmlFile);
    mSedmlExportAction->setEnabled(mSimulation->fileType() != SimulationSupport::Simulation::FileType::CombineArchive);

    connect(mRunPauseResumeSimulationAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::runPauseResumeSimulation);
    connect(mStopSimulationAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::stopSimulation);
    connect(mResetStateModelParametersAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::resetStateModelParameters);
    connect(mResetAllModelParametersAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::resetAllModelParameters);
    connect(mClearSimulationResultsAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::clearSimulationResults);
    connect(mDevelopmentModeAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::developmentMode);
    connect(mAddGraphPanelAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::addGraphPanel);
    connect(mRemoveGraphPanelAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::removeGraphPanel);
    connect(mRemoveCurrentGraphPanelAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::removeCurrentGraphPanel);
    connect(mRemoveAllGraphPanelsAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::removeAllGraphPanels);
    connect(mCellmlOpenAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::openCellmlFile);
    connect(mSedmlExportAction, &QAction::triggered,
            this, (mSedmlExportSedmlFileAction != nullptr)?
                      QOverload<>::of(&SimulationExperimentViewSimulationWidget::sedmlExportSedmlFile):
                      QOverload<>::of(&SimulationExperimentViewSimulationWidget::sedmlExportCombineArchive));

    if (mSedmlExportSedmlFileAction != nullptr) {
        connect(mSedmlExportSedmlFileAction, &QAction::triggered,
                this, QOverload<>::of(&SimulationExperimentViewSimulationWidget::sedmlExportSedmlFile));
    }

    if (mSedmlExportCombineArchiveAction != nullptr) {
        connect(mSedmlExportCombineArchiveAction, &QAction::triggered,
                this, QOverload<>::of(&SimulationExperimentViewSimulationWidget::sedmlExportCombineArchive));
    }

    connect(mDataImportAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::localDataImport);
    connect(mLocalDataImportAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::localDataImport);
    connect(mRemoteDataImportAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::remoteDataImport);
    connect(mPreferencesAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::preferences);

    // Enable/disable our development mode action depending on whether our file
    // is readable/writable and of CellML type

    mDevelopmentModeAction->setEnabled(   Core::FileManager::instance()->isReadableAndWritable(pFileName)
                                       && (mSimulation->fileType() == SimulationSupport::Simulation::FileType::CellmlFile));

    // Create various drop-down menus for our drop-down list widget actions

    mRemoveGraphPanelDropDownMenu = new QMenu(mToolBarWidget);

    mRemoveGraphPanelDropDownMenu->addAction(mRemoveCurrentGraphPanelAction);
    mRemoveGraphPanelDropDownMenu->addAction(mRemoveAllGraphPanelsAction);

    mCellmlOpenDropDownMenu = new QMenu(mToolBarWidget);

    const Plugins cellmlEditingPlugins = pViewWidget->cellmlEditingViewPlugins();

    for (auto cellmlEditingViewPlugin : cellmlEditingPlugins) {
        QAction *action = Core::newAction(Core::mainWindow());

        mCellmlOpenDropDownMenu->addAction(action);

        mCellmlBasedViewPlugins.insert(action, cellmlEditingViewPlugin);

        connect(action, &QAction::triggered,
                this, &SimulationExperimentViewSimulationWidget::openCellmlFile);
    }

    mCellmlOpenDropDownMenu->addSeparator();

    const Plugins cellmlSimulationViewPlugins = pViewWidget->cellmlSimulationViewPlugins();

    for (auto cellmlSimulationViewPlugin : cellmlSimulationViewPlugins) {
        QAction *action = Core::newAction(Core::mainWindow());

        mCellmlOpenDropDownMenu->addAction(action);

        mCellmlBasedViewPlugins.insert(action, cellmlSimulationViewPlugin);

        connect(action, &QAction::triggered,
                this, &SimulationExperimentViewSimulationWidget::openCellmlFile);
    }

    mSedmlExportDropDownMenu = new QMenu(mToolBarWidget);

    if (mSedmlExportSedmlFileAction != nullptr) {
        mSedmlExportDropDownMenu->addAction(mSedmlExportSedmlFileAction);
    }

    if (mSedmlExportCombineArchiveAction != nullptr) {
        mSedmlExportDropDownMenu->addAction(mSedmlExportCombineArchiveAction);
    }

    mDataImportDropDownMenu = new QMenu(mToolBarWidget);

    mDataImportDropDownMenu->addAction(mLocalDataImportAction);
    mDataImportDropDownMenu->addAction(mRemoteDataImportAction);

    mSimulationResultsExportDropDownMenu = new QMenu(mToolBarWidget);

    const DataStoreInterfaces dataStoreInterfaces = Core::dataStoreInterfaces();

    for (auto dataStoreInterface : dataStoreInterfaces) {
        QString dataStoreName = dataStoreInterface->dataStoreName();
        QAction *action = mSimulationResultsExportDropDownMenu->addAction(dataStoreName+"...");

        mDataStoreInterfaces.insert(action, dataStoreInterface);

        connect(action, &QAction::triggered,
                this, &SimulationExperimentViewSimulationWidget::simulationResultsExport);
    }

    updateDataStoreActions();

    // Add the various actions, wheel and tool buttons to our tool bar

    mToolBarWidget->addAction(mRunPauseResumeSimulationAction);
    mToolBarWidget->addAction(mStopSimulationAction);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mResetStateModelParametersAction);
    mToolBarWidget->addAction(mResetAllModelParametersAction);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mClearSimulationResultsAction);
    mToolBarWidget->addSeparator();

    mDelayWheelWidgetAction = mToolBarWidget->addWheelWidgetAction();

    connect(mDelayWheelWidgetAction, &ToolBarWidget::ToolBarWidgetWheelWidgetAction::created,
            this, &SimulationExperimentViewSimulationWidget::delayWheelCreated);
    connect(mDelayWheelWidgetAction, &ToolBarWidget::ToolBarWidgetWheelWidgetAction::valueChanged,
            this, &SimulationExperimentViewSimulationWidget::delayWheelValueChanged);
    connect(mDelayWheelWidgetAction, &ToolBarWidget::ToolBarWidgetWheelWidgetAction::wheelReleased,
            this, &SimulationExperimentViewSimulationWidget::delayWheelReleased);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mToolBarWidget->addSpacerWidgetAction(QSizePolicy::Fixed, QSizePolicy::Expanding);
#endif

    mDelayLabelValueWidgetAction = mToolBarWidget->addLabelWidgetAction();

    connect(mDelayLabelValueWidgetAction, &ToolBarWidget::ToolBarWidgetLabelWidgetAction::created,
            this, &SimulationExperimentViewSimulationWidget::delayValueCreated);

    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mDevelopmentModeAction);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mAddGraphPanelAction);

    mRemoveGraphPanelDropDownListWidgetAction = mToolBarWidget->addDropDownListWidgetAction(mRemoveGraphPanelAction,
                                                                                            mRemoveGraphPanelDropDownMenu);

    mToolBarWidget->addSeparator();

    mCellmlOpenDropDownListWidgetAction = mToolBarWidget->addDropDownListWidgetAction(mCellmlOpenAction,
                                                                                      mCellmlOpenDropDownMenu);

    mToolBarWidget->addSeparator();

    mSedmlExportDropDownListWidgetAction = mToolBarWidget->addDropDownListWidgetAction(mSedmlExportAction,
                                                                                       mSedmlExportDropDownMenu);

    mToolBarWidget->addSeparator();

    mDataImportDropDownListWidgetAction = mToolBarWidget->addDropDownListWidgetAction(mDataImportAction,
                                                                                      mDataImportDropDownMenu);
    mSimulationResultsExportDropDownListWidgetAction = mToolBarWidget->addDropDownListWidgetAction(mSimulationResultsExportAction,
                                                                                                   mSimulationResultsExportDropDownMenu);

    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mPreferencesAction);

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

    connect(mSplitterWidget, &Core::SplitterWidget::splitterMoved,
            this, &SimulationExperimentViewSimulationWidget::emitSplitterMoved);

    // Create our contents widget

    mContentsWidget = new SimulationExperimentViewContentsWidget(pViewWidget, this, this);

    mContentsWidget->setObjectName("Contents");

    // Keep track of changes to some of our simulation and solvers properties

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();

    connect(informationWidget->simulationWidget(), &SimulationExperimentViewInformationSimulationWidget::propertyChanged,
            this, &SimulationExperimentViewSimulationWidget::simulationPropertyChanged);
    connect(informationWidget->solversWidget(), &SimulationExperimentViewInformationSolversWidget::propertyChanged,
            this, &SimulationExperimentViewSimulationWidget::solversPropertyChanged);

    // Keep track of the addition and removal of a graph panel

    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();
    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = informationWidget->graphPanelAndGraphsWidget();

    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphPanelAdded,
            graphPanelAndGraphsWidget, QOverload<GraphPanelWidget::GraphPanelWidget *, const GraphPanelWidget::GraphPanelWidgetProperties &, bool>::of(&SimulationExperimentViewInformationGraphPanelAndGraphsWidget::initialize));
    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphPanelRemoved,
            graphPanelAndGraphsWidget, QOverload<GraphPanelWidget::GraphPanelWidget *>::of(&SimulationExperimentViewInformationGraphPanelAndGraphsWidget::finalize));

    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphPanelAdded,
            this, &SimulationExperimentViewSimulationWidget::graphPanelAdded);
    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphPanelRemoved,
            this, &SimulationExperimentViewSimulationWidget::graphPanelRemoved);

    // Keep track of whether a graph panel has been activated

    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphPanelActivated,
            graphPanelAndGraphsWidget, QOverload<GraphPanelWidget::GraphPanelWidget *>::of(&SimulationExperimentViewInformationGraphPanelAndGraphsWidget::initialize));

    // Keep track of whether a graph panel has been resized

    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::splitterMoved,
            this, &SimulationExperimentViewSimulationWidget::checkGraphPanelsAndGraphs);

    // Keep track of a graph being required

    connect(informationWidget->parametersWidget(), &SimulationExperimentViewInformationParametersWidget::graphRequired,
            this, &SimulationExperimentViewSimulationWidget::addGraph);

    // Keep track of the addition and removal of a graph

    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphAdded,
            graphPanelAndGraphsWidget, QOverload<GraphPanelWidget::GraphPanelWidget *, GraphPanelWidget::GraphPanelPlotGraph *, const GraphPanelWidget::GraphPanelPlotGraphProperties &>::of(&SimulationExperimentViewInformationGraphPanelAndGraphsWidget::addGraph));
    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphsRemoved,
            graphPanelAndGraphsWidget, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::removeGraphs);

    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphAdded,
            this, &SimulationExperimentViewSimulationWidget::graphAdded);
    connect(graphPanelsWidget, &GraphPanelWidget::GraphPanelsWidget::graphsRemoved,
            this, &SimulationExperimentViewSimulationWidget::graphsRemoved);

    // Keep track of the updating of a graph
    // Note: ideally, this would, as for the addition and removal of a graph
    //       (see above), be done through graphPanelsWidget (i.e. a graph would
    //       let people know that it has been updated and the graph panel with
    //       which it is associated would forward the signal to
    //       graphPanelsWidget), but this may result in too many graphUpdated()
    //       signals being emitted. For example, say that you change the model
    //       with which a graph is associated, then both the X and Y parameters
    //       will get updated, and for each of those updates a graphUpdated()
    //       signal would be emitted by the graph, hence we would end up with
    //       two signals when only one would have sufficed. Even worse is that
    //       after having updated the X parameter, the graph would have its X
    //       parameter coming from the 'new' model while its Y parameter from
    //       the 'old' model, which could mess things up quite a bit from a
    //       plotting viewpoint. So, instead, the updating is done through our
    //       graphs property editor...

    connect(graphPanelAndGraphsWidget, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphUpdated,
            this, &SimulationExperimentViewSimulationWidget::graphUpdated);
    connect(graphPanelAndGraphsWidget, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsUpdated,
            this, &SimulationExperimentViewSimulationWidget::graphsUpdated);

    // Create our simulation output widget with a layout on which we put a
    // separating line and our simulation output list view
    // Note: the separating line is because we remove, for aesthetical reasons,
    //       the border of our simulation output list view...

    auto simulationOutputWidget = new QWidget(this);
    auto simulationOutputLayout= new QVBoxLayout(simulationOutputWidget);

    simulationOutputLayout->setContentsMargins({});
    simulationOutputLayout->setSpacing(0);

    simulationOutputWidget->setLayout(simulationOutputLayout);

    mOutputWidget = new QTextEdit(this);

    mOutputWidget->setFrameStyle(QFrame::NoFrame);
    mOutputWidget->setReadOnly(true);

    simulationOutputLayout->addWidget(Core::newLineWidget(this));
    simulationOutputLayout->addWidget(mOutputWidget);

    // Populate our splitter and use as much space as possible for it by asking
    // for its height to be that of the desktop's, and then add our splitter to
    // our Simulation Experiment view widget

    mSplitterWidget->addWidget(mContentsWidget);
    mSplitterWidget->addWidget(simulationOutputWidget);

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

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

SimulationExperimentViewSimulationWidget::~SimulationExperimentViewSimulationWidget()
{
    // Ask our simulation manager to unmanage our file

    SimulationSupport::SimulationManager::instance()->unmanage(mSimulation->fileName());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mRunPauseResumeSimulationAction, tr("Run Simulation"),
                                     tr("Run the simulation"));
    I18nInterface::retranslateAction(mStopSimulationAction, tr("Stop Simulation"),
                                     tr("Stop the simulation"));
    I18nInterface::retranslateAction(mResetStateModelParametersAction, tr("Reset State Model Parameters"),
                                     tr("Reset the state model parameters"));
    I18nInterface::retranslateAction(mResetAllModelParametersAction, tr("Reset All Model Parameters"),
                                     tr("Reset all the model parameters"));
    I18nInterface::retranslateAction(mClearSimulationResultsAction, tr("Clear Simulation Results"),
                                     tr("Clear the simulation results"));
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
    I18nInterface::retranslateAction(mPreferencesAction, tr("Preferences"),
                                     tr("Preferences for the Simulation Experiment view"));

    if (mSedmlExportSedmlFileAction != nullptr) {
        I18nInterface::retranslateAction(mSedmlExportSedmlFileAction, tr("SED-ML File..."),
                                         tr("Export the simulation to SED-ML using a SED-ML file"));
    }

    if (mSedmlExportCombineArchiveAction != nullptr) {
        I18nInterface::retranslateAction(mSedmlExportCombineArchiveAction, tr("COMBINE Archive..."),
                                         tr("Export the simulation to SED-ML using a COMBINE archive"));
    }

    I18nInterface::retranslateAction(mDataImportAction, tr("Data Import"),
                                     tr("Import some data"));
    I18nInterface::retranslateAction(mLocalDataImportAction, tr("Local..."),
                                     tr("Import some local data"));
    I18nInterface::retranslateAction(mRemoteDataImportAction, tr("Remote..."),
                                     tr("Import some remote data"));
    I18nInterface::retranslateAction(mSimulationResultsExportAction, tr("Simulation Results Export"),
                                     tr("Export the simulation results"));

    // Retranslate our delay wheel and label widget actions

    updateDelayWheelWidgetAction();
    updateDelayLabelValueWidgetAction();

    // Retranslate our run/pause action

    updateRunPauseAction(mRunActionEnabled);

    // Retranslate our data store actions

    updateDataStoreActions();

    // Retranslate our CellML editing view actions

    const QList<QAction *> cellmlEditingViewActionKeys = mCellmlBasedViewPlugins.keys();

    for (auto cellmlEditingViewAction : cellmlEditingViewActionKeys) {
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

void SimulationExperimentViewSimulationWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    Core::Widget::changeEvent(pEvent);

    // Check whether we are becoming enabled/disabled, in which case we want to
    // update our output widget while keeping its scrollbars, if any, still at
    // the same position
    // Note: we used to do this only when we were visible, but in some cases
    //       (e.g. when running a simulation from our Python console) this may
    //       result in our output widget not being re-enabled. So, now, we
    //       want to update our output widget even when it's not visible.
    //       However, to be able to do so, we must have been visible at least
    //       once, hence our use of mCanHandleChangeEvent...

    if (isVisible()) {
        mCanHandleChangeEvent = true;
    }

    if (mCanHandleChangeEvent && (pEvent->type() == QEvent::EnabledChange)) {
        int horizontalSliderPosition = mOutputWidget->horizontalScrollBar()->sliderPosition();
        int verticalSliderPosition = mOutputWidget->verticalScrollBar()->sliderPosition();

        mOutputWidget->setHtml(styledOutput());

        mOutputWidget->horizontalScrollBar()->setSliderPosition(horizontalSliderPosition);
        mOutputWidget->verticalScrollBar()->setSliderPosition(verticalSliderPosition);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if it refers to one or
    // several data store files

    const QStringList fileNames = Core::droppedFileNames(pEvent);
    const FileTypeInterfaces fileTypeInterfaces = Core::dataStoreFileTypeInterfaces();
    bool acceptEvent = false;

    for (const auto &fileName : fileNames) {
        for (auto fileTypeInterface : fileTypeInterfaces) {
            if (fileTypeInterface->isFile(fileName)) {
                mFileTypeInterfaces.insert(fileName, fileTypeInterface);

                acceptEvent = true;

                break;
            }
        }
    }

    if (acceptEvent) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dropEvent(QDropEvent *pEvent)
{
    // Import/open the one or several files

    const QStringList fileNames = Core::droppedFileNames(pEvent);

    for (const auto &fileName : fileNames) {
        if (mFileTypeInterfaces.contains(fileName)) {
            import(fileName);
        } else {
            QDesktopServices::openUrl("opencor://openFile/"+fileName);
        }
    }

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateDataStoreActions()
{
    // Update our data store actions

    const QList<QAction *> actionKeys = mDataStoreInterfaces.keys();

    for (auto action : actionKeys) {
        I18nInterface::retranslateAction(action,
                                         action->text(),
                                         tr("Export the simulation results to %1").arg(mDataStoreInterfaces.value(action)->dataStoreName()));
    }
}

//==============================================================================

QString SimulationExperimentViewSimulationWidget::styledOutput()
{
    // Return a styled version of our output

    static const QString DefaultOutputMessage = "<style>"
                                                "    span.good {"
                                                "        color: %1;"
                                                "    }"
                                                ""
                                                "    span.info {"
                                                "        color: %2;"
                                                "    }"
                                                ""
                                                "    span.bad {"
                                                "        color: %3;"
                                                "    }"
                                                "</style>"
                                                "<body>"
                                                "    %4"
                                                "</body>";

    if (isEnabled()) {
        static const QString RedColor   = QColor(Qt::darkRed).name();
        static const QString GreenColor = QColor(Qt::darkGreen).name();
        static const QString BlueColor  = QColor(Qt::darkBlue).name();

        return DefaultOutputMessage.arg(GreenColor, BlueColor, RedColor,
                                        mOutputMessage);
    }

    QString windowTextColor = Core::windowTextColor(QPalette::Disabled).name(QColor::HexArgb);

    return DefaultOutputMessage.arg(windowTextColor, windowTextColor, windowTextColor,
                                    mOutputMessage);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::output(const QString &pMessage)
{
    // Update and set our output message after styling it

    mOutputMessage += pMessage;

    mOutputWidget->setHtml(styledOutput());

    mOutputWidget->moveCursor(QTextCursor::End);
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

    mClearSimulationResultsAction->setEnabled(    (mSimulation->results()->size() != 0)
                                              && !simulationModeEnabled);
    mSimulationResultsExportAction->setEnabled(   !mSimulationResultsExportDropDownMenu->actions().isEmpty()
                                               &&  (mSimulation->results()->size() != 0)
                                               && !simulationModeEnabled);

    // Give the focus to our focus proxy, in case we leave our simulation mode
    // (so that the user can modify simulation data, etc.)

    if (!simulationModeEnabled) {
        focusProxy()->setFocus();
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateRunPauseAction(bool pRunActionEnabled)
{
    // Update our run/pause action

    static const QIcon StartIcon = QIcon(":/oxygen/actions/media-playback-start.png");
    static const QIcon PauseIcon = QIcon(":/oxygen/actions/media-playback-pause.png");

    mRunActionEnabled = pRunActionEnabled;

    mRunPauseResumeSimulationAction->setIcon(pRunActionEnabled?StartIcon:PauseIcon);

    bool simulationPaused = (mSimulation != nullptr) && mSimulation->isPaused();

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

void SimulationExperimentViewSimulationWidget::updateDelayWidget(QWidget *pDelayWidget)
{
    // Update the given delay widget

    pDelayWidget->setToolTip(tr("Simulation Delay"));
    pDelayWidget->setStatusTip(tr("Delay between two data points"));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateDelayWheelWidgetAction()
{
    // Update our delay wheel widget action

    const QList<QwtWheel *> wheels = mDelayWheelWidgetAction->wheels();

    for (auto *wheel : wheels) {
        updateDelayWidget(wheel);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateDelayLabelValueWidgetAction()
{
    // Update our delay label value widget action

    delayWheelValueChanged(mDelayWheelValue);
    // Note: we do this because we want to display the delay using digit
    //       grouping, this respecting the current locale...

    const QList<QLabel *> labels = mDelayLabelValueWidgetAction->labels();

    for (auto *label : labels) {
        updateDelayWidget(label);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateInvalidModelMessageWidget()
{
    // Update our invalid model message

    mInvalidModelMessageWidget->setMessage((mError == Error::InvalidCellmlFile)?
                                               tr("The <strong>%1</strong> view requires a valid CellML file to work...").arg(mPlugin->viewName()):
                                               tr("The <strong>%1</strong> view requires a valid simulation environment to work...").arg(mPlugin->viewName()),
                                           tr("See below for more information."));
}

//==============================================================================

static const char *OutputTab  = "&nbsp;&nbsp;&nbsp;&nbsp;";
static const char *OutputGood = R"(class="good")";
static const char *OutputInfo = R"(class="info")";
static const char *OutputBad  = R"(class="bad")";
static const char *OutputBrLn = "<br/>\n";

//==============================================================================

void SimulationExperimentViewSimulationWidget::initialize(bool pReloading)
{
    // In the case of a SED-ML file and of a COMBINE archive, we will need
    // to further initialise ourselves, to customise graph panels, etc. (see
    // furtherInitialize()), so we ask our central widget to show its busy
    // widget (which will get hidden in CentralWidget::updateGui())

    bool isSedmlFile = mSimulation->fileType() == SimulationSupport::Simulation::FileType::SedmlFile;
    bool isCombineArchive = mSimulation->fileType() == SimulationSupport::Simulation::FileType::CombineArchive;

    if (isVisible() && (isSedmlFile || isCombineArchive)) {
        Core::showCentralBusyWidget();
    }

    processEvents();
    // Note: this ensures that our GUI is all fine before we start disabling
    //       updates (e.g. when reloading a SED-ML file that references a remote
    //       CellML file)...

    // Stop tracking certain things (so that updatePlot() doesn't get called
    // unnecessarily)
    // Note: see the corresponding code towards the end of this method...

    mContentsWidget->setUpdatesEnabled(false);
        SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
        SimulationExperimentViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();

        disconnect(simulationWidget, &SimulationExperimentViewInformationSimulationWidget::propertyChanged,
                   this, &SimulationExperimentViewSimulationWidget::simulationPropertyChanged);

        // Reset our progress

        mProgress = -1;

        // Clean up our output, if needed

        if (pReloading) {
            mOutputMessage = QString();
        }

        // Output some information about our CellML file

        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        QString simulationFileName = mSimulation->fileName();
        QString fileName = fileManagerInstance->isNew(simulationFileName)?
                               tr("File")+" #"+QString::number(fileManagerInstance->newIndex(simulationFileName)):
                               fileManagerInstance->isRemote(simulationFileName)?
                                   fileManagerInstance->url(simulationFileName):
                                   simulationFileName;
        QString information = "<strong>"+QDir::toNativeSeparators(fileName)+"</strong>"+OutputBrLn;
        const SimulationSupport::SimulationIssues simulationIssues = mSimulation->issues();
        CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();
        bool validRuntime = (runtime != nullptr) && runtime->isValid();
        CellMLSupport::CellmlFileRuntimeParameter *voi = validRuntime?runtime->voi():nullptr;

        if (!mSimulation->hasBlockingIssues()) {
            information += QString()+OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

            if (voi != nullptr) {
                // A VOI could be retrieved for our CellML file, so we can also
                // output the model type

                information +=  QString()+"<span "+OutputGood+">"+tr("valid")+"</span>."+OutputBrLn
                               +QString(QString()+OutputTab+"<strong>"+tr("Model type:")+"</strong> <span "+OutputInfo+">%1</span>."+OutputBrLn).arg(runtime->needNlaSolver()?tr("DAE"):tr("ODE"));
            } else {
                // We couldn't retrieve a VOI, which means that we either don't
                // have a runtime or we have one, but it's not valid or it's
                // valid but we really don't have a VOI
                // Note: in the case of a valid runtime and no VOI, we really
                //       shouldn't consider the runtime to be valid, hence we
                //       handle this case here...

                mError = Error::InvalidCellmlFile;

                updateInvalidModelMessageWidget();

                information += QString()+"<span "+OutputBad+">"+((runtime != nullptr)?tr("invalid"):tr("none"))+"</span>."+OutputBrLn;
            }
        }

        if (!simulationIssues.isEmpty()) {
            // There is one or several issues with our Simulation, so list
            // it/them

            for (const auto &simulationIssue : simulationIssues) {
                if ((simulationIssue.line() != 0) && (simulationIssue.column() != 0)) {
                    information +=  OutputTab
                                   +tr("<span %1><strong>[%2:%3] %4:</strong> %5.</span>").arg(OutputBad)
                                                                                          .arg(simulationIssue.line())
                                                                                          .arg(simulationIssue.column())
                                                                                          .arg(simulationIssue.typeAsString(),
                                                                                               Core::formatMessage(simulationIssue.message()))
                                   +OutputBrLn;
                } else {
                    information +=  OutputTab
                                   +tr("<span %1><strong>%2:</strong> %3.</span>").arg(OutputBad,
                                                                                       simulationIssue.typeAsString(),
                                                                                       Core::formatMessage(simulationIssue.message()))
                                   +OutputBrLn;
                }
            }
        }

        output(information);

        // Check whether we have a valid simulation environment, but only if we
        // don't have any blocking SED-ML or COMBINE issues

        SimulationExperimentViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();

        mValidSimulationEnvironment = false;

        if (!mSimulation->hasBlockingIssues()) {
            // Enable/disable our run/pause action depending on whether we have
            // a VOI

            mRunPauseResumeSimulationAction->setEnabled(voi != nullptr);

            // Clear our simulation results, in case we are reloading ourselves,
            // and update our simulation mode

            if (pReloading) {
                clearSimulationResults();
            }

            updateSimulationMode();

            // Initialise our contents widget and make sure that we have the
            // required type(s) of solvers

            if (voi != nullptr) {
                // Show our contents widget in case it got previously hidden
                // Note: indeed, if it was to remain hidden then some
                //       initialisations wouldn't work (e.g. the solvers widget
                //       has a property editor, which all properties need to be
                //       removed and if the contents widget is not visible, then
                //       upon repopulating the property editor, scrollbars will
                //       be shown even though they are not needed)...

                mContentsWidget->show();

                // Check whether we have at least one ODE or DAE solver and, if
                // needed, at least one NLA solver

                if (runtime->needNlaSolver()) {
                    if (solversWidget->nlaSolvers().isEmpty()) {
                        if (solversWidget->odeSolvers().isEmpty()) {
                            simulationError(tr("the model needs both an ODE and an NLA solver, but none are available"),
                                            Error::InvalidSimulationEnvironment);
                        } else {
                            simulationError(tr("the model needs both an ODE and an NLA solver, but no NLA solver is available"),
                                            Error::InvalidSimulationEnvironment);
                        }
                    } else if (solversWidget->odeSolvers().isEmpty()) {
                        simulationError(tr("the model needs both an ODE and an NLA solver, but no ODE solver is available"),
                                        Error::InvalidSimulationEnvironment);
                    } else {
                        mValidSimulationEnvironment = true;
                    }
                } else if (solversWidget->odeSolvers().isEmpty()) {
                    simulationError(tr("the model needs an ODE solver, but none is available"),
                                    Error::InvalidSimulationEnvironment);
                } else {
                    mValidSimulationEnvironment = true;
                }
            }
        }

        // Initialise our GUI based on whether we have a valid simulation
        // environment

        initializeGui(mValidSimulationEnvironment);

        // Some additional initialisations in case we have a valid simulation
        // environment

        if (mValidSimulationEnvironment) {
            // Initialise our GUI's simulation, solvers, graphs and graph panels
            // widgets, but not parameters widget
            // Note #1: this will also initialise some of our simulation data
            //          (i.e. our simulation's starting point and simulation's
            //          NLA solver's properties), which is needed since we want
            //          to be able to reset our simulation below...
            // Note #2: to initialise our graph panel and graphs widget will
            //          result in some graphs being shown/hidden and, therefore,
            //          in graphUpdated() being called. Yet, we don't want
            //          graphUpdated() to update our plots. Indeed, if it did,
            //          then all of our plots' axes' values would be reset while
            //          we want to keep the ones we just retrieved (thus making
            //          it possible for the user to have different views for
            //          different files). So, for this to work we use
            //          mCanUpdatePlotsForUpdatedGraphs, and then 'manually'
            //          replot our plots, once we know which graphs are to be
            //          shown/hidden. We could do the initialisation before the
            //          setting of the plots' axes' values, but then we could
            //          see the graphs being plotted twice. Once after the
            //          plots' axes' values have been reset following the call
            //          to graphUpdated() and another after we update our plots'
            //          axes' values. This is clearly not neat, hence the
            //          current solution...
            // Note #3: to initialise our parameters widget now would result in
            //          some default (in the computer science sense, i.e. wrong)
            //          values being visible for a split second before they get
            //          properly updated. So, instead, we initialise whatever
            //          needs initialising (e.g. NLA solver) so that we can
            //          safely compute our model parameters before showing their
            //          values...

            simulationWidget->initialize(mSimulation);
            solversWidget->initialize(mSimulation);

            mCanUpdatePlotsForUpdatedGraphs = false;
                informationWidget->graphPanelAndGraphsWidget()->initialize(mSimulation);
            mCanUpdatePlotsForUpdatedGraphs = true;

            mContentsWidget->graphPanelsWidget()->initialize(defaultGraphPanelProperties());

            // Initialise our simulation

            initializeSimulation();

            // Now, we can safely update our parameters widget since our model
            // parameters have been computed

            mContentsWidget->informationWidget()->parametersWidget()->initialize(mSimulation, pReloading);
        }

        // Resume the tracking of certain things
        // Note: see the corresponding code at the beginning of this method...

        connect(mContentsWidget->informationWidget()->simulationWidget(), &SimulationExperimentViewInformationSimulationWidget::propertyChanged,
                this, &SimulationExperimentViewSimulationWidget::simulationPropertyChanged);

        // Further initialise ourselves, if we have a valid environment and we are
        // not dealing with a CellML file

        if (mValidSimulationEnvironment && (isSedmlFile || isCombineArchive)) {
            // Further initialise ourselves, update our GUI (by reinitialising
            // it) and initialise our simulation, if we still have a valid
            // simulation environment

            mValidSimulationEnvironment = furtherInitialize();

            initializeGui(mValidSimulationEnvironment);

            if (mValidSimulationEnvironment) {
                initializeSimulation();
            }
        }
    mContentsWidget->setUpdatesEnabled(true);

    // Keep track of the initial size of our different graph panels
    // Note: we do this through a single shot (and after a short delay) to be
    //       certain that the GUI is ready and that the size of our different
    //       graph panels is final. Not to do this might, on Windows at least,
    //       result in a file being considered modified (e.g. when you use the
    //       N62 SED-ML file, then switch to another file and back)...

    QTimer::singleShot(500, this, &SimulationExperimentViewSimulationWidget::finalFurtherInitialize);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::finalize()
{
    // Reinitialise our trackers, if we are not dealing with a CellML file, so
    // that it doesn't look for a split second that we are modified when
    // reloading ourselves

    if (mSimulation->fileType() != SimulationSupport::Simulation::FileType::CellmlFile) {
        initializeTrackers();
    }

    // Finalize/backup a few things in our GUI's solvers, graphs, parameters and
    // graph panels widgets

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();

    informationWidget->graphPanelAndGraphsWidget()->finalize();
    informationWidget->parametersWidget()->finalize();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::setSizes(const QIntList &pSizes)
{
    // Set the sizes of our spliter widget, but only if there effectively are
    // some

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

    return style()->pixelMetric(QStyle::PM_TabBarIconSize, nullptr, this);
}

//==============================================================================

QIcon SimulationExperimentViewSimulationWidget::doFileTabIcon(bool pForEmitting)
{
    // Check whether we should reset our progress
    // Note: this is needed in case a simulation was run from Python, in
    //       which case mProgress may have been updated after a simulation has
    //       completed (because the GUI, a simulation and a Python script are
    //       all run in their own thread, so no guarantee about which event is
    //       going to be handled first), so we need to reset it here if we are
    //       not asking for the file tab icon in order to emit a signal (see
    //       SimulationExperimentViewSimulationWidget::updateSimulationResults()
    //       vs. CentralWidget::updateFileTab() and
    //       CentralWidget::updateFileTabIcons()...

    if (!pForEmitting && !mSimulation->isRunning() && !mSimulation->isPaused()) {
        mProgress = -1;
    }

    // Return a file tab icon that shows the given file's simulation progress

    static const QIcon NoIcon;

    QIcon res = NoIcon;

    if (!isVisible() && (mProgress != -1)) {
        // Create an image that shows the progress of our simulation

        QPixmap tabBarPixmap = QPixmap(tabBarPixmapSize(),
                                       mProgressBarWidget->height()+2);
        QPainter tabBarPixmapPainter(&tabBarPixmap);

        tabBarPixmapPainter.setBrush(Core::windowColor());
        tabBarPixmapPainter.setPen(Core::borderColor());

        tabBarPixmapPainter.drawRect(0, 0, tabBarPixmap.width()-1, tabBarPixmap.height()-1);
        tabBarPixmapPainter.fillRect(1, 1, mProgress, tabBarPixmap.height()-2,
                                     Core::highlightColor());

        res = QIcon(tabBarPixmap);
    }

    return res;
}

//==============================================================================

QIcon SimulationExperimentViewSimulationWidget::fileTabIcon()
{
    // Return our file tab icon

    return doFileTabIcon(false);
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::save(const QString &pFileName)
{
    // Save to the given CellML file, SED-ML file our COMBINE archive, depending
    // on the file type of our simulation

    if (mSimulation->fileType() == SimulationSupport::Simulation::FileType::CellmlFile) {
        // We are dealing with a CellML file, so retrieve all the state and
        // constant parameters which value has changed and update our CellML
        // object with their 'new' values, unless they are imported, in which
        // case we let the user know that their 'new' values cannot be saved

        QString importedParameters;
        ObjRef<iface::cellml_api::CellMLComponentSet> components = mSimulation->cellmlFile()->model()->localComponents();
        QHash<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> parameters = mContentsWidget->informationWidget()->parametersWidget()->parameters();
        const Core::Properties propertyKeys = parameters.keys();

        for (auto property : propertyKeys) {
            CellMLSupport::CellmlFileRuntimeParameter *parameter = parameters.value(property);

            if (   (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Type::State)
                || (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Type::Constant)) {
                QStringList parameterComponents = parameter->componentHierarchy();
                ObjRef<iface::cellml_api::CellMLComponent> component = components->getComponent(parameterComponents.last().toStdWString());
                ObjRef<iface::cellml_api::CellMLVariable> variable = component->variables()->getVariable(property->name().toStdWString());

                if (variable == variable->sourceVariable()) {
                    variable->initialValue(property->value().toStdWString());
                } else {
                    importedParameters += "\n - "+QString::fromStdWString(component->name())+" | "+QString::fromStdWString(variable->name());
                }
            }
        }

        // Now, we can effectively save (update) the CellML file to the given
        // file and let the user know if some parameter values couldn't be saved

        if (mSimulation->cellmlFile()->update(pFileName)) {
            if (!importedParameters.isEmpty()) {
                Core::informationMessageBox(tr("Save File"),
                                            tr("The following parameters are imported and cannot therefore be saved:")+importedParameters);
            }

            return true;
        }

        return false;
    }

    if (mSimulation->fileType() == SimulationSupport::Simulation::FileType::SedmlFile) {
        // Only export our simulation to a SED-ML file if we are not dealing
        // with a new SED-ML file
        // Note: indeed, if we were to do that, we would end up with an
        //       unloadable SED-ML file (since it doesn’t contain everything it
        //       should). So, instead, we should just save our default SED-ML
        //       template, i.e. as if we were to save the SED-ML file using the
        //       Raw SED-ML or Raw Text view...

        if (Core::FileManager::instance()->isNew(mSimulation->fileName())) {
            QFile::copy(mSimulation->fileName(), pFileName);
        } else {
            sedmlExportSedmlFile(pFileName);
        }

        return true;
    }

    if (mSimulation->fileType() == SimulationSupport::Simulation::FileType::CombineArchive) {
        sedmlExportCombineArchive(pFileName);

        return true;
    }

    // Not a relevant type, so return false
    // Note: we should never reach this point...

    return false;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::filePermissionsChanged()
{
    // We have been un/locked, so enable/disable the development mode and keep
    // track of its checked status or recheck it, as necessary

     if (Core::FileManager::instance()->isReadableAndWritable(mSimulation->fileName())) {
         mDevelopmentModeAction->setEnabled(mSimulation->fileType() == SimulationSupport::Simulation::FileType::CellmlFile);
         mDevelopmentModeAction->setChecked(mLockedDevelopmentMode);
     } else {
         mLockedDevelopmentMode = mDevelopmentModeAction->isChecked();

         mDevelopmentModeAction->setChecked(false);
         mDevelopmentModeAction->setEnabled(false);
     }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::fileReloaded()
{
    // The given file has been reloaded, so reload ourselves, i.e. finalise and
    // (re)initialise ourselves, meaning that we have effectively been closed
    // and (re)opened

    finalize();
    initialize(true);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::fileRenamed(const QString &pOldFileName,
                                                           const QString &pNewFileName)
{
    // Let our graph panel and graphs widget know that a file has been renamed

    mContentsWidget->informationWidget()->graphPanelAndGraphsWidget()->fileRenamed(pOldFileName, pNewFileName);

    // Make sure that our GUI is up to date

    updateGui(true);
}

//==============================================================================

SimulationSupport::Simulation * SimulationExperimentViewSimulationWidget::simulation() const
{
    // Return our simulation

    return mSimulation;
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
            if (handlingAction || mSimulation->isRunning()) {
                return;
            }

            handlingAction = true;
        }

        // Finish any editing of our simulation information, and update our
        // simulation and solvers properties (without resetting our NLA solver)
        // before running/resuming it

        mContentsWidget->informationWidget()->finishEditing();

        if (!mSimulation->isPaused()) {
            updateSimulationProperties();
            updateSolversProperties(false);
        }

        // Run or resume our simulation

        if (mSimulation->isPaused()) {
            mSimulation->resume();
        } else {
            // Try to allocate all the memory we need by adding a run to our
            // simulation and, if successful, run our simulation

            if (mSimulation->addRun()) {
                mSimulation->run();
            } else {
                Core::warningMessageBox(tr("Run Simulation"),
                                        tr("We could not allocate the memory required for the simulation."));
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

void SimulationExperimentViewSimulationWidget::resetStateModelParameters()
{
    // Reset our state model parameters

    mSimulation->reset(false);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::resetAllModelParameters()
{
    // Reset all our model parameters

    mSimulation->reset();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::clearSimulationResults()
{
    // Make sure that all editings are finished

    mContentsWidget->informationWidget()->finishEditing();

    // Clear our simulation results
    // Note: we temporarily disable updates to prevent the GUI from taking too
    //       long to update itself (something that would happen if we were to
    //       have several graph panels since they would try to realign
    //       themselves)...

    setUpdatesEnabled(false);
        mSimulation->results()->reset();
    setUpdatesEnabled(true);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::developmentMode()
{
    // The development mode has just been enabled/disabled, so update the
    // modified state of our current file accordingly, if needed

    if (!mDevelopmentModeAction->isChecked()) {
        Core::FileManager::instance()->setModified(mSimulation->fileName(), false);
    }

    // Make sure that our reset button is properly enabled/disabled
    // Note: this is needed if the development mode has just been disabled...

    simulationDataModified(mSimulation->data()->isModified());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addGraphPanel()
{
    // Ask our graph panels widget to add a new graph panel

    ++mNbOfGraphPanels;

    mContentsWidget->graphPanelsWidget()->addGraphPanel(defaultGraphPanelProperties());
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

    mNbOfGraphPanels = qMax(1, mNbOfGraphPanels-1);

    if (mContentsWidget->graphPanelsWidget()->removeCurrentGraphPanel(defaultGraphPanelProperties())) {
        processEvents();
        // Note: this ensures that our remaining graph panels get realigned at
        //       once...
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::removeAllGraphPanels()
{
    // Ask our graph panels widget to remove the current graph panel

    mContentsWidget->graphPanelsWidget()->removeAllGraphPanels(defaultGraphPanelProperties());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::initializeTrackers(bool pInitialzeGraphPanelsWidgetSizes)
{
    // Keep track of our simulation, solver, graph panel and graph properties,
    // and check for changes whenever a property gets changed
    // Note: we pass Qt::UniqueConnection in our calls to connect() so that we
    //       don't end up with several identical connections (something that
    //       would happen if we were to reload our SED-ML file / COMBINE
    //       archive)...

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
    SimulationExperimentViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();
    SimulationExperimentViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();

    mSimulationProperties = allPropertyValues(simulationWidget);
    mSolversProperties = allPropertyValues(solversWidget);

    mSimulationPropertiesModified = false;
    mSolversPropertiesModified = false;

    connect(simulationWidget, &SimulationExperimentViewInformationSimulationWidget::propertyChanged,
            this, &SimulationExperimentViewSimulationWidget::checkSimulationProperties,
            Qt::UniqueConnection);
    connect(solversWidget, &SimulationExperimentViewInformationSolversWidget::propertyChanged,
            this, &SimulationExperimentViewSimulationWidget::checkSolversProperties,
            Qt::UniqueConnection);

    mGraphPanelProperties.clear();
    mGraphsProperties.clear();

    mGraphPanelPropertiesModified.clear();
    mGraphsPropertiesModified.clear();

    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();
    GraphPanelWidget::GraphPanelWidgets graphPanels = graphPanelsWidget->graphPanels();
    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = informationWidget->graphPanelAndGraphsWidget();

    for (int i = 0, iMax = graphPanels.count(); i < iMax; ++i) {
        GraphPanelWidget::GraphPanelWidget *graphPanel = graphPanels[i];
        Core::PropertyEditorWidget *graphPanelPropertyEditor = graphPanelAndGraphsWidget->graphPanelPropertyEditor(graphPanel);
        Core::PropertyEditorWidget *graphsPropertyEditor = graphPanelAndGraphsWidget->graphsPropertyEditor(graphPanel);

        mGraphPanelProperties << allPropertyValues(graphPanelPropertyEditor);
        mGraphsProperties << allPropertyValues(graphsPropertyEditor);

        connect(graphPanelPropertyEditor, &Core::PropertyEditorWidget::propertyChanged,
                this, &SimulationExperimentViewSimulationWidget::checkGraphPanelsAndGraphs,
                Qt::UniqueConnection);
        connect(graphsPropertyEditor, &Core::PropertyEditorWidget::propertyChanged,
                this, &SimulationExperimentViewSimulationWidget::checkGraphPanelsAndGraphs,
                Qt::UniqueConnection);
    }

    mGraphPanelsWidgetSizes = pInitialzeGraphPanelsWidgetSizes?mContentsWidget->graphPanelsWidget()->sizes():QIntList();
    mGraphPanelsWidgetSizesModified = false;
}

//==============================================================================

QString SimulationExperimentViewSimulationWidget::fileName(const QString &pFileName,
                                                           const QString &pBaseFileName,
                                                           const QString &pFileExtension,
                                                           const QString &pCaption,
                                                           const QStringList &pFileFilters)
{
    // Return the given file name, if it is not empty, or ask the user to
    // provide one using the additional information that is given

    if (!pFileName.isEmpty()) {
        return pFileName;
    }

    QString fileName = pBaseFileName;
    QString baseFileCompleteSuffix = QFileInfo(pBaseFileName).completeSuffix();

    if (baseFileCompleteSuffix.isEmpty()) {
        fileName += "."+pFileExtension;
    } else {
        fileName.replace(QRegularExpression(QRegularExpression::escape(baseFileCompleteSuffix)+"$"), pFileExtension);
    }

    QString firstFileFilter = pFileFilters.first();

    return Core::getSaveFileName(pCaption, fileName, pFileFilters, &firstFileFilter);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addSedmlSimulationAlgorithm(libsedml::SedAlgorithm *pAlgorithm,
                                                                           SolverInterface *pSolverInterface,
                                                                           const Solver::Solver::Properties &pSolverProperties)
{
    // Customise the given algorithm using the given solver interface and
    // properties

    if (pSolverInterface != nullptr) {
        const QStringList solverPropertyKeys = pSolverProperties.keys();

        for (const auto &solverProperty : solverPropertyKeys) {
            QString kisaoId = pSolverInterface->kisaoId(solverProperty);
            QVariant solverPropertyValue = pSolverProperties.value(solverProperty);
            QString value = (solverPropertyValue.type() == QVariant::Double)?
                                QString::number(solverPropertyValue.toDouble(), 'g', 15):
                                solverPropertyValue.toString();

            libsedml::SedAlgorithmParameter *sedmlAlgorithmParameter = pAlgorithm->createAlgorithmParameter();

            sedmlAlgorithmParameter->setKisaoID(kisaoId.toStdString());
            sedmlAlgorithmParameter->setValue(value.toStdString());
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addSedmlSimulation(libsedml::SedDocument *pSedmlDocument,
                                                                  libsedml::SedModel *pSedmlModel,
                                                                  libsedml::SedRepeatedTask *pSedmlRepeatedTask,
                                                                  libsedml::SedSimulation *pSedmlSimulation,
                                                                  int pOrder)
{
    // Create, customise and add an algorithm (i.e. an ODE solver) to our given
    // SED-ML simulation

    libsedml::SedAlgorithm *sedmlAlgorithm = pSedmlSimulation->createAlgorithm();
    SolverInterface *odeSolverInterface = mSimulation->data()->odeSolverInterface();

    sedmlAlgorithm->setKisaoID(odeSolverInterface->kisaoId(mSimulation->data()->odeSolverName()).toStdString());

    addSedmlSimulationAlgorithm(sedmlAlgorithm, odeSolverInterface,
                                mSimulation->data()->odeSolverProperties());

    // Check whether the simulation required an NLA solver and, if so, let our
    // SED-ML simulation know about it through an annotation (since we cannot
    // have more than one SED-ML algorithm per SED-ML simulation)

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    if ((runtime != nullptr) && runtime->needNlaSolver()) {
        Solver::Solver::Properties nlaSolverProperties = mSimulation->data()->nlaSolverProperties();
        const QStringList nlaSolverPropertyKeys = nlaSolverProperties.keys();
        QString nlaSolverAnnotation;

        for (const auto &nlaSolverProperty : nlaSolverPropertyKeys) {
            nlaSolverAnnotation += QString(R"(<%1 %2="%3" %4="%5"/>)").arg(SEDMLSupport::SolverProperty,
                                                                           SEDMLSupport::Id,
                                                                           nlaSolverProperty,
                                                                           SEDMLSupport::Value,
                                                                           nlaSolverProperties.value(nlaSolverProperty).toString());
        }

        pSedmlSimulation->appendAnnotation(QString(R"(<%1 xmlns="%2" %3="%4">)"
                                                    "     %5"
                                                    " </%1>").arg(SEDMLSupport::NlaSolver,
                                                                  SEDMLSupport::OpencorNamespace,
                                                                  SEDMLSupport::Name,
                                                                  mSimulation->data()->nlaSolverName(),
                                                                  nlaSolverAnnotation).toStdString());
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

    pSedmlVariable->setTarget(Target.arg(pComponent,
                                         variable).toStdString());

    if (variableDegree != 0) {
        pSedmlVariable->appendAnnotation(QString(R"(<%1 xmlns="%2">)"
                                                  "    %3"
                                                  "</%1>").arg(SEDMLSupport::VariableDegree,
                                                               SEDMLSupport::OpencorNamespace)
                                                          .arg(variableDegree).toStdString());
    }
}

//==============================================================================

static const char *TrueValue  = "true";
static const char *FalseValue = "false";

//==============================================================================

bool SimulationExperimentViewSimulationWidget::createSedmlFile(SEDMLSupport::SedmlFile *pSedmlFile,
                                                               const QString &pFileName,
                                                               const QString &pModelSource)
{
    // Make sure that the given SED-ML file will act as if it was 'new'
    // Note: this is important if we are updating a SED-ML file...

    pSedmlFile->forceNew();

    // Create a SED-ML document and add the CellML namespace to it
    // Note: we retrieve the version of the CellML file using its model rather
    //       than its file name since we may be creating a SED-ML file from
    //       another SED-ML file (as opposed to from a CellML file), hence its
    //       corresponding CellML file won't be managed by our CellML file
    //       manager, which means that its version would be considered to be
    //       unknown, which clearly shouldn't be the case...

    libsedml::SedDocument *sedmlDocument = pSedmlFile->sedmlDocument();
    XMLNamespaces *namespaces = sedmlDocument->getNamespaces();
    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::modelVersion((mSimulation->cellmlFile() != nullptr)?
                                                                                                   mSimulation->cellmlFile()->model():
                                                                                                   nullptr);

    namespaces->add((cellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_0)?
                        CellMLSupport::Cellml_1_0_Namespace.toStdString():
                        CellMLSupport::Cellml_1_1_Namespace.toStdString(),
                    "cellml");

    // Create and customise a model

    libsedml::SedModel *sedmlModel = sedmlDocument->createModel();

    sedmlModel->setId("model");
    sedmlModel->setLanguage((cellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_0)?
                                SEDMLSupport::Language::Cellml_1_0.toStdString():
                                SEDMLSupport::Language::Cellml_1_1.toStdString());
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
    auto nbOfPoints = quint64(ceil((endingPoint-startingPoint)/pointInterval));
    bool needOneStepTask = !qFuzzyCompare((endingPoint-startingPoint)/double(nbOfPoints), pointInterval);

    libsedml::SedUniformTimeCourse *sedmlUniformTimeCourse = sedmlDocument->createUniformTimeCourse();

    ++simulationNumber;

    if (needOneStepTask) {
        --nbOfPoints;
    }

    sedmlUniformTimeCourse->setId(QString("simulation%1").arg(simulationNumber).toStdString());
    sedmlUniformTimeCourse->setInitialTime(startingPoint);
    sedmlUniformTimeCourse->setOutputStartTime(startingPoint);
    sedmlUniformTimeCourse->setOutputEndTime(startingPoint+double(nbOfPoints)*pointInterval);
    sedmlUniformTimeCourse->setNumberOfPoints(int(nbOfPoints));

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

    // Create a 2D plot output for each graph panel that we have, and retrieve
    // all the graphs, if any, that are to be plotted on them (except, at this
    // stage, the ones involving imported data)

    static const QString SedmlProperty = QString("<%1>%2</%1>");

    using GraphsData = struct {
                                  libsedml::SedPlot2D *sedmlPlot2d;
                                  int graphPlotCounter;
                                  bool logAxisX;
                                  bool logAxisY;
                              };

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = mContentsWidget->informationWidget()->graphPanelAndGraphsWidget();
    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();
    const GraphPanelWidget::GraphPanelWidgets graphPanels = graphPanelsWidget->graphPanels();
    QIntList graphPanelsWidgetSizes = graphPanelsWidget->sizes();
    QList<Core::Properties> graphsPropertiesList;
    QMap<Core::Properties, GraphsData> graphsData;
    int graphPlotCounter = 0;

    for (auto graphPanel : graphPanels) {
        // Create and customise the look and feel of our 2D plot

        Core::Properties graphPanelProperties = graphPanelAndGraphsWidget->graphPanelPropertyEditor(graphPanel)->properties();
        libsedml::SedPlot2D *sedmlPlot2d = sedmlDocument->createPlot2D();

        sedmlPlot2d->setId(QString("plot%1").arg(++graphPlotCounter).toStdString());

        QString annotation =  SedmlProperty.arg(SEDMLSupport::BackgroundColor,
                                                graphPanelProperties[0]->stringValue())
                             +SedmlProperty.arg(SEDMLSupport::FontSize,
                                                graphPanelProperties[1]->stringValue())
                             +SedmlProperty.arg(SEDMLSupport::ForegroundColor,
                                                graphPanelProperties[2]->stringValue())
                             +SedmlProperty.arg(SEDMLSupport::Height)
                                           .arg(graphPanelsWidgetSizes[graphPlotCounter-1]);

        // Grid lines

        Core::Properties gridLinesProperties = graphPanelProperties[3]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::GridLines,
                                         SedmlProperty.arg(SEDMLSupport::Style,
                                                           SEDMLSupport::stringLineStyleFromIndex(gridLinesProperties[0]->listValueIndex()))
                                        +SedmlProperty.arg(SEDMLSupport::Width,
                                                           gridLinesProperties[1]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Color,
                                                           gridLinesProperties[2]->stringValue()));

        // Legend

        Core::Properties legendProperties = graphPanelProperties[4]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::Legend,
                                         SedmlProperty.arg(SEDMLSupport::FontSize,
                                                           legendProperties[0]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Visible,
                                                           legendProperties[1]->stringValue()));

        // Point coordinates

        Core::Properties pointCoordinatesProperties = graphPanelProperties[5]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::PointCoordinates,
                                         SedmlProperty.arg(SEDMLSupport::Style,
                                                           SEDMLSupport::stringLineStyleFromIndex(pointCoordinatesProperties[0]->listValueIndex()))
                                        +SedmlProperty.arg(SEDMLSupport::Width,
                                                           pointCoordinatesProperties[1]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Color,
                                                           pointCoordinatesProperties[2]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::FontColor,
                                                           pointCoordinatesProperties[3]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::FontSize,
                                                           pointCoordinatesProperties[4]->stringValue()));

        // Surrounding area

        Core::Properties surroundingAreaProperties = graphPanelProperties[6]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::SurroundingArea,
                                         SedmlProperty.arg(SEDMLSupport::BackgroundColor,
                                                           surroundingAreaProperties[0]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::ForegroundColor,
                                                           surroundingAreaProperties[1]->stringValue()));

        // Title

        annotation += SedmlProperty.arg(SEDMLSupport::Title,
                                        graphPanelProperties[7]->stringValue());

        // X axis

        Core::Properties xAxisProperties = graphPanelProperties[8]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::XAxis,
                                         SedmlProperty.arg(SEDMLSupport::FontSize,
                                                           xAxisProperties[0]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::LogarithmicScale,
                                                           xAxisProperties[1]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Title,
                                                           xAxisProperties[2]->stringValue()));

        // Y axis

        Core::Properties yAxisProperties = graphPanelProperties[9]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::YAxis,
                                         SedmlProperty.arg(SEDMLSupport::FontSize,
                                                           yAxisProperties[0]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::LogarithmicScale,
                                                           yAxisProperties[1]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Title,
                                                           yAxisProperties[2]->stringValue()));

        // Zoom region

        Core::Properties zoomRegionProperties = graphPanelProperties[10]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::ZoomRegion,
                                         SedmlProperty.arg(SEDMLSupport::Style,
                                                           SEDMLSupport::stringLineStyleFromIndex(zoomRegionProperties[0]->listValueIndex()))
                                        +SedmlProperty.arg(SEDMLSupport::Width,
                                                           zoomRegionProperties[1]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Color,
                                                           zoomRegionProperties[2]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::FontColor,
                                                           zoomRegionProperties[3]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::FontSize,
                                                           zoomRegionProperties[4]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Filled,
                                                           zoomRegionProperties[5]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::FillColor,
                                                           zoomRegionProperties[6]->stringValue()));

        // Add our properties as an annotation

        sedmlPlot2d->appendAnnotation(QString(R"(<%1 %2="%3" xmlns="%4">)"
                                               "    %5"
                                               "</%1>").arg(SEDMLSupport::Properties,
                                                            SEDMLSupport::Version)
                                                       .arg(SEDMLSupport::VersionValue)
                                                       .arg(SEDMLSupport::OpencorNamespace,
                                                            annotation).toStdString());

        // Keep track of the graph panel's graphs, if any

        Core::Properties graphsProperties = graphPanelAndGraphsWidget->graphsProperties(graphPanel, mSimulation->fileName());

        if (!graphsProperties.isEmpty()) {
            GraphsData data;

            data.sedmlPlot2d = sedmlPlot2d;
            data.graphPlotCounter = graphPlotCounter;
            data.logAxisX = graphPanel->plot()->logAxisX();
            data.logAxisY = graphPanel->plot()->logAxisY();

            graphsPropertiesList << graphsProperties;

            graphsData.insert(graphsProperties, data);
        }
    }

    // Create and customise 2D plot outputs and data generators for all the
    // graphs that are to be plotted, if any

    for (const auto &graphsProperties : qAsConst(graphsPropertiesList)) {
        // Create some graphs

        GraphsData data = graphsData.value(graphsProperties);
        int graphCounter = 0;

        for (auto property : graphsProperties) {
            // Make sure that our graph doesn't involve some imported data
            // otherwise skip it

            Core::Properties properties = property->properties();
            QStringList propertyX = properties[2]->value().split('.');
            QStringList propertyY = properties[3]->value().split('.');
            QString componentX = propertyX[propertyX.count()-2];
            QString variableX = propertyX.last();
            QString componentY = propertyY[propertyY.count()-2];
            QString variableY = propertyY.last();

            if (   isRuntimeDataParameter(componentX, variableX)
                || isRuntimeDataParameter(componentY, variableY)) {
                continue;
            }

            // Create two data generators for the X and Y parameters of our
            // current graph

            ++graphCounter;

            libsedml::SedDataGenerator *sedmlDataGeneratorX = sedmlDocument->createDataGenerator();
            libsedml::SedDataGenerator *sedmlDataGeneratorY = sedmlDocument->createDataGenerator();
            std::string sedmlDataGeneratorIdX = QString("xDataGenerator%1_%2").arg(data.graphPlotCounter)
                                                                              .arg(graphCounter).toStdString();
            std::string sedmlDataGeneratorIdY = QString("yDataGenerator%1_%2").arg(data.graphPlotCounter)
                                                                              .arg(graphCounter).toStdString();

            sedmlDataGeneratorX->setId(sedmlDataGeneratorIdX);
            sedmlDataGeneratorY->setId(sedmlDataGeneratorIdY);

            libsedml::SedVariable *sedmlVariableX = sedmlDataGeneratorX->createVariable();
            libsedml::SedVariable *sedmlVariableY = sedmlDataGeneratorY->createVariable();

            sedmlVariableX->setId(QString("xVariable%1_%2").arg(data.graphPlotCounter)
                                                           .arg(graphCounter).toStdString());
            sedmlVariableX->setTaskReference(sedmlRepeatedTask->getId());
            addSedmlVariableTarget(sedmlVariableX, componentX, variableX);

            sedmlVariableY->setId(QString("yVariable%1_%2").arg(data.graphPlotCounter)
                                                           .arg(graphCounter).toStdString());
            sedmlVariableY->setTaskReference(sedmlRepeatedTask->getId());
            addSedmlVariableTarget(sedmlVariableY, componentY, variableY);

            sedmlDataGeneratorX->setMath(SBML_parseFormula(sedmlVariableX->getId().c_str()));
            sedmlDataGeneratorY->setMath(SBML_parseFormula(sedmlVariableY->getId().c_str()));

            // Create a curve for our current graph

            libsedml::SedCurve *sedmlCurve = data.sedmlPlot2d->createCurve();

            sedmlCurve->setId(QString("curve%1_%2").arg(data.graphPlotCounter)
                                                   .arg(graphCounter).toStdString());

            sedmlCurve->setXDataReference(sedmlDataGeneratorIdX);
            sedmlCurve->setLogX(data.logAxisX);

            sedmlCurve->setYDataReference(sedmlDataGeneratorIdY);
            sedmlCurve->setLogY(data.logAxisY);

            // Customise our curve using an annotation

            Core::Properties lineProperties = properties[4]->properties();
            Core::Properties symbolProperties = properties[5]->properties();

            sedmlCurve->appendAnnotation(QString(R"(<%1 xmlns="%2">)"
                                                  "    %3"
                                                  "</%1>").arg(SEDMLSupport::Properties,
                                                               SEDMLSupport::OpencorNamespace,
                                                                SedmlProperty.arg(SEDMLSupport::Selected,
                                                                                  property->isChecked()?
                                                                                      TrueValue:
                                                                                      FalseValue)
                                                               +SedmlProperty.arg(SEDMLSupport::Title,
                                                                                  properties[1]->stringValue())
                                                               +SedmlProperty.arg(SEDMLSupport::Line,
                                                                                   SedmlProperty.arg(SEDMLSupport::Style,
                                                                                                     SEDMLSupport::stringLineStyleFromIndex(lineProperties[0]->listValueIndex()))
                                                                                  +SedmlProperty.arg(SEDMLSupport::Width,
                                                                                                     lineProperties[1]->stringValue())
                                                                                  +SedmlProperty.arg(SEDMLSupport::Color,
                                                                                                     lineProperties[2]->stringValue()))
                                                               +SedmlProperty.arg(SEDMLSupport::Symbol,
                                                                                   SedmlProperty.arg(SEDMLSupport::Style,
                                                                                                     SEDMLSupport::stringSymbolStyleFromIndex(symbolProperties[0]->listValueIndex()))
                                                                                  +SedmlProperty.arg(SEDMLSupport::Size,
                                                                                                     symbolProperties[1]->stringValue())
                                                                                  +SedmlProperty.arg(SEDMLSupport::Color,
                                                                                                     symbolProperties[2]->stringValue())
                                                                                  +SedmlProperty.arg(SEDMLSupport::Filled,
                                                                                                     symbolProperties[3]->stringValue())
                                                                                  +SedmlProperty.arg(SEDMLSupport::FillColor,
                                                                                                     symbolProperties[4]->stringValue()))).toStdString());
        }
    }

    // Our SED-ML document is ready, so save (update) it
    // Note: we update it because we don't want the SED-ML file to be
    //       potentially reloaded...

    return pSedmlFile->update(pFileName);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::sedmlExportSedmlFile(const QString &pFileName)
{
    // Note: if there is no given file name, then it means that we want to
    //       export the simulation to a SED-ML file and that mSimulation refers
    //       to a local or remote CellML file. On the other hand, if a file name
    //       is given, then it means that we are dealing with a SED-ML file and
    //       that we want to update it or save it under a new file name, meaning
    //       that mSimulation refers to a local SED-ML file (never a remote
    //       SED-ML file since we don't save those)...

    // Export ourselves to SED-ML using a SED-ML file, but first try to get a
    // file name, if needed

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QString localCellmlFileName = (mSimulation->cellmlFile() != nullptr)?
                                      mSimulation->cellmlFile()->fileName():
                                      QString();
    bool remoteCellmlFile = fileManagerInstance->isRemote(localCellmlFileName);
    QString cellmlFileName = remoteCellmlFile?
                                 fileManagerInstance->url(localCellmlFileName):
                                 localCellmlFileName;
    FileTypeInterface *sedmlFileTypeInterface = SEDMLSupport::fileTypeInterface();
    QString sedmlFileName = fileName(pFileName, cellmlFileName,
                                     SEDMLSupport::SedmlFileExtension,
                                     tr("Export To SED-ML File"),
                                     (sedmlFileTypeInterface != nullptr)?
                                         Core::filters(FileTypeInterfaces() << sedmlFileTypeInterface):
                                         QStringList());

    // Create a SED-ML file using the SED-ML file name that has been provided,
    // if any

    if (!sedmlFileName.isEmpty()) {
        QString modelSource = cellmlFileName;

        if (   !remoteCellmlFile
#ifdef Q_OS_WIN
            &&  (modelSource.left(3) == sedmlFileName.left(3))
#endif
           ) {
            // We are dealing with a local CellML file that is on the same drive
            // as our SED-ML file, so refer to our CellML file relatively to our
            // SED-ML file
            // Note: normally, we would use QFileInfo::canonicalPath(), but this
            //       requires an existing file, so we use QFileInfo::path()
            //       instead...

            QDir sedmlFileDir = QFileInfo(sedmlFileName).path();

            modelSource = sedmlFileDir.relativeFilePath(modelSource);
        }

        // Retrieve our SED-ML file or create a temporary one, if needed

        bool isCellmlFile = mSimulation->fileType() == SimulationSupport::Simulation::FileType::CellmlFile;
        SEDMLSupport::SedmlFile *sedmlFile = isCellmlFile?
                                                 new SEDMLSupport::SedmlFile(sedmlFileName, true):
                                                 mSimulation->sedmlFile();

        if (!createSedmlFile(sedmlFile, sedmlFileName, modelSource)) {
            Core::warningMessageBox(tr("Export To SED-ML File"),
                                    tr("The simulation could not be exported to <strong>%1</strong>.").arg(QDir::toNativeSeparators(sedmlFileName)));
        }

        if (isCellmlFile) {
            delete sedmlFile;
        }

        // Reinitialise our trackers, if we are not dealing with a CellML file

        if (!isCellmlFile) {
            initializeTrackers(true);
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::sedmlExportSedmlFile()
{
    // Export the simulation to a SED-ML file

    sedmlExportSedmlFile({});
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::sedmlExportCombineArchive(const QString &pFileName)
{
    // Note: if there is no given file name, then it means that we want to
    //       export the simulation to a COMBINE archive and that mSimulation
    //       refers to a local or remote CellML or SED-ML file. On the other
    //       hand, if a file name is given, then it means that we are dealing
    //       with a COMBINE archive and that we want to update it or save it
    //       under a new file name, meaning that mSimulation refers to a local
    //       COMBINE archive (never a remote COMBINE archive since we don't save
    //       those)...

    // Export ourselves to SED-ML using a COMBINE archive, but first try to get
    // a file name, if needed

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QString localSimulationFileName = mSimulation->fileName();
    FileTypeInterface *combineFileTypeInterface = COMBINESupport::fileTypeInterface();
    QString combineArchiveName = fileName(pFileName,
                                          fileManagerInstance->isRemote(localSimulationFileName)?
                                              fileManagerInstance->url(localSimulationFileName):
                                              localSimulationFileName,
                                          COMBINESupport::CombineFileExtension,
                                          tr("Export To COMBINE Archive"),
                                          (combineFileTypeInterface != nullptr)?
                                              Core::filters(FileTypeInterfaces() << combineFileTypeInterface):
                                              QStringList());

    // Create a COMBINE archive using the COMBINE archive name that has been
    // provided, if any

    if (!combineArchiveName.isEmpty()) {
        // Determine the path that is common to our main and, if any, imported
        // CellML files, as well as get a copy of our imported CellML files,
        // should they be remote ones

        static const QRegularExpression FileNameRegEx = QRegularExpression("/[^/]*$");

        CellMLSupport::CellmlFile *cellmlFile = mSimulation->cellmlFile();
        const QStringList importedFileNames = cellmlFile->importedFileNames();
        QString localCellmlFileName = cellmlFile->fileName();
        bool remoteCellmlFile = fileManagerInstance->isRemote(localCellmlFileName);
        QString cellmlFileName = remoteCellmlFile?
                                     fileManagerInstance->url(localCellmlFileName):
                                     localCellmlFileName;
        QString commonPath = QString(cellmlFileName).remove(FileNameRegEx)+"/";
        QMap<QString, QString> remoteImportedFileNames;

        for (const auto &importedFileName : importedFileNames) {
            // Check for the common path

            QString importedFilePath = remoteCellmlFile?
                                           QString(importedFileName).remove(FileNameRegEx)+"/":
                                           QFileInfo(importedFileName).canonicalPath()+"/";

            for (int i = 0, iMax = qMin(commonPath.length(), importedFilePath.length()); i < iMax; ++i) {
                if (commonPath[i] != importedFilePath[i]) {
                    commonPath = commonPath.left(i);

                    break;
                }
            }

            commonPath = commonPath.left(qMin(commonPath.length(), importedFilePath.length()));

            // Get a copy of the imported CellML file, if it is a remote one,
            // and keep track of it

            if (remoteCellmlFile) {
                QString localImportedFileName = Core::temporaryFileName();

                Core::writeFile(localImportedFileName,
                                cellmlFile->importedFileContents(importedFileName));

                remoteImportedFileNames.insert(importedFileName, localImportedFileName);
            }
        }

        // Determine the location of our main CellML file

        QString modelSource = QString(cellmlFileName).remove(commonPath, Qt::CaseInsensitive);

        // Create a copy of the SED-ML file that will be the master file in our
        // COMBINE archive

        QString errorMessage;
        QString sedmlFileName = Core::temporaryFileName();
        SEDMLSupport::SedmlFile sedmlFile(sedmlFileName, true);

        createSedmlFile(&sedmlFile, sedmlFileName, modelSource);

        // Create our COMBINE archive after having added all our files to it

        bool isCellmlOrSedmlFile =    (mSimulation->fileType() == SimulationSupport::Simulation::FileType::CellmlFile)
                                   || (mSimulation->fileType() == SimulationSupport::Simulation::FileType::SedmlFile);
        COMBINESupport::CombineArchive *combineArchive = isCellmlOrSedmlFile?
                                                             new COMBINESupport::CombineArchive(combineArchiveName, true):
                                                             mSimulation->combineArchive();
        QFileInfo combineArchiveInfo = QFileInfo(combineArchiveName);
        QString sedmlFileLocation = combineArchiveInfo.fileName();

        sedmlFileLocation.replace(QRegularExpression(QRegularExpression::escape(combineArchiveInfo.completeSuffix())+"$"),
                                  SEDMLSupport::SedmlFileExtension);

        combineArchive->forceNew();

        if (combineArchive->addFile(sedmlFileName, sedmlFileLocation,
                                    COMBINESupport::CombineArchiveFile::Format::Sedml, true)) {
            CellMLSupport::CellmlFile::Version cellmlVersion = cellmlFile->version();

            if (combineArchive->addFile(localCellmlFileName, modelSource,
                                        (cellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_0)?
                                            COMBINESupport::CombineArchiveFile::Format::Cellml_1_0:
                                            COMBINESupport::CombineArchiveFile::Format::Cellml_1_1)) {
                const QStringList subImportedFileNames = cellmlFile->importedFileNames();

                for (const auto &importedFileName : subImportedFileNames) {
                    QString realImportedFileName = remoteCellmlFile?
                                                       remoteImportedFileNames.value(importedFileName):
                                                       importedFileName;
                    QString relativeImportedFileName = QString(importedFileName).remove(commonPath, Qt::CaseInsensitive);

                    if (!combineArchive->addFile(realImportedFileName,
                                                 relativeImportedFileName,
                                                 COMBINESupport::CombineArchiveFile::Format::Cellml)) {
                        errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(QDir::toNativeSeparators(combineArchiveName),
                                                                                                                " ("+tr("<strong>%1</strong> could not be added").arg(QDir::toNativeSeparators(relativeImportedFileName))+")");

                        break;
                    }
                }

                if (errorMessage.isEmpty()) {
                    // Now, we can effectively save (update) the COMBINE archive

                    if (!combineArchive->update(combineArchiveName)) {
                        errorMessage = tr("The simulation could not be exported to <strong>%1</strong>.").arg(QDir::toNativeSeparators(combineArchiveName));
                    }
                }
            } else {
                errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(QDir::toNativeSeparators(combineArchiveName),
                                                                                                        " ("+tr("<strong>%1</strong> could not be added").arg(QDir::toNativeSeparators(modelSource))+")");
            }
        } else {
            errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(QDir::toNativeSeparators(combineArchiveName),
                                                                                                    " ("+tr("the master SED-ML file could not be added")+")");
        }

        if (isCellmlOrSedmlFile) {
            delete combineArchive;
        }

        // Remove the local copy of our remote imported CellML files, if any

        for (const auto &localImportedFileName : remoteImportedFileNames) {
            QFile::remove(localImportedFileName);
        }

        // Let the user know about any error that may have occurred

        if (!errorMessage.isEmpty()) {
            Core::warningMessageBox(tr("Export To COMBINE Archive"), errorMessage);
        }

        // Reinitialise our trackers, if we are neither dealing with a CellML
        // file nor a SED-ML file

        if (!isCellmlOrSedmlFile) {
            initializeTrackers(true);
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::sedmlExportCombineArchive()
{
    // Export the simulation to a COMBINE archive

    sedmlExportCombineArchive({});
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::preferences()
{
    // Show our preferences

    QDesktopServices::openUrl(QUrl("opencor://openPreferencesDialog/SimulationExperimentView"));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSimulationProperties(Core::Property *pProperty)
{
    // Update all the properties, or a particular property (if it exists), of
    // our simulation

    SimulationExperimentViewInformationSimulationWidget *simulationWidget = mContentsWidget->informationWidget()->simulationWidget();

    if ((pProperty == nullptr) || (pProperty == simulationWidget->startingPointProperty())) {
        mSimulation->data()->setStartingPoint(simulationWidget->startingPointProperty()->doubleValue());

        if (pProperty != nullptr) {
            return;
        }
    }

    if ((pProperty == nullptr) || (pProperty == simulationWidget->endingPointProperty())) {
        mSimulation->data()->setEndingPoint(simulationWidget->endingPointProperty()->doubleValue());

        if (pProperty != nullptr) {
            return;
        }
    }

    if ((pProperty == nullptr) || (pProperty == simulationWidget->pointIntervalProperty())) {
        mSimulation->data()->setPointInterval(simulationWidget->pointIntervalProperty()->doubleValue());

        if (pProperty != nullptr) {
            return;
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSolversProperties(Core::Property *pProperty,
                                                                       bool pResetNlaSolver)
{
    // Update all of our solver(s) properties (and solvers widget) or a
    // particular solver property (and the corresponding GUI for that solver)

    SimulationExperimentViewInformationSolversWidget *solversWidget = mContentsWidget->informationWidget()->solversWidget();

    // ODE solver properties

    bool needOdeSolverGuiUpdate = false;

    if (solversWidget->odeSolverData() != nullptr) {
        bool isOdeSolversListProperty = pProperty == solversWidget->odeSolverData()->solversListProperty();

        if ((pProperty == nullptr) || isOdeSolversListProperty) {
            mSimulation->data()->setOdeSolverName(solversWidget->odeSolverData()->solversListProperty()->value());

            needOdeSolverGuiUpdate = true;
        }

        for (auto property : solversWidget->odeSolverData()->solversProperties().value(mSimulation->data()->odeSolverName())) {
            if ((pProperty == nullptr) || isOdeSolversListProperty || (pProperty == property)) {
                // Note: we pass false to variantValue() because we want to
                //       retrieve the value of list properties as a string
                //       rather than an index...

                mSimulation->data()->setOdeSolverProperty(property->id(),
                                                          property->variantValue(false));

                needOdeSolverGuiUpdate = true;

                if (pProperty == property) {
                    break;
                }
            }
        }
    }

    if (needOdeSolverGuiUpdate) {
        mContentsWidget->informationWidget()->solversWidget()->updateGui(solversWidget->odeSolverData());

        if (pProperty != nullptr) {
            return;
        }
    }

    // NLA solver properties

    bool needNlaSolverGuiUpdate = false;

    if (solversWidget->nlaSolverData() != nullptr) {
        bool isNlaSolversListProperty = pProperty == solversWidget->nlaSolverData()->solversListProperty();

        if ((pProperty == nullptr) || isNlaSolversListProperty) {
            mSimulation->data()->setNlaSolverName(solversWidget->nlaSolverData()->solversListProperty()->value(), pResetNlaSolver);

            needNlaSolverGuiUpdate = true;
        }

        for (auto property : solversWidget->nlaSolverData()->solversProperties().value(mSimulation->data()->nlaSolverName())) {
            if ((pProperty == nullptr) || isNlaSolversListProperty || (pProperty == property)) {
                // Note: we pass false to variantValue() because we want to
                //       retrieve the value of list properties as a string
                //       rather than an index...

                mSimulation->data()->setNlaSolverProperty(property->id(),
                                                          property->variantValue(false),
                                                          pResetNlaSolver);

                needNlaSolverGuiUpdate = true;

                if (pProperty == property) {
                    break;
                }
            }
        }
    }

    if (needNlaSolverGuiUpdate) {
        mContentsWidget->informationWidget()->solversWidget()->updateGui(solversWidget->nlaSolverData());
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSolversProperties(Core::Property *pProperty)
{
    // Update a particular solver property (and the corresponding GUI for that
    // solver)

    updateSolversProperties(pProperty, true);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSolversProperties(bool pResetNlaSolver)
{
    // Update all of our solver(s) properties (and solvers widget)

    updateSolversProperties(nullptr, pResetNlaSolver);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSolversProperties()
{
    // Update all of our solver(s) properties (and solvers widget)

    updateSolversProperties(nullptr, true);
}

//==============================================================================

CellMLSupport::CellmlFileRuntimeParameter * SimulationExperimentViewSimulationWidget::runtimeParameter(libsedml::SedVariable *pSedmlVariable,
                                                                                                       QString &pCellmlComponent,
                                                                                                       QString &pCellmlVariable)
{
    // Retrieve the CellML runtime parameter corresponding to the given SED-ML
    // variable

    static const QRegularExpression TargetStartRegEx  = QRegularExpression(R"(^\/cellml:model\/cellml:component\[@name=')");
    static const QRegularExpression TargetMiddleRegEx = QRegularExpression(R"(']\/cellml:variable\[@name=')");
    static const QRegularExpression TargetEndRegEx    = QRegularExpression(R"('\]$)");
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

    if (annotation != nullptr) {
        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            libsbml::XMLNode &variableDegreeNode = annotation->getChild(i);

            if (   (QString::fromStdString(variableDegreeNode.getURI()) == SEDMLSupport::OpencorNamespace)
                && (QString::fromStdString(variableDegreeNode.getName()) == SEDMLSupport::VariableDegree)) {
                variableDegree = QString::fromStdString(variableDegreeNode.getChild(0).getCharacters()).toInt();
            }
        }
    }

    // Go through the runtime parameters to see if one of them corresponds to
    // our given SED-ML variable

    pCellmlComponent = componentName;
    pCellmlVariable = variableName+QString(variableDegree, '\'');

    const CellMLSupport::CellmlFileRuntimeParameters parameters = mSimulation->runtime()->parameters();

    for (auto parameter : parameters) {
        QStringList components = parameter->componentHierarchy();

        if (   (componentName == components.last())
            && (variableName == parameter->name())
            && (variableDegree == parameter->degree())) {
            return parameter;
        }
    }

    return nullptr;
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::isRuntimeDataParameter(const QString &pComponent,
                                                                      const QString &pVariable)
{
    // Go through the runtime data parameters to see if one of them corresponds
    // to the given component/variable

    const CellMLSupport::CellmlFileRuntimeParameters parameters = mSimulation->runtime()->dataParameters();

    for (auto parameter : parameters) {
        QStringList components = parameter->componentHierarchy();

        if (   (pComponent == components.last())
            && (pVariable == parameter->name())) {
            return true;
        }
    }

    return false;
}

//==============================================================================

GraphPanelWidget::GraphPanelWidgetProperties SimulationExperimentViewSimulationWidget::defaultGraphPanelProperties() const
{
    // Return our default graph panel properties

    return GraphPanelWidget::GraphPanelWidgetProperties(PreferencesInterface::preference(PluginName,
                                                                                         SettingsPreferencesGraphPanelBackgroundColor,
                                                                                         SettingsPreferencesGraphPanelBackgroundColorDefault).value<QColor>(),
                                                        PreferencesInterface::preference(PluginName,
                                                                                         SettingsPreferencesGraphPanelForegroundColor,
                                                                                         SettingsPreferencesGraphPanelForegroundColorDefault).value<QColor>(),
                                                        PreferencesInterface::preference(PluginName,
                                                                                         SettingsPreferencesGraphPanelLegend,
                                                                                         SettingsPreferencesGraphPanelLegendDefault).toBool());
}

//==============================================================================

GraphPanelWidget::GraphPanelPlotGraphProperties SimulationExperimentViewSimulationWidget::defaultGraphProperties(const QString &pTitle,
                                                                                                                 const QColor &pColor) const
{
    // Return our default graph properties

    return GraphPanelWidget::GraphPanelPlotGraphProperties(true,
                                                           pTitle,
                                                           qtPenStyleFromString(PreferencesInterface::preference(PluginName,
                                                                                                                 SettingsPreferencesGraphLineStyle,
                                                                                                                 stringLineStyleFromQtPenStyle(SettingsPreferencesGraphLineStyleDefault)).toString()),
                                                           PreferencesInterface::preference(PluginName,
                                                                                            SettingsPreferencesGraphLineWidth,
                                                                                            SettingsPreferencesGraphLineWidthDefault).toInt(),
                                                           pColor,
                                                           qwtSymbolStyleFromString(PreferencesInterface::preference(PluginName,
                                                                                                                     SettingsPreferencesGraphSymbolStyle,
                                                                                                                     stringSymbolStyleFromQwtSymbolStyle(SettingsPreferencesGraphSymbolStyleDefault)).toString()),
                                                           PreferencesInterface::preference(PluginName,
                                                                                            SettingsPreferencesGraphSymbolSize,
                                                                                            SettingsPreferencesGraphSymbolSizeDefault).toInt(),
                                                           pColor,
                                                           PreferencesInterface::preference(PluginName,
                                                                                            SettingsPreferencesGraphSymbolFilled,
                                                                                            SettingsPreferencesGraphSymbolFilledDefault).toBool(),
                                                           PreferencesInterface::preference(PluginName,
                                                                                            SettingsPreferencesGraphSymbolFillColor,
                                                                                            SettingsPreferencesGraphSymbolFillColorDefault).value<QColor>());
}

//==============================================================================

GraphPanelWidget::GraphPanelPlotGraphProperties SimulationExperimentViewSimulationWidget::defaultGraphProperties(const QColor &pColor) const
{
    // Return our default graph properties

    return defaultGraphProperties({}, pColor);
}

//==============================================================================

#define GUI_SUPPORT
    #include "furtherinitialize.cpp.inl"
#undef GUI_SUPPORT

//==============================================================================

void SimulationExperimentViewSimulationWidget::finalFurtherInitialize()
{
    // The GUI is all ready, so we can initialise mGraphPanelsWidgetSizes, as
    // well as mGraphPanelPropertiesModified and mGraphsPropertiesModified by
    // calling checkGraphPanelsAndGraphs()

    mGraphPanelsWidgetSizes = mContentsWidget->graphPanelsWidget()->sizes();

    checkGraphPanelsAndGraphs();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::initializeGui(bool pValidSimulationEnvironment)
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

void SimulationExperimentViewSimulationWidget::initializeSimulation()
{
    // Reset both the simulation's data and results (well, initialise in the
    // case of its data)

    mSimulation->data()->reset();
    mSimulation->results()->reset();

    // Retrieve our simulation and solvers properties since they may have an
    // effect on our parameter values (as well as result in some solver
    // properties being shown/hidden)

    updateSimulationProperties();
    updateSolversProperties();
}

//==============================================================================

#define GUI_SUPPORT
    #include "initializesolver.cpp.inl"
#undef GUI_SUPPORT

//==============================================================================

void SimulationExperimentViewSimulationWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(mSplitterWidget->sizes());
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::import(const QString &pFileName,
                                                      bool pShowWarning)
{
    // Determine the type of file we are dealing with so we can use the correct
    // data store interface
    // Note: we check whether mFileTypeInterfaces contains an entry for the
    //       given file (which would mean that the file was dropped on us) and
    //       if not (i.e. we want to import a file using our main menu or our
    //       URI scheme) then check whether the file is a data file...

    FileTypeInterface *fileTypeInterface = mFileTypeInterfaces.value(pFileName);

    if (fileTypeInterface == nullptr) {
        const FileTypeInterfaces dataStoreFileTypeInterfaces = Core::dataStoreFileTypeInterfaces();

        for (auto dataStoreFileTypeInterface : dataStoreFileTypeInterfaces) {
            if (dataStoreFileTypeInterface->isFile(pFileName)) {
                fileTypeInterface = dataStoreFileTypeInterface;

                break;
            }
        }
    }

    // Make sure that we have a got a file type interface for the given file or
    // leave if not

    if (fileTypeInterface != nullptr) {
        mFileTypeInterfaces.remove(pFileName);
    } else {
        if (pShowWarning) {
            Core::warningMessageBox(tr("Data Import"),
                                    tr("<strong>%1</strong> is not a data file.").arg(pFileName));
        }

        return false;
    }

    // Retrieve some imported data for the given file

    enum Problem {
        None,
        FileAccess,
        MemoryAllocation
    };

    QList<quint64> runSizes;

    for (int i = 0, iMax = mSimulation->runsCount(); i < iMax; ++i) {
        runSizes << mSimulation->runSize(i);
    }

    DataStoreInterface *dataStoreInterface = Core::dataStoreInterface(fileTypeInterface);
    DataStore::DataStoreImportData *dataStoreImportData = dataStoreInterface->getImportData(pFileName, mSimulation->importData()->addDataStore(),
                                                                                                       mSimulation->results()->dataStore(),
                                                                                                       runSizes);
    Problem problem = None;

    if (dataStoreImportData != nullptr) {
        // We have some import data, so now check whether it is actually valid

        if (!dataStoreImportData->valid()) {
            delete dataStoreImportData;

            dataStoreImportData = nullptr;

            problem = MemoryAllocation;
        }
    } else {
        problem = FileAccess;
    }

    // Do the actual import unless there was a problem, in which case we let
    // people know about the problem

    if (problem == None) {
        // Everything is fine, so do the actual import, waiting for the import
        // to be done before carrying on
        // Note: this is needed since we may have to import several files and we
        //       don't want the GUI to be all messed up (e.g. with the progress
        //       with an import "overlapping" with that of another)...

        Core::showCentralProgressBusyWidget();

        DataStore::DataStoreImporter *dataStoreImporter = dataStoreInterface->dataStoreImporterInstance();

        connect(dataStoreImporter, &DataStore::DataStoreImporter::progress,
                this, &SimulationExperimentViewSimulationWidget::dataStoreImportProgress);

        connect(dataStoreImporter, &DataStore::DataStoreImporter::done,
                this, &SimulationExperimentViewSimulationWidget::dataStoreImportDone);

        connect(this, &SimulationExperimentViewSimulationWidget::importDone,
                dataStoreImportData, &DataStore::DataStoreImportData::deleteLater,
                Qt::UniqueConnection);
        connect(this, &SimulationExperimentViewSimulationWidget::importDone,
                this, &SimulationExperimentViewSimulationWidget::dataStoreImportReallyDone,
                Qt::UniqueConnection);

        dataStoreImporter->importData(dataStoreImportData);

        mWaitLoop.exec();

        return true;
    }

    Core::warningMessageBox(tr("Data Import"),
                            tr("<strong>%1</strong> could not be imported (%2).").arg(pFileName,
                                                                                      (problem == FileAccess)?
                                                                                          tr("the file could not be accessed"):
                                                                                          tr("the memory needed to store the data could not be allocated")));

    return false;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreImportReallyDone(DataStore::DataStoreImporter *pDataStoreImporter)
{
    // Reset our connections for the given data store importer and ask our wait
    // loop to quit with a delay (so that mWaitLoop.exec() has time to be called
    // in case the import was very quick)
    // Note: we reset our data store importer connections once the import is
    //       done otherwise if we were to import data from another file then
    //       our "local" dataStoreImportProgress() and dataStoreImportDone()
    //       methods would be called again, which would mean that at least
    //       two files would be pointing to the same imported data,
    //       resulting in OpenCOR crashing upon closing (since the simulation
    //       object of both files would try to delete the same memory block)...

    disconnect(pDataStoreImporter, &DataStore::DataStoreImporter::progress,
               this, &SimulationExperimentViewSimulationWidget::dataStoreImportProgress);

    disconnect(pDataStoreImporter, &DataStore::DataStoreImporter::done,
               this, &SimulationExperimentViewSimulationWidget::dataStoreImportDone);

    QTimer::singleShot(169, this, &SimulationExperimentViewSimulationWidget::quitWaitLoop);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::quitWaitLoop()
{
    // Quit our wait loop

    mWaitLoop.quit();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataImport(const QStringList &pFileNames)
{
    // Import the one or several files

    for (const auto &fileName : pFileNames) {
        import(fileName);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::localDataImport()
{
    // Ask for the data files to be imported and import them

    dataImport(Core::getOpenFileNames(tr("Data Import"),
                                      Core::filters(Core::dataStoreFileTypeInterfaces())));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::remoteDataImport()
{
    // Ask for the URL of the remote file that is to be imported

    Core::RemoteFileDialog remoteFileDialog(tr("Data Import"), this);

    remoteFileDialog.exec();

    if (remoteFileDialog.result() == QMessageBox::Accepted) {
        QDesktopServices::openUrl("opencor://importFile/"+remoteFileDialog.url());
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationResultsExport()
{
    // Retrieve some data so that we can effectively export our simulation
    // results

    DataStoreInterface *dataStoreInterface = mDataStoreInterfaces.value(qobject_cast<QAction *>(sender()));
    DataStore::DataStoreExportData *dataStoreExportData = dataStoreInterface->getExportData(mSimulation->fileName(),
                                                                                            mSimulation->results()->dataStore(),
                                                                                            CellMLSupport::CellmlFileRuntimeParameter::icons());

    if (dataStoreExportData != nullptr) {
        // We have got the data we need, so do the actual export

        Core::showCentralProgressBusyWidget();

        DataStore::DataStoreExporter *dataStoreExporter = dataStoreInterface->dataStoreExporterInstance();

        connect(dataStoreExporter, &DataStore::DataStoreExporter::progress,
                this, &SimulationExperimentViewSimulationWidget::dataStoreExportProgress);

        connect(dataStoreExporter, &DataStore::DataStoreExporter::done,
                this, &SimulationExperimentViewSimulationWidget::dataStoreExportDone);

        connect(this, &SimulationExperimentViewSimulationWidget::exportDone,
                dataStoreExportData, &DataStore::DataStoreExportData::deleteLater,
                Qt::UniqueConnection);
        connect(this, &SimulationExperimentViewSimulationWidget::exportDone,
                this, &SimulationExperimentViewSimulationWidget::dataStoreExportReallyDone,
                Qt::UniqueConnection);

        dataStoreExporter->exportData(dataStoreExportData);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreExportReallyDone(DataStore::DataStoreExporter *pDataStoreExporter)
{
    // Reset our connections for the given data store exporter
    // Note: we reset our data store exporter connections once the export is
    //       done otherwise if we were to export data from another file then
    //       our "local" dataStoreExportProgress() and dataStoreExportDone()
    //       methods would be called again. This shouldn't normally cause
    //       problems, but better be safe than sorry...

    disconnect(pDataStoreExporter, &DataStore::DataStoreExporter::progress,
               this, &SimulationExperimentViewSimulationWidget::dataStoreExportProgress);

    disconnect(pDataStoreExporter, &DataStore::DataStoreExporter::done,
               this, &SimulationExperimentViewSimulationWidget::dataStoreExportDone);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::delayWheelCreated(QwtWheel *pWheel)
{
    // Configure our delay wheel, if still valid

    if (!mDelayWheelWidgetAction->validWheel(pWheel)) {
        return;
    }

    pWheel->setRange(0.0, 55.0);
    pWheel->setValue(mDelayWheelValue);

    // (Re)translate ourselves by updating ourselves

    updateDelayWheelWidgetAction();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::delayValueCreated(QLabel *pLabel)
{
    // Configure our delay value, if still valid

    if (!mDelayLabelValueWidgetAction->validLabel(pLabel)) {
        return;
    }

    pLabel->setText(mDelayLabelValue);

    // (Re)translate ourselves by updating ourselves

    updateDelayLabelValueWidgetAction();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::delayWheelValueChanged(double pValue)
{
    // Update our delay value widget

    mDelayWheelValue = pValue;

    quint64 delay = 0;
    quint64 increment = 1;
    quint64 multiple = 10;

    for (int i = 0, iMax = int(pValue); i < iMax; ++i) {
        delay += increment;

        if (delay%multiple == 0) {
            increment *= 10;
            multiple *= 10;
        }
    }

    const QList<QLabel *> labels = mDelayLabelValueWidgetAction->labels();
    QString delayString = QLocale().toString(delay);

    for (auto *label : labels) {
        label->setText(delayString);
    }

    // Also update our simulation object

    mSimulation->setDelay(delay);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::delayWheelReleased()
{
    // Update the value of our various delay wheels

    const QList<QwtWheel *> wheels = mDelayWheelWidgetAction->wheels();

    for (auto *wheel : wheels) {
        wheel->setValue(mDelayWheelValue);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationRunning(bool pIsResuming)
{
    Q_UNUSED(pIsResuming)

    // Our simulation is running, so update our simulation mode and check for
    // results

    updateSimulationMode();

    mViewWidget->checkSimulationResults(mSimulation->fileName());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationPaused()
{
    // Our simulation is paused, so update our simulation mode and parameters,
    // and check for results

    updateSimulationMode();

    mContentsWidget->informationWidget()->parametersWidget()->updateParameters(mSimulation->currentPoint());

    mViewWidget->checkSimulationResults(mSimulation->fileName());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationDone(qint64 pElapsedTime)
{
    // Output the given elapsed time, if valid

    if (pElapsedTime != -1) {
        QString solversInformation = mSimulation->data()->odeSolverName();

        if (!mSimulation->data()->nlaSolverName().isEmpty()) {
            solversInformation += "+"+mSimulation->data()->nlaSolverName();
        }

        output(QString(QString()+OutputTab+"<strong>"+tr("Simulation time:")+"</strong> <span "+OutputInfo+">"+tr("%1 using %2").arg(Core::formatTime(pElapsedTime),
                                                                                                                                    solversInformation)+"</span>."+OutputBrLn));
    }

    // Update our parameters and simulation mode

    updateSimulationMode();

    mContentsWidget->informationWidget()->parametersWidget()->updateParameters(mSimulation->currentPoint());

    // Stop tracking our simulation progress and reset our file tab icon

    mProgress = -1;

    resetFileTabIcon();
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
    // Let people know that our file tab icon should be reset

    static const QIcon NoIcon;

    emit mViewWidget->updateFileTabIcon(mPlugin->viewName(),
                                        mSimulation->fileName(),
                                        NoIcon);
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
    //          completed, but this is only if we don't need to reload
    //          ourselves. Indeed, if we need to reload ourselves (see
    //          fileReloaded()), we want things to be done as quickly as
    //          possible. Not only that, but we don't want to risk problems with
    //          our simulation being used while it has already been deleted due
    //          to threading issues...

    QTimer::singleShot(169, this,
                       isVisible()?
                           &SimulationExperimentViewSimulationWidget::resetProgressBar:
                           &SimulationExperimentViewSimulationWidget::resetFileTabIcon);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationError(const QString &pMessage,
                                                               Error pError)
{
    // Output the simulation error

    mError = pError;

    updateInvalidModelMessageWidget();

    output(QString()+OutputTab+"<span "+OutputBad+"><strong>"+tr("Error:")+"</strong> "+pMessage+".</span>"+OutputBrLn);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationError(const QString &pMessage)
{
    // Output the simulation error

    simulationError(pMessage, Error::General);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationDataModified(bool pIsModified)
{
    // Update some of our actions based on whether we are modified

    if (mDevelopmentModeAction->isChecked()) {
        Core::FileManager::instance()->setModified(mSimulation->fileName(), pIsModified);
    }

    mResetStateModelParametersAction->setEnabled(mSimulation->data()->isStatesModified());
    mResetAllModelParametersAction->setEnabled(pIsModified);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationResultsReset()
{
    // Update our simulation mode and check for results
    // Note: see clearSimulationResults() for the reason behind temporarily
    //       disabling updates...

    setUpdatesEnabled(false);
        updateSimulationMode();

        mViewWidget->checkSimulationResults(mSimulation->fileName(), Task::ResetRuns);
    setUpdatesEnabled(true);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationResultsRunAdded()
{
    // A run has been added, so check our simulation results

    mViewWidget->checkSimulationResults(mSimulation->fileName(), Task::AddRun);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationPropertyChanged(Core::Property *pProperty)
{
    // Update our simulation properties, as well as our plots

    updateSimulationProperties(pProperty);

    bool needProcessingEvents = false;
    // Note: needProcessingEvents is used to ensure that our plots are all
    //       updated at once...

    for (auto plot : qAsConst(mPlots)) {
        if (updatePlot(plot, true, false)) {
            needProcessingEvents = true;
        }
    }

    if (needProcessingEvents) {
        processEvents();
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::solversPropertyChanged(Core::Property *pProperty)
{
    // Update our solvers properties

    updateSolversProperties(pProperty);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphPanelAdded(GraphPanelWidget::GraphPanelWidget *pGraphPanel)
{
    // Let people know when some graph panel settings or graphs settings have
    // been requested

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    connect(plot, &GraphPanelWidget::GraphPanelPlotWidget::graphPanelSettingsRequested,
            this, &SimulationExperimentViewSimulationWidget::graphPanelSettingsRequested);
    connect(plot, &GraphPanelWidget::GraphPanelPlotWidget::graphsSettingsRequested,
            this, &SimulationExperimentViewSimulationWidget::graphsSettingsRequested);

    // Let people know when we a graph has been toggled

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = contentsWidget()->informationWidget()->graphPanelAndGraphsWidget();

    connect(plot, &GraphPanelWidget::GraphPanelPlotWidget::graphToggled,
            graphPanelAndGraphsWidget, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::toggleGraph);

    // Let people know when we the legend has been toggled

    connect(plot, &GraphPanelWidget::GraphPanelPlotWidget::legendToggled,
            graphPanelAndGraphsWidget, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::toggleLegend);

    // Let people know when we the X/Y logarithmic axis has been toggled

    connect(plot, &GraphPanelWidget::GraphPanelPlotWidget::logarithmicXAxisToggled,
            graphPanelAndGraphsWidget, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::toggleLogarithmicXAxis);
    connect(plot, &GraphPanelWidget::GraphPanelPlotWidget::logarithmicYAxisToggled,
            graphPanelAndGraphsWidget, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::toggleLogarithmicYAxis);

    // Check our graph panels and their graphs

    checkGraphPanelsAndGraphs();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphPanelRemoved(GraphPanelWidget::GraphPanelWidget *pGraphPanel)
{
    // A graph panel has been removed, so stop tracking it

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    mPlots.removeOne(plot);

    // Check our graph panels and their graphs
    // Note: we use a single shot to give the GUI time to update, especially if
    //       we removed the last graph panel in which case a new graph panel
    //       will be added...

    QTimer::singleShot(0, this, &SimulationExperimentViewSimulationWidget::checkGraphPanelsAndGraphs);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                                                        CellMLSupport::CellmlFileRuntimeParameter *pParameterY)
{
    // Ask the current graph panel to add a new graph for the given parameters
    // Note: due to the way legend labels are handled by Qwt, we temporarily
    //       disable their update (and re-enable it once the graph has been
    //       added; see graphAdded()) since otherwise we may see the label flash
    //       due to its properties (e.g. font size) not being the same as those
    //       of QwtLegendLabel...

    GraphPanelWidget::GraphPanelWidget *graphPanel = mContentsWidget->graphPanelsWidget()->activeGraphPanel();

    graphPanel->plot()->legend()->setUpdatesEnabled(false);

    auto graph = new GraphPanelWidget::GraphPanelPlotGraph(pParameterX, pParameterY, graphPanel);

    graphPanel->addGraph(graph, defaultGraphProperties(pParameterY->formattedName(), graph->color()));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphAdded(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                          GraphPanelWidget::GraphPanelPlotGraph *pGraph)
{
    // A new graph has been added, so add runs to it, if our simulation has some

    for (int i = 0, iMax = mSimulation->runsCount(); i < iMax; ++i) {
        pGraph->addRun();
    }

    // Allow our plot's legend to get updated

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    plot->legend()->setUpdatesEnabled(true);

    // Now, keep track of the graph and update its plot
    // Note: updating the plot will, if needed, update the plot's axes and, as a
    //       result, replot the graphs including our new one. On the other hand,
    //       if the plot's axes don't get updated, we need to draw our new
    //       graph...

    for (int i = 0, iMax = mSimulation->runsCount(); i < iMax; ++i) {
        updateGraphData(pGraph, mSimulation->results()->size(i), i);
    }

    if (   updatePlot(plot, true, false)
        || plot->drawGraphFrom(pGraph, 0)) {
        processEvents();
        // Note: this ensures that our plot is updated at once...
    }

    // Keep track of the plot itself, if needed

    if (!mPlots.contains(plot)) {
        mPlots << plot;
    }

    // Check our graph panels and their graphs

    checkGraphPanelsAndGraphs();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphsRemoved(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                             const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs)
{
    Q_UNUSED(pGraphs)

    // One or several graphs have been removed, so update and stop tracking (if
    // needed) their corresponding plot
    // Note: even if the axes' values of the plot haven't changed, we still want
    //       to replot the plot since at least one of its graphs has been
    //       removed...

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    updatePlot(plot, true, true);

    processEvents();
    // Note: this ensures that our plot is updated at once...

    if (plot->graphs().isEmpty()) {
        mPlots.removeOne(plot);
    }

    // Check our graph panels and their graphs

    checkGraphPanelsAndGraphs();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphsUpdated(const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs,
                                                             bool pCanSetAxes)
{
    // One or several graphs have been updated, so make sure that their
    // corresponding plots are up to date

    GraphPanelWidget::GraphPanelPlotWidgets plots = GraphPanelWidget::GraphPanelPlotWidgets();

    for (auto graph : pGraphs) {
        // Show/hide the graph

        graph->setVisible(graph->isValid() && graph->isSelected());

        // Update the graph's data, accounting for the fact that the simulation
        // may (now) have a different number of runs
        // Note: we need to check that we have a simulation since our graph may
        //       indeed refer to a file that has not yet been activated and/or
        //       doesn't yet have a simulation associated with it...

        SimulationSupport::Simulation *simulation = mViewWidget->simulation(graph->fileName());

        graph->removeRuns();

        if (simulation != nullptr) {
            for (int i = 0, iMax = simulation->runsCount(); i < iMax; ++i) {
                graph->addRun();

                updateGraphData(graph, simulation->results()->size(i), i);
            }
        }

        // Keep track of the plot that needs to be updated and replotted

        plots << qobject_cast<GraphPanelWidget::GraphPanelPlotWidget *>(graph->plot());
    }

    // Update and replot our various plots, if allowed

    if (mCanUpdatePlotsForUpdatedGraphs) {
        for (auto plot : qAsConst(plots)) {
            updatePlot(plot, pCanSetAxes, true);
            // Note: even if the axes' values of the plot haven't changed, we
            //       still want to replot the plot since at least one of its
            //       graphs has been updated...
        }

        processEvents();
        // Note: this ensures that our plots are all updated at once...
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphUpdated(GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                                                            bool pCanSetAxes)
{
    // The given graph has been updated, so make sure that its corresponding
    // plots are up to date

    graphsUpdated(GraphPanelWidget::GraphPanelPlotGraphs() << pGraph, pCanSetAxes);
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::updatePlot(GraphPanelWidget::GraphPanelPlotWidget *pPlot,
                                                          bool pCanSetAxes,
                                                          bool pForceReplot)
{
    // Retrieve the current axes' linear and log values or use some default
    // ones, if none are available

    double minX = GraphPanelWidget::DefaultMinAxis;
    double maxX = GraphPanelWidget::DefaultMaxAxis;
    double minY = GraphPanelWidget::DefaultMinAxis;
    double maxY = GraphPanelWidget::DefaultMaxAxis;

    QRectF dataRect;

    if (pPlot->dataRect(dataRect)) {
        minX = dataRect.left();
        maxX = minX+dataRect.width();
        minY = dataRect.top();
        maxY = minY+dataRect.height();
    }

    double minLogX = GraphPanelWidget::DefaultMinLogAxis;
    double maxLogX = GraphPanelWidget::DefaultMaxAxis;
    double minLogY = GraphPanelWidget::DefaultMinLogAxis;
    double maxLogY = GraphPanelWidget::DefaultMaxAxis;

    QRectF dataLogRect;

    if (pPlot->dataLogRect(dataLogRect)) {
        minLogX = dataLogRect.left();
        maxLogX = minLogX+dataLogRect.width();
        minLogY = dataLogRect.top();
        maxLogY = minLogY+dataLogRect.height();
    }

    // Check all the graphs associated with the given plot and see whether any
    // of them uses the VOI as parameter X and/or Y, and if so then asks the
    // plot to use the starting/ending points as the minimum/maximum values for
    // the X and/or Y axes

    const GraphPanelWidget::GraphPanelPlotGraphs graphs = pPlot->graphs();
    bool hasData = pPlot->hasData();
    bool needInitialisationX = true;
    bool needInitialisationY = true;

    for (auto graph : graphs) {
        if (graph->isVisible()) {
            SimulationSupport::Simulation *simulation = mViewWidget->simulation(graph->fileName());
            double startingPoint = simulation->data()->startingPoint();
            double endingPoint = simulation->data()->endingPoint();
            double pointInterval = simulation->data()->pointInterval();
            double startingLogPoint = startingPoint;
            double endingLogPoint = endingPoint;
            int nbOfPointIntervals = 0;

            if (pointInterval > 0.0) {
                while (startingLogPoint <= 0.0) {
                    startingLogPoint = startingPoint+(++nbOfPointIntervals)*pointInterval;
                }

                nbOfPointIntervals = 0;

                while (endingLogPoint <= 0.0) {
                    endingLogPoint = endingPoint+(++nbOfPointIntervals)*pointInterval;
                }
            } else if (pointInterval < 0.0) {
                while (startingLogPoint <= 0.0) {
                    startingLogPoint = startingPoint-(++nbOfPointIntervals)*pointInterval;
                }

                nbOfPointIntervals = 0;

                while (endingLogPoint <= 0.0) {
                    endingLogPoint = endingPoint-(++nbOfPointIntervals)*pointInterval;
                }
            }

            if (startingPoint > endingPoint) {
                std::swap(startingPoint, endingPoint);
            }

            if (static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterX())->type() == CellMLSupport::CellmlFileRuntimeParameter::Type::Voi) {
                if (!hasData && needInitialisationX) {
                    minX = startingPoint;
                    maxX = endingPoint;

                    minLogX = startingLogPoint;
                    maxLogX = endingLogPoint;

                    needInitialisationX = false;
                } else {
                    minX = qMin(minX, startingPoint);
                    maxX = qMax(maxX, endingPoint);

                    minLogX = qMin(minLogX, startingLogPoint);
                    maxLogX = qMax(maxLogX, endingLogPoint);
                }
            }

            if (static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterY())->type() == CellMLSupport::CellmlFileRuntimeParameter::Type::Voi) {
                if (!hasData && needInitialisationY) {
                    minY = startingPoint;
                    maxY = endingPoint;

                    minLogY = startingLogPoint;
                    maxLogY = endingLogPoint;

                    needInitialisationY = false;
                } else {
                    minY = qMin(minY, startingPoint);
                    maxY = qMax(maxY, endingPoint);

                    minLogY = qMin(minLogY, startingLogPoint);
                    maxLogY = qMax(maxLogY, endingLogPoint);
                }
            }
        }
    }

    // Optimise our axes' values before setting them and replotting our plot, if
    // needed

    pPlot->optimizeAxisX(minX, maxX, GraphPanelWidget::GraphPanelPlotWidget::Optimization::Linear);
    pPlot->optimizeAxisY(minY, maxY, GraphPanelWidget::GraphPanelPlotWidget::Optimization::Linear);

    pPlot->optimizeAxisX(minLogX, maxLogX, GraphPanelWidget::GraphPanelPlotWidget::Optimization::Logarithmic);
    pPlot->optimizeAxisY(minLogY, maxLogY, GraphPanelWidget::GraphPanelPlotWidget::Optimization::Logarithmic);

    pPlot->setDefaultAxesValues(minX, maxX, minLogX, maxLogX,
                                minY, maxY, minLogY, maxLogY);

    if (   pCanSetAxes
        && pPlot->setAxes(pPlot->logAxisX()?minLogX:minX,
                          pPlot->logAxisX()?maxLogX:maxX,
                          pPlot->logAxisY()?minLogY:minY,
                          pPlot->logAxisY()?maxLogY:maxY,
                          true, false, true, true, false, false)) {
        return true;
    }

    if (pForceReplot) {
        pPlot->replot();

        return true;
    }

    return false;
}

//==============================================================================

double * SimulationExperimentViewSimulationWidget::data(SimulationSupport::Simulation *pSimulation,
                                                        CellMLSupport::CellmlFileRuntimeParameter *pParameter,
                                                        int pRun) const
{
    // Return the array of data points associated with the given parameter

    CellMLSupport::CellmlFileRuntimeParameter::Type paraameterType = pParameter->type();

    if (paraameterType == CellMLSupport::CellmlFileRuntimeParameter::Type::Voi) {
        return pSimulation->results()->points(pRun);
    }

    if (   (paraameterType == CellMLSupport::CellmlFileRuntimeParameter::Type::Constant)
        || (paraameterType == CellMLSupport::CellmlFileRuntimeParameter::Type::ComputedConstant)) {
        return pSimulation->results()->constants(pParameter->index(), pRun);
    }

    if (paraameterType == CellMLSupport::CellmlFileRuntimeParameter::Type::Rate) {
        return pSimulation->results()->rates(pParameter->index(), pRun);
    }

    if (paraameterType == CellMLSupport::CellmlFileRuntimeParameter::Type::State) {
        return pSimulation->results()->states(pParameter->index(), pRun);
    }

    if (paraameterType == CellMLSupport::CellmlFileRuntimeParameter::Type::Algebraic) {
        return pSimulation->results()->algebraic(pParameter->index(), pRun);
    }

    if (paraameterType == CellMLSupport::CellmlFileRuntimeParameter::Type::Data) {
        return pSimulation->results()->data(pParameter->data(),
                                            pParameter->index(), pRun);
    }

    // Not a relevant type, so return null
    // Note: we should never reach this point...

    return nullptr;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateGraphData(GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                                                               quint64 pSize,
                                                               int pRun)
{
    // Update our graph's data from the given run

    if (pGraph->isValid()) {
        SimulationSupport::Simulation *simulation = mViewWidget->simulation(pGraph->fileName());

        pGraph->setData(data(simulation, static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(pGraph->parameterX()), pRun),
                        data(simulation, static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(pGraph->parameterY()), pRun),
                        pSize, pRun);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateGui(bool pCheckVisibility)
{
    // Make sure that we are visible, if requested

    if (pCheckVisibility && !isVisible()) {
        return;
    }

    // Make sure that our graph panel and graphs widget's GUI is up to date

    mContentsWidget->informationWidget()->graphPanelAndGraphsWidget()->updateGui();

    // Make sure that our plots are up to date
    // Note: indeed, say that we start a simulation with some graphs and switch
    //       to another file before the simulation is complete. Now, if we
    //       switch back to the file after the simulation has completed, the
    //       plots won't be up to date (since the simulation widget was not
    //       visible and therefore didn't update its plots), so we need to do it
    //       here...

    if (mNeedUpdatePlots) {
        mNeedUpdatePlots = false;

        for (auto plot : qAsConst(mPlots)) {
            updatePlot(plot, true, true);
        }

        processEvents();
        // Note: this ensures that our plots are all updated at once...
    }

    // Make sure that our progress bar is up to date

    mProgressBarWidget->setValue(double(mViewWidget->simulationResultsSize(mSimulation->fileName()))/double(mSimulation->size()));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSimulationResults(SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                       quint64 pSimulationResultsSize,
                                                                       int pSimulationRun,
                                                                       Task pTask)
{
    // Update our simulation results

    SimulationSupport::Simulation *simulation = pSimulationWidget->simulation();

    if (simulation == mSimulation) {
        simulationDataModified(simulation->data()->isModified());
    }

    // Update all the graphs of all our plots, but only if we are visible
    // Note: needProcessingEvents is used to ensure that our plots are all
    //       updated at once...

    bool visible = isVisible();
    bool needProcessingEvents = false;

    for (auto plot : qAsConst(mPlots)) {
        // Now we are ready to actually update all the graphs of all our plots
        // Note: we determine whether our plot's axes are dirty before adding
        //       any new data to it, so that we can determine whether we need
        //       to update our plot's axes...

        const GraphPanelWidget::GraphPanelPlotGraphs graphs = plot->graphs();
        bool needFullUpdatePlot = !plot->isOptimizedAxes();
        double plotMinX = plot->minX();
        double plotMaxX = plot->maxX();
        double plotMinY = plot->minY();
        double plotMaxY = plot->maxY();
        bool hasDirtyAxes = plot->hasDirtyAxes();

        for (auto graph : graphs) {
            if (graph->fileName() == pSimulationWidget->simulation()->fileName()) {
                if (pTask != Task::None) {
                    mOldDataSizes.remove(graph);
                }

                // Reset our runs or a add new one, if needed

                if (pTask == Task::ResetRuns) {
                    graph->removeRuns();
                } else if (pTask == Task::AddRun) {
                    graph->addRun();
                }

                // Update our graph's data and keep track of our new old data
                // size, if we are visible
                // Note: indeed, to update our graph's old data size if we are
                //       not visible means that when we come back to this file,
                //       part of the graphs will be missing...

                quint64 oldDataSize = graph->dataSize();

                if (visible) {
                    mOldDataSizes.insert(graph, oldDataSize);
                }

                updateGraphData(graph, pSimulationResultsSize, pSimulationRun);

                // We need to update our plot, if we are drawing this graph's
                // first segment or if we were invisible at some point during
                // the simulation

                quint64 realOldDataSize = mOldDataSizes.value(graph);

                needFullUpdatePlot =    needFullUpdatePlot || (realOldDataSize == 0)
                                     || (oldDataSize != realOldDataSize);

                // Draw the graph's new segment, but only if we and our graph
                // are visible, and that there is no need to update the plot and
                // that there is some data to plot

                if (    visible && graph->isVisible()
                    && !needFullUpdatePlot && (pSimulationResultsSize != 0)) {
                    // Check that our graph segment can fit within our plot's
                    // current viewport, but only if the user hasn't changed the
                    // plot's viewport since we last came here (e.g. by panning
                    // the plot's contents)

                    if (!hasDirtyAxes) {
                        double minX = plotMinX;
                        double maxX = plotMaxX;
                        double minY = plotMinY;
                        double maxY = plotMaxY;
                        auto graphData = graph->data(pSimulationRun);

                        for (quint64 i = (oldDataSize != 0)?oldDataSize-1:0; i < pSimulationResultsSize; ++i) {
                            auto val = graphData->sample(i);
                            auto valX = val.x();
                            auto valY = val.y();

                            if (   !qIsInf(valX) && !qIsNaN(valX)
                                && !qIsInf(valY) && !qIsNaN(valY)) {
                                minX = qMin(minX, valX);
                                maxX = qMax(maxX, valX);
                                minY = qMin(minY, valY);
                                maxY = qMax(maxY, valY);
                            }
                        }

                        // Update our plot, if our graph segment cannot fit
                        // within our plot's current viewport

                        needFullUpdatePlot =    (minX < plotMinX) || (maxX > plotMaxX)
                                             || (minY < plotMinY) || (maxY > plotMaxY);
                    }

                    if (!needFullUpdatePlot) {
                        if (plot->drawGraphFrom(graph, realOldDataSize-1)) {
                            needProcessingEvents = true;
                        }
                    }
                }
            }
        }

        // Check whether we need to update/replot our plot, but only if we are
        // visible

        if (visible) {
            if (needFullUpdatePlot || (pSimulationResultsSize == 0)) {
                // Either we need a full update of our plot (because we are
                // drawing a graph's first segment or a graph's new segment
                // doesn't fit within the plot's current viewport) or the size
                // of our simulation results is zero (because we are starting a
                // simulation or clearing our plot), so update our plot
                // Note: in case we are starting a simulation or clearing our
                //       plot, we don't want a full update of our plot since
                //       this is going to reset its axes' values and therefore
                //       result in some (expected) flickering, if some data is
                //       to be drawn straight away (e.g. when we start a
                //       simulation)...

                updatePlot(plot, needFullUpdatePlot && !hasDirtyAxes, true);

                needProcessingEvents = true;
            }
        } else if (needFullUpdatePlot || (pSimulationResultsSize == 0)) {
            // We would normally update our plot, but we are not visible, so no
            // point in doing it, so instead we keep track of the fact that we
            // will need to update our plots the next time we become visible

            mNeedUpdatePlots = true;
        }
    }

    // Process events, if needed

    if (needProcessingEvents) {
        processEvents();
    }

    // Update our progress bar or our tab icon, if needed

    if (simulation == mSimulation) {
        QString simulationFileName = mSimulation->fileName();
        double simulationProgress = double(mViewWidget->simulationResultsSize(simulationFileName))/double(simulation->size());

        if ((pTask != Task::None) || visible) {
            mProgressBarWidget->setValue(simulationProgress);
        } else {
            // We are not visible, so create an icon that shows our simulation's
            // progress and let people know about it

            int newProgress = int((tabBarPixmapSize()-2)*simulationProgress);
            // Note: tabBarPixmapSize()-2 because we want a one-pixel wide
            //       border...

            if ((newProgress != mProgress)) {
                // The progress has changed, so keep track of its new value and
                // update our file tab icon

                mProgress = newProgress;

                // Let people know about the file tab icon to be used for the
                // model

                emit mViewWidget->updateFileTabIcon(mPlugin->viewName(),
                                                    simulationFileName,
                                                    doFileTabIcon(true));
            }
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::openCellmlFile()
{
    // Ask OpenCOR to open our referenced CellML file

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QString cellmlFileName = mSimulation->cellmlFile()->fileName();

    QDesktopServices::openUrl(QString("opencor://openFile/%1").arg(fileManagerInstance->isRemote(cellmlFileName)?
                                                                       fileManagerInstance->url(cellmlFileName):
                                                                       QUrl::fromLocalFile(cellmlFileName).toEncoded()));

    // Ask OpenCOR to switch to the requested CellML editing view

    Plugin *plugin = mCellmlBasedViewPlugins.value(qobject_cast<QAction *>(sender()));

    if (plugin != nullptr) {
        QDesktopServices::openUrl("opencor://Core.selectView/"+plugin->name());
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreImportProgress(DataStore::DataStoreImportData *pImportData,
                                                                       double pProgress)
{
    Q_UNUSED(pImportData)

    // There has been some progress with our import, so update our busy widget

    Core::setCentralBusyWidgetProgress(pProgress);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreImportDone(DataStore::DataStoreImportData *pImportData,
                                                                   const QString &pErrorMessage)
{
    // Ask our simulation to account for our imported data, and update our
    // Graphs and Parameters sections with our imported data

    mSimulation->importData(pImportData);

    mContentsWidget->informationWidget()->graphPanelAndGraphsWidget()->importData(pImportData);
    mContentsWidget->informationWidget()->parametersWidget()->importData(pImportData);

    // Hide our busy widget

    Core::hideCentralBusyWidget();

    // Let people know about any error that we came across

    if (!pErrorMessage.isEmpty()) {
        Core::warningMessageBox(tr("Data Import"),
                                tr("<strong>%1</strong> could not be imported (%2).").arg(pImportData->fileName(),
                                                                                          Core::formatMessage(pErrorMessage, true)));
    }

    emit importDone(static_cast<DataStore::DataStoreImporter *>(sender()));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreExportProgress(DataStore::DataStoreExportData *pDataStoreData,
                                                                       double pProgress)
{
    Q_UNUSED(pDataStoreData)

    // There has been some progress with our export, so update our busy widget

    Core::setCentralBusyWidgetProgress(pProgress);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreExportDone(DataStore::DataStoreExportData *pDataStoreData,
                                                                   const QString &pErrorMessage)
{
    Q_UNUSED(pDataStoreData)

    // We are done with the export, so hide our busy widget

    Core::hideCentralBusyWidget();

    // Display the given error message, if any

    if (!pErrorMessage.isEmpty()) {
        Core::warningMessageBox(tr("Simulation Results Export"),
                                pErrorMessage);
    }

    emit exportDone(static_cast<DataStore::DataStoreExporter *>(sender()));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::checkSimulationProperties()
{
    // Check whether any of our simulation properties has changed

    mSimulationPropertiesModified = allPropertyValues(mContentsWidget->informationWidget()->simulationWidget()) != mSimulationProperties;

    // Update our file's modified status

    updateSedmlFileOrCombineArchiveModifiedStatus();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::checkSolversProperties()
{
    // Check whether any of our solver properties has changed

    mSolversPropertiesModified = allPropertyValues(mContentsWidget->informationWidget()->solversWidget()) != mSolversProperties;

    // Update our file's modified status

    updateSedmlFileOrCombineArchiveModifiedStatus();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::checkGraphPanelsAndGraphs()
{
    // Make sure that we are dealing with a non-CellML file and that
    // mGraphPanelsWidgetSizes has been initialised

    if (   (   (mSimulation->fileType() != SimulationSupport::Simulation::FileType::SedmlFile)
            && (mSimulation->fileType() != SimulationSupport::Simulation::FileType::CombineArchive))
        || mGraphPanelsWidgetSizes.isEmpty()) {
        return;
    }

    // Make sure that we have the expected number of graph panels
    // Note: when adding/removing graph panels, we will come here when the graph
    //       panel hasn't yet been added/removed. So, we want to make sure that
    //       it has been. This is particularly important when the user deletes
    //       the last graph panel (i.e. resets it) since
    //       GraphPanelsWidget::removeCurrentGraphPanel() will first add a graph
    //       panel and then delete the "last" one, which if we were not to test
    //       things would result in the file to be considered modified for a
    //       split second...

    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();
    const GraphPanelWidget::GraphPanelWidgets graphPanels = graphPanelsWidget->graphPanels();

    if (mNbOfGraphPanels != graphPanels.count()) {
        return;
    }

    // Check whether any of our graph panels' height has changed

    mGraphPanelsWidgetSizesModified = graphPanelsWidget->sizes() != mGraphPanelsWidgetSizes;

    // Check whether any of our graph panel / graphs properties has changed

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = mContentsWidget->informationWidget()->graphPanelAndGraphsWidget();

    mGraphPanelPropertiesModified.clear();
    mGraphsPropertiesModified.clear();

    int i = -1;

    for (auto graphPanel : graphPanels) {
        ++i;

        mGraphPanelPropertiesModified << (   (i < mGraphPanelProperties.count())
                                          && (allPropertyValues(graphPanelAndGraphsWidget->graphPanelPropertyEditor(graphPanel)) != mGraphPanelProperties[i]));
        mGraphsPropertiesModified << (   (i < mGraphsProperties.count())
                                      && (allPropertyValues(graphPanelAndGraphsWidget->graphsPropertyEditor(graphPanel)) != mGraphsProperties[i]));
    }

    // Update our file's modified status

    updateSedmlFileOrCombineArchiveModifiedStatus();
}

//==============================================================================

QVariantList SimulationExperimentViewSimulationWidget::allPropertyValues(Core::PropertyEditorWidget *pPropertyEditor) const
{
    // Return all the property values of the given property editor

    QVariantList res;
    const Core::Properties properties = pPropertyEditor->allProperties();

    for (auto property : properties) {
        res << property->isChecked() << property->variantValue();
    }

    return res;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSedmlFileOrCombineArchiveModifiedStatus()
{
    // Make sure that we are not dealing with a remote file

    QString simulationFileName = mSimulation->fileName();

    if (Core::FileManager::instance()->isRemote(simulationFileName)) {
        return;
    }

    // Update the modified status of the current file, based on whether its
    // simulation, solvers, graph panel or graphs properties have changed, and
    // keeping in mind that we may have added/removed graph panels

    bool graphPanelPropertiesModified = mGraphPanelProperties.count() != mGraphPanelProperties.count();

    if (!graphPanelPropertiesModified) {
        for (auto someGraphPanelPropertiesModified : qAsConst(mGraphPanelPropertiesModified)) {
            graphPanelPropertiesModified = graphPanelPropertiesModified || someGraphPanelPropertiesModified;
        }
    }

    bool graphsPropertiesModified = mGraphsProperties.count() != mGraphsPropertiesModified.count();

    if (!graphsPropertiesModified) {
        for (auto someGraphsPropertiesModified : qAsConst(mGraphsPropertiesModified)) {
            graphsPropertiesModified = graphsPropertiesModified || someGraphsPropertiesModified;
        }
    }

    Core::FileManager::instance()->setModified(simulationFileName,
                                                  mValidSimulationEnvironment
                                               && (   mSimulationPropertiesModified
                                                   || mSolversPropertiesModified
                                                   || graphPanelPropertiesModified
                                                   || graphsPropertiesModified
                                                   || mGraphPanelsWidgetSizesModified));
}

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

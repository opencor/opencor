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

#include "centralwidget.h"
#include "combineinterface.h"
#include "combinesupportplugin.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "graphpanelswidget.h"
#include "interfaces.h"
#include "progressbarwidget.h"
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
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"
#include "simulationmanager.h"
#include "toolbarwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QDragEnterEvent>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMimeData>
#include <QScreen>
#include <QTextEdit>
#include <QTimer>
#include <QToolButton>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_wheel.h"
#include "qwtend.h"

//==============================================================================

#include "libsbmlbegin.h"
    #include "sbml/math/FormulaParser.h"
#include "libsbmlend.h"

//==============================================================================

#include "libsedmlbegin.h"
    #include "sedml/SedAlgorithm.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedOneStep.h"
    #include "sedml/SedPlot2D.h"
    #include "sedml/SedRepeatedTask.h"
    #include "sedml/SedSimulation.h"
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
    mViewWidget(pViewWidget),
    mDataStoreInterfaces(QMap<QAction *, DataStoreInterface *>()),
    mCellmlBasedViewPlugins(QMap<QAction *, Plugin *>()),
    mProgress(-1),
    mLockedDevelopmentMode(false),
    mRunActionEnabled(true),
    mErrorType(General),
    mValidSimulationEnvironment(false),
    mPlots(GraphPanelWidget::GraphPanelPlotWidgets()),
    mUpdatablePlotViewports(QMap<GraphPanelWidget::GraphPanelPlotWidget *, bool>()),
    mSimulationProperties(QStringList()),
    mSolversProperties(QStringList()),
    mGraphPanelProperties(QMap<Core::PropertyEditorWidget *, QStringList>()),
    mGraphsProperties(QMap<Core::PropertyEditorWidget *, QStringList>()),
    mSimulationPropertiesModified(false),
    mSolversPropertiesModified(false),
    mGraphPanelPropertiesModified(QMap<Core::PropertyEditorWidget *, bool>()),
    mGraphsPropertiesModified(QMap<Core::PropertyEditorWidget *, bool>()),
    mGraphPanelsWidgetSizes(QIntList()),
    mGraphPanelsWidgetSizesModified(false),
    mCanUpdatePlotsForUpdatedGraphs(true),
    mNeedUpdatePlots(false),
    mOldDataSizes(QMap<GraphPanelWidget::GraphPanelPlotGraph *, quint64>()),
    mDataImportProgresses(QMap<DataStore::DataStoreImportData *, double>()),
    mDataImportErrorMessages(QMap<DataStore::DataStoreImportData *, QString>()),
    mDataStoreFiles(QMap<QString, FileTypeInterface *>())
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

    connect(mSimulation->data(), &SimulationSupport::SimulationData::modified,
            this, &SimulationExperimentViewSimulationWidget::simulationDataModified);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create a tool bar

    mToolBarWidget = new Core::ToolBarWidget();

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
    mSedmlExportSedmlFileAction = (mSimulation->fileType() == SimulationSupport::Simulation::CellmlFile)?
                                      Core::newAction(mToolBarWidget):
                                      nullptr;
    mSedmlExportCombineArchiveAction = (mSimulation->fileType() != SimulationSupport::Simulation::CombineArchive)?
                                           Core::newAction(mToolBarWidget):
                                           nullptr;
    mDataImportAction = Core::newAction(QIcon(":/oxygen/actions/document-import.png"),
                                        mToolBarWidget);
    mSimulationResultsExportAction = Core::newAction(QIcon(":/oxygen/actions/document-export.png"),
                                                     mToolBarWidget);
    mPreferencesAction = Core::newAction(QIcon(":/oxygen/categories/preferences-system.png"),
                                         mToolBarWidget);

    mCellmlOpenAction->setEnabled(mSimulation->fileType() != SimulationSupport::Simulation::CellmlFile);
    mSedmlExportAction->setEnabled(mSimulation->fileType() != SimulationSupport::Simulation::CombineArchive);

    connect(mRunPauseResumeSimulationAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::runPauseResumeSimulation);
    connect(mStopSimulationAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::stopSimulation);
    connect(mResetStateModelParametersAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::resetStateModelParameters);
    connect(mResetAllModelParametersAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::resetAllModelParameters);
    connect(mClearSimulationResultsAction, &QAction::triggered,
            this, QOverload<>::of(&SimulationExperimentViewSimulationWidget::clearSimulationResults));
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
            this, mSedmlExportSedmlFileAction?
                      QOverload<>::of(&SimulationExperimentViewSimulationWidget::sedmlExportSedmlFile):
                      QOverload<>::of(&SimulationExperimentViewSimulationWidget::sedmlExportCombineArchive));

    if (mSedmlExportSedmlFileAction) {
        connect(mSedmlExportSedmlFileAction, &QAction::triggered,
                this, QOverload<>::of(&SimulationExperimentViewSimulationWidget::sedmlExportSedmlFile));
    }

    if (mSedmlExportCombineArchiveAction) {
        connect(mSedmlExportCombineArchiveAction, &QAction::triggered,
                this, QOverload<>::of(&SimulationExperimentViewSimulationWidget::sedmlExportCombineArchive));
    }

    connect(mDataImportAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::dataImport);
    connect(mPreferencesAction, &QAction::triggered,
            this, &SimulationExperimentViewSimulationWidget::preferences);

    // Enable/disable our development mode action depending on whether our file
    // is readable/writable and of CellML type

    mDevelopmentModeAction->setEnabled(   Core::FileManager::instance()->isReadableAndWritable(pFileName)
                                       && (mSimulation->fileType() == SimulationSupport::Simulation::CellmlFile));

    // Create a wheel (and a label to show its value) to specify the delay (in
    // milliseconds) between the output of two data points

    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();

    mDelayWidget = new QwtWheel(mToolBarWidget);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    QWidget *delaySpaceWidget = new QWidget(mToolBarWidget);
#endif
    mDelayValueWidget = new QLabel(mToolBarWidget);

    mDelayWidget->setBorderWidth(0);
    mDelayWidget->setFixedSize(int(0.07*availableGeometry.width()),
                               mDelayWidget->height() >> 1);
    mDelayWidget->setFocusPolicy(Qt::NoFocus);
    mDelayWidget->setRange(0.0, 55.0);
    mDelayWidget->setWheelBorderWidth(0);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    delaySpaceWidget->setFixedWidth(4);
#endif

    connect(mDelayWidget, &QwtWheel::valueChanged,
            this, &SimulationExperimentViewSimulationWidget::updateDelayValue);

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

    for (auto cellmlEditingViewPlugin : pViewWidget->cellmlEditingViewPlugins()) {
        QAction *action = Core::newAction(Core::mainWindow());

        cellmlOpenDropDownMenu->addAction(action);

        mCellmlBasedViewPlugins.insert(action, cellmlEditingViewPlugin);

        connect(action, &QAction::triggered,
                this, &SimulationExperimentViewSimulationWidget::openCellmlFile);
    }

    cellmlOpenDropDownMenu->addSeparator();

    for (auto cellmlSimulationViewPlugin : pViewWidget->cellmlSimulationViewPlugins()) {
        QAction *action = Core::newAction(Core::mainWindow());

        cellmlOpenDropDownMenu->addAction(action);

        mCellmlBasedViewPlugins.insert(action, cellmlSimulationViewPlugin);

        connect(action, &QAction::triggered,
                this, &SimulationExperimentViewSimulationWidget::openCellmlFile);
    }

    QToolButton *sedmlExportToolButton = new QToolButton(mToolBarWidget);
    QMenu *sedmlExportDropDownMenu = new QMenu(sedmlExportToolButton);

    sedmlExportToolButton->setDefaultAction(mSedmlExportAction);
    sedmlExportToolButton->setMenu(sedmlExportDropDownMenu);
    sedmlExportToolButton->setPopupMode(QToolButton::MenuButtonPopup);

    if (mSedmlExportSedmlFileAction)
        sedmlExportDropDownMenu->addAction(mSedmlExportSedmlFileAction);

    if (mSedmlExportCombineArchiveAction)
        sedmlExportDropDownMenu->addAction(mSedmlExportCombineArchiveAction);

    QToolButton *simulationResultsExportToolButton = new QToolButton(mToolBarWidget);

    mSimulationResultsExportDropDownMenu = new QMenu(simulationResultsExportToolButton);

    simulationResultsExportToolButton->setDefaultAction(mSimulationResultsExportAction);
    simulationResultsExportToolButton->setMenu(mSimulationResultsExportDropDownMenu);
    simulationResultsExportToolButton->setPopupMode(QToolButton::InstantPopup);

    for (auto dataStoreInterface : Core::dataStoreInterfaces()) {
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
    mToolBarWidget->addAction(mDataImportAction);
    mToolBarWidget->addWidget(simulationResultsExportToolButton);
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

    QWidget *simulationOutputWidget = new QWidget(this);
    QVBoxLayout *simulationOutputLayout= new QVBoxLayout(simulationOutputWidget);

    simulationOutputLayout->setContentsMargins(QMargins());
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

    mSplitterWidget->setSizes(QIntList() << availableGeometry.height() << 1);

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

    if (mSedmlExportSedmlFileAction) {
        I18nInterface::retranslateAction(mSedmlExportSedmlFileAction, tr("SED-ML File..."),
                                         tr("Export the simulation to SED-ML using a SED-ML file"));
    }

    if (mSedmlExportCombineArchiveAction) {
        I18nInterface::retranslateAction(mSedmlExportCombineArchiveAction, tr("COMBINE Archive..."),
                                         tr("Export the simulation to SED-ML using a COMBINE archive"));
    }

    I18nInterface::retranslateAction(mDataImportAction, tr("Data Import"),
                                     tr("Import some data"));
    I18nInterface::retranslateAction(mSimulationResultsExportAction, tr("Simulation Results Export"),
                                     tr("Export the simulation results"));

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

    for (auto cellmlEditingViewAction : mCellmlBasedViewPlugins.keys()) {
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

void SimulationExperimentViewSimulationWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if it refers to one or
    // several data store files

    bool acceptEvent = false;

    for (const auto &fileName : Core::droppedFileNames(pEvent)) {
        for (auto fileTypeInterface : Core::dataStoreFileTypeInterfaces()) {
            if (fileTypeInterface->isFile(fileName)) {
                mDataStoreFiles.insert(fileName, fileTypeInterface);

                acceptEvent = true;

                break;
            }
        }

        if (acceptEvent)
            break;
    }

    if (acceptEvent)
        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
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
    // Import the one or several data files

    importDataFiles(Core::droppedFileNames(pEvent));

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateDataStoreActions()
{
    // Update our data store actions

    for (auto action : mDataStoreInterfaces.keys()) {
        I18nInterface::retranslateAction(action,
                                         action->text(),
                                         tr("Export the simulation results to %1").arg(mDataStoreInterfaces.value(action)->dataStoreName()));
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

    mClearSimulationResultsAction->setEnabled(    mSimulation->results()->size()
                                              && !simulationModeEnabled);
    mSimulationResultsExportAction->setEnabled(    mSimulationResultsExportDropDownMenu->actions().count()
                                               &&  mSimulation->results()->size()
                                               && !simulationModeEnabled);

    // Give the focus to our focus proxy, in case we leave our simulation mode
    // (so that the user can modify simulation data, etc.)

    if (!simulationModeEnabled)
        focusProxy()->setFocus();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateRunPauseAction(bool pRunActionEnabled)
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

void SimulationExperimentViewSimulationWidget::initialize(bool pReloadingView)
{
    // In the case of a SED-ML file and of a COMBINE archive, we will need
    // to further initialise ourselves, to customise graph panels, etc. (see
    // SimulationExperimentViewSimulationWidget::furtherInitialize()), so we
    // ask our central widget to show its busy widget (which will get hidden
    // in CentralWidget::updateGui())

    bool isSedmlFile = mSimulation->fileType() == SimulationSupport::Simulation::SedmlFile;
    bool isCombineArchive = mSimulation->fileType() == SimulationSupport::Simulation::CombineArchive;

    if (isVisible() && (isSedmlFile || isCombineArchive))
        Core::centralWidget()->showBusyWidget();

    processEvents();
    // Note: this ensures that our GUI is all fine before we start disabling
    //       updates (e.g. when reloading a SED-ML file that references a remote
    //       CellML file)...

    mContentsWidget->setUpdatesEnabled(false);
        // Stop keeping track of certain things (so that updatePlot() doesn't
        // get called unnecessarily)
        // Note: see the corresponding code towards the end of this method...

        SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
        SimulationExperimentViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();

        disconnect(simulationWidget, &SimulationExperimentViewInformationSimulationWidget::propertyChanged,
                   this, &SimulationExperimentViewSimulationWidget::simulationPropertyChanged);

        // Reset our progress

        mProgress = -1;

        // Clean up our output, if needed

        if (pReloadingView)
            mOutputWidget->document()->clear();

        // Output some information about our CellML file

        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        QString simulationFileName = mSimulation->fileName();
        QString fileName = fileManagerInstance->isNew(simulationFileName)?
                               tr("File")+" #"+QString::number(fileManagerInstance->newIndex(simulationFileName)):
                               fileManagerInstance->isRemote(simulationFileName)?
                                   fileManagerInstance->url(simulationFileName):
                                   simulationFileName;
        QString information = "<strong>"+QDir::toNativeSeparators(fileName)+"</strong>"+OutputBrLn;
        SEDMLSupport::SedmlFileIssues sedmlFileIssues = mSimulation->sedmlFile()?
                                                            mSimulation->sedmlFile()->issues():
                                                            SEDMLSupport::SedmlFileIssues();
        COMBINESupport::CombineArchiveIssues combineArchiveIssues = mSimulation->combineArchive()?
                                                                        mSimulation->combineArchive()->issues():
                                                                        COMBINESupport::CombineArchiveIssues();
        bool atLeastOneBlockingSedmlIssue = false;
        bool atLeastOneBlockingCombineIssue = false;

        if (!combineArchiveIssues.isEmpty()) {
            // There is one or several issues with our COMBINE archive, so list
            // it/them

            for (const auto &combineArchiveIssue : combineArchiveIssues) {
                QString issueType;

                switch (combineArchiveIssue.type()) {
                case COMBINESupport::CombineArchiveIssue::Information:
                    issueType = tr("Information:");

                    break;
                case COMBINESupport::CombineArchiveIssue::Error:
                    issueType = tr("Error:");

                    atLeastOneBlockingCombineIssue = true;

                    break;
                case COMBINESupport::CombineArchiveIssue::Warning:
                    issueType = tr("Warning:");

                    break;
                case COMBINESupport::CombineArchiveIssue::Fatal:
                    issueType = tr("Fatal:");

                    atLeastOneBlockingCombineIssue = true;

                    break;
                }

                information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg(issueType)
                                                                                                                .arg(Core::formatMessage(combineArchiveIssue.message()));
            }
        }

        if (!sedmlFileIssues.isEmpty()) {
            // There is one or several issues with our SED-ML file, so list
            // it/them

            for (const auto &sedmlFileIssue : sedmlFileIssues) {
                QString issueType;

                switch (sedmlFileIssue.type()) {
                case SEDMLSupport::SedmlFileIssue::Unknown:
                    // We should never come here...

#ifdef QT_DEBUG
                    qFatal("FATAL ERROR | %s:%d: a SED-ML file issue cannot be of unknown type.", __FILE__, __LINE__);
#else
                    break;
#endif
                case SEDMLSupport::SedmlFileIssue::Information:
                    issueType = tr("Information:");

                    break;
                case SEDMLSupport::SedmlFileIssue::Error:
                    issueType = tr("Error:");

                    atLeastOneBlockingSedmlIssue = true;

                    break;
                case SEDMLSupport::SedmlFileIssue::Warning:
                    issueType = tr("Warning:");

                    break;
                case SEDMLSupport::SedmlFileIssue::Fatal:
                    issueType = tr("Fatal:");

                    atLeastOneBlockingSedmlIssue = true;

                    break;
                }

                if (sedmlFileIssue.line() && sedmlFileIssue.column()) {
                    information += QString(OutputTab+"<span"+OutputBad+"><strong>[%1:%2] %3</strong> %4.</span>"+OutputBrLn).arg(sedmlFileIssue.line())
                                                                                                                            .arg(sedmlFileIssue.column())
                                                                                                                            .arg(issueType)
                                                                                                                            .arg(Core::formatMessage(sedmlFileIssue.message()));
                } else {
                    information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg(issueType)
                                                                                                                    .arg(Core::formatMessage(sedmlFileIssue.message()));
                }
            }
        }

        CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();
        bool validRuntime = runtime && runtime->isValid();

        CellMLSupport::CellmlFileRuntimeParameter *voi = validRuntime?runtime->voi():nullptr;

        if (!atLeastOneBlockingSedmlIssue && !atLeastOneBlockingCombineIssue) {
            information += OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

            if (voi) {
                // A VOI could be retrieved for our CellML file, so we can also
                // output the model type

                information +=  "<span"+OutputGood+">"+tr("valid")+"</span>."+OutputBrLn
                               +QString(OutputTab+"<strong>"+tr("Model type:")+"</strong> <span"+OutputInfo+">%1</span>."+OutputBrLn).arg(runtime->needNlaSolver()?tr("DAE"):tr("ODE"));
            } else {
                // We couldn't retrieve a VOI, which means that we either don't
                // have a runtime or we have one, but it's not valid or it's
                // valid but we really don't have a VOI
                // Note: in the case of a valid runtime and no VOI, we really
                //       shouldn't consider the runtime to be valid, hence we
                //       handle this case here...

                mErrorType = InvalidCellmlFile;

                updateInvalidModelMessageWidget();

                information += "<span"+OutputBad+">"+(runtime?tr("invalid"):tr("none"))+"</span>."+OutputBrLn;

                if (validRuntime) {
                    // We have a valid runtime, but no VOI, which means that the
                    // model doesn't contain any ODE or DAE

                    information += OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+tr("the model must have at least one ODE or DAE")+".</span>"+OutputBrLn;
                } else {
                    // We don't have a valid runtime, so either there are some
                    // problems with the CellML file, its runtime, or even the
                    // parent SED-ML file and/or COMBINE archive
                    // Note: in the case of problems with the SED-ML file and/or
                    //       COMBINE archive, we will already have listed the
                    //       problems, so no need to do anything more in those
                    //       cases...

                    if (sedmlFileIssues.isEmpty() && combineArchiveIssues.isEmpty()) {
                        for (const auto &issue : runtime?
                                                     runtime->issues():
                                                     mSimulation->cellmlFile()?
                                                         mSimulation->cellmlFile()->issues():
                                                         CellMLSupport::CellmlFileIssues()) {
                            information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?tr("Error:"):tr("Warning:"))
                                                                                                                            .arg(issue.message());
                        }
                    }
                }
            }
        }

        output(information);

        // Check whether we have a valid simulation environment, but only if we
        // don't have any blocking SED-ML or COMBINE issues

        SimulationExperimentViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();

        mValidSimulationEnvironment = false;

        if (!atLeastOneBlockingSedmlIssue && !atLeastOneBlockingCombineIssue) {
            // Enable/disable our run/pause action depending on whether we have
            // a VOI

            mRunPauseResumeSimulationAction->setEnabled(voi);

            // Update our simulation mode or clear our simulation results
            // (should there be some) in case we are reloading ourselves
            // Note: to clear our simualtion data will also update our
            //       simulation mode, so we are fine...

            if (pReloadingView)
                clearSimulationResults();
            else
                updateSimulationMode();

            // Initialise our contents widget and make sure that we have the
            // required type(s) of solvers

            if (voi) {
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
                                            InvalidSimulationEnvironment);
                        } else {
                            simulationError(tr("the model needs both an ODE and an NLA solver, but no NLA solver is available"),
                                            InvalidSimulationEnvironment);
                        }
                    } else if (solversWidget->odeSolvers().isEmpty()) {
                        simulationError(tr("the model needs both an ODE and an NLA solver, but no ODE solver is available"),
                                        InvalidSimulationEnvironment);
                    } else {
                        mValidSimulationEnvironment = true;
                    }
                } else if (solversWidget->odeSolvers().isEmpty()) {
                    simulationError(tr("the model needs an ODE solver, but none is available"),
                                    InvalidSimulationEnvironment);
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

            mContentsWidget->informationWidget()->parametersWidget()->initialize(mSimulation, pReloadingView);
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

            if (mValidSimulationEnvironment)
                initializeSimulation();
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

    if (mSimulation->fileType() != SimulationSupport::Simulation::CellmlFile)
        initializeTrackers();

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

    return style()->pixelMetric(QStyle::PM_TabBarIconSize, nullptr, this);
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

        tabBarPixmapPainter.setBrush(Core::windowColor());
        tabBarPixmapPainter.setPen(Core::borderColor());

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
    // Save to the given CellML file, SED-ML file our COMBINE archive, depending
    // on the file type of our simulation

    switch (mSimulation->fileType()) {
    case SimulationSupport::Simulation::CellmlFile: {
        // We are dealing with a CellML file, so retrieve all the state and
        // constant parameters which value has changed and update our CellML
        // object with their 'new' values, unless they are imported, in which
        // case we let the user know that their 'new' values cannot be saved

        QString importedParameters = QString();
        ObjRef<iface::cellml_api::CellMLComponentSet> components = mSimulation->cellmlFile()->model()->localComponents();
        QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> parameters = mContentsWidget->informationWidget()->parametersWidget()->parameters();

        for (auto property : parameters.keys()) {
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

        // Now, we can effectively save (update) the CellML file to the given
        // file and let the user know if some parameter values couldn't be saved

        if (mSimulation->cellmlFile()->update(pFileName)) {
            if (!importedParameters.isEmpty()) {
                Core::informationMessageBox(tr("Save File"),
                                            tr("The following parameters are imported and cannot therefore be saved:")+importedParameters);
            }

            return true;
        } else {
            return false;
        }
    }
    case SimulationSupport::Simulation::SedmlFile:
        // Only export our simulation to a SED-ML file if we are not dealing
        // with a new SED-ML file
        // Note: indeed, if we were to do that, we would end up with an
        //       unloadable SED-ML file (since it doesn’t contain everything it
        //       should). So, instead, we should just save our default SED-ML
        //       template, i.e. as if we were to save the SED-ML file using the
        //       Raw SED-ML or Raw Text view...

        if (Core::FileManager::instance()->isNew(mSimulation->fileName()))
            QFile::copy(mSimulation->fileName(), pFileName);
        else
            sedmlExportSedmlFile(pFileName);

        return true;
    case SimulationSupport::Simulation::CombineArchive:
        sedmlExportCombineArchive(pFileName);

        return true;
    }

    return false;
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::filePermissionsChanged()
{
    // We have been un/locked, so enable/disable the development mode and keep
    // track of its checked status or recheck it, as necessary

     if (Core::FileManager::instance()->isReadableAndWritable(mSimulation->fileName())) {
         mDevelopmentModeAction->setEnabled(mSimulation->fileType() == SimulationSupport::Simulation::CellmlFile);
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
            if (handlingAction || mSimulation->isRunning())
                return;

            handlingAction = true;
        }

        // Finish any editing of our simulation information, and update our
        // simulation and solvers properties (without resetting our NLA solver)
        // before running/resuming it

        mContentsWidget->informationWidget()->finishEditing(mSimulation->isPaused());

        if (!mSimulation->isPaused()) {
            updateSimulationProperties();
            updateSolversProperties(false);
        }

        // Run or resume our simulation

        if (mSimulation->isPaused()) {
            mSimulation->resume();
        } else {
            // Try to allocate all the memory we need by adding a run to our
            // simulation

            bool runSimulation = mSimulation->addRun();

            // Run our simulation (after having added a run to our graphs), in
            // case we were able to allocate all the memory we need

            if (runSimulation) {
                mViewWidget->checkSimulationResults(mSimulation->fileName(), AddRun);

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
    setUpdatesEnabled(false);
        // Clear our simulation results
        // Note: we temporarily disable updates to prevent the GUI from taking
        //       too long to update itself (something that might happen when we
        //       have several graph panels since they will try to realign
        //       themselves)...

        mSimulation->results()->reset();

        // Update our simulation mode and check for results

        updateSimulationMode();

        mViewWidget->checkSimulationResults(mSimulation->fileName(), ResetRuns);
    setUpdatesEnabled(true);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::developmentMode()
{
    // The development mode has just been enabled/disabled, so update the
    // modified state of our current file accordingly, if needed

    if (!mDevelopmentModeAction->isChecked())
        Core::FileManager::instance()->setModified(mSimulation->fileName(), false);

    // Make sure that our reset button is properly enabled/disabled
    // Note: this is needed if the development mode has just been disabled...

    simulationDataModified(mSimulation->data()->isModified());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addGraphPanel()
{
    // Ask our graph panels widget to add a new graph panel

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
    //       might happen if we reload our SED-ML file / COMBINE archive for
    //       example)...

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
    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = informationWidget->graphPanelAndGraphsWidget();

    for (int i = 0, iMax = graphPanelsWidget->graphPanels().count(); i < iMax; ++i) {
        GraphPanelWidget::GraphPanelWidget *graphPanel = graphPanelsWidget->graphPanels()[i];
        Core::PropertyEditorWidget *graphPanelPropertyEditor = graphPanelAndGraphsWidget->graphPanelPropertyEditor(graphPanel);
        Core::PropertyEditorWidget *graphsPropertyEditor = graphPanelAndGraphsWidget->graphsPropertyEditor(graphPanel);

        mGraphPanelProperties.insert(graphPanelPropertyEditor, allPropertyValues(graphPanelPropertyEditor));
        mGraphsProperties.insert(graphsPropertyEditor, allPropertyValues(graphsPropertyEditor));

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

    if (!pFileName.isEmpty())
        return pFileName;

    QString fileName = pBaseFileName;
    QString baseFileCompleteSuffix = QFileInfo(pBaseFileName).completeSuffix();

    if (baseFileCompleteSuffix.isEmpty())
        fileName += "."+pFileExtension;
    else
        fileName.replace(QRegularExpression(QRegularExpression::escape(baseFileCompleteSuffix)+"$"), pFileExtension);

    QString firstFileFilter = pFileFilters.first();

    return Core::getSaveFileName(pCaption, fileName, pFileFilters, &firstFileFilter);
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
    // Note: the algorithm parameters require the use of KiSAO ids, so if none
    //       exists for an algorithm parameter then we set the algorithm
    //       parameter using an annotation...

    libsedml::SedAlgorithm *sedmlAlgorithm = pSedmlSimulation->createAlgorithm();
    SolverInterface *odeSolverInterface = mSimulation->data()->odeSolverInterface();
    Solver::Solver::Properties odeSolverProperties = mSimulation->data()->odeSolverProperties();
    QString annotation = QString();

    if (odeSolverInterface) {
        sedmlAlgorithm->setKisaoID(odeSolverInterface->kisaoId(mSimulation->data()->odeSolverName()).toStdString());

        for (const auto &odeSolverProperty : odeSolverProperties.keys()) {
            QString kisaoId = odeSolverInterface->kisaoId(odeSolverProperty);
            QVariant odeSolverPropertyValue = odeSolverProperties.value(odeSolverProperty);
            QString value = (odeSolverPropertyValue.type() == QVariant::Double)?
                                QString::number(odeSolverPropertyValue.toDouble(), 'g', 15):
                                odeSolverPropertyValue.toString();

            if (kisaoId.isEmpty()) {
                annotation += QString("<%1 %2=\"%3\" %4=\"%5\"/>").arg(SEDMLSupport::SolverProperty)
                                                                  .arg(SEDMLSupport::Id)
                                                                  .arg(odeSolverProperty)
                                                                  .arg(SEDMLSupport::Value)
                                                                  .arg(value);
            } else {
                libsedml::SedAlgorithmParameter *sedmlAlgorithmParameter = sedmlAlgorithm->createAlgorithmParameter();

                sedmlAlgorithmParameter->setKisaoID(kisaoId.toStdString());
                sedmlAlgorithmParameter->setValue(value.toStdString());
            }
        }
    }

    if (!annotation.isEmpty()) {
        sedmlAlgorithm->appendAnnotation(QString("<%1 xmlns=\"%2\">"
                                                 "    %3"
                                                 "</%1>").arg(SEDMLSupport::SolverProperties)
                                                         .arg(SEDMLSupport::OpencorNamespace)
                                                         .arg(annotation).toStdString());
    }

    // Check whether the simulation required an NLA solver and, if so, let our
    // SED-ML simulation know about it through an annotation (since we cannot
    // have more than one SED-ML algorithm per SED-ML simulation)

    CellMLSupport::CellmlFileRuntime *runtime = mSimulation->runtime();

    if (runtime && runtime->needNlaSolver()) {
        QString annotation = QString();
        Solver::Solver::Properties nlaSolverProperties = mSimulation->data()->nlaSolverProperties();

        for (const auto &nlaSolverProperty : nlaSolverProperties.keys()) {
            annotation += QString("<%1 %2=\"%3\" %4=\"%5\"/>").arg(SEDMLSupport::SolverProperty)
                                                              .arg(SEDMLSupport::Id)
                                                              .arg(nlaSolverProperty)
                                                              .arg(SEDMLSupport::Value)
                                                              .arg(nlaSolverProperties.value(nlaSolverProperty).toString());
        }

        pSedmlSimulation->appendAnnotation(QString("<%1 xmlns=\"%2\" %3=\"%4\">"
                                                   "    %5"
                                                   "</%1>").arg(SEDMLSupport::NlaSolver)
                                                           .arg(SEDMLSupport::OpencorNamespace)
                                                           .arg(SEDMLSupport::Name)
                                                           .arg(mSimulation->data()->nlaSolverName())
                                                           .arg(annotation).toStdString());
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

    pSedmlVariable->setTarget(Target.arg(pComponent)
                                    .arg(variable).toStdString());

    if (variableDegree) {
        pSedmlVariable->appendAnnotation(QString("<%1 xmlns=\"%2\">"
                                                 "    %3"
                                                 "</%1>").arg(SEDMLSupport::VariableDegree)
                                                         .arg(SEDMLSupport::OpencorNamespace)
                                                         .arg(variableDegree).toStdString());
    }
}

//==============================================================================

static const auto TrueValue  = QStringLiteral("true");
static const auto FalseValue = QStringLiteral("false");

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
    CellMLSupport::CellmlFile::Version cellmlVersion = CellMLSupport::CellmlFile::modelVersion(mSimulation->cellmlFile()?
                                                                                                   mSimulation->cellmlFile()->model():
                                                                                                   nullptr);

    namespaces->add((cellmlVersion == CellMLSupport::CellmlFile::Cellml_1_0)?
                        CellMLSupport::Cellml_1_0_Namespace.toStdString():
                        CellMLSupport::Cellml_1_1_Namespace.toStdString(),
                    "cellml");

    // Create and customise a model

    libsedml::SedModel *sedmlModel = sedmlDocument->createModel();

    sedmlModel->setId("model");
    sedmlModel->setLanguage((cellmlVersion == CellMLSupport::CellmlFile::Cellml_1_0)?
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
    quint64 nbOfPoints = quint64(ceil((endingPoint-startingPoint)/pointInterval));
    bool needOneStepTask = !qFuzzyCompare((endingPoint-startingPoint)/nbOfPoints, pointInterval);

    libsedml::SedUniformTimeCourse *sedmlUniformTimeCourse = sedmlDocument->createUniformTimeCourse();

    ++simulationNumber;

    if (needOneStepTask)
        --nbOfPoints;

    sedmlUniformTimeCourse->setId(QString("simulation%1").arg(simulationNumber).toStdString());
    sedmlUniformTimeCourse->setInitialTime(startingPoint);
    sedmlUniformTimeCourse->setOutputStartTime(startingPoint);
    sedmlUniformTimeCourse->setOutputEndTime(startingPoint+nbOfPoints*pointInterval);
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

    typedef struct {
        libsedml::SedPlot2D *sedmlPlot2d;
        int graphPlotCounter;
        bool logAxisX;
        bool logAxisY;
        char padding[2];   // Just for alignment
    } GraphsData;

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = mContentsWidget->informationWidget()->graphPanelAndGraphsWidget();
    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();
    QIntList graphPanelsWidgetSizes = graphPanelsWidget->sizes();
    QList<Core::Properties> graphsPropertiesList = QList<Core::Properties>();
    QMap<Core::Properties, GraphsData> graphsData = QMap<Core::Properties, GraphsData>();
    int graphPlotCounter = 0;

    for (auto graphPanel : graphPanelsWidget->graphPanels()) {
        // Create and customise the look and feel of our 2D plot

        Core::Properties graphPanelProperties = graphPanelAndGraphsWidget->graphPanelPropertyEditor(graphPanel)->properties();
        libsedml::SedPlot2D *sedmlPlot2d = sedmlDocument->createPlot2D();

        sedmlPlot2d->setId(QString("plot%1").arg(++graphPlotCounter).toStdString());

        QString annotation =  SedmlProperty.arg(SEDMLSupport::BackgroundColor)
                                           .arg(graphPanelProperties[0]->stringValue())
                             +SedmlProperty.arg(SEDMLSupport::FontSize)
                                           .arg(graphPanelProperties[1]->stringValue())
                             +SedmlProperty.arg(SEDMLSupport::ForegroundColor)
                                           .arg(graphPanelProperties[2]->stringValue())
                             +SedmlProperty.arg(SEDMLSupport::Height)
                                           .arg(graphPanelsWidgetSizes[graphPlotCounter-1]);

        // Grid lines

        Core::Properties gridLinesProperties = graphPanelProperties[3]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::GridLines)
                                   .arg( SedmlProperty.arg(SEDMLSupport::Style)
                                                      .arg(SEDMLSupport::stringLineStyle(gridLinesProperties[0]->listValueIndex()))
                                        +SedmlProperty.arg(SEDMLSupport::Width)
                                                      .arg(gridLinesProperties[1]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Color)
                                                      .arg(gridLinesProperties[2]->stringValue()));

        // Legend

        annotation += SedmlProperty.arg(SEDMLSupport::Legend)
                                   .arg(graphPanelProperties[4]->stringValue());

        // Point coordinates

        Core::Properties pointCoordinatesProperties = graphPanelProperties[5]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::PointCoordinates)
                                   .arg( SedmlProperty.arg(SEDMLSupport::Style)
                                                      .arg(SEDMLSupport::stringLineStyle(pointCoordinatesProperties[0]->listValueIndex()))
                                        +SedmlProperty.arg(SEDMLSupport::Width)
                                                      .arg(pointCoordinatesProperties[1]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Color)
                                                      .arg(pointCoordinatesProperties[2]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::FontColor)
                                                      .arg(pointCoordinatesProperties[3]->stringValue()));

        // Surrounding area

        Core::Properties surroundingAreaProperties = graphPanelProperties[6]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::SurroundingArea)
                                   .arg( SedmlProperty.arg(SEDMLSupport::BackgroundColor)
                                                      .arg(surroundingAreaProperties[0]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::ForegroundColor)
                                                      .arg(surroundingAreaProperties[1]->stringValue()));

        // Title

        annotation += SedmlProperty.arg(SEDMLSupport::Title)
                                   .arg(graphPanelProperties[7]->stringValue());

        // X axis

        Core::Properties xAxisProperties = graphPanelProperties[8]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::XAxis)
                                   .arg( SedmlProperty.arg(SEDMLSupport::LogarithmicScale)
                                                      .arg(xAxisProperties[0]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Title)
                                                      .arg(xAxisProperties[1]->stringValue()));

        // Y axis

        Core::Properties yAxisProperties = graphPanelProperties[9]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::YAxis)
                                   .arg( SedmlProperty.arg(SEDMLSupport::LogarithmicScale)
                                                      .arg(yAxisProperties[0]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Title)
                                                      .arg(yAxisProperties[1]->stringValue()));

        // Zoom region

        Core::Properties zoomRegionProperties = graphPanelProperties[10]->properties();

        annotation += SedmlProperty.arg(SEDMLSupport::ZoomRegion)
                                   .arg( SedmlProperty.arg(SEDMLSupport::Style)
                                                      .arg(SEDMLSupport::stringLineStyle(zoomRegionProperties[0]->listValueIndex()))
                                        +SedmlProperty.arg(SEDMLSupport::Width)
                                                      .arg(zoomRegionProperties[1]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Color)
                                                      .arg(zoomRegionProperties[2]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::FontColor)
                                                      .arg(zoomRegionProperties[3]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::Filled)
                                                      .arg(zoomRegionProperties[4]->stringValue())
                                        +SedmlProperty.arg(SEDMLSupport::FillColor)
                                                      .arg(zoomRegionProperties[5]->stringValue()));;

        // Add our properties as an annotation

        sedmlPlot2d->appendAnnotation(QString("<%1 xmlns=\"%2\">"
                                              "    %3"
                                              "</%1>").arg(SEDMLSupport::Properties)
                                                      .arg(SEDMLSupport::OpencorNamespace)
                                                      .arg(annotation).toStdString());

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

    for (const auto &graphsProperties : graphsPropertiesList) {
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

            sedmlCurve->appendAnnotation(QString("<%1 xmlns=\"%2\">"
                                                 "    %3"
                                                 "</%1>").arg(SEDMLSupport::Properties)
                                                         .arg(SEDMLSupport::OpencorNamespace)
                                                         .arg( SedmlProperty.arg(SEDMLSupport::Selected)
                                                                            .arg(property->isChecked()?
                                                                                     TrueValue:
                                                                                     FalseValue)
                                                              +SedmlProperty.arg(SEDMLSupport::Title)
                                                                            .arg(properties[1]->stringValue())
                                                              +SedmlProperty.arg(SEDMLSupport::Line)
                                                                            .arg( SedmlProperty.arg(SEDMLSupport::Style)
                                                                                               .arg(SEDMLSupport::stringLineStyle(lineProperties[0]->listValueIndex()))
                                                                                 +SedmlProperty.arg(SEDMLSupport::Width)
                                                                                               .arg(lineProperties[1]->stringValue())
                                                                                 +SedmlProperty.arg(SEDMLSupport::Color)
                                                                                               .arg(lineProperties[2]->stringValue()))
                                                              +SedmlProperty.arg(SEDMLSupport::Symbol)
                                                                            .arg( SedmlProperty.arg(SEDMLSupport::Style)
                                                                                               .arg(SEDMLSupport::stringSymbolStyle(symbolProperties[0]->listValueIndex()))
                                                                                 +SedmlProperty.arg(SEDMLSupport::Size)
                                                                                               .arg(symbolProperties[1]->stringValue())
                                                                                 +SedmlProperty.arg(SEDMLSupport::Color)
                                                                                               .arg(symbolProperties[2]->stringValue())
                                                                                 +SedmlProperty.arg(SEDMLSupport::Filled)
                                                                                               .arg(symbolProperties[3]->stringValue())
                                                                                 +SedmlProperty.arg(SEDMLSupport::FillColor)
                                                                                               .arg(symbolProperties[4]->stringValue()))).toStdString());
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
    QString localCellmlFileName = mSimulation->cellmlFile()?
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
                                     sedmlFileTypeInterface?
                                         Core::filters(FileTypeInterfaces() << sedmlFileTypeInterface):
                                         QStringList());

    // Create a SED-ML file using the SED-ML file name that has been provided,
    // if any

    if (!sedmlFileName.isEmpty()) {
        QString modelSource = cellmlFileName;

        if (   !remoteCellmlFile
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

        // Retrieve our SED-ML file or create a temporary one, if needed

        bool isCellmlFile = mSimulation->fileType() == SimulationSupport::Simulation::CellmlFile;
        SEDMLSupport::SedmlFile *sedmlFile = isCellmlFile?
                                                 new SEDMLSupport::SedmlFile(sedmlFileName, true):
                                                 mSimulation->sedmlFile();

        if (!createSedmlFile(sedmlFile, sedmlFileName, modelSource)) {
            Core::warningMessageBox(tr("Export To SED-ML File"),
                                    tr("The simulation could not be exported to <strong>%1</strong>.").arg(QDir::toNativeSeparators(sedmlFileName)));
        }

        if (isCellmlFile)
            delete sedmlFile;

        // Reinitialise our trackers, if we are not dealing with a CellML file

        if (!isCellmlFile)
            initializeTrackers(true);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::sedmlExportSedmlFile()
{
    // Export the simulation to a SED-ML file

    sedmlExportSedmlFile(QString());
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
                                          combineFileTypeInterface?
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
        QString localCellmlFileName = cellmlFile->fileName();
        bool remoteCellmlFile = fileManagerInstance->isRemote(localCellmlFileName);
        QString cellmlFileName = remoteCellmlFile?
                                     fileManagerInstance->url(localCellmlFileName):
                                     localCellmlFileName;
        QString commonPath = QString(cellmlFileName).remove(FileNameRegEx)+"/";
        QMap<QString, QString> remoteImportedFileNames = QMap<QString, QString>();

        for (const auto &importedFileName : cellmlFile->importedFileNames()) {
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

        QString modelSource = QString(cellmlFileName).remove(commonPath);

        // Create a copy of the SED-ML file that will be the master file in our
        // COMBINE archive

        QString errorMessage = QString();
        QString sedmlFileName = Core::temporaryFileName();
        SEDMLSupport::SedmlFile sedmlFile(sedmlFileName, true);

        createSedmlFile(&sedmlFile, sedmlFileName, modelSource);

        // Create our COMBINE archive after having added all our files to it

        bool isCellmlOrSedmlFile =    (mSimulation->fileType() == SimulationSupport::Simulation::CellmlFile)
                                   || (mSimulation->fileType() == SimulationSupport::Simulation::SedmlFile);
        COMBINESupport::CombineArchive *combineArchive = isCellmlOrSedmlFile?
                                                             new COMBINESupport::CombineArchive(combineArchiveName, true):
                                                             mSimulation->combineArchive();
        QFileInfo combineArchiveInfo = QFileInfo(combineArchiveName);
        QString sedmlFileLocation = combineArchiveInfo.fileName();

        sedmlFileLocation.replace(QRegularExpression(QRegularExpression::escape(combineArchiveInfo.completeSuffix())+"$"),
                                  SEDMLSupport::SedmlFileExtension);

        combineArchive->forceNew();

        if (combineArchive->addFile(sedmlFileName, sedmlFileLocation,
                                    COMBINESupport::CombineArchiveFile::Sedml, true)) {
            CellMLSupport::CellmlFile::Version cellmlVersion = cellmlFile->version();

            if (combineArchive->addFile(localCellmlFileName, modelSource,
                                        (cellmlVersion == CellMLSupport::CellmlFile::Cellml_1_0)?
                                            COMBINESupport::CombineArchiveFile::Cellml_1_0:
                                            COMBINESupport::CombineArchiveFile::Cellml_1_1)) {
                for (const auto &importedFileName : cellmlFile->importedFileNames()) {
                    QString realImportedFileName = remoteCellmlFile?
                                                       remoteImportedFileNames.value(importedFileName):
                                                       importedFileName;
                    QString relativeImportedFileName = QString(importedFileName).remove(commonPath);

                    if (!combineArchive->addFile(realImportedFileName,
                                                 relativeImportedFileName,
                                                 COMBINESupport::CombineArchiveFile::Cellml)) {
                        errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(combineArchiveName)
                                                                                                           .arg(" ("+tr("<strong>%1</strong> could not be added").arg(relativeImportedFileName)+").");

                        break;
                    }
                }

                if (errorMessage.isEmpty()) {
                    // Now, we can effectively save (update) the COMBINE archive

                    if (!combineArchive->update(combineArchiveName))
                        errorMessage = tr("The simulation could not be exported to <strong>%1</strong>.").arg(combineArchiveName);
                }
            } else {
                errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(combineArchiveName)
                                                                                                   .arg(" ("+tr("<strong>%1</strong> could not be added").arg(modelSource)+").");
            }
        } else {
            errorMessage = tr("The simulation could not be exported to <strong>%1</strong>%2.").arg(combineArchiveName)
                                                                                               .arg(" ("+tr("the master SED-ML file could not be added")+").");
        }

        if (isCellmlOrSedmlFile)
            delete combineArchive;

        // Remove the local copy of our remote imported CellML files, if any

        for (const auto &localImportedFileName : remoteImportedFileNames.values())
            QFile::remove(localImportedFileName);

        // Let the user know about any error that may have occurred

        if (!errorMessage.isEmpty()) {
            Core::warningMessageBox(tr("Export To COMBINE Archive"),
                                    errorMessage);
        }

        // Reinitialise our trackers, if we are neither dealing with a CellML
        // file nor a SED-ML file

        if (!isCellmlOrSedmlFile)
            initializeTrackers(true);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::sedmlExportCombineArchive()
{
    // Export the simulation to a COMBINE archive

    sedmlExportCombineArchive(QString());
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

void SimulationExperimentViewSimulationWidget::updateSolversProperties(Core::Property *pProperty,
                                                                       bool pResetNlaSolver)
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
            for (auto property : solversWidget->odeSolverData()->solversProperties().value(mSimulation->data()->odeSolverName())) {
                if (!pProperty || (pProperty == property)) {
                    mSimulation->data()->addOdeSolverProperty(property->id(), property->variantValue());

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

    // NLA solver properties

    bool needNlaSolverGuiUpdate = false;

    if (solversWidget->nlaSolverData()) {
        if (!pProperty || (pProperty == solversWidget->nlaSolverData()->solversListProperty())) {
            mSimulation->data()->setNlaSolverName(solversWidget->nlaSolverData()->solversListProperty()->value(), pResetNlaSolver);

            needNlaSolverGuiUpdate = true;
        }

        if (!pProperty || !needNlaSolverGuiUpdate) {
            for (auto property : solversWidget->nlaSolverData()->solversProperties().value(mSimulation->data()->nlaSolverName())) {
                if (!pProperty || (pProperty == property)) {
                    mSimulation->data()->addNlaSolverProperty(property->id(), property->variantValue(), pResetNlaSolver);

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
            const libsbml::XMLNode &variableDegreeNode = annotation->getChild(i);

            if (   !QString::fromStdString(variableDegreeNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                && !QString::fromStdString(variableDegreeNode.getName()).compare(SEDMLSupport::VariableDegree)) {
                variableDegree = QString::fromStdString(variableDegreeNode.getChild(0).getCharacters()).toInt();
            }
        }
    }

    // Go through the runtime parameters to see if one of them corresponds to
    // our given SED-ML variable

    pCellmlComponent = componentName;
    pCellmlVariable = variableName+QString(variableDegree, '\'');

    for (auto parameter : mSimulation->runtime()->parameters()) {
        if (   !componentName.compare(parameter->componentHierarchy().last())
            && !variableName.compare(parameter->name())
            &&  (variableDegree == parameter->degree())) {
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

    for (auto parameter : mSimulation->runtime()->dataParameters()) {
        if (   !pComponent.compare(parameter->componentHierarchy().last())
            && !pVariable.compare(parameter->name())) {
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
                                                           SEDMLSupport::lineStyle(PreferencesInterface::preference(PluginName,
                                                                                                                    SettingsPreferencesGraphLineStyle,
                                                                                                                    SEDMLSupport::stringLineStyle(SettingsPreferencesGraphLineStyleDefault)).toString()),
                                                           PreferencesInterface::preference(PluginName,
                                                                                            SettingsPreferencesGraphLineWidth,
                                                                                            SettingsPreferencesGraphLineWidthDefault).toInt(),
                                                           pColor,
                                                           SEDMLSupport::symbolStyle(PreferencesInterface::preference(PluginName,
                                                                                                                      SettingsPreferencesGraphSymbolStyle,
                                                                                                                      SEDMLSupport::stringSymbolStyle(SettingsPreferencesGraphSymbolStyleDefault)).toString()),
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

    return defaultGraphProperties(QString(), pColor);
}

//==============================================================================

bool SimulationExperimentViewSimulationWidget::furtherInitialize()
{
    // Customise our simulation widget

    SimulationExperimentViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
    SimulationExperimentViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();
    SimulationExperimentViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();
    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = informationWidget->graphPanelAndGraphsWidget();

    libsedml::SedDocument *sedmlDocument = mSimulation->sedmlFile()->sedmlDocument();
    libsedml::SedUniformTimeCourse *sedmlUniformTimeCourse = static_cast<libsedml::SedUniformTimeCourse *>(sedmlDocument->getSimulation(0));
    libsedml::SedOneStep *sedmlOneStep = static_cast<libsedml::SedOneStep *>(sedmlDocument->getSimulation(1));

    double startingPoint = sedmlUniformTimeCourse->getOutputStartTime();
    double endingPoint = sedmlUniformTimeCourse->getOutputEndTime();
    double pointInterval = (endingPoint-startingPoint)/sedmlUniformTimeCourse->getNumberOfPoints();

    if (sedmlOneStep)
        endingPoint += sedmlOneStep->getStep();

    simulationWidget->startingPointProperty()->setDoubleValue(startingPoint);
    simulationWidget->endingPointProperty()->setDoubleValue(endingPoint);
    simulationWidget->pointIntervalProperty()->setDoubleValue(pointInterval);

    // Customise our solvers widget by:
    //  - Specifying the ODE solver to use
    //  - Customising the solver's properties for which we have a KiSAO id
    //  - Customising the solver's properties for which we don't have a KiSAO id
    //    (this shouldn't happen, but better be safe than sorry)
    //  - Specifying the NLA solver, if any

    SimulationExperimentViewInformationSolversWidgetData *odeSolverData = solversWidget->odeSolverData();
    const libsedml::SedAlgorithm *sedmlAlgorithm = sedmlUniformTimeCourse->getAlgorithm();
    SolverInterface *odeSolverInterface = nullptr;
    SolverInterfaces solverInterfaces = Core::solverInterfaces();
    Core::Properties solverProperties = Core::Properties();
    QString kisaoId = QString::fromStdString(sedmlAlgorithm->getKisaoID());

    for (auto solverInterface : solverInterfaces) {
        if (!solverInterface->id(kisaoId).compare(solverInterface->solverName())) {
            odeSolverInterface = solverInterface;
            solverProperties = odeSolverData->solversProperties().value(solverInterface->solverName());

            odeSolverData->solversListProperty()->setValue(solverInterface->solverName());

            break;
        }
    }

    if (!odeSolverInterface) {
        simulationError(tr("the requested solver (%1) could not be found").arg(kisaoId),
                        InvalidSimulationEnvironment);

        return false;
    }

    for (uint i = 0, iMax = sedmlAlgorithm->getNumAlgorithmParameters(); i < iMax; ++i) {
        const libsedml::SedAlgorithmParameter *sedmlAlgorithmParameter = sedmlAlgorithm->getAlgorithmParameter(i);
        QString kisaoId = QString::fromStdString(sedmlAlgorithmParameter->getKisaoID());
        QString id = odeSolverInterface->id(kisaoId);
        bool propertySet = false;

        for (auto solverProperty : solverProperties) {
            if (!solverProperty->id().compare(id)) {
                QVariant solverPropertyValue = QString::fromStdString(sedmlAlgorithmParameter->getValue());

                switch (solverProperty->type()) {
                case Core::Property::Section:
                    // We should never come here...

#ifdef QT_DEBUG
                    qFatal("FATAL ERROR | %s:%d: a solver property cannot be of section type.", __FILE__, __LINE__);
#else
                    break;
#endif
                case Core::Property::String:
                    solverProperty->setValue(solverPropertyValue.toString());

                    break;
                case Core::Property::Integer:
                case Core::Property::IntegerGe0:
                case Core::Property::IntegerGt0:
                    solverProperty->setIntegerValue(solverPropertyValue.toInt());

                    break;
                case Core::Property::Double:
                case Core::Property::DoubleGe0:
                case Core::Property::DoubleGt0:
                    solverProperty->setDoubleValue(solverPropertyValue.toDouble());

                    break;
                case Core::Property::List:
                    solverProperty->setListValue(solverPropertyValue.toString());

                    break;
                case Core::Property::Boolean:
                    solverProperty->setBooleanValue(solverPropertyValue.toBool());

                    break;
                case Core::Property::Color:
                    // We should never come here...

#ifdef QT_DEBUG
                    qFatal("FATAL ERROR | %s:%d: a solver property cannot be of colour type.", __FILE__, __LINE__);
#else
                    break;
#endif
                }

                propertySet = solverProperty->type() != Core::Property::Section;

                break;
            }
        }

        if (!propertySet) {
            simulationError(tr("the requested solver property (%1) could not be set").arg(kisaoId),
                            InvalidSimulationEnvironment);

            return false;
        }
    }

    libsbml::XMLNode *annotation = sedmlAlgorithm->getAnnotation();

    if (annotation) {
        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const libsbml::XMLNode &solverPropertiesNode = annotation->getChild(i);

            if (   !QString::fromStdString(solverPropertiesNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                && !QString::fromStdString(solverPropertiesNode.getName()).compare(SEDMLSupport::SolverProperties)) {
                for (uint j = 0, jMax = solverPropertiesNode.getNumChildren(); j < jMax; ++j) {
                    const libsbml::XMLNode &solverPropertyNode = solverPropertiesNode.getChild(j);

                    if (   !QString::fromStdString(solverPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                        && !QString::fromStdString(solverPropertyNode.getName()).compare(SEDMLSupport::SolverProperty)) {
                        QString id = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Id.toStdString())));
                        QString value = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Value.toStdString())));
                        bool propertySet = false;

                        for (auto solverProperty : solverProperties) {
                            if (!solverProperty->id().compare(id)) {
                                solverProperty->setValue(value);

                                propertySet = true;

                                break;
                            }
                        }

                        if (!propertySet) {
                            simulationError(tr("the requested solver property (%1) could not be set").arg(id),
                                            InvalidSimulationEnvironment);

                            return false;
                        }
                    }
                }
            }
        }
    }

    annotation = sedmlUniformTimeCourse->getAnnotation();

    if (annotation) {
        bool mustHaveNlaSolver = false;
        bool hasNlaSolver = false;
        QString nlaSolverName = QString();

        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const libsbml::XMLNode &nlaSolverNode = annotation->getChild(i);

            if (   !QString::fromStdString(nlaSolverNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                && !QString::fromStdString(nlaSolverNode.getName()).compare(SEDMLSupport::NlaSolver)) {
                SimulationExperimentViewInformationSolversWidgetData *nlaSolverData = solversWidget->nlaSolverData();
                Core::Properties solverProperties = Core::Properties();

                mustHaveNlaSolver = true;
                nlaSolverName = QString::fromStdString(nlaSolverNode.getAttrValue(nlaSolverNode.getAttrIndex(SEDMLSupport::Name.toStdString())));

                for (auto solverInterface : solverInterfaces) {
                    if (!nlaSolverName.compare(solverInterface->solverName())) {
                        solverProperties = nlaSolverData->solversProperties().value(solverInterface->solverName());

                        nlaSolverData->solversListProperty()->setValue(nlaSolverName);

                        hasNlaSolver = true;

                        break;
                    }
                }

                if (hasNlaSolver) {
                    for (uint j = 0, jMax = nlaSolverNode.getNumChildren(); j < jMax; ++j) {
                        const libsbml::XMLNode &solverPropertyNode = nlaSolverNode.getChild(j);

                        if (   !QString::fromStdString(solverPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                            && !QString::fromStdString(solverPropertyNode.getName()).compare(SEDMLSupport::SolverProperty)) {
                            QString id = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Id.toStdString())));
                            QString value = QString::fromStdString(solverPropertyNode.getAttrValue(solverPropertyNode.getAttrIndex(SEDMLSupport::Value.toStdString())));
                            bool propertySet = false;

                            for (auto solverProperty : solverProperties) {
                                if (!solverProperty->id().compare(id)) {
                                    solverProperty->setValue(value);

                                    propertySet = true;

                                    break;
                                }
                            }

                            if (!propertySet) {
                                simulationError(tr("the requested solver property (%1) could not be set").arg(id),
                                                InvalidSimulationEnvironment);

                                return false;
                            }
                        }
                    }

                    break;
                }
            }
        }

        if (mustHaveNlaSolver && !hasNlaSolver) {
            simulationError(tr("the requested NLA solver (%1) could not be set").arg(nlaSolverName),
                            InvalidSimulationEnvironment);

            return false;
        }
    }

    // Add/remove some graph panels, so that our final number of graph panels
    // corresponds to the number of 2D outputs mentioned in the SED-ML file
    // Note: no need to pass defaultGraphPanelProperties() to our
    //       removeCurrentGraphPanel() and addGraphPanel() methods since all the
    //       graph panels get fully customised afterwards...

    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();
    int oldNbOfGraphPanels = graphPanelsWidget->graphPanels().count();
    int newNbOfGraphPanels = int(sedmlDocument->getNumOutputs());

    if (oldNbOfGraphPanels < newNbOfGraphPanels) {
        for (int i = 0, iMax = newNbOfGraphPanels-oldNbOfGraphPanels; i < iMax; ++i)
            graphPanelsWidget->addGraphPanel();
    } else if (oldNbOfGraphPanels > newNbOfGraphPanels) {
        for (int i = 0, iMax = oldNbOfGraphPanels-newNbOfGraphPanels; i < iMax; ++i)
            graphPanelsWidget->removeCurrentGraphPanel();
    }

    // Customise our graph panel and graphs

    QIntList graphPanelsWidgetSizes = QIntList();

    for (int i = 0; i < newNbOfGraphPanels; ++i) {
        // Customise our graph panel

        libsedml::SedPlot2D *sedmlPlot2d = static_cast<libsedml::SedPlot2D *>(sedmlDocument->getOutput(uint(i)));
        GraphPanelWidget::GraphPanelWidget *graphPanel = graphPanelsWidget->graphPanels()[i];

        graphPanelAndGraphsWidget->reinitialize(graphPanel);

        annotation = sedmlPlot2d->getAnnotation();

        if (annotation) {
            Core::Properties graphPanelProperties = graphPanelAndGraphsWidget->graphPanelPropertyEditor(graphPanel)->properties();

            for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
                const libsbml::XMLNode &sedmlPlot2dPropertiesNode = annotation->getChild(i);

                if (   !QString::fromStdString(sedmlPlot2dPropertiesNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                    && !QString::fromStdString(sedmlPlot2dPropertiesNode.getName()).compare(SEDMLSupport::Properties)) {
                    for (uint j = 0, jMax = sedmlPlot2dPropertiesNode.getNumChildren(); j < jMax; ++j) {
                        const libsbml::XMLNode &sedmlPlot2dPropertyNode = sedmlPlot2dPropertiesNode.getChild(j);
                        QString sedmlPlot2dPropertyNodeName = QString::fromStdString(sedmlPlot2dPropertyNode.getName());
                        QString sedmlPlot2dPropertyNodeValue = QString::fromStdString(sedmlPlot2dPropertyNode.getChild(0).getCharacters());

                        if (!sedmlPlot2dPropertyNodeName.compare(SEDMLSupport::BackgroundColor)) {
                            graphPanelProperties[0]->setValue(sedmlPlot2dPropertyNodeValue);
                        } else if (!sedmlPlot2dPropertyNodeName.compare(SEDMLSupport::FontSize)) {
                            graphPanelProperties[1]->setValue(sedmlPlot2dPropertyNodeValue);
                        } else if (!sedmlPlot2dPropertyNodeName.compare(SEDMLSupport::ForegroundColor)) {
                            graphPanelProperties[2]->setValue(sedmlPlot2dPropertyNodeValue);
                        } else if (!sedmlPlot2dPropertyNodeName.compare(SEDMLSupport::Height)) {
                            graphPanelsWidgetSizes << sedmlPlot2dPropertyNodeValue.toInt();

                        // Grid lines

                        } else if (   !QString::fromStdString(sedmlPlot2dPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                                   && !QString::fromStdString(sedmlPlot2dPropertyNode.getName()).compare(SEDMLSupport::GridLines)) {
                            Core::Properties gridLinesProperties = graphPanelProperties[3]->properties();

                            for (uint k = 0, kMax = sedmlPlot2dPropertyNode.getNumChildren(); k < kMax; ++k) {
                                const libsbml::XMLNode &gridLinesPropertyNode = sedmlPlot2dPropertyNode.getChild(k);
                                QString gridLinesPropertyNodeName = QString::fromStdString(gridLinesPropertyNode.getName());
                                QString gridLinesPropertyNodeValue = QString::fromStdString(gridLinesPropertyNode.getChild(0).getCharacters());

                                if (!gridLinesPropertyNodeName.compare(SEDMLSupport::Style)) {
                                    gridLinesProperties[0]->setValue(gridLinesPropertyNodeValue);
                                } else if (!gridLinesPropertyNodeName.compare(SEDMLSupport::Width)) {
                                    gridLinesProperties[1]->setValue(gridLinesPropertyNodeValue);
                                } else if (!gridLinesPropertyNodeName.compare(SEDMLSupport::Color)) {
                                    gridLinesProperties[2]->setValue(gridLinesPropertyNodeValue);
                                }
                            }

                        // Legend

                        } else if (!sedmlPlot2dPropertyNodeName.compare(SEDMLSupport::Legend)) {
                            graphPanelProperties[4]->setBooleanValue(!sedmlPlot2dPropertyNodeValue.compare(TrueValue));

                        // Point coordinates

                        } else if (   !QString::fromStdString(sedmlPlot2dPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                                   && !QString::fromStdString(sedmlPlot2dPropertyNode.getName()).compare(SEDMLSupport::PointCoordinates)) {
                            Core::Properties pointCoordinatesProperties = graphPanelProperties[5]->properties();

                            for (uint k = 0, kMax = sedmlPlot2dPropertyNode.getNumChildren(); k < kMax; ++k) {
                                const libsbml::XMLNode &pointCoordinatesPropertyNode = sedmlPlot2dPropertyNode.getChild(k);
                                QString pointCoordinatesPropertyNodeName = QString::fromStdString(pointCoordinatesPropertyNode.getName());
                                QString pointCoordinatesPropertyNodeValue = QString::fromStdString(pointCoordinatesPropertyNode.getChild(0).getCharacters());

                                if (!pointCoordinatesPropertyNodeName.compare(SEDMLSupport::Style)) {
                                    pointCoordinatesProperties[0]->setValue(pointCoordinatesPropertyNodeValue);
                                } else if (!pointCoordinatesPropertyNodeName.compare(SEDMLSupport::Width)) {
                                    pointCoordinatesProperties[1]->setValue(pointCoordinatesPropertyNodeValue);
                                } else if (!pointCoordinatesPropertyNodeName.compare(SEDMLSupport::Color)) {
                                    pointCoordinatesProperties[2]->setValue(pointCoordinatesPropertyNodeValue);
                                } else if (!pointCoordinatesPropertyNodeName.compare(SEDMLSupport::FontColor)) {
                                    pointCoordinatesProperties[3]->setValue(pointCoordinatesPropertyNodeValue);
                                }
                            }

                        // Surrounding area

                        } else if (   !QString::fromStdString(sedmlPlot2dPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                                   && !QString::fromStdString(sedmlPlot2dPropertyNode.getName()).compare(SEDMLSupport::SurroundingArea)) {
                            Core::Properties surroundingAreaProperties = graphPanelProperties[6]->properties();

                            for (uint k = 0, kMax = sedmlPlot2dPropertyNode.getNumChildren(); k < kMax; ++k) {
                                const libsbml::XMLNode &surroundingAreaPropertyNode = sedmlPlot2dPropertyNode.getChild(k);
                                QString surroundingAreaPropertyNodeName = QString::fromStdString(surroundingAreaPropertyNode.getName());
                                QString surroundingAreaPropertyNodeValue = QString::fromStdString(surroundingAreaPropertyNode.getChild(0).getCharacters());

                                if (!surroundingAreaPropertyNodeName.compare(SEDMLSupport::BackgroundColor)) {
                                    surroundingAreaProperties[0]->setValue(surroundingAreaPropertyNodeValue);
                                } else if (!surroundingAreaPropertyNodeName.compare(SEDMLSupport::ForegroundColor)) {
                                    surroundingAreaProperties[1]->setValue(surroundingAreaPropertyNodeValue);
                                }
                            }

                        // Title

                        } else if (!sedmlPlot2dPropertyNodeName.compare(SEDMLSupport::Title)) {
                            graphPanelProperties[7]->setValue(sedmlPlot2dPropertyNodeValue);

                        // X axis

                        } else if (   !QString::fromStdString(sedmlPlot2dPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                                   && !QString::fromStdString(sedmlPlot2dPropertyNode.getName()).compare(SEDMLSupport::XAxis)) {
                            Core::Properties xAxisProperties = graphPanelProperties[8]->properties();

                            for (uint k = 0, kMax = sedmlPlot2dPropertyNode.getNumChildren(); k < kMax; ++k) {
                                const libsbml::XMLNode &xAxisPropertyNode = sedmlPlot2dPropertyNode.getChild(k);
                                QString xAxisPropertyNodeName = QString::fromStdString(xAxisPropertyNode.getName());
                                QString xAxisPropertyNodeValue = QString::fromStdString(xAxisPropertyNode.getChild(0).getCharacters());

                                if (!xAxisPropertyNodeName.compare(SEDMLSupport::LogarithmicScale)) {
                                    xAxisProperties[0]->setBooleanValue(!xAxisPropertyNodeValue.compare(TrueValue));
                                } else if (!xAxisPropertyNodeName.compare(SEDMLSupport::Title)) {
                                    xAxisProperties[1]->setValue(xAxisPropertyNodeValue);
                                }
                            }

                        // Y axis

                        } else if (   !QString::fromStdString(sedmlPlot2dPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                                   && !QString::fromStdString(sedmlPlot2dPropertyNode.getName()).compare(SEDMLSupport::YAxis)) {
                            Core::Properties yAxisProperties = graphPanelProperties[9]->properties();

                            for (uint k = 0, kMax = sedmlPlot2dPropertyNode.getNumChildren(); k < kMax; ++k) {
                                const libsbml::XMLNode &yAxisPropertyNode = sedmlPlot2dPropertyNode.getChild(k);
                                QString yAxisPropertyNodeName = QString::fromStdString(yAxisPropertyNode.getName());
                                QString yAxisPropertyNodeValue = QString::fromStdString(yAxisPropertyNode.getChild(0).getCharacters());

                                if (!yAxisPropertyNodeName.compare(SEDMLSupport::LogarithmicScale)) {
                                    yAxisProperties[0]->setBooleanValue(!yAxisPropertyNodeValue.compare(TrueValue));
                                } else if (!yAxisPropertyNodeName.compare(SEDMLSupport::Title)) {
                                    yAxisProperties[1]->setValue(yAxisPropertyNodeValue);
                                }
                            }

                        // Zoom region

                        } else if (   !QString::fromStdString(sedmlPlot2dPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                                   && !QString::fromStdString(sedmlPlot2dPropertyNode.getName()).compare(SEDMLSupport::ZoomRegion)) {
                            Core::Properties zoomRegionProperties = graphPanelProperties[10]->properties();

                            for (uint k = 0, kMax = sedmlPlot2dPropertyNode.getNumChildren(); k < kMax; ++k) {
                                const libsbml::XMLNode &zoomRegionPropertyNode = sedmlPlot2dPropertyNode.getChild(k);
                                QString zoomRegionPropertyNodeName = QString::fromStdString(zoomRegionPropertyNode.getName());
                                QString zoomRegionPropertyNodeValue = QString::fromStdString(zoomRegionPropertyNode.getChild(0).getCharacters());

                                if (!zoomRegionPropertyNodeName.compare(SEDMLSupport::Style)) {
                                    zoomRegionProperties[0]->setValue(zoomRegionPropertyNodeValue);
                                } else if (!zoomRegionPropertyNodeName.compare(SEDMLSupport::Width)) {
                                    zoomRegionProperties[1]->setValue(zoomRegionPropertyNodeValue);
                                } else if (!zoomRegionPropertyNodeName.compare(SEDMLSupport::Color)) {
                                    zoomRegionProperties[2]->setValue(zoomRegionPropertyNodeValue);
                                } else if (!zoomRegionPropertyNodeName.compare(SEDMLSupport::FontColor)) {
                                    zoomRegionProperties[3]->setValue(zoomRegionPropertyNodeValue);
                                } else if (!zoomRegionPropertyNodeName.compare(SEDMLSupport::Filled)) {
                                    zoomRegionProperties[4]->setBooleanValue(!zoomRegionPropertyNodeValue.compare(TrueValue));
                                } else if (!zoomRegionPropertyNodeName.compare(SEDMLSupport::FillColor)) {
                                    zoomRegionProperties[5]->setValue(zoomRegionPropertyNodeValue);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Customise our graphs

        graphPanel->removeAllGraphs();

        for (uint j = 0, jMax = sedmlPlot2d->getNumCurves(); j < jMax; ++j) {
            libsedml::SedCurve *sedmlCurve = sedmlPlot2d->getCurve(j);

            libsedml::SedVariable *xVariable = sedmlDocument->getDataGenerator(sedmlCurve->getXDataReference())->getVariable(0);
            libsedml::SedVariable *yVariable = sedmlDocument->getDataGenerator(sedmlCurve->getYDataReference())->getVariable(0);
            QString xCellmlComponent;
            QString yCellmlComponent;
            QString xCellmlVariable;
            QString yCellmlVariable;
            CellMLSupport::CellmlFileRuntimeParameter *xParameter = runtimeParameter(xVariable, xCellmlComponent, xCellmlVariable);
            CellMLSupport::CellmlFileRuntimeParameter *yParameter = runtimeParameter(yVariable, yCellmlComponent, yCellmlVariable);

            if (!xParameter) {
                if (!yParameter) {
                    simulationError(tr("the requested curve (%1) could not be set (the variable %2 in component %3 and the variable %4 in component %5 could not be found)").arg(QString::fromStdString(sedmlCurve->getId()))
                                                                                                                                                                            .arg(xCellmlVariable)
                                                                                                                                                                            .arg(xCellmlComponent)
                                                                                                                                                                            .arg(yCellmlVariable)
                                                                                                                                                                            .arg(yCellmlComponent),
                                    InvalidSimulationEnvironment);

                    return false;
                } else {
                    simulationError(tr("the requested curve (%1) could not be set (the variable %2 in component %3 could not be found)").arg(QString::fromStdString(sedmlCurve->getId()))
                                                                                                                                        .arg(xCellmlVariable)
                                                                                                                                        .arg(xCellmlComponent),
                                    InvalidSimulationEnvironment);

                    return false;
                }
            } else if (!yParameter) {
                simulationError(tr("the requested curve (%1) could not be set (the variable %2 in component %3 could not be found)").arg(QString::fromStdString(sedmlCurve->getId()))
                                                                                                                                    .arg(yCellmlVariable)
                                                                                                                                    .arg(yCellmlComponent),
                                InvalidSimulationEnvironment);

                return false;
            }

            bool selected = GraphPanelWidget::DefaultGraphSelected;
            QString title = GraphPanelWidget::DefaultGraphTitle;
            Qt::PenStyle lineStyle = GraphPanelWidget::DefaultGraphLineStyle;
            int lineWidth = GraphPanelWidget::DefaultGraphLineWidth;
            QColor lineColor = GraphPanelWidget::DefaultGraphLineColor;
            QwtSymbol::Style symbolStyle = GraphPanelWidget::DefaultGraphSymbolStyle;
            int symbolSize = GraphPanelWidget::DefaultGraphSymbolSize;
            QColor symbolColor = GraphPanelWidget::DefaultGraphSymbolColor;
            bool symbolFilled = GraphPanelWidget::DefaultGraphSymbolFilled;
            QColor symbolFillColor = GraphPanelWidget::DefaultGraphSymbolFillColor;

            annotation = sedmlCurve->getAnnotation();

            if (annotation) {
                for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
                    const libsbml::XMLNode &curvePropertiesNode = annotation->getChild(i);

                    if (   !QString::fromStdString(curvePropertiesNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                        && !QString::fromStdString(curvePropertiesNode.getName()).compare(SEDMLSupport::Properties)) {
                        for (uint j = 0, jMax = curvePropertiesNode.getNumChildren(); j < jMax; ++j) {
                            const libsbml::XMLNode &curvePropertyNode = curvePropertiesNode.getChild(j);
                            QString curvePropertyNodeName = QString::fromStdString(curvePropertyNode.getName());
                            QString curvePropertyNodeValue = QString::fromStdString(curvePropertyNode.getChild(0).getCharacters());

                            if (!curvePropertyNodeName.compare(SEDMLSupport::Selected)) {
                                selected = !curvePropertyNodeValue.compare(TrueValue);
                            } else if (!curvePropertyNodeName.compare(SEDMLSupport::Title)) {
                                title = curvePropertyNodeValue;
                            } else if (!curvePropertyNodeName.compare(SEDMLSupport::Line)) {
                                for (uint k = 0, kMax = curvePropertyNode.getNumChildren(); k < kMax; ++k) {
                                    const libsbml::XMLNode &linePropertyNode = curvePropertyNode.getChild(k);
                                    QString linePropertyNodeName = QString::fromStdString(linePropertyNode.getName());
                                    QString linePropertyNodeValue = QString::fromStdString(linePropertyNode.getChild(0).getCharacters());

                                    if (!linePropertyNodeName.compare(SEDMLSupport::Style)) {
                                        lineStyle = SEDMLSupport::lineStyle(linePropertyNodeValue);
                                    } else if (!linePropertyNodeName.compare(SEDMLSupport::Width)) {
                                        lineWidth = linePropertyNodeValue.toInt();
                                    } else if (!linePropertyNodeName.compare(SEDMLSupport::Color)) {
                                        lineColor.setNamedColor(linePropertyNodeValue);
                                    }
                                }
                            } else if (!curvePropertyNodeName.compare(SEDMLSupport::Symbol)) {
                                for (uint k = 0, kMax = curvePropertyNode.getNumChildren(); k < kMax; ++k) {
                                    const libsbml::XMLNode &symbolPropertyNode = curvePropertyNode.getChild(k);
                                    QString symbolPropertyNodeName = QString::fromStdString(symbolPropertyNode.getName());
                                    QString symbolPropertyNodeValue = QString::fromStdString(symbolPropertyNode.getChild(0).getCharacters());

                                    if (!symbolPropertyNodeName.compare(SEDMLSupport::Style))
                                        symbolStyle = SEDMLSupport::symbolStyle(symbolPropertyNodeValue);
                                    else if (!symbolPropertyNodeName.compare(SEDMLSupport::Size))
                                        symbolSize = symbolPropertyNodeValue.toInt();
                                    else if (!symbolPropertyNodeName.compare(SEDMLSupport::Color))
                                        symbolColor.setNamedColor(symbolPropertyNodeValue);
                                    else if (!symbolPropertyNodeName.compare(SEDMLSupport::Filled))
                                        symbolFilled = !symbolPropertyNodeValue.compare(TrueValue);
                                    else if (!symbolPropertyNodeName.compare(SEDMLSupport::FillColor))
                                        symbolFillColor.setNamedColor(symbolPropertyNodeValue);
                                }
                            }
                        }
                    }
                }
            }

            graphPanel->addGraph(new GraphPanelWidget::GraphPanelPlotGraph(xParameter, yParameter, graphPanel),
                                 GraphPanelWidget::GraphPanelPlotGraphProperties(selected, title, lineStyle, lineWidth, lineColor, symbolStyle, symbolSize, symbolColor, symbolFilled, symbolFillColor));
        }
    }

    // Set our graph panels sizes and if none were provided then use some
    // default sizes

    if(graphPanelsWidgetSizes.isEmpty()) {
        for (int i = 0, iMax = graphPanelsWidget->count(); i < iMax; ++i)
            graphPanelsWidgetSizes << 1;
    }

    graphPanelsWidget->setSizes(graphPanelsWidgetSizes);

    // Select our first graph panel, now that we are fully initialised

    graphPanelsWidget->setActiveGraphPanel(graphPanelsWidget->graphPanels().first());

    // Initialise our trackers, so we know if a SED-ML file or a COMBINE archive
    // has been modified

    initializeTrackers();

    return true;
}

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

void SimulationExperimentViewSimulationWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(mSplitterWidget->sizes());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataImport()
{
    // Ask for the data files to be imported

    QStringList fileNames = Core::getOpenFileNames(tr("Data Import"),
                                                   Core::filters(Core::dataStoreFileTypeInterfaces()));

    // Import the one or several data files

    importDataFiles(fileNames);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationResultsExport()
{
    // Retrieve some data so that we can effectively export our simulation
    // results

    DataStoreInterface *dataStoreInterface = mDataStoreInterfaces.value(qobject_cast<QAction *>(sender()));
    DataStore::DataStoreExportData *dataStoreData = dataStoreInterface->getExportData(mSimulation->fileName(),
                                                                                      mSimulation->results()->dataStore(),
                                                                                      CellMLSupport::CellmlFileRuntimeParameter::icons());

    if (dataStoreData) {
        // We have got the data we need, so do the actual export

        Core::centralWidget()->showProgressBusyWidget();

        DataStore::DataStoreExporter *dataStoreExporter = dataStoreInterface->dataStoreExporterInstance();

        connect(dataStoreExporter, &DataStore::DataStoreExporter::progress,
                this, &SimulationExperimentViewSimulationWidget::dataStoreExportProgress,
                Qt::UniqueConnection);

        connect(dataStoreExporter, &DataStore::DataStoreExporter::done,
                this, &SimulationExperimentViewSimulationWidget::dataStoreExportDone,
                Qt::UniqueConnection);
        connect(dataStoreExporter, &DataStore::DataStoreExporter::done,
                dataStoreData, &DataStore::DataStoreExportData::deleteLater);

        dataStoreExporter->exportData(dataStoreData);
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateDelayValue(double pDelayValue)
{
    // Update our delay value widget

    quint64 delay = 0;
    quint64 increment = 1;
    quint64 multiple = 10;

    for (int i = 0, iMax = int(pDelayValue); i < iMax; ++i) {
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

void SimulationExperimentViewSimulationWidget::simulationRunning(bool pIsResuming)
{
    Q_UNUSED(pIsResuming);

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

        if (!mSimulation->data()->nlaSolverName().isEmpty())
            solversInformation += "+"+mSimulation->data()->nlaSolverName();

        output(QString(OutputTab+"<strong>"+tr("Simulation time:")+"</strong> <span"+OutputInfo+">"+tr("%1 using %2").arg(Core::formatTime(pElapsedTime))
                                                                                                                     .arg(solversInformation)+"</span>."+OutputBrLn));
    }

    // Update our parameters and simulation mode

    updateSimulationMode();

    mContentsWidget->informationWidget()->parametersWidget()->updateParameters(mSimulation->currentPoint());

    // Stop keeping track of our simulation progress

    mProgress = -1;

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

    emit mViewWidget->updateFileTabIcon(mPlugin->viewName(), mSimulation->fileName(), NoIcon);
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

    QTimer::singleShot(169, this,
                       isVisible()?
                           &SimulationExperimentViewSimulationWidget::resetProgressBar:
                           &SimulationExperimentViewSimulationWidget::resetFileTabIcon);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationError(const QString &pMessage,
                                                               ErrorType pErrorType)
{
    // Output the simulation error

    mErrorType = pErrorType;

    updateInvalidModelMessageWidget();

    output(OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+pMessage+".</span>"+OutputBrLn);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationError(const QString &pMessage)
{
    // Output the simulation error

    simulationError(pMessage, General);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationDataModified(bool pIsModified)
{
    // Update some of our actions based on whether we are modified

    if (mDevelopmentModeAction->isChecked())
        Core::FileManager::instance()->setModified(mSimulation->fileName(), pIsModified);

    mResetStateModelParametersAction->setEnabled(mSimulation->data()->isStatesModified());
    mResetAllModelParametersAction->setEnabled(pIsModified);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::simulationPropertyChanged(Core::Property *pProperty)
{
    // Update our simulation properties, as well as our plots

    updateSimulationProperties(pProperty);

    bool needProcessingEvents = false;
    // Note: needProcessingEvents is used to ensure that our plots are all
    //       updated at once...

    for (auto plot : mPlots) {
        if (updatePlot(plot, true, false))
            needProcessingEvents = true;
    }

    if (needProcessingEvents)
        processEvents();
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
    // Keep track of the fact that we want to know if a graph panel's plot's
    // axes have been changed
    // Note: we don't need to keep track of the graph panel's plot (in mPlots)
    //       since we only want to do this if the plot actually has graphs
    //       associated with it (see graphAdded())...

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    mUpdatablePlotViewports.insert(plot, true);

    connect(plot, &GraphPanelWidget::GraphPanelPlotWidget::axesChanged,
            this, &SimulationExperimentViewSimulationWidget::plotAxesChanged);

    // Let people know when some graph panel settings or graphs settings have
    // been requested

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

    removePlot(plot);

    // Check our graph panels and their graphs

    checkGraphPanelsAndGraphs();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                                                        CellMLSupport::CellmlFileRuntimeParameter *pParameterY)
{
    // Ask the current graph panel to add a new graph for the given parameters
    // Note: due to the way legend labels are handled by Qwt, we temporarily
    //       disable their update (and reenable it once the graph has been
    //       added; see graphAdded()) since otherwise we may see the label flash
    //       due to its properties (e.g. font size) not being the same as those
    //       of QwtLegendLabel...

    GraphPanelWidget::GraphPanelWidget *graphPanel = mContentsWidget->graphPanelsWidget()->activeGraphPanel();

    graphPanel->plot()->legend()->setUpdatesEnabled(false);

    GraphPanelWidget::GraphPanelPlotGraph *graph = new GraphPanelWidget::GraphPanelPlotGraph(pParameterX, pParameterY, graphPanel);

    graphPanel->addGraph(graph, defaultGraphProperties(pParameterY->formattedName(), graph->color()));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphAdded(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                          GraphPanelWidget::GraphPanelPlotGraph *pGraph)
{
    // A new graph has been added, so add runs to it, if our simulation has some

    for (int i = 0, iMax = mSimulation->runsCount(); i < iMax; ++i)
        pGraph->addRun();

    // Allow our plot's legend to get updated

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    plot->legend()->setUpdatesEnabled(true);

    // Now, keep track of the graph and update its plot
    // Note: updating the plot will, if needed, update the plot's axes and, as a
    //       result, replot the graphs including our new one. On the other hand,
    //       if the plot's axes don't get updated, we need to draw our new
    //       graph...

    for (int i = 0, iMax = mSimulation->runsCount(); i < iMax; ++i)
        updateGraphData(pGraph, mSimulation->results()->size(i), i);

    if (   updatePlot(plot, true, false)
        || plot->drawGraphFrom(pGraph, 0)) {
        processEvents();
        // Note: this ensures that our plot is updated at once...
    }

    // Keep track of the plot itself, if needed

    if (!mPlots.contains(plot))
        mPlots << plot;

    // Check our graph panels and their graphs

    checkGraphPanelsAndGraphs();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphsRemoved(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                                             const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs)
{
    Q_UNUSED(pGraphs);

    // One or several graphs have been removed, so update and stop tracking (if
    // needed) their corresponding plot
    // Note: even if the axes' values of the plot haven't changed, we still want
    //       to replot the plot since at least one of its graphs has been
    //       removed...

    GraphPanelWidget::GraphPanelPlotWidget *plot = pGraphPanel->plot();

    updatePlot(plot, true, true);

    processEvents();
    // Note: this ensures that our plot is updated at once...

    if (plot->graphs().isEmpty())
        removePlot(plot);

    // Check our graph panels and their graphs

    checkGraphPanelsAndGraphs();
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphsUpdated(const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs)
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

        if (simulation) {
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
        for (auto plot : plots) {
            updatePlot(plot, true, true);
            // Note: even if the axes' values of the plot haven't changed, we
            //       still want to replot the plot since at least one of its
            //       graphs has been updated...
        }

        processEvents();
        // Note: this ensures that our plots are all updated at once...
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::graphUpdated(GraphPanelWidget::GraphPanelPlotGraph *pGraph)
{
    // The given graph has been updated, so make sure that its corresponding
    // plots are up to date

    graphsUpdated(GraphPanelWidget::GraphPanelPlotGraphs() << pGraph);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::removePlot(GraphPanelWidget::GraphPanelPlotWidget *pPlot)
{
    // Stop tracking the given plot

    mPlots.removeOne(pPlot);
    mUpdatablePlotViewports.remove(pPlot);
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

    QRectF dataRect = QRectF();

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

    QRectF dataLogRect = QRectF();

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

    bool hasData = pPlot->hasData();

    bool needInitialisationX = true;
    bool needInitialisationY = true;

    for (auto graph : pPlot->graphs()) {
        if (graph->isVisible()) {
            SimulationSupport::Simulation *simulation = mViewWidget->simulation(graph->fileName());
            double startingPoint = simulation->data()->startingPoint();
            double endingPoint = simulation->data()->endingPoint();
            double pointInterval = simulation->data()->pointInterval();
            double startingLogPoint = startingPoint;
            double endingLogPoint = endingPoint;
            int nbOfPointIntervals = 0;

            if (pointInterval > 0.0) {
                while (startingLogPoint <= 0.0)
                    startingLogPoint = startingPoint+(++nbOfPointIntervals)*pointInterval;

                nbOfPointIntervals = 0;

                while (endingLogPoint <= 0.0)
                    endingLogPoint = endingPoint+(++nbOfPointIntervals)*pointInterval;
            } else if (pointInterval < 0.0) {
                while (startingLogPoint <= 0.0)
                    startingLogPoint = startingPoint-(++nbOfPointIntervals)*pointInterval;

                nbOfPointIntervals = 0;

                while (endingLogPoint <= 0.0)
                    endingLogPoint = endingPoint-(++nbOfPointIntervals)*pointInterval;
            }

            if (startingPoint > endingPoint)
                std::swap(startingPoint, endingPoint);

            if (static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterX())->type() == CellMLSupport::CellmlFileRuntimeParameter::Voi) {
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

            if (static_cast<CellMLSupport::CellmlFileRuntimeParameter *>(graph->parameterY())->type() == CellMLSupport::CellmlFileRuntimeParameter::Voi) {
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

    pPlot->optimizeAxisX(minX, maxX, GraphPanelWidget::GraphPanelPlotWidget::Linear);
    pPlot->optimizeAxisY(minY, maxY, GraphPanelWidget::GraphPanelPlotWidget::Linear);

    pPlot->optimizeAxisX(minLogX, maxLogX, GraphPanelWidget::GraphPanelPlotWidget::Logarithmic);
    pPlot->optimizeAxisY(minLogY, maxLogY, GraphPanelWidget::GraphPanelPlotWidget::Logarithmic);

    pPlot->setDefaultAxesValues(minX, maxX, minLogX, maxLogX,
                                minY, maxY, minLogY, maxLogY);

    if (   pCanSetAxes
        && pPlot->setAxes(pPlot->logAxisX()?minLogX:minX,
                          pPlot->logAxisX()?maxLogX:maxX,
                          pPlot->logAxisY()?minLogY:minY,
                          pPlot->logAxisY()?maxLogY:maxY,
                          true, true, false, false, false, false)) {
        return true;
    } else if (pForceReplot) {
        pPlot->replot();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

double * SimulationExperimentViewSimulationWidget::data(SimulationSupport::Simulation *pSimulation,
                                                        CellMLSupport::CellmlFileRuntimeParameter *pParameter,
                                                        int pRun) const
{
    // Return the array of data points associated with the given parameter

    switch (pParameter->type()) {
    case CellMLSupport::CellmlFileRuntimeParameter::Voi:
        return pSimulation->results()->points(pRun);
    case CellMLSupport::CellmlFileRuntimeParameter::Constant:
    case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
        return pSimulation->results()->constants(pParameter->index(), pRun);
    case CellMLSupport::CellmlFileRuntimeParameter::Rate:
        return pSimulation->results()->rates(pParameter->index(), pRun);
    case CellMLSupport::CellmlFileRuntimeParameter::State:
        return pSimulation->results()->states(pParameter->index(), pRun);
    case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
        return pSimulation->results()->algebraic(pParameter->index(), pRun);
    case CellMLSupport::CellmlFileRuntimeParameter::Data:
        return pSimulation->results()->data(pParameter->data(),
                                            pParameter->index(), pRun);
    default:
        // Not a relevant type, so return null
        // Note: we should never reach this point...

        return nullptr;
    }
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

    if (pCheckVisibility && !isVisible())
        return;

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

        for (auto plot : mPlots)
            updatePlot(plot, true, true);

        processEvents();
        // Note: this ensures that our plots are all updated at once...
    }

    // Make sure that our progress bar is up to date

    mProgressBarWidget->setValue(mViewWidget->simulationResultsSize(mSimulation->fileName())/mSimulation->size());
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSimulationResults(SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                       quint64 pSimulationResultsSize,
                                                                       int pSimulationRun,
                                                                       Task pTask)
{
    // Update our simulation results

    SimulationSupport::Simulation *simulation = pSimulationWidget->simulation();

    if (simulation == mSimulation)
        simulationDataModified(simulation->data()->isModified());

    // Update all the graphs of all our plots, but only if we are visible
    // Note: needProcessingEvents is used to ensure that our plots are all
    //       updated at once...

    bool visible = isVisible();
    bool needProcessingEvents = false;

    for (auto plot : mPlots) {
        // If our runs are to be reset (i.e. our plot's viewport are going to be
        // reset) or a run to be added (be it really or faked), then we want to
        // be able to update our plot's viewport if needed (i.e. a graph segment
        // doesn't fit within our plot's current viewport anymore)

        if (pTask != None)
            mUpdatablePlotViewports.insert(plot, true);

        // Now we are ready to actually update all the graphs of all our plots

        bool needFullUpdatePlot = !plot->isOptimizedAxes();
        double plotMinX = plot->minX();
        double plotMaxX = plot->maxX();
        double plotMinY = plot->minY();
        double plotMaxY = plot->maxY();

        for (auto graph : plot->graphs()) {
            if (!graph->fileName().compare(pSimulationWidget->simulation()->fileName())) {
                if (pTask != None)
                    mOldDataSizes.remove(graph);

                // Reset our runs or a add new one, if needed

                if (pTask == ResetRuns)
                    graph->removeRuns();
                else if (pTask == AddRun)
                    graph->addRun();

                // Update our graph's data and keep track of our new old data
                // size, if we are visible
                // Note: indeed, to update our graph's old data size if we are
                //       not visible means that when we come back to this file,
                //       part of the graphs will be missing...

                quint64 oldDataSize = graph->dataSize();

                if (visible)
                    mOldDataSizes.insert(graph, oldDataSize);

                updateGraphData(graph, pSimulationResultsSize, pSimulationRun);

                // We need to update our plot, if we are drawing this graph's
                // first segment or if we were invisible at some point during
                // the simulation

                quint64 realOldDataSize = mOldDataSizes.value(graph);

                needFullUpdatePlot =    needFullUpdatePlot || !realOldDataSize
                                     || (oldDataSize != realOldDataSize);

                // Draw the graph's new segment, but only if we and our graph
                // are visible, and that there is no need to update the plot and
                // that there is some data to plot

                if (    visible && graph->isVisible()
                    && !needFullUpdatePlot && pSimulationResultsSize) {
                    // Check that our graph segment can fit within our plot's
                    // current viewport, but only if the user hasn't changed the
                    // plot's viewport since we last came here (e.g. by panning
                    // the plot's contents)

                    if (mUpdatablePlotViewports.value(plot)) {
                        double minX = plotMinX;
                        double maxX = plotMaxX;
                        double minY = plotMinY;
                        double maxY = plotMaxY;

                        for (quint64 i = oldDataSize?oldDataSize-1:0;
                             i < pSimulationResultsSize; ++i) {
                            double valX = graph->data(pSimulationRun)->sample(i).x();
                            double valY = graph->data(pSimulationRun)->sample(i).y();

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
                        if (plot->drawGraphFrom(graph, realOldDataSize-1))
                            needProcessingEvents = true;
                    }
                }
            }
        }

        // Check whether we need to update/replot our plot, but only if we are
        // visible

        if (visible) {
            if (needFullUpdatePlot || !pSimulationResultsSize) {
                // Either we need a full update of our plot (because we are
                // drawing a graph's first segment or a graph's new segment
                // doesn't fit within the plot's current viewport) or the size
                // of our simulation results is zero (because we are starting a
                // simulation or clearing our plot), so update our plot
                // Note: in case we are starting a simulation or clearing our
                //       plot, we don't want a full update of our plot since
                //       this is going to reset its axes' values and therefore
                //       result in some (expected) flickering, if some data is
                //       to be drawn straightaway (e.g. when we start a
                //       simulation)...

                updatePlot(plot, needFullUpdatePlot, true);

                needProcessingEvents = true;
            }
        } else if (needFullUpdatePlot || !pSimulationResultsSize) {
            // We would normally update our plot, but we are not visible, so no
            // point in doing it, so instead we keep track of the fact that we
            // will need to update our plots the next time we become visible

            mNeedUpdatePlots = true;
        }
    }

    // Process events, if needed

    if (needProcessingEvents)
        processEvents();

    // Update our progress bar or our tab icon, if needed

    if (simulation == mSimulation) {
        QString simulationFileName = mSimulation->fileName();
        double simulationProgress = double(mViewWidget->simulationResultsSize(simulationFileName))/simulation->size();

        if ((pTask != None) || visible) {
            mProgressBarWidget->setValue(simulationProgress);
        } else {
            // We are not visible, so create an icon that shows our simulation's
            // progress and let people know about it

            int newProgress = int((tabBarPixmapSize()-2)*simulationProgress);
            // Note: tabBarPixmapSize()-2 because we want a one-pixel wide
            //       border...

            if (newProgress != mProgress) {
                // The progress has changed, so keep track of its new value and
                // update our file tab icon

                mProgress = newProgress;

                // Let people know about the file tab icon to be used for the
                // model

                emit mViewWidget->updateFileTabIcon(mPlugin->viewName(),
                                                    simulationFileName,
                                                    fileTabIcon());
            }
        }
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSimulationResults(SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                       quint64 pSimulationResultsSize,
                                                                       Task pTask)
{
    // Update our simulation results

    updateSimulationResults(pSimulationWidget, pSimulationResultsSize, -1, pTask);
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

void SimulationExperimentViewSimulationWidget::dataStoreImportProgress(DataStore::DataStoreImportData *pImportData,
                                                                       double pProgress)
{
    // There has been some progress with our import, so update our busy widget

    mDataImportProgresses.insert(pImportData, pProgress);

    Core::centralWidget()->setBusyWidgetProgress(*std::min_element(mDataImportProgresses.begin(),
                                                                   mDataImportProgresses.end()));
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreImportDone(DataStore::DataStoreImportData *pImportData,
                                                                   const QString &pErrorMessage)
{
    // We are done with an import, so don't track its progress anymore and keep
    // track of its error message

    mDataImportProgresses.remove(pImportData);
    mDataImportErrorMessages.insert(pImportData, pErrorMessage);

    // If mImportDataProgresses is empty then it means that all our imports are
    // done, in which case we need to update our Parameters section with our
    // imported data, hide our busy widget and display error messages, if needed

    if (mDataImportProgresses.isEmpty()) {
        // Ask our simulation to account for our imported data, and update our
        // Graphs and Parameters sections with our imported data

        QList<DataStore::DataStoreImportData *> dataStoreImportDatas = mDataImportErrorMessages.keys();

        for (auto dataStoreImportData : dataStoreImportDatas) {
            mSimulation->importData(dataStoreImportData);

            mContentsWidget->informationWidget()->graphPanelAndGraphsWidget()->importData(dataStoreImportData);
            mContentsWidget->informationWidget()->parametersWidget()->importData(dataStoreImportData);
        }

        // Hide our busy widget

        Core::centralWidget()->hideBusyWidget();

        // Let people know about any error that we came across

        for (auto dataStoreImportData : dataStoreImportDatas) {
            QString errorMessage = mDataImportErrorMessages.value(dataStoreImportData);

            if (!errorMessage.isEmpty()) {
                Core::warningMessageBox(tr("Data Import"),
                                        tr("<strong>%1</strong> could not be imported (%2).").arg(dataStoreImportData->fileName())
                                                                                             .arg(Core::formatMessage(errorMessage, true)));
            }
        }

        mDataImportErrorMessages.clear();

        emit allImportsDone();
    }
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreExportProgress(DataStore::DataStoreExportData *pDataStoreData,
                                                                       double pProgress)
{
    Q_UNUSED(pDataStoreData);

    // There has been some progress with our export, so update our busy widget

    Core::centralWidget()->setBusyWidgetProgress(pProgress);
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::dataStoreExportDone(DataStore::DataStoreExportData *pDataStoreData,
                                                                   const QString &pErrorMessage)
{
    Q_UNUSED(pDataStoreData);

    // We are done with the export, so hide our busy widget

    Core::centralWidget()->hideBusyWidget();

    // Display the given error message, if any

    if (!pErrorMessage.isEmpty()) {
        Core::warningMessageBox(tr("Simulation Results Export"),
                                pErrorMessage);
    }
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

    if (   (   (mSimulation->fileType() != SimulationSupport::Simulation::SedmlFile)
            && (mSimulation->fileType() != SimulationSupport::Simulation::CombineArchive))
        || mGraphPanelsWidgetSizes.isEmpty()) {
        return;
    }

    // Check whether any of our graph panels' height has changed

    GraphPanelWidget::GraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();

    mGraphPanelsWidgetSizesModified = graphPanelsWidget->sizes() != mGraphPanelsWidgetSizes;

    // Check whether any of our graph panel / graphs properties has changed

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget *graphPanelAndGraphsWidget = mContentsWidget->informationWidget()->graphPanelAndGraphsWidget();

    mGraphPanelPropertiesModified.clear();
    mGraphsPropertiesModified.clear();

    for (auto graphPanel : graphPanelsWidget->graphPanels()) {
        Core::PropertyEditorWidget *propertyEditor = graphPanelAndGraphsWidget->graphPanelPropertyEditor(graphPanel);

        mGraphPanelPropertiesModified.insert(propertyEditor,
                                             mGraphPanelProperties.contains(propertyEditor)?
                                                 allPropertyValues(propertyEditor) != mGraphPanelProperties.value(propertyEditor):
                                                 true);

        propertyEditor = graphPanelAndGraphsWidget->graphsPropertyEditor(graphPanel);

        mGraphsPropertiesModified.insert(propertyEditor,
                                         mGraphsProperties.contains(propertyEditor)?
                                             allPropertyValues(propertyEditor) != mGraphsProperties.value(propertyEditor):
                                             true);
    }

    // Update our file's modified status

    updateSedmlFileOrCombineArchiveModifiedStatus();
}

//==============================================================================

QStringList SimulationExperimentViewSimulationWidget::allPropertyValues(Core::PropertyEditorWidget *pPropertyEditor) const
{
    // Return all the property values of the given property editor

    QStringList res = QStringList();

    for (auto property : pPropertyEditor->allProperties()) {
        if (property->isCheckable()) {
            if (property->isChecked())
                res << TrueValue;
            else
                res << FalseValue;
        }

        res << property->value();
    }

    return res;
}

//==============================================================================

void SimulationExperimentViewSimulationWidget::updateSedmlFileOrCombineArchiveModifiedStatus()
{
    // Make sure that we are not dealing with a remote file

    QString simulationFileName = mSimulation->fileName();

    if (Core::FileManager::instance()->isRemote(simulationFileName))
        return;

    // Update the modified status of the current file, based on whether its
    // simulation, solvers, graph panel or graphs properties have changed, and
    // keeping in mind that we may have added/removed graph panels

    bool graphPanelPropertiesModified = mGraphPanelProperties.keys() != mGraphPanelPropertiesModified.keys();
    bool graphsPropertiesModified = mGraphsProperties.keys() != mGraphsPropertiesModified.keys();

    if (!graphPanelPropertiesModified) {
        for (auto someGraphPanelPropertiesModified : mGraphPanelPropertiesModified.values())
            graphPanelPropertiesModified = graphPanelPropertiesModified || someGraphPanelPropertiesModified;
    }

    if (!graphsPropertiesModified) {
        for (auto someGraphsPropertiesModified : mGraphsPropertiesModified.values())
            graphsPropertiesModified = graphsPropertiesModified || someGraphsPropertiesModified;
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

void SimulationExperimentViewSimulationWidget::importDataFiles(const QStringList &pFileNames)
{
    // Import the given data files

    QMap<QString, DataStoreInterface *> dataStoreInterfaces = QMap<QString, DataStoreInterface *>();

    QStringList invalidDataFileNames = QStringList();

    for (const auto &fileName : pFileNames) {
        // Determine the type of data file we are dealing with so we can use the
        // correct data store interface
        // Note: we check whether mDataStoreFiles contains an entry for the
        //       current file (which would mean that the file was dropped on the
        //       Simulation Experiment view) and if not (i.e. we want to open a
        //       data file using the main menu) then check whether the file is a
        //       data file...

        FileTypeInterface *fileTypeInterface = mDataStoreFiles.value(fileName);

        if (!fileTypeInterface) {
            for (auto dataStoreFileTypeInterface : Core::dataStoreFileTypeInterfaces()) {
                if (dataStoreFileTypeInterface->isFile(fileName)) {
                    fileTypeInterface = dataStoreFileTypeInterface;

                    break;
                }
            }
        }

        if (fileTypeInterface) {
            dataStoreInterfaces.insert(fileName, Core::dataStoreInterface(fileTypeInterface));

            mDataStoreFiles.remove(fileName);
        } else {
            invalidDataFileNames << fileName;
        }
    }

    // Let people know about our invalid data files, if any

    for (const auto &invalidDataFileName : invalidDataFileNames) {
        Core::warningMessageBox(tr("Data Import"),
                                tr("<strong>%1</strong> is not a data file.").arg(invalidDataFileName));
    }

    // Retrieve some imported data for our different data files

    enum Problem {
        FileAccess,
        MemoryAllocation
    };

    QMap<QString, DataStore::DataStoreImportData *> dataStoreImportDatas = QMap<QString, DataStore::DataStoreImportData *>();
    QMap<QString, Problem> problems = QMap<QString, Problem>();
    QList<quint64> runSizes = QList<quint64>();

    for (int i = 0, iMax = mSimulation->runsCount(); i < iMax; ++i)
        runSizes << mSimulation->runSize(i);

    for (const auto &fileName : dataStoreInterfaces.keys()) {
        DataStore::DataStoreImportData *dataStoreImportData = dataStoreInterfaces.value(fileName)->getImportData(fileName, mSimulation->importData()->addDataStore(),
                                                                                                                           mSimulation->results()->dataStore(),
                                                                                                                           runSizes);

        if (dataStoreImportData) {
            // We have some import data, so now check whether it is actually
            // valid

            if (dataStoreImportData->valid()) {
                dataStoreImportDatas.insert(fileName, dataStoreImportData);

                mDataImportProgresses.insert(dataStoreImportData, 0.0);
            } else {
                delete dataStoreImportData;

                problems.insert(fileName, MemoryAllocation);
            }
        } else {
            problems.insert(fileName, FileAccess);
        }
    }

    // Let people know about the problems, if any, we got while trying to
    // retrieve our imported data

    for (const auto &fileName : problems.keys()) {
        Core::warningMessageBox(tr("Data Import"),
                                tr("<strong>%1</strong> could not be imported (%2).").arg(fileName)
                                                                                     .arg((problems.value(fileName) == FileAccess)?
                                                                                              tr("the file could not be accessed"):
                                                                                              tr("the memory needed to store the data could not be allocated")));
    }

    // We have got the imported data we need, so now do the actual import of our
    // data files

    if (!dataStoreImportDatas.isEmpty())
        Core::centralWidget()->showProgressBusyWidget();

    for (const auto &fileName : dataStoreImportDatas.keys()) {
        DataStore::DataStoreImporter *dataStoreImporter = dataStoreInterfaces.value(fileName)->dataStoreImporterInstance();
        DataStore::DataStoreImportData *dataStoreImportData = dataStoreImportDatas.value(fileName);

        connect(dataStoreImporter, &DataStore::DataStoreImporter::progress,
                this, &SimulationExperimentViewSimulationWidget::dataStoreImportProgress,
                Qt::UniqueConnection);

        connect(dataStoreImporter, &DataStore::DataStoreImporter::done,
                this, &SimulationExperimentViewSimulationWidget::dataStoreImportDone,
                Qt::UniqueConnection);
        connect(this, &SimulationExperimentViewSimulationWidget::allImportsDone,
                dataStoreImportData, &DataStore::DataStoreImportData::deleteLater);

        dataStoreImporter->importData(dataStoreImportData);
    }
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

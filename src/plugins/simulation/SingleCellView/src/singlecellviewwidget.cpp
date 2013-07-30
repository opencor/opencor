//==============================================================================
// Single cell view widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlfileruntime.h"
#include "coreutils.h"
#include "progressbarwidget.h"
#include "propertyeditorwidget.h"
#include "singlecellviewcontentswidget.h"
#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewgraphpanelswidget.h"
#include "singlecellviewgraphpanelwidget.h"
#include "singlecellviewinformationgraphswidget.h"
#include "singlecellviewinformationparameterswidget.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewinformationsolverswidget.h"
#include "singlecellviewinformationwidget.h"
#include "singlecellviewplugin.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewwidget.h"
#include "toolbarwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_singlecellviewwidget.h"

//==============================================================================

#include <QBrush>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QImage>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QScrollBar>
#include <QSettings>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include <QToolButton>
#include <QVariant>

//==============================================================================

#include "float.h"

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_wheel.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

static const QString OutputTab  = "&nbsp;&nbsp;&nbsp;&nbsp;";
static const QString OutputGood = " style=\"color: green;\"";
static const QString OutputInfo = " style=\"color: navy;\"";
static const QString OutputBad  = " style=\"color: maroon;\"";
static const QString OutputBrLn = "<br/>\n";

//==============================================================================

SingleCellViewWidget::SingleCellViewWidget(SingleCellViewPlugin *pPluginParent,
                                           QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::SingleCellViewWidget),
    mPluginParent(pPluginParent),
    mSolverInterfaces(SolverInterfaces()),
    mSimulation(0),
    mSimulations(QMap<QString, SingleCellViewSimulation *>()),
    mStoppedSimulations(QList<SingleCellViewSimulation *>()),
    mProgresses(QMap<QString, int>()),
    mResets(QMap<QString, bool>()),
    mDelays(QMap<QString, int>()),
    mSplitterWidgetSizes(QList<int>()),
    mRunActionEnabled(true),
    mOldSimulationResultsSizes(QMap<SingleCellViewSimulation *, qulonglong>()),
    mCheckResultsSimulations(QList<SingleCellViewSimulation *>()),
    mGraphs(QList<SingleCellViewGraphPanelPlotGraph *>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a wheel (and a label to show its value) to specify the delay (in
    // milliseconds) between the output of two data points

    mDelayWidget = new QwtWheel(this);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    QWidget *delaySpaceWidget = new QWidget(this);
#endif
    mDelayValueWidget = new QLabel(this);

    mDelayWidget->setBorderWidth(0);
    mDelayWidget->setFixedSize(0.07*qApp->desktop()->screenGeometry().width(),
                               0.5*mDelayWidget->height());
    mDelayWidget->setFocusPolicy(Qt::NoFocus);
    mDelayWidget->setRange(0.0, 50.0);
    mDelayWidget->setWheelBorderWidth(0);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    delaySpaceWidget->setFixedWidth(4);
#endif

    connect(mDelayWidget, SIGNAL(valueChanged(double)),
            this, SLOT(updateDelayValue(const double &)));

    mDelayWidget->setValue(0);

    updateDelayValue(mDelayWidget->value());
    // Note: our call to updateDelayValue() is because the connection is not yet
    //       effective when we set the value of the delay widget...

    // Create a tool bar widget with different buttons

    mToolBarWidget = new Core::ToolBarWidget(this);

    QToolButton *removeGraphPanelToolButton = new QToolButton(mToolBarWidget);
    QMenu *removeGraphPanelDropDownMenu = new QMenu(removeGraphPanelToolButton);

    removeGraphPanelDropDownMenu->addAction(mGui->actionRemoveCurrentGraphPanel);
    removeGraphPanelDropDownMenu->addAction(mGui->actionRemoveAllGraphPanels);

    removeGraphPanelToolButton->setDefaultAction(mGui->actionRemoveGraphPanel);
    removeGraphPanelToolButton->setMenu(removeGraphPanelDropDownMenu);
    removeGraphPanelToolButton->setPopupMode(QToolButton::MenuButtonPopup);

    mToolBarWidget->addAction(mGui->actionRunPauseResumeSimulation);
    mToolBarWidget->addAction(mGui->actionStopSimulation);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mGui->actionResetModelParameters);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addWidget(mDelayWidget);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mToolBarWidget->addWidget(delaySpaceWidget);
#endif
    mToolBarWidget->addWidget(mDelayValueWidget);
/*---GRY--- THE BELOW SHOULD BE RE-ENABLED AT SOME POINT...
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mGui->actionDebugMode);
*/
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mGui->actionAddGraphPanel);
    mToolBarWidget->addWidget(removeGraphPanelToolButton);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mGui->actionSimulationDataCsvExport);

    mTopSeparator = Core::newLineWidget(this);

    mGui->layout->addWidget(mToolBarWidget);
    mGui->layout->addWidget(mTopSeparator);

    // Create and add our invalid simulation message widget

    mInvalidModelMessageWidget = new Core::UserMessageWidget(":/oxygen/actions/help-about.png",
                                                             pParent);

    mGui->layout->addWidget(mInvalidModelMessageWidget);

    // Create our splitter widget and keep track of its movement
    // Note: we need to keep track of its movement so that saveSettings() can
    //       work fine even when mContentsWidget is not visible (which happens
    //       when a CellML file cannot be run for some reason or another)...

    mSplitterWidget = new QSplitter(Qt::Vertical, this);

    connect(mSplitterWidget, SIGNAL(splitterMoved(int,int)),
            this, SLOT(splitterWidgetMoved()));

    // Create our contents widget

    mContentsWidget = new SingleCellViewContentsWidget(this);

    mContentsWidget->setObjectName("Contents");

    // Keep track of changes to some of our simulation and solvers properties

    SingleCellViewInformationWidget *informationWidget = mContentsWidget->informationWidget();

    connect(informationWidget->simulationWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(simulationPropertyChanged(Core::Property *)));
    connect(informationWidget->solversWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(solversPropertyChanged(Core::Property *)));

    // Keep track of whether we can remove graph panels

    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(removeGraphPanelsEnabled(const bool &)),
            mGui->actionRemoveGraphPanel, SLOT(setEnabled(bool)));

    // Keep track of the addition and removal of a graph panel

    SingleCellViewInformationGraphsWidget *graphsWidget = informationWidget->graphsWidget();

    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(graphPanelAdded(SingleCellViewGraphPanelWidget *)),
            graphsWidget, SLOT(initialize(SingleCellViewGraphPanelWidget *)));
    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(graphPanelRemoved(SingleCellViewGraphPanelWidget *)),
            graphsWidget, SLOT(finalize(SingleCellViewGraphPanelWidget *)));

    // Keep track of whether a graph panel has been activated

    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(graphPanelActivated(SingleCellViewGraphPanelWidget *)),
            graphsWidget, SLOT(initialize(SingleCellViewGraphPanelWidget *)));

    // Keep track of a graph being required

    connect(informationWidget->parametersWidget(), SIGNAL(graphRequired(CellMLSupport::CellmlFileRuntimeParameter *, CellMLSupport::CellmlFileRuntimeParameter *)),
            this, SLOT(addGraph(CellMLSupport::CellmlFileRuntimeParameter *, CellMLSupport::CellmlFileRuntimeParameter *)));

    // Keep track of the addition and removal of a graph

    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(graphAdded(SingleCellViewGraphPanelPlotGraph *)),
            graphsWidget, SLOT(addGraph(SingleCellViewGraphPanelPlotGraph *)));
    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(graphRemoved(SingleCellViewGraphPanelPlotGraph *)),
            graphsWidget, SLOT(removeGraph(SingleCellViewGraphPanelPlotGraph *)));

    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(graphAdded(SingleCellViewGraphPanelPlotGraph *)),
            this, SLOT(graphAdded(SingleCellViewGraphPanelPlotGraph *)));
    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(graphRemoved(SingleCellViewGraphPanelPlotGraph *)),
            this, SLOT(graphRemoved(SingleCellViewGraphPanelPlotGraph *)));

    // Keep track of the updating of a graph
    // Note: ideally, this would, as for the addition and removal of a graph
    //       (see above), be done through mContentsWidget->graphPanelsWidget()
    //       (i.e. a graph would let people know that it has been updated and
    //       the graph panel with which it is associated would forward the
    //       signal to mContentsWidget->graphPanelsWidget()), but this may
    //       result in too many graphUpdated() signals being emitted. For
    //       example, say that you change the model with which a graph is
    //       associated, then both the X and Y parameters will get updated, and
    //       for each of those updates a graphUpdated() signal would be emitted
    //       by the graph, hence we would end up with two signals when only one
    //       would have sufficed. Even worse is that after having updated the X
    //       parameter, the graph would have its X parameter coming from the
    //       'new' model while its Y parameter from the 'old' model, which could
    //       mess things up quite a bit from a plotting viewpoint. So, instead,
    //       the updating is done through our graphs property editor...

    connect(graphsWidget, SIGNAL(graphUpdated(SingleCellViewGraphPanelPlotGraph *)),
            this, SLOT(graphUpdated(SingleCellViewGraphPanelPlotGraph *)));

    // Create our simulation output widget with a layout on which we put a
    // separating line and our simulation output list view
    // Note: the separating line is because we remove, for aesthetical reasons,
    //       the border of our simulation output list view...

    QWidget *simulationOutputWidget = new QWidget(this);
    QVBoxLayout *simulationOutputLayout= new QVBoxLayout(simulationOutputWidget);

    simulationOutputLayout->setMargin(0);
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
    // our single cell view widget

    mSplitterWidget->addWidget(mContentsWidget);
    mSplitterWidget->addWidget(simulationOutputWidget);

    mSplitterWidget->setSizes(QList<int>() << qApp->desktop()->screenGeometry().height() << 1);

    mGui->layout->addWidget(mSplitterWidget);

    // Create our (thin) simulation progress widget

    mBottomSeparator = Core::newLineWidget(this);
    mProgressBarWidget = new Core::ProgressBarWidget(this);

    mProgressBarWidget->setFixedHeight(3);
    mProgressBarWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    mGui->layout->addWidget(mBottomSeparator);
    mGui->layout->addWidget(mProgressBarWidget);

    // Make our contents widget our focus proxy

    setFocusProxy(mContentsWidget);
}

//==============================================================================

SingleCellViewWidget::~SingleCellViewWidget()
{
    // Delete our simulation objects

    foreach (SingleCellViewSimulation *simulation, mSimulations)
        delete simulation;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellViewWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate our delay and delay value widgets

    mDelayWidget->setToolTip(tr("Simulation Delay"));
    mDelayValueWidget->setToolTip(mDelayWidget->toolTip());

    mDelayWidget->setStatusTip(tr("Delay in milliseconds between two data points"));
    mDelayValueWidget->setStatusTip(mDelayWidget->statusTip());

    // Retranslate our run/pause action

    updateRunPauseAction(mRunActionEnabled);

    // Retranslate our invalid model message

    updateInvalidModelMessageWidget();

    // Retranslate our contents widget

    mContentsWidget->retranslateUi();
}

//==============================================================================

static const QString SettingsSizesCount = "SizesCount";
static const QString SettingsSize       = "Size%1";

//==============================================================================

void SingleCellViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve and set the sizes of our splitter

    int sizesCount = pSettings->value(SettingsSizesCount, 0).toInt();

    if (sizesCount) {
        mSplitterWidgetSizes = QList<int>();

        for (int i = 0; i < sizesCount; ++i)
            mSplitterWidgetSizes << pSettings->value(SettingsSize.arg(i)).toInt();

        mSplitterWidget->setSizes(mSplitterWidgetSizes);
    }

    // Retrieve the settings of our contents widget

    pSettings->beginGroup(mContentsWidget->objectName());
        mContentsWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our splitter sizes

    pSettings->setValue(SettingsSizesCount, mSplitterWidgetSizes.count());

    for (int i = 0, iMax = mSplitterWidgetSizes.count(); i < iMax; ++i)
        pSettings->setValue(SettingsSize.arg(i), mSplitterWidgetSizes[i]);

    // Keep track of the settings of our contents widget

    pSettings->beginGroup(mContentsWidget->objectName());
        mContentsWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellViewWidget::setSolverInterfaces(const SolverInterfaces &pSolverInterfaces)
{
    // Let our solvers widget know about the solver interfaces

    mContentsWidget->informationWidget()->solversWidget()->setSolverInterfaces(pSolverInterfaces);

    // Keep track of the solver interfaces

    mSolverInterfaces = pSolverInterfaces;
}

//==============================================================================

void SingleCellViewWidget::output(const QString &pMessage)
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

void SingleCellViewWidget::updateSimulationMode()
{
    // Update our run/pause action

    updateRunPauseAction(!mSimulation->isRunning() || mSimulation->isPaused());

    // Enable/disable our stop action

    bool simulationModeEnabled = mSimulation->isRunning() || mSimulation->isPaused();

    mGui->actionStopSimulation->setEnabled(simulationModeEnabled);

    // Enable or disable our simulation and solvers widgets

    mContentsWidget->informationWidget()->simulationWidget()->setEnabled(!simulationModeEnabled);
    mContentsWidget->informationWidget()->solversWidget()->setEnabled(!simulationModeEnabled);

    // Enable/disable our export to CSV

    mGui->actionSimulationDataCsvExport->setEnabled(    mSimulation->results()->size()
                                                    && !simulationModeEnabled);

    // Give the focus to our focus proxy, in case we leave our simulation mode
    // (so that the user can modify simulation data, etc.)

    if (!simulationModeEnabled)
        focusProxy()->setFocus();
}

//==============================================================================

void SingleCellViewWidget::updateRunPauseAction(const bool &pRunActionEnabled)
{
    mRunActionEnabled = pRunActionEnabled;

    mGui->actionRunPauseResumeSimulation->setIcon(pRunActionEnabled?
                                                      QIcon(":/oxygen/actions/media-playback-start.png"):
                                                      QIcon(":/oxygen/actions/media-playback-pause.png"));

    bool simulationPaused = mSimulation && mSimulation->isPaused();

    mGui->actionRunPauseResumeSimulation->setIconText(pRunActionEnabled?
                                                          simulationPaused?
                                                              tr("Resume Simulation"):
                                                              tr("Run Simulation"):
                                                          tr("Pause Simulation"));
    mGui->actionRunPauseResumeSimulation->setStatusTip(pRunActionEnabled?
                                                           simulationPaused?
                                                               tr("Resume the simulation"):
                                                               tr("Run the simulation"):
                                                           tr("Pause the simulation"));
    mGui->actionRunPauseResumeSimulation->setText(pRunActionEnabled?
                                                      simulationPaused?
                                                          tr("Resume Simulation"):
                                                          tr("Run Simulation"):
                                                      tr("Pause Simulation"));
    mGui->actionRunPauseResumeSimulation->setToolTip(pRunActionEnabled?
                                                         simulationPaused?
                                                             tr("Resume Simulation"):
                                                             tr("Run Simulation"):
                                                         tr("Pause Simulation"));
}

//==============================================================================

void SingleCellViewWidget::updateInvalidModelMessageWidget()
{
    // Update our invalid model message

    mInvalidModelMessageWidget->setMessage("<div align=center>"
                                           "    <p>"
                                          +((mErrorType == InvalidCellmlFile)?
                                                "        "+tr("Sorry, but the <strong>%1</strong> view requires a valid CellML file to work...").arg(mPluginParent->viewName()):
                                                "        "+tr("Sorry, but the <strong>%1</strong> view requires a valid simulation environment to work...").arg(mPluginParent->viewName())
                                           )
                                          +"    </p>"
                                           "    <p>"
                                           "        <small><em>("+tr("See below for more information.")+")</em></small>"
                                           "    </p>"
                                           "</div>");
}

//==============================================================================

void SingleCellViewWidget::initialize(const QString &pFileName)
{
    // Keep track of our simulation data for our previous model and finalise a
    // few things, if needed

    SingleCellViewSimulation *previousSimulation = mSimulation;
    SingleCellViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
    SingleCellViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();
    SingleCellViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();

    if (previousSimulation) {
        // There is a previous simulation, so backup our simulation and solvers'
        // settings

        QString previousFileName = previousSimulation->fileName();

        simulationWidget->backup(previousFileName);
        solversWidget->backup(previousFileName);

        // Keep track of the status of the reset action and of the value of the
        // delay widget

        mResets.insert(previousFileName, mGui->actionResetModelParameters->isEnabled());
        mDelays.insert(previousFileName, mDelayWidget->value());
    }

    // Retrieve our simulation object for the current model, if any

    bool newSimulation = false;

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);
    CellMLSupport::CellmlFileRuntime *cellmlFileRuntime = cellmlFile->runtime();

    mSimulation = mSimulations.value(pFileName);

    if (!mSimulation) {
        // No simulation object currently exists for the model, so create one

        mSimulation = new SingleCellViewSimulation(pFileName, cellmlFileRuntime, mSolverInterfaces);

        newSimulation = true;

        // Initialise our simulation object's delay

        mSimulation->setDelay(mDelayWidget->value());

        // Create a few connections

        connect(mSimulation, SIGNAL(running(const bool &)),
                this, SLOT(simulationRunning(const bool &)));
        connect(mSimulation, SIGNAL(paused()),
                this, SLOT(simulationPaused()));
        connect(mSimulation, SIGNAL(stopped(const int &)),
                this, SLOT(simulationStopped(const int &)));

        connect(mSimulation, SIGNAL(error(const QString &)),
                this, SLOT(simulationError(const QString &)));

        connect(mSimulation->data(), SIGNAL(modified(const bool &)),
                this, SLOT(simulationDataModified(const bool &)));

        // Keep track of our simulation object

        mSimulations.insert(pFileName, mSimulation);
    }

    // Retrieve the status of the reset action and the value of the delay widget

    mGui->actionResetModelParameters->setEnabled(mResets.value(pFileName, false));

    mDelayWidget->setValue(mDelays.value(pFileName, 0));

    // Stop tracking our simulation progress and let people know that our file
    // tab icon should be reset, these in case our simulation is running or
    // paused

    if (mSimulation->isRunning() || mSimulation->isPaused()) {
        mProgresses.remove(mSimulation->fileName());

        emit updateFileTabIcon(mSimulation->fileName(), QIcon());
    }

    // Determine whether the CellML file has a valid runtime

    bool validCellmlFileRuntime = cellmlFileRuntime && cellmlFileRuntime->isValid();

    // Retrieve our variable of integration, if possible

    CellMLSupport::CellmlFileRuntimeParameter *variableOfIntegration = validCellmlFileRuntime?cellmlFileRuntime->variableOfIntegration():0;

    // Output some information about our CellML file

    QString information = QString();

    if (!mOutputWidget->document()->isEmpty())
        information += "<hr/>\n";

    information += "<strong>"+pFileName+"</strong>"+OutputBrLn;
    information += OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

    if (variableOfIntegration) {
        // A variable of integration could be retrieved for our CellML file, so
        // we can also output the model type

        QString additionalInformation = QString();

        if (cellmlFileRuntime->needNlaSolver())
            additionalInformation = " + "+tr("NLA system(s)");

        information += "<span"+OutputGood+">"+tr("valid")+"</span>."+OutputBrLn;
        information += QString(OutputTab+"<strong>"+tr("Model type:")+"</strong> <span"+OutputInfo+">%1%2</span>."+OutputBrLn).arg((cellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?tr("ODE"):tr("DAE"),
                                                                                                                                   additionalInformation);
    } else {
        // We couldn't retrieve a variable a variable of integration, which
        // means that we either don't have a runtime or we have one, but it's
        // not valid or it's valid but we really don't have a variable of
        // integration
        // Note: in the case of a valid runtime and no variable of integration,
        //       we really shouldn't consider the runtime to be valid, hence we
        //       handle this case here...

        mErrorType = InvalidCellmlFile;

        updateInvalidModelMessageWidget();

        information += "<span"+OutputBad+">"+(cellmlFileRuntime?tr("invalid"):tr("none"))+"</span>."+OutputBrLn;

        if (validCellmlFileRuntime)
            // We have a valid runtime, but no variable of integration, which
            // means that the model doesn't contain any ODE or DAE

            information += OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+tr("the model must have at least one ODE or DAE")+".</span>"+OutputBrLn;
        else
            // We don't have a valid runtime, so either there are some problems
            // with the CellML file or its runtime

            foreach (const CellMLSupport::CellmlFileIssue &issue,
                     cellmlFileRuntime?cellmlFileRuntime->issues():cellmlFile->issues())
                information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2</span>."+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?tr("Error:"):tr("Warning:"),
                                                                                                                     issue.message());
    }

    output(information);

    // Enable/disable our run/pause action depending on whether we have a
    // variable of integration

    mGui->actionRunPauseResumeSimulation->setEnabled(variableOfIntegration);

    // Update our simulation mode

    updateSimulationMode();

    // Update our previous (if any) and current simulation results

    if (   previousSimulation
        && (previousSimulation->isRunning() || previousSimulation->isPaused()))
        updateResults(previousSimulation, previousSimulation->results()->size());

    updateResults(mSimulation, mSimulation->results()->size(), true);

    // Initialise our contents widget and make sure that we have the required
    // type(s) of solvers

    bool validSimulationEnvironment = false;

    if (variableOfIntegration) {
        // Show our contents widget in case it got previously hidden
        // Note: indeed, if it was to remain hidden then some initialisations
        //       wouldn't work (e.g. the solvers widget has a property editor
        //       which all properties need to be removed and if the contents
        //       widget is not visible, then upon repopulating the property
        //       editor, scrollbars will be shown even though they are not
        //       needed)...

        mContentsWidget->setVisible(true);

        // Initialise our GUI's simulation, solvers, graphs and parameters
        // widgets
        // Note: this will also initialise some of our simulation data (i.e. our
        //       simulation's starting point and simulation's NLA solver's
        //       properties) which is needed since we want to be able to reset
        //       our simulation below...

        simulationWidget->initialize(pFileName, cellmlFileRuntime, mSimulation);
        solversWidget->initialize(pFileName, cellmlFileRuntime, mSimulation);
        informationWidget->graphsWidget()->initialize(pFileName, cellmlFileRuntime, mSimulation);
        informationWidget->parametersWidget()->initialize(pFileName, cellmlFileRuntime, mSimulation);

        // Check whether we have at least one ODE or DAE solver and, if needed,
        // at least one NLA solver

        if (cellmlFileRuntime->needNlaSolver()) {
            if (solversWidget->nlaSolvers().isEmpty()) {
                if (cellmlFileRuntime->needOdeSolver()) {
                    if (solversWidget->odeSolvers().isEmpty())
                        simulationError(tr("the model needs both an ODE and an NLA solver, but none are available"),
                                        InvalidSimulationEnvironment);
                    else
                        simulationError(tr("the model needs both an ODE and an NLA solver, but no NLA solver is available"),
                                        InvalidSimulationEnvironment);
                } else {
                    if (solversWidget->daeSolvers().isEmpty())
                        simulationError(tr("the model needs both a DAE and an NLA solver, but none are available"),
                                        InvalidSimulationEnvironment);
                    else
                        simulationError(tr("the model needs both a DAE and an NLA solver, but no NLA solver is available"),
                                        InvalidSimulationEnvironment);
                }
            } else if (   cellmlFileRuntime->needOdeSolver()
                       && solversWidget->odeSolvers().isEmpty()) {
                simulationError(tr("the model needs both an ODE and an NLA solver, but no ODE solver is available"),
                                InvalidSimulationEnvironment);
            } else if (   cellmlFileRuntime->needDaeSolver()
                       && solversWidget->daeSolvers().isEmpty()) {
                    simulationError(tr("the model needs both a DAE and an NLA solver, but no DAE solver is available"),
                                    InvalidSimulationEnvironment);
            } else {
                // We have the solvers we need, so...

                validSimulationEnvironment = true;
            }
        } else if (   cellmlFileRuntime->needOdeSolver()
                   && solversWidget->odeSolvers().isEmpty()) {
            simulationError(tr("the model needs an ODE solver, but none is available"),
                            InvalidSimulationEnvironment);
        } else if (   cellmlFileRuntime->needDaeSolver()
                   && solversWidget->daeSolvers().isEmpty()) {
            simulationError(tr("the model needs a DAE solver, but none is available"),
                            InvalidSimulationEnvironment);
        } else {
            // We have the solver we need, so...

            validSimulationEnvironment = true;
        }
    }

    // Show/hide some widgets depending on whether we have a valid simulation
    // environment

    bool previousValidSimulationEnvironment = mInvalidModelMessageWidget->isHidden();

    mToolBarWidget->setVisible(validSimulationEnvironment);
    mTopSeparator->setVisible(validSimulationEnvironment);

    mContentsWidget->setVisible(validSimulationEnvironment);
    mInvalidModelMessageWidget->setVisible(!validSimulationEnvironment);

    mBottomSeparator->setVisible(validSimulationEnvironment);
    mProgressBarWidget->setVisible(validSimulationEnvironment);

    // Make sure that the last output message is visible
    // Note: indeed, to (re)show some widgets (see above) might change the
    //       height of our output widget, messing up the vertical scroll bar a
    //       bit (if visible), resulting in the output being shifted a bit...

    if (previousValidSimulationEnvironment != validSimulationEnvironment) {
        qApp->processEvents();

        mOutputWidget->ensureCursorVisible();
    }

    // If we have a valid simulation environment and we are dealing with a new
    // simulation, then reset both the simulation's data and results (well,
    // initialise in the case of its data)

    if (validSimulationEnvironment && newSimulation) {
        mSimulation->data()->reset();
        mSimulation->results()->reset(false);
    }
}

//==============================================================================

void SingleCellViewWidget::finalize(const QString &pFileName)
{
    // Remove our simulation object, should there be one for the given file name

    SingleCellViewSimulation *simulation = mSimulations.value(pFileName);

    if (simulation) {
        // There is a simulation object for the given file name, so delete it
        // and remove it from our list

        delete simulation;

        mSimulations.remove(pFileName);

        // Reset our memory of the current simulation object, but only if it's
        // the same as our simulation object

        if (simulation == mSimulation)
            mSimulation = 0;
    }

    // Remove various information associated with the given file name

    mProgresses.remove(pFileName);

    mResets.remove(pFileName);
    mDelays.remove(pFileName);

    // Finalize a few things in our GUI's simulation, solvers, graphs and
    // parameters widgets

    SingleCellViewInformationWidget *informationWidget = mContentsWidget->informationWidget();

    informationWidget->simulationWidget()->finalize(pFileName);
    informationWidget->solversWidget()->finalize(pFileName);
    informationWidget->graphsWidget()->finalize(pFileName);
    informationWidget->parametersWidget()->finalize(pFileName);
}

//==============================================================================

int SingleCellViewWidget::tabBarIconSize() const
{
    // Return the size of a file tab icon

    return style()->pixelMetric(QStyle::PM_TabBarIconSize, 0, this);
}

//==============================================================================

QIcon SingleCellViewWidget::fileTabIcon(const QString &pFileName) const
{
    // Return a file tab icon which shows the given file's simulation progress

    SingleCellViewSimulation *simulation = mSimulations.value(pFileName);
    int progress = simulation?mProgresses.value(simulation->fileName(), -1):-1;

    if (simulation && (progress != -1)) {
        // Create an image that shows the progress of our simulation

        QImage tabBarIcon = QImage(tabBarIconSize(), mProgressBarWidget->height()+2,
                                   QImage::Format_ARGB32_Premultiplied);
        QPainter tabBarIconPainter(&tabBarIcon);

        tabBarIconPainter.setBrush(QBrush(CommonWidget::windowColor()));
        tabBarIconPainter.setPen(QPen(CommonWidget::borderColor()));

        tabBarIconPainter.drawRect(0, 0, tabBarIcon.width()-1, tabBarIcon.height()-1);
        tabBarIconPainter.fillRect(1, 1, progress, tabBarIcon.height()-2,
                                   CommonWidget::highlightColor());

        return QIcon(QPixmap::fromImage(tabBarIcon));
    } else {
        // No simulation object currently exists for the model, so...

        return QIcon();
    }
}

//==============================================================================

void SingleCellViewWidget::fileOpened(const QString &pFileName)
{
    // Let our graphs widget know that a file has been opened

    mContentsWidget->informationWidget()->graphsWidget()->fileOpened(pFileName);
}

//==============================================================================

void SingleCellViewWidget::fileRenamed(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    // Let our graphs widget know that a file has been renamed

    mContentsWidget->informationWidget()->graphsWidget()->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void SingleCellViewWidget::on_actionRunPauseResumeSimulation_triggered()
{
    // Run or resume our simulation, or pause it

    if (mRunActionEnabled) {
        if (mSimulation->isPaused()) {
            // Our simulation is paused, so resume it

            mSimulation->resume();
        } else {
            // Protect ourselves against two successive (and very) quick
            // attempts at trying to run a simulation

            static bool handlingAction = false;

            if (handlingAction || mSimulation->isRunning())
                return;

            handlingAction = true;

            // Our simulation is not paused, so finish any editing of our
            // simulation information before running it

            mContentsWidget->informationWidget()->finishEditing();

            // Now, we would normally retrieve our simulation's properties, but
            // there is no need for it since they have already been retrieved,
            // thanks to simulationPropertyChanged()

            // Retrieve our solvers' properties
            // Note: we don't need to retrieve the NLA solver's properties since
            //       we already have them, thanks to solversPropertyChanged()...

            SingleCellViewInformationSolversWidget *solversWidget = mContentsWidget->informationWidget()->solversWidget();

            mSimulation->data()->setOdeSolverName(solversWidget->odeSolverData()->solversListProperty()->value());
            mSimulation->data()->setDaeSolverName(solversWidget->daeSolverData()->solversListProperty()->value());

            foreach (Core::Property *property, solversWidget->odeSolverData()->solversProperties().value(mSimulation->data()->odeSolverName()))
                mSimulation->data()->addOdeSolverProperty(property->id(),
                                                          (property->type() == Core::Property::Integer)?
                                                              property->integerValue():
                                                              property->doubleValue());

            foreach (Core::Property *property, solversWidget->daeSolverData()->solversProperties().value(mSimulation->data()->daeSolverName()))
                mSimulation->data()->addDaeSolverProperty(property->id(),
                                                          (property->type() == Core::Property::Integer)?
                                                              property->integerValue():
                                                              property->doubleValue());

            // Check that we have enough memory to run our simulation

            bool runSimulation = true;

            double freeMemory = Core::freeMemory();
            double requiredMemory = mSimulation->requiredMemory();

            if (requiredMemory > freeMemory) {
                QMessageBox::warning(qApp->activeWindow(), tr("Run the simulation"),
                                     tr("Sorry, but the simulation requires %1 of memory and you have only %2 left.").arg(Core::sizeAsString(requiredMemory), Core::sizeAsString(freeMemory)));
            } else {
                // Theoretically speaking, we have enough memory to run the
                // simulation, so try to allocate all the memory we need by
                // resetting our simulation settings

                runSimulation = mSimulation->results()->reset();

                qulonglong simulationResultsSize = mSimulation->results()->size();

                mOldSimulationResultsSizes.insert(mSimulation, simulationResultsSize);

                updateResults(mSimulation, simulationResultsSize);

                // Effectively run our simulation in case we were able to
                // allocate all the memory we need to run the simulation

                if (runSimulation)
                    // Now, we really run our simulation

                    mSimulation->run();
                else
                    QMessageBox::warning(qApp->activeWindow(), tr("Run the simulation"),
                                         tr("Sorry, but we could not allocate the %1 of memory required for the simulation.").arg(Core::sizeAsString(requiredMemory)));
            }

            // We are done handling the action, so...

            handlingAction = false;
        }
    } else {
        // Pause our simulation

        mSimulation->pause();
    }
}

//==============================================================================

void SingleCellViewWidget::on_actionStopSimulation_triggered()
{
    // Stop our simulation

    mSimulation->stop();
}

//==============================================================================

void SingleCellViewWidget::on_actionResetModelParameters_triggered()
{
    // Reset our simulation

    mSimulation->reset();
}

//==============================================================================

void SingleCellViewWidget::on_actionDebugMode_triggered()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellViewWidget::on_actionAddGraphPanel_triggered()
{
    // Ask our graph panels widget to add a new graph panel

    mContentsWidget->graphPanelsWidget()->addGraphPanel();
}

//==============================================================================

void SingleCellViewWidget::on_actionRemoveGraphPanel_triggered()
{
    // Default action for our removing of graph panel, i.e. remove the current
    // graph panel

    on_actionRemoveCurrentGraphPanel_triggered();
}

//==============================================================================

void SingleCellViewWidget::on_actionRemoveCurrentGraphPanel_triggered()
{
    // Ask our graph panels widget to remove the current graph panel

    mContentsWidget->graphPanelsWidget()->removeCurrentGraphPanel();
}

//==============================================================================

void SingleCellViewWidget::on_actionRemoveAllGraphPanels_triggered()
{
    // Ask our graph panels widget to remove the current graph panel

    mContentsWidget->graphPanelsWidget()->removeAllGraphPanels();
}

//==============================================================================

void SingleCellViewWidget::on_actionSimulationDataCsvExport_triggered()
{
    // Export our simulation data results to a CSV file

    QString fileName = Core::getSaveFileName(tr("Export to a CSV file"),
                                             QString(),
                                             tr("CSV File")+" (*.csv)");

    if (!fileName.isEmpty())
        mSimulation->results()->exportToCsv(fileName);
}

//==============================================================================

void SingleCellViewWidget::updateDelayValue(const double &pDelayValue)
{
    // Update our delay value widget

    mDelayValueWidget->setText(QLocale().toString(pDelayValue)+" ms");

    // Also update our simulation object

    if (mSimulation)
        mSimulation->setDelay(pDelayValue);
}

//==============================================================================

void SingleCellViewWidget::simulationRunning(const bool &pIsResuming)
{
    Q_UNUSED(pIsResuming);

    // Our simulation is running, so do a few things, but only if we are dealing
    // with the active simulation

    if (qobject_cast<SingleCellViewSimulation *>(sender()) == mSimulation) {
        // Update our simulation mode and check for results

        updateSimulationMode();

        checkResults(mSimulation);
    }
}

//==============================================================================

void SingleCellViewWidget::simulationPaused()
{
    // Our simulation is paused, so do a few things, but only if we are dealing
    // with the active simulation

    if (qobject_cast<SingleCellViewSimulation *>(sender()) == mSimulation) {
        // Update our simulation mode and parameters, and check for results

        updateSimulationMode();

        mContentsWidget->informationWidget()->parametersWidget()->updateParameters(mSimulation->currentPoint());

        checkResults(mSimulation);
    }
}

//==============================================================================

void SingleCellViewWidget::simulationStopped(const int &pElapsedTime)
{
    // We want a short delay before resetting the progress bar and the file tab
    // icon, so that the user can really see when our simulation has completed

    enum {
        ResetDelay = 169
    };

    // Our simulation worker has stopped, so do a few things, but only if we are
    // dealing with the active simulation

    SingleCellViewSimulation *simulation = qobject_cast<SingleCellViewSimulation *>(sender());

    if (simulation == mSimulation) {
        // Output the elapsed time, if valid, and reset our progress bar (with a
        // short delay)

        if (pElapsedTime != -1) {
            // We have a valid elapsed time, so show our simulation time

            SingleCellViewSimulationData *simulationData = simulation->data();
            QString solversInformation = QString();

            if (!simulationData->odeSolverName().isEmpty())
                solversInformation += simulationData->odeSolverName();
            else
                solversInformation += simulationData->daeSolverName();

            if (!simulationData->nlaSolverName().isEmpty())
                solversInformation += "+"+simulationData->nlaSolverName();

            output(QString(OutputTab+"<strong>"+tr("Simulation time:")+"</strong> <span"+OutputInfo+">"+tr("%1 s using %2").arg(QString::number(0.001*pElapsedTime, 'g', 3), solversInformation)+"</span>."+OutputBrLn));
        }

        QTimer::singleShot(ResetDelay, this, SLOT(resetProgressBar()));

        // Update our parameters and simulation mode

        updateSimulationMode();

        mContentsWidget->informationWidget()->parametersWidget()->updateParameters(simulation->currentPoint());
    }

    // Stop keeping track of our simulation progress and let people know that we
    // should reset our file tab icon, but only if we are the active simulation

    if (simulation) {
        // Stop keeping track of our simulation progress

        mProgresses.remove(simulation->fileName());

        // Reset our file tab icon (with a bit of a delay)
        // Note: we can't directly pass simulation to resetFileTabIcon(), so
        //       instead we use mStoppedSimulations which is a list of
        //       simulations in case several simulations were to stop at around
        //       the same time...

        if (simulation != mSimulation) {
            mStoppedSimulations << simulation;

            QTimer::singleShot(ResetDelay, this, SLOT(resetFileTabIcon()));
        }
    }
}

//==============================================================================

void SingleCellViewWidget::resetProgressBar()
{
    // Reset our progress bar

    mProgressBarWidget->setValue(0.0);
}

//==============================================================================

void SingleCellViewWidget::resetFileTabIcon()
{
    // Let people know that we want to reset our file tab icon

    SingleCellViewSimulation *simulation = mStoppedSimulations.first();

    mStoppedSimulations.removeFirst();

    emit updateFileTabIcon(simulation->fileName(), QIcon());
}

//==============================================================================

void SingleCellViewWidget::simulationError(const QString &pMessage,
                                           const ErrorType &pErrorType)
{
    // Output the simulation error, but only if we are dealing with the active
    // simulation or if we came here directly (i.e. not as a result of the
    // SingleCellViewSimulation::error() signal being emitted)

    SingleCellViewSimulation *simulation = qobject_cast<SingleCellViewSimulation *>(sender());

    if (!simulation || (simulation == mSimulation)) {
        mErrorType = pErrorType;

        updateInvalidModelMessageWidget();

        output(OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+pMessage+".</span>"+OutputBrLn);
    }
}

//==============================================================================

void SingleCellViewWidget::simulationDataModified(const bool &pIsModified)
{
    // Update our refresh action, but only if we are dealing with the active
    // simulation

    if (qobject_cast<SingleCellViewSimulationData *>(sender()) == mSimulation->data())
        mGui->actionResetModelParameters->setEnabled(pIsModified);
}

//==============================================================================

void SingleCellViewWidget::splitterWidgetMoved()
{
    // Our splitter has been moved, so keep track of its new sizes

    mSplitterWidgetSizes = mSplitterWidget->sizes();
}

//==============================================================================

void SingleCellViewWidget::simulationPropertyChanged(Core::Property *pProperty)
{
    // Update one of our simulation's properties and, if needed, update the
    // minimum or maximum value for our X axis
    // Note: with regards to the starting point property, we need to update it
    //       because it's can potentially have an effect on the value of our
    //       'computed constants' and 'variables'...

//---GRY---
//    bool needUpdating = true;

    SingleCellViewInformationSimulationWidget *simulationWidget = mContentsWidget->informationWidget()->simulationWidget();

    if (pProperty == simulationWidget->startingPointProperty()) {
        mSimulation->data()->setStartingPoint(pProperty->doubleValue());
    } else if (pProperty == simulationWidget->endingPointProperty()) {
        mSimulation->data()->setEndingPoint(pProperty->doubleValue());
    } else if (pProperty == simulationWidget->pointIntervalProperty()) {
        mSimulation->data()->setPointInterval(pProperty->doubleValue());

//        needUpdating = false;
    }

    // Update the minimum/maximum values of our X axis and replot ourselves, if
    // needed

//    if (needUpdating) {
//        if (mSimulation->data()->startingPoint() < mSimulation->data()->endingPoint()) {
//            mActiveGraphPanel->plot()->setMinMaxX(mSimulation->data()->startingPoint(),
//                                                  mSimulation->data()->endingPoint());
//            mActiveGraphPanel->plot()->setLocalMinMaxX(mActiveGraphPanel->plot()->minX(),
//                                                       mActiveGraphPanel->plot()->maxX());
//        } else {
//            mActiveGraphPanel->plot()->setMinMaxX(mSimulation->data()->endingPoint(),
//                                                  mSimulation->data()->startingPoint());
//            mActiveGraphPanel->plot()->setLocalMinMaxX(mActiveGraphPanel->plot()->minX(),
//                                                       mActiveGraphPanel->plot()->maxX());
//        }

//        mActiveGraphPanel->plot()->replotNow();
//    }
}

//==============================================================================

void SingleCellViewWidget::solversPropertyChanged(Core::Property *pProperty)
{
    // Check whether any of our NLA solver's properties has been modified and,
    // if so, then update our simulation data object accordingly
    // Note #1: these are the only solvers properties we need to check since
    //          they are the only ones that can potentially have an effect on
    //          the value of 'computed constants' and 'variables'...
    // Note #2: we must check that we have some NLA solver data since there may
    //          may be no NLA solver, in which case there would be no NLA solver
    //          data...

    SingleCellViewInformationSolversWidget *solversWidget = mContentsWidget->informationWidget()->solversWidget();

    if (solversWidget->nlaSolverData()) {
        if (pProperty == solversWidget->nlaSolverData()->solversListProperty())
            mSimulation->data()->setNlaSolverName(pProperty->value());
        else
            foreach (Core::Property *property, solversWidget->nlaSolverData()->solversProperties().value(mSimulation->data()->nlaSolverName()))
                if (pProperty == property) {
                    mSimulation->data()->addNlaSolverProperty(pProperty->id(),
                                                              (pProperty->type() == Core::Property::Integer)?
                                                                  pProperty->integerValue():
                                                                  pProperty->doubleValue());

                    break;
                }
    }
}

//==============================================================================

void SingleCellViewWidget::addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                                    CellMLSupport::CellmlFileRuntimeParameter *pParameterY)
{
    // Ask the current graph panel to add the required graph

    mContentsWidget->graphPanelsWidget()->activeGraphPanel()->addGraph(new SingleCellViewGraphPanelPlotGraph(pParameterX, pParameterY));
}

//==============================================================================

void SingleCellViewWidget::graphAdded(SingleCellViewGraphPanelPlotGraph *pGraph)
{
qDebug(">>> Added graph [%ld]", long(pGraph));
    // A new graph has been added, so keep track of it and update our results

    mGraphs << pGraph;

    updateResults(mSimulation, mSimulation->results()->size()/*, true*/);
}

//==============================================================================

void SingleCellViewWidget::graphRemoved(SingleCellViewGraphPanelPlotGraph *pGraph)
{
qDebug(">>> Removed graph [%ld]", long(pGraph));
    // A graph has been removed, so stop tracking it and update our results

    mGraphs.removeOne(pGraph);

    updateResults(mSimulation, mSimulation->results()->size()/*, true*/);
}

//==============================================================================

void SingleCellViewWidget::graphUpdated(SingleCellViewGraphPanelPlotGraph *pGraph)
{
qDebug(">>> Updated graph [%ld]", long(pGraph));
    // A graph has been updated, so update our results, but only if some are
    // available
    // Note: the rationale for testing for mSimulation is that upon quitting
    //       OpenCOR, some of the properties of our graphs may end up being
    //       modified (e.g. the X and Y parameters because the current file has
    //       been closed and another has been selected), resulting in our graphs
    //       widget emitting a graphUpdated() signal, which we handle here...

    if (mSimulation)
        updateResults(mSimulation, mSimulation->results()->size()/*, true*/);
}

//==============================================================================

double * SingleCellViewWidget::dataPoints(CellMLSupport::CellmlFileRuntimeParameter *pParameter) const
{
    // Return the data points associated with the given parameter

    if (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Voi)
        return mSimulation->results()->points()?mSimulation->results()->points():0;
    else if (   (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Constant)
             || (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant))
        return mSimulation->results()->constants()?mSimulation->results()->constants()[pParameter->index()]:0;
    else if (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Rate)
        return mSimulation->results()->rates()?mSimulation->results()->rates()[pParameter->index()]:0;
    else if (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::State)
        return mSimulation->results()->states()?mSimulation->results()->states()[pParameter->index()]:0;
    else if (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Algebraic)
        return mSimulation->results()->algebraic()?mSimulation->results()->algebraic()[pParameter->index()]:0;
    else
        // Undefined type

        return 0;
}

//==============================================================================

void SingleCellViewWidget::updateResults(SingleCellViewSimulation *pSimulation,
                                         const qulonglong &pSize,
                                         const bool &pReplot)
{
//---GRY--- DO WE ACTUALLY NEED THE pReplot PARAMETER?...
    // Update our graphs, if any and only if actually necessary

    SingleCellViewSimulation *simulation = pSimulation?pSimulation:mSimulation;

    // Our simulation worker has made some progress, so update our progress bar,
    // but only if we are dealing with the active simulation

    if (simulation == mSimulation) {
        // We are dealing with the active simulation, so update our graphs and
        // progress bar, and enable/disable the reset action

        // Enable/disable the reset action
        // Note: normally, our simulation worker would, for each point interval,
        //       call SingleCellViewSimulationData::checkForModifications(),
        //       but this would result in a signal being emitted (and then
        //       handled by SingleCellViewWidget::simulationDataModified()),
        //       resulting in some time overhead, so we check things here
        //       instead...

        mGui->actionResetModelParameters->setEnabled(mSimulation->data()->isModified());

        // Update our graphs, if any

Q_UNUSED(pSize);
Q_UNUSED(pReplot);
        foreach (SingleCellViewGraphPanelPlotGraph *graph, mGraphs) {
            // Show/hide the graph, depending on whether it's valid

            graph->setVisible(graph->isValid());

            // Keep track of our graph's old size

//            qulonglong oldDataSize = graphData->graph()->dataSize();

            // Update our graph's data

            if (graph->isValid())
                graph->setRawSamples(dataPoints(graph->parameterX()),
                                     dataPoints(graph->parameterY()),
                                     pSize);

            // Draw the graph's new segment, but only if there is some data to
            // plot and that we don't want to replot everything

//            if (!pReplot && (pSize > 1))
//                mActiveGraphPanel->plot()->drawGraphSegment(graphData->graph(), oldDataSize?oldDataSize-1:0, pSize-1);
        }

        // Replot our active graph panel, if needed

//        if (pReplot || (pSize <= 1))
//            // We want to initialise the plot and/or there is no data to plot,
//            // so replot our active graph panel

//            mActiveGraphPanel->plot()->replotNow();

// Replot all of our graph panels

foreach (SingleCellViewGraphPanelWidget *graphPanel, mContentsWidget->graphPanelsWidget()->graphPanels())
    graphPanel->replot();

        // Update our progress bar

        mProgressBarWidget->setValue(mSimulation->progress());
    } else {
        // We are dealing with another simulation, so simply create an icon that
        // shows the other simulation's progress and let people know about it
        // Note: we need to retrieve the name of the file associated with the
        //       other simulation since we have only one simulation object at
        //       any given time, and anyone handling the updateFileTabIcon()
        //       signal (e.g. CentralWidget) won't be able to tell for which
        //       simulation the update is...

        int oldProgress = mProgresses.value(simulation->fileName(), -1);
        int newProgress = (tabBarIconSize()-2)*simulation->progress();
        // Note: tabBarIconSize()-2 because we want a one-pixel wide
        //       border...

        if (newProgress != oldProgress) {
            // The progress has changed (or we want to force the updating of a
            // specific simulation), so keep track of its new value and update
            // our file tab icon

            mProgresses.insert(simulation->fileName(), newProgress);

            // Let people know about the file tab icon to be used for the model

            emit updateFileTabIcon(simulation->fileName(),
                                   fileTabIcon(simulation->fileName()));
        }
    }
}

//==============================================================================

void SingleCellViewWidget::checkResults(SingleCellViewSimulation *pSimulation)
{
    // Update our simulation results size

    qulonglong simulationResultsSize = pSimulation->results()->size();

    // Update our results, but only if needed

    if (simulationResultsSize != mOldSimulationResultsSizes.value(pSimulation)) {
        mOldSimulationResultsSizes.insert(pSimulation, simulationResultsSize);

        updateResults(pSimulation, simulationResultsSize);
    }

    // Ask to recheck our simulation's results, but only if our simulation is
    // still running

    if (   pSimulation->isRunning()
        || (simulationResultsSize != pSimulation->results()->size())) {
        // Note: we cannot ask QTimer::singleShot() to call checkResults() since
        //       it expects a pointer to a simulation as a parameter, so instead
        //       we call a method with no arguments which will make use of our
        //       list to know which simulation should be passed as an argument
        //       to checkResults()...

        mCheckResultsSimulations << pSimulation;

        QTimer::singleShot(0, this, SLOT(callCheckResults()));
    }
}

//==============================================================================

void SingleCellViewWidget::callCheckResults()
{
    // Retrieve the simulation for which we want to call checkResults() and then
    // call checkResults()

    SingleCellViewSimulation *simulation = mCheckResultsSimulations.first();

    mCheckResultsSimulations.removeFirst();

    checkResults(simulation);
}

//==============================================================================

QIcon SingleCellViewWidget::parameterIcon(const CellMLSupport::CellmlFileRuntimeParameter::ParameterType &pParameterType)
{
    // Return an icon that illustrates the type of a parameter

    switch (pParameterType) {
    case CellMLSupport::CellmlFileRuntimeParameter::Voi:
        return QIcon(":SingleCellView_voi");
    case CellMLSupport::CellmlFileRuntimeParameter::Constant:
        return QIcon(":SingleCellView_constant");
    case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
        return QIcon(":SingleCellView_computedConstant");
    case CellMLSupport::CellmlFileRuntimeParameter::Rate:
        return QIcon(":SingleCellView_rate");
    case CellMLSupport::CellmlFileRuntimeParameter::State:
        return QIcon(":SingleCellView_state");
    case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
        return QIcon(":SingleCellView_algebraic");
    default:
        // We are dealing with a type of parameter which is of no interest to us
        // Note: we should never reach this point...

        return QIcon(":CellMLSupport_errorNode");
    }
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

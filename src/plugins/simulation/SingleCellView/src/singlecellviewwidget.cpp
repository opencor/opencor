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
    mPlotsRects(QMap<QString, QMap<SingleCellViewGraphPanelPlotWidget *, QRectF> >()),
    mSplitterWidgetSizes(QList<int>()),
    mRunActionEnabled(true),
    mOldSimulationResultsSizes(QMap<SingleCellViewSimulation *, qulonglong>()),
    mCheckResultsSimulations(QList<SingleCellViewSimulation *>()),
    mPlots(QList<SingleCellViewGraphPanelPlotWidget *>()),
    mGraphPanelPlots(QMap<SingleCellViewGraphPanelWidget *, SingleCellViewGraphPanelPlotWidget *>()),
    mGraphPlots(QMap<SingleCellViewGraphPanelPlotGraph *, SingleCellViewGraphPanelPlotWidget *>())
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
    mToolBarWidget->addAction(mGui->actionClearSimulationData);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addWidget(mDelayWidget);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mToolBarWidget->addWidget(delaySpaceWidget);
#endif
    mToolBarWidget->addWidget(mDelayValueWidget);
/*---GRY--- DISABLED UNTIL WE ACTUALLY SUPPORT DEBUG MODE...
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
    SingleCellViewGraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();

    connect(informationWidget->simulationWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(simulationPropertyChanged(Core::Property *)));
    connect(informationWidget->solversWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(solversPropertyChanged(Core::Property *)));

    // Keep track of whether we can remove graph panels

    connect(graphPanelsWidget, SIGNAL(removeGraphPanelsEnabled(const bool &)),
            mGui->actionRemoveGraphPanel, SLOT(setEnabled(bool)));

    // Keep track of the addition and removal of a graph panel

    SingleCellViewInformationGraphsWidget *graphsWidget = informationWidget->graphsWidget();

    connect(graphPanelsWidget, SIGNAL(graphPanelAdded(SingleCellViewGraphPanelWidget *)),
            graphsWidget, SLOT(initialize(SingleCellViewGraphPanelWidget *)));
    connect(graphPanelsWidget, SIGNAL(graphPanelRemoved(SingleCellViewGraphPanelWidget *)),
            graphsWidget, SLOT(finalize(SingleCellViewGraphPanelWidget *)));

    connect(graphPanelsWidget, SIGNAL(graphPanelRemoved(SingleCellViewGraphPanelWidget *)),
            this, SLOT(graphPanelRemoved(SingleCellViewGraphPanelWidget *)));

    // Keep track of whether a graph panel has been activated

    connect(graphPanelsWidget, SIGNAL(graphPanelActivated(SingleCellViewGraphPanelWidget *)),
            graphsWidget, SLOT(initialize(SingleCellViewGraphPanelWidget *)));

    // Keep track of a graph being required

    connect(informationWidget->parametersWidget(), SIGNAL(graphRequired(CellMLSupport::CellmlFileRuntimeParameter *, CellMLSupport::CellmlFileRuntimeParameter *)),
            this, SLOT(addGraph(CellMLSupport::CellmlFileRuntimeParameter *, CellMLSupport::CellmlFileRuntimeParameter *)));

    // Keep track of the addition and removal of a graph

    connect(graphPanelsWidget, SIGNAL(graphAdded(SingleCellViewGraphPanelPlotGraph *)),
            graphsWidget, SLOT(addGraph(SingleCellViewGraphPanelPlotGraph *)));
    connect(graphPanelsWidget, SIGNAL(graphsRemoved(QList<SingleCellViewGraphPanelPlotGraph *> &)),
            graphsWidget, SLOT(removeGraphs(QList<SingleCellViewGraphPanelPlotGraph *> &)));

    connect(graphPanelsWidget, SIGNAL(graphAdded(SingleCellViewGraphPanelPlotGraph *)),
            this, SLOT(graphAdded(SingleCellViewGraphPanelPlotGraph *)));
    connect(graphPanelsWidget, SIGNAL(graphsRemoved(QList<SingleCellViewGraphPanelPlotGraph *> &)),
            this, SLOT(graphsRemoved(QList<SingleCellViewGraphPanelPlotGraph *> &)));

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

    connect(graphsWidget, SIGNAL(graphsUpdated(const QList<SingleCellViewGraphPanelPlotGraph *> &)),
            this, SLOT(graphsUpdated(const QList<SingleCellViewGraphPanelPlotGraph *> &)));

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

    // Enable/disable some actions

    mGui->actionClearSimulationData->setEnabled(    mSimulation->results()->size()
                                                && !simulationModeEnabled);
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
    // Stop keeping track of certain things (so that updatePlot() doesn't get
    // called unnecessarily)
    // Note: see the corresponding code at the end of this method...

    SingleCellViewInformationWidget *informationWidget = mContentsWidget->informationWidget();

    disconnect(informationWidget->simulationWidget(), SIGNAL(propertyChanged(Core::Property *)),
               this, SLOT(simulationPropertyChanged(Core::Property *)));

    // Keep track of our simulation data for our previous model and finalise a
    // few things, if needed

    SingleCellViewSimulation *previousSimulation = mSimulation;
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

        // Keep track of the axes' values of the different plots

        QMap<SingleCellViewGraphPanelPlotWidget *, QRectF> plotsRects = QMap<SingleCellViewGraphPanelPlotWidget *, QRectF>();

        foreach (SingleCellViewGraphPanelPlotWidget *plot, mPlots)
            plotsRects.insert(plot, QRectF(QPointF(plot->minX(), plot->minY()),
                                           QPointF(plot->maxX(), plot->maxY())));

        mPlotsRects.insert(previousFileName, plotsRects);
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

    // Update our plots (so that all of our graphs are up to date)

    if (validSimulationEnvironment) {
        QMap<SingleCellViewGraphPanelPlotWidget *, QRectF> plotsRects = mPlotsRects.value(pFileName, QMap<SingleCellViewGraphPanelPlotWidget *, QRectF>());

        foreach (SingleCellViewGraphPanelPlotWidget *plot, mPlots) {
            QRectF dataRect = plotsRects.value(plot);

            if (dataRect.isNull())
                updatePlot(plot, true);
            else if (!plot->setAxes(dataRect))
                plot->replotNow();
        }
    }

    // If we have a valid simulation environment and we are dealing with a new
    // simulation, then reset both the simulation's data and results (well,
    // initialise in the case of its data)

    if (validSimulationEnvironment && newSimulation) {
        mSimulation->data()->reset();
        mSimulation->results()->reset(false);
    }

    // Resume the tracking of certain things
    // Note: see the corresponding code at the beginning of this method...

    connect(informationWidget->simulationWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(simulationPropertyChanged(Core::Property *)));
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

    mPlotsRects.remove(pFileName);

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
                // Note: to call updateResults() will effectively reset our
                //       graphs...

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
    // Reset our model parameters

    mSimulation->reset();
}

//==============================================================================

void SingleCellViewWidget::on_actionClearSimulationData_triggered()
{
    // Clear our simulation data

    mSimulation->results()->reset(false);

    // Update our simulation mode and check for results

    updateSimulationMode();

    checkResults(mSimulation);
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
    // Update one of our simulation's properties

    SingleCellViewInformationSimulationWidget *simulationWidget = mContentsWidget->informationWidget()->simulationWidget();
    bool needUpdatePlots = true;

    if (pProperty == simulationWidget->startingPointProperty()) {
        mSimulation->data()->setStartingPoint(pProperty->doubleValue());
    } else if (pProperty == simulationWidget->endingPointProperty()) {
        mSimulation->data()->setEndingPoint(pProperty->doubleValue());
    } else if (pProperty == simulationWidget->pointIntervalProperty()) {
        mSimulation->data()->setPointInterval(pProperty->doubleValue());

        needUpdatePlots = false;
    }

    // Now, update our plots, if needed

    if (needUpdatePlots)
        foreach (SingleCellViewGraphPanelPlotWidget *plot, mPlots)
            updatePlot(plot);
}

//==============================================================================

void SingleCellViewWidget::solversPropertyChanged(Core::Property *pProperty)
{
    // Check whether any of our NLA solver's properties has been modified and,
    // if so, then update our simulation data object accordingly
    // Note #1: we only need to check our NLA solver's properties since they are
    //          the only ones that can potentially have an effect on the value
    //          of our 'computed constants' and 'variables'...
    // Note #2: we must check that we have some NLA solver data since there may
    //          may be no NLA solver (and therefore no NLA solver data)...

    SingleCellViewInformationSolversWidgetData *nlaSolverData = mContentsWidget->informationWidget()->solversWidget()->nlaSolverData();

    if (nlaSolverData) {
        if (pProperty == nlaSolverData->solversListProperty())
            // The property for selecting a particular NLA solver

            mSimulation->data()->setNlaSolverName(pProperty->value());
        else
            // We are dealing with one of the selected NLA solver's properties,
            // so go through them and check which one it is

            foreach (Core::Property *property, nlaSolverData->solversProperties().value(mSimulation->data()->nlaSolverName()))
                if (pProperty == property) {
                    // We have found the NLA solver's property that got changed,
                    // so keep track of the new value

                    mSimulation->data()->addNlaSolverProperty(pProperty->id(),
                                                              (pProperty->type() == Core::Property::Integer)?
                                                                  pProperty->integerValue():
                                                                  pProperty->doubleValue());

                    break;
                }
    }
}

//==============================================================================

void SingleCellViewWidget::graphPanelRemoved(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    // A graph panel has been removed, so stop tracking its plot

    mPlots.removeOne(mGraphPanelPlots.value(pGraphPanel));

    mGraphPanelPlots.remove(pGraphPanel);
}

//==============================================================================

void SingleCellViewWidget::addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                                    CellMLSupport::CellmlFileRuntimeParameter *pParameterY)
{
    // Ask the current graph panel to add a new graph for the given parameters

    mContentsWidget->graphPanelsWidget()->activeGraphPanel()->addGraph(new SingleCellViewGraphPanelPlotGraph(pParameterX, pParameterY));
}

//==============================================================================

void SingleCellViewWidget::graphAdded(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // A new graph has been added, so keep track of it and update its
    // corresponding plot

    SingleCellViewGraphPanelPlotWidget *plot = qobject_cast<SingleCellViewGraphPanelPlotWidget *>(pGraph->plot());

    mGraphPlots.insert(pGraph, plot);

    updateGraphData(pGraph, mSimulations.value(pGraph->fileName())->results()->size());
    updatePlot(plot);

    plot->drawGraphSegment(pGraph, 0, -1);

    // Keep track of the plot itself, if needed

    if (!mPlots.contains(plot)) {
        mPlots << plot;

        mGraphPanelPlots.insert(qobject_cast<SingleCellViewGraphPanelWidget *>(plot->parentWidget()), plot);
    }
}

//==============================================================================

void SingleCellViewWidget::graphsRemoved(QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs)
{
    // One or several graphs have been removed, so update and stop tracking (if
    // needed) their corresponding plots
    // Note: pGraph points to a graph which has already been deleted, hence we
    //       retrieve its 'old' plot from mGraphPlots

    QList<SingleCellViewGraphPanelPlotWidget *> plots = QList<SingleCellViewGraphPanelPlotWidget *>();

    foreach (SingleCellViewGraphPanelPlotGraph *graph, pGraphs) {
        // Keep track of the plot that we will need to update
        // Note: normally, the plot associated with a graph should always be the
        //       same for all the graphs, but we never know we might, in the
        //       future, come here with graphs from different plots...

        SingleCellViewGraphPanelPlotWidget *plot = mGraphPlots.value(graph);

        if (!plots.contains(plot))
            plots << plot;

        // Stop tracking the plot associated with the graph and stop tracking
        // the plot itself, if it doesn't have any graph anymore

        mGraphPlots.remove(graph);

        if (plot->graphs().isEmpty())
            mPlots.removeOne(plot);
    }

    // Update the relevant plots

    foreach (SingleCellViewGraphPanelPlotWidget *plot, plots)
        updatePlot(plot, true);
        // Note: even if the axes' values of the plot haven't changed, we still
        //       want to replot the plot since at least one of its graphs has
        //       been removed...
}

//==============================================================================

void SingleCellViewWidget::graphsUpdated(const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs)
{
    // One or several graphs have been updated, so make sure that their
    // corresponding plots are up to date

    QList<SingleCellViewGraphPanelPlotWidget *> plots = QList<SingleCellViewGraphPanelPlotWidget *>();

    foreach (SingleCellViewGraphPanelPlotGraph *graph, pGraphs) {
        // Show/hide the graph

        graph->setVisible(graph->isValid() && graph->isSelected());

        // Update the graph's data and replot it
        // Note: it may happen that we don't have a simulation associated with the
        //       given graph, hence we must check for it. Indeed, say that you have
        //       two files opened, but only one has been selected so far. From
        //       there, say you create a graph and then double click on its model
        //       property so that the other file gets selected. In this case, for
        //       example, there won't be a simulation associated with the file and
        //       therefore the graph...

        SingleCellViewSimulation *simulation = mSimulations.value(graph->fileName());

        if (simulation) {
            // Update the graph's data
            // Note: we don't want graph segments to be drawn (hence we pass
            //       false to updateGraph()) since we come here as a result of a
            //       graph being un/selected, or as a result of its model, X or
            //       Y parameter being updated, meaning that we have to replot
            //       everything...

            updateGraph(graph, simulation->results()->size(), false);

            // Keep track of the plot that we will need to update and replot
            // Note: see the corresponding comment in
            //       SingleCellViewWidget::graphsRemoved() above...

            SingleCellViewGraphPanelPlotWidget *plot = qobject_cast<SingleCellViewGraphPanelPlotWidget *>(graph->plot());

            if (!plots.contains(plot))
                plots << plot;
        }
    }

    // Update and replot our various plots

    foreach (SingleCellViewGraphPanelPlotWidget *plot, plots)
        updatePlot(plot, true);
        // Note: even if the axes' values of the plot haven't changed, we still
        //       want to replot the plot since at least one of its graphs has
        //       been updated...
}

//==============================================================================

void SingleCellViewWidget::checkAxisValue(double &pValue,
                                          const double &pOrigValue,
                                          const QList<double> &pTestValues)
{
    // Check whether pOrigValue is equal to one of the values in pTestValues and
    // if so then update pValue with pOrigValue

    foreach (const double &testValue, pTestValues)
        if (pOrigValue == testValue) {
            pValue = pOrigValue;

            break;
        }
}

//==============================================================================

bool SingleCellViewWidget::updatePlot(SingleCellViewGraphPanelPlotWidget *pPlot,
                                      const bool &pForceReplot)
{
    // Retrieve the current axes' values or use some default ones, if none are
    // available

    bool hasAxesValues = false;

    double minX =    0.0;
    double maxX = 1000.0;
    double minY =    0.0;
    double maxY = 1000.0;

    QRectF dataRect = pPlot->dataRect();

    if (!dataRect.isNull()) {
        minX = dataRect.left();
        maxX = minX+dataRect.width();
        minY = dataRect.top();
        maxY = minY+dataRect.height();

        hasAxesValues = true;
    }

    // Check all the graphs associated with the given plot and see whether any
    // of them uses the variable of integration as parameter X and/or Y, and if
    // so then asks the plot to use the minimum/maximum points as the
    // minimum/maximum values for the X and/or Y axes

    bool needInitialisationX = true;
    bool needInitialisationY = true;

    bool canOptimiseAxisX = true;
    bool canOptimiseAxisY = true;

    QList<double> startingPoints = QList<double>();
    QList<double> endingPoints = QList<double>();

    foreach (SingleCellViewGraphPanelPlotGraph *graph, pPlot->graphs())
        if (graph->isValid() && graph->isSelected()) {
            SingleCellViewSimulation *simulation = mSimulations.value(graph->fileName());

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

            if (graph->parameterX()->type() == CellMLSupport::CellmlFileRuntimeParameter::Voi) {
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

            if (graph->parameterY()->type() == CellMLSupport::CellmlFileRuntimeParameter::Voi)
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

    // Set our axes' values and replot the plot, if needed

    if (pPlot->setAxes(minX, maxX, minY, maxY)) {
        return true;
    } else if (pForceReplot) {
        pPlot->replotNow();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

double * SingleCellViewWidget::dataPoints(SingleCellViewSimulation *pSimulation,
                                          CellMLSupport::CellmlFileRuntimeParameter *pParameter) const
{
    // Return the array of data points associated with the given simulation and
    // parameter

    if (!pSimulation || !pParameter)
        return 0;

    if (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Voi)
        return pSimulation->results()->points()?pSimulation->results()->points():0;
    else if (   (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Constant)
             || (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant))
        return pSimulation->results()->constants()?pSimulation->results()->constants()[pParameter->index()]:0;
    else if (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Rate)
        return pSimulation->results()->rates()?pSimulation->results()->rates()[pParameter->index()]:0;
    else if (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::State)
        return pSimulation->results()->states()?pSimulation->results()->states()[pParameter->index()]:0;
    else if (pParameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Algebraic)
        return pSimulation->results()->algebraic()?pSimulation->results()->algebraic()[pParameter->index()]:0;
    else
        // Undefined type

        return 0;
}

//==============================================================================

void SingleCellViewWidget::updateGraphData(SingleCellViewGraphPanelPlotGraph *pGraph,
                                           const qulonglong &pSize)
{
    // Update our graph's data

    if (pGraph->isValid()) {
        SingleCellViewSimulation *simulation = mSimulations.value(pGraph->fileName());

        pGraph->setRawSamples(dataPoints(simulation, pGraph->parameterX()),
                              dataPoints(simulation, pGraph->parameterY()),
                              pSize);
    }
}

//==============================================================================

void SingleCellViewWidget::updateGraph(SingleCellViewGraphPanelPlotGraph *pGraph,
                                       const qulonglong &pSize,
                                       const bool &pDrawGraphSegment)
{
    // Keep track of our graph's old size

    qulonglong oldDataSize = pGraph->dataSize();

    // Update our graph's data

    updateGraphData(pGraph, pSize);

    // Draw the graph's new segment, but only if required and if there is some
    // data to plot and the graph is visible

    if (pDrawGraphSegment && pSize && pGraph->isVisible()) {
        SingleCellViewGraphPanelPlotWidget *plot = qobject_cast<SingleCellViewGraphPanelPlotWidget *>(pGraph->plot());

        plot->drawGraphSegment(pGraph, oldDataSize?oldDataSize-1:0, pSize-1);
    }
}

//==============================================================================

void SingleCellViewWidget::updateResults(SingleCellViewSimulation *pSimulation,
                                         const qulonglong &pSize)
{
    // Enable/disable the reset action, in case we are dealing with the active
    // simulation
    // Note: normally, our simulation worker would, for each point interval,
    //       call SingleCellViewSimulationData::checkForModifications(), but
    //       this would result in a signal being emitted (and then handled by
    //       SingleCellViewWidget::simulationDataModified()), resulting in some
    //       time overhead, so we check things here instead...

    if (pSimulation == mSimulation)
        mGui->actionResetModelParameters->setEnabled(pSimulation->data()->isModified());

    // Update all the graphs associated with the given simulation
    // Note: if pSize is greater than zero, then the graphs will be individually
    //       updated. On the other hand, if we have a pSize value of zero (e.g.
    //       when starting a simulation), then we ask the plot to directly
    //       replot itself...

    foreach (SingleCellViewGraphPanelPlotWidget *plot, mPlots) {
        foreach (SingleCellViewGraphPanelPlotGraph *graph, plot->graphs())
            if (!graph->fileName().compare(pSimulation->fileName()))
                updateGraph(graph, pSize);

        if (!pSize)
            plot->replotNow();
    }

    // Update our progress bar (or the tab icon, in case we are not dealing with
    // the active simulation)

    if (pSimulation == mSimulation) {
        mProgressBarWidget->setValue(mSimulation->progress());
    } else {
        // We are not dealing with the active simulation, so create an icon that
        // shows the simulation's progress and let people know about it
        // Note: we need to retrieve the name of the file associated with the
        //       other simulation since we have only one simulation object at
        //       any given time, and anyone handling the updateFileTabIcon()
        //       signal (e.g. CentralWidget) won't be able to tell for which
        //       simulation the update is...

        int oldProgress = mProgresses.value(pSimulation->fileName(), -1);
        int newProgress = (tabBarIconSize()-2)*pSimulation->progress();
        // Note: tabBarIconSize()-2 because we want a one-pixel wide border...

        if (newProgress != oldProgress) {
            // The progress has changed, so keep track of its new value and
            // update our file tab icon

            mProgresses.insert(pSimulation->fileName(), newProgress);

            // Let people know about the file tab icon to be used for the model

            emit updateFileTabIcon(pSimulation->fileName(),
                                   fileTabIcon(pSimulation->fileName()));
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
    // call checkResults() for it

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

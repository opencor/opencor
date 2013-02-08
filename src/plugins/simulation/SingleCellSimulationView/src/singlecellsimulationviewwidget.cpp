//==============================================================================
// Single cell simulation view widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlfileruntime.h"
#include "coreutils.h"
#include "progressbarwidget.h"
#include "propertyeditorwidget.h"
#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewgraphpanelswidget.h"
#include "singlecellsimulationviewgraphpanelwidget.h"
#include "singlecellsimulationviewinformationparameterswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationsolverswidget.h"
#include "singlecellsimulationviewinformationtraceswidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewplugin.h"
#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewwidget.h"
#include "toolbarwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_singlecellsimulationviewwidget.h"

//==============================================================================

#include <QBrush>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QScrollBar>
#include <QSettings>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include <QVariant>

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_wheel.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

static const QString OutputTab  = "&nbsp;&nbsp;&nbsp;&nbsp;";
static const QString OutputGood = " style=\"color: green;\"";
static const QString OutputInfo = " style=\"color: navy;\"";
static const QString OutputBad  = " style=\"color: maroon;\"";
static const QString OutputBrLn = "<br/>\n";

//==============================================================================

SingleCellSimulationViewWidget::SingleCellSimulationViewWidget(SingleCellSimulationViewPlugin *pPluginParent,
                                                               QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::SingleCellSimulationViewWidget),
    mPluginParent(pPluginParent),
    mSimulation(0),
    mSimulations(QMap<QString, SingleCellSimulationViewSimulation *>()),
    mStoppedSimulations(QList<SingleCellSimulationViewSimulation *>()),
    mDelays(QMap<QString, int>()),
    mSplitterWidgetSizes(QList<int>()),
    mProgresses(QMap<QString, int>()),
mTraces(QMap<QString, QwtPlotCurve *>())
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...
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

    setDelayValue(0);

    // Create a tool bar widget with different buttons

    mToolBarWidget = new Core::ToolBarWidget(this);

    mToolBarWidget->addAction(mGui->actionRun);
    mToolBarWidget->addAction(mGui->actionPause);
    mToolBarWidget->addAction(mGui->actionStop);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addWidget(mDelayWidget);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mToolBarWidget->addWidget(delaySpaceWidget);
#endif
    mToolBarWidget->addWidget(mDelayValueWidget);
//    mToolBarWidget->addSeparator();
//    mToolBarWidget->addAction(mGui->actionDebugMode);
//    mToolBarWidget->addSeparator();
//    mToolBarWidget->addAction(mGui->actionAdd);
//    mToolBarWidget->addAction(mGui->actionRemove);
    //---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mGui->actionCsvExport);

    mTopSeparator = Core::newLineWidget(this);

    mGui->layout->addWidget(mToolBarWidget);
    mGui->layout->addWidget(mTopSeparator);

    // Create our splitter widget and keep track of its movement
    // Note: we need to keep track of its movement so that saveSettings() can
    //       work fine even when mContentsWidget is not visible (which happens
    //       when a CellML file cannot be run for some reason or another)...

    mSplitterWidget = new QSplitter(Qt::Vertical, this);

    connect(mSplitterWidget, SIGNAL(splitterMoved(int,int)),
            this, SLOT(splitterWidgetMoved()));

    // Create our contents widget

    mContentsWidget = new SingleCellSimulationViewContentsWidget(this);

    mContentsWidget->setObjectName("Contents");

    // Keep track of changes to some of our simulation properties

    connect(mContentsWidget->informationWidget()->simulationWidget(), SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(simulationPropertyChanged(Core::Property *)));

    // Keep track of whether we can remove graph panels

    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(removeGraphPanelsEnabled(const bool &)),
            mGui->actionRemove, SLOT(setEnabled(bool)));
connect(mContentsWidget->informationWidget()->parametersWidget(), SIGNAL(parameterNeeded(const QString &,
                                                                                         CellMLSupport::CellmlFileRuntimeModelParameter *,
                                                                                         const bool &)),
        this, SLOT(parameterNeeded(const QString &,
                                   CellMLSupport::CellmlFileRuntimeModelParameter *,
                                   const bool &)));
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...

    // Create and add our invalid simulation message widget

    mInvalidModelMessageWidget = new Core::UserMessageWidget(":/oxygen/actions/help-about.png",
                                                             pParent);

    mGui->layout->addWidget(mInvalidModelMessageWidget);

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

    mOutputWidget->setFrameStyle(QFrame::NoFrame);

    simulationOutputLayout->addWidget(Core::newLineWidget(this));
    simulationOutputLayout->addWidget(mOutputWidget);

    // Populate our splitter and use as much space as possible for it by asking
    // for its height to be that of the desktop's, and then add our splitter to
    // our single cell simulation view widget

    mSplitterWidget->addWidget(mContentsWidget);
    mSplitterWidget->addWidget(simulationOutputWidget);

    mSplitterWidget->setSizes(QList<int>() << qApp->desktop()->screenGeometry().height() << 1);

    mGui->layout->addWidget(mSplitterWidget);

    // Create our (thin) simulation progress widget

    mProgressBarWidget = new Core::ProgressBarWidget(this);

    mProgressBarWidget->setFixedHeight(3);
    mProgressBarWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    mBottomSeparator = Core::newLineWidget(this);

    mGui->layout->addWidget(mBottomSeparator);
    mGui->layout->addWidget(mProgressBarWidget);

    // Make our contents widget our focus proxy

    setFocusProxy(mContentsWidget);
}

//==============================================================================

SingleCellSimulationViewWidget::~SingleCellSimulationViewWidget()
{
    // Delete our simulation objects

    foreach (SingleCellSimulationViewSimulation *simulation, mSimulations)
        delete simulation;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate our delay and delay value widgets

    mDelayWidget->setToolTip(tr("Delay"));
    mDelayValueWidget->setToolTip(mDelayWidget->toolTip());

    mDelayWidget->setStatusTip(tr("Delay in milliseconds between two data points"));
    mDelayValueWidget->setStatusTip(mDelayWidget->statusTip());

    // Retranslate our invalid model message

    updateInvalidModelMessageWidget();

    // Retranslate our contents widget

    mContentsWidget->retranslateUi();
}

//==============================================================================

static const QString SettingsSizesCount = "SizesCount";
static const QString SettingsSize       = "Size%1";

//==============================================================================

void SingleCellSimulationViewWidget::loadSettings(QSettings *pSettings)
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
mActiveGraphPanel = mContentsWidget->graphPanelsWidget()->activeGraphPanel();
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...
}

//==============================================================================

void SingleCellSimulationViewWidget::saveSettings(QSettings *pSettings) const
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

void SingleCellSimulationViewWidget::setSolverInterfaces(const SolverInterfaces &pSolverInterfaces)
{
    // Let our solvers widget know about the solver interfaces

    mContentsWidget->informationWidget()->solversWidget()->setSolverInterfaces(pSolverInterfaces);

    // Keep track of the solver interfaces

    mSolverInterfaces = pSolverInterfaces;
}

//==============================================================================

void SingleCellSimulationViewWidget::clearGraphPanels()
{
    // Ask our graph panels widget to clear all the graph panels

    mContentsWidget->graphPanelsWidget()->clearGraphPanels();
}

//==============================================================================

void SingleCellSimulationViewWidget::clearActiveGraphPanel()
{
    // Ask our graph panels widget to clear the current graph panel

    mContentsWidget->graphPanelsWidget()->clearActiveGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewWidget::output(const QString &pMessage)
{
    // Move to the end of the output (just in case...)

    mOutputWidget->moveCursor(QTextCursor::End);

    // Make sure that the output ends as expected and if not then add BrLn to it

    static const QString EndOfOutput = "<br /></p></body></html>";

    if (mOutputWidget->toHtml().right(EndOfOutput.size()).compare(EndOfOutput))
        mOutputWidget->insertHtml(OutputBrLn);

    // Output the message and make sure that it's visible

    mOutputWidget->insertHtml(pMessage);
    mOutputWidget->ensureCursorVisible();
}

//==============================================================================

void SingleCellSimulationViewWidget::setSimulationMode(const bool &pEnabled,
                                                       const bool &pRunVisible)
{
    // Show/hide our run and pause actions

    mGui->actionRun->setVisible(pRunVisible);
    mGui->actionPause->setVisible(!pRunVisible);

    // Enable/disable our stop action

    mGui->actionStop->setEnabled(pEnabled);

    // Enable or disable the simulation and solvers widgets

    mContentsWidget->informationWidget()->simulationWidget()->setEnabled(!pEnabled);
    mContentsWidget->informationWidget()->solversWidget()->setEnabled(!pEnabled);

    // Give the focus to our focus proxy, in case we leave the simulation mode
    // (so that the user can modify simulation data, etc.)

    if (!pEnabled)
        focusProxy()->setFocus();
}

//==============================================================================

void SingleCellSimulationViewWidget::updateInvalidModelMessageWidget()
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

void SingleCellSimulationViewWidget::initialize(const QString &pFileName)
{
    // Keep track of our simulation data for our previous model and finalise a
    // few things, if needed

    SingleCellSimulationViewSimulation *previousSimulation = mSimulation;
    SingleCellSimulationViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
    SingleCellSimulationViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();
    SingleCellSimulationViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();
    SingleCellSimulationViewInformationParametersWidget *parametersWidget = informationWidget->parametersWidget();

    if (previousSimulation) {
        // There is a previous simulation, so finalise a few things

        QString previousFileName = previousSimulation->fileName();

        simulationWidget->finalize(previousFileName);
        solversWidget->finalize(previousFileName);

        // Keep track of the value of the delay widget

        mDelays.insert(previousFileName, mDelayWidget->value());
    }

    // Retrieve the value of our delay widget

    setDelayValue( mDelays.value(pFileName, 0));

    // Retrieve our simulation object for the current model, if any

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);
    CellMLSupport::CellmlFileRuntime *cellmlFileRuntime = cellmlFile->runtime();

    mSimulation = mSimulations.value(pFileName);

    if (!mSimulation) {
        // No simulation object currently exists for the model, so create one

        mSimulation = new SingleCellSimulationViewSimulation(pFileName, cellmlFileRuntime);

        // Set our simulation object's delay

        mSimulation->setDelay(mDelayWidget->value());

        // Create a few connections

        connect(mSimulation, SIGNAL(running()),
                this, SLOT(simulationRunning()));
        connect(mSimulation, SIGNAL(pausing()),
                this, SLOT(simulationPausing()));
        connect(mSimulation, SIGNAL(stopped(const int &)),
                this, SLOT(simulationStopped(const int &)));

        connect(mSimulation, SIGNAL(progress(const double &)),
                this, SLOT(simulationProgress(const double &)));

        connect(mSimulation, SIGNAL(error(const QString &)),
                this, SLOT(simulationError(const QString &)));

connect(mSimulation->data(), SIGNAL(results(SingleCellSimulationViewSimulationDataResults *)),
        this, SLOT(results(SingleCellSimulationViewSimulationDataResults *)));
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...
        // Keep track of our simulation object

        mSimulations.insert(pFileName, mSimulation);
    }

    // Reset our file tab icon and stop tracking its simulation progress (in
    // case a simulation is running)

    if (mSimulation->workerStatus() != SingleCellSimulationViewSimulationWorker::Unknown) {
        mProgresses.remove(mSimulation->fileName());

        emit updateFileTabIcon(mSimulation->fileName(), QIcon());
    }

    // Output some information about our CellML file

    QString information = QString();

    if (!mOutputWidget->document()->isEmpty())
        information += "<hr/>\n";

    information += "<strong>"+pFileName+"</strong>"+OutputBrLn;
    information += OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

    bool validCellmlFileRuntime = cellmlFileRuntime && cellmlFileRuntime->isValid();

    if (validCellmlFileRuntime) {
        // A valid runtime could be retrieved for the CellML file

        QString additionalInformation = QString();

        if (cellmlFileRuntime->needNlaSolver())
            additionalInformation = " + "+tr("Non-linear algebraic system(s)");

        information += "<span"+OutputGood+">"+tr("valid")+"</span>."+OutputBrLn;
        information += QString(OutputTab+"<strong>"+tr("Model type:")+"</strong> <span"+OutputInfo+">%1%2</span>."+OutputBrLn).arg((cellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?tr("ODE"):tr("DAE"),
                                                                                                                                   additionalInformation);
    } else {
        // We couldn't retrieve a runtime for the CellML file or we could, but
        // it's not valid

        information += "<span"+OutputBad+">"+(cellmlFileRuntime?tr("invalid"):tr("none"))+"</span>."+OutputBrLn;

        foreach (const CellMLSupport::CellmlFileIssue &issue,
                 cellmlFileRuntime?cellmlFileRuntime->issues():cellmlFile->issues())
            information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2</span>."+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?tr("Error:"):tr("Warning:"),
                                                                                                                 issue.message());
    }

    output(information);

    // Retrieve our variable of integration, if any

    CellMLSupport::CellmlFileRuntimeModelParameter *variableOfIntegration = validCellmlFileRuntime?cellmlFileRuntime->variableOfIntegration():0;

    // Enable/disable our run action depending on whether we have a variable of
    // integration

    mGui->actionRun->setEnabled(variableOfIntegration);

    // Set our simulation mode

    setSimulationMode(   (mSimulation->workerStatus() == SingleCellSimulationViewSimulationWorker::Running)
                      || (mSimulation->workerStatus() == SingleCellSimulationViewSimulationWorker::Pausing),
                         (mSimulation->workerStatus() == SingleCellSimulationViewSimulationWorker::Unknown)
                      || (mSimulation->workerStatus() == SingleCellSimulationViewSimulationWorker::Pausing));

    // Update our previous (if any) and current simulation progresses

    if (   previousSimulation
        && (previousSimulation->workerStatus() != SingleCellSimulationViewSimulationWorker::Unknown))
        simulationProgress(previousSimulation->workerProgress(), previousSimulation);

    simulationProgress(mSimulation->workerProgress());

    // Check that we have a valid runtime

    bool hasError = true;

    if (validCellmlFileRuntime) {
        // We have a valid runtime
        // Note: if we didn't have a valid runtime, then there would be no need
        //       to output an error message since one would have already been
        //       generated while trying to get the runtime (see above)...

        // Retrieve the unit of the variable of integration, if any

        if (!variableOfIntegration) {
            // We don't have a variable of integration, so...

            simulationError(tr("the model must have at least one ODE or DAE"),
                            InvalidCellmlFile);
        } else {
            // Show our contents widget in case it got previously hidden
            // Note: indeed, if it was to remain hidden then some
            //       initialisations wouldn't work (e.g. the solvers widget has
            //       property editor which all properties need to be removed and
            //       if the contents widget is not visible, then upon
            //       repopulating the property editor, scrollbars will be shown
            //       even though they are not needed), so...

            mContentsWidget->setVisible(true);

            // Initialise our GUI's simulation, solvers and parameters widgets

            simulationWidget->initialize(pFileName, cellmlFileRuntime);
            solversWidget->initialize(pFileName, cellmlFileRuntime);
            parametersWidget->initialize(pFileName, cellmlFileRuntime, mSimulation->data());

#ifdef QT_DEBUG
            // Output the type of solvers that are available to run the model

            qDebug("---------------------------------------");
            qDebug("%s", qPrintable(pFileName));

            information = QString();

            if (cellmlFileRuntime->needOdeSolver()) {
                information += "\n - ODE solver(s): ";

                int solverCounter = 0;

                foreach (const QString &odeSolver, solversWidget->odeSolvers())
                    if (++solverCounter == 1)
                        information += odeSolver;
                    else
                        information += " | "+odeSolver;

                if (!solverCounter)
                    information += "none available";
            }

            if (cellmlFileRuntime->needDaeSolver()) {
                information += "\n - DAE solver(s): ";

                int solverCounter = 0;

                foreach (const QString &daeSolver, solversWidget->daeSolvers())
                    if (++solverCounter == 1)
                        information += daeSolver;
                    else
                        information += " | "+daeSolver;

                if (!solverCounter)
                    information += "none available";
            }

            if (cellmlFileRuntime->needNlaSolver()) {
                information += "\n - NLA solver(s): ";

                int solverCounter = 0;

                foreach (const QString &nlaSolver, solversWidget->nlaSolvers())
                    if (++solverCounter == 1)
                        information += nlaSolver;
                    else
                        information += " | "+nlaSolver;

                if (!solverCounter)
                    information += "none available";
            }

            qDebug("%s", qPrintable(information.remove(0, 1)));
            // Note: we must remove the leading '\n'...
#endif

            // Check whether we have at least one ODE or DAE solver and, if
            // needed, at least one NLA solver

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
                    // We have the solver(s) we need, so...

                    hasError = false;
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
                // We have the solver(s) we need, so...

                hasError = false;
            }
        }
    }

    // Check if an error occurred and, if so, show/hide some widgets

    bool previousHasError = mInvalidModelMessageWidget->isVisible();

    mToolBarWidget->setVisible(!hasError);
    mTopSeparator->setVisible(!hasError);

    mContentsWidget->setVisible(!hasError);
    mInvalidModelMessageWidget->setVisible(hasError);

    mBottomSeparator->setVisible(!hasError);
    mProgressBarWidget->setVisible(!hasError);

    // Make sure that the last output message is visible
    // Note: indeed, to (re)show some widgets (see above) will change the height
    //       of our output widget, messing up the vertical scroll bar a bit (if
    //       visible), resulting in the output being shifted a bit, so...

    if (previousHasError != hasError) {
        qApp->processEvents();

        mOutputWidget->ensureCursorVisible();
    }

    // If no error occurred, then reset our simulation, so that its data gets
    // reset (initialised)

    if (!hasError)
        mSimulation->reset();

//---GRY--- THE BELOW IS TEMPORARY, JUST FOR OUR DEMO...
QMap<QString, QwtPlotCurve *>::const_iterator iter = mTraces.constBegin();

while (iter != mTraces.constEnd()) {
    // Retrieve the file name associated with the trace

    QString fileName = iter.key();

    fileName.chop(fileName.size()-fileName.indexOf('|'));

    // Show/hide our trace depending on whether it is associated with the
    // requested file name

    iter.value()->setVisible(!fileName.compare(pFileName));

    // Go to the next trace

    ++iter;
}

mActiveGraphPanel->plot()->setAxisScale(QwtPlot::xBottom, mSimulation->data()->startingPoint(), mSimulation->data()->endingPoint());
}

//==============================================================================

bool SingleCellSimulationViewWidget::isManaged(const QString &pFileName) const
{
    // Return whether the given file name is managed

    return mSimulations.value(pFileName);
}

//==============================================================================

void SingleCellSimulationViewWidget::finalize(const QString &pFileName)
{
    // Remove the simulation object, should there be one for the given file name

    SingleCellSimulationViewSimulation *simulation = mSimulations.value(pFileName);

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
}

//==============================================================================

int SingleCellSimulationViewWidget::tabBarIconSize() const
{
    // Return the size of a file tab icon

    return style()->pixelMetric(QStyle::PM_TabBarIconSize, 0, this);
}

//==============================================================================

QIcon SingleCellSimulationViewWidget::fileTabIcon(const QString &pFileName) const
{
    // Return the requested file tab icon

    SingleCellSimulationViewSimulation *simulation = mSimulations.value(pFileName);
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

void SingleCellSimulationViewWidget::on_actionRun_triggered()
{
    // Make sure that we have a simulation

    if (!mSimulation)
        return;

    // Get ready for the simulation
    // Note: there are a few things that we need to do before running the
    //       simulation, but those things don't need to be done again when
    //       resuming a simulation...

    if (mSimulation->workerStatus() == SingleCellSimulationViewSimulationWorker::Unknown) {
        // Cancel any editing of our simulation information

        mContentsWidget->informationWidget()->cancelEditing();

        // Retrieve our simulation properties
        // Note: we don't need to retrieve the value of our starting point value
        //       since we already have it (see simulationPropertyChanged())...

        SingleCellSimulationViewSimulationData *simulationData = mSimulation->data();
        SingleCellSimulationViewInformationSimulationWidget *simulationWidget = mContentsWidget->informationWidget()->simulationWidget();

        simulationData->setEndingPoint(Core::PropertyEditorWidget::doublePropertyItem(simulationWidget->endingPointProperty()->value()));
        simulationData->setPointInterval(Core::PropertyEditorWidget::doublePropertyItem(simulationWidget->pointIntervalProperty()->value()));

        // Retrieve our solvers' properties

        SingleCellSimulationViewInformationSolversWidget *solversWidget = mContentsWidget->informationWidget()->solversWidget();

        simulationData->setOdeSolverName(solversWidget->odeSolverData()->solversListProperty()->value()->text());
        simulationData->setDaeSolverName(solversWidget->daeSolverData()->solversListProperty()->value()->text());
        simulationData->setNlaSolverName(solversWidget->nlaSolverData()->solversListProperty()->value()->text());

        foreach (Core::Property *property, solversWidget->odeSolverData()->solversProperties().value(simulationData->odeSolverName()))
            simulationData->addOdeSolverProperty(property->name()->text(),
                                                 (property->value()->type() == Core::PropertyItem::Integer)?
                                                     Core::PropertyEditorWidget::integerPropertyItem(property->value()):
                                                     Core::PropertyEditorWidget::doublePropertyItem(property->value()));

        foreach (Core::Property *property, solversWidget->daeSolverData()->solversProperties().value(simulationData->daeSolverName()))
            simulationData->addDaeSolverProperty(property->name()->text(),
                                                 (property->value()->type() == Core::PropertyItem::Integer)?
                                                     Core::PropertyEditorWidget::integerPropertyItem(property->value()):
                                                     Core::PropertyEditorWidget::doublePropertyItem(property->value()));

        foreach (Core::Property *property, solversWidget->nlaSolverData()->solversProperties().value(simulationData->nlaSolverName()))
            simulationData->addNlaSolverProperty(property->name()->text(),
                                                 (property->value()->type() == Core::PropertyItem::Integer)?
                                                     Core::PropertyEditorWidget::integerPropertyItem(property->value()):
                                                     Core::PropertyEditorWidget::doublePropertyItem(property->value()));

// Retrieve the active graph panel
//---GRY--- THE BELOW IS TEMPORARY, JUST FOR OUR DEMO...

mActiveGraphPanel->plot()->setAxisScale(QwtPlot::xBottom, simulationData->startingPoint(), simulationData->endingPoint());

        // Check how much memory is needed to run our simulation

        bool runSimulation = true;

        double freeMemory = Core::freeMemory();
        double neededMemory = mSimulation->neededMemory();

        if (neededMemory > freeMemory) {
            // More memory is needed to run the simulation than currently
            // available, so ask the user whether he still wants to go ahead

            QString freeMemoryAsString = Core::sizeAsString(freeMemory);
            QString neededMemoryAsString = Core::sizeAsString(neededMemory);

            if (QMessageBox::question(qApp->activeWindow(),
                                      qApp->applicationName(),
                                      tr("This simulation requires %1 of memory to run while you currently have %2 of memory left. Do you still want to go ahead and (try to) run the simulation?").arg(neededMemoryAsString, freeMemoryAsString),
                                      QMessageBox::Yes|QMessageBox::No,
                                      QMessageBox::No) == QMessageBox::No)
                // The user doesn't want to run the simulation

                runSimulation = false;
        }

        // Run the simulation if possible/wanted

        if (runSimulation)
            mSimulation->run(mSolverInterfaces);
    } else if (mSimulation->workerStatus() == SingleCellSimulationViewSimulationWorker::Pausing) {
        // Our simulation was paused, so resume it

        mSimulation->resume();
    }
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionPause_triggered()
{
    // Make sure that we have a simulation

    if (!mSimulation)
        return;

    // Pause the simulation

    mSimulation->pause();
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionStop_triggered()
{
    // Make sure that we have a simulation

    if (!mSimulation)
        return;

    // Stop the simulation

    mSimulation->stop();
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionDebugMode_triggered()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionAdd_triggered()
{
    // Ask our graph panels widget to add a new graph panel

    mContentsWidget->graphPanelsWidget()->addGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionRemove_triggered()
{
    // Ask our graph panels widget to remove the current graph panel

    mContentsWidget->graphPanelsWidget()->removeGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionCsvExport_triggered()
{
    // Make sure that we have simulation data results to export to CSV

    if (   !mSimulation->data()->results()
        || !mSimulation->data()->results()->size()) {
        QMessageBox::warning(qApp->activeWindow(), tr("CSV Export"),
                             tr("Sorry, but there are no simulation results to export to CSV."));

        return;
    }

    // Export our simulation data results to a CSV file

    QString fileName = Core::getSaveFileName(tr("Export to a CSV file"),
                                             QString(),
                                             tr("CSV File")+" (*.csv)");

    if (!fileName.isEmpty())
        mSimulation->data()->results()->exportToCsv(fileName);
}

//==============================================================================

void SingleCellSimulationViewWidget::updateDelayValue(const double &pDelayValue)
{
    // Update our delay value widget

    mDelayValueWidget->setText(QLocale().toString(pDelayValue)+" ms");

    // Also update our simulation object

    if (mSimulation)
        mSimulation->setDelay(pDelayValue);
}

//==============================================================================

void SingleCellSimulationViewWidget::simulationRunning()
{
    // Our simulation worker is running, so update our simulation mode, but only
    // if it is the active simulation

    if (qobject_cast<SingleCellSimulationViewSimulation *>(sender()) == mSimulation)
        setSimulationMode(true, false);
}

//==============================================================================

void SingleCellSimulationViewWidget::simulationPausing()
{
    // Our simulation worker is pausing, so update our run/pause mode, but only
    // if it is the active simulation

    if (qobject_cast<SingleCellSimulationViewSimulation *>(sender()) == mSimulation)
        setSimulationMode(true, true);
}

//==============================================================================

void SingleCellSimulationViewWidget::simulationStopped(const int &pElapsedTime)
{
    // We want a short delay before resetting the progress bar and the file tab
    // icon, so that the user can really see when the simulation has completed

    static const int ResetDelay = 169;

    // Our simulation worker has stopped, so output the elapsed time, reset our
    // progress bar (with a bit of a delay) and update our simulation mode, but
    // only if it is the active simulation

    SingleCellSimulationViewSimulation *simulation = qobject_cast<SingleCellSimulationViewSimulation *>(sender());

    if (simulation == mSimulation) {
        if (pElapsedTime != -1) {
            // We have a valid elapsed time, so show the simulation time

            SingleCellSimulationViewSimulationData *simulationData = mSimulation->data();
            QString solversInformation = QString();

            if (!simulationData->odeSolverName().isEmpty())
                solversInformation += simulationData->odeSolverName();
            else
                solversInformation += simulationData->daeSolverName();

            if (!simulationData->nlaSolverName().isEmpty())
                solversInformation += "+"+simulationData->nlaSolverName();

            output(QString(OutputTab+"<strong>Simulation time:</strong> <span"+OutputInfo+">"+QString::number(0.001*pElapsedTime, 'g', 3)+" s (using "+solversInformation+")</span>."+OutputBrLn));
        }

        QTimer::singleShot(ResetDelay, this, SLOT(resetProgressBar()));

        setSimulationMode(false, true);
    }

    // Remove our tracking of our simulation progress and let people know that
    // we should reset our file tab icon, but only if we are the active
    // simulation

    if (simulation) {
        // Stop keeping track of the simulation progress

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

void SingleCellSimulationViewWidget::resetProgressBar()
{
    // Reset our progress bar

    mProgressBarWidget->setValue(0.0);
}

//==============================================================================

void SingleCellSimulationViewWidget::resetFileTabIcon()
{
    // Let people know that we want to reset our file tab icon

    SingleCellSimulationViewSimulation *simulation = mStoppedSimulations.first();

    mStoppedSimulations.removeFirst();

    emit updateFileTabIcon(simulation->fileName(), QIcon());
}

//==============================================================================

void SingleCellSimulationViewWidget::simulationProgress(const double &pProgress,
                                                        SingleCellSimulationViewSimulation *pSimulation)
{
    // Our simulation worker has made some progres, so update our progress bar,
    // but only if it is the active simulation

    SingleCellSimulationViewSimulation *simulation = pSimulation?pSimulation:qobject_cast<SingleCellSimulationViewSimulation *>(sender());

    if (!simulation || (simulation == mSimulation))
        // Note: we test for simulation to be valid since simulationProgress()
        //       can also be called directly (as opposed to being called as a
        //       response to a signal) as is done in initialize() above...

        mProgressBarWidget->setValue(pProgress);

    // Let people know that we should update the icon of our file tab, but only
    // if it isn't the active simulation (there is already the progress bar, so
    // no need to duplicate this simulation progress information)
    // Note: we need to retrieve the name of the file associated with the
    //       simulation since we have only one SingleCellSimulationViewWidget
    //       object and anyone handling this signal (e.g. CentralWidget) won't
    //       be able to tell for which simulation the progress is...

    if (simulation && (simulation != mSimulation)) {
        int oldProgress = mProgresses.value(simulation->fileName(), -1);
        int newProgress = (tabBarIconSize()-2)*pProgress;
        // Note: tabBarIconSize()-2 because we want a one-pixel wide
        //       border...

        if (newProgress != oldProgress) {
            // The progress has changed (or we want to force the updating of a
            // specific simulation), so keep track of its new value and update
            // our file tab icon

            mProgresses.insert(simulation->fileName(), newProgress);

            // Let people know about the file tab icon to be used for the
            // model

            emit updateFileTabIcon(simulation->fileName(),
                                   fileTabIcon(simulation->fileName()));
        }
    }
}

//==============================================================================

void SingleCellSimulationViewWidget::simulationError(const QString &pMessage,
                                                     const ErrorType &pErrorType)
{
    // Output the simulation error, but only if it is for the active simulation

    SingleCellSimulationViewSimulation *simulation = qobject_cast<SingleCellSimulationViewSimulation *>(sender());

    if (!simulation || (simulation == mSimulation)) {
        // Note: see the corresponding note in simulationProgress() above...

        mErrorType = pErrorType;

        updateInvalidModelMessageWidget();

        output(OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+pMessage+".</span>"+OutputBrLn);
    }
}

//==============================================================================

SingleCellSimulationViewSimulation * SingleCellSimulationViewWidget::simulation() const
{
    // Return our (current) simulation

    return mSimulation;
}

//==============================================================================

void SingleCellSimulationViewWidget::setDelayValue(const int &pDelayValue)
{
    // Set the value of our delay widget

    mDelayWidget->setValue(pDelayValue);

    updateDelayValue(pDelayValue);
}

//==============================================================================

SingleCellSimulationViewContentsWidget * SingleCellSimulationViewWidget::contentsWidget() const
{
    // Return our contents widget

    return mContentsWidget;
}

//==============================================================================

void SingleCellSimulationViewWidget::splitterWidgetMoved()
{
    // Our splitter has been moved, so keep track of its new sizes

    mSplitterWidgetSizes = mSplitterWidget->sizes();
}

//==============================================================================

void SingleCellSimulationViewWidget::simulationPropertyChanged(Core::Property *pProperty)
{
    // Check whether our simulation's starting point property has been modified
    // and if so, then update our simulation data object accordingly
    // Note: this is the only simulation property we need to check because it's
    //       the only one that can potentially have an effect on the value of
    //       'computed constants' and 'variables'...


    SingleCellSimulationViewInformationSimulationWidget *simulationWidget = mContentsWidget->informationWidget()->simulationWidget();

    if (pProperty == simulationWidget->startingPointProperty())
        mSimulation->data()->setStartingPoint(Core::PropertyEditorWidget::doublePropertyItem(pProperty->value()));
}

//==============================================================================

QString SingleCellSimulationViewWidget::parameterKey(const QString &pFileName,
                                                     CellMLSupport::CellmlFileRuntimeModelParameter *pParameter) const
{
    // Determine and return the key for the parameter

    return pFileName+"|"+QString::number(pParameter->type())+"|"+QString::number(pParameter->index());
}

//==============================================================================

void SingleCellSimulationViewWidget::parameterNeeded(const QString &pFileName,
                                                     CellMLSupport::CellmlFileRuntimeModelParameter *pParameter,
                                                     const bool &pNeeded)
{
    // Determine the key for the parameter

    QString key = parameterKey(pFileName, pParameter);

    // Retrieve the trace associated with the key, if any

    QwtPlotCurve *trace = mTraces.value(key);

    // Check whether to create/remove the trace

    if (trace && !pNeeded) {
        // We have a trace and we want to remove it

        mActiveGraphPanel->removeTrace(trace);

        mTraces.remove(key);
    } else if (!trace && pNeeded) {
        // We don't have a trace and want to create one

        QwtPlotCurve *trace = mActiveGraphPanel->addTrace();

        mTraces.insert(key, trace);

        // Populate the trace if some results are available

        SingleCellSimulationViewSimulationDataResults *results = mSimulation->data()->results();

        if (results) {
            double *yData;

            if (   (pParameter->type() == CellMLSupport::CellmlFileRuntimeModelParameter::Constant)
                || (pParameter->type() == CellMLSupport::CellmlFileRuntimeModelParameter::ComputedConstant))
                yData = results->constants()[pParameter->index()];
            else if (pParameter->type() == CellMLSupport::CellmlFileRuntimeModelParameter::State)
                yData = results->states()[pParameter->index()];
            else if (pParameter->type() == CellMLSupport::CellmlFileRuntimeModelParameter::Rate)
                yData = results->rates()[pParameter->index()];
            else
                yData = results->algebraic()[pParameter->index()];

            trace->setRawSamples(results->points(), yData, results->size());
        }
    }
}

//==============================================================================

void SingleCellSimulationViewWidget::results(SingleCellSimulationViewSimulationDataResults *pResults)
{
    QMap<QString, QwtPlotCurve *>::const_iterator iter = mTraces.constBegin();

    while (iter != mTraces.constEnd()) {
        // Retrieve the file name associated with the trace

        QString fileName = iter.key();

        fileName.chop(fileName.size()-fileName.indexOf('|'));

        // Retrieve the type of the parameter associated with the trace

        QString typeAsString = iter.key();

        typeAsString.remove(fileName+"|");
        typeAsString.chop(typeAsString.size()-typeAsString.indexOf('|'));

        // Retrieve the index of the parameter associated with the trace

        QString indexAsString = iter.key();

        indexAsString.remove(fileName+"|"+typeAsString+"|");

        // Update the traces which are associated with the current file name

        QwtPlotCurve *trace = iter.value();

        if (!fileName.compare(mSimulation->fileName())) {
            if (pResults) {
                double *yData;

                CellMLSupport::CellmlFileRuntimeModelParameter::ModelParameterType type = CellMLSupport::CellmlFileRuntimeModelParameter::ModelParameterType(typeAsString.toInt());
                int index = indexAsString.toInt();

                if (   (type == CellMLSupport::CellmlFileRuntimeModelParameter::Constant)
                    || (type == CellMLSupport::CellmlFileRuntimeModelParameter::ComputedConstant))
                    yData = pResults->constants()[index];
                else if (type == CellMLSupport::CellmlFileRuntimeModelParameter::State)
                    yData = pResults->states()[index];
                else if (type == CellMLSupport::CellmlFileRuntimeModelParameter::Rate)
                    yData = pResults->rates()[index];
                else
                    yData = pResults->algebraic()[index];

                trace->setRawSamples(pResults->points(), yData, pResults->size());
            }
        }

        // Go to the next trace

        ++iter;
    }
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Single cell simulation view widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "cellmlfilevariable.h"
#include "coreutils.h"
#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewgraphpanelswidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewplugin.h"
#include "singlecellsimulationviewprogressbarwidget.h"
#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewsimulationdata.h"
#include "singlecellsimulationviewinformationparameterswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationsolverswidget.h"
#include "singlecellsimulationviewwidget.h"
#include "toolbarwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_singlecellsimulationviewwidget.h"

//==============================================================================

#include <QBrush>
#include <QDesktopWidget>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QSettings>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include <QVariant>

//==============================================================================

#include "qwt_slider.h"

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
    mCanSaveSettings(false),
    mSolverInterfaces(SolverInterfaces()),
    mSimulation(0),
    mSimulations(QMap<QString, SingleCellSimulationViewSimulation *>()),
    mStoppedSimulations(QList<SingleCellSimulationViewSimulation *>()),
    mSplitterWidgetSizes(QList<int>()),
    mProgresses(QMap<QString, int>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a slider (and a label to show its value) to specify the delay
    // (in milliseconds) between two data points

    mDelayWidget = new QwtSlider(this);
#ifndef Q_WS_MAC
    QWidget *delaySliderSpaceWidget = new QWidget(this);
#endif
    mDelayValueWidget = new QLabel(this);

    mDelayWidget->setBorderWidth(1);
    mDelayWidget->setFixedWidth(0.1*qApp->desktop()->screenGeometry().width());
    mDelayWidget->setHandleSize(0.5*mDelayWidget->handleSize().width(),
                                    mDelayWidget->handleSize().height());
    mDelayWidget->setRange(0.0, 50.0, 1.0, 10.0);

#ifndef Q_WS_MAC
    delaySliderSpaceWidget->setFixedWidth(4);
#endif

    connect(mDelayWidget, SIGNAL(valueChanged(double)),
            this, SLOT(updateDelayValue(const double &)));

    updateDelayValue(mDelayWidget->value());

    // Create a tool bar widget with different buttons

    mToolBarWidget = new Core::ToolBarWidget(this);

    mToolBarWidget->addAction(mGui->actionRun);
    mToolBarWidget->addAction(mGui->actionPause);
    mToolBarWidget->addAction(mGui->actionStop);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addWidget(mDelayWidget);
#ifndef Q_WS_MAC
    mToolBarWidget->addWidget(delaySliderSpaceWidget);
#endif
    mToolBarWidget->addWidget(mDelayValueWidget);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mGui->actionDebugMode);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mGui->actionAdd);
    mToolBarWidget->addAction(mGui->actionRemove);
    mToolBarWidget->addSeparator();
    mToolBarWidget->addAction(mGui->actionCsvExport);

    mGui->layout->addWidget(mToolBarWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));

    // Create our splitter widget and keep track of its movement
    // Note: we need to keep track of its movement so that saveSettings() can
    //       work fine even when mContentsWidget is not visible (which happens
    //       when a CellML file cannot be run for some reason or another)...

    mSplitterWidget = new QSplitter(Qt::Vertical, this);

    connect(mSplitterWidget, SIGNAL(splitterMoved(int,int)),
            this, SLOT(splitterWidgetMoved()));

    // Create our contents widget and create a connection to keep track of
    // whether we can remove graph panels

    mContentsWidget = new SingleCellSimulationViewContentsWidget(this);

    mContentsWidget->setObjectName("Contents");

    connect(mContentsWidget->graphPanelsWidget(), SIGNAL(removeGraphPanelsEnabled(const bool &)),
            mGui->actionRemove, SLOT(setEnabled(bool)));

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

    mProgressBarWidget = new SingleCellSimulationViewProgressBarWidget(this);

    mProgressBarWidget->setFixedHeight(3);
    mProgressBarWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mProgressBarWidget);

    // Make our contents widget our focus proxy

    setFocusProxy(mContentsWidget);
}

//==============================================================================

SingleCellSimulationViewWidget::~SingleCellSimulationViewWidget()
{
    // Delete our simulation data

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

    mInvalidModelMessageWidget->setMessage("<div align=center>"
                                           "    <p>"
                                           "        "+tr("Sorry, but the <strong>%1</strong> view requires a valid CellML file to work...").arg(mPluginParent->viewName())
                                          +"    </p>"
                                           "    <p>"
                                           "        <small><em>("+tr("See below for more information.")+")</em></small>"
                                           "    </p>"
                                           "</div>");

    // Retranslate our contents widget

    mContentsWidget->retranslateUi();
}

//==============================================================================

static const QString SettingsSizesCount = "SizesCount";
static const QString SettingsSize       = "Size";

//==============================================================================

void SingleCellSimulationViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve and set the sizes of our splitter

    int sizesCount = pSettings->value(SettingsSizesCount, 0).toInt();

    if (sizesCount) {
        mSplitterWidgetSizes = QList<int>();

        for (int i = 0; i < sizesCount; ++i)
            mSplitterWidgetSizes << pSettings->value(SettingsSize+QString::number(i)).toInt();

        mSplitterWidget->setSizes(mSplitterWidgetSizes);
    }

    // Retrieve the settings of our contents widget

    pSettings->beginGroup(mContentsWidget->objectName());
        mContentsWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewWidget::saveSettings(QSettings *pSettings) const
{
    if (!mCanSaveSettings)
        return;

    // Keep track of our splitter sizes

    pSettings->setValue(SettingsSizesCount, mSplitterWidgetSizes.count());

    for (int i = 0, iMax = mSplitterWidgetSizes.count(); i < iMax; ++i)
        pSettings->setValue(SettingsSize+QString::number(i), mSplitterWidgetSizes[i]);

    // Keep track of the settings of our contents widget

    pSettings->beginGroup(mContentsWidget->objectName());
        mContentsWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewWidget::addSolverInterface(SolverInterface *pSolverInterface)
{
    // Add the solver interface to our list

    if (!mSolverInterfaces.contains(pSolverInterface))
        // The solver interface is not yet in our list, so...

        mSolverInterfaces << pSolverInterface;
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
    mOutputWidget->moveCursor(QTextCursor::End);
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

    // Enable or disable the simulation widget

    mContentsWidget->informationWidget()->simulationWidget()->setEnabled(!pEnabled);

    // Give the focus to our focus proxy, in case we leave the simulation mode
    // (so that the user can modify simulation data, etc.)

    if (!pEnabled)
        focusProxy()->setFocus();
}

//==============================================================================

void SingleCellSimulationViewWidget::initialize(const QString &pFileName)
{
    // Do a few things for the previous model, if needed

    SingleCellSimulationViewSimulation *previousSimulation = mSimulation;

    if (previousSimulation)
        // Update our previous simulation's data from the GUI

        previousSimulation->data()->updateFromGui(this);

    // Retrieve our simulation data for the current model, if any

    mSimulation = mSimulations.value(pFileName);

    if (!mSimulation) {
        // No simulation data currently exist for the model, so create some

        mSimulation = new SingleCellSimulationViewSimulation(pFileName);

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

        // Keep track of our simulation data

        mSimulations.insert(pFileName, mSimulation);
    }

    // Reset our file tab icon and stop tracking its simulation progress (in
    // case a simulation is running)

    if (mSimulation->workerStatus() != SingleCellSimulationViewSimulationWorker::Unknown) {
        mProgresses.remove(mSimulation->fileName());

        emit updateFileTabIcon(mSimulation->fileName(), QIcon());
    }

    // Output some information about our CellML file

    CellMLSupport::CellmlFileRuntime *cellmlFileRuntime = mSimulation->cellmlFileRuntime();

    QString information = QString();

    if (!mOutputWidget->document()->isEmpty())
        information += "<hr/>\n";

    information += "<strong>"+pFileName+"</strong>"+OutputBrLn;
    information += OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

    if (cellmlFileRuntime->isValid()) {
        QString additionalInformation = QString();

        if (cellmlFileRuntime->needNlaSolver())
            additionalInformation = " + "+tr("Non-linear algebraic system(s)");

        information += "<span"+OutputGood+">"+tr("valid")+"</span>."+OutputBrLn;
        information += QString(OutputTab+"<strong>"+tr("Model type:")+"</strong> <span"+OutputInfo+">%1%2</span>."+OutputBrLn).arg((cellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?tr("ODE"):tr("DAE"),
                                                                                                                                   additionalInformation);
    } else {
        information += "<span"+OutputBad+">"+tr("invalid")+"</span>."+OutputBrLn;

        foreach (const CellMLSupport::CellmlFileIssue &issue,
                 cellmlFileRuntime->issues())
            information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2</span>."+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?tr("Error:"):tr("Warning:"),
                                                                                                                 issue.message());
    }

    output(information);

    // Retrieve our variable of integration, if any

    CellMLSupport::CellmlFileVariable *variableOfIntegration = cellmlFileRuntime->isValid()?cellmlFileRuntime->variableOfIntegration():0;

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

    if (cellmlFileRuntime->isValid()) {
        // We have a valid runtime
        // Note: if we didn't have a valid runtime, then there would be no need
        //       to output an error message since one would have already been
        //       generated while trying to get the runtime (see above)...

        // Retrieve the unit of the variable of integration, if any

        if (!variableOfIntegration) {
            // We don't have a variable of integration, so...

            simulationError(tr("the model must have at least one ODE or DAE"));
        } else {
            // Retrieve the unit of our variable of integration

            mContentsWidget->informationWidget()->simulationWidget()->setUnit(cellmlFileRuntime->variableOfIntegration()->unit());

            // Initialise our GUI's solvers and parameters widgets using our
            // simulation

            mContentsWidget->informationWidget()->solversWidget()->initialize(cellmlFileRuntime);
            mContentsWidget->informationWidget()->parametersWidget()->initialize(cellmlFileRuntime);

            // Update our GUI using our simulation's data

            mSimulation->data()->updateGui(this);

#ifdef QT_DEBUG
            // Output the type of solvers that are available to run the model

            qDebug("---------------------------------------");
            qDebug("%s", qPrintable(pFileName));

            Solver::Type solverType;

            if (cellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode) {
                solverType = Solver::Ode;

                information = " - ODE solver(s): ";
            } else {
                solverType = Solver::Dae;

                information = " - DAE solver(s): ";
            }

            int solverCounter = 0;

            foreach (SolverInterface *solverInterface, mSolverInterfaces)
                if (solverInterface->type() == solverType) {
                    if (++solverCounter == 1)
                        information += solverInterface->name();
                    else
                        information += " | "+solverInterface->name();
                }

            if (!solverCounter)
                information += "none";

            if (cellmlFileRuntime->needNlaSolver()) {
                information += "\n - NLA solver(s): ";

                int solverCounter = 0;

                foreach (SolverInterface *solverInterface, mSolverInterfaces)
                    if (solverInterface->type() == Solver::Nla) {
                        if (++solverCounter == 1)
                            information += solverInterface->name();
                        else
                            information += " | "+solverInterface->name();
                    }

                if (!solverCounter)
                    information += "none";
            }

            qDebug("%s", qPrintable(information));
#endif

            // Everything went fine, so...

            hasError = false;
        }
    }

    // Check if an error occurred and, if so, show/hide some widgets

    mToolBarWidget->setVisible(!hasError);

    mContentsWidget->setVisible(!hasError);
    mInvalidModelMessageWidget->setVisible(hasError);

    mProgressBarWidget->setVisible(!hasError);
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
        // No simulation data currently exist for the model, so...

        return QIcon();
    }
}

//==============================================================================

void SingleCellSimulationViewWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    Widget::paintEvent(pEvent);

    // The view has been painted at least once which means that the sizes of
    // mSplitterWidget are meaningful and, as a consequence, we can save our
    // settings upon leaving OpenCOR

    mCanSaveSettings = true;
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionRun_triggered()
{
    // Make sure that we have a simulation

    if (!mSimulation)
        return;

    // Make sure that our simulation data are up-to-date

    mSimulation->data()->updateFromGui(this);

    // Start the simulation

    mSimulation->run();
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
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewWidget::updateDelayValue(const double &pDelayValue)
{
    // Update our delay value widget

    mDelayValueWidget->setText(QLocale().toString(pDelayValue)+" ms");

    // Also update our simulation data

    if (mSimulation)
        mSimulation->data()->setDelay(mDelayWidget->value());
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
        output(QString(OutputTab+"<strong>Simulation time:</strong> <span"+OutputInfo+">"+QString::number(0.001*pElapsedTime, 'g', 3)+" s</span>."+OutputBrLn));

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

void SingleCellSimulationViewWidget::simulationError(const QString &pError)
{
    // Output the simulation error, but only if it is for the active simulation

    SingleCellSimulationViewSimulation *simulation = qobject_cast<SingleCellSimulationViewSimulation *>(sender());

    if (!simulation || (simulation == mSimulation))
        // Note: see the corresponding note in simulationProgress() above...

        output(OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+pError+".</span>"+OutputBrLn);
}

//==============================================================================

SingleCellSimulationViewSimulation * SingleCellSimulationViewWidget::simulation() const
{
    // Return our (current) simulation

    return mSimulation;
}

//==============================================================================

QwtSlider * SingleCellSimulationViewWidget::delayWidget() const
{
    // Return our delay widget

    return mDelayWidget;
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

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

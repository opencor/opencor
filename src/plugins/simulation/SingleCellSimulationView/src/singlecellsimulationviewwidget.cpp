//==============================================================================
// Single cell simulation view widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "cellmlfilevariable.h"
#include "coreutils.h"
#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulation.h"
#include "singlecellsimulationviewsimulationdata.h"
#include "singlecellsimulationviewsimulationinformationwidget.h"
#include "singlecellsimulationviewwidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_singlecellsimulationviewwidget.h"

//==============================================================================

#include <QBrush>
#include <QDesktopWidget>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QProgressBar>
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

SingleCellSimulationViewWidget::SingleCellSimulationViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::SingleCellSimulationViewWidget),
    mCanSaveSettings(false),
    mSolverInterfaces(SolverInterfaces()),
    mSimulation(0),
    mSimulations(QMap<QString, SingleCellSimulationViewSimulation *>()),
    mStoppedSimulations(QList<SingleCellSimulationViewSimulation *>()),
    mProgresses(QMap<QString, int>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a slider (and a label to show its value) to specify the delay
    // (in milliseconds) between two data points

    mDelaySlider = new QwtSlider(this);
#ifndef Q_WS_MAC
    QWidget *delaySliderSpace = new QWidget(this);
#endif
    mDelaySliderValue = new QLabel(this);

    mDelaySlider->setBorderWidth(1);
    mDelaySlider->setFixedWidth(0.1*qApp->desktop()->screenGeometry().width());
    mDelaySlider->setHandleSize(0.5*mDelaySlider->handleSize().width(),
                                    mDelaySlider->handleSize().height());
    mDelaySlider->setRange(0.0, 50.0, 1.0, 10.0);

#ifndef Q_WS_MAC
    delaySliderSpace->setFixedWidth(4);
#endif

    connect(mDelaySlider, SIGNAL(valueChanged(double)),
            this, SLOT(updateSliderValue(const double &)));

    updateSliderValue(mDelaySlider->value());

    // Create a tool bar widget with different buttons

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionRun);
    toolBarWidget->addAction(mGui->actionPause);
    toolBarWidget->addAction(mGui->actionStop);
    toolBarWidget->addSeparator();
    toolBarWidget->addWidget(mDelaySlider);
#ifndef Q_WS_MAC
    toolBarWidget->addWidget(delaySliderSpace);
#endif
    toolBarWidget->addWidget(mDelaySliderValue);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionDebugMode);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionAdd);
    toolBarWidget->addAction(mGui->actionRemove);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionCsvExport);

    mGui->layout->addWidget(toolBarWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));

    // Create our splitter

    mSplitter = new QSplitter(Qt::Vertical, this);

    // Create our contents widget and create a connection to keep track of
    // whether we can remove graph panels

    mContentsWidget = new SingleCellSimulationViewContentsWidget(this);

    mContentsWidget->setObjectName("Contents");

    connect(mContentsWidget, SIGNAL(removeGraphPanelsEnabled(const bool &)),
            mGui->actionRemove, SLOT(setEnabled(bool)));

    // Create a simulation output widget with a layout on which we put a
    // separating line and our simulation output list view
    // Note: the separating line is because we remove, for aesthetical reasons,
    //       the border of our simulation output list view...

    QWidget *simulationOutputWidget = new QWidget(this);
    QVBoxLayout *simulationOutputLayout= new QVBoxLayout(simulationOutputWidget);

    simulationOutputLayout->setMargin(0);
    simulationOutputLayout->setSpacing(0);

    simulationOutputWidget->setLayout(simulationOutputLayout);

    mOutput = new QTextEdit(this);

    mOutput->setFrameStyle(QFrame::NoFrame);

    simulationOutputLayout->addWidget(Core::newLineWidget(this));
    simulationOutputLayout->addWidget(mOutput);

    // Populate our splitter and use as much space as possible for it by asking
    // for its height to be that of the desktop's, and then add our splitter to
    // our single cell simulation view widget

    mSplitter->addWidget(mContentsWidget);
    mSplitter->addWidget(simulationOutputWidget);

    mSplitter->setSizes(QList<int>() << qApp->desktop()->screenGeometry().height() << 1);

    mGui->layout->addWidget(mSplitter);

    // Create our (thin) simulation progress widget
    // Note: we set the maximum value to 32,768 (rather than leaving it to the
    //       default value of 100), so that we can get a smooth progress and not
    //       a jerky one (should the progress bar be more than 100 pixels
    //       wide)...

    mProgressBar = new QProgressBar(this);

    mProgressBar->setAlignment(Qt::AlignCenter);
    mProgressBar->setFixedHeight(3);
    mProgressBar->setMaximum(32768);
    mProgressBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mProgressBar->setTextVisible(false);

    setProgressBarStyleSheet();

    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mProgressBar);

    // Make our contents widget our focus proxy

    setFocusProxy(mContentsWidget);
}

//==============================================================================

SingleCellSimulationViewWidget::~SingleCellSimulationViewWidget()
{
    // Delete our simulation settings

    foreach (SingleCellSimulationViewSimulation *simulation, mSimulations)
        delete simulation;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewWidget::retranslateUi()
{
    // Retranslate the whole view

    mGui->retranslateUi(this);

    // Retranslate our slider and its value

    mDelaySlider->setToolTip(tr("Delay"));
    mDelaySliderValue->setToolTip(mDelaySlider->toolTip());

    mDelaySlider->setStatusTip(tr("Delay in milliseconds between two data points"));
    mDelaySliderValue->setStatusTip(mDelaySlider->statusTip());

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
        QList<int> newSizes = QList<int>();

        for (int i = 0; i < sizesCount; ++i)
            newSizes << pSettings->value(SettingsSize+QString::number(i)).toInt();

        mSplitter->setSizes(newSizes);
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

    QList<int> crtSizes = mSplitter->sizes();

    pSettings->setValue(SettingsSizesCount, crtSizes.count());

    for (int i = 0, iMax = crtSizes.count(); i < iMax; ++i)
        pSettings->setValue(SettingsSize+QString::number(i), crtSizes[i]);

    // Keep track of the settings of our contents widget

    pSettings->beginGroup(mContentsWidget->objectName());
        mContentsWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewWidget::addSolverInterface(SolverInterface *pSolverInterface)
{
    // Add the solver interface to our list

    if (!mSolverInterfaces.contains(pSolverInterface)) {
        // The solver interface is not yet in our list, so...

        mSolverInterfaces << pSolverInterface;

#ifdef QT_DEBUG
        // Display the solver's properties

        qDebug("---------------------------------------");
        qDebug("'%s' solver:", qPrintable(pSolverInterface->name()));
        qDebug(" - Type: %s", qPrintable(pSolverInterface->typeAsString()));

        Solver::Properties properties = pSolverInterface->properties();

        if (properties.count()) {
            qDebug(" - Properties:");

            Solver::Properties::const_iterator iter = properties.constBegin();
            Solver::Properties::const_iterator iterEnd = properties.constEnd();

            while (iter != iterEnd) {
                QString type;

                switch (iter.value()) {
                case Solver::Double:
                    type = "Double";

                    break;
                case Solver::Integer:
                    type = "Integer";

                    break;
                default:
                    type = "???";
                }

                qDebug("    - %s: %s", qPrintable(iter.key()), qPrintable(type));

                ++iter;
            }
        } else {
            qDebug(" - Properties: none");
        }
#endif
    }
}

//==============================================================================

void SingleCellSimulationViewWidget::clearGraphPanels()
{
    // Ask our contents widget to clear all the graph panels

    mContentsWidget->clearGraphPanels();
}

//==============================================================================

void SingleCellSimulationViewWidget::clearActiveGraphPanel()
{
    // Ask our contents widget to clear the current graph panel

    mContentsWidget->clearActiveGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewWidget::output(const QString &pMessage)
{
    // Move to the end of the output (just in case...)

    mOutput->moveCursor(QTextCursor::End);

    // Make sure that the output ends as expected and if not then add BrLn to it

    static const QString EndOfOutput = "<br /></p></body></html>";

    if (mOutput->toHtml().right(EndOfOutput.size()).compare(EndOfOutput))
        mOutput->insertHtml(OutputBrLn);

    // Output the message and make sure that it's visible

    mOutput->insertHtml(pMessage);
    mOutput->moveCursor(QTextCursor::End);
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
    // (so that the user can modify simulation settings, etc.)

    if (!pEnabled)
        focusProxy()->setFocus();
}

//==============================================================================

void SingleCellSimulationViewWidget::initialize(const QString &pFileName)
{
    // Do a few things for the previous model, if needed

    SingleCellSimulationViewSimulation *previousSimulation = mSimulation;
    SingleCellSimulationViewSimulationInformationWidget *simulationSettings = mContentsWidget->informationWidget()->simulationWidget();

    if (previousSimulation)
        // Update our simulation settings for the previous model from the GUI

        previousSimulation->data()->updateFromGui(mDelaySlider,
                                                  simulationSettings);

    // Retrieve our simulation settings for the current model, if any

    mSimulation = mSimulations.value(pFileName);

    if (!mSimulation) {
        // No simulation settings currently exist for the model, so create some

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

        // Keep track of our simulation settings

        mSimulations.insert(pFileName, mSimulation);
    }

    // Reset our file tab icon and stop tracking its simulation progress (in
    // case a simulation is running)

    if (mSimulation->workerStatus() != SingleCellSimulationViewSimulationWorker::Unknown) {
        mProgresses.remove(mSimulation->fileName());

        emit updateFileTabIcon(mSimulation->fileName(), QIcon());
    }

    // Update our GUI using our simulation settings for the current model

    mSimulation->data()->updateGui(mDelaySlider, simulationSettings);

    // Output some information about our CellML file

    CellMLSupport::CellmlFileRuntime *cellmlFileRuntime = mSimulation->cellmlFileRuntime();

    QString information = QString();

    if (!mOutput->document()->isEmpty())
        information += "<hr/>\n";

    information += "<strong>"+pFileName+"</strong>"+OutputBrLn;
    information += OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

    if (cellmlFileRuntime->isValid()) {
        QString additionalInformation = QString();

        if (cellmlFileRuntime->needNlaSolver())
            additionalInformation = " + "+tr("Non-linear algebraic system(s)");

        information += "<span"+OutputGood+">"+tr("valid")+"</span>.<br/>\n";
        information += QString(OutputTab+"<strong>"+tr("Model type:")+"</strong> <span"+OutputInfo+">%1%2</span>."+OutputBrLn).arg((cellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?tr("ODE"):tr("DAE"),
                                                                                                                                   additionalInformation);
    } else {
        information += "<span"+OutputBad+">"+tr("invalid")+"</span>."+OutputBrLn;

        foreach (const CellMLSupport::CellmlFileIssue &issue,
                 cellmlFileRuntime->issues())
            information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?tr("Error:"):tr("Warning:"),
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

    // By default, we assume that the runtime isn't valid or that there is no
    // variable of integration, meaning that that we can't show the unit of the
    // variable of integration

    simulationSettings->setUnit("???");

    // Check if we have an invalid runtime and, if so, set the unit to an
    // unknown one and leave

    if (!cellmlFileRuntime->isValid())
        // Note: no need to output an error since one will have already been
        //       generated while trying to get the runtime (see above)...

        return;

    // Retrieve the unit of the variable of integration, if any

    if (variableOfIntegration)
        // We have a variable of integration, so we can retrieve its unit

        simulationSettings->setUnit(cellmlFileRuntime->variableOfIntegration()->unit());
    else
        // We don't have a variable of integration, so...

        simulationError(tr("the model must have at least one ODE or DAE"));
}

//==============================================================================

int SingleCellSimulationViewWidget::fileTabBarIconSize() const
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

        QImage tabBarIcon = QImage(fileTabBarIconSize(), mProgressBar->height()+2,
                                   QImage::Format_ARGB32_Premultiplied);
        QPainter tabBarIconPainter(&tabBarIcon);

        QPalette tabBarIconPalette = palette();

        tabBarIconPainter.setBrush(QBrush(tabBarIconPalette.color(QPalette::Window)));
        tabBarIconPainter.setPen(QPen(CommonWidget::borderColor()));

        tabBarIconPainter.drawRect(0, 0, tabBarIcon.width()-1, tabBarIcon.height()-1);
        tabBarIconPainter.fillRect(QRect(1, 1, progress, tabBarIcon.height()-2),
                                   QBrush(tabBarIconPalette.color(QPalette::Highlight)));

        return QIcon(QPixmap::fromImage(tabBarIcon));
    } else {
        // No simulation settings currently exist for the model, so...

        return QIcon();
    }
}

//==============================================================================

void SingleCellSimulationViewWidget::setProgressBarStyleSheet()
{
    // Customise our progress bar to be a very simple (and fast) one

    QPalette progressBarPalette = palette();
    QColor progressBarBackground = progressBarPalette.color(QPalette::Window);
    QColor progressBarChunkBackground = progressBarPalette.color(QPalette::Highlight);

    mProgressBar->setStyleSheet(QString("QProgressBar {"
                                        "    background: rgb(%1, %2, %3);"
                                        "    border: 0px;"
                                        "}"
                                        ""
                                        "QProgressBar::chunk {"
                                        "    background: rgb(%4, %5, %6);"
                                        "    border: 0px;"
                                        "}").arg(QString::number(progressBarBackground.red()),
                                                 QString::number(progressBarBackground.green()),
                                                 QString::number(progressBarBackground.blue()),
                                                 QString::number(progressBarChunkBackground.red()),
                                                 QString::number(progressBarChunkBackground.green()),
                                                 QString::number(progressBarChunkBackground.blue())));
}

//==============================================================================

void SingleCellSimulationViewWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    Widget::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update the colours
    // to be used by our progress bar

    if (pEvent->type() == QEvent::PaletteChange)
        setProgressBarStyleSheet();
}

//==============================================================================

void SingleCellSimulationViewWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    Widget::paintEvent(pEvent);

    // The view has been painted at least once which means that the sizes of
    // mSplitter are meaningful and, as a consequence, we can save our settings
    // upon leaving OpenCOR

    mCanSaveSettings = true;
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionRun_triggered()
{
    // Make sure that we have a simulation

    if (!mSimulation)
        return;

    // Make sure that our simulation settings are up-to-date

    mSimulation->data()->updateFromGui(mDelaySlider,
                                       mContentsWidget->informationWidget()->simulationWidget());

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
    // Ask our contents widget to add a new graph panel

    mContentsWidget->addGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionRemove_triggered()
{
    // Ask our contents widget to remove the current graph panel

    mContentsWidget->removeGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionCsvExport_triggered()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewWidget::updateSliderValue(const double &pSliderValue)
{
    // Update our slider value

    mDelaySliderValue->setText(QLocale().toString(pSliderValue)+" ms");

    // Also update our simulation settings

    if (mSimulation)
        mSimulation->data()->setDelay(mDelaySlider->value());
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

    mProgressBar->setValue(0);
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

        mProgressBar->setValue(pProgress*mProgressBar->maximum());

    // Let people know that we should update the icon of our file tab, but only
    // if it isn't the active simulation (there is already the progress bar, so
    // no need to duplicate this simulation progress information)
    // Note: we need to retrieve the name of the file associated with the
    //       simulation since we have only one SingleCellSimulationViewWidget
    //       object and anyone handling this signal (e.g. CentralWidget) won't
    //       be able to tell for which simulation the progress is...

    if (simulation && (simulation != mSimulation)) {
        int oldProgress = mProgresses.value(simulation->fileName(), -1);
        int newProgress = (fileTabBarIconSize()-2)*pProgress;
        // Note: fileTabBarIconSize()-2 because we want a one-pixel wide
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

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

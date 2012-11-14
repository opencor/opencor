//==============================================================================
// Single cell simulation view widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "coreutils.h"
#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulationinformationwidget.h"
#include "singlecellsimulationviewwidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_singlecellsimulationviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
#include <QProgressBar>
#include <QSettings>
#include <QSplitter>
#include <QTextEdit>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewWidgetUserSettings::SingleCellSimulationViewWidgetUserSettings() :
    mStartingPoint(0.0),
    mEndingPoint(1000.0),
    mPointInterval(1.0)
{
}

//==============================================================================

void SingleCellSimulationViewWidgetUserSettings::get(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings)
{
    // Get our user settings from our simulation information widget

    mStartingPoint = pSimulationSettings->startingPoint();
    mEndingPoint   = pSimulationSettings->endingPoint();
    mPointInterval = pSimulationSettings->pointInterval();
}

//==============================================================================

void SingleCellSimulationViewWidgetUserSettings::set(SingleCellSimulationViewSimulationInformationWidget *pSimulationSettings)
{
    // Set our user settings to our simulation information widget

    pSimulationSettings->setStartingPoint(mStartingPoint);
    pSimulationSettings->setEndingPoint(mEndingPoint);
    pSimulationSettings->setPointInterval(mPointInterval);
}

//==============================================================================

static const QString OutputTab  = "&nbsp;&nbsp;&nbsp;&nbsp;";
static const QString OutputGood = " style=\"color: green;\"";
static const QString OutputInfo = " style=\"color: navy;\"";
static const QString OutputBad  = " style=\"color: maroon;\"";
static const QString OutputBrLn = "<br/>\n";

//==============================================================================

SingleCellSimulationViewWidget::SingleCellSimulationViewWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewWidget),
    mCanSaveSettings(false),
    mSolverInterfaces(SolverInterfaces()),
    mCellmlFileRuntime(0),
    mUserSettings(0),
    mModelUserSettings(QMap<QString, SingleCellSimulationViewWidgetUserSettings *>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a tool bar widget with different buttons

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionRun);
    toolBarWidget->addAction(mGui->actionPause);
    toolBarWidget->addAction(mGui->actionStop);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionDebugMode);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionAdd);
    toolBarWidget->addAction(mGui->actionRemove);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionCsvExport);

    mGui->actionPause->setVisible(false);

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

    mProgressBar = new QProgressBar(this);

    mProgressBar->setAlignment(Qt::AlignCenter);
    mProgressBar->setFixedHeight(3);
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
    // Delete our model settings

    foreach (SingleCellSimulationViewWidgetUserSettings *modelSettings, mModelUserSettings)
        delete modelSettings;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewWidget::retranslateUi()
{
    // Retranslate the whole view

    mGui->retranslateUi(this);

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

void SingleCellSimulationViewWidget::outputStatus(const QString &pStatus)
{
    // Move to the end of the output (just in case...)

    mOutput->moveCursor(QTextCursor::End);

    // Make sure that the output ends as expected and if not then add BrLn to it

    static const QString EndOfOutput = "<br /></p></body></html>";

    if (mOutput->toHtml().right(EndOfOutput.size()).compare(EndOfOutput))
        mOutput->insertHtml(OutputBrLn);

    // Output the status and make sure it's visible

    mOutput->insertHtml(pStatus);
    mOutput->moveCursor(QTextCursor::End);
}

//==============================================================================

void SingleCellSimulationViewWidget::outputStatusError(const QString &pStatusError)
{
    // Output the status error

    outputStatus(OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+pStatusError+".</span>"+OutputBrLn);
}

//==============================================================================

void SingleCellSimulationViewWidget::outputStatusSimulationError(const QString &pStatusSimulationError)
{
    // Output the status simulation error

    outputStatusError(pStatusSimulationError);

    // Leave the simulation mode

    setSimulationMode(false);
}

//==============================================================================

void SingleCellSimulationViewWidget::setSimulationMode(const bool &pEnabled)
{
    // Hide our run action and show our pause action instead

    mGui->actionRun->setVisible(!pEnabled);
    mGui->actionPause->setVisible(pEnabled);

    // Enable or disable the user settings

    mContentsWidget->informationWidget()->simulationWidget()->setEnabled(!pEnabled);
}

//==============================================================================

void SingleCellSimulationViewWidget::initialize(const QString &pFileName)
{
    // Keep track of the user settings for the previous model, if any

    SingleCellSimulationViewSimulationInformationWidget *simulationSettings = mContentsWidget->informationWidget()->simulationWidget();

    if (mUserSettings)
        mUserSettings->get(simulationSettings);

    // Retrieve our settings related to the current model, if any

    mUserSettings = mModelUserSettings.value(pFileName);

    if (!mUserSettings) {
        // No user settings currently exist for the model, so create some,
        // initialise them and then keep track of them

        mUserSettings = new SingleCellSimulationViewWidgetUserSettings();

        mModelUserSettings.insert(pFileName, mUserSettings);
    }

    // (Re-)initialise our GUI with the user settings for the model

    mUserSettings->set(simulationSettings);

    // Get a runtime for the CellML file

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mCellmlFileRuntime = cellmlFile->runtime();

    QString status = QString();

    if (!mOutput->document()->isEmpty())
        status += "<hr/>\n";

    status += "<strong>"+pFileName+"</strong>"+OutputBrLn;
    status += OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

    if (mCellmlFileRuntime->isValid()) {
        QString additionalInformation = QString();

        if (mCellmlFileRuntime->needNlaSolver())
            additionalInformation = " + "+tr("Non-linear algebraic system(s)");

        status += "<span"+OutputGood+">"+tr("valid")+"</span>.<br/>\n";
        status += QString(OutputTab+"<strong>"+tr("Model type:")+"</strong> <span"+OutputInfo+">%1%2</span>."+OutputBrLn).arg((mCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?tr("ODE"):tr("DAE"),
                                                                                                                              additionalInformation);
    } else {
        status += "<span"+OutputBad+">"+tr("invalid")+"</span>."+OutputBrLn;

        foreach (const CellMLSupport::CellmlFileIssue &issue,
                 mCellmlFileRuntime->issues())
            status += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?tr("Error:"):tr("Warning:"),
                                                                                                            issue.message());
    }

    outputStatus(status);

    // Enable or disable the run and stop actions depending on whether the
    // runtime is valid

    CellMLSupport::CellmlFileVariable *variableOfIntegration = mCellmlFileRuntime->isValid()?mCellmlFileRuntime->variableOfIntegration():0;

    mGui->actionRun->setEnabled(variableOfIntegration);
    mGui->actionStop->setEnabled(variableOfIntegration);

    // By default, we assume that the runtime isn't valid or that there is no
    // variable of integration, meaning that that we can't show the unit of the
    // variable of integration

    simulationSettings->setUnit("???");

    // Check if we have an invalid runtime and, if so, set the unit to an
    // unknown one and leave

    if (!mCellmlFileRuntime->isValid())
        // Note: no need to output a status error since one will have already
        //       been generated while trying to get the runtime (see above)...

        return;

    // Retrieve the unit of the variable of integration, if any

    if (variableOfIntegration)
        // We have a variable of integration, so we can retrieve its unit

        simulationSettings->setUnit(mCellmlFileRuntime->variableOfIntegration()->unit());
    else
        // We don't have a variable of integration, so...

        outputStatusError(tr("the model must have at least one ODE or DAE"));
}

//==============================================================================

void SingleCellSimulationViewWidget::setProgressBarStyleSheet()
{
    // Customise our progress bar to be a very simple (and fast) one

    QPalette progressBarPalette = qApp->palette();
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
    // Go into simulation mode

    setSimulationMode(true);

    // Leave the simulation mode

    setSimulationMode(false);
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionPause_triggered()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionStop_triggered()
{
//---GRY--- TO BE DONE...
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

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Single cell simulation view widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlfileruntime.h"
#include "coredaesolver.h"
#include "corenlasolver.h"
#include "coreodesolver.h"
#include "coreutils.h"
#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewgraphpanelwidget.h"
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
#include <QTime>

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"

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
    Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewWidget),
    mCanSaveSettings(false),
    mCellmlFileRuntime(0), mModel(Unknown),
    mStatesCount(0), mCondVarCount(0),
    mConstants(0), mRates(0), mStates(0), mAlgebraic(0), mCondVar(0),
    mVoiEnd(0), mVoiStep(0), mVoiMaximumStep(0), mVoiOutput(0), mStatesIndex(0),
    mOdeSolverName("CVODE"),
    mSlowPlotting(true),
    mSolverInterfaces(SolverInterfaces()),
    mSolverErrorMsg(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a tool bar widget with different buttons

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionRun);
    toolBarWidget->addAction(mGui->actionStop);
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
    // Delete the arrays for our model

    delete[] mConstants;
    delete[] mRates;
    delete[] mStates;
    delete[] mAlgebraic;
    delete[] mCondVar;

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

    // Output the status and make sure it's visible

    mOutput->insertHtml(pStatus);
    mOutput->moveCursor(QTextCursor::End);
}

//==============================================================================

void SingleCellSimulationViewWidget::initialize(const QString &pFileName)
{
    // Get a runtime for the CellML file

    CellMLSupport::CellmlFile *mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mCellmlFileRuntime = mCellmlFile->runtime();

    QString status = QString();

    if (!mOutput->document()->isEmpty())
        status += "<hr/>\n";

    status += "<strong>"+pFileName+"</strong>"+OutputBrLn;
    status += OutputTab+"<strong>Runtime:</strong> ";

    if (mCellmlFileRuntime->isValid()) {
        QString additionalInformation = QString();

        if (mCellmlFileRuntime->needNlaSolver())
            additionalInformation = " + Non-linear algebraic system(s)";

        status += "<span"+OutputGood+">valid</span>.<br/>\n";
        status += QString(OutputTab+"<strong>Model type:</strong> <span"+OutputInfo+">%1%2</span>."+OutputBrLn).arg((mCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?"ODE":"DAE",
                                                                                                                    additionalInformation);
    } else {
        status += "<span"+OutputBad+">invalid</span>."+OutputBrLn;

        foreach (const CellMLSupport::CellmlFileIssue &issue,
                 mCellmlFileRuntime->issues())
            status += QString(OutputTab+"<span"+OutputBad+"><strong>%1:</strong> %2.</span>"+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?"Error":"Warning",
                                                                                                             issue.message());
    }

    outputStatus(status);

    // Leave if we got an invalid runtime

    if (!mCellmlFileRuntime->isValid())
        return;

    // Retrieve the unit of the variable of integration

//---GRY--- TO BE DONE...

    // Check whether we 'support' the model

    QString fileBaseName = QFileInfo(pFileName).baseName();

    if (!fileBaseName.compare("van_der_pol_model_1928")) {
        mModel = VanDerPol1928;
    } else if (   !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952")
               || !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952_modified")
               || !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952_original")) {
        mModel = Hodgkin1952;
    } else if (!fileBaseName.compare("noble_model_1962")) {
        mModel = Noble1962;
    } else if (!fileBaseName.compare("noble_noble_SAN_model_1984")) {
        mModel = Noble1984;
    } else if (!fileBaseName.compare("luo_rudy_1991")) {
        mModel = LuoRudy1991;
    } else if (!fileBaseName.compare("noble_model_1991")) {
        mModel = Noble1991;
    } else if (!fileBaseName.compare("noble_model_1998")) {
        mModel = Noble1998;
    } else if (   !fileBaseName.compare("zhang_SAN_model_2000_0D_capable")
               || !fileBaseName.compare("zhang_SAN_model_2000_1D_capable")
               || !fileBaseName.compare("zhang_SAN_model_2000_all")
               || !fileBaseName.compare("zhang_SAN_model_2000_published")) {
        mModel = Zhang2000;
    } else if (!fileBaseName.compare("mitchell_schaeffer_2003")) {
        mModel = Mitchell2003;
    } else if (   !fileBaseName.compare("cortassa_2006_1_0_S1")
               || !fileBaseName.compare("cortassa_2006_1_1_S1")) {
        mModel = Cortassa2006;
    } else if (!fileBaseName.compare("parabola_as_ode_model")) {
        mModel = Parabola;
    } else if (   !fileBaseName.compare("dae_model")
               || !fileBaseName.compare("newton_raphson_parabola_model")
               || !fileBaseName.compare("parabola_as_dae_model")
               || !fileBaseName.compare("parabola_variant_as_dae_model")
               || !fileBaseName.compare("saucerman_brunton_michailova_mcculloch_model_2003")
               || !fileBaseName.compare("simple_dae_model")) {
        mModel = Dae;
    } else {
        // The model is not 'supported', so...

        mModel = Unknown;

        outputStatus(OutputTab+"<span"+OutputBad+"><strong>Warning:</strong> the model is not 'supported'.</span>"+OutputBrLn);

        return;
    }

    // Delete the arrays for our model

    delete[] mConstants;
    delete[] mRates;
    delete[] mStates;
    delete[] mAlgebraic;
    delete[] mCondVar;

    // Create and initialise our arrays for our model

    int constantsCount = mCellmlFileRuntime->constantsCount();
    int ratesCount = mCellmlFileRuntime->ratesCount();
    mStatesCount = mCellmlFileRuntime->statesCount();
    int algebraicCount = mCellmlFileRuntime->algebraicCount();
    mCondVarCount = mCellmlFileRuntime->condVarCount();

    mConstants = new double[constantsCount];
    mRates     = new double[ratesCount];
    mStates    = new double[mStatesCount];
    mAlgebraic = new double[algebraicCount];
    mCondVar   = new double[mCondVarCount];

    for (int i = 0; i < constantsCount; ++i)
        mConstants[i] = 0;

    for (int i = 0; i < ratesCount; ++i)
        mRates[i] = 0;

    for (int i = 0; i < mStatesCount; ++i)
        mStates[i] = 0;

    for (int i = 0; i < algebraicCount; ++i)
        mAlgebraic[i] = 0;

    for (int i = 0; i < mCondVarCount; ++i)
        mCondVar[i] = 0;

    // Get some initial values for the ODE solver and our simulation in general

    mVoiMaximumStep = 0;

    switch (mModel) {
    case Hodgkin1952:
        mVoiEnd    = 50;      // ms
        mVoiStep   =  0.01;   // ms
        mVoiOutput =  0.1;    // ms

        break;
    case Noble1962:
    case LuoRudy1991:
    case Mitchell2003:
        mVoiEnd         = 1000;      // ms
        mVoiStep        =    0.01;   // ms
        mVoiOutput      =    1;      // ms
        mVoiMaximumStep =    1;      // ms

        break;
    case Noble1984:
    case Zhang2000:
        mVoiEnd    = 1;         // s
        mVoiStep   = 0.00001;   // s
        mVoiOutput = 0.001;     // s

        break;
    case Noble1991:
        mVoiEnd         = 1;         // s
        mVoiStep        = 0.00001;   // s
        mVoiOutput      = 0.001;     // s
        mVoiMaximumStep = 0.002;     // s

        break;
    case Noble1998:
        mVoiEnd         = 1;         // s
        mVoiStep        = 0.00001;   // s
        mVoiOutput      = 0.001;     // s
        mVoiMaximumStep = 0.003;     // s

        break;
    case Cortassa2006:
        mVoiEnd         = 300;      // dimensionless
        mVoiStep        =   0.01;   // dimensionless
        mVoiOutput      =   1;      // dimensionless
        mVoiMaximumStep =   0.5;    // dimensionless
        mStatesIndex    =   1;

        break;
    case Parabola:
    case Dae:
        mVoiEnd         = 10;     // dimensionless
        mVoiOutput      =  0.1;   // dimensionless
        mVoiMaximumStep =  1;     // dimensionless

        break;
    default:   // van der Pol 1928
        mVoiEnd    = 10;      // s
        mVoiStep   =  0.01;   // s
        mVoiOutput =  0.01;   // s
    }
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

void SingleCellSimulationViewWidget::outputSolverErrorMsg()
{
    // Output the current solver error message

    outputStatus(OutputTab+"<span"+OutputBad+"><strong"+OutputBad+">Solver error:</strong> "+mSolverErrorMsg+".</span>"+OutputBrLn);
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionRun_triggered()
{
    if ((mModel == Unknown) || !mCellmlFileRuntime->isValid())
        // The model is either not supported or not valid, so...

        return;

    // Clear the graph panels and output

    clearGraphPanels();

    // Retrieve the active graph panel

    SingleCellSimulationViewGraphPanelWidget *activeGraphPanel = mContentsWidget->activeGraphPanel();

    // Retrieve the requested ODE/DAE solver

    bool needOdeSolver = mModel != Dae;

    CoreSolver::CoreOdeSolver *odeSolver = 0;
    CoreSolver::CoreDaeSolver *daeSolver = 0;
    QString solverName = QString();

    if (needOdeSolver) {
        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare(mOdeSolverName)) {
                // The ODE solver was found, so retrieve an instance of it

                odeSolver = reinterpret_cast<CoreSolver::CoreOdeSolver *>(solverInterface->instance());
                solverName = solverInterface->name();

                break;
            }

        if (!odeSolver) {
            // The ODE solver couldn't be found, so...

            outputStatus(OutputTab+"<span"+OutputBad+"><strong>Error:</strong> the "+mOdeSolverName+" solver is needed, but it could not be found.</span>"+OutputBrLn);

            return;
        }
    } else {
        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare("IDA")) {
                // The DAE solver was found, so retrieve an instance of it

                daeSolver = reinterpret_cast<CoreSolver::CoreDaeSolver *>(solverInterface->instance());
                solverName = "IDA";

                break;
            }

        if (!daeSolver) {
            // The DAE solver couldn't be found, so...

            outputStatus(OutputTab+"<span"+OutputBad+"><strong>Error:</strong> the IDA solver is needed, but it could not be found.</span>"+OutputBrLn);

            return;
        }
    }

    // Check whether we need a non-linear algebraic solver

    if (mCellmlFileRuntime->needNlaSolver()) {
        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare("KINSOL")) {
                // The KINSOL solver was found, so retrieve an instance of it

                CoreSolver::setGlobalNlaSolver(reinterpret_cast<CoreSolver::CoreNlaSolver *>(solverInterface->instance()));

                break;
            }

        if (!CoreSolver::globalNlaSolver()) {
            // The NLA solver couldn't be found, so...

            outputStatus(OutputTab+"<span"+OutputBad+"><strong>Error:</strong> the KINSOL solver is needed, but it could not be found.</span>"+OutputBrLn);

            return;
        }
    }

    // Keep track of any error that might be reported by any of our solvers

    if (needOdeSolver)
        connect(odeSolver, SIGNAL(error(const QString &)),
                this, SLOT(solverError(const QString &)));
    else
        connect(daeSolver, SIGNAL(error(const QString &)),
                this, SLOT(solverError(const QString &)));

    if (CoreSolver::globalNlaSolver())
        connect(CoreSolver::globalNlaSolver(), SIGNAL(error(const QString &)),
                this, SLOT(solverError(const QString &)));

    mSolverErrorMsg = QString();

    // Get some initial values for the ODE solver and our simulation in general

    double voi = 0;
    double voiStart = voi;
    int voiOutputCount = 0;

    double hundredOverVoiEnd = 100/mVoiEnd;

    // Set the minimal and maximal value for the X axis

    activeGraphPanel->plot()->setAxisScale(QwtPlot::xBottom, voiStart, mVoiEnd);

    // Get some arrays to store the simulation data

    typedef QVector<double> Doubles;

    Doubles xData;
    Doubles yData;

    // Add a curve to our plotting area

    QwtPlotCurve *curve = activeGraphPanel->addCurve();

    // Retrieve the ODE functions from the CellML file runtime

    CellMLSupport::CellmlFileRuntime::OdeFunctions odeFunctions = mCellmlFileRuntime->odeFunctions();
    CellMLSupport::CellmlFileRuntime::DaeFunctions daeFunctions = mCellmlFileRuntime->daeFunctions();

    // Initialise the model's 'constants'

    if (needOdeSolver)
        odeFunctions.initializeConstants(mConstants, mRates, mStates);
    else
        daeFunctions.initializeConstants(mConstants, mRates, mStates);

    // Initialise our ODE/DAE solver

    if (needOdeSolver) {
        if (!solverName.compare("CVODE"))
            odeSolver->setProperty("Maximum step", mVoiMaximumStep);
        else
            odeSolver->setProperty("Step", mVoiStep);

        odeSolver->initialize(voi, mStatesCount, mConstants, mRates, mStates,
                              mAlgebraic, odeFunctions.computeRates);
    } else {
        daeSolver->setProperty("Maximum step", mVoiMaximumStep);

        daeSolver->initialize(voi, mVoiEnd, mStatesCount, mCondVarCount,
                              mConstants, mRates, mStates, mAlgebraic, mCondVar,
                              daeFunctions.computeEssentialVariables,
                              daeFunctions.computeResiduals,
                              daeFunctions.computeRootInformation,
                              daeFunctions.computeStateInformation);
        // Note: the second argument is just to provide the DAE solver with an
        //       indication of the direction in which we want to integrate the
        //       model, so that it can properly determine the model's initial
        //       conditions
    }

    // Initialise the constants and compute the rates and variables, but only if
    // the initialisation of the solver went fine

    if (!mSolverErrorMsg.isEmpty()) {
        // We couldn't initialise the solver, so...

        outputSolverErrorMsg();
    } else {
        QTime time;

        time.start();

        do {
            // Output the current simulation data after making sure that all the
            // variables have been computed

            if (needOdeSolver)
                odeFunctions.computeVariables(voi, mConstants, mRates, mStates, mAlgebraic);
            else
                daeFunctions.computeVariables(voi, mConstants, mRates, mStates, mAlgebraic);

            xData << voi;
            yData << mStates[mStatesIndex];

            // Make sure that the graph panel is up-to-date
            //---GRY--- NOT AT ALL THE WAY IT SHOULD BE DONE, BUT GOOD ENOUGH
            //          FOR DEMONSTRATION PURPOSES...

            if (mSlowPlotting) {
                curve->setSamples(xData, yData);

                activeGraphPanel->plot()->replot();
            }

            // Solve the model and compute its variables

            if (needOdeSolver)
                odeSolver->solve(voi, qMin(voiStart+(++voiOutputCount)*mVoiOutput, mVoiEnd));
            else
                daeSolver->solve(voi, qMin(voiStart+(++voiOutputCount)*mVoiOutput, mVoiEnd));

            // Update the progress bar
            //---GRY--- OUR USE OF QProgressBar IS VERY SLOW AT THE MOMENT...

            if (mSlowPlotting)
                mProgressBar->setValue(voi*hundredOverVoiEnd);
        } while ((voi != mVoiEnd) && mSolverErrorMsg.isEmpty());

        // Reset the progress bar

        mProgressBar->setValue(0);

        if (!mSolverErrorMsg.isEmpty()) {
            // The solver reported an error, so...

            outputSolverErrorMsg();

            // Make sure that the graph panel is up-to-date
            //---GRY--- NOT AT ALL THE WAY IT SHOULD BE DONE, BUT GOOD ENOUGH
            //          FOR DEMONSTRATION PURPOSES...

            if (!mSlowPlotting) {
                curve->setSamples(xData, yData);

                activeGraphPanel->plot()->replot();
            }
        } else {
            // Output the total simulation time

            outputStatus(QString(OutputTab+"<strong>Simulation time:</strong> <span"+OutputInfo+">%1 s</span>."+OutputBrLn).arg(QString::number(0.001*time.elapsed(), 'g', 3)));

            // Last bit of simulation data

            xData << voi;
            yData << mStates[mStatesIndex];

            // Make sure that the graph panel is up-to-date
            //---GRY--- NOT AT ALL THE WAY IT SHOULD BE DONE, BUT GOOD ENOUGH
            //          FOR DEMONSTRATION PURPOSES...

            curve->setSamples(xData, yData);

            activeGraphPanel->plot()->replot();
        }
    }

    // Delete the solver

    delete odeSolver;
    delete daeSolver;

    CoreSolver::resetGlobalNlaSolver();
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionDebugMode_triggered()
{
    //---GRY--- TEMPORARY WAY TO DETERMINE WHICH ODE SOLVER TO USE

    if (mGui->actionDebugMode->isChecked())
        mOdeSolverName = "Forward Euler";
    else
        mOdeSolverName = "CVODE";
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
    //---GRY--- TEMPORARY WAY TO SPECIFY WHETHER TO PLOT SLOWLY

    mSlowPlotting = !mSlowPlotting;
}

//==============================================================================

void SingleCellSimulationViewWidget::solverError(const QString &pErrorMsg)
{
    // The solver emitted an error, so...

    mSolverErrorMsg = pErrorMsg;
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

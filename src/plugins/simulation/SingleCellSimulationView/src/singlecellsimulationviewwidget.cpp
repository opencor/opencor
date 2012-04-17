//==============================================================================
// Single cell simulation view widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlfileruntime.h"
#include "coredaesolver.h"
#include "coreodesolver.h"
#include "coreutils.h"
#include "singlecellsimulationviewgraphpanel.h"
#include "singlecellsimulationviewgraphpanels.h"
#include "singlecellsimulationviewwidget.h"
#include "toolbar.h"

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

SingleCellSimulationViewWidget::SingleCellSimulationViewWidget(QWidget *pParent) :
    Widget(pParent),
    mUi(new Ui::SingleCellSimulationViewWidget),
    mCellmlFileRuntime(0), mModel(Unknown),
    mStatesCount(0), mCondVarCount(0),
    mConstants(0), mRates(0), mStates(0), mAlgebraic(0), mCondVar(0),
    mVoiEnd(0), mVoiStep(0), mVoiMaximumStep(0), mVoiOutput(0), mStatesIndex(0),
    mOdeSolverName("CVODE"),
    mSlowPlotting(true),
    mSolverInterfaces(SolverInterfaces()),
    mSolverErrorMsg(QString())
{
    // Set up the UI

    mUi->setupUi(this);

    // Create a toolbar with different buttons

    Core::ToolBar *toolbar = new Core::ToolBar(this);

    toolbar->addAction(mUi->actionRun);
    toolbar->addAction(mUi->actionStop);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionDebugMode);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionAdd);
    toolbar->addAction(mUi->actionRemove);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionCsvExport);

    mUi->verticalLayout->addWidget(toolbar);
    mUi->verticalLayout->addWidget(Core::newLineWidget(this));

    // Create our vertical splitter

    mVerticalSplitter = new QSplitter(Qt::Vertical, this);

    // Create a splitter for our graph panels and create a connection to keep
    // track of whether we can remove graph panels

    mGraphPanels = new SingleCellSimulationViewGraphPanels(this);

    mGraphPanels->setObjectName("GraphPanels");

    connect(mGraphPanels, SIGNAL(removeGraphPanelsEnabled(const bool &)),
            mUi->actionRemove, SLOT(setEnabled(bool)));

    // Create a simulation output widget with a vertical layout on which we put
    // a separating line and our simulation output list view
    // Note: the separating line is because we remove, for aesthetical reasons,
    //       the border of our simulation output list view...

    QWidget *simulationOutputWidget = new QWidget(this);
    QVBoxLayout *simulationOutputVerticalLayout= new QVBoxLayout(simulationOutputWidget);

    simulationOutputVerticalLayout->setContentsMargins(0, 0, 0, 0);
    simulationOutputVerticalLayout->setSpacing(0);

    simulationOutputWidget->setLayout(simulationOutputVerticalLayout);

    mOutput = new QTextEdit(this);

    mOutput->setFrameStyle(QFrame::NoFrame);

    simulationOutputVerticalLayout->addWidget(Core::newLineWidget(this));
    simulationOutputVerticalLayout->addWidget(mOutput);

    // Populate our vertical splitter and use as much space as possible for the
    // graph panels (by asking their height to be that of the desktop's), and
    // add it to our single cell simulation view widget
    // Note: we add some spacing before our vertical splitter to match the space
    //       that exists between the graph panels and the simulation output
    //       widget. So, yes, it's purely about aesthetic...

    mVerticalSplitter->addWidget(mGraphPanels);
    mVerticalSplitter->addWidget(simulationOutputWidget);

    mVerticalSplitter->setSizes(QList<int>() << qApp->desktop()->screenGeometry().height() << 1);

    mUi->verticalLayout->addSpacing(mVerticalSplitter->handleWidth());
    mUi->verticalLayout->addWidget(mVerticalSplitter);

    // Create our (thin) simulation progress widget

    mProgressBar = new QProgressBar(this);

    mProgressBar->setAlignment(Qt::AlignCenter);
    mProgressBar->setFixedHeight(3);
    mProgressBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mProgressBar->setTextVisible(false);

    setProgressBarStyleSheet();

    mUi->verticalLayout->addWidget(Core::newLineWidget(this));
    mUi->verticalLayout->addWidget(mProgressBar);
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

    // Delete the UI

    delete mUi;
}

//==============================================================================

void SingleCellSimulationViewWidget::retranslateUi()
{
    // Retranslate the whole view

    mUi->retranslateUi(this);
}

//==============================================================================

static const QString SettingsVerticalSplitterSizesCount = "GraphPanelsSizesCount";
static const QString SettingsVerticalSplitterSize       = "GraphPanelsSize%1";

//==============================================================================

void SingleCellSimulationViewWidget::loadSettings(QSettings *pSettings)
{
    foreach (SolverInterface *solverInterface, mSolverInterfaces) {
        qDebug("---------------------------------------");
        qDebug("'%s' solver:", qPrintable(solverInterface->name()));
        qDebug(" - Type: %s", (solverInterface->type() == Solver::Ode)?"ODE":"DAE");

        Solver::Properties properties = solverInterface->properties();

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
    }



    // Retrieve and set the sizes of our vertical splitter

    int sizesCount = pSettings->value(SettingsVerticalSplitterSizesCount, 0).toInt();

    if (sizesCount) {
        QList<int> sizes = QList<int>();

        for (int i = 0; i < sizesCount; ++i)
            sizes << pSettings->value(SettingsVerticalSplitterSize.arg(QString::number(i))).toInt();

        mVerticalSplitter->setSizes(sizes);
    }

    // Retrieve the settings of our graph panels widget

    pSettings->beginGroup(mGraphPanels->objectName());
        mGraphPanels->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our vertical splitter sizes

    QList<int> sizes = mVerticalSplitter->sizes();

    if (!sizes.count() || !sizes.first())
        // Either we have no vertical splitter sizes (how could this ever be the
        // case?!) or our first vertical splitter size has a value of zero
        // (which would mean that we previously left OpenCOR without going into
        // Simulation mode and the vertical splitter sizes are not meaningful),
        // so...

        return;

    pSettings->setValue(SettingsVerticalSplitterSizesCount, sizes.count());

    for (int i = 0, iMax = sizes.count(); i < iMax; ++i)
        pSettings->setValue(SettingsVerticalSplitterSize.arg(QString::number(i)), sizes.at(i));

    // Keep track of the settings of our graph panels widget

    pSettings->beginGroup(mGraphPanels->objectName());
        mGraphPanels->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewWidget::addSolverInterface(SolverInterface *pSolverInterface)
{
    // Add the solver interface to our list

    if (!mSolverInterfaces.contains(pSolverInterface))
        // The solver interface is not yet in our list, so...

        mSolverInterfaces.append(pSolverInterface);
}

//==============================================================================

void SingleCellSimulationViewWidget::clearGraphPanels()
{
    // Clear all the graph panels

    for (int i = 0, iMax = mGraphPanels->count(); i < iMax; ++i)
        qobject_cast<SingleCellSimulationViewGraphPanel *>(mGraphPanels->widget(i))->resetCurves();
}

//==============================================================================

void SingleCellSimulationViewWidget::clearActiveGraphPanel()
{
    // Clear the current graph panel

    mGraphPanels->activeGraphPanel()->resetCurves();
}

//==============================================================================

void SingleCellSimulationViewWidget::initialize(const QString &pFileName)
{
    // Clear the graph panels and output

    clearGraphPanels();

    mOutput->clear();

    // Get a runtime for the file

    mCellmlFileRuntime = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName)->runtime();

    if (mCellmlFileRuntime->isValid()) {
        mOutput->append(" - The CellML file's runtime was properly generated.");
        mOutput->append(QString("    [Information] Model type = %1").arg((mCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?"ODE":"DAE"));
    } else {
        mOutput->append(" - The CellML file's runtime was NOT properly generated:");

        foreach (const CellMLSupport::CellmlFileIssue &issue,
                 mCellmlFileRuntime->issues())
            mOutput->append(QString("    [%1] %2").arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?"Error":"Warning",
                                                       issue.formattedMessage()));

        // The runtime is not valid, so...

        return;
    }

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
    } else if (   !fileBaseName.compare("cortassa_2006_1_0")
               || !fileBaseName.compare("cortassa_2006_1_1")) {
        mModel = Cortessa2006;
    } else if (   !fileBaseName.compare("dae_model")
               || !fileBaseName.compare("newton_raphson_parabola_model")
               || !fileBaseName.compare("saucerman_brunton_michailova_mcculloch_model_2003")
               || !fileBaseName.compare("simple_dae_model")) {
        mModel = Dae;
    } else {
        // The model is not 'supported', so...

        mModel = Unknown;

        mOutput->append(" - The model is not 'supported'.");

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
    mStatesCount  = mCellmlFileRuntime->statesCount();
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
    case Cortessa2006:
        mVoiEnd         = 5000;      // dimensionless
        mVoiStep        =    0.01;   // dimensionless
        mVoiOutput      =    1;      // dimensionless
        mVoiMaximumStep =    0.5;    // dimensionless
        mStatesIndex    =    1;

        break;
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

//    mVoiEnd *= 10;
//---GRY--- JUST FOR TESTING...
}

//==============================================================================

void SingleCellSimulationViewWidget::setProgressBarStyleSheet()
{
    // Customise our progress bar to be a very simple (and fast) one

    QPalette progressBarPalette = qApp->palette();
    QColor progressBarBackground = progressBarPalette.color(QPalette::Window);
    QColor progressBarChunkBackground = progressBarPalette.color(QPalette::Highlight);

    mProgressBar->setStyleSheet(QString("QProgressBar {"
                                        "   background: rgb(%1, %2, %3);"
                                        "   border: 0px;"
                                        "}"
                                        ""
                                        "QProgressBar::chunk {"
                                        "   background: rgb(%4, %5, %6);"
                                        "   border: 0px;"
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

    Core::Widget::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update the colours
    // to be used by our progress bar

    if (pEvent->type() == QEvent::PaletteChange)
        setProgressBarStyleSheet();
}

//==============================================================================

void SingleCellSimulationViewWidget::outputSolverErrorMsg()
{
    // Output the current solver error message, if any

    if (!mSolverErrorMsg.isEmpty())
        mOutput->append(QString(" - Solver error: %1").arg(mSolverErrorMsg));
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionRun_triggered()
{
    if (mModel == Unknown)
        // The model is not supported, so...

        return;

    // Clear the graph panels and output

    clearGraphPanels();

    // Retrieve the active graph panel

    SingleCellSimulationViewGraphPanel *firstGraphPanel = qobject_cast<SingleCellSimulationViewGraphPanel *>(mGraphPanels->widget(0));

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

            mOutput->append(QString(" - The %1 solver is needed, but it could not be found.").arg(mOdeSolverName));

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

            mOutput->append(" - The IDA solver is needed, but it could not be found.");

            return;
        }
    }

    // Keep track of any error that might be reported by our solver

    if (needOdeSolver)
        connect(odeSolver, SIGNAL(error(const QString &)),
                this, SLOT(solverError(const QString &)));
    else
        connect(daeSolver, SIGNAL(error(const QString &)),
                this, SLOT(solverError(const QString &)));

    mSolverErrorMsg = QString();

    // Get some initial values for the ODE solver and our simulation in general

    double voi = 0;
    double voiStart = voi;
    int voiOutputCount = 0;

    double hundredOverVoiEnd = 100/mVoiEnd;

    // Set the minimal and maximal value for the X axis

    firstGraphPanel->plot()->setAxisScale(QwtPlot::xBottom, voiStart, mVoiEnd);

    // Get some arrays to store the simulation data

    typedef QVector<double> Doubles;

    Doubles xData;
    Doubles yData;

    // Add a curve to our plotting area

    QwtPlotCurve *curve = firstGraphPanel->addCurve();

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

        daeSolver->initialize(voi, mVoiEnd > 0,
                              mStatesCount, mCondVarCount, mConstants, mRates,
                              mStates, mAlgebraic, mCondVar,
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

            xData.append(voi);
            yData.append(mStates[mStatesIndex]);

            // Make sure that the graph panel is up-to-date
            //---GRY--- NOT AT ALL THE WAY IT SHOULD BE DONE, BUT GOOD ENOUGH
            //          FOR DEMONSTRATION PURPOSES...

            if (mSlowPlotting) {
                curve->setSamples(xData, yData);

                firstGraphPanel->plot()->replot();
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

                firstGraphPanel->plot()->replot();
            }
        } else {
            // Output the total simulation time

            mOutput->append(QString(" - Simulation time (using the %1 solver): %2 s").arg(solverName,
                                                                                          QString::number(0.001*time.elapsed(), 'g', 3)));

            // Last bit of simulation data

            xData.append(voi);
            yData.append(mStates[mStatesIndex]);

            // Make sure that the graph panel is up-to-date
            //---GRY--- NOT AT ALL THE WAY IT SHOULD BE DONE, BUT GOOD ENOUGH
            //          FOR DEMONSTRATION PURPOSES...

            curve->setSamples(xData, yData);

            firstGraphPanel->plot()->replot();
        }
    }

    // Delete the solver

    delete odeSolver;
    delete daeSolver;
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionDebugMode_triggered()
{
    //---GRY--- TEMPORARY WAY TO DETERMINE WHICH ODE SOLVER TO USE

    if (mUi->actionDebugMode->isChecked())
        mOdeSolverName = "Forward Euler";
    else
        mOdeSolverName = "CVODE";
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionAdd_triggered()
{
    // Add a new graph panel

    mGraphPanels->addGraphPanel();
}

//==============================================================================

void SingleCellSimulationViewWidget::on_actionRemove_triggered()
{
    // Remove the current graph panel

    mGraphPanels->removeGraphPanel();
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

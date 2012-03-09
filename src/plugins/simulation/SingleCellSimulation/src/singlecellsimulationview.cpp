//==============================================================================
// Single cell simulation view
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlfileruntime.h"
#include "coreodesolver.h"
#include "singlecellsimulationgraphpanel.h"
#include "singlecellsimulationgraphpanels.h"
#include "singlecellsimulationview.h"
#include "toolbar.h"

//==============================================================================

#include "ui_singlecellsimulationview.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
#include <QProgressBar>
#include <QSettings>
#include <QSplitter>
#include <QTextEdit>
#include <QTime>

//==============================================================================

#include "qwt_plot_curve.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

SingleCellSimulationView::SingleCellSimulationView(QWidget *pParent) :
    Widget(pParent),
    mUi(new Ui::SingleCellSimulationView),
    mSolverInterfaces(SolverInterfaces())
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
    mUi->verticalLayout->addWidget(newSeparatingLine());

    // Create our vertical splitter

    QSplitter *mainVerticalSplitter = new QSplitter(Qt::Vertical, this);

    // Create a splitter for our graph panels and create a connection to keep
    // track of whether we can remove graph panels

    mGraphPanels = new SingleCellSimulationGraphPanels("GraphPanels", this);

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

    simulationOutputVerticalLayout->addWidget(newSeparatingLine());
    simulationOutputVerticalLayout->addWidget(mOutput);

    // Populate our splitter and use as much space as possible for the graph
    // panels (by asking their height to be that of the desktop's) and add it to
    // our view

    mainVerticalSplitter->addWidget(mGraphPanels);
    mainVerticalSplitter->addWidget(simulationOutputWidget);

    mainVerticalSplitter->setSizes(QList<int>() << qApp->desktop()->screenGeometry().height() << 1);

    mUi->verticalLayout->addWidget(mainVerticalSplitter);

    // Create our simulation progress widget

    mProgressBar = new QProgressBar(this);

    mProgressBar->setAlignment(Qt::AlignCenter);

    mUi->verticalLayout->addWidget(newSeparatingLine());
    mUi->verticalLayout->addWidget(mProgressBar);
}

//==============================================================================

SingleCellSimulationView::~SingleCellSimulationView()
{
    // Delete the UI

    delete mUi;
}

//==============================================================================

void SingleCellSimulationView::retranslateUi()
{
    // Retranslate the whole view

    mUi->retranslateUi(this);
}

//==============================================================================

void SingleCellSimulationView::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our graph panels widget

    pSettings->beginGroup(mGraphPanels->objectName());
        mGraphPanels->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationView::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our graph panels widget

    pSettings->beginGroup(mGraphPanels->objectName());
        mGraphPanels->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationView::addSolverInterface(SolverInterface *pSolverInterface)
{
    // Add the solver interface to our list

    if (!mSolverInterfaces.contains(pSolverInterface))
        // The solver interface is not yet in our list, so...

        mSolverInterfaces.append(pSolverInterface);
}

//==============================================================================

QFrame * SingleCellSimulationView::newSeparatingLine()
{
    // Return a separating line widget

    QFrame *res = new QFrame(this);

    res->setFrameShape(QFrame::HLine);
    res->setFrameShadow(QFrame::Sunken);

    return res;
}

//==============================================================================

void SingleCellSimulationView::updateWith(const QString &pFileName)
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



    mOutput->clear();
    mOutput->append(QString("%1:").arg(pFileName));

    // Retrieve the active graph panel

    SingleCellSimulationGraphPanel *activeGraphPanel = mGraphPanels->activeGraphPanel();

    // Remove any existing curve

    activeGraphPanel->resetCurves();

    // Check whether we 'support' the model

    enum Model {
        Unknown,
        VanDerPol1928,
        Hodgkin1952,
        Noble1962,
        Noble1984,
        Noble1991,
        Noble1998,
        Zhang2000,
        Mitchell2003
    } model;

    QString fileBaseName = QFileInfo(pFileName).baseName();

    if (!fileBaseName.compare("van_der_pol_model_1928")) {
        model = VanDerPol1928;
    } else if (   !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952")
               || !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952_modified")
               || !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952_original")) {
        model = Hodgkin1952;
    } else if (!fileBaseName.compare("noble_model_1962")) {
        model = Noble1962;
    } else if (!fileBaseName.compare("noble_noble_SAN_model_1984")) {
        model = Noble1984;
    } else if (!fileBaseName.compare("noble_model_1991")) {
        model = Noble1991;
    } else if (!fileBaseName.compare("noble_model_1998")) {
        model = Noble1998;
    } else if (   !fileBaseName.compare("zhang_SAN_model_2000_0D_capable")
               || !fileBaseName.compare("zhang_SAN_model_2000_1D_capable")
               || !fileBaseName.compare("zhang_SAN_model_2000_all")
               || !fileBaseName.compare("zhang_SAN_model_2000_published")) {
        model = Zhang2000;
    } else if (!fileBaseName.compare("mitchell_schaeffer_2003")) {
        model = Mitchell2003;
    } else {
        // The model is not 'supported', so...

        mOutput->append(" - The model is not 'supported'.");

        return;
    }

    // Make sure that either the CVODE or the Forward Euler solver is available

    CoreSolver::CoreOdeSolver *odeSolver = 0;
    QString odeSolverName = QString();

    foreach (SolverInterface *solverInterface, mSolverInterfaces)
        if (!solverInterface->name().compare("CVODE")) {
            // The CVODE solver could be found, so retrieve an instance of it

            odeSolver = reinterpret_cast<CoreSolver::CoreOdeSolver *>(solverInterface->instance());
            odeSolverName = solverInterface->name();

            break;
        }

    if (!odeSolver)
        // The CVODE solver is not available, so try for the Forward Euler
        // solver

        foreach (SolverInterface *solverInterface, mSolverInterfaces)
            if (!solverInterface->name().compare("Forward Euler")) {
                // The Forward Euler solver could be found, so retrieve an
                // instance of it

                odeSolver = reinterpret_cast<CoreSolver::CoreOdeSolver *>(solverInterface->instance());
                odeSolverName = solverInterface->name();

                break;
            }

    if (!odeSolver) {
        // Neither the CVODE nor the Forward Euler solver could be found, so...

        mOutput->append(" - Either the CVODE or Forward Euler solver is needed, but neither could be found.");

        return;
    }

    // Get a runtime for the file

    CellMLSupport::CellmlFileRuntime *cellmlFileRuntime = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName)->runtime();

    if (cellmlFileRuntime->isValid()) {
        mOutput->append(" - The CellML file's runtime was properly generated.");
        mOutput->append(QString("    [Information] Model type = %1").arg((cellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?"ODE":"DAE"));
    } else {
        mOutput->append(" - The CellML file's runtime was NOT properly generated:");

        foreach (const CellMLSupport::CellmlFileIssue &issue,
                 cellmlFileRuntime->issues())
            mOutput->append(QString("    [%1] %2").arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?"Error":"Warning",
                                                       issue.formattedMessage()));

        // The runtime is not valid, so...

        return;
    }

    // Get some arrays for our model

    int statesCount = cellmlFileRuntime->statesCount();

    double *constants = new double[cellmlFileRuntime->constantsCount()];
    double *rates = new double[cellmlFileRuntime->ratesCount()];
    double *states = new double[statesCount];
    double *algebraic = new double[cellmlFileRuntime->algebraicCount()];

    // Get some arrays to store the simulation data

    typedef QVector<double> Doubles;

    Doubles xData;
    Doubles yData[statesCount];

    // Get some initial values for the ODE solver and our simulation in general

    double voi = 0;   // ms

    double voiStart = voi;   // ms
    double voiEnd;           // ms

    double voiStep;              // ms
    double voiMaximumStep = 0;   // ms


    double voiOutput;   // ms
    int voiOutputCount = 0;

    switch (model) {
    case Hodgkin1952:
        voiEnd    = 50;     // ms
        voiStep   = 0.01;   // ms
        voiOutput = 0.1;    // ms

        break;
    case Noble1962:
    case Mitchell2003:
        voiEnd         = 1000;   // ms
        voiStep        = 0.01;   // ms
        voiOutput      = 1;      // ms
        voiMaximumStep = 1;      // ms

        break;
    case Noble1984:
    case Zhang2000:
        voiEnd    = 1;         // s
        voiStep   = 0.00001;   // s
        voiOutput = 0.001;     // s

        break;
    case Noble1991:
        voiEnd         = 1;         // s
        voiStep        = 0.00001;   // s
        voiOutput      = 0.001;     // s
        voiMaximumStep = 0.002;     // s

        break;
    case Noble1998:
        voiEnd         = 1;         // s
        voiStep        = 0.00001;   // s
        voiOutput      = 0.001;     // s
        voiMaximumStep = 0.003;     // s

        break;
    default:   // van der Pol 1928
        voiEnd    = 10;     // s
        voiStep   = 0.01;   // s
        voiOutput = 0.01;   // s
    }

    // Initialise our ODE solver

    if (!odeSolverName.compare("CVODE"))
        odeSolver->setProperty("Maximum step", voiMaximumStep);
    else
        odeSolver->setProperty("Step", voiStep);

    odeSolver->initialize(voi, statesCount, constants, rates, states, algebraic);

    // Retrieve the ODE functions from the CellML file runtime

    CellMLSupport::CellmlFileRuntime::OdeFunctions odeFunctions = cellmlFileRuntime->odeFunctions();

    // Initialise the constants and compute the rates and variables

    QTime time;

    time.start();

    odeFunctions.initializeConstants(constants, rates, states);
    odeFunctions.computeRates(voi, constants, rates, states, algebraic);
    odeFunctions.computeVariables(voi, constants, rates, states, algebraic);

    do {
        // Output the current simulation data

        xData.append(voi);

        for (int i = 0; i < statesCount; ++i)
            yData[i].append(states[i]);

        // Solve the model

        odeSolver->solve(voi,
                         qMin(voiStart+(++voiOutputCount)*voiOutput, voiEnd),
                         odeFunctions.computeRates);

        odeFunctions.computeVariables(voi, constants, rates, states, algebraic);
    } while (voi != voiEnd);

    xData.append(voi);

    for (int i = 0; i < statesCount; ++i)
        yData[i].append(states[i]);

    mOutput->append(QString(" - Simulation time (using the %1 solver): %2 s").arg(odeSolverName,
                                                                                  QString::number(0.001*time.elapsed(), 'g', 3)));

    // Add some curves to our plotting area

    for (int i = 0, iMax = (model == VanDerPol1928)?statesCount:1; i < iMax; ++i) {
        QwtPlotCurve *curve = activeGraphPanel->addCurve();

        if (!i%2)
            curve->setPen(QPen(Qt::darkRed));

        curve->setSamples(xData, yData[i]);
    }

    // Make sure that the view is up-to-date

    activeGraphPanel->replot();

    // Free the memory we allocated to solve the model

    delete[] constants;
    delete[] rates;
    delete[] states;
    delete[] algebraic;

    delete odeSolver;
}

//==============================================================================

void SingleCellSimulationView::on_actionAdd_triggered()
{
    // Add a new graph panel

    mGraphPanels->addGraphPanel();
}

//==============================================================================

void SingleCellSimulationView::on_actionRemove_triggered()
{
    // Remove the current graph panel

    mGraphPanels->removeGraphPanel();
}

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

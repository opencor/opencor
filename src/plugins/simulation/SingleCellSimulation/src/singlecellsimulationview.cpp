//==============================================================================
// Single cell simulation view
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlfileruntime.h"
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
    qDebug(" - Type: %s", (solverInterface->type() == SolverInterface::Ode)?"ODE":"DAE");
}



    mOutput->clear();
    mOutput->append(QString("%1:").arg(pFileName));

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
    }

    // Retrieve the active graph panel

    SingleCellSimulationGraphPanel *activeGraphPanel = mGraphPanels->activeGraphPanel();

    if (!activeGraphPanel)
        return;

    // Remove any existing curve

    activeGraphPanel->resetCurves();

    // Compute the model, if supported

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

    if (!fileBaseName.compare("van_der_pol_model_1928"))
        model = VanDerPol1928;
    else if (   !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952")
             || !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952_modified")
             || !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952_original"))
        model = Hodgkin1952;
    else if (!fileBaseName.compare("noble_model_1962"))
        model = Noble1962;
    else if (!fileBaseName.compare("noble_noble_SAN_model_1984"))
        model = Noble1984;
    else if (!fileBaseName.compare("noble_model_1991"))
        model = Noble1991;
    else if (!fileBaseName.compare("noble_model_1998"))
        model = Noble1998;
    else if (   !fileBaseName.compare("zhang_SAN_model_2000_0D_capable")
             || !fileBaseName.compare("zhang_SAN_model_2000_1D_capable")
             || !fileBaseName.compare("zhang_SAN_model_2000_all")
             || !fileBaseName.compare("zhang_SAN_model_2000_published"))
        model = Zhang2000;
    else if (!fileBaseName.compare("mitchell_schaeffer_2003"))
        model = Mitchell2003;
    else
        model = Unknown;

    if (cellmlFileRuntime->isValid() && (model != Unknown)) {
        typedef QVector<double> Doubles;

        int statesCount = cellmlFileRuntime->statesCount();

        Doubles xData;
        Doubles yData[statesCount];

        double voi = 0;   // ms
        double voiStep;   // ms
        double voiMax;    // ms
        double constants[cellmlFileRuntime->constantsCount()];
        double rates[cellmlFileRuntime->ratesCount()];
        double states[statesCount];
        double algebraic[cellmlFileRuntime->algebraicCount()];
        int voiCount = 0;
        int voiOutputCount;   // ms

        switch (model) {
        case Hodgkin1952:
            voiStep        = 0.01;   // ms
            voiMax         = 50;     // ms
            voiOutputCount = 10;

            break;
        case Noble1962:
        case Mitchell2003:
            voiStep        = 0.01;   // ms
            voiMax         = 1000;   // ms
            voiOutputCount = 100;

            break;
        case Noble1984:
        case Noble1991:
        case Noble1998:
        case Zhang2000:
            voiStep        = 0.00001;   // s
            voiMax         = 1;         // s
            voiOutputCount = 100;

            break;
        default:   // van der Pol 1928
            voiStep        = 0.01;   // s
            voiMax         = 10;     // s
            voiOutputCount = 1;
        }

        CellMLSupport::CellmlFileRuntimeOdeFunctions odeFunctions = cellmlFileRuntime->odeFunctions();

        // Initialise the constants and compute the rates and variables

        QTime time;

        time.start();

        odeFunctions.initializeConstants(constants, rates, states);
        odeFunctions.computeRates(voi, constants, rates, states, algebraic);
        odeFunctions.computeVariables(voi, constants, rates, states, algebraic);

        do {
            // Output the current data, if needed

            if(voiCount % voiOutputCount == 0) {
                xData.append(voi);

                for (int i = 0; i < statesCount; ++i)
                    yData[i].append(states[i]);
            }

            // Compute the rates and variables

            odeFunctions.computeRates(voi, constants, rates, states, algebraic);
            odeFunctions.computeVariables(voi, constants, rates, states, algebraic);

            // Go to the next voiStep and integrate the states

            voi = ++voiCount*voiStep;

            for (int i = 0; i < statesCount; ++i)
                states[i] += voiStep*rates[i];
        } while (voi < voiMax);

        xData.append(voi);

        for (int i = 0; i < statesCount; ++i)
            yData[i].append(states[i]);

        mOutput->append(QString(" - Simulation time: %1 s").arg(QString::number(0.001*time.elapsed(), 'g', 3)));

        // Add some curves to our plotting area

        for (int i = 0, iMax = (model == VanDerPol1928)?statesCount:1; i < iMax; ++i) {
            QwtPlotCurve *curve = activeGraphPanel->addCurve();

            if (!i%2)
                curve->setPen(QPen(Qt::darkRed));

            curve->setSamples(xData, yData[i]);
        }

        // Make sure that the view is up-to-date

        activeGraphPanel->replot();
    }
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

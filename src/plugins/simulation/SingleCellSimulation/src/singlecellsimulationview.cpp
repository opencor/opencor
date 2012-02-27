//==============================================================================
// Single cell simulation view
//==============================================================================

#include "singlecellsimulationview.h"

//==============================================================================

#include "ui_singlecellsimulationview.h"

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"

//==============================================================================

//#include "llvm/Module.h"
//#include "llvm/ExecutionEngine/ExecutionEngine.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

SingleCellSimulationView::SingleCellSimulationView(QWidget *pParent) :
    QWidget(pParent),
    mUi(new Ui::SingleCellSimulationView)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create our simulation view widget

    mSimulationView = new QwtPlot(this);

    // Customise our simulation view widget

    mSimulationView->setCanvasBackground(Qt::white);

    // Remove the canvas' border as it otherwise looks odd, not to say ugly,
    // with one

    mSimulationView->setCanvasLineWidth(0);

    // Add a grid to our simulation view widget

    QwtPlotGrid *grid = new QwtPlotGrid;

    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));

    grid->attach(mSimulationView);

    // Add the simluation view to the view

    mUi->verticalLayout->addWidget(mSimulationView);
}

//==============================================================================

SingleCellSimulationView::~SingleCellSimulationView()
{
    // Delete some internal objects

    resetCurves();

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

void SingleCellSimulationView::resetCurves()
{
    // Remove any existing curve

    foreach (QwtPlotCurve *curve, mCurves) {
        curve->detach();

        delete curve;
    }

    mCurves.clear();

    // Refresh the view

    mSimulationView->replot();
}

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================



////--- TESTING --- BEGIN ---

//    qDebug("=======================================");
//    qDebug("%s:", qPrintable(pFileName));

//    // Get a runtime for the file

//    CellMLSupport::CellmlFileRuntime *cellmlFileRuntime = cellmlFile->runtime();

//    if (cellmlFileRuntime->isValid()) {
//        qDebug(" - The CellML file's runtime was properly generated.");
//        qDebug("    [Information] Model type = %s", (cellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?"ODE":"DAE");
//    } else {
//        qDebug(" - The CellML file's runtime was NOT properly generated:");

//        foreach (const CellMLSupport::CellmlFileIssue &issue,
//                 cellmlFileRuntime->issues())
//            qDebug("    [%s] %s",
//                   (issue.type() == CellMLSupport::CellmlFileIssue::Error)?"Error":"Warning",
//                   qPrintable(issue.formattedMessage()));
//    }

//    // Remove any existing curve

//    resetCurves();

//    // Compute the model, if supported

//    enum Model {
//        Unknown,
//        VanDerPol1928,
//        Hodgkin1952,
//        Noble1962,
//        Noble1984,
//        Noble1991,
//        Noble1998,
//        Zhang2000,
//        Mitchell2003
//    } model;

//    QString fileBaseName = QFileInfo(pFileName).baseName();

//    if (!fileBaseName.compare("van_der_pol_model_1928"))
//        model = VanDerPol1928;
//    else if (   !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952")
//             || !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952_modified")
//             || !fileBaseName.compare("hodgkin_huxley_squid_axon_model_1952_original"))
//        model = Hodgkin1952;
//    else if (!fileBaseName.compare("noble_model_1962"))
//        model = Noble1962;
//    else if (!fileBaseName.compare("noble_noble_SAN_model_1984"))
//        model = Noble1984;
//    else if (!fileBaseName.compare("noble_model_1991"))
//        model = Noble1991;
//    else if (!fileBaseName.compare("noble_model_1998"))
//        model = Noble1998;
//    else if (   !fileBaseName.compare("zhang_SAN_model_2000_0D_capable")
//             || !fileBaseName.compare("zhang_SAN_model_2000_1D_capable")
//             || !fileBaseName.compare("zhang_SAN_model_2000_all")
//             || !fileBaseName.compare("zhang_SAN_model_2000_published"))
//        model = Zhang2000;
//    else if (!fileBaseName.compare("mitchell_schaeffer_2003"))
//        model = Mitchell2003;
//    else
//        model = Unknown;

//    if (cellmlFileRuntime->isValid() && (model != Unknown)) {
//        typedef QVector<double> Doubles;

//        int statesCount = cellmlFileRuntime->statesCount();

//        Doubles xData;
//        Doubles yData[statesCount];

//        double voi = 0;   // ms
//        double voiStep;   // ms
//        double voiMax;    // ms
//        double constants[cellmlFileRuntime->constantsCount()];
//        double rates[cellmlFileRuntime->ratesCount()];
//        double states[statesCount];
//        double algebraic[cellmlFileRuntime->algebraicCount()];
//        int voiCount = 0;
//        int voiOutputCount;   // ms

//        switch (model) {
//        case Hodgkin1952:
//            voiStep        = 0.01;   // ms
//            voiMax         = 50;     // ms
//            voiOutputCount = 10;

//            break;
//        case Noble1962:
//        case Mitchell2003:
//            voiStep        = 0.01;   // ms
//            voiMax         = 1000;   // ms
//            voiOutputCount = 100;

//            break;
//        case Noble1984:
//        case Noble1991:
//        case Noble1998:
//        case Zhang2000:
//            voiStep        = 0.00001;   // s
//            voiMax         = 1;         // s
//            voiOutputCount = 100;

//            break;
//        default:   // van der Pol 1928
//            voiStep        = 0.01;   // s
//            voiMax         = 10;     // s
//            voiOutputCount = 1;
//        }

//        CellMLSupport::CellmlFileRuntimeOdeFunctions odeFunctions = cellmlFileRuntime->odeFunctions();

//        // Initialise the constants and compute the rates and variables

//        QTime time;

//        time.start();

//        odeFunctions.initializeConstants(constants, rates, states);
//        odeFunctions.computeRates(voi, constants, rates, states, algebraic);
//        odeFunctions.computeVariables(voi, constants, rates, states, algebraic);

//        do {
//            // Output the current data, if needed

//            if(voiCount % voiOutputCount == 0) {
//                xData.append(voi);

//                for (int i = 0; i < statesCount; ++i)
//                    yData[i].append(states[i]);
//            }

//            // Compute the rates and variables

//            odeFunctions.computeRates(voi, constants, rates, states, algebraic);
//            odeFunctions.computeVariables(voi, constants, rates, states, algebraic);

//            // Go to the next voiStep and integrate the states

//            voi = ++voiCount*voiStep;

//            for (int i = 0; i < statesCount; ++i)
//                states[i] += voiStep*rates[i];
//        } while (voi < voiMax);

//        xData.append(voi);

//        for (int i = 0; i < statesCount; ++i)
//            yData[i].append(states[i]);

//        qDebug(" - Simulation time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));

//        // Add some curves to our plotting area

//        for (int i = 0, iMax = (model == VanDerPol1928)?statesCount:1; i < iMax; ++i) {
//            QwtPlotCurve *curve = new QwtPlotCurve();

//            curve->setRenderHint(QwtPlotItem::RenderAntialiased);
//            curve->setPen(QPen(i%2?Qt::darkBlue:Qt::darkRed));

//            curve->setSamples(xData, yData[i]);

//            curve->attach(mSimulationView);

//            // Keep track of the curve

//            mCurves.append(curve);
//        }

//        // Make sure that the view is up-to-date

//        mSimulationView->replot();
//    }

////--- TESTING --- END ---

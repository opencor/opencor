//==============================================================================
// SingleCellSimulation plugin
//==============================================================================

#include "singlecellsimulationplugin.h"
#include "cellmlmodel.h"
#include "cellmlsupportplugin.h"

//==============================================================================

#include <QFileInfo>
#include <QMainWindow>
#include <QPen>
#include <QTime>
#include <QVector>

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"

//==============================================================================

#include "llvm/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

PLUGININFO_FUNC SingleCellSimulationPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to run single cell simulations");
    descriptions.insert("fr", "Une extension pour exécuter des simulations unicellulaires");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Simulation,
                      true,
                      QStringList() << "CoreSimulation" << "CellMLSupport" << "Qwt",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(SingleCellSimulation, SingleCellSimulationPlugin)

//==============================================================================

SingleCellSimulationPlugin::SingleCellSimulationPlugin()
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Simulation, 0);
}

//==============================================================================

void SingleCellSimulationPlugin::initialize()
{
    // Create our simulation view widget

    mSimulationView = new QwtPlot(mMainWindow);

    // Hide our simulation view widget since it may not initially be shown in
    // our central widget

    mSimulationView->setVisible(false);

    // Customise our simulation view widget

    mSimulationView->setCanvasBackground(Qt::white);

    // Remove the canvas' border as it otherwise looks odd, not to say ugly,
    // with one

    mSimulationView->setCanvasLineWidth(0);

    // Add a grid to our simulation view widget

    QwtPlotGrid *grid = new QwtPlotGrid;

    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));

    grid->attach(mSimulationView);
}

//==============================================================================

void SingleCellSimulationPlugin::finalize()
{
    // Delete some internal objects

    resetCurves();
}
//==============================================================================

void SingleCellSimulationPlugin::resetCurves()
{
    // Remove any existing curve

    foreach (QwtPlotCurve *curve, mCurves) {
        curve->detach();

        delete curve;
    }

    mCurves.clear();
}

//==============================================================================

QWidget * SingleCellSimulationPlugin::viewWidget(const QString &pFileName,
                                                 const int &)
{
    // Check that we are dealing with a CellML file and, if so, return our
    // generic simulation view widget

    if (QFileInfo(pFileName).suffix().compare(CellMLSupport::CellmlFileExtension))
        // Not the expected file extension, so...

        return 0;
    else
{
//--- TESTING --- BEGIN ---

        qDebug("=======================================");
        qDebug("%s:", qPrintable(pFileName));

        // Load the CellML model

        CellMLSupport::CellmlModel *cellmlModel = new CellMLSupport::CellmlModel(pFileName);

        // Check the model's validity

        if (cellmlModel->isValid()) {
            // The model is valid, but let's see whether warnings were generated

            int warningsCount = cellmlModel->issues().count();

            if (warningsCount)
                qDebug(" - The model was properly loaded:");
            else
                qDebug(" - The model was properly loaded.");
        } else {
            qDebug(" - The model was NOT properly loaded:");
        }

        // Output any warnings/errors that were generated

        foreach (const CellMLSupport::CellmlModelIssue &issue, cellmlModel->issues()) {
            QString type = QString((issue.type() == CellMLSupport::CellmlModelIssue::Error)?"Error":"Warrning");
            QString message = issue.formattedMessage();
            uint32_t line = issue.line();
            uint32_t column = issue.column();
            QString importedModel = issue.importedModel();

            if (line && column) {
                if (importedModel.isEmpty())
                    qDebug("    [%s at line %s column %s] %s", qPrintable(type),
                                                               qPrintable(QString::number(issue.line())),
                                                               qPrintable(QString::number(issue.column())),
                                                               qPrintable(message));
                else
                    qDebug("    [%s at line %s column %s from imported model %s] %s", qPrintable(type),
                                                                                      qPrintable(QString::number(issue.line())),
                                                                                      qPrintable(QString::number(issue.column())),
                                                                                      qPrintable(importedModel),
                                                                                      qPrintable(message));
            } else {
                if (importedModel.isEmpty())
                    qDebug("    [%s] %s", qPrintable(type),
                                          qPrintable(message));
                else
                    qDebug("    [%s from imported model %s] %s", qPrintable(type),
                                                                 qPrintable(importedModel),
                                                                 qPrintable(message));
            }
        }

        // Get a runtime for the model

        CellMLSupport::CellmlModelRuntime *cellmlModelRuntime = cellmlModel->runtime();

        if (cellmlModelRuntime->isValid()) {
            qDebug(" - The model's runtime was properly generated.");
            qDebug("    [Information] Model type = %s", (cellmlModelRuntime->modelType() == CellMLSupport::CellmlModelRuntime::Ode)?"ODE":"DAE");
        } else {
            qDebug(" - The model's runtime was NOT properly generated:");

            foreach (const CellMLSupport::CellmlModelIssue &issue,
                     cellmlModelRuntime->issues()) {
                QString type = QString((issue.type() == CellMLSupport::CellmlModelIssue::Error)?"Error":"Warrning");
                QString message = issue.formattedMessage();

                qDebug("    [%s] %s", qPrintable(type), qPrintable(message));
            }
        }

        // Compute the model, if supported

        enum Model {
            VanDerPol1928,
            Noble1962,
            Zhang2000,
            Unknown
        } model;

        QString fileBaseName = QFileInfo(pFileName).baseName();

        if (!fileBaseName.compare("van_der_pol_model_1928"))
            model = VanDerPol1928;
        else if (!fileBaseName.compare("noble_model_1962"))
            model = Noble1962;
        else if (!fileBaseName.compare("zhang_SAN_model_2000_1D_capable"))
            model = Zhang2000;

        if (model != Unknown) {
            typedef QVector<double> Doubles;

            int statesCount = cellmlModelRuntime->statesCount();

            Doubles xData;
            Doubles yData[statesCount];

            typedef void (*InitConstsFunction)(double *, double *, double *);
            typedef void (*RatesFunction)(double, double *, double *, double *, double *);

            double voi = 0;   // ms
            double voiStep;   // ms
            double voiMax;    // ms
            double constants[cellmlModelRuntime->constantsCount()];
            double rates[cellmlModelRuntime->ratesCount()];
            double states[statesCount];
            double algebraic[cellmlModelRuntime->algebraicCount()];
            int voiCount = 0;
            int voiOutputCount;   // ms
            double timeConversion;

            switch (model) {
            case Noble1962:
                voiStep        = 0.01;   // ms
                voiMax         = 1000;   // ms
                voiOutputCount = 100;
                timeConversion = 1;      // ms/ms

                break;
            case Zhang2000:
                voiStep        = 0.01;    // ms
                voiMax         = 1000;    // ms
                voiOutputCount = 100;
                timeConversion = 0.001;   // ms/s

                break;
            default:   // van Der Pol 1928
                voiStep        = 10;      // ms
                voiMax         = 10000;   // ms
                voiOutputCount = 1;
                timeConversion = 0.001;   // ms/s
            }

            CellMLSupport::CellmlModelRuntimeOdeFunctions odeFunctions = cellmlModelRuntime->odeFunctions();

            QTime time;

            time.start();

            odeFunctions.initConsts(constants, rates, states);

            do {
                // Output the current data, if needed

                if(voiCount % voiOutputCount == 0) {
                    xData.append(voi);

                    for (int i = 0; i < statesCount; ++i)
                        yData[i].append(states[i]);
                }

                // Compute the rates

                odeFunctions.rates(voi, constants, rates, states, algebraic);

                // Go to the next voiStep and integrate the states

                voi = ++voiCount*voiStep;

                for (int i = 0; i < statesCount; ++i)
                    states[i] += timeConversion*voiStep*rates[i];
                    // Note: the time conversion is because we may need to go
                    //       from, say, seconds to milliseconds...
            } while (voi < voiMax);

            xData.append(voi);

            for (int i = 0; i < statesCount; ++i)
                yData[i].append(states[i]);

            qDebug(" - Simulation time: %s s", qPrintable(QString::number(0.001*time.elapsed(), 'g', 3)));

            // Remove any existing curve

            resetCurves();

            // Add some curves to our plotting area

            for (int i = 0, iMax = (model == VanDerPol1928)?statesCount:1; i < iMax; ++i) {
                QwtPlotCurve *curve = new QwtPlotCurve();

                curve->setRenderHint(QwtPlotItem::RenderAntialiased);
                curve->setPen(QPen(i%2?Qt::darkBlue:Qt::darkRed));

                curve->setSamples(xData, yData[i]);

                curve->attach(mSimulationView);

                // Keep track of the curve

                mCurves.append(curve);
            }

            // Update the range for the axes

            mSimulationView->replot();
        }

        // Done with our testing, so...

        delete cellmlModel;

//--- TESTING --- END ---

        // The expected file extension, so return our generic simulation view
        // widget

        return mSimulationView;
}
}

//==============================================================================

QString SingleCellSimulationPlugin::viewName(const int &pViewIndex)
{
    // We have only one view, so return its name otherwise call the GuiInterface
    // implementation of viewName

    switch (pViewIndex) {
    case 0:
        return tr("Single Cell");
    default:
        return GuiInterface::viewName(pViewIndex);
    }
}

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

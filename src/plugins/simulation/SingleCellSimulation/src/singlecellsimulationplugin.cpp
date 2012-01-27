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
        qDebug("%s:", pFileName.toLatin1().constData());

        // Load the CellML model

        CellMLSupport::CellmlModel *cellmlModel = new CellMLSupport::CellmlModel(pFileName);

        // Check the model's validity

        if (cellmlModel->isValid()) {
            // The model is valid, but let's see whether warnings were generated

            int nbOfWarnings = cellmlModel->issues().count();

            if (nbOfWarnings)
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
                    qDebug("    [%s at line %s column %s] %s", type.toLatin1().constData(),
                                                               QString::number(issue.line()).toLatin1().constData(),
                                                               QString::number(issue.column()).toLatin1().constData(),
                                                               message.toUtf8().constData());
                else
                    qDebug("    [%s at line %s column %s from imported model %s] %s", type.toLatin1().constData(),
                                                                                      QString::number(issue.line()).toLatin1().constData(),
                                                                                      QString::number(issue.column()).toLatin1().constData(),
                                                                                      importedModel.toLatin1().constData(),
                                                                                      message.toUtf8().constData());
            } else {
                if (importedModel.isEmpty())
                    qDebug("    [%s] %s", type.toLatin1().constData(),
                                          message.toUtf8().constData());
                else
                    qDebug("    [%s from imported model %s] %s", type.toLatin1().constData(),
                                                                 importedModel.toLatin1().constData(),
                                                                 message.toUtf8().constData());
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

                qDebug("    [%s] %s", type.toLatin1().constData(),
                                      message.toUtf8().constData());
            }
        }

        // Compute the model

        if (!QFileInfo(pFileName).baseName().compare("van_der_pol_model_1928")) {
            typedef QVector<double> Doubles;

            static const int nbOfStates = 2;

            Doubles xData;
            Doubles yData[nbOfStates];

            typedef void (*InitConstsFunction)(double *, double *, double *);
            typedef void (*RatesFunction)(double, double *, double *, double *, double *);

            llvm::Function *initConstsFunction = cellmlModelRuntime->computerEngine()->module()->getFunction("initConsts");
            llvm::Function *ratesFunction      = cellmlModelRuntime->computerEngine()->module()->getFunction("rates");

            double voi = 0;   // ms
            static const double voiStep = 10;   // ms
            static const double voiMax = 10000;   // ms
            double constants[nbOfStates];
            double rates[nbOfStates];
            double states[nbOfStates];
            int voiNbOfSteps = 0;

            InitConstsFunction initConstsFunc = (InitConstsFunction)(intptr_t) cellmlModelRuntime->computerEngine()->executionEngine()->getPointerToFunction(initConstsFunction);
            RatesFunction ratesFunc           = (RatesFunction)(intptr_t) cellmlModelRuntime->computerEngine()->executionEngine()->getPointerToFunction(ratesFunction);

            initConstsFunc(constants, rates, states);

            do {
                // Output the current data

                xData.append(voi);

                for (int i = 0; i < nbOfStates; ++i)
                    yData[i].append(states[i]);

                // Compute the rates

                ratesFunc(voi, constants, rates, states, 0);

                // Go to the next voiStep and integrate the states

                voi = ++voiNbOfSteps*voiStep;

                for (int i = 0; i < nbOfStates; ++i)
                    states[i] += 0.001*voiStep*rates[i];
            } while (voi < voiMax);

            xData.append(voi);

            for (int i = 0; i < nbOfStates; ++i)
                yData[i].append(states[i]);

            // Add a curve to our view

            QwtPlotCurve *curve[nbOfStates];

            for (int i = 0; i < nbOfStates; ++i) {
                curve[i] = new QwtPlotCurve();

                curve[i]->setRenderHint(QwtPlotItem::RenderAntialiased);
                curve[i]->setPen(QPen(i?Qt::darkBlue:Qt::darkRed));

                curve[i]->setSamples(xData, yData[i]);

                curve[i]->attach(mSimulationView);
            }
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

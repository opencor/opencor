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

        typedef QVector<double> Doubles;

        static const int NbOfDataPoints = 100;
        static const double Factor      = 6.28/double(NbOfDataPoints-1);

        Doubles xData;
        Doubles yData;

        for (int i = 0; i < NbOfDataPoints; ++i) {
            xData.append(i*Factor);
            yData.append(sin(xData.last()));
        }

        // Add a curve to our view

        QwtPlotCurve *curve = new QwtPlotCurve;

        curve->setRenderHint(QwtPlotItem::RenderAntialiased);
        curve->setPen(QPen(Qt::darkBlue));
        curve->setSamples(xData, yData);

        curve->attach(mSimulationView);

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

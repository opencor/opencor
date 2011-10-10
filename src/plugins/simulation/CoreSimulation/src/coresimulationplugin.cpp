#include "coresimulationplugin.h"

#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QPen>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "qwt_plot_grid.h"

namespace OpenCOR {
namespace CoreSimulation {

PLUGININFO_FUNC CoreSimulationPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "The core simulation plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de simulation de base pour OpenCOR");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Simulation,
                      false,
                      QStringList() << "Core" << "Qwt",
                      descriptions);
}

Q_EXPORT_PLUGIN2(CoreSimulation, CoreSimulationPlugin)

void CoreSimulationPlugin::initialize()
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

    // Read the pre-computed CellML simulation results

    QFile mResultsFile;

    mResultsFile.setFileName(QDir::tempPath()+QDir::separator()+"test_cellml_results.csv");

    mResultsFile.open( QIODevice::ReadOnly);

    QByteArray dataLine;
    bool header = true;

    dataLine = mResultsFile.readLine();

    while (dataLine != QByteArray()) {
        if (header)
            // Ignore the header

            header = false;
        else {
            // Retrieve the time and membrane potential

            QStringList dataLineStringList = QString(dataLine).split(",");

            mXData << dataLineStringList.at(0).toDouble();
            mYData << dataLineStringList.at(1).toDouble();
        }

        // Read the next line

        dataLine = mResultsFile.readLine();
    };

    // Add a curve to the plotting widget

    QwtPlotCurve *curve = new QwtPlotCurve;

    curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve->setPen(QPen(Qt::darkBlue));
    curve->setSamples(mXData, mYData);

    curve->attach(mSimulationView);

    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Simulation);
    // Note: this won't so much set a new view (since the simulation mode should
    //       have only one view), but ensure that the simulation mode becomes
    //       available
}

QWidget * CoreSimulationPlugin::viewWidget(const QString &)
{
    // Always return our simulation view widget, no matter which file name is
    // being passed

    return mSimulationView;
}

} }

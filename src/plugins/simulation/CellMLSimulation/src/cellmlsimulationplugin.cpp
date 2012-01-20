//==============================================================================
// CellMLSimulation plugin
//==============================================================================

#include "cellmlsimulationplugin.h"
#include "cellmlsupportplugin.h"

//==============================================================================

#include <QFileInfo>
#include <QMainWindow>
#include <QPen>

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_grid.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSimulation {

//==============================================================================

PLUGININFO_FUNC CellMLSimulationPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to simulate <a href=\"http://www.cellml.org/\">CellML</a> files");
    descriptions.insert("fr", "Une extension pour simuler des fichiers <a href=\"http://www.cellml.org/\">CellML</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Simulation,
                      true,
                      QStringList() << "CoreSimulation" << "CellMLSupport" << "Qwt",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLSimulation, CellMLSimulationPlugin)

//==============================================================================

CellMLSimulationPlugin::CellMLSimulationPlugin()
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Simulation, 0);
}

//==============================================================================

void CellMLSimulationPlugin::initialize()
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

QWidget * CellMLSimulationPlugin::viewWidget(const QString &pFileName,
                                             const int &)
{
    // Check that we are dealing with a CellML file and, if so, return our
    // generic simulation view widget

    if (QFileInfo(pFileName).suffix().compare(CellMLSupport::CellmlFileExtension))
        // Not the expected file extension, so...

        return 0;
    else
        // The expected file extension, so return our generic simulation view
        // widget

        return mSimulationView;
}

//==============================================================================

QString CellMLSimulationPlugin::viewName(const int &pViewIndex)
{
    // We have only one view, so return its name otherwise call the GuiInterface
    // implementation of viewName

    switch (pViewIndex) {
    case 0:
        return tr("CellML");
    default:
        return GuiInterface::viewName(pViewIndex);
    }
}

//==============================================================================

}   // namespace CellMLSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Single cell simulation graph panel widget
//==============================================================================

#include "singlecellsimulationgraphpanel.h"

//==============================================================================

#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

SingleCellSimulationGraphPanel::SingleCellSimulationGraphPanel(QWidget *pParent) :
    QwtPlot(pParent)
{
    // Allow the graph panel to be of any vertical size

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

    // Have a white background by default

    setCanvasBackground(Qt::white);

    // Remove the canvas' border as it otherwise looks odd, not to say ugly,
    // with one

    setCanvasLineWidth(0);

    // Add a grid to our graph panel

    QwtPlotGrid *grid = new QwtPlotGrid;

    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));

    grid->attach(this);
}

//==============================================================================

SingleCellSimulationGraphPanel::~SingleCellSimulationGraphPanel()
{
    // Delete some internal objects

    resetCurves();
}

//==============================================================================

QwtPlotCurve * SingleCellSimulationGraphPanel::addCurve()
{
    // Create a new curve

    QwtPlotCurve *res = new QwtPlotCurve();

    // Customise it a bit

    res->setRenderHint(QwtPlotItem::RenderAntialiased);
    res->setPen(QPen(Qt::darkBlue));

    // Attach it to ourselves

    res->attach(this);

    // Add it to our list of curves

    mCurves.append(res);

    // Return it to the caller

    return res;
}

//==============================================================================

void SingleCellSimulationGraphPanel::resetCurves()
{
    // Remove any existing curve

    foreach (QwtPlotCurve *curve, mCurves) {
        curve->detach();

        delete curve;
    }

    mCurves.clear();

    // Refresh the graph panel

    replot();
}

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

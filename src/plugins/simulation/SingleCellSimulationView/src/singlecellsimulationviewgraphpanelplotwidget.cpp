//==============================================================================
// Single cell simulation view graph panel plot widget
//==============================================================================

#include "singlecellsimulationviewgraphpanelplotwidget.h"

//==============================================================================

#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewGraphPanelPlotWidget::SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent) :
    QwtPlot(pParent)
{
    // Customise ourselves a bit

    setAutoReplot(true);
    setCanvasBackground(Qt::white);

    // We don't want a frame around ourselves

    qobject_cast<QwtPlotCanvas *>(canvas())->setFrameShape(QFrame::NoFrame);

    // Attach a grid to ourselves

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajorPen(Qt::gray, 0, Qt::DotLine);

    grid->attach(this);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

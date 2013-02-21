//==============================================================================
// Single cell simulation view graph panel plot widget
//==============================================================================

#include "singlecellsimulationviewgraphpanelplotwidget.h"

//==============================================================================

#include <float.h>

//==============================================================================

#include "qwt_painter.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_directpainter.h"
#include "qwt_plot_grid.h"
#include "qwt_scale_div.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewGraphPanelPlotWidget::SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent) :
    QwtPlot(pParent)
{
    // Get ourselves a direct painter

    mDirectPainter = new QwtPlotDirectPainter(this);

    // Speedup painting on X11 systems
    // Note: this can only be done on X11 systems...

    if (QwtPainter::isX11GraphicsSystem())
        canvas()->setAttribute( Qt::WA_PaintOnScreen, true );

    // Customise ourselves a bit

    setCanvasBackground(Qt::white);

    // We don't want a frame around ourselves

    qobject_cast<QwtPlotCanvas *>(canvas())->setFrameShape(QFrame::NoFrame);

    // Attach a grid to ourselves

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajorPen(Qt::gray, 0, Qt::DotLine);

    grid->attach(this);
}

//==============================================================================

SingleCellSimulationViewGraphPanelPlotWidget::~SingleCellSimulationViewGraphPanelPlotWidget()
{
    // Delete some internal objects

    delete mDirectPainter;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::drawTraceSegment(QwtPlotCurve *pTrace,
                                                                    const qulonglong &pFrom,
                                                                    const qulonglong &pTo)
{
    // Determine the Y min/max of our new data

    double yMin =  DBL_MAX;
    double yMax = -DBL_MAX;

    for (qulonglong i = pFrom; i <= pTo; ++i) {
        double yVal = pTrace->data()->sample(i).y();

        yMin = qMin(yMin, yVal);
        yMax = qMax(yMax, yVal);
    }

    // Check whether our Y axis can handle the Y min/max of our new data

    if (   (yMin < axisScaleDiv(QwtPlot::yLeft).lowerBound())
        || (yMax > axisScaleDiv(QwtPlot::yLeft).upperBound())) {
        // Our Y axis cannot handle the Y min/max of our new data, so rescale it
        // by replot ourselves

        setAutoReplot(true);
            replot();
        setAutoReplot(false);
    } else {
        // Our Y axis can handle the Y min/max of our new data, so just draw our
        // new trace segment

        mDirectPainter->drawSeries(pTrace, pFrom, pTo);
    }
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Single cell simulation view graph panel plot widget
//==============================================================================

#include "singlecellsimulationviewgraphpanelplotwidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QMouseEvent>

//==============================================================================

#include <float.h>

//==============================================================================

#include "qwt_painter.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_directpainter.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_renderer.h"
#include "qwt_scale_div.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewGraphPanelPlotWidget::SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent) :
    QwtPlot(pParent),
    mAction(None),
    mOriginPoint(QPoint())
{
    // Get ourselves a direct painter

    mDirectPainter = new QwtPlotDirectPainter(this);

    // Speedup painting on X11 systems
    // Note: this can only be done on X11 systems...

    if (QwtPainter::isX11GraphicsSystem())
        canvas()->setAttribute(Qt::WA_PaintOnScreen, true);

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

bool SingleCellSimulationViewGraphPanelPlotWidget::eventFilter(QObject *pObject,
                                                               QEvent *pEvent)
{
    // Default handling of the event

    bool res = QwtPlot::eventFilter(pObject, pEvent);

    // We want to handle a double mouse click, but for some reasons to override
    // mouseDoubleClickEvent() doesn't work, so...

    if(pEvent->type() == QEvent::MouseButtonDblClick)
        handleMouseDoubleClickEvent(static_cast<QMouseEvent *>(pEvent));

    // We are all done, so...

    return res;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::handleMouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Copy the contents of the plot to the clipboard, in case we double-clicked
    // using the left mouse button

    if (pEvent->button() == Qt::LeftButton) {
        // Render the plot to an image

        QwtPlotRenderer renderer;
        QImage image = QImage(size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&image);

        renderer.render(this, &painter, rect());

        // Set the image to the clipboard

        QApplication::clipboard()->setImage(image);
    }
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::rescaleAxis(const int &pAxisId,
                                                               const double &pScalingFactor)
{
    // Rescale the given axis using the given scaling factor

    QwtScaleDiv scaleDiv = axisScaleDiv(pAxisId);
    double center = scaleDiv.lowerBound()+0.5*scaleDiv.range();
    double rangeOverTwo = 0.5*pScalingFactor*scaleDiv.range();

    setAxisScale(pAxisId, center-rangeOverTwo, center+rangeOverTwo);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Carry out the action

    switch (mAction) {
    case Zoom: {
        // Rescale our X axis, if needed

        static const double ScalingFactor = 0.95;

        int deltaX = pEvent->pos().x()-mOriginPoint.x();

        if (deltaX) {
            double scalingFactor = ScalingFactor;

            if (deltaX < 0)
                scalingFactor = 1.0/scalingFactor;

            rescaleAxis(QwtPlot::xBottom, scalingFactor);
        }

        // Rescale our Y axis, if needed

        int deltaY = pEvent->pos().y()-mOriginPoint.y();

        if (deltaY) {
            double scalingFactor = ScalingFactor;

            if (deltaY < 0)
                scalingFactor = 1.0/scalingFactor;

            rescaleAxis(QwtPlot::yLeft, scalingFactor);
        }

        // Replot ourselves and reset our point of origin, if needed

        if (deltaX || deltaY) {
            replot();

            mOriginPoint = pEvent->pos();
        }

        break;
    }
    default:
        // None

        ;
    }
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mousePressEvent(pEvent);

    // Check which action we can carry out

    if (   (pEvent->button() == Qt::RightButton)
        && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to zoom in/out

        mAction = Zoom;
    } else {
        // None of the actions we can carry out, so...

        mAction = None;

        return;
    }

    // Keep track of the mouse position and make sure that we track the mouse

    mOriginPoint = pEvent->pos();

    setMouseTracking(true);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseReleaseEvent(pEvent);

    // Check whether we need to carry out an action

    if (mAction == None)
        return;

    // Stop tracking the mouse

    setMouseTracking(false);

    // We are done carrying out an action, so...

    mAction = None;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::wheelEvent(pEvent);

    // The only action we support using the wheel is zooming in/out, but this
    // requires no modifiers being used

    if (pEvent->modifiers() != Qt::NoModifier)
        return;

    // Determine the zoom factor, making sure that it's valid

    static const double OneOverOneHundredAndTwenty = 1.0/120.0;

    double scalingFactor = qPow(0.9, qAbs(pEvent->delta()*OneOverOneHundredAndTwenty));

    if ((scalingFactor == 0.0) || (scalingFactor == 1.0))
        return;

    if (pEvent->delta() > 0)
        scalingFactor = 1.0/scalingFactor;

    // Rescale our two axes

    rescaleAxis(QwtPlot::xBottom, scalingFactor);
    rescaleAxis(QwtPlot::yLeft, scalingFactor);

    // Replot ourselves

    replot();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::drawTraceSegment(QwtPlotCurve *pTrace,
                                                                    const qulonglong &pFrom,
                                                                    const qulonglong &pTo)
{
    // Make sure that we have a trace segment to draw

    if (pFrom == pTo)
        return;

    // Determine the Y min/max of our new data

    double yMin =  DBL_MAX;
    double yMax = -DBL_MAX;

    for (qulonglong i = pFrom; i <= pTo; ++i) {
        double yVal = pTrace->data()->sample(i).y();

        yMin = qMin(yMin, yVal);
        yMax = qMax(yMax, yVal);
    }

    // Check which trace segment we are dealing with and whether our Y axis can
    // handle the Y min/max of our new data

    if (   !pFrom
        || (yMin < axisScaleDiv(QwtPlot::yLeft).lowerBound())
        || (yMax > axisScaleDiv(QwtPlot::yLeft).upperBound()))
        // Either it's our first trace segment and/or our Y axis cannot handle
        // the Y min/max of our new data, so replot ourselves

        replot();
    else
        // Our Y axis can handle the Y min/max of our new data, so just draw our
        // new trace segment

        mDirectPainter->drawSeries(pTrace, pFrom, pTo);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::replot()
{
    // Replot ourselves, making sure that our axes get rescaled (if needed)

    setAutoReplot(true);
        QwtPlot::replot();
    setAutoReplot(false);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
    mCurves(QList<QwtPlotCurve *>()),
    mAction(None),
    mOriginPoint(QPoint()),
    mMinFixedScaleX(0.0),
    mMaxFixedScaleX(0.0),
    mMinFixedScaleY(0.0),
    mMaxFixedScaleY(0.0),
    mInteractive(true)
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

    removeCurves();

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

void SingleCellSimulationViewGraphPanelPlotWidget::setFixedAxisScale(const Axis &pAxis,
                                                                     const double &pMin,
                                                                     const double &pMax)
{
    // Update our fixed scale information and axes scales

    if (pAxis == AxisX) {
        mMinFixedScaleX = pMin;
        mMaxFixedScaleX = pMax;

        if (pMin || pMax)
            // We want our X axis to have a fixed scale, so just set our axes
            // scales

            setAxesScales(pMin, pMax,
                          axisScaleDiv(QwtPlot::yLeft).lowerBound(), axisScaleDiv(QwtPlot::yLeft).upperBound());
        else
            // We want our X axis not to have a fixed scale, so just check that
            // our axes scales are fine

            checkAxesScales();
    } else {
        mMinFixedScaleY = pMin;
        mMaxFixedScaleY = pMax;

        if (pMin || pMax)
            // We want our X axis to have a fixed scale, so just set our axes
            // scales

            setAxesScales(axisScaleDiv(QwtPlot::xBottom).lowerBound(), axisScaleDiv(QwtPlot::xBottom).upperBound(),
                          pMin, pMax);
        else
            // We want our X axis not to have a fixed scale, so just check that
            // our axes scales are fine

            checkAxesScales();
    }
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setInteractive(const bool &pInteractive)
{
    // Make ourselves interactive or not

    mInteractive = pInteractive;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setAxesScales(const double &pMinX,
                                                                 const double &pMaxX,
                                                                 const double &pMinY,
                                                                 const double &pMaxY,
                                                                 const bool &pCanReplot)
{
    // Update our axes scales

    double oldMinX = axisScaleDiv(QwtPlot::xBottom).lowerBound();
    double oldMaxX = axisScaleDiv(QwtPlot::xBottom).upperBound();
    double oldMinY = axisScaleDiv(QwtPlot::yLeft).lowerBound();
    double oldMaxY = axisScaleDiv(QwtPlot::yLeft).upperBound();

    double newMinX = pMinX;
    double newMaxX = pMaxX;
    double newMinY = pMinY;
    double newMaxY = pMaxY;

    // Check whether our new axes scales should be fixed

    bool axisFixedX = mMinFixedScaleX || mMaxFixedScaleX;
    bool axisFixedY = mMinFixedScaleY || mMaxFixedScaleY;

    // Determine what our new axes scales really should be

    if ((!axisFixedX || !axisFixedY) && mCurves.count()) {
        // There is at least one of our axes which is not fixed and there is at
        // least one curve, so retrieve the bounding rectangle for all our
        // curves

        QRectF boundingRect = QRectF();

        foreach (QwtPlotCurve *curve, mCurves)
            if (curve->dataSize())
                boundingRect |= curve->boundingRect();

        if (boundingRect != QRectF()) {
            // We have a valid bounding rectangle, so update our new axes scales

            newMinX = qMax(pMinX, boundingRect.left());
            newMaxX = qMin(pMaxX, boundingRect.right());
            newMinY = qMax(pMinY, boundingRect.top());
            newMaxY = qMin(pMaxY, boundingRect.bottom());
        }
    }

    // Fix our axes scales, if needed

    if (axisFixedX) {
        newMinX = mMinFixedScaleX;
        newMaxX = mMaxFixedScaleX;
    }

    if (axisFixedY) {
        newMinY = mMinFixedScaleY;
        newMaxY = mMaxFixedScaleY;
    }

    // Update one/both of our axes scales and replot ourselves, if needed

    bool needReplot = false;

    if ((newMinX != oldMinX) || (newMaxX != oldMaxX)) {
        setAxisScale(QwtPlot::xBottom, newMinX, newMaxX);

        needReplot = true;
    }

    if ((newMinY != oldMinY) || (newMaxY != oldMaxY)) {
        setAxisScale(QwtPlot::yLeft, newMinY, newMaxY);

        needReplot = true;
    }

    if (needReplot && pCanReplot)
        replotNow();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::resetAxesScales()
{
    // Reset our axes scales

    setAxesScales(-DBL_MAX, DBL_MAX, -DBL_MAX, DBL_MAX);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::scaleAxesScales(const double &pScalingFactorX,
                                                                   const double &pScalingFactorY)
{
    // Determine the min/max values for our two axes

    QwtScaleDiv xScaleDiv = axisScaleDiv(QwtPlot::xBottom);
    double xCenter = xScaleDiv.lowerBound()+0.5*xScaleDiv.range();
    double xRangeOverTwo = 0.5*pScalingFactorX*xScaleDiv.range();

    QwtScaleDiv yScaleDiv = axisScaleDiv(QwtPlot::yLeft);
    double yCenter = yScaleDiv.lowerBound()+0.5*yScaleDiv.range();
    double yRangeOverTwo = 0.5*pScalingFactorY*yScaleDiv.range();

    // Rescale our two axes

    setAxesScales(xCenter-xRangeOverTwo, xCenter+xRangeOverTwo,
                  yCenter-yRangeOverTwo, yCenter+yRangeOverTwo);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::checkAxesScales(const bool &pCanReplot)
{
    // Check our axes scales by trying to set them

    setAxesScales(axisScaleDiv(QwtPlot::xBottom).lowerBound(),
                  axisScaleDiv(QwtPlot::xBottom).upperBound(),
                  axisScaleDiv(QwtPlot::yLeft).lowerBound(),
                  axisScaleDiv(QwtPlot::yLeft).upperBound(),
                  pCanReplot);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Carry out the action, but only if we allow interaction

    if (!mInteractive)
        return;

    switch (mAction) {
    case Zoom: {
        // Determine the scaling factor for our X axis

        static const double ScalingFactor = 0.95;

        int deltaX = pEvent->pos().x()-mOriginPoint.x();
        double xScalingFactor = ScalingFactor;

        if (!deltaX)
            xScalingFactor = 1.0;
        else if (deltaX < 0)
            xScalingFactor = 1.0/xScalingFactor;

        // Determine the scaling factor for our Y axis

        int deltaY = pEvent->pos().y()-mOriginPoint.y();
        double yScalingFactor = ScalingFactor;

        if (!deltaY)
            yScalingFactor = 1.0;
        else if (deltaY < 0)
            yScalingFactor = 1.0/yScalingFactor;

        // Rescale and replot ourselves, as well as reset our point of origin,
        // if needed

        if (deltaX || deltaY) {
            scaleAxesScales(xScalingFactor, yScalingFactor);

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

    // Check which action we can carry out, but only if we allow interaction

    if (!mInteractive)
        return;

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

    // Check whether we need to carry out an action, but only if we allow
    // interaction

    if (!mInteractive)
        return;

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
    // requires no modifiers being used, but only if we allow interaction

    if (!mInteractive)
        return;

    if (pEvent->modifiers() != Qt::NoModifier)
        return;

    // Determine the zoom factor, making sure that it's valid

    static const double OneOverOneHundredAndTwenty = 1.0/120.0;

    double scalingFactor = qPow(0.9, qAbs(pEvent->delta()*OneOverOneHundredAndTwenty));

    if ((scalingFactor == 0.0) || (scalingFactor == 1.0))
        return;

    if (pEvent->delta() > 0)
        scalingFactor = 1.0/scalingFactor;

    // Scale our two axes

    scaleAxesScales(scalingFactor, scalingFactor);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::replotNow()
{
    // Replot ourselves

    replot();

    // Make sure that the replotting occurs immediately
    // Note: this is needed when running a simulation since, otherwise,
    //       replotting won't occur immediately (because of threading)...

    qApp->processEvents();
}

//==============================================================================

QwtPlotCurve * SingleCellSimulationViewGraphPanelPlotWidget::addCurve(double *pX, double *pY,
                                                                      const qulonglong &pOriginalSize)
{
    // Create a new curve

    QwtPlotCurve *res = new QwtPlotCurve();

    // Customise it a bit

    res->setRenderHint(QwtPlotItem::RenderAntialiased);
    res->setPen(QPen(Qt::darkBlue));

    // Populate our curve

    res->setRawSamples(pX, pY, pOriginalSize);

    // Attach it to ourselves

    res->attach(this);

    // Add it to our list of curves

    mCurves << res;

    // Make sure that our axes scales are fine and replot ourselves, but only if
    // needed

    if (pOriginalSize) {
        // Set our axes scales so that we can see all the curves
        // Note: we are adding a curve, so we always want to replot, hence our
        //       passing false to setAxesScales()...

        QRectF boundingRect = QRectF();

        foreach (QwtPlotCurve *curve, mCurves)
            if (curve->dataSize())
                boundingRect |= curve->boundingRect();

        setAxesScales(boundingRect.left(), boundingRect.right(),
                      boundingRect.top(), boundingRect.bottom(),
                      false);

        replotNow();
    }

    // Return it to the caller

    return res;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::removeCurve(QwtPlotCurve *pCurve,
                                                               const bool &pReplot)
{
    // Make sure that we have a curve

    if (!pCurve)
        return;

    // Detach and then delete the curve

    pCurve->detach();

    delete pCurve;

    // Stop tracking the curve

    mCurves.removeOne(pCurve);

    // Make sure that our axes scales are fine and replot ourselves, if needed

    if (pReplot) {
        if (mCurves.count())
            checkAxesScales(false);

        replotNow();
    }
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::removeCurves()
{
    // Remove any existing curve

    foreach (QwtPlotCurve *curve, mCurves)
        removeCurve(curve, false);

    // Replot ourselves

    replotNow();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::drawCurveSegment(QwtPlotCurve *pCurve,
                                                                    const qulonglong &pFrom,
                                                                    const qulonglong &pTo)
{
    // Make sure that we have a curve segment to draw

    if (pFrom == pTo)
        return;

    // Determine the Y min/max of our new data

    double xMin =  DBL_MAX;
    double xMax = -DBL_MAX;
    double yMin =  DBL_MAX;
    double yMax = -DBL_MAX;

    for (qulonglong i = pFrom; i <= pTo; ++i) {
        double xVal = pCurve->data()->sample(i).x();
        double yVal = pCurve->data()->sample(i).y();

        xMin = qMin(xMin, xVal);
        xMax = qMax(xMax, xVal);

        yMin = qMin(yMin, yVal);
        yMax = qMax(yMax, yVal);
    }

    // Check which curve segment we are dealing with and whether our X/Y axis
    // can handle the X/Y min/max of our new data

    if (   !pFrom
        || (xMin < axisScaleDiv(QwtPlot::xBottom).lowerBound())
        || (xMax > axisScaleDiv(QwtPlot::xBottom).upperBound())
        || (yMin < axisScaleDiv(QwtPlot::yLeft).lowerBound())
        || (yMax > axisScaleDiv(QwtPlot::yLeft).upperBound())) {
        // Either it's our first curve segment and/or our X/Y axis cannot handle
        // the X/Y min/max of our new data, so check our axes scales and replot
        // ourselves

        resetAxesScales();
    } else {
        // Our X/Y axis can handle the X/Y min/max of our new data, so just draw
        // our new curve segment

        mDirectPainter->drawSeries(pCurve, pFrom, pTo);
    }
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

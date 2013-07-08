//==============================================================================
// Single cell view graph panel plot widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "singlecellviewgraphpanelplotwidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QMouseEvent>

//==============================================================================

#include <QtNumeric>

//==============================================================================

#include <float.h>

//==============================================================================

#include "qwt_painter.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_directpainter.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_layout.h"
#include "qwt_plot_renderer.h"
#include "qwt_scale_div.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewGraphPanelPlotGraph::SingleCellViewGraphPanelPlotGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                                                                     CellMLSupport::CellmlFileRuntimeParameter *pParameterY) :
    QwtPlotCurve(),
    mParameterX(pParameterX),
    mParameterY(pParameterY)
{
qDebug(">>> Creating graph [%p] for:", this);
qDebug(">>>  - X: %s", qPrintable(mParameterX->name()));
qDebug(">>>  - Y: %s", qPrintable(mParameterY->name()));
    // Customise ourselves a bit

    setPen(QPen(Qt::darkBlue));
    setRenderHint(QwtPlotItem::RenderAntialiased);
}

//==============================================================================

SingleCellViewGraphPanelPlotGraph::~SingleCellViewGraphPanelPlotGraph()
{
qDebug(">>> Deleting graph [%p]...", this);
}

//==============================================================================

static const double MinZoomFactor =    1.0;
static const double MaxZoomFactor = 1024.0;

//==============================================================================

SingleCellViewGraphPanelPlotWidget::SingleCellViewGraphPanelPlotWidget(QWidget *pParent) :
    QwtPlot(pParent),
    mGraphs(QList<SingleCellViewGraphPanelPlotGraph *>()),
    mInteractive(true),
    mAction(None),
    mOriginPoint(QPointF()),
    mEndPoint(QPointF()),
    mMinX(0.0),
    mMinY(0.0),
    mMaxX(1000.0),
    mMaxY(1000.0),
    mFixedAxisX(false),
    mFixedAxisY(false),
    mCanvasPixmap(QPixmap()),
    mZoomFactorX(MinZoomFactor),
    mZoomFactorY(MinZoomFactor)
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

    // Set our axes' local minimum/maximum values

    setLocalMinMaxX(mMinX, mMaxX);
    setLocalMinMaxY(mMinY, mMaxY);

    // Attach a grid to ourselves

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajorPen(Qt::gray, 0, Qt::DotLine);

    grid->attach(this);
}

//==============================================================================

SingleCellViewGraphPanelPlotWidget::~SingleCellViewGraphPanelPlotWidget()
{
    // Delete some internal objects

    delete mDirectPainter;
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::eventFilter(QObject *pObject,
                                                     QEvent *pEvent)
{
    // Default handling of the event

    bool res = QwtPlot::eventFilter(pObject, pEvent);

    // We want to handle a double mouse click, but for some reasons to override
    // mouseDoubleClickEvent() doesn't work, so...

    if (pEvent->type() == QEvent::MouseButtonDblClick)
        handleMouseDoubleClickEvent(static_cast<QMouseEvent *>(pEvent));

    // We are all done, so...

    return res;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::handleMouseDoubleClickEvent(QMouseEvent *pEvent)
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

double SingleCellViewGraphPanelPlotWidget::minX() const
{
    // Return our minimum X value

    return mMinX;
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::maxX() const
{
    // Return our maximum X value

    return mMaxX;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setMinMaxX(const double &pMinX,
                                                    const double &pMaxX)
{
    // Set our minimum/maximum X values

    mMinX = pMinX;
    mMaxX = pMaxX;

    // Make sure that our zoom factors are up-to-date

    updateZoomFactors();
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::minY() const
{
    // Return our minimum Y value

    return mMinY;
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::maxY() const
{
    // Return our maximum Y value

    return mMaxY;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setMinMaxY(const double &pMinY,
                                                    const double &pMaxY)
{
    // Set our minimum/maximum Y values

    mMinY = pMinY;
    mMaxY = pMaxY;

    // Make sure that our zoom factors are up-to-date

    updateZoomFactors();
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::updateZoomFactors()
{
    // Update our zoom factors
    // Note: ideally, we wouldn't keep track of them, but say that you are fully
    //       zoomed in, then to pan the plotting area might result in the 'new'
    //       zoom factors not being exactly the same as the 'old' ones (typical
    //       issue when using floating numbers). This, in turn, might result in
    //       the user being wrongly allowed to zoom in, so...

    mZoomFactorX = (mMaxX-mMinX)/(localMaxX()-localMinX());
    mZoomFactorY = (mMaxY-mMinY)/(localMaxY()-localMinY());
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setLocalAxis(const int &pAxis,
                                                      const double &pMin,
                                                      const double &pMax)
{
    // Set our local axes
    // Note #1: to use setAxisScale() on its own is not sufficient unless we
    //          were to replot ourselves immediately after, but we don't want to
    //          do that, so instead we also use setAxisScaleDiv() to make sure
    //          that our local axes are indeed taken into account (i.e. we can
    //          retrieve them using localMinX(), localMaxX(), localMinY() and
    //          localMaxY()). Also, we must call setAxisScaleDiv() before
    //          setAxisScale() to make sure that the axis data is not considered
    //          as valid which is important when it comes to plotting
    //          ourselves...
    // Note #2: the way QwtPlot create ticks for an axis means that it cannot
    //          handle an axis that would go from -DBL_MAX to DBL_MAX, even
    //          though it would be a valid axis, so we check that the axis fits
    //          within what we know works fine with QwtPlot...

    static const double DBL_MAX_AXIS = 0.3*DBL_MAX;

    setAxisScaleDiv(pAxis, QwtScaleDiv(qMax(-DBL_MAX_AXIS, pMin),
                                       qMin( DBL_MAX_AXIS, pMax)));
    setAxisScale(pAxis, qMax(-DBL_MAX_AXIS, pMin),
                        qMin( DBL_MAX_AXIS, pMax));

    // Make sure that our zoom factors are up-to-date

    updateZoomFactors();
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::checkLocalAxisValues(const int &pAxis,
                                                              double &pMin,
                                                              double &pMax)
{
    // Make sure that the minimum/maximum values of our local axis have a valid
    // zoom factor

    double min = (pAxis == QwtPlot::xBottom)?mMinX:mMinY;
    double max = (pAxis == QwtPlot::xBottom)?mMaxX:mMaxY;
    double zoomFactor = (max-min)/(pMax-pMin);

    if (zoomFactor < MinZoomFactor) {
        // The local axis' zoom factor is too small, so reset our
        // minimum/maximum values

        pMin = min;
        pMax = max;
    } else if (zoomFactor > MaxZoomFactor) {
        // The local axis' zoom factor is too big, so reset our minimum/maximum
        // values

        double length = (max-min)/MaxZoomFactor;

        pMin = qMax(min, 0.5*(pMin+pMax-length));
        pMax = qMin(max, pMin+length);
        pMin = pMax-length;
        // Note: the last statement is in case pMax has been set to max, in
        //       which case we need to update pMin...
    }
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::localMinX() const
{
    // Return our local minimum X value

    return axisScaleDiv(QwtPlot::xBottom).lowerBound();
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::localMaxX() const
{
    // Return our local maximum X value

    return axisScaleDiv(QwtPlot::xBottom).upperBound();
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setLocalMinMaxX(const double &pLocalMinX,
                                                         const double &pLocalMaxX)
{
    // Set our local minimum X value

    double locMinX = pLocalMinX;
    double locMaxX = pLocalMaxX;

    checkLocalAxisValues(QwtPlot::xBottom, locMinX, locMaxX);

    setLocalAxis(QwtPlot::xBottom, locMinX, locMaxX);
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::localMinY() const
{
    // Return our local minimum Y value

    return axisScaleDiv(QwtPlot::yLeft).lowerBound();
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::localMaxY() const
{
    // Return our local maximum Y value

    return axisScaleDiv(QwtPlot::yLeft).upperBound();
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setLocalMinMaxY(const double &pLocalMinY,
                                                         const double &pLocalMaxY)
{
    // Set our local minimum Y value

    double locMinY = pLocalMinY;
    double locMaxY = pLocalMaxY;

    checkLocalAxisValues(QwtPlot::yLeft, locMinY, locMaxY);

    setLocalAxis(QwtPlot::yLeft, locMinY, locMaxY);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setFixedAxisX(const bool &pFixedAxisX)
{
    // Specify whether our X axis is to be fixed

    mFixedAxisX = pFixedAxisX;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setFixedAxisY(const bool &pFixedAxisY)
{
    // Specify whether our Y axis is to be fixed

    mFixedAxisY = pFixedAxisY;
}

//==============================================================================

QList<SingleCellViewGraphPanelPlotGraph *> SingleCellViewGraphPanelPlotWidget::graphs() const
{
    // Return all our graphs

    return mGraphs;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setInteractive(const bool &pInteractive)
{
    // Specify whether interaction is allowed

    mInteractive = pInteractive;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::checkAnyAxesValues(double &pMinX,
                                                            double &pMaxX,
                                                            double &pMinY,
                                                            double &pMaxY)
{
    // Make sure that the minimum/maximum values of our (local) axes have finite
    // values

    if (!qIsFinite(pMinX))
        pMinX = -DBL_MAX;

    if (!qIsFinite(pMaxX))
        pMaxX = DBL_MAX;

    if (!qIsFinite(pMinY))
        pMinY = -DBL_MAX;

    if (!qIsFinite(pMaxY))
        pMaxY = DBL_MAX;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setLocalAxes(const double &pLocalMinX,
                                                      const double &pLocalMaxX,
                                                      const double &pLocalMinY,
                                                      const double &pLocalMaxY,
                                                      const bool &pCanReplot,
                                                      const bool &pForceMinMaxValues,
                                                      const bool &pUpdateMinMaxValues,
                                                      const bool &pResetMinMaxValues)
{
    // Update our axes

    double oldLocalMinX = localMinX();
    double oldLocalMaxX = localMaxX();
    double oldLocalMinY = localMinY();
    double oldLocalMaxY = localMaxY();

    double newLocalMinX = pLocalMinX;
    double newLocalMaxX = pLocalMaxX;
    double newLocalMinY = pLocalMinY;
    double newLocalMaxY = pLocalMaxY;

    // Retrieve the bounding rectangle for all our graphs (but only for those
    // that have some data)

    QRectF boundingRect = QRectF();

    foreach (SingleCellViewGraphPanelPlotGraph *graph, mGraphs)
        if (graph->dataSize())
            boundingRect |= graph->boundingRect();

    // Update the minimum/maximum values of our axes, should we have retrieved a
    // valid bounding rectangle

    if (boundingRect != QRectF()) {
        // Optimise our bounding rectangle by first retrieving the
        // minimum/maximum values of our axes

        double xMin = boundingRect.left();
        double xMax = boundingRect.right();
        double yMin = boundingRect.top();
        double yMax = boundingRect.bottom();

        // Make sure that the minimum/maximum values of our axes have finite
        // values

        checkAnyAxesValues(xMin, xMax, yMin, yMax);

        // Optimise the minimum/maximum values of our axes by rounding them
        // down/up, respectively

        double powerX = qPow(10.0, qMax(xMin?qFloor(log10(qAbs(xMin))):0, xMax?qFloor(log10(qAbs(xMax))):0));
        double powerY = qPow(10.0, qMax(yMin?qFloor(log10(qAbs(yMin))):0, yMax?qFloor(log10(qAbs(yMax))):0));

        xMin = powerX*qFloor(xMin/powerX);
        xMax = powerX*qCeil(xMax/powerX);
        yMin = powerY*qFloor(yMin/powerY);
        yMax = powerY*qCeil(yMax/powerY);

        // Make sure that the optimised minimum/maximum values of our axes have
        // finite values

        checkAnyAxesValues(xMin, xMax, yMin, yMax);

        // Update the minimum/maximum values of our axes, if required

        if (!mFixedAxisX) {
            if (pResetMinMaxValues)
                setMinMaxX(xMin, xMax);
            else if (pUpdateMinMaxValues)
                setMinMaxX(qMin(mMinX, xMin), qMax(mMaxX, xMax));
        }

        if (!mFixedAxisY) {
            if (pResetMinMaxValues)
                setMinMaxY(yMin, yMax);
            else if (pUpdateMinMaxValues)
                setMinMaxY(qMin(mMinY, yMin), qMax(mMaxY, yMax));
        }
    }

    // Make sure that the new minimum/maximum values of our local axes fit
    // within the minimum/maximum values of our axes

    if (pForceMinMaxValues) {
        newLocalMinX = mMinX;
        newLocalMaxX = mMaxX;
        newLocalMinY = mMinY;
        newLocalMaxY = mMaxY;
    } else {
        newLocalMinX = qMax(newLocalMinX, mMinX);
        newLocalMaxX = qMin(newLocalMaxX, mMaxX);
        newLocalMinY = qMax(newLocalMinY, mMinY);
        newLocalMaxY = qMin(newLocalMaxY, mMaxY);
    }

    // Make sure that the new minimum/maximum values of our local axes have
    // finite values

    checkAnyAxesValues(newLocalMinX, newLocalMaxX, newLocalMinY, newLocalMaxY);

    // Make sure that the new minimum/maximum values of our local axes have a
    // valid zoom factor

    checkLocalAxisValues(QwtPlot::xBottom, newLocalMinX, newLocalMaxX);
    checkLocalAxisValues(QwtPlot::yLeft, newLocalMinY, newLocalMaxY);

    // Update the minimum/maximum values of our local axes, if needed

    bool needReplot = false;

    if ((newLocalMinX != oldLocalMinX) || (newLocalMaxX != oldLocalMaxX)) {
        setLocalMinMaxX(newLocalMinX, newLocalMaxX);

        needReplot = true;
    }

    if ((newLocalMinY != oldLocalMinY) || (newLocalMaxY != oldLocalMaxY)) {
        setLocalMinMaxY(newLocalMinY, newLocalMaxY);

        needReplot = true;
    }

    // Replot ourselves, if needed and allowed

    if (needReplot && pCanReplot)
        replotNow();
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::scaleLocalAxes(const double &pScalingFactorX,
                                                        const double &pScalingFactorY)
{
    // Rescale our X axis, but only if zooming in/out is possible on that axis

    bool needRescaling = false;

    double xMin = localMinX();
    double xMax = localMaxX();

    if (   ((pScalingFactorX < 1.0) && (mZoomFactorX < MaxZoomFactor))
        || ((pScalingFactorX > 1.0) && (mZoomFactorX > MinZoomFactor))) {
        double rangeX = pScalingFactorX*(xMax-xMin);

        xMin = qMax(mMinX, mOriginPoint.x()-0.5*rangeX);
        xMax = qMin(mMaxX, xMin+rangeX);
        xMin = xMax-rangeX;
        // Note: the last statement is in case xMax has been set to mMaxX, in
        //       which case we need to update xMin...

        needRescaling = true;
    }

    // Rescale our Y axis, but only if zooming in/out is possible on that axis

    double yMin = localMinY();
    double yMax = localMaxY();

    if (   ((pScalingFactorY < 1.0) && (mZoomFactorY < MaxZoomFactor))
        || ((pScalingFactorY > 1.0) && (mZoomFactorY > MinZoomFactor))) {
        double rangeY = pScalingFactorY*(yMax-yMin);

        yMin = qMax(mMinY, mOriginPoint.y()-0.5*rangeY);
        yMax = qMin(mMaxY, yMin+rangeY);
        yMin = yMax-rangeY;
        // Note: the last statement is in case yMax has been set to mMaxY, in
        //       which case we need to update yMin...

        needRescaling = true;
    }

    // Rescale our two local axes, if needed

    if (needRescaling)
        setLocalAxes(xMin, xMax, yMin, yMax);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::checkLocalAxes(const bool &pCanReplot,
                                                        const bool &pForceMinMaxValues,
                                                        const bool &pUpdateMinMaxValues)
{
    // Check our local axes by trying to set them

    setLocalAxes(localMinX(), localMaxX(), localMinY(), localMaxY(),
                 pCanReplot, pForceMinMaxValues, pUpdateMinMaxValues);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::resetLocalAxes(const bool &pCanReplot)
{
    // Reset our local axes by trying to set them

    setLocalAxes(0.0, 0.0, 0.0, 0.0, pCanReplot, true, false, true);
}

//==============================================================================

static const double NoScalingFactor  = 1.0;
static const double ScalingInFactor  = 0.57;
static const double ScalingOutFactor = 1.0/ScalingInFactor;

//==============================================================================

QPointF SingleCellViewGraphPanelPlotWidget::mousePositionWithinCanvas(const QPoint &pPoint) const
{
    // Return the mouse position relative to our canvas, after making sure that
    // its mapped values are within our local ranges

    QPoint realPoint = pPoint-plotLayout()->canvasRect().toRect().topLeft();

    return QPointF(qMin(localMaxX(), qMax(localMinX(), canvasMap(QwtPlot::xBottom).invTransform(realPoint.x()))),
                   qMin(localMaxY(), qMax(localMinY(), canvasMap(QwtPlot::yLeft).invTransform(realPoint.y()))));
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Check that interaction is allowed

    if (!mInteractive)
        return;

    // Retrieve the current point

    QPointF currentPoint = mousePositionWithinCanvas(pEvent->pos());

    // Carry out the action

    switch (mAction) {
    case Pan: {
        // Determine the X/Y shifts for the panning

        double shiftX = currentPoint.x()-mOriginPoint.x();
        double shiftY = currentPoint.y()-mOriginPoint.y();

        // Determine our new local minimum/maximum values for our axes

        double newLocalMinX = localMinX()-shiftX;
        double newLocalMaxX = localMaxX()-shiftX;
        double newLocalMinY = localMinY()-shiftY;
        double newLocalMaxY = localMaxY()-shiftY;

        // Make sure that our new local minimum/maximum values for our axes
        // are within our local minimum/maximum values

        if (newLocalMinX < minX()) {
            newLocalMinX = minX();
            newLocalMaxX = newLocalMinX+localMaxX()-localMinX();
        } else if (newLocalMaxX > maxX()) {
            newLocalMaxX = maxX();
            newLocalMinX = newLocalMaxX-localMaxX()+localMinX();
        }

        if (newLocalMinY < minY()) {
            newLocalMinY = minY();
            newLocalMaxY = newLocalMinY+localMaxY()-localMinY();
        } else if (newLocalMaxY > maxY()) {
            newLocalMaxY = maxY();
            newLocalMinY = newLocalMaxY-localMaxY()+localMinY();
        }

        // Set our new local minimum/maximum values for our local axes which
        // will replot ourselves as a result

        setLocalAxes(newLocalMinX, newLocalMaxX, newLocalMinY, newLocalMaxY);

        break;
    }
    case ShowCoordinates:
        // Show the coordinates by simply replotting ourselves

        replotNow();

        break;
    case Zoom: {
        // Rescale ourselves (which will replot ourselves as a result)

        double deltaX = currentPoint.x()-mOriginPoint.x();
        double deltaY = currentPoint.y()-mOriginPoint.y();

        scaleLocalAxes(deltaX?
                           (deltaX > 0)?
                               ScalingInFactor:
                               ScalingOutFactor:
                           NoScalingFactor,
                       deltaY?
                           (deltaY < 0)?
                               ScalingInFactor:
                               ScalingOutFactor:
                           NoScalingFactor);

        break;
    }
    case ZoomRegion:
        // Draw our zoom region by updating our end point and then replotting
        // ourselves

        mEndPoint = mousePositionWithinCanvas(pEvent->pos());

        replotNow();

        break;
    default:
        // None

        ;
    }

    // Reset our point of origin, but only if we are doing something and it's
    // not zooming a region

    if ((mAction != None) && (mAction != ZoomRegion))
        mOriginPoint = mousePositionWithinCanvas(pEvent->pos());
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mousePressEvent(pEvent);

    // Check that interaction is allowed

    if (!mInteractive)
        return;

    // Check that the position of the mouse is over our canvas

    if (!plotLayout()->canvasRect().contains(pEvent->pos()))
        return;

    // Check which action we can carry out

    if (   (pEvent->button() == Qt::LeftButton)
        && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to pan

        mAction = Pan;
    } else if (   (pEvent->button() == Qt::LeftButton)
               && (pEvent->modifiers() == Qt::ShiftModifier)) {
        // We want to show the coordinates

        mAction = ShowCoordinates;
    } else if (   (pEvent->button() == Qt::RightButton)
               && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to zoom in/out

        mAction = Zoom;
    } else if (   (pEvent->button() == Qt::RightButton)
               && (pEvent->modifiers() == Qt::ControlModifier)) {
        // We want to zoom a region, but we can only do this if we are not
        // already fully zoomed in

        if ((mZoomFactorX < MaxZoomFactor) || (mZoomFactorY < MaxZoomFactor)) {
            mAction = ZoomRegion;
        } else {
            // We are already fully zoomed in, so...

            mAction = None;

            return;
        }
    } else {
        // We cannot carry out any action, so check whether we need to replot
        // ourselves (in case we were in the middle of carrying out a visual
        // action), make sure that we have no action to carry out, replot
        // ourselves if needed, and then leave

        bool needReplotNow;

        switch (mAction) {
        case ShowCoordinates:
        case ZoomRegion:
            needReplotNow = true;

            break;
        default:
            needReplotNow = false;
        }

        mAction = None;

        if (needReplotNow)
            replotNow();

        return;
    }

    // Retrieve a pixmap version of our canvas, if needed

    if ((mAction == ShowCoordinates) || (mAction == ZoomRegion))
        mCanvasPixmap = grab(plotLayout()->canvasRect().toRect());

    // Keep track of the mouse position

    mOriginPoint = mousePositionWithinCanvas(pEvent->pos());
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseReleaseEvent(pEvent);

    // Check that interaction is allowed

    if (!mInteractive)
        return;

    // Check whether we need to carry out an action

    if (mAction == None)
        return;

    // Keep track of the action to carry out
    // Note: this is so that we can reset mAction while still being able to
    //       finish carrying out the action...

    Action action = mAction;

    // We are done carrying out an action, so...

    mAction = None;

    // Finish carrying out the action

    switch (action) {
    case ShowCoordinates:
        // Remove the coordinates by replotting ourselves

        replotNow();

        break;
    case ZoomRegion: {
        // Zoom our region

        QRectF zoomRegionRect = zoomRegion();

        setLocalAxes(zoomRegionRect.left(), zoomRegionRect.right(),
                     zoomRegionRect.bottom(), zoomRegionRect.top());

        break;
    }
    default:
        // Another action which needs nothing more to be done

        ;
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::wheelEvent(pEvent);

    // Check that interaction is allowed

    if (!mInteractive)
        return;

    // The only action we support using the wheel is zooming in/out, but this
    // requires no modifiers being used

    if (pEvent->modifiers() != Qt::NoModifier)
        return;

    // Make sure that we actually want to zoom in/out

    if (!pEvent->delta())
        return;

    // Zoom in/out by scaling our two local axes

    double scalingFactor = (pEvent->delta() > 0)?ScalingInFactor:ScalingOutFactor;

    mOriginPoint = mousePositionWithinCanvas(pEvent->pos());

    scaleLocalAxes(scalingFactor, scalingFactor);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::replotNow()
{
    // Replot ourselves

    replot();

    // Make sure that the replotting occurs immediately
    // Note: this is needed when running a simulation since, otherwise,
    //       replotting won't occur immediately (because of threading)...

    qApp->processEvents();
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::drawCoordinates(QPainter *pPainter,
                                                         const QPointF &pCoordinates,
                                                         const QColor &pBackgroundColor,
                                                         const QColor &pForegroundColor,
                                                         const Location &pLocation,
                                                         const bool &pCanMoveLocation)
{
    // Retrieve the size of coordinates as they will appear on the screen,
    // which means using the same font as the one used for the axes

    pPainter->setFont(axisFont(QwtPlot::xBottom));

    QString coords = QString("(%1, %2)").arg(QString::number(pCoordinates.x()),
                                             QString::number(pCoordinates.y()));
    QRect desktopGeometry = qApp->desktop()->availableGeometry();
    QRectF coordsRect = pPainter->boundingRect(QRectF(0.0, 0.0, desktopGeometry.width(), desktopGeometry.height()), coords);

    // Determine where the coordinates and its background should be drawn

    QPoint coordinates = QPoint(canvasMap(QwtPlot::xBottom).transform(pCoordinates.x()),
                                canvasMap(QwtPlot::yLeft).transform(pCoordinates.y()));

    switch (pLocation) {
    case TopLeft:
        coordsRect.moveTo(coordinates.x()-coordsRect.right()-1,
                          coordinates.y()-coordsRect.bottom()-1);

        break;
    case TopRight:
        coordsRect.moveTo(coordinates.x()+2,
                          coordinates.y()-coordsRect.bottom()-1);

        break;
    case BottomLeft:
        coordsRect.moveTo(coordinates.x()-coordsRect.right()-1,
                          coordinates.y()+2);

        break;
    case BottomRight:
        coordsRect.moveTo(coordinates.x()+2,
                          coordinates.y()+2);

        break;
    }

    if (pCanMoveLocation) {
        if (coordsRect.top() < 0)
            coordsRect.moveTop(coordinates.y()+2);

        if (coordsRect.left() < 0)
            coordsRect.moveLeft(coordinates.x()+2);

        if (coordsRect.bottom() > plotLayout()->canvasRect().height())
            coordsRect.moveTop(coordinates.y()-coordsRect.height()-1);

        if (coordsRect.right() > plotLayout()->canvasRect().width())
            coordsRect.moveLeft(coordinates.x()-coordsRect.width()-1);
    }

    // Draw a filled rectangle to act as the background of the coordinates
    // we are to show

    pPainter->fillRect(coordsRect, pBackgroundColor);

    // Draw the text for the coordinates, using a white pen

    QPen pen = pPainter->pen();

    pen.setColor(pForegroundColor);

    pPainter->setPen(pen);

    pPainter->drawText(coordsRect, coords);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::drawCanvas(QPainter *pPainter)
{
    switch (mAction) {
    case ShowCoordinates: {
        // We are showing some coordinates, so start by drawing our pixmap

        pPainter->drawPixmap(0, 0, mCanvasPixmap);

        // Draw the two dashed lines that show the coordinates, using a dark
        // cyan pen

        QPen pen = pPainter->pen();
        QColor backgroundColor = Qt::darkCyan;

        backgroundColor.setAlphaF(0.69);

        pen.setColor(backgroundColor);
        pen.setStyle(Qt::DashLine);

        pPainter->setPen(pen);

        QPointF coordinates = QPointF(canvasMap(QwtPlot::xBottom).transform(mOriginPoint.x()),
                                      canvasMap(QwtPlot::yLeft).transform(mOriginPoint.y()));

        pPainter->drawLine(0.0, coordinates.y(),
                           plotLayout()->canvasRect().width(), coordinates.y());
        pPainter->drawLine(coordinates.x(), 0.0,
                           coordinates.x(), plotLayout()->canvasRect().height());

        // Draw the coordinates

        drawCoordinates(pPainter, mOriginPoint, backgroundColor, Qt::white);

        break;
    }
    case ZoomRegion: {
        // We are zooming a region, so start by drawing our pixmap

        pPainter->drawPixmap(0, 0, mCanvasPixmap);

        // Retrieve the coordinates of the region to be zoomed

        QRectF zoomRegionRect = zoomRegion();

        // Now, draw the region to be zoomed

        QColor penColor = Qt::darkRed;
        QColor brushColor = Qt::yellow;

        penColor.setAlphaF(0.69);
        brushColor.setAlphaF(0.19);

        pPainter->setPen(penColor);

        QwtScaleMap canvasMapX = canvasMap(QwtPlot::xBottom);
        QwtScaleMap canvasMapY = canvasMap(QwtPlot::yLeft);

        double left = canvasMapX.transform(zoomRegionRect.left());
        double top = canvasMapY.transform(zoomRegionRect.top());

        QRectF unmappedZoomRegionRect = QRectF(left, top,
                                               canvasMapX.transform(zoomRegionRect.right())-left,
                                               canvasMapY.transform(zoomRegionRect.bottom())-top);

        pPainter->fillRect(unmappedZoomRegionRect, brushColor);
        pPainter->drawRect(unmappedZoomRegionRect);

        // Draw the two sets of coordinates

        drawCoordinates(pPainter, zoomRegionRect.topLeft(), penColor, Qt::white, BottomRight, false);
        drawCoordinates(pPainter, zoomRegionRect.bottomRight(), penColor, Qt::white, TopLeft, false);

        break;
    }
    default:
        // We aren't doing anything special, so just draw our canvas normally

        QwtPlot::drawCanvas(pPainter);
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::addGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // Make sure that the given graph is not already attached to us

    if (mGraphs.contains(pGraph))
        return;

    // Attach the given graph to ourselves and keep track of it

    pGraph->attach(this);

    mGraphs << pGraph;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // Check that the given graph is attached to us

    if (!mGraphs.contains(pGraph))
        return;

    // Detach the given graph from ourselves and stop tracking it

    pGraph->detach();

    mGraphs.removeOne(pGraph);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::drawGraphSegment(SingleCellViewGraphPanelPlotGraph *pGraph,
                                                          const qulonglong &pFrom,
                                                          const qulonglong &pTo)
{
    // Make sure that we have a graph segment to draw

    if (pFrom == pTo)
        return;

    // Reset our local axes and replot ourselves, if it is our first graph
    // segment, or carry on as normal

    if (!pFrom) {
        // It is our first graph segment, so check our local axes
        // Note: we always want to replot, hence our passing false as an
        //       argument to resetLocalAxes()...

        resetLocalAxes(false);
        replotNow();
    } else {
        // It's not our first graph segment, so determine the minimum/maximum
        // X/Y values of our new data

        double xMin = 0.0;
        double xMax = 0.0;
        double yMin = 0.0;
        double yMax = 0.0;

        for (qulonglong i = pFrom; i <= pTo; ++i)
            if (i == pFrom) {
                xMin = xMax = pGraph->data()->sample(i).x();
                yMin = yMax = pGraph->data()->sample(i).y();
            } else {
                double xVal = pGraph->data()->sample(i).x();
                double yVal = pGraph->data()->sample(i).y();

                xMin = qMin(xMin, xVal);
                xMax = qMax(xMax, xVal);

                yMin = qMin(yMin, yVal);
                yMax = qMax(yMax, yVal);
            }

        // Check whether our X/Y axis can handle the minimum/maximum X/Y values
        // of our new data

        if (   (xMin < minX()) || (xMax > maxX())
            || (yMin < minY()) || (yMax > maxY()))
            // Our X/Y axis cannot handle the minimum/maximum X/Y values of our
            // new data, so check our local axes

            checkLocalAxes(true, true, true);
        else
            // Our X/Y axis can handle the X/Y min/max of our new data, so just
            // draw our new graph segment

            mDirectPainter->drawSeries(pGraph, pFrom, pTo);
    }
}

//==============================================================================

QRectF SingleCellViewGraphPanelPlotWidget::zoomRegion() const
{
    // Return the region to be zoomed based on the origin and end points
    // Note: by default, we assume that we are already fully zoomed in in
    //       both directions...

    double xMin = localMinX();
    double xMax = localMaxX();
    double yMin = localMinY();
    double yMax = localMaxY();

    if (mZoomFactorX < MaxZoomFactor) {
        xMin = qMin(mOriginPoint.x(), mEndPoint.x());
        xMax = qMax(mOriginPoint.x(), mEndPoint.x());
    }

    if (mZoomFactorY < MaxZoomFactor) {
        yMin = qMin(mOriginPoint.y(), mEndPoint.y());
        yMax = qMax(mOriginPoint.y(), mEndPoint.y());
    }

    return QRectF(xMin, yMax, xMax-xMin, yMin-yMax);
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

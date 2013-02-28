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

SingleCellSimulationViewGraphPanelPlotCurve::SingleCellSimulationViewGraphPanelPlotCurve() :
    QwtPlotCurve(),
    mEnabled(true)
{
    // Customise it a bit

    setPen(QPen(Qt::darkBlue));
    setRenderHint(QwtPlotItem::RenderAntialiased);
}

//==============================================================================

bool SingleCellSimulationViewGraphPanelPlotCurve::isValid() const
{
    // Return whether we are valid, i.e. enabled and visible

    return mEnabled && isVisible();
}

//==============================================================================

bool SingleCellSimulationViewGraphPanelPlotCurve::isEnabled() const
{
    // Return whether we are enabled

    return mEnabled;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotCurve::setEnabled(const bool &pEnabled)
{
    // Set our enabled status

    mEnabled = pEnabled;
}

//==============================================================================

SingleCellSimulationViewGraphPanelPlotWidget::SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent) :
    QwtPlot(pParent),
    mCurves(QList<SingleCellSimulationViewGraphPanelPlotCurve *>()),
    mAction(None),
    mOriginPoint(QPoint()),
    mMinX(100.0),
    mMaxX(300.0),
    mMinY(0.0),
    mMaxY(1000.0)
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

    setLocalMinX(mMinX, false);
    setLocalMaxX(mMaxX, false);
    setLocalMinY(mMinY, false);
    setLocalMaxY(mMaxY, false);

    // Attach a grid to ourselves

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajorPen(Qt::gray, 0, Qt::DotLine);

    grid->attach(this);
}

//==============================================================================

SingleCellSimulationViewGraphPanelPlotWidget::~SingleCellSimulationViewGraphPanelPlotWidget()
{
    // Delete some internal objects

    foreach (SingleCellSimulationViewGraphPanelPlotCurve *curve, mCurves)
        delete curve;

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

double SingleCellSimulationViewGraphPanelPlotWidget::minX() const
{
    // Return our minimum X value

    return mMinX;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setMinX(const double &pValue,
                                                           const bool &pCheckAxes)
{
    // Set our minimum X value

    mMinX = pValue;

    // Check our axes

    if (pCheckAxes)
        checkAxes();
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::maxX() const
{
    // Return our maximum X value

    return mMaxX;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setMaxX(const double &pValue,
                                                           const bool &pCheckAxes)
{
    // Set our maximum X value

    mMaxX = pValue;

    // Check our axes

    if (pCheckAxes)
        checkAxes();
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::minY() const
{
    // Return our minimum Y value

    return mMinY;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setMinY(const double &pValue,
                                                           const bool &pCheckAxes)
{
    // Set our minimum Y value

    mMinY = pValue;

    // Check our axes

    if (pCheckAxes)
        checkAxes();
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::maxY() const
{
    // Return our maximum Y value

    return mMaxY;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setMaxY(const double &pValue,
                                                           const bool &pCheckAxes)
{
    // Set our maximum Y value

    mMaxY = pValue;

    // Check our axes

    if (pCheckAxes)
        checkAxes();
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::localMinX() const
{
    // Return our local minimum X value

    return axisScaleDiv(QwtPlot::xBottom).lowerBound();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setLocalMinX(const double &pValue,
                                                                const bool &pCheckAxes)
{
    // Set our local minimum X value
    // Note: to use setAxisScale() on its own is not sufficient unless we were
    //       to replot ourselves immediately after, but we don't necessarily
    //       want to do that, so instead we also use setAxisScaleDiv() to make
    //       sure that our local minimum X value is indeed taken into account
    //       (i.e. we can retrieve it using localMinX()). Also, we must call
    //       setAxisScaleDiv() before setAxisScale() to make sure that the axis
    //       data is not considered as valid which is important when it comes to
    //       plotting ourselves...

    setAxisScaleDiv(QwtPlot::xBottom, QwtScaleDiv(pValue, localMaxX()));
    setAxisScale(QwtPlot::xBottom, pValue, localMaxX());

    // Check our axes

    if (pCheckAxes)
        checkAxes();
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::localMaxX() const
{
    // Return our local maximum X value

    return axisScaleDiv(QwtPlot::xBottom).upperBound();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setLocalMaxX(const double &pValue,
                                                                const bool &pCheckAxes)
{
    // Set our local maximum X value
    // Note: see the corresponding note in setLocalMinX()...

    setAxisScaleDiv(QwtPlot::xBottom, QwtScaleDiv(localMinX(), pValue));
    setAxisScale(QwtPlot::xBottom, localMinX(), pValue);

    // Check our axes

    if (pCheckAxes)
        checkAxes();
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::localMinY() const
{
    // Return our local minimum Y value

    return axisScaleDiv(QwtPlot::yLeft).lowerBound();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setLocalMinY(const double &pValue,
                                                                const bool &pCheckAxes)
{
    // Set our local minimum Y value
    // Note: see the corresponding note in setLocalMinX()...

    setAxisScaleDiv(QwtPlot::yLeft, QwtScaleDiv(pValue, localMaxY()));
    setAxisScale(QwtPlot::yLeft, pValue, localMaxY());

    // Check our axes

    if (pCheckAxes)
        checkAxes();
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::localMaxY() const
{
    // Return our local maximum Y value

    return axisScaleDiv(QwtPlot::yLeft).upperBound();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setLocalMaxY(const double &pValue,
                                                                const bool &pCheckAxes)
{
    // Set our local maximum Y value
    // Note: see the corresponding note in setLocalMinX()...

    setAxisScaleDiv(QwtPlot::yLeft, QwtScaleDiv(localMinY(), pValue));
    setAxisScale(QwtPlot::yLeft, localMinY(), pValue);

    // Check our axes

    if (pCheckAxes)
        checkAxes();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setAxes(const double &pMinX,
                                                           const double &pMaxX,
                                                           const double &pMinY,
                                                           const double &pMaxY,
                                                           const bool &pCanReplot,
                                                           const bool &pForceMinMaxValues)
{
    // Update our axes

    double oldLocalMinX = localMinX();
    double oldLocalMaxX = localMaxX();
    double oldLocalMinY = localMinY();
    double oldLocalMaxY = localMaxY();

    double newLocalMinX = pMinX;
    double newLocalMaxX = pMaxX;
    double newLocalMinY = pMinY;
    double newLocalMaxY = pMaxY;

    // Retrieve the bounding rectangle for all our curves (that is, as long as
    // they are valid and have some data)

    QRectF boundingRect = QRectF();

    foreach (SingleCellSimulationViewGraphPanelPlotCurve *curve, mCurves)
        if (curve->isValid() && curve->dataSize())
            boundingRect |= curve->boundingRect();

    // Update the minimum/maximum values of our axes, should we have retrieved a
    // valid bounding rectangle

    if (boundingRect != QRectF()) {
        mMinX = qMin(mMinX, boundingRect.left());
        mMaxX = qMax(mMaxX, boundingRect.right());
        mMinY = qMin(mMinY, boundingRect.top());
        mMaxY = qMax(mMaxY, boundingRect.bottom());
    }

    // Make sure that the new local minimum/maximum values of our axes fit
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

    // Update the local minimum/maximum of our axes, if needed

    bool needReplot = false;

    if (newLocalMinX != oldLocalMinX) {
        setLocalMinX(newLocalMinX, false);

        needReplot = true;
    }

    if (newLocalMaxX != oldLocalMaxX) {
        setLocalMaxX(newLocalMaxX, false);

        needReplot = true;
    }

    if (newLocalMinY != oldLocalMinY) {
        setLocalMinY(newLocalMinY, false);

        needReplot = true;
    }

    if (newLocalMaxY != oldLocalMaxY) {
        setLocalMaxY(newLocalMaxY, false);

        needReplot = true;
    }

    // Replot ourselves, if needed and allowed

    if (needReplot && pCanReplot)
        replotNow();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::scaleAxes(const double &pScalingFactorX,
                                                             const double &pScalingFactorY)
{
    // Determine the local minimum/maximum values of our two axes

    QwtScaleDiv xScaleDiv = axisScaleDiv(QwtPlot::xBottom);
    double xCenter = xScaleDiv.lowerBound()+0.5*xScaleDiv.range();
    double xRangeOverTwo = 0.5*pScalingFactorX*xScaleDiv.range();

    QwtScaleDiv yScaleDiv = axisScaleDiv(QwtPlot::yLeft);
    double yCenter = yScaleDiv.lowerBound()+0.5*yScaleDiv.range();
    double yRangeOverTwo = 0.5*pScalingFactorY*yScaleDiv.range();

    // Rescale our two axes

    setAxes(xCenter-xRangeOverTwo, xCenter+xRangeOverTwo,
            yCenter-yRangeOverTwo, yCenter+yRangeOverTwo);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::checkAxes(const bool &pCanReplot,
                                                             const bool &pForceMinMaxValues)
{
    // Check our axes by trying to set them

    setAxes(localMinX(), localMaxX(), localMinY(), localMaxY(),
            pCanReplot, pForceMinMaxValues);
}

//==============================================================================

static const double NoScalingFactor  = 1.0;
static const double ScalingInFactor  = 0.57;
static const double ScalingOutFactor = 1.0/ScalingInFactor;

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Carry out the action

    switch (mAction) {
    case Zoom: {
        // Determine how much we have moved our mouse since last time

        int deltaX = pEvent->pos().x()-mOriginPoint.x();
        int deltaY = pEvent->pos().y()-mOriginPoint.y();

        // Rescale and replot ourselves, as well as reset our point of origin,
        // if needed

        if (deltaX || deltaY) {
            scaleAxes(deltaX?
                          (deltaX > 0)?
                              ScalingInFactor:
                              ScalingOutFactor:
                          NoScalingFactor,
                      deltaY?
                          (deltaY > 0)?
                              ScalingInFactor:
                              ScalingOutFactor:
                          NoScalingFactor);

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

    // Make sure that there is a need to zoom in/out

    if (!pEvent->delta())
        return;

    // Zoom in/out by scaling our two axes

    double scalingFactor = (pEvent->delta() > 0)?ScalingInFactor:ScalingOutFactor;

    scaleAxes(scalingFactor, scalingFactor);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::replotNow()
{
    // Replot ourselves, but only if the local minimum/maximum values of our
    // axes are valid

    if (   (qAbs(localMinX()) == DBL_MAX) || (qAbs(localMaxX()) == DBL_MAX)
        || (qAbs(localMinY()) == DBL_MAX) || (qAbs(localMaxY()) == DBL_MAX))
        return;

    replot();

    // Make sure that the replotting occurs immediately
    // Note: this is needed when running a simulation since, otherwise,
    //       replotting won't occur immediately (because of threading)...

    qApp->processEvents();
}

//==============================================================================

SingleCellSimulationViewGraphPanelPlotCurve * SingleCellSimulationViewGraphPanelPlotWidget::addCurve()
{
    // Create a new curve

    SingleCellSimulationViewGraphPanelPlotCurve *res = new SingleCellSimulationViewGraphPanelPlotCurve();

    // Attach it to ourselves

    res->attach(this);

    // Add it to our list of curves

    mCurves << res;

    // Return it to the caller

    return res;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::drawCurveSegment(SingleCellSimulationViewGraphPanelPlotCurve *pCurve,
                                                                    const qulonglong &pFrom,
                                                                    const qulonglong &pTo)
{
    // Make sure that we have a curve segment to draw

    if (pFrom == pTo)
        return;

    // Determine the minimum/maximum X/Y values of our new data

    double xMin;
    double xMax;
    double yMin;
    double yMax;

    for (qulonglong i = pFrom; i <= pTo; ++i)
        if (i == pFrom) {
            xMin = xMax = pCurve->data()->sample(i).x();
            yMin = yMax = pCurve->data()->sample(i).y();
        } else {
            double xVal = pCurve->data()->sample(i).x();
            double yVal = pCurve->data()->sample(i).y();

            xMin = qMin(xMin, xVal);
            xMax = qMax(xMax, xVal);

            yMin = qMin(yMin, yVal);
            yMax = qMax(yMax, yVal);
        }

    // Check which curve segment we are dealing with and whether our X/Y axis
    // can handle the minimum/maximum X/Y values of our new data

    if (   !pFrom
        || (xMin < minX()) || (xMax > maxX())
        || (yMin < minY()) || (yMax > maxY()))
        // Either it's our first curve segment and/or our X/Y axis cannot handle
        // the minimum/maximum X/Y values of our new data, so check our axes

        checkAxes(true, true);
    else
        // Our X/Y axis can handle the X/Y min/max of our new data, so just draw
        // our new curve segment

        mDirectPainter->drawSeries(pCurve, pFrom, pTo);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

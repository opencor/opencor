/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view graph panel plot widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "guiinterface.h"
#include "singlecellviewgraphpanelplotwidget.h"

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QDesktopWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QWidget>

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
    mSelected(true),
    mFileName(QString()),
    mParameterX(pParameterX),
    mParameterY(pParameterY)
{
    // Customise ourselves a bit

    setPen(QPen(Qt::darkBlue));
    setRenderHint(QwtPlotItem::RenderAntialiased);
}

//==============================================================================

bool SingleCellViewGraphPanelPlotGraph::isValid() const
{
    // Return whether we are valid

    return   !mFileName.isEmpty()
           && mParameterX && mParameterY;
}

//==============================================================================

bool SingleCellViewGraphPanelPlotGraph::isSelected() const
{
    // Return whether we are selected

    return mSelected;
}

//==============================================================================

void SingleCellViewGraphPanelPlotGraph::setSelected(const bool &pSelected)
{
    // Set our selected state

    mSelected = pSelected;
}

//==============================================================================

QString SingleCellViewGraphPanelPlotGraph::fileName() const
{
    // Return our file name

    return mFileName;
}

//==============================================================================

void SingleCellViewGraphPanelPlotGraph::setFileName(const QString &pFileName)
{
    // Set our file name

    mFileName = pFileName;
}

//==============================================================================

CellMLSupport::CellmlFileRuntimeParameter * SingleCellViewGraphPanelPlotGraph::parameterX() const
{
    // Return our parameter X

    return mParameterX;
}

//==============================================================================

void SingleCellViewGraphPanelPlotGraph::setParameterX(CellMLSupport::CellmlFileRuntimeParameter *pParameterX)
{
    // Set our parameter X

    mParameterX = pParameterX;
}

//==============================================================================

CellMLSupport::CellmlFileRuntimeParameter * SingleCellViewGraphPanelPlotGraph::parameterY() const
{
    // Return our parameter Y

    return mParameterY;
}

//==============================================================================

void SingleCellViewGraphPanelPlotGraph::setParameterY(CellMLSupport::CellmlFileRuntimeParameter *pParameterY)
{
    // Set our parameter Y

    mParameterY = pParameterY;
}

//==============================================================================

static const double MinZoomFactor =    1.0;
static const double MaxZoomFactor = 1024.0;

static const double MinAxis =    0.0;
static const double MaxAxis = 1000.0;

//==============================================================================

SingleCellViewGraphPanelPlotWidget::SingleCellViewGraphPanelPlotWidget(QWidget *pParent) :
    QwtPlot(pParent),
    CommonWidget(pParent),
    mGraphs(QList<SingleCellViewGraphPanelPlotGraph *>()),
    mAction(None),
    mOriginPoint(QPointF()),
    mEndPoint(QPointF()),
    mMinX(MinAxis),
    mMaxX(MaxAxis),
    mMinY(MinAxis),
    mMaxY(MaxAxis),
    mNeedMinX(0.0),
    mNeedMaxX(0.0),
    mNeedMinY(0.0),
    mNeedMaxY(0.0),
    mCanvasPixmap(QPixmap()),
    mZoomFactorX(MinZoomFactor),
    mZoomFactorY(MinZoomFactor),
    mNeedCustomContextMenu(false),
    mZoomInAction(GuiInterface::newAction(this)),
    mZoomOutAction(GuiInterface::newAction(this)),
    mResetZoomAction(GuiInterface::newAction(this))
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

    // Create and populate our custom context menu

    mCustomContextMenu = new QMenu(this);

    mCustomContextMenu->addAction(mZoomInAction);
    mCustomContextMenu->addAction(mZoomOutAction);
    mCustomContextMenu->addSeparator();
    mCustomContextMenu->addAction(mResetZoomAction);

    // Some connections

    connect(mZoomInAction, SIGNAL(triggered(bool)),
            this, SLOT(zoomIn()));
    connect(mZoomOutAction, SIGNAL(triggered(bool)),
            this, SLOT(zoomOut()));
    connect(mResetZoomAction, SIGNAL(triggered(bool)),
            this, SLOT(resetZoom()));
}

//==============================================================================

SingleCellViewGraphPanelPlotWidget::~SingleCellViewGraphPanelPlotWidget()
{
    // Delete some internal objects

    delete mDirectPainter;

    foreach (SingleCellViewGraphPanelPlotGraph *graph, mGraphs)
        delete graph;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::retranslateUi()
{
    // Retranslate our actions

    GuiInterface::retranslateAction(mZoomInAction, tr("Zoom In"),
                                    tr("Zoom in the plot"));
    GuiInterface::retranslateAction(mZoomOutAction, tr("Zoom Out"),
                                    tr("Zoom out the plot"));
    GuiInterface::retranslateAction(mResetZoomAction, tr("Reset Zoom"),
                                    tr("Reset the zoom level of the plot"));
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

        // Make sure that our layout updated
        // Note: indeed, our call to QwtPlotRenderer::render() invalidates our
        //       layout just before returning. This means that a call to
        //       plotLayout()->canvasRect() would return an empty rectangle,
        //       which in turn means that the user couldn't carry out another
        //       action (e.g. see mousePressEvent()), so...

        updateLayout();
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setNeedMinMaxX(const double &pNeedMinX,
                                                        const double &pNeedMaxX)
{
    // Set our needed minimum/maximum X values

    mNeedMinX = pNeedMinX;
    mNeedMaxX = pNeedMaxX;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setNeedMinMaxY(const double &pNeedMinY,
                                                        const double &pNeedMaxY)
{
    // Set our needed minimum/maximum Y values

    mNeedMinY = pNeedMinY;
    mNeedMaxY = pNeedMaxY;
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

    // Update the enabled status of our actions

    mZoomInAction->setEnabled((mZoomFactorX < MaxZoomFactor) || (mZoomFactorY < MaxZoomFactor));
    mZoomOutAction->setEnabled((mZoomFactorX > MinZoomFactor) || (mZoomFactorY > MinZoomFactor));
    mResetZoomAction->setEnabled((mZoomFactorX != MinZoomFactor) && (mZoomFactorY != MinZoomFactor));
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

QList<SingleCellViewGraphPanelPlotGraph *> SingleCellViewGraphPanelPlotWidget::graphs() const
{
    // Return all our graphs

    return mGraphs;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::checkAnyAxesValues(double &pMinX,
                                                            double &pMaxX,
                                                            double &pMinY,
                                                            double &pMaxY)
{
    // Make sure that the minimum/maximum values of our axes have finite values

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

double SingleCellViewGraphPanelPlotWidget::optimisedValue(const double &pValue,
                                                          const double &pPower,
                                                          const bool &pLowValue)
{
    // Optimise the given value using the given power

    double res = pPower*(pLowValue?qFloor(pValue/pPower):qCeil(pValue/pPower));

    // Now, try to optimise even further by testing against half of the given
    // power

    if (pLowValue) {
        double otherRes = res+0.5*pPower;

        return (otherRes < pValue)?otherRes:res;
    } else {
        double otherRes = res-0.5*pPower;

        return (otherRes > pValue)?otherRes:res;
    }
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
        if (graph->isValid() && graph->isSelected() && graph->dataSize())
            boundingRect |= graph->boundingRect();

    // Take into account the needed minimum/maximum values for our X and Y axes,
    // if any, or use MinAxis/MaxAxis if boundingRect is empty

    bool boundingRectIsEmpty = boundingRect.isEmpty();
    bool needMinMaxX = mNeedMinX != mNeedMaxX;
    bool needMinMaxY = mNeedMinY != mNeedMaxY;

    if (needMinMaxX) {
        boundingRect.setLeft(boundingRectIsEmpty?mNeedMinX:qMin(boundingRect.left(), mNeedMinX));
        boundingRect.setRight(boundingRectIsEmpty?mNeedMaxX:qMax(boundingRect.right(), mNeedMaxX));
    } else if (boundingRectIsEmpty) {
        boundingRect.setLeft(MinAxis);
        boundingRect.setRight(MaxAxis);
    }

    if (needMinMaxY) {
        boundingRect.setTop(boundingRectIsEmpty?mNeedMinY:qMin(boundingRect.top(), mNeedMinY));
        boundingRect.setBottom(boundingRectIsEmpty?mNeedMaxY:qMax(boundingRect.bottom(), mNeedMaxY));
    } else if (boundingRectIsEmpty) {
        boundingRect.setTop(MinAxis);
        boundingRect.setBottom(MaxAxis);
    }

    // Update the minimum/maximum values of our axes, should we have retrieved a
    // valid bounding rectangle

    if (!boundingRect.isEmpty()) {
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

        double powerX = qPow(10.0, qMin(qFloor(log10(qAbs(xMax-xMin))),
                                        qMax(xMin?
                                                 qFloor(log10(qAbs(xMin))):
                                                 0,
                                             xMax?
                                                 qFloor(log10(qAbs(xMax))):
                                                 0)));
        double powerY = qPow(10.0, qMin(qFloor(log10(qAbs(yMax-yMin))),
                                        qMax(yMin?
                                                 qFloor(log10(qAbs(yMin))):
                                                 0,
                                             yMax?
                                                 qFloor(log10(qAbs(yMax))):
                                                 0)));

        xMin = (needMinMaxX && (xMin == mNeedMinX))?mNeedMinX:optimisedValue(xMin, powerX, true);
        xMax = (needMinMaxX && (xMax == mNeedMaxX))?mNeedMaxX:optimisedValue(xMax, powerX, false);
        yMin = (needMinMaxY && (yMin == mNeedMinY))?mNeedMinY:optimisedValue(yMin, powerY, true);
        yMax = (needMinMaxY && (yMax == mNeedMaxY))?mNeedMaxY:optimisedValue(yMax, powerY, false);

        // Make sure that the optimised minimum/maximum values of our axes have
        // finite values

        checkAnyAxesValues(xMin, xMax, yMin, yMax);

        // Update the minimum/maximum values of our axes, if required

        if (pResetMinMaxValues)
            setMinMaxX(xMin, xMax);
        else if (pUpdateMinMaxValues)
            setMinMaxX(qMin(mMinX, xMin), qMax(mMaxX, xMax));

        if (pResetMinMaxValues)
            setMinMaxY(yMin, yMax);
        else if (pUpdateMinMaxValues)
            setMinMaxY(qMin(mMinY, yMin), qMax(mMaxY, yMax));
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

    // Update the minimum/maximum values of our local axes, if needed, or make
    // sure that the new minimum/maximum values of our local axes have a valid
    // zoom factor

    bool needReplot = false;

    if ((newLocalMinX != oldLocalMinX) || (newLocalMaxX != oldLocalMaxX)) {
        setLocalMinMaxX(newLocalMinX, newLocalMaxX);

        needReplot = true;
    } else {
        checkLocalAxisValues(QwtPlot::xBottom, newLocalMinX, newLocalMaxX);
    }

    if ((newLocalMinY != oldLocalMinY) || (newLocalMaxY != oldLocalMaxY)) {
        setLocalMinMaxY(newLocalMinY, newLocalMaxY);

        needReplot = true;
    } else {
        checkLocalAxisValues(QwtPlot::yLeft, newLocalMinY, newLocalMaxY);
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

static const double NoScalingFactor     = 1.0;
static const double ScalingInFactor     = 0.9;
static const double ScalingOutFactor    = 1.0/ScalingInFactor;
static const double BigScalingInFactor  = 0.5*ScalingInFactor;
static const double BigScalingOutFactor = 1.0/BigScalingInFactor;

//==============================================================================

QPointF SingleCellViewGraphPanelPlotWidget::mousePositionWithinCanvas(const QPoint &pPoint) const
{
    // Return the mouse position relative to our canvas, after making sure that
    // its mapped values are within our local ranges

    QPointF realPoint = pPoint-plotLayout()->canvasRect().topLeft();

    return QPointF(qMin(localMaxX(), qMax(localMinX(), canvasMap(QwtPlot::xBottom).invTransform(realPoint.x()))),
                   qMin(localMaxY(), qMax(localMinY(), canvasMap(QwtPlot::yLeft).invTransform(realPoint.y()))));
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Carry out the action

    switch (mAction) {
    case Pan: {
        // Determine the X/Y shifts for our panning

        QPointF currentPoint = mousePositionWithinCanvas(pEvent->pos());

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

        // Update our point of origin

        mOriginPoint = mousePositionWithinCanvas(pEvent->pos());

        break;
    }
    case ShowCoordinates:
        // Update our point of origin

        mOriginPoint = mousePositionWithinCanvas(pEvent->pos());

        // Show the coordinates by simply replotting ourselves

        replotNow();

        break;
    case Zoom: {
        // Determine our X/Y delta values

        QPointF currentPoint = mousePositionWithinCanvas(pEvent->pos());

        double deltaX = currentPoint.x()-mOriginPoint.x();
        double deltaY = currentPoint.y()-mOriginPoint.y();

        // Rescale ourselves
        // Note: this will automatically replot ourselves...

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

        // Update our point of origin

        mOriginPoint = mousePositionWithinCanvas(pEvent->pos());

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

    // The mouse has moved, so we definitely won't need to show our custom
    // context menu

    mNeedCustomContextMenu = false;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mousePressEvent(pEvent);

    // Check that the position of the mouse is over our canvas

    if (!plotLayout()->canvasRect().contains(pEvent->pos()))
        return;

    // Make sure that we are not already carrying out an action (e.g. we were
    // zooming in/out and then press on the left mouse button)

    if (mAction != None) {
        // Check whether we need to replot ourselves, reset our action, replot
        // ourselves, if needed, and then leave

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

    // Check which action we can carry out

    if (   (pEvent->button() == Qt::LeftButton)
        && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to pan, but only do this if we are zoomed in

        if ((mZoomFactorX > MinZoomFactor) || (mZoomFactorY > MinZoomFactor)) {
            mAction = Pan;

            mOriginPoint = mousePositionWithinCanvas(pEvent->pos());
        }
    } else if (   (pEvent->button() == Qt::LeftButton)
               && (pEvent->modifiers() == Qt::ShiftModifier)) {
        // We want to show the coordinates

        mAction = ShowCoordinates;

        mOriginPoint = mousePositionWithinCanvas(pEvent->pos());

        mCanvasPixmap = grab(plotLayout()->canvasRect().toRect());

        replotNow();
    } else if (   (pEvent->button() == Qt::RightButton)
               && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to zoom in/out

        mAction = Zoom;

        mOriginPoint = mousePositionWithinCanvas(pEvent->pos());
    } else if (   (pEvent->button() == Qt::RightButton)
               && (pEvent->modifiers() == Qt::ControlModifier)) {
        // We want to zoom a region, but only do this if we are not already
        // fully zoomed in

        if ((mZoomFactorX < MaxZoomFactor) || (mZoomFactorY < MaxZoomFactor)) {
            mAction = ZoomRegion;

            mOriginPoint = mousePositionWithinCanvas(pEvent->pos());
            mEndPoint = mOriginPoint;

            mCanvasPixmap = grab(plotLayout()->canvasRect().toRect());
        }
    }

    // Check whether we might need to show our custom context menu

    mNeedCustomContextMenu = pEvent->button() == Qt::RightButton;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseReleaseEvent(pEvent);

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

        if (zoomRegionRect.width() && zoomRegionRect.height())
            setLocalAxes(zoomRegionRect.left(), zoomRegionRect.right(),
                         zoomRegionRect.bottom(), zoomRegionRect.top());

        break;
    }
    default:
        // Another action which doesn't require anything more to be done

        ;
    }

    // Show our custom context menu, if still needed

    if (mNeedCustomContextMenu)
        mCustomContextMenu->exec(QCursor::pos());
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::wheelEvent(pEvent);

    // Check whether we are already carrying out an action

    if (mAction != None)
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

    QPointF coordinates = QPointF(canvasMap(QwtPlot::xBottom).transform(pCoordinates.x()),
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

bool SingleCellViewGraphPanelPlotWidget::addGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // Make sure that the given graph is not already attached to us

    if (mGraphs.contains(pGraph))
        return false;

    // Attach the given graph to ourselves and keep track of it

    pGraph->attach(this);

    mGraphs << pGraph;

    return true;
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // Check that the given graph is attached to us

    if (!mGraphs.contains(pGraph))
        return false;

    // Detach the given graph from ourselves, stop tracking it and delete it

    pGraph->detach();

    mGraphs.removeOne(pGraph);

    delete pGraph;

    return true;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::updateAxes()
{
    // Update our axes by trying to set them

    setLocalAxes(0.0, 0.0, 0.0, 0.0, true, true, false, true);
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
        // It is our first graph segment, so reset our local axes by trying to
        // set them
        // Note: we always want to replot, hence our first boolean being false
        //       in our call to setLocalAxes()...

        setLocalAxes(0.0, 0.0, 0.0, 0.0, false, true, false, true);
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

void SingleCellViewGraphPanelPlotWidget::zoomIn()
{
    // Zoom in by scaling our two local axes

    mOriginPoint = mousePositionWithinCanvas(mapFromGlobal(QCursor::pos()));

    scaleLocalAxes(BigScalingInFactor, BigScalingInFactor);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::zoomOut()
{
    // Zoom out by scaling our two local axes

    mOriginPoint = mousePositionWithinCanvas(mapFromGlobal(QCursor::pos()));

    scaleLocalAxes(BigScalingOutFactor, BigScalingOutFactor);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::resetZoom()
{
    // Reset the zoom level by resetting our two local axes

    setLocalAxes(mMinX, mMaxX, mMinY, mMaxY);
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

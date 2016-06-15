/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view graph panel plot widget
//==============================================================================

#include "coreguiutils.h"
#include "i18ninterface.h"
#include "singlecellviewgraphpanelplotwidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QMenu>
#include <QPaintEvent>

//==============================================================================

#include <float.h>

//==============================================================================

#include "qwt_painter.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_directpainter.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_layout.h"
#include "qwt_scale_engine.h"
#include "qwt_scale_widget.h"

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

SingleCellViewGraphPanelPlotOverlayWidget::SingleCellViewGraphPanelPlotOverlayWidget(SingleCellViewGraphPanelPlotWidget *pParent) :
    QWidget(pParent),
    mOwner(pParent),
    mOriginPoint(QPoint()),
    mPoint(QPoint())
{
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    setFocusPolicy(Qt::NoFocus);
}

//==============================================================================

QPoint SingleCellViewGraphPanelPlotOverlayWidget::optimisedPoint(const QPoint &pPoint) const
{
    // Optimise the given point so that it fits within our owner's ranges

    QPoint realPoint = pPoint-mOwner->plotLayout()->canvasRect().topLeft().toPoint();

    QwtScaleMap canvasMapX = mOwner->canvasMap(QwtPlot::xBottom);
    QwtScaleMap canvasMapY = mOwner->canvasMap(QwtPlot::yLeft);

    return QPoint(qMin(qRound(canvasMapX.transform(mOwner->maxX())),
                       qMax(qRound(canvasMapX.transform(mOwner->minX())),
                            realPoint.x())),
                  qMin(qRound(canvasMapY.transform(mOwner->minY())),
                       qMax(qRound(canvasMapY.transform(mOwner->maxY())),
                            realPoint.y())));
}

//==============================================================================

void SingleCellViewGraphPanelPlotOverlayWidget::paintEvent(QPaintEvent *pEvent)
{
    // Accept the event

    pEvent->accept();

    // Check whether an action is to be carried out

    if (mOwner->action() == SingleCellViewGraphPanelPlotWidget::None)
        return;

    // Paint the overlay, if any is needed

    QPainter painter(this);

    QRectF canvasRect = mOwner->plotLayout()->canvasRect();

    painter.translate(canvasRect.x(), canvasRect.y());

    switch (mOwner->action()) {
    case SingleCellViewGraphPanelPlotWidget::ShowCoordinates: {
        // Draw the two dashed lines that show the coordinates, using a dark
        // cyan pen

        QPen pen = painter.pen();
        QColor penColor = Qt::darkCyan;

        penColor.setAlphaF(0.69);

        pen.setColor(penColor);
        pen.setStyle(Qt::DashLine);

        painter.setPen(pen);

        QPoint point = optimisedPoint(mPoint);
        // Note: see drawCoordinates() as why we use QPoint rather than
        //       QPointF...

        painter.drawLine(0, point.y(), canvasRect.width(), point.y());
        painter.drawLine(point.x(), 0, point.x(), canvasRect.height());

        // Draw the coordinates of our point

        drawCoordinates(&painter, point, penColor, Qt::white, BottomRight);

        break;
    }
    case SingleCellViewGraphPanelPlotWidget::ZoomRegion: {
        // Draw the region to be zoomed

        QColor penColor = Qt::darkRed;
        QColor brushColor = Qt::yellow;

        penColor.setAlphaF(0.69);
        brushColor.setAlphaF(0.19);

        painter.setPen(penColor);

        QRect zoomRegionRect = zoomRegion();
        // Note: see drawCoordinates() as why we use QRect rather than QRectF...

        painter.fillRect(zoomRegionRect, brushColor);
        painter.drawRect(zoomRegionRect);

        // Draw the two sets of coordinates

        drawCoordinates(&painter, zoomRegionRect.topLeft(),
                        penColor, Qt::white, BottomRight, false);
        drawCoordinates(&painter, zoomRegionRect.topLeft()+QPoint(zoomRegionRect.width(), zoomRegionRect.height()),
                        penColor, Qt::white, TopLeft, false);

        break;
    }
    default:
        // Either no action or not an action we know how to handle

        ;
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotOverlayWidget::setOriginPoint(const QPoint &pOriginPoint)
{
    // Set our point of origin

    mOriginPoint = pOriginPoint;
    // Note: unlike for setPoint(), we must not repaint ourselves since a call
    //       to setOriginPoint() should always be followed by one to setPoint(),
    //       in which case we don't want to repaint ourselves twice, not least
    //       because the first time we may not have a correct mPoint value...
}

//==============================================================================

void SingleCellViewGraphPanelPlotOverlayWidget::setPoint(const QPoint &pPoint)
{
    // Set our point

    mPoint = pPoint;

    update();
}

//==============================================================================

QRect SingleCellViewGraphPanelPlotOverlayWidget::zoomRegion() const
{
    // Return the region to be zoomed based on mOriginPoint and mPoint
    // Note: by default, we assume that we are already fully zoomed in in both
    //       directions...

    QwtScaleMap canvasMapX = mOwner->canvasMap(QwtPlot::xBottom);
    QwtScaleMap canvasMapY = mOwner->canvasMap(QwtPlot::yLeft);

    int minX = canvasMapX.transform(mOwner->minX());
    int maxX = canvasMapX.transform(mOwner->maxX());
    int minY = canvasMapY.transform(mOwner->maxY());
    int maxY = canvasMapY.transform(mOwner->minY());

    if (mOwner->canZoomInX() || mOwner->canZoomInY()) {
        QPoint originPoint = optimisedPoint(mOriginPoint);
        QPoint point = optimisedPoint(mPoint);

        if (mOwner->canZoomInX()) {
            minX = qMin(originPoint.x(), point.x());
            maxX = qMax(originPoint.x(), point.x());
        }

        if (mOwner->canZoomInY()) {
            minY = qMin(originPoint.y(), point.y());
            maxY = qMax(originPoint.y(), point.y());
        }
    }

    return QRect(minX, minY, maxX-minX, maxY-minY);
}

//==============================================================================

void SingleCellViewGraphPanelPlotOverlayWidget::drawCoordinates(QPainter *pPainter,
                                                                const QPoint &pPoint,
                                                                const QColor &pBackgroundColor,
                                                                const QColor &pForegroundColor,
                                                                const Location &pLocation,
                                                                const bool &pCanMoveLocation)
{
    // Retrieve the size of coordinates as they will appear on the screen,
    // which means using the same font as the one used for the axes
    // Note: normally, pPoint would be a QPointF, but we want the coordinates to
    //       be drawn relative to something (see paintEvent()) and the only way
    //       to guarantee that everything will be painted as expected is to use
    //       QPoint. Indeed, if we were to use QPointF, then QPainter would have
    //       to do some rouding and though everything should be fine (since we
    //       always add/subtract a rounded number), it happens that it's not
    //       always the case. Indeed, we should always have a gap of one pixel
    //       between the coordinates and pPoint, but it could happen that we
    //       have either no gap or one of two pixels...

    pPainter->setFont(mOwner->axisFont(QwtPlot::xBottom));

    QPointF point = mOwner->canvasPoint(pPoint, false);
    QString coordinates = QString("X: %1\nY: %2").arg(QLocale().toString(point.x(), 'g', 15),
                                                      QLocale().toString(point.y(), 'g', 15));
    QRect coordinatesRect = pPainter->boundingRect(qApp->desktop()->availableGeometry(), 0, coordinates);

    // Determine where the coordinates and its background should be drawn

    switch (pLocation) {
    case TopLeft:
        coordinatesRect.moveTo(pPoint.x()-coordinatesRect.width()-1,
                               pPoint.y()-coordinatesRect.height()-1);

        break;
    case TopRight:
        coordinatesRect.moveTo(pPoint.x()+2,
                               pPoint.y()-coordinatesRect.height()-1);

        break;
    case BottomLeft:
        coordinatesRect.moveTo(pPoint.x()-coordinatesRect.width()-1,
                               pPoint.y()+2);

        break;
    case BottomRight:
        coordinatesRect.moveTo(pPoint.x()+2,
                               pPoint.y()+2);

        break;
    }

    if (pCanMoveLocation) {
        QwtScaleMap canvasMapX = mOwner->canvasMap(QwtPlot::xBottom);
        QwtScaleMap canvasMapY = mOwner->canvasMap(QwtPlot::yLeft);

        QPoint topLeftPoint = QPoint(canvasMapX.transform(mOwner->minX()),
                                     canvasMapY.transform(mOwner->maxY()));
        QPoint bottomRightPoint = QPoint(canvasMapX.transform(mOwner->maxX()),
                                         canvasMapY.transform(mOwner->minY()));

        if (coordinatesRect.top() < topLeftPoint.y())
            coordinatesRect.moveTop(pPoint.y()+2);
        else if (coordinatesRect.top()+coordinatesRect.height()-1 > bottomRightPoint.y())
            coordinatesRect.moveTop(pPoint.y()-coordinatesRect.height()-1);

        if (coordinatesRect.left() < topLeftPoint.x())
            coordinatesRect.moveLeft(pPoint.x()+2);
        else if (coordinatesRect.left()+coordinatesRect.width()-1 > bottomRightPoint.x())
            coordinatesRect.moveLeft(pPoint.x()-coordinatesRect.width()-1);

        // Note: the -1 for the else-if tests is because fillRect() below works
        //       on (0, 0; width-1, height-1)...
    }

    // Draw a filled rectangle to act as the background for the coordinates
    // we are to show

    pPainter->fillRect(coordinatesRect, pBackgroundColor);

    // Draw the text for the coordinates, using a white pen

    QPen pen = pPainter->pen();

    pen.setColor(pForegroundColor);

    pPainter->setPen(pen);

    pPainter->drawText(coordinatesRect, coordinates);
}

//==============================================================================

void SingleCellViewGraphPanelPlotScaleDraw::retranslateUi()
{
    // Retranslate ourselves by invalidating our cache

    invalidateCache();
}

//==============================================================================

QwtText SingleCellViewGraphPanelPlotScaleDraw::label(double pValue) const
{
    if (qFuzzyCompare(pValue, 0.0))
        // Due to the limited precision of floating point numbers, pValue isn't
        // exactly equal to zero while it really oughht to be, so set it
        // properly

        pValue = 0.0;

    return QLocale().toString(pValue, 'g', 15);
}

//==============================================================================

static const double DblMaxAxis = 0.3*DBL_MAX;
// Note: normally, we would use DBL_MAX, but this means that our maximum axis
//       range would be 2*DBL_MAX, which would cause problems with QwtPlot (e.g.
//       to create ticks), so instead we use a value that results in a range
//       that we know will work...

static const double MinAxis = -DblMaxAxis;
static const double MaxAxis =  DblMaxAxis;

static const double MaxAxisRange = MaxAxis-MinAxis;
static const double MinAxisRange = 1.0e-5;
// Note: we use double precision (as opposed to single precision), however it
//       would seem that, at times, Qwt switches between the two types of
//       precisions. Indeed, if we are to zoom in too much then graphs may be
//       rendered incorrectly. So, knowing that single precision corresponds to
//       a 6-digit precision, we give ourselves a bit of a safety net and allow
//       minimum axis range of 10^-5...

//==============================================================================

SingleCellViewGraphPanelPlotWidget::SingleCellViewGraphPanelPlotWidget(const SingleCellViewGraphPanelPlotWidgets &pNeighbors, QWidget *pParent) :
    QwtPlot(pParent),
    Core::CommonWidget(),
    mGraphs(SingleCellViewGraphPanelPlotGraphs()),
    mAction(None),
    mOriginPoint(QPoint()),
    mPoint(QPoint()),
    mCanDirectPaint(true),
    mCanZoomInX(true),
    mCanZoomOutX(true),
    mCanZoomInY(true),
    mCanZoomOutY(true),
    mNeedContextMenu(false),
    mNeighbors(pNeighbors)
{
    // Get ourselves a direct painter

    mDirectPainter = new QwtPlotDirectPainter(this);

    mDirectPainter->setAttribute(QwtPlotDirectPainter::CopyBackingStore, true);

    // Speedup painting on X11 systems
    // Note: this can only be done on X11 systems...

    if (QwtPainter::isX11GraphicsSystem())
        canvas()->setAttribute(Qt::WA_PaintOnScreen, true);

    // Customise ourselves a bit

    setCanvasBackground(Qt::white);

    mAxisX = new SingleCellViewGraphPanelPlotScaleDraw();
    mAxisY = new SingleCellViewGraphPanelPlotScaleDraw();

    setAxisScaleDraw(QwtPlot::xBottom, mAxisX);
    setAxisScaleDraw(QwtPlot::yLeft, mAxisY);

    // We don't want a frame around ourselves

    qobject_cast<QwtPlotCanvas *>(canvas())->setFrameShape(QFrame::NoFrame);

    // Attach a grid to ourselves

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajorPen(Qt::gray, 0, Qt::DotLine);

    grid->attach(this);

    // Create our overlay widget

    mOverlayWidget = new SingleCellViewGraphPanelPlotOverlayWidget(this);

    // Create our context menu

    mContextMenu = new QMenu(this);

    mCopyToClipboardAction = Core::newAction(this);
    mZoomInAction = Core::newAction(this);
    mZoomOutAction = Core::newAction(this);
    mResetZoomAction = Core::newAction(this);

    connect(mCopyToClipboardAction, SIGNAL(triggered(bool)),
            this, SLOT(copyToClipboard()));
    connect(mZoomInAction, SIGNAL(triggered(bool)),
            this, SLOT(zoomIn()));
    connect(mZoomOutAction, SIGNAL(triggered(bool)),
            this, SLOT(zoomOut()));
    connect(mResetZoomAction, SIGNAL(triggered(bool)),
            this, SLOT(resetZoom()));

    mContextMenu->addAction(mCopyToClipboardAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mZoomInAction);
    mContextMenu->addAction(mZoomOutAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mResetZoomAction);

    // Set our axes' values
    // Note: we are not all initialised yet, so we don't want setAxes() to
    //       replot ourselves...

    setAxes(DefMinAxis, DefMaxAxis, DefMinAxis, DefMaxAxis, false, false);

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
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

    I18nInterface::retranslateAction(mCopyToClipboardAction, tr("Copy"),
                                     tr("Copy the contents of the graph panel to the clipboard"));
    I18nInterface::retranslateAction(mZoomInAction, tr("Zoom In"),
                                     tr("Zoom in the graph panel"));
    I18nInterface::retranslateAction(mZoomOutAction, tr("Zoom Out"),
                                     tr("Zoom out the graph panel"));
    I18nInterface::retranslateAction(mResetZoomAction, tr("Reset Zoom"),
                                     tr("Reset the zoom level of the graph panel"));

    // Replot ourselves
    // Note: we do this because we want to display numbers using digit grouping,
    //       this respecting the current locale...

    mAxisX->retranslateUi();
    mAxisY->retranslateUi();
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::eventFilter(QObject *pObject,
                                                     QEvent *pEvent)
{
    // Default handling of the event

    bool res = QwtPlot::eventFilter(pObject, pEvent);

    // We want to handle a double mouse click, but for some reasons to override
    // mouseDoubleClickEvent() doesn't work, so we do it ourselves

    if (pEvent->type() == QEvent::MouseButtonDblClick)
        handleMouseDoubleClickEvent(static_cast<QMouseEvent *>(pEvent));

    return res;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::handleMouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Reset the zoom level (i.e. our axes), in case we double-clicked using the
    // left mouse button with no modifiers

    if (   (pEvent->button() == Qt::LeftButton)
        && (pEvent->modifiers() == Qt::NoModifier)) {
        resetAxes();
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::updateActions()
{
    // Update our actions

    double crtMinX = minX();
    double crtMaxX = maxX();
    double crtMinY = minY();
    double crtMaxY = maxY();

    double crtRangeX = crtMaxX-crtMinX;
    double crtRangeY = crtMaxY-crtMinY;

    mCanZoomInX  = crtRangeX > MinAxisRange;
    mCanZoomOutX = crtRangeX < MaxAxisRange;

    mCanZoomInY  = crtRangeY > MinAxisRange;
    mCanZoomOutY = crtRangeY < MaxAxisRange;

    // Update the enabled status of our actions

    mZoomInAction->setEnabled(mCanZoomInX || mCanZoomInY);
    mZoomOutAction->setEnabled(mCanZoomOutX || mCanZoomOutY);

    QRectF dRect = dataRect();

    if (dRect == QRectF()) {
        dRect = QRectF(DefMinAxis, DefMinAxis,
                       DefMaxAxis-DefMinAxis, DefMaxAxis-DefMinAxis);
    } else {
        dRect = optimisedRect(dRect);
    }

    mResetZoomAction->setEnabled(   (crtMinX != dRect.left())
                                 || (crtMaxX != dRect.left()+dRect.width())
                                 || (crtMinY != dRect.top())
                                 || (crtMaxY != dRect.top()+dRect.height()));
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::checkAxisValues(double &pMin,
                                                         double &pMax)
{
    // Make sure that our axis' values have finite values

    if (!qIsFinite(pMin))
        pMin = MinAxis;

    if (!qIsFinite(pMax))
        pMax = MaxAxis;

    // Make sure that our axis' values are valid

    double range = pMax-pMin;

    if (range > MaxAxisRange) {
        // The range is too big, so reset our values

        pMin = MinAxis;
        pMax = MaxAxis;
    } else if (range < MinAxisRange) {
        // The range is too small, so reset our values

        pMin = qMax(MinAxis, 0.5*(pMin+pMax-MinAxisRange));
        pMax = qMin(MaxAxis, pMin+MinAxisRange);
        pMin = pMax-MinAxisRange;
        // Note: the last statement is in case pMax was set to MaxAxis, in which
        //       case pMin has to be re-reset...
    } else if (pMin < MinAxis) {
        // The minimum value is too small, so reset it

        pMin = MinAxis;
        pMax = pMin+range;
    } else if (pMax > MaxAxis) {
        // The maximum value is too big, so reset it

        pMax = MaxAxis;
        pMin = pMax-range;
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::checkAxesValues(double &pMinX,
                                                         double &pMaxX,
                                                         double &pMinY,
                                                         double &pMaxY)
{
    // Make sure that our axes' values are fine

    checkAxisValues(pMinX, pMaxX);
    checkAxisValues(pMinY, pMaxY);
}

//==============================================================================

SingleCellViewGraphPanelPlotWidget::Action SingleCellViewGraphPanelPlotWidget::action() const
{
    // Return our action

    return mAction;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::resetAction()
{
    // Reset our action and our overlay widget, by updating it

    mAction = None;

    mOverlayWidget->update();
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::minX() const
{
    // Return our minimum X value

    return axisScaleDiv(QwtPlot::xBottom).lowerBound();
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::maxX() const
{
    // Return our maximum X value

    return axisScaleDiv(QwtPlot::xBottom).upperBound();
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::minY() const
{
    // Return our minimum Y value

    return axisScaleDiv(QwtPlot::yLeft).lowerBound();
}

//==============================================================================

double SingleCellViewGraphPanelPlotWidget::maxY() const
{
    // Return our maximum Y value

    return axisScaleDiv(QwtPlot::yLeft).upperBound();

}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::canZoomInX() const
{
    // Return whether we can zoom in on the X axis

    return mCanZoomInX;
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::canZoomOutX() const
{
    // Return whether we can zoom out on the X axis

    return mCanZoomOutX;
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::canZoomInY() const
{
    // Return whether we can zoom in on the Y axis

    return mCanZoomInY;
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::canZoomOutY() const
{
    // Return whether we can zoom out on the Y axis

    return mCanZoomOutY;
}

//==============================================================================

SingleCellViewGraphPanelPlotGraphs SingleCellViewGraphPanelPlotWidget::graphs() const
{
    // Return all our graphs

    return mGraphs;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::optimiseAxis(const int &pAxisId,
                                                      double &pMin,
                                                      double &pMax) const
{
    // Make sure that the given values are different

    if (pMin == pMax) {
        // The given values are the same, so update them so that we can properly
        // optimise them below

        double powerValue = pMin?qFloor(log10(qAbs(pMin)))-1.0:0.0;

        pMin = pMin-pow(10.0, powerValue);
        pMax = pMax+pow(10.0, powerValue);
    }

    // Optimise the axis' values so that they fall onto a factor of the axis'
    // minor step

    uint base = axisScaleEngine(pAxisId)->base();
    double majorStep = QwtScaleArithmetic::divideInterval(pMax-pMin,
                                                          axisMaxMajor(pAxisId),
                                                          base);
    double minorStep = QwtScaleArithmetic::divideInterval(majorStep,
                                                          axisMaxMinor(pAxisId),
                                                          base);

    pMin = qFloor(pMin/minorStep)*minorStep;
    pMax = qCeil(pMax/minorStep)*minorStep;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::optimiseAxisX(double &pMin, double &pMax) const
{
    // Optimise our X axis' values

    optimiseAxis(QwtPlot::xBottom, pMin, pMax);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::optimiseAxisY(double &pMin, double &pMax) const
{
    // Optimise our Y axis' values

    optimiseAxis(QwtPlot::yLeft, pMin, pMax);
}

//==============================================================================

QRectF SingleCellViewGraphPanelPlotWidget::optimisedRect(const QRectF &pAxes) const
{
    // Optimise our axes' values

    double minX = pAxes.left();
    double maxX = minX+pAxes.width();
    double minY = pAxes.top();
    double maxY = minY+pAxes.height();

    optimiseAxisX(minX, maxX);
    optimiseAxisY(minY, maxY);

    return QRectF(minX, minY, maxX-minX, maxY-minY);
}

//==============================================================================

QRectF SingleCellViewGraphPanelPlotWidget::dataRect() const
{
    // Determine and return the rectangle within which all the graphs, which are
    // valid, selected and have some data, can fit

    QRectF res = QRect();

    foreach (SingleCellViewGraphPanelPlotGraph *graph, mGraphs) {
        if (graph->isValid() && graph->isSelected() && graph->dataSize())
            res |= graph->boundingRect();
    }

    return res;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::setAxis(const int &pAxis,
                                                 double pMin, double pMax)
{
    // Set our axis
    // Note: to use setAxisScale() on its own is not sufficient unless we were
    //       to replot ourselves immediately after, but we don't want to do
    //       that, so instead we also use setAxisScaleDiv() to make sure that
    //       our axis is indeed taken into account (i.e. we can retrieve them
    //       using minX(), maxX(), minY() and maxY()). Also, we must call
    //       setAxisScaleDiv() before setAxisScale() to make sure that the axis
    //       data is not considered as valid, which is important when it comes
    //       to plotting ourselves...

    setAxisScaleDiv(pAxis, QwtScaleDiv(pMin, pMax));
    setAxisScale(pAxis, pMin, pMax);
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::setAxes(double pMinX, double pMaxX,
                                                 double pMinY, double pMaxY,
                                                 const bool &pCanReplot,
                                                 const bool &pEmitSignal)
{
    // Keep track of our axes' old values

    double oldMinX = minX();
    double oldMaxX = maxX();
    double oldMinY = minY();
    double oldMaxY = maxY();

    // Make sure that the given axes' values are fine

    checkAxesValues(pMinX, pMaxX, pMinY, pMaxY);

    // Update our axes' values, if needed

    bool axesValuesChanged = false;

    if ((pMinX != oldMinX) || (pMaxX != oldMaxX)) {
        setAxis(QwtPlot::xBottom, pMinX, pMaxX);

        axesValuesChanged = true;
    }

    if ((pMinY != oldMinY) || (pMaxY != oldMaxY)) {
        setAxis(QwtPlot::yLeft, pMinY, pMaxY);

        axesValuesChanged = true;
    }

    // Update our actions and align ourselves with our neighbours (which will
    // result in ourselves, and maybe its neighbours, bein replotted, if
    // allowed), in case the axes' values have changed

    if (axesValuesChanged) {
        mCanDirectPaint = false;

        updateActions();

        alignWithNeighbors(pCanReplot);

        if (pEmitSignal)
            emit axesChanged(pMinX, pMaxX, pMinY, pMaxY);

        return pCanReplot;
    } else {
        return false;
    }
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::resetAxes()
{
    // Reset our axes by setting their values to either default ones or to some
    // that allow to see all the graphs

    QRectF axesRect = dataRect().isNull()?
                          QRectF(DefMinAxis, DefMinAxis,
                                 DefMaxAxis-DefMinAxis, DefMaxAxis-DefMinAxis):
                          optimisedRect(dataRect());

    return setAxes(axesRect.left(), axesRect.left()+axesRect.width(),
                   axesRect.top(), axesRect.top()+axesRect.height());
}

//==============================================================================

bool SingleCellViewGraphPanelPlotWidget::scaleAxis(const double &pScalingFactor,
                                                   const bool &pCanZoomIn,
                                                   const bool &pCanZoomOut,
                                                   const double pOriginPoint,
                                                   double &pMin, double &pMax)
{
    // Check whether we can scale the axis and, if so, determine what its new
    // values should be

    if (   ((pScalingFactor < 1.0) && pCanZoomIn)
        || ((pScalingFactor > 1.0) && pCanZoomOut)) {
        double oldRange = pMax-pMin;
        double newRange = pScalingFactor*oldRange;
        double factor = qMin(1.0, qMax(0.0, (pOriginPoint-pMin)/oldRange));
        // Note: QwtPlot puts some extra space around the area we want to show,
        //       which means that we could end up with a factor which is either
        //       smaller than zero or bigger than one, hence we have to make
        //       sure that it is clamped within the [0; 1] range...

        pMin = qMax(MinAxis, pOriginPoint-factor*newRange);
        pMax = qMin(MaxAxis, pMin+newRange);
        pMin = pMax-newRange;
        // Note: the last statement is in case pNewMax has been set to MaxAxis,
        //       in which case we need to re-update pNewMin...

        return true;
    } else {
        return false;
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::scaleAxes(const QPoint &pPoint,
                                                   const double &pScalingFactorX,
                                                   const double &pScalingFactorY)
{
    // Rescale our X axis, but only if zooming in/out is possible on that axis

    QPointF originPoint = canvasPoint(pPoint);

    double newMinX = minX();
    double newMaxX = maxX();
    double newMinY = minY();
    double newMaxY = maxY();
    bool scaledAxisX = scaleAxis(pScalingFactorX, mCanZoomInX, mCanZoomOutX,
                                 originPoint.x(), newMinX, newMaxX);
    bool scaledAxisY = scaleAxis(pScalingFactorY, mCanZoomInY, mCanZoomOutY,
                                 originPoint.y(), newMinY, newMaxY);
    // Note: we want to make both calls to scaleAxis(), hence they are not part
    //       of the if() statement below...

    if (scaledAxisX || scaledAxisY)
        setAxes(newMinX, newMaxX, newMinY, newMaxY);
}

//==============================================================================

QPointF SingleCellViewGraphPanelPlotWidget::canvasPoint(const QPoint &pPoint,
                                                        const bool pNeedOffset) const
{
    // Return the mouse position using canvas coordinates, making sure that they
    // are within our ranges

    QPointF realPoint = pPoint;

    if (pNeedOffset)
        realPoint -= plotLayout()->canvasRect().topLeft();

    return QPointF(qMin(maxX(), qMax(minX(), canvasMap(QwtPlot::xBottom).invTransform(realPoint.x()))),
                   qMin(maxY(), qMax(minY(), canvasMap(QwtPlot::yLeft).invTransform(realPoint.y()))));
}

//==============================================================================

static const double NoScalingFactor     = 1.0;
static const double ScalingInFactor     = 0.9;
static const double ScalingOutFactor    = 1.0/ScalingInFactor;
static const double BigScalingInFactor  = 0.5*ScalingInFactor;
static const double BigScalingOutFactor = 1.0/BigScalingInFactor;

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Carry out the action

    switch (mAction) {
    case None:
        // No action, so do nothing

        ;
    case Pan: {
        // Determine the X/Y shifts for our panning

        QPointF origPoint = canvasPoint(mPoint);
        QPointF crtPoint = canvasPoint(pEvent->pos());

        double shiftX = crtPoint.x()-origPoint.x();
        double shiftY = crtPoint.y()-origPoint.y();

        mPoint = pEvent->pos();

        // Set our axes' new values

        setAxes(minX()-shiftX, maxX()-shiftX, minY()-shiftY, maxY()-shiftY);

        break;
    }
    case ShowCoordinates:
        // Update the point of our overlay widget

        mOverlayWidget->setPoint(pEvent->pos());

        break;
    case Zoom: {
        // Determine our X/Y delta values

        QPointF origPoint = canvasPoint(mPoint);
        QPointF crtPoint = canvasPoint(pEvent->pos());

        double deltaX = crtPoint.x()-origPoint.x();
        double deltaY = crtPoint.y()-origPoint.y();

        mPoint = pEvent->pos();

        // Rescale ourselves
        // Note: this will automatically replot ourselves...

        scaleAxes(mOriginPoint,
                  deltaX?
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
        // Update our zoom region by updating the point of our overlay widget

        mOverlayWidget->setPoint(pEvent->pos());

        break;
    }

    // The mouse has moved, so we definitely won't need to show our context menu

    mNeedContextMenu = false;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mousePressEvent(pEvent);

    // Check that the position of the mouse is over our canvas

    if (!plotLayout()->canvasRect().contains(pEvent->pos()))
        return;

    // Make sure that we are not already carrying out a action (e.g. we were
    // zooming in/out and then pressed on the left mouse button) and if so, then
    // cancel it by resetting our action

    if (mAction != None) {
        resetAction();

        return;
    }

    // Check which action to can carry out

    if (   (pEvent->button() == Qt::LeftButton)
        && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to pan, but only do this if we are not completely zoomed out

        if (mCanZoomOutX || mCanZoomOutY) {
            mAction = Pan;

            mPoint = pEvent->pos();
        }
    } else if (   (pEvent->button() == Qt::LeftButton)
               && (pEvent->modifiers() == Qt::ShiftModifier)) {
        // We want to show the coordinates

        mAction = ShowCoordinates;

        mOverlayWidget->setPoint(pEvent->pos());
    } else if (   (pEvent->button() == Qt::RightButton)
               && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to zoom in/out

        mAction = Zoom;

        mOriginPoint = pEvent->pos();
        mPoint = pEvent->pos();
    } else if (   (pEvent->button() == Qt::RightButton)
               && (pEvent->modifiers() == Qt::ControlModifier)) {
        // We want to zoom a region, but only allow it if we are not already
        // fully zoomed in

        if (mCanZoomInX || mCanZoomInY) {
            mAction = ZoomRegion;

            mOverlayWidget->setOriginPoint(pEvent->pos());
            mOverlayWidget->setPoint(pEvent->pos());
        }
    }

    // Check whether we might need to show our context menu

    mNeedContextMenu = pEvent->button() == Qt::RightButton;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseReleaseEvent(pEvent);

    // Check whether we need to carry out a action

    if (mAction == None)
        return;

    // Finish carrying out the action, if needed

    switch (mAction) {
    case ZoomRegion: {
        // Retrieve our zoom region

        QRect zoomRegionRect = mOverlayWidget->zoomRegion();

        // Reset our action

        resetAction();

        // Effectively zoom our region, if possible, by updating our axes

        QRectF zoomRegion = QRectF(canvasPoint(zoomRegionRect.topLeft(), false),
                                   canvasPoint(zoomRegionRect.topLeft()+QPoint(zoomRegionRect.width(), zoomRegionRect.height()), false));

        if (zoomRegion.width() && zoomRegion.height()) {
            setAxes(zoomRegion.left(), zoomRegion.left()+zoomRegion.width(),
                    zoomRegion.top()+zoomRegion.height(), zoomRegion.top());
        }

        break;
    }
    default:
        // An action that doesn't require anything specific to be done, except
        // being reset

        resetAction();
    }

    // Show our context menu, if still needed

    if (mNeedContextMenu) {
        mOriginPoint = mapFromGlobal(QCursor::pos());

        mContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::resizeEvent(pEvent);

    // Update the size of our overlay widget

    mOverlayWidget->resize(pEvent->size());
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Handle the wheel mouse button for zooming in/out

    if (pEvent->modifiers() == Qt::NoModifier) {
        // Make sure that we are not already carrying out a action

        if (mAction == None) {
            int delta = pEvent->delta();
            double scalingFactor = 0.0;

            if (delta > 0)
                scalingFactor = ScalingInFactor;
            else if (delta < 0)
                scalingFactor = ScalingOutFactor;

            if (scalingFactor)
                scaleAxes(pEvent->pos(), scalingFactor, scalingFactor);
        }

        pEvent->accept();
    } else {
        // Not the modifier we were expecting, so call the default handling of
        // the event

        QwtPlot::wheelEvent(pEvent);
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::replotNow()
{
    // Replot ourselves

    replot();

    // Make sure that the replotting occurs immediately
    // Note: this is needed when running a simulation since, otherwise,
    //       replotting won't occur immediately (because of threading)...

    QCoreApplication::processEvents();
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

void SingleCellViewGraphPanelPlotWidget::drawGraphFrom(SingleCellViewGraphPanelPlotGraph *pGraph,
                                                       const qulonglong &pFrom)
{
    // Direct paint our graph from the given point unless we can't direct paint
    // (due to the axes having been changed), in which case we replot ourselves

    if (mCanDirectPaint) {
        mDirectPainter->drawSeries(pGraph, pFrom, -1);
    } else {
        replotNow();

        mCanDirectPaint = true;
    }
}

//==============================================================================

SingleCellViewGraphPanelPlotWidgets SingleCellViewGraphPanelPlotWidget::neighbors() const
{
    // Return our neighbours

    return mNeighbors;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::addNeighbor(SingleCellViewGraphPanelPlotWidget *pPlot)
{
    // Add the plot as a neighbour

    if ((pPlot != this) && !mNeighbors.contains(pPlot))
        mNeighbors << pPlot;
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::removeNeighbor(SingleCellViewGraphPanelPlotWidget *pPlot)
{
    // Remove the plot from our neighbours

    mNeighbors.removeOne(pPlot);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::alignWithNeighbors(const bool &pCanReplot,
                                                            const bool &pForceAlignment)
{
    // Align ourselves with our neighbours unless we don't have any in which
    // case we just replot ourselves, if allowed

    if (mNeighbors.isEmpty()) {
        if (pCanReplot)
            replotNow();
    } else {
        for (int i = 0; i < 2; ++i) {
            // Note: we do the below twice because there are cases where it may
            //       not work properly. Indeed, say that we have two graph
            //       panels, which by default will have their Y axis ranging
            //       from 0 to 1,000. Now, say that we zoom out the first graph
            //       panel and end up with somewhat wide Y labels, e.g. from
            //       -1,000,000 to 1,000. While doing this, the second graph
            //       panel will have kept aligning itself with the first graph
            //       panel, as expected. Now, say that we reset the zoom of the
            //       first graph panel. At this point, you would expect the Y
            //       axis to range from 0 to 1,000 and it is the case, but we
            //       can see loads of empty space to the left of the Y axis and
            //       this on both graph panels. However, that empty space
            //       disappears if we do the below twice...

            SingleCellViewGraphPanelPlotWidgets selfPlusNeighbors = SingleCellViewGraphPanelPlotWidgets() << this << mNeighbors;
            double oldMaxExtent = axisWidget(QwtPlot::yLeft)->scaleDraw()->minimumExtent();
            double newMaxExtent = 0;

            foreach (SingleCellViewGraphPanelPlotWidget *plot, selfPlusNeighbors) {
                QwtScaleWidget *scaleWidget = plot->axisWidget(QwtPlot::yLeft);
                QwtScaleDraw *scaleDraw = scaleWidget->scaleDraw();

                scaleDraw->setMinimumExtent(0.0);

                double extent = scaleDraw->extent(scaleWidget->font());

                if (extent > newMaxExtent)
                    newMaxExtent = extent;
            }

            foreach (SingleCellViewGraphPanelPlotWidget *plot, selfPlusNeighbors) {
                plot->axisWidget(QwtPlot::yLeft)->scaleDraw()->setMinimumExtent(newMaxExtent);

                if (pCanReplot) {
                    if (pForceAlignment || (newMaxExtent != oldMaxExtent)) {
                        plot->updateLayout();
                        plot->replotNow();
                    } else if (plot == this) {
                        replotNow();
                    }
                }
            }
        }
    }
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::forceAlignWithNeighbors()
{
    // Force the re-alignment with our neighbours

    alignWithNeighbors(true, true);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::copyToClipboard()
{
    // Copy our contents to the clipboard

    QApplication::clipboard()->setPixmap(grab());
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::zoomIn()
{
    // Zoom in by scaling our two axes around the point where the context menu
    // was shown

    scaleAxes(mOriginPoint, BigScalingInFactor, BigScalingInFactor);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::zoomOut()
{
    // Zoom out by scaling our two axes around the point where the context menu
    // was shown

    scaleAxes(mOriginPoint, BigScalingOutFactor, BigScalingOutFactor);
}

//==============================================================================

void SingleCellViewGraphPanelPlotWidget::resetZoom()
{
    // Reset the zoom level by resetting our axes

    resetAxes();
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

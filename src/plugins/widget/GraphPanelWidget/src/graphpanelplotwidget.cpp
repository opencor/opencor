/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Graph panel plot widget
//==============================================================================

#include "coreguiutils.h"
#include "graphpanelplotwidget.h"
#include "graphpanelwidgetcustomaxesdialog.h"
#include "i18ninterface.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QImageWriter>
#include <QMenu>
#include <QPaintEvent>

//==============================================================================

#include <float.h>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_painter.h"
    #include "qwt_plot_canvas.h"
    #include "qwt_plot_directpainter.h"
    #include "qwt_plot_grid.h"
    #include "qwt_plot_layout.h"
    #include "qwt_plot_renderer.h"
    #include "qwt_scale_engine.h"
    #include "qwt_scale_widget.h"
    #include "qwt_text_label.h"
#include "qwtend.h"

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

GraphPanelPlotGraphProperties::GraphPanelPlotGraphProperties(const Qt::PenStyle &pLineStyle,
                                                             const double &pLineWidth,
                                                             const QColor &pLineColor,
                                                             const QwtSymbol::Style &pSymbolStyle,
                                                             const int &pSymbolSize,
                                                             const QColor &pSymbolColor,
                                                             const bool &pSymbolFilled,
                                                             const QColor &pSymbolFillColor) :
    mLineStyle(pLineStyle),
    mLineWidth(pLineWidth),
    mLineColor(pLineColor),
    mSymbolStyle(pSymbolStyle),
    mSymbolSize(pSymbolSize),
    mSymbolColor(pSymbolColor),
    mSymbolFilled(pSymbolFilled),
    mSymbolFillColor(pSymbolFillColor)
{
}

//==============================================================================

Qt::PenStyle GraphPanelPlotGraphProperties::lineStyle() const
{
    // Return our line style

    return mLineStyle;
}

//==============================================================================

double GraphPanelPlotGraphProperties::lineWidth() const
{
    // Return our line width

    return mLineWidth;
}

//==============================================================================

QColor GraphPanelPlotGraphProperties::lineColor() const
{
    // Return our line colour

    return mLineColor;
}

//==============================================================================

QwtSymbol::Style GraphPanelPlotGraphProperties::symbolStyle() const
{
    // Return our symbol style

    return mSymbolStyle;
}

//==============================================================================

int GraphPanelPlotGraphProperties::symbolSize() const
{
    // Return our symbol size

    return mSymbolSize;
}

//==============================================================================

QColor GraphPanelPlotGraphProperties::symbolColor() const
{
    // Return our symbol colour

    return mSymbolColor;
}

//==============================================================================

bool GraphPanelPlotGraphProperties::symbolFilled() const
{
    // Return our symbol filled status

    return mSymbolFilled;
}

//==============================================================================

QColor GraphPanelPlotGraphProperties::symbolFillColor() const
{
    // Return our symbol fill colour

    return mSymbolFillColor;
}

//==============================================================================

static const QRectF NoBoundingLogRect = QRectF(0.0, 0.0, -1.0, -1.0);

//==============================================================================

GraphPanelPlotGraph::GraphPanelPlotGraph(void *pParameterX, void *pParameterY) :
    QwtPlotCurve(),
    mSelected(true),
    mFileName(QString()),
    mParameterX(pParameterX),
    mParameterY(pParameterY),
    mBoundingLogRect(NoBoundingLogRect)
{
    // Customise ourselves a bit

    setPen(QPen(Qt::darkBlue, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setRenderHint(QwtPlotItem::RenderAntialiased);
}

//==============================================================================

bool GraphPanelPlotGraph::isValid() const
{
    // Return whether we are valid

    return   !mFileName.isEmpty()
           && mParameterX && mParameterY;
}

//==============================================================================

bool GraphPanelPlotGraph::isSelected() const
{
    // Return whether we are selected

    return mSelected;
}

//==============================================================================

void GraphPanelPlotGraph::setSelected(const bool &pSelected)
{
    // Set our selected state

    mSelected = pSelected;
}

//==============================================================================

QString GraphPanelPlotGraph::fileName() const
{
    // Return our file name

    return mFileName;
}

//==============================================================================

void GraphPanelPlotGraph::setFileName(const QString &pFileName)
{
    // Set our file name

    mFileName = pFileName;
}

//==============================================================================

void * GraphPanelPlotGraph::parameterX() const
{
    // Return our parameter X

    return mParameterX;
}

//==============================================================================

void GraphPanelPlotGraph::setParameterX(void *pParameterX)
{
    // Set our parameter X

    mParameterX = pParameterX;
}

//==============================================================================

void * GraphPanelPlotGraph::parameterY() const
{
    // Return our parameter Y

    return mParameterY;
}

//==============================================================================

void GraphPanelPlotGraph::setParameterY(void *pParameterY)
{
    // Set our parameter Y

    mParameterY = pParameterY;
}

//==============================================================================

void GraphPanelPlotGraph::setData(double *pDataX, double *pDataY,
                                  const int &pSize)
{
    // Set our data, i.e. raw samples

    setRawSamples(pDataX, pDataY, pSize);

    // Reset our cache version of our log data rectangle

    mBoundingLogRect = NoBoundingLogRect;
}

//==============================================================================

QRectF GraphPanelPlotGraph::boundingLogRect()
{
    // Return the cached version of our bounding log rectangle, if we have one,
    // or compute it and return it

    if (mBoundingLogRect == NoBoundingLogRect) {
        const QwtSeriesData<QPointF> *crtData = data();
        bool needInitMinX = true;
        bool needInitMaxX = true;
        bool needInitMinY = true;
        bool needInitMaxY = true;
        double minX = 1.0;
        double maxX = 1.0;
        double minY = 1.0;
        double maxY = 1.0;

        for (size_t i = 0, iMax = crtData->size(); i < iMax; ++i) {
            QPointF sample = crtData->sample(i);

            if (sample.x() > 0.0) {
                if (needInitMinX) {
                    minX = sample.x();

                    needInitMinX = false;
                } else if (sample.x() < minX) {
                    minX = sample.x();
                }

                if (needInitMaxX) {
                    maxX = sample.x();

                    needInitMaxX = false;
                } else if (sample.x() > maxX) {
                    maxX = sample.x();
                }
            }

            if (sample.y() > 0.0) {
                if (needInitMinY) {
                    minY = sample.y();

                    needInitMinY = false;
                } else if (sample.y() < minY) {
                    minY = sample.y();
                }

                if (needInitMaxY) {
                    maxY = sample.y();

                    needInitMaxY = false;
                } else if (sample.y() > maxY) {
                    maxY = sample.y();
                }
            }
        }

        if (!needInitMinX && !needInitMaxX && !needInitMinY && !needInitMaxY)
            mBoundingLogRect = QRectF(minX, minY, maxX-minX, maxY-minY);
    }

    return mBoundingLogRect;
}

//==============================================================================

GraphPanelPlotOverlayWidget::GraphPanelPlotOverlayWidget(GraphPanelPlotWidget *pParent) :
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

QPoint GraphPanelPlotOverlayWidget::optimisedPoint(const QPoint &pPoint) const
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

void GraphPanelPlotOverlayWidget::paintEvent(QPaintEvent *pEvent)
{
    // Accept the event

    pEvent->accept();

    // Check whether an action is to be carried out

    if (mOwner->action() == GraphPanelPlotWidget::None)
        return;

    // Paint the overlay, if any is needed

    QPainter painter(this);

    QRectF canvasRect = mOwner->plotLayout()->canvasRect();

    painter.translate(canvasRect.x(), canvasRect.y());

    switch (mOwner->action()) {
    case GraphPanelPlotWidget::ShowCoordinates: {
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
    case GraphPanelPlotWidget::ZoomRegion: {
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

void GraphPanelPlotOverlayWidget::setOriginPoint(const QPoint &pOriginPoint)
{
    // Set our point of origin

    mOriginPoint = pOriginPoint;
    // Note: unlike for setPoint(), we must not repaint ourselves since a call
    //       to setOriginPoint() should always be followed by one to setPoint(),
    //       in which case we don't want to repaint ourselves twice, not least
    //       because the first time we may not have a correct mPoint value...
}

//==============================================================================

void GraphPanelPlotOverlayWidget::setPoint(const QPoint &pPoint)
{
    // Set our point

    mPoint = pPoint;

    update();
}

//==============================================================================

QRect GraphPanelPlotOverlayWidget::zoomRegion() const
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

void GraphPanelPlotOverlayWidget::drawCoordinates(QPainter *pPainter,
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

    QPointF point = mOwner->canvasPoint(pPoint);
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

void GraphPanelPlotScaleDraw::retranslateUi()
{
    // Retranslate ourselves by invalidating our cache

    invalidateCache();
}

//==============================================================================

QwtText GraphPanelPlotScaleDraw::label(double pValue) const
{
    // Return pValue as a string, keeping in mind the current locale

    QString label = QLocale().toString(pValue);
    QString fullLabel = QLocale().toString(pValue, 'g', 15);

    return fullLabel.startsWith(label)?fullLabel:label;
}

//==============================================================================

static const double DblMinAxis = 1000*DBL_MIN;
// Note: normally, we would use DBL_MIN, but this would cause problems with
//       QwtPlot (e.g. to create ticks), so instead we use a value that results
//       in a range that we know works...
static const double DblMaxAxis = 0.3*DBL_MAX;
// Note: normally, we would use DBL_MAX, but this means that our maximum axis
//       range would be 2*DBL_MAX, which would cause problems with QwtPlot (e.g.
//       to create ticks), so instead we use a value that results in a range
//       that we know works...

static const double MinAxis    = -DblMaxAxis;
static const double MinLogAxis =  DblMinAxis ;
static const double MaxAxis    =  DblMaxAxis;

static const double MaxAxisRange    = MaxAxis-MinAxis;
static const double MaxLogAxisRange = MaxAxis-MinLogAxis;
static const double MinAxisRange    = DblMinAxis;

//==============================================================================

GraphPanelPlotWidget::GraphPanelPlotWidget(const GraphPanelPlotWidgets &pNeighbors,
                                           QAction *pSynchronizeXAxisAction,
                                           QAction *pSynchronizeYAxisAction,
                                           QWidget *pParent) :
    QwtPlot(pParent),
    Core::CommonWidget(this),
    mColor(QColor()),
    mGraphs(GraphPanelPlotGraphs()),
    mAction(None),
    mOriginPoint(QPoint()),
    mPoint(QPoint()),
    mCanDirectPaint(true),
    mCanReplot(true),
    mCanZoomInX(true),
    mCanZoomOutX(true),
    mCanZoomInY(true),
    mCanZoomOutY(true),
    mNeedContextMenu(false),
    mCanUpdateActions(true),
    mSynchronizeXAxisAction(pSynchronizeXAxisAction),
    mSynchronizeYAxisAction(pSynchronizeYAxisAction),
    mLogAxisX(false),
    mLogAxisY(false),
    mDefaultMinX(DefaultMinAxis),
    mDefaultMaxX(DefaultMaxAxis),
    mDefaultMinY(DefaultMinAxis),
    mDefaultMaxY(DefaultMaxAxis),
    mDefaultMinLogX(DefaultMinLogAxis),
    mDefaultMaxLogX(DefaultMaxAxis),
    mDefaultMinLogY(DefaultMinLogAxis),
    mDefaultMaxLogY(DefaultMaxAxis),
    mNeighbors(pNeighbors)
{
    // Keep track of when our grand parent (i.e. a GraphPanelsWidget object)
    // gets destroyed
    // Note: indeed, when that happens, all of its children (i.e.
    //       GraphPanelWidget objects) will tell their plot's neighbours (i.e.
    //       objects like this one) that they are not going to be their
    //       neighbour anymore (see removeNeighbor()). This will, in turn,
    //       involve updating our actions (see updateActions()), something that
    //       we cannot do when our grand parent gets destroyed...

    connect(pParent->parent(), SIGNAL(destroyed(QObject *)),
            this, SLOT(cannotUpdateActions()));

    // Get ourselves a direct painter

    mDirectPainter = new QwtPlotDirectPainter(this);

    mDirectPainter->setAttribute(QwtPlotDirectPainter::CopyBackingStore, true);

    // Speedup painting on X11 systems
    // Note: this can only be done on X11 systems...

    if (QwtPainter::isX11GraphicsSystem())
        canvas()->setAttribute(Qt::WA_PaintOnScreen, true);

    // We don't want a frame around ourselves

    qobject_cast<QwtPlotCanvas *>(canvas())->setFrameShape(QFrame::NoFrame);

    // Customise ourselves a bit

    setAutoFillBackground(true);
    setColor(Qt::white);
    setFontSize(10, true);

    // Add some axes to ourselves

    mAxisX = new GraphPanelPlotScaleDraw();
    mAxisY = new GraphPanelPlotScaleDraw();

    setAxisScaleDraw(QwtPlot::xBottom, mAxisX);
    setAxisScaleDraw(QwtPlot::yLeft, mAxisY);

    // Attach a grid to ourselves

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajorPen(Qt::gray, 1, Qt::DotLine);

    grid->attach(this);

    // Create our overlay widget

    mOverlayWidget = new GraphPanelPlotOverlayWidget(this);

    // Create our context menu

    mContextMenu = new QMenu(this);

    mExportToAction = Core::newAction(this);
    mCopyToClipboardAction = Core::newAction(this);
    mCustomAxesAction = Core::newAction(this);
    mGraphPanelSettingsAction = Core::newAction(this);
    mGraphsSettingsAction = Core::newAction(this);
    mZoomInAction = Core::newAction(this);
    mZoomOutAction = Core::newAction(this);
    mResetZoomAction = Core::newAction(this);

    connect(mExportToAction, SIGNAL(triggered(bool)),
            this, SLOT(exportTo()));
    connect(mCopyToClipboardAction, SIGNAL(triggered(bool)),
            this, SLOT(copyToClipboard()));
    connect(mCustomAxesAction, SIGNAL(triggered(bool)),
            this, SLOT(customAxes()));
    connect(mGraphPanelSettingsAction, SIGNAL(triggered(bool)),
            this, SIGNAL(graphPanelSettingsRequested()));
    connect(mGraphsSettingsAction, SIGNAL(triggered(bool)),
            this, SIGNAL(graphsSettingsRequested()));
    connect(mZoomInAction, SIGNAL(triggered(bool)),
            this, SLOT(zoomIn()));
    connect(mZoomOutAction, SIGNAL(triggered(bool)),
            this, SLOT(zoomOut()));
    connect(mResetZoomAction, SIGNAL(triggered(bool)),
            this, SLOT(resetZoom()));

    mContextMenu->addAction(mExportToAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyToClipboardAction);

    if (pSynchronizeXAxisAction && pSynchronizeYAxisAction) {
        mContextMenu->addSeparator();
        mContextMenu->addAction(pSynchronizeXAxisAction);
        mContextMenu->addAction(pSynchronizeYAxisAction);
    }

    mContextMenu->addSeparator();
    mContextMenu->addAction(mGraphPanelSettingsAction);
    mContextMenu->addAction(mGraphsSettingsAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCustomAxesAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mZoomInAction);
    mContextMenu->addAction(mZoomOutAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mResetZoomAction);

    // Set our axes' values
    // Note: we are not all initialised yet, so we don't want setAxes() to
    //       replot ourselves...

    setAxes(DefaultMinAxis, DefaultMaxAxis, DefaultMinAxis, DefaultMaxAxis, false, false, false);

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

GraphPanelPlotWidget::~GraphPanelPlotWidget()
{
    // Delete some internal objects

    delete mDirectPainter;

    foreach (GraphPanelPlotGraph *graph, mGraphs)
        delete graph;
}

//==============================================================================

void GraphPanelPlotWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mExportToAction, tr("Export To..."),
                                     tr("Export the contents of the graph panel to a PDF, PNG, SVG, etc. file"));
    I18nInterface::retranslateAction(mCopyToClipboardAction, tr("Copy To Clipboard"),
                                     tr("Copy the contents of the graph panel to the clipboard"));
    I18nInterface::retranslateAction(mCustomAxesAction, tr("Custom Axes..."),
                                     tr("Specify custom axes for the graph panel"));
    I18nInterface::retranslateAction(mGraphPanelSettingsAction, tr("Graph Panel Settings..."),
                                     tr("Customise the graph panel"));
    I18nInterface::retranslateAction(mGraphsSettingsAction, tr("Graphs Settings..."),
                                     tr("Customise the graphs"));
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

bool GraphPanelPlotWidget::eventFilter(QObject *pObject, QEvent *pEvent)
{
    // Default handling of the event

    bool res = QwtPlot::eventFilter(pObject, pEvent);

    // We want to handle a double mouse click, but for some reasons to override
    // mouseDoubleClickEvent() doesn't work, so we do it ourselves

    if (pEvent->type() == QEvent::MouseButtonDblClick) {
        // Reset the zoom level (i.e. our axes), in case we double-clicked using
        // the left mouse button with no modifiers

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(pEvent);

        if (   !(mouseEvent->modifiers() & Qt::ShiftModifier)
            && !(mouseEvent->modifiers() & Qt::ControlModifier)
            && !(mouseEvent->modifiers() & Qt::AltModifier)
            && !(mouseEvent->modifiers() & Qt::MetaModifier)
            &&  (mouseEvent->button() == Qt::LeftButton)) {
            resetAxes();
        }
    }

    return res;
}

//==============================================================================

void GraphPanelPlotWidget::updateActions()
{
    // Leave straightaway if we cannot update our actions anymore

    if (!mCanUpdateActions)
        return;

    // Update our actions

    double crtMinX = minX();
    double crtMaxX = maxX();
    double crtMinY = minY();
    double crtMaxY = maxY();

    double crtRangeX = crtMaxX-crtMinX;
    double crtRangeY = crtMaxY-crtMinY;

    mCanZoomInX = crtRangeX > MinAxisRange;
    mCanZoomOutX = crtRangeX < (logAxisX()?MaxLogAxisRange:MaxAxisRange);

    mCanZoomInY = crtRangeY > MinAxisRange;
    mCanZoomOutY = crtRangeY < (logAxisY()?MaxLogAxisRange:MaxAxisRange);

    // Update the enabled status of our actions

    mSynchronizeXAxisAction->setVisible(!mNeighbors.isEmpty());
    mSynchronizeYAxisAction->setVisible(!mNeighbors.isEmpty());

    mZoomInAction->setEnabled(mCanZoomInX || mCanZoomInY);
    mZoomOutAction->setEnabled(mCanZoomOutX || mCanZoomOutY);

    QRectF dRect = realDataRect();

    mResetZoomAction->setEnabled(   (crtMinX != dRect.left())
                                 || (crtMaxX != dRect.left()+dRect.width())
                                 || (crtMinY != dRect.top())
                                 || (crtMaxY != dRect.top()+dRect.height()));
}

//==============================================================================

void GraphPanelPlotWidget::checkAxisValues(const bool &pLogAxis, double &pMin,
                                           double &pMax)
{
    // Make sure that our axis' values have finite values

    double minAxis = pLogAxis?MinLogAxis:MinAxis;

    if (!qIsFinite(pMin))
        pMin = minAxis;

    if (!qIsFinite(pMax))
        pMax = MaxAxis;

    // Make sure that our axis' values are valid

    double range = pMax-pMin;

    if (range > (pLogAxis?MaxLogAxisRange:MaxAxisRange)) {
        // The range is too big, so reset our values

        pMin = minAxis;
        pMax = MaxAxis;
    } else if (range < MinAxisRange) {
        // The range is too small, so reset our values

        pMin = qMax(minAxis, 0.5*(pMin+pMax-MinAxisRange));
        pMax = qMin(MaxAxis, pMin+MinAxisRange);
        pMin = pMax-MinAxisRange;
        // Note: the last statement is in case pMax was set to MaxAxis, in which
        //       case pMin has to be re-reset...
    } else if (pMin < minAxis) {
        // The minimum value is too small, so reset it

        pMin = minAxis;
        pMax = pMin+range;
    } else if (pMax > MaxAxis) {
        // The maximum value is too big, so reset it

        pMax = MaxAxis;
        pMin = pMax-range;
    }
}

//==============================================================================

GraphPanelPlotWidget::Action GraphPanelPlotWidget::action() const
{
    // Return our action

    return mAction;
}

//==============================================================================

void GraphPanelPlotWidget::resetAction()
{
    // Reset our action and our overlay widget, by updating it

    mAction = None;

    mOverlayWidget->update();
}

//==============================================================================

QColor GraphPanelPlotWidget::color() const
{
    // Return our colour

    return mColor;
}

//==============================================================================

void GraphPanelPlotWidget::setColor(const QColor &pColor)
{
    // Set our colour
    // Note: setCanvasBackground() doesn't handle semi-transparent colours and,
    //       even if it did, it might take into account the grey background that
    //       is used by the rest of our object while we want a semi-transparent
    //       colour to rely on a white background, which we do here by computing
    //       an opaque colour from an opaque white colour and the given
    //       (potentially semi-transparent) colour...

    if (pColor != mColor) {
        mColor = pColor;

        // Set our canvas background colour

        static const QColor White = Qt::white;

        QBrush brush = canvasBackground();
        double ratio = pColor.alpha()/256.0;
        QColor color = QColor((1.0-ratio)*White.red()+ratio*pColor.red(),
                              (1.0-ratio)*White.green()+ratio*pColor.green(),
                              (1.0-ratio)*White.blue()+ratio*pColor.blue());

        brush.setColor(color);

        setCanvasBackground(brush);

        // Set the colour of the background surrounding our canvas, which we
        // make slightly darker than that of our canvas background colour, based
        // on the typical colour used for a widget's background

        QColor winColor = Core::windowColor();
        QPalette pal = palette();

        pal.setColor(QPalette::Window, QColor(winColor.redF()*color.red(),
                                              winColor.greenF()*color.green(),
                                              winColor.blueF()*color.blue()));

        setPalette(pal);

        replot();
    }
}

//==============================================================================

int GraphPanelPlotWidget::fontSize() const
{
    // Return our font size

    return font().pointSize();
}

//==============================================================================

void GraphPanelPlotWidget::setFontSize(const int &pFontSize,
                                       const bool &pForceSetting)
{
    // Set our font size

    if (pForceSetting || (pFontSize != fontSize())) {
        // Main font

        QFont newFont = font();

        newFont.setPointSize(pFontSize);

        setFont(newFont);

        // Title's font

        newFont = titleLabel()->font();

        newFont.setPointSizeF(2.0*pFontSize);

        titleLabel()->setFont(newFont);

        // X axis' font

        newFont = axisFont(QwtPlot::xBottom);

        newFont.setPointSize(pFontSize);

        setAxisFont(QwtPlot::xBottom, newFont);
        setTitleAxisX(titleAxisX());

        // Y axis' font

        newFont = axisFont(QwtPlot::yLeft);

        newFont.setPointSize(pFontSize);

        setAxisFont(QwtPlot::yLeft, newFont);
        setTitleAxisY(titleAxisY());

        forceAlignWithNeighbors();
    }
}

//==============================================================================

bool GraphPanelPlotWidget::logAxisX() const
{
    // Return whether our X axis uses a logarithmic scale

    return mLogAxisX;
}

//==============================================================================

void GraphPanelPlotWidget::setLogAxisX(const bool &pLogAxisX)
{
    // Specify whether our X axis should use a logarithmic scale

    if (pLogAxisX != mLogAxisX) {
        mLogAxisX = pLogAxisX;

        setAxisScaleEngine(QwtPlot::xBottom,
                           pLogAxisX?
                               static_cast<QwtScaleEngine *>(new QwtLogScaleEngine()):
                               static_cast<QwtScaleEngine *>(new QwtLinearScaleEngine()));

        resetAxes();

        replot();
    }
}

//==============================================================================

QString GraphPanelPlotWidget::titleAxisX() const
{
    // Return the title for our X axis

    return axisTitle(QwtPlot::xBottom).text();
}

//==============================================================================

void GraphPanelPlotWidget::setTitleAxisX(const QString &pTitleAxisX)
{
    // Set the title for our X axis

    QwtText axisTitle = QwtText(pTitleAxisX);

    QFont axisTitleFont = axisTitle.font();

    axisTitleFont.setPointSizeF(1.25*fontSize());

    axisTitle.setFont(axisTitleFont);

    setAxisTitle(QwtPlot::xBottom, axisTitle);
}

//==============================================================================

bool GraphPanelPlotWidget::logAxisY() const
{
    // Return whether our Y axis uses a logarithmic scale

    return mLogAxisY;
}

//==============================================================================

void GraphPanelPlotWidget::setLogAxisY(const bool &pLogAxisY)
{
    // Specify whether our Y axis should use a logarithmic scale

    if (pLogAxisY != mLogAxisY) {
        mLogAxisY = pLogAxisY;

        setAxisScaleEngine(QwtPlot::yLeft,
                           pLogAxisY?
                               static_cast<QwtScaleEngine *>(new QwtLogScaleEngine()):
                               static_cast<QwtScaleEngine *>(new QwtLinearScaleEngine()));

        resetAxes();

        replot();
    }
}

//==============================================================================

QString GraphPanelPlotWidget::titleAxisY() const
{
    // Return the title for our Y axis

    return axisTitle(QwtPlot::yLeft).text();
}

//==============================================================================

void GraphPanelPlotWidget::setTitleAxisY(const QString &pTitleAxisY)
{
    // Set the title for our Y axis

    QwtText axisTitle = QwtText(pTitleAxisY);

    QFont axisTitleFont = axisTitle.font();

    axisTitleFont.setPointSizeF(1.25*fontSize());

    axisTitle.setFont(axisTitleFont);

    setAxisTitle(QwtPlot::yLeft, axisTitle);
}

//==============================================================================

double GraphPanelPlotWidget::minX() const
{
    // Return our minimum X value

    return axisScaleDiv(QwtPlot::xBottom).lowerBound();
}

//==============================================================================

double GraphPanelPlotWidget::maxX() const
{
    // Return our maximum X value

    return axisScaleDiv(QwtPlot::xBottom).upperBound();
}

//==============================================================================

double GraphPanelPlotWidget::minY() const
{
    // Return our minimum Y value

    return axisScaleDiv(QwtPlot::yLeft).lowerBound();
}

//==============================================================================

double GraphPanelPlotWidget::maxY() const
{
    // Return our maximum Y value

    return axisScaleDiv(QwtPlot::yLeft).upperBound();

}

//==============================================================================

bool GraphPanelPlotWidget::canZoomInX() const
{
    // Return whether we can zoom in on the X axis

    return mCanZoomInX;
}

//==============================================================================

bool GraphPanelPlotWidget::canZoomOutX() const
{
    // Return whether we can zoom out on the X axis

    return mCanZoomOutX;
}

//==============================================================================

bool GraphPanelPlotWidget::canZoomInY() const
{
    // Return whether we can zoom in on the Y axis

    return mCanZoomInY;
}

//==============================================================================

bool GraphPanelPlotWidget::canZoomOutY() const
{
    // Return whether we can zoom out on the Y axis

    return mCanZoomOutY;
}

//==============================================================================

GraphPanelPlotGraphs GraphPanelPlotWidget::graphs() const
{
    // Return all our graphs

    return mGraphs;
}

//==============================================================================

void GraphPanelPlotWidget::optimiseAxis(double &pMin, double &pMax) const
{
    // Make sure that the given values are different

    if (pMin == pMax) {
        // The given values are the same, so update them so that we can properly
        // optimise them below

        double powerValue = pMin?std::floor(log10(qAbs(pMin)))-1.0:0.0;

        pMin = pMin-pow(10.0, powerValue);
        pMax = pMax+pow(10.0, powerValue);
    }
}

//==============================================================================

bool GraphPanelPlotWidget::hasData() const
{
    // Determine and return whether at least one of the graphs, which are valid
    // and selected, has some data

    foreach (GraphPanelPlotGraph *graph, mGraphs) {
        if (graph->isValid() && graph->isSelected() && graph->dataSize())
            return true;
    }

    return false;
}

//==============================================================================

bool GraphPanelPlotWidget::dataRect(QRectF &pDataRect) const
{
    // Determine and return the rectangle within which all the graphs, which are
    // valid, selected and have some data, can fit

    bool res = false;

    pDataRect = QRectF();

    foreach (GraphPanelPlotGraph *graph, mGraphs) {
        if (graph->isValid() && graph->isSelected() && graph->dataSize()) {
            pDataRect |= graph->boundingRect();

            res = true;
        }
    }

    return res;
}

//==============================================================================

bool GraphPanelPlotWidget::dataLogRect(QRectF &pDataLogRect) const
{
    // Determine and return the log rectangle within which all the graphs, which
    // are valid, selected and have some data, can fit

    bool res = false;

    pDataLogRect = QRectF();

    foreach (GraphPanelPlotGraph *graph, mGraphs) {
        if (graph->isValid() && graph->isSelected() && graph->dataSize()) {
            QRectF boundingLogRect = graph->boundingLogRect();

            if (boundingLogRect != NoBoundingLogRect) {
                pDataLogRect |= boundingLogRect;

                res = true;
            }
        }
    }

    return res;
}

//==============================================================================

QRectF GraphPanelPlotWidget::realDataRect() const
{
    // Return an optimised version of dataRect() or a default rectangle, if no
    // dataRect() exists

    QRectF dRect = QRectF();
    QRectF dLogRect = QRectF();

    if (dataRect(dRect) && dataLogRect(dLogRect)) {
        // Optimise our axes' values

        double minX = logAxisX()?dLogRect.left():dRect.left();
        double maxX = minX+(logAxisX()?dLogRect.width():dRect.width());
        double minY = logAxisY()?dLogRect.top():dRect.top();
        double maxY = minY+(logAxisY()?dLogRect.height():dRect.height());

        optimiseAxis(minX, maxX);
        optimiseAxis(minY, maxY);

        return QRectF(minX, minY, maxX-minX, maxY-minY);
    } else {
        double minX = logAxisX()?mDefaultMinLogX:mDefaultMinX;
        double maxX = logAxisX()?mDefaultMaxLogX:mDefaultMaxX;
        double minY = logAxisY()?mDefaultMinLogY:mDefaultMinY;
        double maxY = logAxisY()?mDefaultMaxLogY:mDefaultMaxY;

        return QRectF(minX, minY, maxX-minX, maxY-minY);
    }
}

//==============================================================================

void GraphPanelPlotWidget::setAxis(const int &pAxisId, double pMin, double pMax)
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

    setAxisScaleDiv(pAxisId, QwtScaleDiv(pMin, pMax));
    setAxisScale(pAxisId, pMin, pMax);
}

//==============================================================================

void GraphPanelPlotWidget::setDefaultAxesValues(const double &pDefaultMinX,
                                                const double &pDefaultMaxX,
                                                const double &pDefaultMinLogX,
                                                const double &pDefaultMaxLogX,
                                                const double &pDefaultMinY,
                                                const double &pDefaultMaxY,
                                                const double &pDefaultMinLogY,
                                                const double &pDefaultMaxLogY)
{
    // Set the default axes values

    mDefaultMinX = pDefaultMinX;
    mDefaultMaxX = pDefaultMaxX;
    mDefaultMinLogX = pDefaultMinLogX;
    mDefaultMaxLogX = pDefaultMaxLogX;

    mDefaultMinY = pDefaultMinY;
    mDefaultMaxY = pDefaultMaxY;
    mDefaultMinLogY = pDefaultMinLogY;
    mDefaultMaxLogY = pDefaultMaxLogY;
}

//==============================================================================

bool GraphPanelPlotWidget::setAxes(double pMinX, double pMaxX, double pMinY,
                                   double pMaxY, const bool &pSynchronizeAxes,
                                   const bool &pCanReplot,
                                   const bool &pEmitSignal,
                                   const bool &pForceXAxisSetting,
                                   const bool &pForceYAxisSetting)
{
    // Keep track of our axes' old values

    double oldMinX = minX();
    double oldMaxX = maxX();
    double oldMinY = minY();
    double oldMaxY = maxY();

    // Make sure that the given axes' values are fine

    checkAxisValues(logAxisX(), pMinX, pMaxX);
    checkAxisValues(logAxisY(), pMinY, pMaxY);

    // Update our axes' values, if needed

    bool xAxisValuesChanged = false;
    bool yAxisValuesChanged = false;

    if (pForceXAxisSetting || (pMinX != oldMinX) || (pMaxX != oldMaxX)) {
        setAxis(QwtPlot::xBottom, pMinX, pMaxX);

        xAxisValuesChanged = true;
    }

    if (pForceYAxisSetting || (pMinY != oldMinY) || (pMaxY != oldMaxY)) {
        setAxis(QwtPlot::yLeft, pMinY, pMaxY);

        yAxisValuesChanged = true;
    }

    // Update our actions and align ourselves with our neighbours (which will
    // result in ourselves, and maybe its neighbours, bein replotted, if
    // allowed), in case the axes' values have changed

    if (xAxisValuesChanged || yAxisValuesChanged) {
        mCanDirectPaint = false;

        if (xAxisValuesChanged || yAxisValuesChanged)
            updateActions();

        if (pSynchronizeAxes) {
            if (   mSynchronizeXAxisAction->isChecked()
                && mSynchronizeYAxisAction->isChecked()) {
                foreach (GraphPanelPlotWidget *neighbor, mNeighbors)
                    neighbor->setAxes(pMinX, pMaxX, pMinY, pMaxY, false, false, false);
            } else if (xAxisValuesChanged && mSynchronizeXAxisAction->isChecked()) {
                foreach (GraphPanelPlotWidget *neighbor, mNeighbors)
                    neighbor->setAxes(pMinX, pMaxX, neighbor->minY(), neighbor->maxY(), false, false, false);
            } else if (yAxisValuesChanged && mSynchronizeYAxisAction->isChecked()) {
                foreach (GraphPanelPlotWidget *neighbor, mNeighbors)
                    neighbor->setAxes(neighbor->minX(), neighbor->maxX(), pMinY, pMaxY, false, false, false);
            }

            alignWithNeighbors(pCanReplot,
                                  mSynchronizeXAxisAction->isChecked()
                               || mSynchronizeYAxisAction->isChecked());
        }

        if (pEmitSignal)
            emit axesChanged(pMinX, pMaxX, pMinY, pMaxY);

        return pCanReplot;
    } else {
        return false;
    }
}

//==============================================================================

bool GraphPanelPlotWidget::resetAxes()
{
    // Reset our axes by setting their values to either default ones or to some
    // that allow us to see all the graphs

    QRectF dRect = realDataRect();

    return setAxes(dRect.left(), dRect.left()+dRect.width(),
                   dRect.top(), dRect.top()+dRect.height());
}

//==============================================================================

bool GraphPanelPlotWidget::scaleAxis(const Scaling &pScaling,
                                     const bool &pCanZoomIn,
                                     const bool &pCanZoomOut,
                                     const QwtScaleMap &pCanvasMap,
                                     const double &pPoint, double &pMin,
                                     double &pMax)
{
    // Check whether we can scale the axis and, if so, determine what its new
    // values should be

    if (   ((pScaling < NoScaling) && pCanZoomIn)
        || ((pScaling > NoScaling) && pCanZoomOut)) {
        static const double ScalingInFactor     = 0.9;
        static const double ScalingOutFactor    = 1.0/ScalingInFactor;
        static const double BigScalingInFactor  = 0.5*ScalingInFactor;
        static const double BigScalingOutFactor = 1.0/BigScalingInFactor;

        double min = pCanvasMap.transform(pMin);
        double range = pCanvasMap.transform(pMax)-min;
        double factor = qMin(1.0, qMax(0.0, (pPoint-min)/range));
        // Note: QwtPlot puts some extra space around the area we want to show,
        //       which means that we could end up with a factor which is either
        //       smaller than zero or bigger than one, hence we have to make
        //       sure that it is clamped within the [0; 1] range...

        switch (pScaling) {
        case BigScalingIn:
            range *= BigScalingInFactor;

            break;
        case ScalingIn:
            range *= ScalingInFactor;

            break;
        case NoScaling:
            break;
        case ScalingOut:
            range *= ScalingOutFactor;

            break;
        case BigScalingOut:
            range *= BigScalingOutFactor;

            break;
        }

        pMin = qMax(MinAxis, pCanvasMap.invTransform(pPoint-factor*range));
        pMax = qMin(MaxAxis, pCanvasMap.invTransform(pCanvasMap.transform(pMin)+range));
        pMin = pCanvasMap.invTransform(pCanvasMap.transform(pMax)-range);
        // Note: the last statement is in case pMax has been set to MaxAxis, in
        //       which case we need to re-update pMin...

        return true;
    } else {
        return false;
    }
}

//==============================================================================

void GraphPanelPlotWidget::scaleAxes(const QPoint &pPoint,
                                     const Scaling &pScalingX,
                                     const Scaling &pScalingY)
{
    // Rescale our X axis, but only if zooming in/out is possible on that axis

    QPointF point = pPoint-plotLayout()->canvasRect().topLeft();

    double newMinX = minX();
    double newMaxX = maxX();
    double newMinY = minY();
    double newMaxY = maxY();

    bool scaledAxisX = scaleAxis(pScalingX, mCanZoomInX, mCanZoomOutX,
                                 canvasMap(QwtPlot::xBottom), point.x(),
                                 newMinX, newMaxX);
    bool scaledAxisY = scaleAxis(pScalingY, mCanZoomInY, mCanZoomOutY,
                                 canvasMap(QwtPlot::yLeft), point.y(),
                                 newMinY, newMaxY);
    // Note: we want to make both calls to scaleAxis(), hence they are not part
    //       of the if() statement below...

    if (scaledAxisX || scaledAxisY)
        setAxes(newMinX, newMaxX, newMinY, newMaxY);
}

//==============================================================================

QPointF GraphPanelPlotWidget::canvasPoint(const QPoint &pPoint) const
{
    // Return the mouse position using canvas coordinates, making sure that they
    // are within our ranges

    return QPointF(qMin(maxX(), qMax(minX(), canvasMap(QwtPlot::xBottom).invTransform(pPoint.x()))),
                   qMin(maxY(), qMax(minY(), canvasMap(QwtPlot::yLeft).invTransform(pPoint.y()))));
}

//==============================================================================

void GraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Carry out the action

    switch (mAction) {
    case None:
        // No action, so do nothing

        break;
    case Pan: {
        // Determine the X/Y shifts for our panning

        int shiftX = pEvent->pos().x()-mPoint.x();
        int shiftY = pEvent->pos().y()-mPoint.y();

        mPoint = pEvent->pos();

        // Set our axes' new values

        QwtScaleMap canvasMapX = canvasMap(QwtPlot::xBottom);
        QwtScaleMap canvasMapY = canvasMap(QwtPlot::yLeft);

        setAxes(canvasMapX.invTransform(canvasMapX.transform(minX())-shiftX),
                canvasMapX.invTransform(canvasMapX.transform(maxX())-shiftX),
                canvasMapY.invTransform(canvasMapY.transform(minY())-shiftY),
                canvasMapY.invTransform(canvasMapY.transform(maxY())-shiftY));

        break;
    }
    case ShowCoordinates:
        // Update the point of our overlay widget

        mOverlayWidget->setPoint(pEvent->pos());

        break;
    case Zoom: {
        // Determine our X/Y delta values

        int deltaX = pEvent->pos().x()-mPoint.x();
        int deltaY = pEvent->pos().y()-mPoint.y();

        mPoint = pEvent->pos();

        // Rescale ourselves
        // Note: this will automatically replot ourselves...

        scaleAxes(mOriginPoint,
                  deltaX?(deltaX > 0)?ScalingIn:ScalingOut:NoScaling,
                  deltaY?(deltaY < 0)?ScalingIn:ScalingOut:NoScaling);

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

void GraphPanelPlotWidget::mousePressEvent(QMouseEvent *pEvent)
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

    bool noModifiers =    !(pEvent->modifiers() & Qt::ShiftModifier)
                       && !(pEvent->modifiers() & Qt::ControlModifier)
                       && !(pEvent->modifiers() & Qt::AltModifier)
                       && !(pEvent->modifiers() & Qt::MetaModifier);

    if (noModifiers && (pEvent->button() == Qt::LeftButton)) {
        // We want to pan, but only do this if we are not completely zoomed out

        if (mCanZoomOutX || mCanZoomOutY) {
            mAction = Pan;

            mPoint = pEvent->pos();
        }
    } else if (    (pEvent->modifiers() & Qt::ShiftModifier)
               && !(pEvent->modifiers() & Qt::ControlModifier)
               && !(pEvent->modifiers() & Qt::AltModifier)
               && !(pEvent->modifiers() & Qt::MetaModifier)
               && (pEvent->button() == Qt::LeftButton)) {
        // We want to show the coordinates

        mAction = ShowCoordinates;

        mOverlayWidget->setPoint(pEvent->pos());
    } else if (noModifiers && (pEvent->button() == Qt::RightButton)) {
        // We want to zoom in/out

        mAction = Zoom;

        mOriginPoint = pEvent->pos();
        mPoint = pEvent->pos();
    } else if (   !(pEvent->modifiers() & Qt::ShiftModifier)
               &&  (pEvent->modifiers() & Qt::ControlModifier)
               && !(pEvent->modifiers() & Qt::AltModifier)
               && !(pEvent->modifiers() & Qt::MetaModifier)
               && (pEvent->button() == Qt::RightButton)) {
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

void GraphPanelPlotWidget::mouseReleaseEvent(QMouseEvent *pEvent)
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

        QRectF zoomRegion = QRectF(canvasPoint(zoomRegionRect.topLeft()),
                                   canvasPoint(zoomRegionRect.topLeft()+QPoint(zoomRegionRect.width(), zoomRegionRect.height())));

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

void GraphPanelPlotWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::paintEvent(pEvent);

    // We have just (re)painted ourselves, which means that we can (re)allow
    // direct painting and replotting

    mCanDirectPaint = true;
    mCanReplot = true;
}

//==============================================================================

void GraphPanelPlotWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::resizeEvent(pEvent);

    // Update the size of our overlay widget

    mOverlayWidget->resize(pEvent->size());
}

//==============================================================================

void GraphPanelPlotWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Handle the wheel mouse button for zooming in/out

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        && !(pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)) {
        // Make sure that we are not already carrying out a action

        if (mAction == None) {
            int delta = pEvent->delta();
            Scaling scaling = NoScaling;

            if (delta > 0)
                scaling = ScalingIn;
            else if (delta < 0)
                scaling = ScalingOut;

            if (scaling)
                scaleAxes(pEvent->pos(), scaling, scaling);
        }

        pEvent->accept();
    } else {
        // Not the modifier we were expecting, so call the default handling of
        // the event

        QwtPlot::wheelEvent(pEvent);
    }
}

//==============================================================================

bool GraphPanelPlotWidget::addGraph(GraphPanelPlotGraph *pGraph)
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

bool GraphPanelPlotWidget::removeGraph(GraphPanelPlotGraph *pGraph)
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

bool GraphPanelPlotWidget::drawGraphFrom(GraphPanelPlotGraph *pGraph,
                                         const qulonglong &pFrom)
{
    // Direct paint our graph from the given point unless we can't direct paint
    // (due to the axes having been changed), in which case we replot ourselves

    if (mCanDirectPaint) {
        mDirectPainter->drawSeries(pGraph, pFrom, -1);

        return false;
    } else {
        if (mCanReplot) {
            replot();

            mCanReplot = false;
        }

        return true;
    }
}

//==============================================================================

GraphPanelPlotWidgets GraphPanelPlotWidget::neighbors() const
{
    // Return our neighbours

    return mNeighbors;
}

//==============================================================================

void GraphPanelPlotWidget::addNeighbor(GraphPanelPlotWidget *pPlot)
{
    // Add the plot as a neighbour and make sure our actions are up to date

    if ((pPlot != this) && !mNeighbors.contains(pPlot)) {
        mNeighbors << pPlot;

        updateActions();
    }
}

//==============================================================================

void GraphPanelPlotWidget::removeNeighbor(GraphPanelPlotWidget *pPlot)
{
    // Remove the plot from our neighbours and make sure our actions are up to
    // date

    mNeighbors.removeOne(pPlot);

    updateActions();
}

//==============================================================================

void GraphPanelPlotWidget::alignWithNeighbors(const bool &pCanReplot,
                                              const bool &pForceAlignment)
{
    // Align ourselves with our neighbours

    GraphPanelPlotWidgets selfPlusNeighbors = GraphPanelPlotWidgets() << this << mNeighbors;
    double oldMaxExtent = axisWidget(QwtPlot::yLeft)->scaleDraw()->minimumExtent();
    double newMaxExtent = 0;

    foreach (GraphPanelPlotWidget *plot, selfPlusNeighbors) {
        QwtScaleWidget *scaleWidget = plot->axisWidget(QwtPlot::yLeft);
        QwtScaleDraw *scaleDraw = scaleWidget->scaleDraw();

        scaleDraw->setMinimumExtent(0.0);

        plot->updateAxes();
        // Note: this ensures that our major ticks (which are used to compute
        //       the extent) are up to date...

        double extent = scaleDraw->extent(scaleWidget->font());

        if (extent > newMaxExtent)
            newMaxExtent = extent;
    }

    foreach (GraphPanelPlotWidget *plot, selfPlusNeighbors) {
        plot->axisWidget(QwtPlot::yLeft)->scaleDraw()->setMinimumExtent(newMaxExtent);

        if (pCanReplot) {
            if (pForceAlignment || (newMaxExtent != oldMaxExtent)) {
                plot->updateLayout();
                plot->replot();
            } else if (plot == this) {
                replot();
            }
        }
    }
}

//==============================================================================

void GraphPanelPlotWidget::forceAlignWithNeighbors()
{
    // Force the re-alignment with our neighbours

    alignWithNeighbors(true, true);
}

//==============================================================================

void GraphPanelPlotWidget::cannotUpdateActions()
{
    // Keep track of the fact that we cannot update our actions anymore

    mCanUpdateActions = false;
}

//==============================================================================

void GraphPanelPlotWidget::exportTo()
{
    // Export our contents to a PDF, SVG, etc. file
    // Note: if no file extension is given, then we export our contents to a PDF
    //       file...

    QString pdfFilter = tr("PDF File - Portable Document Format (*.pdf)");
    QStringList filters = QStringList() << pdfFilter
                                        << tr("SVG File - Scalable Vector Graphics (*.svg)");

    foreach (const QString &supportedMimeType, QImageWriter::supportedMimeTypes()) {
        if (!supportedMimeType.compare("image/bmp"))
            filters << tr("BMP File - Windows Bitmap (*.bmp)");
        else if (!supportedMimeType.compare("image/jpeg"))
            filters << tr("JPEG File - Joint Photographic Experts Group (*.jpg)");
        else if (!supportedMimeType.compare("image/png"))
            filters << tr("PNG File - Portable Network Graphics (*.png)");
        else if (!supportedMimeType.compare("image/x-portable-bitmap"))
            filters << tr("PBM File - Portable Bitmap (*.pbm)");
        else if (!supportedMimeType.compare("image/x-portable-graymap"))
            filters << tr("PGM File - Portable Graymap (*.pgm)");
        else if (!supportedMimeType.compare("image/x-portable-pixmap"))
            filters << tr("PPM File - Portable Pixmap (*.ppm)");
        else if (!supportedMimeType.compare("image/x-xbitmap"))
            filters << tr("XBM File - X11 Bitmap (*.xbm)");
        else if (!supportedMimeType.compare("image/x-xpixmap"))
            filters << tr("XPM File - X11 Pixmap (*.xpm)");
    }

    std::sort(filters.begin(), filters.end());

    QString fileName = Core::getSaveFileName(tr("Export To"), filters, &pdfFilter);

    if (!fileName.isEmpty()) {
        static double InToMm = 25.4;
        static double Dpi = 85.0;

        if (QFileInfo(fileName).completeSuffix().isEmpty())
            QwtPlotRenderer().renderDocument(this, fileName, "pdf", QSizeF(width()*InToMm/Dpi, height()*InToMm/Dpi), Dpi);
        else
            QwtPlotRenderer().renderDocument(this, fileName, QSizeF(width()*InToMm/Dpi, height()*InToMm/Dpi), Dpi);
    }

    // QwtPlotRenderer::renderDocument() changes and then invalidates our
    // layout, so we need to update it
    // Note: indeed, the plot layout's canvas rectangle is, among other things,
    //       used to determine whether we can interact with our plot...

    updateLayout();
}

//==============================================================================

void GraphPanelPlotWidget::copyToClipboard()
{
    // Copy our contents to the clipboard

    QApplication::clipboard()->setPixmap(grab());
}

//==============================================================================

void GraphPanelPlotWidget::customAxes()
{
    // Specify custom axes for the graph panel

    double oldMinX = minX();
    double oldMaxX = maxX();
    double oldMinY = minY();
    double oldMaxY = maxY();

    GraphPanelWidgetCustomAxesDialog customAxesDialog(oldMinX, oldMaxX, oldMinY, oldMaxY, this);

    customAxesDialog.exec();

    // Update our axes' values, if requested and only if they have actually
    // changed

    if (customAxesDialog.result() == QMessageBox::Accepted) {
        double newMinX = customAxesDialog.minX();
        double newMaxX = customAxesDialog.maxX();
        double newMinY = customAxesDialog.minY();
        double newMaxY = customAxesDialog.maxY();

        if (   (newMinX != oldMinX) || (newMaxX != oldMaxX)
            || (newMinY != oldMinY) || (newMaxY != oldMaxY)) {
            setAxes(newMinX, newMaxX, newMinY, newMaxY);
        }
    }
}

//==============================================================================

void GraphPanelPlotWidget::zoomIn()
{
    // Zoom in by scaling our two axes around the point where the context menu
    // was shown

    scaleAxes(mOriginPoint, BigScalingIn, BigScalingIn);
}

//==============================================================================

void GraphPanelPlotWidget::zoomOut()
{
    // Zoom out by scaling our two axes around the point where the context menu
    // was shown

    scaleAxes(mOriginPoint, BigScalingOut, BigScalingOut);
}

//==============================================================================

void GraphPanelPlotWidget::resetZoom()
{
    // Reset the zoom level by resetting our axes

    resetAxes();
}

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

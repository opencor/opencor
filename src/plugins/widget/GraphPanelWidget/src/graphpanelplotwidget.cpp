/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Graph panel plot widget
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "graphpanelplotwidget.h"
#include "graphpanelwidget.h"
#include "graphpanelwidgetcustomaxesdialog.h"
#include "i18ninterface.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QGestureEvent>
#include <QImageWriter>
#include <QMenu>
#include <QPaintEvent>
#include <QScreen>
#include <QTimer>

//==============================================================================

#include <cfloat>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_clipper.h"
    #include "qwt_curve_fitter.h"
    #include "qwt_dyngrid_layout.h"
    #include "qwt_legend_label.h"
    #include "qwt_painter.h"
    #include "qwt_plot_canvas.h"
    #include "qwt_plot_directpainter.h"
    #include "qwt_plot_grid.h"
    #include "qwt_plot_layout.h"
    #include "qwt_plot_renderer.h"
    #include "qwt_point_mapper.h"
    #include "qwt_scale_engine.h"
    #include "qwt_text_label.h"
#include "qwtend.h"

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

GraphPanelPlotGraphProperties::GraphPanelPlotGraphProperties(bool pSelected,
                                                             const QString &pTitle,
                                                             const Qt::PenStyle &pLineStyle,
                                                             int pLineWidth,
                                                             const QColor &pLineColor,
                                                             const QwtSymbol::Style &pSymbolStyle,
                                                             int pSymbolSize,
                                                             const QColor &pSymbolColor,
                                                             bool pSymbolFilled,
                                                             const QColor &pSymbolFillColor) :
    mSelected(pSelected),
    mTitle(pTitle),
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

bool GraphPanelPlotGraphProperties::isSelected() const
{
    // Return our selected state

    return mSelected;
}

//==============================================================================

QString GraphPanelPlotGraphProperties::title() const
{
    // Return our title

    return mTitle;
}

//==============================================================================

Qt::PenStyle GraphPanelPlotGraphProperties::lineStyle() const
{
    // Return our line style

    return mLineStyle;
}

//==============================================================================

int GraphPanelPlotGraphProperties::lineWidth() const
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

GraphPanelPlotGraphRun::GraphPanelPlotGraphRun(GraphPanelPlotGraph *pOwner) :
    mOwner(pOwner)
{
    // Customise ourselves a bit

    setLegendAttribute(LegendShowLine);
    setLegendAttribute(LegendShowSymbol);
    setPen(QPen(pOwner->color(), DefaultGraphLineWidth, DefaultGraphLineStyle, Qt::RoundCap, Qt::RoundJoin));
    setRenderHint(RenderAntialiased);
}

//==============================================================================

GraphPanelPlotGraph * GraphPanelPlotGraphRun::owner() const
{
    // Return our owner

    return mOwner;
}

//==============================================================================

void GraphPanelPlotGraphRun::setRawSamples(const double *pDataX,
                                           const double *pDataY,
                                           int pSize)
{
    // Set the given raw samples and keep track of those that are valid

    static const QPair<int, int> EmptyData = QPair<int, int>(-1, -1);

    QPair<int, int> validData = EmptyData;

    if (!mValidData.isEmpty()) {
        validData = mValidData.last();

        mValidData.removeLast();
    }

    for (int i = mSize; i < pSize; ++i) {
        if (   !qIsInf(pDataX[i]) && !qIsNaN(pDataX[i])
            && !qIsInf(pDataY[i]) && !qIsNaN(pDataY[i])) {
            if (validData == EmptyData) {
                validData.first = i;
                validData.second = i;
            } else if (validData.second == i-1) {
                validData.second = i;
            } else {
                mValidData << validData;

                validData.first = i;
                validData.second = i;
            }
        }
    }

    if (validData != EmptyData) {
        mValidData << validData;
    }

    mSize = pSize;

    QwtPlotCurve::setRawSamples(pDataX, pDataY, pSize);
}

//==============================================================================

void GraphPanelPlotGraphRun::drawLines(QPainter *pPainter,
                                       const QwtScaleMap &pMapX,
                                       const QwtScaleMap &pMapY,
                                       const QRectF &pCanvasRect,
                                       int pFrom, int pTo) const
{
    // Draw our lines

    for (const auto &validData : mValidData) {
        if ((pFrom <= validData.first) || (pTo >= validData.second)) {
            int from = (   (pFrom >= validData.first)
                        && (pFrom <= validData.second))?
                           pFrom:
                           validData.first;
            int to = (   (pTo >= validData.first)
                      && (pTo <= validData.second))?
                         pTo:
                         validData.second;

            QwtPlotCurve::drawLines(pPainter, pMapX, pMapY,
                                    pCanvasRect, from, to);
        }
    }
}

//==============================================================================

void GraphPanelPlotGraphRun::drawSymbols(QPainter *pPainter,
                                         const QwtSymbol &pSymbol,
                                         const QwtScaleMap &pMapX,
                                         const QwtScaleMap &pMapY,
                                         const QRectF &pCanvasRect,
                                         int pFrom, int pTo) const
{
    // Draw our symbols

    for (const auto &validData : mValidData) {
        if ((pFrom <= validData.first) || (pTo >= validData.second)) {
            int from = (   (pFrom >= validData.first)
                        && (pFrom <= validData.second))?
                           pFrom:
                           validData.first;
            int to = (   (pTo >= validData.first)
                      && (pTo <= validData.second))?
                         pTo:
                         validData.second;

            QwtPlotCurve::drawSymbols(pPainter, pSymbol, pMapX, pMapY,
                                      pCanvasRect, from, to);
        }
    }
}

//==============================================================================

static const QRectF InvalidRect = QRectF(0.0, 0.0, -1.0, -1.0);

//==============================================================================

GraphPanelPlotGraph::GraphPanelPlotGraph(void *pParameterX, void *pParameterY,
                                         GraphPanelWidget *pOwner) :
    mParameterX(pParameterX),
    mParameterY(pParameterY),
    mBoundingRect(InvalidRect),
    mBoundingLogRect(InvalidRect)
{
    // Determine our default colour

    static QList<QColor> GraphColors = { DarkBlue, Orange, Yellow, Purple, Green, LightBlue, Red };
    static QMap<GraphPanelWidget *, int> GraphColorIndexes;

    int graphColorIndex = pOwner->graphs().isEmpty()?
                              -1:
                              GraphColorIndexes.value(pOwner, -1);

    graphColorIndex = (graphColorIndex+1)%GraphColors.count();

    mColor = GraphColors[graphColorIndex];

    GraphColorIndexes.insert(pOwner, graphColorIndex);

    // Create our dummy run
    // Note: a dummy run (i.e. a run that is never used, shown, etc.) is needed
    //       to ensure that our legend labels don't disappear (see
    //       https://github.com/opencor/opencor/issues/1537)...

    addRun();
}

//==============================================================================

GraphPanelPlotGraph::GraphPanelPlotGraph(GraphPanelWidget *pOwner) :
    GraphPanelPlotGraph(nullptr, nullptr, pOwner)
{
}

//==============================================================================

bool GraphPanelPlotGraph::isValid() const
{
    // Return whether we are valid

    return   !mFileName.isEmpty()
           && (mParameterX != nullptr) && (mParameterY != nullptr);
}

//==============================================================================

void GraphPanelPlotGraph::attach(GraphPanelPlotWidget *pPlot)
{
    // Attach our different runs to the given plot and keep track of the given
    // plot

    Q_ASSERT(mDummyRun);

    mDummyRun->attach(pPlot);

    for (auto run : mRuns) {
        run->attach(pPlot);
    }

    mPlot = pPlot;
}

//==============================================================================

void GraphPanelPlotGraph::detach()
{
    // Detach our different runs from their plot and reset our plot

    Q_ASSERT(mDummyRun);

    mDummyRun->detach();

    for (auto run : mRuns) {
        run->detach();
    }

    mPlot = nullptr;
}

//==============================================================================

GraphPanelPlotWidget * GraphPanelPlotGraph::plot() const
{
    // Return our plot

    return mPlot;
}

//==============================================================================

int GraphPanelPlotGraph::runsCount() const
{
    // Return our number of runs

    return mRuns.count();
}

//==============================================================================

void GraphPanelPlotGraph::addRun()
{
    // Add a run, after having customised it using our dummy run, if available

    auto run = new GraphPanelPlotGraphRun(this);

    if (mDummyRun != nullptr) {
        const QwtSymbol *symbol = mDummyRun->symbol();
        int symbolSize = symbol->size().width();

        run->setPen(mDummyRun->pen());
        run->setSymbol(new QwtSymbol(symbol->style(), symbol->brush(),
                                     symbol->pen(), QSize(symbolSize, symbolSize)));
        run->setTitle(mDummyRun->title());
        run->setVisible(mDummyRun->isVisible());
    }

    run->attach(mPlot);

    // If our dummy run already exists, then track the run we just created
    // otherwise make it our dummy run

    if (mDummyRun != nullptr) {
        mRuns << run;
    } else {
        mDummyRun = run;
    }
}

//==============================================================================

void GraphPanelPlotGraph::removeRuns()
{
    // Delete all our runs

    for (auto run : mRuns) {
        delete run;
    }

    mRuns.clear();
}

//==============================================================================

GraphPanelPlotGraphRun * GraphPanelPlotGraph::lastRun() const
{
    // Return our last run, if any

    return mRuns.isEmpty()?nullptr:mRuns.last();
}

//==============================================================================

bool GraphPanelPlotGraph::isSelected() const
{
    // Return whether we are selected

    return mSelected;
}

//==============================================================================

void GraphPanelPlotGraph::setSelected(bool pSelected)
{
    // Set our selected state

    mSelected = pSelected;

    // Un/check our corresponding legend item

    static_cast<GraphPanelPlotLegendWidget *>(mPlot->legend())->setChecked(this, pSelected);
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

QColor GraphPanelPlotGraph::color() const
{
    // Return our colour

    return mColor;
}

//==============================================================================

const QPen & GraphPanelPlotGraph::pen() const
{
    // Return the pen of our dummy run

    Q_ASSERT(mDummyRun);

    return mDummyRun->pen();
}

//==============================================================================

void GraphPanelPlotGraph::setPen(const QPen &pPen)
{
    // Set the pen of our different runs

    Q_ASSERT(mDummyRun);

    mDummyRun->setPen(pPen);

    for (auto run : mRuns) {
        run->setPen(pPen);
    }
}

//==============================================================================

const QwtSymbol * GraphPanelPlotGraph::symbol() const
{
    // Return the symbol of our dummy run

    Q_ASSERT(mDummyRun);

    return mDummyRun->symbol();
}

//==============================================================================

void GraphPanelPlotGraph::setSymbol(const QwtSymbol::Style &pStyle,
                                    const QBrush &pBrush, const QPen &pPen,
                                    const QSize &pSize)
{
    // Set the symbol of our different runs

    Q_ASSERT(mDummyRun);

    mDummyRun->setSymbol(new QwtSymbol(pStyle, pBrush, pPen, pSize));

    for (auto run : mRuns) {
        run->setSymbol(new QwtSymbol(pStyle, pBrush, pPen, pSize));
    }
}

//==============================================================================

QString GraphPanelPlotGraph::title() const
{
    // Return our title

    Q_ASSERT(mDummyRun);

    return mDummyRun->title().text();
}

//==============================================================================

void GraphPanelPlotGraph::setTitle(const QString &pTitle)
{
    // Set the title of our different runs

    Q_ASSERT(mDummyRun);

    mDummyRun->setTitle(pTitle);

    for (auto run : mRuns) {
        run->setTitle(pTitle);
    }
}

//==============================================================================

bool GraphPanelPlotGraph::isVisible() const
{
    // Return whether our dummy run is visible

    Q_ASSERT(mDummyRun);

    return mDummyRun->isVisible();
}

//==============================================================================

void GraphPanelPlotGraph::setVisible(bool pVisible)
{
    // Set the visibility of our different runs

    Q_ASSERT(mDummyRun);

    mDummyRun->setVisible(pVisible);

    for (auto run : mRuns) {
        run->setVisible(pVisible);
    }
}

//==============================================================================

bool GraphPanelPlotGraph::hasData() const
{
    // Return whether we have some data for any of our runs

    for (auto run : mRuns) {
        if (run->dataSize() != 0) {
            return true;
        }
    }

    return false;
}

//==============================================================================

quint64 GraphPanelPlotGraph::dataSize() const
{
    // Return the size of our data, i.e. raw samples, for our current (i.e.
    // last) run, if any

    return mRuns.isEmpty()?0:mRuns.last()->dataSize();
}

//==============================================================================

QwtSeriesData<QPointF> * GraphPanelPlotGraph::data(int pRun) const
{
    // Return the data, i.e. raw samples, of the given run, if it exists

    if (mRuns.isEmpty()) {
        return nullptr;
    }

    if (pRun == -1) {
        return mRuns.last()->data();
    }

    return ((pRun >= 0) && (pRun < mRuns.count()))?mRuns[pRun]->data():nullptr;
}

//==============================================================================

void GraphPanelPlotGraph::setData(double *pDataX, double *pDataY, quint64 pSize,
                                  int pRun)
{
    // Set our data, i.e. raw samples, to the given run, if it exists

    GraphPanelPlotGraphRun *run = nullptr;

    if (!mRuns.isEmpty()) {
        if (pRun == -1) {
            run = mRuns.last();
        } else {
            run = ((pRun >= 0) && (pRun < mRuns.count()))?mRuns[pRun]:nullptr;
        }
    }

    if (run == nullptr) {
        return;
    }

    run->setRawSamples(pDataX, pDataY, int(pSize));

    // Reset the cached version of our bounding rectangles

    mBoundingRect = InvalidRect;
    mBoundingLogRect = InvalidRect;

    mBoundingRects.remove(run);
    mBoundingLogRects.remove(run);
}

//==============================================================================

QRectF GraphPanelPlotGraph::boundingRect()
{
    // Return the cached version of our bounding rectangle, if we have one, or
    // compute it and return it

    if ((mBoundingRect == InvalidRect) && !mRuns.isEmpty()) {
        mBoundingRect = QRectF();

        for (auto run : mRuns) {
            if (run->dataSize() != 0) {
                QRectF boundingRect = mBoundingRects.value(run, InvalidRect);

                if (boundingRect == InvalidRect) {
                    bool needInitMinX = true;
                    bool needInitMaxX = true;
                    bool needInitMinY = true;
                    bool needInitMaxY = true;
                    double minX = 1.0;
                    double maxX = 1.0;
                    double minY = 1.0;
                    double maxY = 1.0;

                    for (size_t i = 0, iMax = run->dataSize(); i < iMax; ++i) {
                        QPointF sample = run->data()->sample(i);

                        if (   !qIsInf(sample.x()) && !qIsNaN(sample.x())
                            && !qIsInf(sample.y()) && !qIsNaN(sample.y())) {
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

                    if (!needInitMinX && !needInitMaxX && !needInitMinY && !needInitMaxY) {
                        boundingRect = QRectF(minX, minY, maxX-minX, maxY-minY);

                        mBoundingRects.insert(run, boundingRect);
                    }
                }

                if (boundingRect != InvalidRect) {
                    mBoundingRect |= boundingRect;
                }
            }
        }
    }

    return mBoundingRect;
}

//==============================================================================

QRectF GraphPanelPlotGraph::boundingLogRect()
{
    // Return the cached version of our bounding log rectangle, if we have one,
    // or compute it and return it

    if ((mBoundingLogRect == InvalidRect) && !mRuns.isEmpty()) {
        mBoundingLogRect = QRectF();

        for (auto run : mRuns) {
            if (run->dataSize() != 0) {
                QRectF boundingLogRect = mBoundingLogRects.value(run, InvalidRect);

                if (boundingLogRect == InvalidRect) {
                    bool needInitMinX = true;
                    bool needInitMaxX = true;
                    bool needInitMinY = true;
                    bool needInitMaxY = true;
                    double minX = 1.0;
                    double maxX = 1.0;
                    double minY = 1.0;
                    double maxY = 1.0;

                    for (size_t i = 0, iMax = run->dataSize(); i < iMax; ++i) {
                        QPointF sample = run->data()->sample(i);

                        if (   !qIsInf(sample.x()) && !qIsNaN(sample.x())
                            &&  (sample.x() > 0.0)
                            && !qIsInf(sample.y()) && !qIsNaN(sample.y())
                            &&  (sample.y() > 0.0)) {
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

                    if (!needInitMinX && !needInitMaxX && !needInitMinY && !needInitMaxY) {
                        boundingLogRect = QRectF(minX, minY, maxX-minX, maxY-minY);

                        mBoundingLogRects.insert(run, boundingLogRect);
                    }
                }

                if (boundingLogRect != InvalidRect) {
                    mBoundingLogRect |= boundingLogRect;
                }
            }
        }
    }

    return mBoundingLogRect;
}

//==============================================================================

GraphPanelPlotOverlayWidget::GraphPanelPlotOverlayWidget(GraphPanelPlotWidget *pParent) :
    QWidget(pParent),
    mOwner(pParent)
{
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    setFocusPolicy(Qt::NoFocus);
}

//==============================================================================

QPoint GraphPanelPlotOverlayWidget::optimizedPoint(const QPoint &pPoint) const
{
    // Optimise the given point so that it fits within our owner's ranges

    QPoint realPoint = pPoint-mOwner->plotLayout()->canvasRect().topLeft().toPoint();

    QwtScaleMap canvasMapX = mOwner->canvasMap(QwtPlot::xBottom);
    QwtScaleMap canvasMapY = mOwner->canvasMap(QwtPlot::yLeft);

    return { qMin(qRound(canvasMapX.transform(mOwner->maxX())),
                  qMax(qRound(canvasMapX.transform(mOwner->minX())),
                       realPoint.x())),
             qMin(qRound(canvasMapY.transform(mOwner->minY())),
                  qMax(qRound(canvasMapY.transform(mOwner->maxY())),
                       realPoint.y())) };
}

//==============================================================================

void GraphPanelPlotOverlayWidget::paintEvent(QPaintEvent *pEvent)
{
    // Accept the event

    pEvent->accept();

    // Check whether an action is to be carried out

    if (mOwner->action() == GraphPanelPlotWidget::Action::None) {
        return;
    }

    // Paint the overlay, if any is needed

    QPainter painter(this);

    QRectF canvasRect = mOwner->plotLayout()->canvasRect();

    painter.translate(canvasRect.x(), canvasRect.y());

    if (mOwner->action() == GraphPanelPlotWidget::Action::ShowCoordinates) {
        // Draw the lines that show the coordinates

        QPen pen = painter.pen();

        pen.setColor(mOwner->pointCoordinatesColor());
        pen.setStyle(mOwner->pointCoordinatesStyle());
        pen.setWidth(mOwner->pointCoordinatesWidth());

        painter.setPen(pen);

        QPoint point = optimizedPoint(mPoint);
        // Note: see drawCoordinates() as why we use QPoint rather than
        //       QPointF...

        painter.drawLine(0, point.y(), int(canvasRect.width()), point.y());
        painter.drawLine(point.x(), 0, point.x(), int(canvasRect.height()));

        // Draw the coordinates of our point

        drawCoordinates(&painter, point, mOwner->pointCoordinatesColor(),
                        mOwner->pointCoordinatesFontColor(),
                        mOwner->pointCoordinatesFontSize(),
                        (mOwner->pointCoordinatesStyle() == Qt::NoPen)?
                            0:
                            mOwner->pointCoordinatesWidth(),
                        Position::BottomRight);
    } else if (mOwner->action() == GraphPanelPlotWidget::Action::ZoomRegion) {
        // Draw the region to be zoomed in

        QRect zoomRegionRect = zoomRegion();
        // Note: see drawCoordinates() as why we use QRect rather than QRectF...

        if (mOwner->zoomRegionFilled()) {
            painter.fillRect(zoomRegionRect, mOwner->zoomRegionFillColor());
        }

        QPen pen = painter.pen();

        pen.setJoinStyle(Qt::RoundJoin);
        pen.setColor(mOwner->zoomRegionColor());
        pen.setStyle(mOwner->zoomRegionStyle());
        pen.setWidth(mOwner->zoomRegionWidth());

        painter.setPen(pen);

        painter.drawRect(zoomRegionRect);

        // Draw the two sets of coordinates

        drawCoordinates(&painter, zoomRegionRect.topLeft(),
                        mOwner->zoomRegionColor(),
                        mOwner->zoomRegionFontColor(),
                        mOwner->zoomRegionFontSize(),
                        (mOwner->zoomRegionStyle() == Qt::NoPen)?
                            0:
                            mOwner->zoomRegionWidth(),
                        Position::BottomRight, false);
        drawCoordinates(&painter, zoomRegionRect.topLeft()+QPoint(zoomRegionRect.width(), zoomRegionRect.height()),
                        mOwner->zoomRegionColor(),
                        mOwner->zoomRegionFontColor(),
                        mOwner->zoomRegionFontSize(),
                        (mOwner->zoomRegionStyle() == Qt::NoPen)?
                            0:
                            mOwner->zoomRegionWidth(),
                        Position::TopLeft, false);
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

    int minX = int(canvasMapX.transform(mOwner->minX()));
    int maxX = int(canvasMapX.transform(mOwner->maxX()));
    int minY = int(canvasMapY.transform(mOwner->maxY()));
    int maxY = int(canvasMapY.transform(mOwner->minY()));

    if (mOwner->canZoomInX() || mOwner->canZoomInY()) {
        QPoint originPoint = optimizedPoint(mOriginPoint);
        QPoint point = optimizedPoint(mPoint);

        if (mOwner->canZoomInX()) {
            minX = qMin(originPoint.x(), point.x());
            maxX = qMax(originPoint.x(), point.x());
        }

        if (mOwner->canZoomInY()) {
            minY = qMin(originPoint.y(), point.y());
            maxY = qMax(originPoint.y(), point.y());
        }
    }

    return { minX, minY, maxX-minX, maxY-minY };
}

//==============================================================================

void GraphPanelPlotOverlayWidget::drawCoordinates(QPainter *pPainter,
                                                  const QPoint &pPoint,
                                                  const QColor &pBackgroundColor,
                                                  const QColor &pForegroundColor,
                                                  const int &pFontSize,
                                                  int pLineWidth,
                                                  Position pPosition,
                                                  bool pCanMovePosition)
{
    // Retrieve the size of coordinates as they will appear on the screen
    // Note #1: normally, pPoint would be a QPointF, but we want the coordinates
    //          to be drawn relative to something (see paintEvent()) and the
    //          only way to guarantee that everything will be painted as
    //          expected is to use QPoint. Indeed, if we were to use QPointF,
    //          then QPainter would have to do some rounding and though
    //          everything should be fine (since we always add/subtract a
    //          rounded number), it happens that it's not always the case.
    //          Indeed, we should always have a gap of one pixel between the
    //          coordinates and pPoint, but it could happen that we have either
    //          no gap, or one or two pixels...
    // Note #2: we set the painter's pen's style to a solid line otherwise
    //          coordinatesRect will be empty if there is no style for showing
    //          the coordinates of a point or zooming in a region...

    QFont newFont = mOwner->font();

    newFont.setPointSize(pFontSize);

    pPainter->setFont(newFont);

    QPointF point = mOwner->canvasPoint(pPoint);
    QString coordinates = QString("X: %1\nY: %2").arg(QLocale().toString(point.x(), 'g', 15),
                                                      QLocale().toString(point.y(), 'g', 15));
    QPen pen = pPainter->pen();

    pen.setStyle(Qt::SolidLine);

    pPainter->setPen(pen);

    QRect coordinatesRect = pPainter->boundingRect(qApp->primaryScreen()->availableGeometry(), 0, coordinates);

    // Determine where the coordinates and its background should be drawn

    int shift = pLineWidth/2+pLineWidth%2;
    int plusShift = shift+1;
    int minusShift = shift+int(pLineWidth%2 == 0);

    switch (pPosition) {
    case Position::TopLeft:
        coordinatesRect.moveTo(pPoint.x()-coordinatesRect.width()-minusShift,
                               pPoint.y()-coordinatesRect.height()-minusShift);

        break;
    case Position::TopRight:
        coordinatesRect.moveTo(pPoint.x()+plusShift,
                               pPoint.y()-coordinatesRect.height()-minusShift);

        break;
    case Position::BottomLeft:
        coordinatesRect.moveTo(pPoint.x()-coordinatesRect.width()-minusShift,
                               pPoint.y()+plusShift);

        break;
    case Position::BottomRight:
        coordinatesRect.moveTo(pPoint.x()+plusShift,
                               pPoint.y()+plusShift);

        break;
    }

    if (pCanMovePosition) {
        QwtScaleMap canvasMapX = mOwner->canvasMap(QwtPlot::xBottom);
        QwtScaleMap canvasMapY = mOwner->canvasMap(QwtPlot::yLeft);

        QPoint topLeftPoint = QPoint(int(canvasMapX.transform(mOwner->minX())),
                                     int(canvasMapY.transform(mOwner->maxY())));
        QPoint bottomRightPoint = QPoint(int(canvasMapX.transform(mOwner->maxX())),
                                         int(canvasMapY.transform(mOwner->minY())));

        if (coordinatesRect.top() < topLeftPoint.y()) {
            coordinatesRect.moveTop(pPoint.y()+plusShift);
        } else if (coordinatesRect.top()+coordinatesRect.height()-1 > bottomRightPoint.y()) {
            coordinatesRect.moveTop(pPoint.y()-coordinatesRect.height()-minusShift);
        }

        if (coordinatesRect.left() < topLeftPoint.x()) {
            coordinatesRect.moveLeft(pPoint.x()+plusShift);
        } else if (coordinatesRect.left()+coordinatesRect.width()-1 > bottomRightPoint.x()) {
            coordinatesRect.moveLeft(pPoint.x()-coordinatesRect.width()-minusShift);
        }

        // Note: the -1 for the else-if tests is because fillRect() below works
        //       on (0, 0; width-1, height-1)...
    }

    // Draw a filled rectangle to act as the background for the coordinates
    // we are to show

    pPainter->fillRect(coordinatesRect, pBackgroundColor);

    // Draw the text for the coordinates

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

void GraphPanelPlotScaleWidget::updateLayout()
{
    // Our layout has changed, so update our internals

    layoutScale();
}

//==============================================================================

GraphPanelPlotLegendWidget::GraphPanelPlotLegendWidget(GraphPanelPlotWidget *pParent) :
    QwtLegend(pParent),
    mOwner(pParent),
    mFontSize(pParent->fontSize()),
    mBackgroundColor(pParent->backgroundColor()),
    mForegroundColor(pParent->surroundingAreaForegroundColor())
{
    // Have our legend items use as much horizontal space as possible

    auto contentsWidgetLayout = static_cast<QwtDynGridLayout *>(contentsWidget()->layout());

    contentsWidgetLayout->setExpandingDirections(Qt::Horizontal);
    contentsWidgetLayout->setSpacing(0);

    // Customise ourselves a bit

    setDefaultItemMode(QwtLegendData::Checkable);

    // Check when someone clicks on a legend item

    connect(this, &GraphPanelPlotLegendWidget::checked,
            this, &GraphPanelPlotLegendWidget::legendChecked);
}

//==============================================================================

bool GraphPanelPlotLegendWidget::isActive() const
{
    // Return our active state

    return mActive;
}

//==============================================================================

void GraphPanelPlotLegendWidget::setActive(bool pActive)
{
    // Set our active state

    if (pActive != mActive) {
        mActive = pActive;

        mOwner->updateLegend();
    }
}

//==============================================================================

bool GraphPanelPlotLegendWidget::isEmpty() const
{
    // Return whether we are empty, based on whether we have a zero size hint

    return sizeHint().isNull();
}

//==============================================================================

void GraphPanelPlotLegendWidget::setChecked(GraphPanelPlotGraph *pGraph,
                                            bool pChecked)
{
    // Un/check the legend label associated with the given graph

    mLegendLabels.value(pGraph)->setChecked(pChecked);
}

//==============================================================================

int GraphPanelPlotLegendWidget::fontSize() const
{
    // Return our font size

    return mFontSize;
}

//==============================================================================

void GraphPanelPlotLegendWidget::setFontSize(int pFontSize)
{
    // Set our font size

    if (pFontSize != mFontSize) {
        mFontSize = pFontSize;

        mOwner->updateLegend();
    }
}

//==============================================================================

void GraphPanelPlotLegendWidget::setBackgroundColor(const QColor &pBackgroundColor)
{
    // Set our background color

    if (pBackgroundColor != mBackgroundColor) {
        mBackgroundColor = pBackgroundColor;

        mOwner->updateLegend();
    }
}

//==============================================================================

void GraphPanelPlotLegendWidget::setForegroundColor(const QColor &pForegroundColor)
{
    // Set our foreground color

    if (pForegroundColor != mForegroundColor) {
        mForegroundColor = pForegroundColor;

        mOwner->updateLegend();
    }
}

//==============================================================================

void GraphPanelPlotLegendWidget::renderLegend(QPainter *pPainter,
                                              const QRectF &pRect,
                                              bool pFillBackground) const
{
    // Render our legend
    // Note: this is based on QwtLegend::renderLegend() and it fixes a problem
    //       with the layout when it has its expanding directions set to
    //       horizontal. More information on this issue can be found at
    //       https://qtcentre.org/threads/68983-Problem-with-QwtDynGridLayout-layoutItems()...

    if (!mActive) {
        return;
    }

    if (pFillBackground) {
        if (autoFillBackground() || testAttribute(Qt::WA_StyledBackground)) {
            QwtPainter::drawBackgound(pPainter, pRect, this);
        }
    }

    int top;
    int left;
    int bottom;
    int right;

    getContentsMargins(&left, &top, &right, &bottom);

    QRect layoutRect = QRect();

    layoutRect.setTop(qCeil(pRect.top())+top);
    layoutRect.setLeft(qCeil(pRect.left())+left);
    layoutRect.setBottom(qFloor(pRect.bottom())-bottom);
    layoutRect.setRight(qFloor(pRect.right())-right);

    auto legendLayout = static_cast<QwtDynGridLayout *>(contentsWidget()->layout());
    QList<QRect> itemRects = legendLayout->layoutItems(layoutRect, legendLayout->columnsForWidth(layoutRect.width()));

    if ((legendLayout->expandingDirections() & Qt::Horizontal) != 0) {
        for (auto &itemRect : itemRects) {
            itemRect.adjust(layoutRect.left(), 0, layoutRect.left(), 0);
        }
    }

    int index = -1;

    for (int i = 0, iMax = legendLayout->count(); i < iMax; ++i) {
        QWidget *widget = legendLayout->itemAt(i)->widget();

        if (widget != nullptr) {
            ++index;

            pPainter->save();
            pPainter->setClipRect(itemRects[index], Qt::IntersectClip);

            renderItem(pPainter, widget, itemRects[index], pFillBackground);

            pPainter->restore();
        }
    }
}

//==============================================================================

void GraphPanelPlotLegendWidget::setSizeHintWidth(int pSizeHintWidth)
{
    // Set our internal size hint width

    mSizeHintWidth = pSizeHintWidth;
}

//==============================================================================

QSize GraphPanelPlotLegendWidget::sizeHint() const
{
    // Return our size hint, based on our internal size hint width and the
    // height of our default size hint

    return { mSizeHintWidth, QwtLegend::sizeHint().height() };
}

//==============================================================================

void GraphPanelPlotLegendWidget::updateWidget(QWidget *pWidget,
                                              const QwtLegendData &pLegendData)
{
    // Check whether we are dealing with one of our main legend labels

    auto legendLabel = static_cast<QwtLegendLabel *>(pWidget);
    bool mainLegendLabel = std::find(mLegendLabels.begin(), mLegendLabels.end(), legendLabel) != mLegendLabels.end();

    if (mainLegendLabel) {
        // We are dealing with a main legend label, i.e. a legend label
        // associated with our dummy run, so start with the default handling of
        // our method

        QwtLegend::updateWidget(pWidget, pLegendData);

        // Update our visible state

        legendLabel->setAutoFillBackground(true);
        legendLabel->setVisible(mActive);

        // Update our font size, as well as background and foreground colours

        QFont newFont = legendLabel->font();

        newFont.setPointSize(mFontSize);

        legendLabel->setFont(newFont);

        QPalette newPalette = legendLabel->palette();

        newPalette.setColor(QPalette::Window, mBackgroundColor);
        newPalette.setColor(QPalette::Text, mForegroundColor);

        legendLabel->setPalette(newPalette);

        // Make sure that updates are enabled
        // Note: indeed, when setting its data, QwtLegendLabel (which is used by
        //       QwtLegend) prevents itself from updating, and then reallows
        //       itself to be updated, if it was originally allowed. Now, the
        //       problem is that if one of our ancestors decides to temporarily
        //       disable updates (e.g. our simulation experiment view) then
        //       QwtLegendLabel won't reenable itself, which means that the
        //       legend may not actually be visible in some cases (e.g. when
        //       opening a SED-ML file)...

        pWidget->setUpdatesEnabled(true);
    } else {
        // This is not one of our main legend label, so ignore it (i.e. minimise
        // and hide it)

        legendLabel->resize(0, 0);
        legendLabel->hide();
    }
}

//==============================================================================

void GraphPanelPlotLegendWidget::addGraph(GraphPanelPlotGraph *pGraph)
{
    // Associate the given graph with our last contents widget's child

    mLegendLabels.insert(pGraph, static_cast<QwtLegendLabel *>(contentsWidget()->children().last()));
}

//==============================================================================

void GraphPanelPlotLegendWidget::removeGraph(GraphPanelPlotGraph *pGraph)
{
    // Stop associating our last contents widget's child with the given graph

    mLegendLabels.remove(pGraph);
}

//==============================================================================

bool GraphPanelPlotLegendWidget::needScrollBar() const
{
    // Determine and return whether we need a (vertical) scroll bar

    int legendLabelsHeight = 0;

    for (auto legendLabel : mLegendLabels) {
        legendLabelsHeight += legendLabel->height();
    }

    return (pos().y() == 0) && (legendLabelsHeight > height());
}

//==============================================================================

void GraphPanelPlotLegendWidget::legendChecked(const QVariant &pItemInfo)
{
    // Make sure that the corresponding graph panel is selected
    // Note: this makes it much easier to handle the graphToggled() signal
    //       afterwards...

    mOwner->setActive(true);

    // Let people know that the given graph has been toggled

    emit graphToggled(static_cast<GraphPanelPlotGraph *>(static_cast<GraphPanelPlotGraphRun *>(mOwner->infoToItem(pItemInfo))->owner()));
}

//==============================================================================

static const double DblMinAxis = 1000.0*DBL_MIN;
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
                                           GraphPanelWidget *pParent) :
    QwtPlot(pParent),
    Core::CommonWidget(this),
    mOwner(pParent),
    mLegend(new GraphPanelPlotLegendWidget(this)),
    mSynchronizeXAxisAction(pSynchronizeXAxisAction),
    mSynchronizeYAxisAction(pSynchronizeYAxisAction),
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

    connect(pParent->parent(), &QObject::destroyed,
            this, &GraphPanelPlotWidget::cannotUpdateActions);

    // Get ourselves a direct painter

    mDirectPainter = new QwtPlotDirectPainter(this);

    mDirectPainter->setAttribute(QwtPlotDirectPainter::CopyBackingStore, true);

    // Speedup painting on X11 systems
    // Note: this can only be done on X11 systems...

    if (QwtPainter::isX11GraphicsSystem()) {
        canvas()->setAttribute(Qt::WA_PaintOnScreen, true);
    }

    // Customise ourselves a bit

    setAutoFillBackground(true);
    setBackgroundColor(Qt::white);
    setFontSize(10);
    setForegroundColor(Qt::black);

    QColor pointCoordinatesColor = Qt::darkCyan;

    pointCoordinatesColor.setAlphaF(0.69);

    setPointCoordinatesColor(pointCoordinatesColor);

    setSurroundingAreaBackgroundColor(Core::windowColor());
    setSurroundingAreaForegroundColor(Qt::black);

    QColor zoomRegionColor = Qt::darkRed;
    QColor zoomRegionFillColor = Qt::yellow;

    zoomRegionColor.setAlphaF(0.69);
    zoomRegionFillColor.setAlphaF(0.19);

    setZoomRegionColor(zoomRegionColor);
    setZoomRegionFillColor(zoomRegionFillColor);

    grabGesture(Qt::PinchGesture);

    // Customise our canvas a bit
    // Note: to use immediate paint prevents our canvas from becoming black in
    //       some cases (e.g. when running a long simulation)...

    auto plotCanvas = static_cast<QwtPlotCanvas *>(canvas());

    plotCanvas->setFrameShape(QFrame::NoFrame);
    plotCanvas->setPaintAttribute(QwtPlotCanvas::ImmediatePaint);

    // Add some axes to ourselves and prevent them from auto-scaling

    mAxisX = new GraphPanelPlotScaleDraw();
    mAxisY = new GraphPanelPlotScaleDraw();

    setAxisScaleDraw(QwtPlot::xBottom, mAxisX);
    setAxisScaleDraw(QwtPlot::yLeft, mAxisY);

    setAxisAutoScale(QwtPlot::xBottom, false);
    setAxisAutoScale(QwtPlot::yLeft, false);

    setFontSizeAxisX(10);
    setFontSizeAxisY(10);

    // Attach a grid to ourselves

    mGrid = new QwtPlotGrid();

    mGrid->setMajorPen(Qt::gray, 1, Qt::DotLine);

    mGrid->attach(this);

    // Create our overlay widget

    mOverlayWidget = new GraphPanelPlotOverlayWidget(this);

    // Add our (empty) legend and let people know whenever one of its
    // corresponding graphs has been toggled

    insertLegend(mLegend);

    setLegendFontSize(10);

    connect(mLegend, &GraphPanelPlotLegendWidget::graphToggled,
            this, &GraphPanelPlotWidget::graphToggled);

    // Create our context menu

    mContextMenu = new QMenu(this);

    mExportToAction = Core::newAction(this);
    mCopyToClipboardAction = Core::newAction(this);
    mGraphPanelSettingsAction = Core::newAction(this);
    mGraphsSettingsAction = Core::newAction(this);
    mLegendAction = Core::newAction(true, this);
    mLogarithmicXAxisAction = Core::newAction(true, this);
    mLogarithmicYAxisAction = Core::newAction(true, this);
    mCustomAxesAction = Core::newAction(this);
    mZoomInAction = Core::newAction(this);
    mZoomOutAction = Core::newAction(this);
    mResetZoomAction = Core::newAction(this);

    connect(mExportToAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::exportTo);
    connect(mCopyToClipboardAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::copyToClipboard);
    connect(mGraphPanelSettingsAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::graphPanelSettingsRequested);
    connect(mGraphsSettingsAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::graphsSettingsRequested);
    connect(mLegendAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::legendToggled);
    connect(mLogarithmicXAxisAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::logarithmicXAxisToggled);
    connect(mLogarithmicYAxisAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::logarithmicYAxisToggled);
    connect(mCustomAxesAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::customAxes);
    connect(mZoomInAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::zoomIn);
    connect(mZoomOutAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::zoomOut);
    connect(mResetZoomAction, &QAction::triggered,
            this, &GraphPanelPlotWidget::resetZoom);

    mContextMenu->addAction(mExportToAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyToClipboardAction);

    if ((pSynchronizeXAxisAction != nullptr) && (pSynchronizeYAxisAction != nullptr)) {
        mContextMenu->addSeparator();
        mContextMenu->addAction(pSynchronizeXAxisAction);
        mContextMenu->addAction(pSynchronizeYAxisAction);
    }

    mContextMenu->addSeparator();
    mContextMenu->addAction(mGraphPanelSettingsAction);
    mContextMenu->addAction(mGraphsSettingsAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mLegendAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mLogarithmicXAxisAction);
    mContextMenu->addAction(mLogarithmicYAxisAction);
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

    setAxes(DefaultMinAxis, DefaultMaxAxis, DefaultMinAxis, DefaultMaxAxis,
            false, false, false, true, false, false);

    mDirtyAxes = false;

    // We want our legend to be active by default

    setLegendVisible(true);

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

GraphPanelPlotWidget::~GraphPanelPlotWidget()
{
    // Delete some internal objects

    delete mDirectPainter;

    for (auto graph : mGraphs) {
        delete graph;
    }
}

//==============================================================================

void GraphPanelPlotWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mExportToAction, tr("Export To..."),
                                     tr("Export the contents of the graph panel to a PDF, PNG, SVG, etc. file"));
    I18nInterface::retranslateAction(mCopyToClipboardAction, tr("Copy To Clipboard"),
                                     tr("Copy the contents of the graph panel to the clipboard"));
    I18nInterface::retranslateAction(mGraphPanelSettingsAction, tr("Graph Panel Settings..."),
                                     tr("Customise the graph panel"));
    I18nInterface::retranslateAction(mGraphsSettingsAction, tr("Graphs Settings..."),
                                     tr("Customise the graphs"));
    I18nInterface::retranslateAction(mLegendAction, tr("Legend"),
                                     tr("Show/hide the legend"));
    I18nInterface::retranslateAction(mLogarithmicXAxisAction, tr("Logarithmic X Axis"),
                                     tr("Enable/disable logarithmic scaling on the X axis"));
    I18nInterface::retranslateAction(mLogarithmicYAxisAction, tr("Logarithmic Y Axis"),
                                     tr("Enable/disable logarithmic scaling on the Y axis"));
    I18nInterface::retranslateAction(mCustomAxesAction, tr("Custom Axes..."),
                                     tr("Specify custom axes for the graph panel"));
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

void GraphPanelPlotWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::changeEvent(pEvent);

    // Check whether our enabled state has changed and if so then update our
    // background colour

    if (pEvent->type() == QEvent::EnabledChange) {
        setUpdatesEnabled(false);
            if (isEnabled()) {
                // Use the original colour of different things, but only if they
                // have been set, i.e. if we were disabled and now enabled

                if (mGraphs.count() == mEnabledGraphPens.count()) {
                    setBackgroundColor(mEnabledBackgroundColor);
                    setForegroundColor(mEnabledForegroundColor);

                    setSurroundingAreaBackgroundColor(mEnabledSurroundingAreaBackgroundColor);
                    setSurroundingAreaForegroundColor(mEnabledSurroundingAreaForegroundColor);

                    setGridLinesColor(mEnabledGridLinesColor);

                    int index = -1;

                    for (auto graph : mGraphs) {
                        const QwtSymbol *graphSymbol = graph->symbol();

                        ++index;

                        graph->setPen(mEnabledGraphPens[index]);
                        graph->setSymbol(graphSymbol->style(),
                                         mEnabledGraphSymbolBrushes[index],
                                         mEnabledGraphSymbolPens[index],
                                         graphSymbol->size());
                    }

                    // Reset a few things

                    mEnabledGraphPens.clear();
                    mEnabledGraphSymbolBrushes.clear();
                    mEnabledGraphSymbolPens.clear();
                }
            } else {
                // Keep track of various original colours

                mEnabledBackgroundColor = mBackgroundColor;
                mEnabledForegroundColor = mForegroundColor;

                mEnabledSurroundingAreaBackgroundColor = mSurroundingAreaBackgroundColor;
                mEnabledSurroundingAreaForegroundColor = mSurroundingAreaForegroundColor;

                mEnabledGridLinesColor = gridLinesColor();

                for (auto graph : mGraphs) {
                    const QwtSymbol *graphSymbol = graph->symbol();

                    mEnabledGraphPens << graph->pen();
                    mEnabledGraphSymbolBrushes << graphSymbol->brush();
                    mEnabledGraphSymbolPens << graphSymbol->pen();
                }

                // Use a disabled looking colour for different things

                QColor windowColor = Core::windowColor(QPalette::Disabled);
                QColor windowTextColor = Core::windowTextColor(QPalette::Disabled);

                setBackgroundColor(windowColor);
                setForegroundColor(windowTextColor);

                setSurroundingAreaBackgroundColor(windowColor);
                setSurroundingAreaForegroundColor(windowTextColor);

                QColor opaqueWindowTextColor = Core::opaqueColor(windowTextColor, windowColor);
                QBrush symbolBrush = opaqueWindowTextColor;
                QPen symbolPen = opaqueWindowTextColor.darker();

                setGridLinesColor(opaqueWindowTextColor);

                for (auto graph : mGraphs) {
                    QPen pen = graph->pen();
                    const QwtSymbol *graphSymbol = graph->symbol();

                    pen.setColor(opaqueWindowTextColor);

                    graph->setPen(pen);
                    graph->setSymbol(graphSymbol->style(), symbolBrush,
                                     symbolPen, graphSymbol->size());
                }
            }
        setUpdatesEnabled(true);
    }
}

//==============================================================================

bool GraphPanelPlotWidget::event(QEvent *pEvent)
{
    // Handle pinch gestures to zoom in/out

    if (pEvent->type() == QEvent::Gesture) {
        auto gestureEvent = static_cast<QGestureEvent*>(pEvent);

        if (QGesture *pinch = gestureEvent->gesture(Qt::PinchGesture)) {
            auto pinchGesture = static_cast<QPinchGesture *>(pinch);

            if ((pinchGesture->changeFlags() & QPinchGesture::ScaleFactorChanged) != 0) {
                scaleAxes(pinchGesture->centerPoint().toPoint(),
                          Scaling::CustomScaling, Scaling::CustomScaling,
                          1.0/pinchGesture->scaleFactor());

                return true;
            }
        }
    }

    // Default handling of the event

    return QwtPlot::event(pEvent);
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

        auto mouseEvent = static_cast<QMouseEvent *>(pEvent);

        if (   ((mouseEvent->modifiers() & Qt::ShiftModifier) == 0)
            && ((mouseEvent->modifiers() & Qt::ControlModifier) == 0)
            && ((mouseEvent->modifiers() & Qt::AltModifier) == 0)
            && ((mouseEvent->modifiers() & Qt::MetaModifier) == 0)
            && (mouseEvent->button() == Qt::LeftButton)) {
            resetAxes();
        }
    }

    return res;
}

//==============================================================================

void GraphPanelPlotWidget::updateActions()
{
    // Leave straightaway if we cannot update our actions anymore

    if (!mCanUpdateActions) {
        return;
    }

    // Update our actions

    double crtMinX = minX();
    double crtMaxX = maxX();
    double crtMinY = minY();
    double crtMaxY = maxY();

    double crtRangeX = crtMaxX-crtMinX;
    double crtRangeY = crtMaxY-crtMinY;

    mCanZoomInX = crtRangeX > MinAxisRange;
    mCanZoomOutX = crtRangeX < (mLogAxisX?MaxLogAxisRange:MaxAxisRange);

    mCanZoomInY = crtRangeY > MinAxisRange;
    mCanZoomOutY = crtRangeY < (mLogAxisY?MaxLogAxisRange:MaxAxisRange);

    // Update the enabled status of our actions

    mSynchronizeXAxisAction->setVisible(!mNeighbors.isEmpty());
    mSynchronizeYAxisAction->setVisible(!mNeighbors.isEmpty());

    mZoomInAction->setEnabled(mCanZoomInX || mCanZoomInY);
    mZoomOutAction->setEnabled(mCanZoomOutX || mCanZoomOutY);

    QRectF dRect = realDataRect();

    mResetZoomAction->setEnabled(   !qFuzzyCompare(crtMinX, dRect.left())
                                 || !qFuzzyCompare(crtMaxX, dRect.left()+dRect.width())
                                 || !qFuzzyCompare(crtMinY, dRect.top())
                                 || !qFuzzyCompare(crtMaxY, dRect.top()+dRect.height()));
}

//==============================================================================

void GraphPanelPlotWidget::checkAxisValues(bool pLogAxis, double &pMin,
                                           double &pMax)
{
    // Make sure that our axis' values have finite values

    double minAxis = pLogAxis?MinLogAxis:MinAxis;

    if (!qIsFinite(pMin)) {
        pMin = minAxis;
    }

    if (!qIsFinite(pMax)) {
        pMax = MaxAxis;
    }

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

    mAction = Action::None;

    mOverlayWidget->update();
}

//==============================================================================

QColor GraphPanelPlotWidget::backgroundColor() const
{
    // Return our background colour

    return mBackgroundColor;
}

//==============================================================================

void GraphPanelPlotWidget::setBackgroundColor(const QColor &pBackgroundColor)
{
    // Set our background colour
    // Note: setCanvasBackground() doesn't handle semi-transparent colours and,
    //       even if it did, it might take into account the grey background that
    //       is used by the rest of our object while we want a semi-transparent
    //       colour to rely on a white background, which we do here by computing
    //       an opaque colour from an opaque white colour and the given
    //       (potentially semi-transparent) colour...

    if (pBackgroundColor != mBackgroundColor) {
        mBackgroundColor = pBackgroundColor;

        static const QColor White = Qt::white;

        QBrush brush = canvasBackground();
        double ratio = pBackgroundColor.alpha()/256.0;
        QColor color;

        color.setRedF((1.0-ratio)*White.redF()+ratio*pBackgroundColor.redF());
        color.setGreenF((1.0-ratio)*White.greenF()+ratio*pBackgroundColor.greenF());
        color.setBlueF((1.0-ratio)*White.blueF()+ratio*pBackgroundColor.blueF());

        brush.setColor(color);

        setCanvasBackground(brush);

        // Legend

        mLegend->setBackgroundColor(color);

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

void GraphPanelPlotWidget::setFontSize(int pFontSize)
{
    // Set our font size

    if (pFontSize != fontSize()) {
        QFont newFont = font();

        newFont.setPointSize(pFontSize);

        setFont(newFont);

        // Title

        setTitle(title().text());
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::foregroundColor() const
{
    // Return our foreground colour

    return mForegroundColor;
}

//==============================================================================

void GraphPanelPlotWidget::setForegroundColor(const QColor &pForegroundColor)
{
    // Set our foreground colour

    if (pForegroundColor != mForegroundColor) {
        mForegroundColor = pForegroundColor;

        // Legend

        mLegend->setForegroundColor(pForegroundColor);

        replot();
    }
}

//==============================================================================

Qt::PenStyle GraphPanelPlotWidget::gridLinesStyle() const
{
    // Return our grid lines style

    return mGrid->majorPen().style();
}

//==============================================================================

void GraphPanelPlotWidget::setGridLinesStyle(const Qt::PenStyle &pGridLinesStyle)
{
    // Set our grid lines style

    if (pGridLinesStyle != gridLinesStyle()) {
        QPen newPen = mGrid->majorPen();

        newPen.setStyle(pGridLinesStyle);

        mGrid->setMajorPen(newPen);

        replot();
    }
}

//==============================================================================

int GraphPanelPlotWidget::gridLinesWidth() const
{
    // Return our grid lines width

    return mGrid->majorPen().width();
}

//==============================================================================

void GraphPanelPlotWidget::setGridLinesWidth(int pGridLinesWidth)
{
    // Set our grid lines width

    if (pGridLinesWidth != gridLinesWidth()) {
        QPen newPen = mGrid->majorPen();

        newPen.setWidth(pGridLinesWidth);

        mGrid->setMajorPen(newPen);

        replot();
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::gridLinesColor() const
{
    // Return our grid lines colour

    return mGrid->majorPen().color();
}

//==============================================================================

void GraphPanelPlotWidget::setGridLinesColor(const QColor &pGridLinesColor)
{
    // Set our grid lines colour

    if (pGridLinesColor != gridLinesColor()) {
        QPen newPen = mGrid->majorPen();

        newPen.setColor(pGridLinesColor);

        mGrid->setMajorPen(newPen);

        replot();
    }
}

//==============================================================================

int GraphPanelPlotWidget::legendFontSize() const
{
    // Return our legend font size

    return mLegend->fontSize();
}

//==============================================================================

void GraphPanelPlotWidget::setLegendFontSize(int pLegendFontSize)
{
    // Set our legend font size

    if (pLegendFontSize != legendFontSize()) {
        mLegend->setFontSize(pLegendFontSize);

        // To change the font size of our legen may have some effects on the
        // alignment with our neighbours, so update ourselves

        updateGui(false, true);
    }
}

//==============================================================================

bool GraphPanelPlotWidget::isLegendVisible() const
{
    // Return whether our legend is visible

    return mLegend->isActive();
}

//==============================================================================

void GraphPanelPlotWidget::setLegendVisible(bool pLegendVisible)
{
    // Show/hide our legend

    if (pLegendVisible != isLegendVisible()) {
        mLegend->setActive(pLegendVisible);
        mLegendAction->setChecked(pLegendVisible);

        // To show/hide our legend may have some effects on the alignment with
        // our neighbours, so update ourselves

        updateGui(false, true);
    }
}

//==============================================================================

Qt::PenStyle GraphPanelPlotWidget::pointCoordinatesStyle() const
{
    // Return our point coordinates style

    return mPointCoordinatesStyle;
}

//==============================================================================

void GraphPanelPlotWidget::setPointCoordinatesStyle(const Qt::PenStyle &pPointCoordinatesStyle)
{
    // Set our point coordinates style

    if (pPointCoordinatesStyle != mPointCoordinatesStyle) {
        mPointCoordinatesStyle = pPointCoordinatesStyle;
    }
}

//==============================================================================

int GraphPanelPlotWidget::pointCoordinatesWidth() const
{
    // Return our point coordinates width

    return mPointCoordinatesWidth;
}

//==============================================================================

void GraphPanelPlotWidget::setPointCoordinatesWidth(int pPointCoordinatesWidth)
{
    // Set our point coordinates width

    if (pPointCoordinatesWidth != mPointCoordinatesWidth) {
        mPointCoordinatesWidth = pPointCoordinatesWidth;
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::pointCoordinatesColor() const
{
    // Return our point coordinates colour

    return mPointCoordinatesColor;
}

//==============================================================================

void GraphPanelPlotWidget::setPointCoordinatesColor(const QColor &pPointCoordinatesColor)
{
    // Set our point coordinates colour

    if (pPointCoordinatesColor != mPointCoordinatesColor) {
        mPointCoordinatesColor = pPointCoordinatesColor;
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::pointCoordinatesFontColor() const
{
    // Return our point coordinates font colour

    return mPointCoordinatesFontColor;
}

//==============================================================================

void GraphPanelPlotWidget::setPointCoordinatesFontColor(const QColor &pPointCoordinatesFontColor)
{
    // Set our point coordinates font colour

    if (pPointCoordinatesFontColor != mPointCoordinatesFontColor) {
        mPointCoordinatesFontColor = pPointCoordinatesFontColor;
    }
}

//==============================================================================

int GraphPanelPlotWidget::pointCoordinatesFontSize() const
{
    // Return our point coordinates font size

    return mPointCoordinatesFontSize;
}

//==============================================================================

void GraphPanelPlotWidget::setPointCoordinatesFontSize(int pPointCoordinatesFontSize)
{
    // Set our point coordinates font size

    if (pPointCoordinatesFontSize != mPointCoordinatesFontSize) {
        mPointCoordinatesFontSize = pPointCoordinatesFontSize;
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::surroundingAreaBackgroundColor() const
{
    // Return our surrounding area background colour

    return mSurroundingAreaBackgroundColor;
}

//==============================================================================

void GraphPanelPlotWidget::setSurroundingAreaBackgroundColor(const QColor &pSurroundingAreaBackgroundColor)
{
    // Set our surrounding area background colour

    if (pSurroundingAreaBackgroundColor != mSurroundingAreaBackgroundColor) {
        mSurroundingAreaBackgroundColor = pSurroundingAreaBackgroundColor;

        // Plot area

        QPalette newPalette = palette();

        newPalette.setColor(QPalette::Window, pSurroundingAreaBackgroundColor);

        setPalette(newPalette);

        replot();
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::surroundingAreaForegroundColor() const
{
    // Return our surrounding area foreground colour

    return mSurroundingAreaForegroundColor;
}

//==============================================================================

void GraphPanelPlotWidget::setSurroundingAreaForegroundColor(const QColor &pSurroundingAreaForegroundColor)
{
    // Set our surrounding area foreground colour

    if (pSurroundingAreaForegroundColor != mSurroundingAreaForegroundColor) {
        mSurroundingAreaForegroundColor = pSurroundingAreaForegroundColor;

        // Title

        setTitle(title().text());

        // X axis

        QwtScaleWidget *scaleWidget = axisWidget(QwtPlot::xBottom);
        QPalette newPalette = scaleWidget->palette();

        newPalette.setColor(QPalette::Text, pSurroundingAreaForegroundColor);
        newPalette.setColor(QPalette::WindowText, pSurroundingAreaForegroundColor);

        scaleWidget->setPalette(newPalette);

        setTitleAxisX(titleAxisX());

        // Y axis

        scaleWidget = axisWidget(QwtPlot::yLeft);
        newPalette = scaleWidget->palette();

        newPalette.setColor(QPalette::Text, pSurroundingAreaForegroundColor);
        newPalette.setColor(QPalette::WindowText, pSurroundingAreaForegroundColor);

        scaleWidget->setPalette(newPalette);

        setTitleAxisY(titleAxisY());
    }
}

//==============================================================================

void GraphPanelPlotWidget::setTitle(const QString &pTitle)
{
    // Set our title

    if (pTitle.isEmpty()) {
        QwtPlot::setTitle(QString());
    } else {
        QwtText title = QwtText(pTitle);
        QFont newFont = title.font();

        newFont.setPointSize(fontSize());

        title.setColor(mSurroundingAreaForegroundColor);
        title.setFont(newFont);

        QwtPlot::setTitle(title);
    }
}

//==============================================================================

int GraphPanelPlotWidget::fontSizeAxisX() const
{
    // Return our X axis font size

    return axisFont(QwtPlot::xBottom).pointSize();
}

//==============================================================================

void GraphPanelPlotWidget::setFontSizeAxisX(int pFontSizeAxisX)
{
    // Set our X axis font size

    if (pFontSizeAxisX != fontSizeAxisX()) {
        QFont fontAxisX = axisFont(QwtPlot::xBottom);

        fontAxisX.setPointSize(pFontSizeAxisX);

        setAxisFont(QwtPlot::xBottom, fontAxisX);
        setTitleAxisX(titleAxisX());

        // To change the font size of our X axis may have some effects on the
        // alignment with our neighbours, so update ourselves

        updateGui(false, true);
    }
}

//==============================================================================

bool GraphPanelPlotWidget::logAxisX() const
{
    // Return whether our X axis uses a logarithmic scale

    return mLogAxisX;
}

//==============================================================================

void GraphPanelPlotWidget::setLogAxisX(bool pLogAxisX)
{
    // Specify whether our X axis should use a logarithmic scale

    if (pLogAxisX != mLogAxisX) {
        mLogAxisX = pLogAxisX;
        mLogarithmicXAxisAction->setChecked(pLogAxisX);

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

    setTitleAxis(QwtPlot::xBottom, pTitleAxisX);
}

//==============================================================================

int GraphPanelPlotWidget::fontSizeAxisY() const
{
    // Return our Y axis font size

    return axisFont(QwtPlot::yLeft).pointSize();
}

//==============================================================================

void GraphPanelPlotWidget::setFontSizeAxisY(int pFontSizeAxisY)
{
    // Set our Y axis font size

    if (pFontSizeAxisY != fontSizeAxisY()) {
        QFont fontAxisY = axisFont(QwtPlot::yLeft);

        fontAxisY.setPointSize(pFontSizeAxisY);

        setAxisFont(QwtPlot::yLeft, fontAxisY);
        setTitleAxisY(titleAxisY());

        // To change the font size of our Y axis may have some effects on the
        // alignment with our neighbours, so update ourselves

        updateGui(false, true);
    }
}

//==============================================================================

bool GraphPanelPlotWidget::logAxisY() const
{
    // Return whether our Y axis uses a logarithmic scale

    return mLogAxisY;
}

//==============================================================================

void GraphPanelPlotWidget::setLogAxisY(bool pLogAxisY)
{
    // Specify whether our Y axis should use a logarithmic scale

    if (pLogAxisY != mLogAxisY) {
        mLogAxisY = pLogAxisY;
        mLogarithmicYAxisAction->setChecked(pLogAxisY);

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

    QwtText oldAxisTitle = axisTitle(QwtPlot::yLeft);

    setTitleAxis(QwtPlot::yLeft, pTitleAxisY);

    // To change the title of our Y axis may have some effects on the alignment
    // with our neighbours, so update ourselves

    if (axisTitle(QwtPlot::yLeft).text() != oldAxisTitle.text()) {
        updateGui(false, true);
    }
}

//==============================================================================

Qt::PenStyle GraphPanelPlotWidget::zoomRegionStyle() const
{
    // Return our zoom region style

    return mZoomRegionStyle;
}

//==============================================================================

void GraphPanelPlotWidget::setZoomRegionStyle(const Qt::PenStyle &pZoomRegionStyle)
{
    // Set our zoom region style

    if (pZoomRegionStyle != mZoomRegionStyle) {
        mZoomRegionStyle = pZoomRegionStyle;
    }
}

//==============================================================================

int GraphPanelPlotWidget::zoomRegionWidth() const
{
    // Return our zoom region width

    return mZoomRegionWidth;
}

//==============================================================================

void GraphPanelPlotWidget::setZoomRegionWidth(int pZoomRegionWidth)
{
    // Set our zoom region width

    if (pZoomRegionWidth != mZoomRegionWidth) {
        mZoomRegionWidth = pZoomRegionWidth;
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::zoomRegionColor() const
{
    // Return our zoom region colour

    return mZoomRegionColor;
}

//==============================================================================

void GraphPanelPlotWidget::setZoomRegionColor(const QColor &pZoomRegionColor)
{
    // Set our zoom region colour

    if (pZoomRegionColor != mZoomRegionColor) {
        mZoomRegionColor = pZoomRegionColor;
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::zoomRegionFontColor() const
{
    // Return our zoom region font colour

    return mZoomRegionFontColor;
}

//==============================================================================

void GraphPanelPlotWidget::setZoomRegionFontColor(const QColor &pZoomRegionFontColor)
{
    // Set our zoom region font colour

    if (pZoomRegionFontColor != mZoomRegionFontColor) {
        mZoomRegionFontColor = pZoomRegionFontColor;
    }
}

//==============================================================================

int GraphPanelPlotWidget::zoomRegionFontSize() const
{
    // Return our zoom region font size

    return mZoomRegionFontSize;
}

//==============================================================================

void GraphPanelPlotWidget::setZoomRegionFontSize(int pZoomRegionFontSize)
{
    // Set our zoom region font size

    if (pZoomRegionFontSize != mZoomRegionFontSize) {
        mZoomRegionFontSize = pZoomRegionFontSize;
    }
}

//==============================================================================

bool GraphPanelPlotWidget::zoomRegionFilled() const
{
    // Return our zoom region filled status

    return mZoomRegionFilled;
}

//==============================================================================

void GraphPanelPlotWidget::setZoomRegionFilled(bool pZoomRegionFilled)
{
    // Set our zoom region filled status

    if (pZoomRegionFilled != mZoomRegionFilled) {
        mZoomRegionFilled = pZoomRegionFilled;
    }
}

//==============================================================================

QColor GraphPanelPlotWidget::zoomRegionFillColor() const
{
    // Return our zoom region fill colour

    return mZoomRegionFillColor;
}

//==============================================================================

void GraphPanelPlotWidget::setZoomRegionFillColor(const QColor &pZoomRegionFillColor)
{
    // Set our zoom region fill colour

    if (pZoomRegionFillColor != mZoomRegionFillColor) {
        mZoomRegionFillColor = pZoomRegionFillColor;
    }
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

void GraphPanelPlotWidget::setActive(bool pActive)
{
    // In/activate ourselves by asking our owner to in/activate itself

    mOwner->setActive(pActive);
}

//==============================================================================

GraphPanelPlotGraphs GraphPanelPlotWidget::graphs() const
{
    // Return all our graphs

    return mGraphs;
}

//==============================================================================

bool GraphPanelPlotWidget::isOptimizedAxes() const
{
    // Return whether both of our axes are optimised

    return mOptimizedAxisX && mOptimizedAxisY;
}

//==============================================================================

void GraphPanelPlotWidget::optimizeAxis(int pAxisId, double &pMin, double &pMax,
                                        Optimization pOptimization)
{
    // Make sure that the given values are different (and therefore optimisable
    // as such)

    if (qFuzzyCompare(pMin, pMax)) {
        // The given values are the same, so update them so that we can properly
        // optimise them below

        double powerValue = qFuzzyIsNull(pMin)?
                                0.0:
                                qFloor(log10(qAbs(pMin)))-1.0;

        pMin = pMin-pow(10.0, powerValue);
        pMax = pMax+pow(10.0, powerValue);

        if (pAxisId == QwtPlot::xBottom) {
            mOptimizedAxisX = false;
        } else {
            mOptimizedAxisY = false;
        }
    } else {
        if (pAxisId == QwtPlot::xBottom) {
            mOptimizedAxisX = true;
        } else {
            mOptimizedAxisY = true;
        }
    }

    // Optimise the axis' values, using either a linear or logarithmic approach

    if (   (   (pOptimization == Optimization::Default)
            && (   ((pAxisId == QwtPlot::xBottom) && !mLogAxisX)
                || ((pAxisId == QwtPlot::yLeft) && !mLogAxisY)))
        || (pOptimization == Optimization::Linear)) {
        uint base = axisScaleEngine(pAxisId)->base();
        double majorStep = QwtScaleArithmetic::divideInterval(pMax-pMin,
                                                              axisMaxMajor(pAxisId),
                                                              base);
        double minorStep = QwtScaleArithmetic::divideInterval(majorStep,
                                                              axisMaxMinor(pAxisId),
                                                              base);
        double minOverMinorStep = pMin/minorStep;
        double maxOverMinorStep = pMax/minorStep;

        pMin = qFuzzyCompare(minOverMinorStep, int(minOverMinorStep))?
                    minOverMinorStep*minorStep:
                    qFloor(minOverMinorStep)*minorStep;
        pMax = qFuzzyCompare(maxOverMinorStep, int(maxOverMinorStep))?
                    maxOverMinorStep*minorStep:
                    qCeil(maxOverMinorStep)*minorStep;
    } else {
        double minStep = pow(10.0, qFloor(log10(pMin))-1);
        double maxStep = pow(10.0, qCeil(log10(pMax))-1);
        double minOverMinStep = pMin/minStep;
        double maxOverMaxStep = pMax/maxStep;

        pMin = qFuzzyCompare(minOverMinStep, int(minOverMinStep))?
                    minOverMinStep*minStep:
                    qFloor(minOverMinStep)*minStep;
        pMax = qFuzzyCompare(maxOverMaxStep, int(maxOverMaxStep))?
                    maxOverMaxStep*maxStep:
                    qCeil(maxOverMaxStep)*maxStep;
    }
}

//==============================================================================

void GraphPanelPlotWidget::optimizeAxisX(double &pMin, double &pMax,
                                         Optimization pOptimization)
{
    // Optimise our X axis' values

    optimizeAxis(QwtPlot::xBottom, pMin, pMax, pOptimization);
}

//==============================================================================

void GraphPanelPlotWidget::optimizeAxisY(double &pMin, double &pMax,
                                         Optimization pOptimization)
{
    // Optimise our Y axis' values

    optimizeAxis(QwtPlot::yLeft, pMin, pMax, pOptimization);
}

//==============================================================================

bool GraphPanelPlotWidget::hasData() const
{
    // Determine and return whether at least one of the graphs, which are valid
    // and selected, has some data

    for (auto graph : mGraphs) {
        if (graph->isValid() && graph->isSelected() && graph->hasData()) {
            return true;
        }
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

    for (auto graph : mGraphs) {
        if (graph->isValid() && graph->isSelected() && graph->hasData()) {
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

    for (auto graph : mGraphs) {
        if (graph->isValid() && graph->isSelected() && graph->hasData()) {
            pDataLogRect |= graph->boundingLogRect();

            res = true;
        }
    }

    return res;
}

//==============================================================================

QRectF GraphPanelPlotWidget::realDataRect()
{
    // Return an optimised version of dataRect()/dataLogRect() or a default
    // rectangle, if no dataRect()/dataLogRect() exists

    QRectF dRect;
    QRectF dLogRect;

    if (dataRect(dRect) && dataLogRect(dLogRect)) {
        // Optimise our axes' values

        double minX = mLogAxisX?
                          mOptimizedAxisX?
                              qMin(mDefaultMinLogX, dLogRect.left()):
                              dLogRect.left():
                          mOptimizedAxisX?
                              qMin(mDefaultMinX, dRect.left()):
                              dRect.left();
        double maxX = mLogAxisX?
                          mOptimizedAxisX?
                              qMax(mDefaultMaxLogX, dLogRect.right()):
                              dLogRect.right():
                          mOptimizedAxisX?
                              qMax(mDefaultMaxX, dRect.right()):
                              dRect.right();
        double minY = mLogAxisY?
                          mOptimizedAxisY?
                              qMin(mDefaultMinLogY, dLogRect.top()):
                              dLogRect.top():
                          mOptimizedAxisY?
                              qMin(mDefaultMinY, dRect.top()):
                              dRect.top();
        double maxY = mLogAxisY?
                          mOptimizedAxisY?
                              qMax(mDefaultMaxLogY, dLogRect.bottom()):
                              dLogRect.bottom():
                          mOptimizedAxisY?
                              qMax(mDefaultMaxY, dRect.bottom()):
                              dRect.bottom();

        optimizeAxisX(minX, maxX);
        optimizeAxisY(minY, maxY);

        return { minX, minY, maxX-minX, maxY-minY };
    }

    double minX = mLogAxisX?mDefaultMinLogX:mDefaultMinX;
    double maxX = mLogAxisX?mDefaultMaxLogX:mDefaultMaxX;
    double minY = mLogAxisY?mDefaultMinLogY:mDefaultMinY;
    double maxY = mLogAxisY?mDefaultMaxLogY:mDefaultMaxY;

    return { minX, minY, maxX-minX, maxY-minY };
}

//==============================================================================

void GraphPanelPlotWidget::setAxis(int pAxisId, double pMin, double pMax)
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

void GraphPanelPlotWidget::setDefaultAxesValues(double pDefaultMinX,
                                                double pDefaultMaxX,
                                                double pDefaultMinLogX,
                                                double pDefaultMaxLogX,
                                                double pDefaultMinY,
                                                double pDefaultMaxY,
                                                double pDefaultMinLogY,
                                                double pDefaultMaxLogY)
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
                                   double pMaxY, bool pSynchronizeAxes,
                                   bool pCanReplot, bool pEmitSignal,
                                   bool pForceAxesSetting,
                                   bool pSynchronizeXAxis,
                                   bool pSynchronizeYAxis)
{
    // Axes can only be set if they are not dirty or if we want to force the
    // setting of our X/Y axes

    if (mDirtyAxes && !pForceAxesSetting && !pSynchronizeXAxis && !pSynchronizeYAxis) {
        return false;
    }

    // Keep track of our axes' old values

    double oldMinX = minX();
    double oldMaxX = maxX();
    double oldMinY = minY();
    double oldMaxY = maxY();

    // Make sure that the given axes' values are fine

    checkAxisValues(mLogAxisX, pMinX, pMaxX);
    checkAxisValues(mLogAxisY, pMinY, pMaxY);

    // Update our axes' values, if needed

    bool xAxisValuesChanged = false;
    bool yAxisValuesChanged = false;

    if (    pForceAxesSetting || pSynchronizeXAxis
        || !qFuzzyCompare(pMinX, oldMinX)
        || !qFuzzyCompare(pMaxX, oldMaxX)) {
        setAxis(QwtPlot::xBottom, pMinX, pMaxX);

        xAxisValuesChanged = true;
    }

    if (    pForceAxesSetting || pSynchronizeYAxis
        || !qFuzzyCompare(pMinY, oldMinY)
        || !qFuzzyCompare(pMaxY, oldMaxY)) {
        setAxis(QwtPlot::yLeft, pMinY, pMaxY);

        yAxisValuesChanged = true;
    }

    // Update our actions and align ourselves with our neighbours (which will
    // result in ourselves, and maybe our neighbours, being replotted, if
    // allowed), in case the axes' values have changed

    if (xAxisValuesChanged || yAxisValuesChanged) {
        mCanDirectPaint = false;
        mDirtyAxes = mDirtyAxes || pForceAxesSetting;

        if (xAxisValuesChanged || yAxisValuesChanged) {
            updateActions();
        }

        if (pSynchronizeAxes) {
            if (   mSynchronizeXAxisAction->isChecked()
                && mSynchronizeYAxisAction->isChecked()) {
                for (auto plot : mNeighbors) {
                    plot->setAxes(pMinX, pMaxX, pMinY, pMaxY,
                                  false, false, false, true, false, false);
                }
            } else if (   xAxisValuesChanged
                       && mSynchronizeXAxisAction->isChecked()) {
                for (auto plot : mNeighbors) {
                    plot->setAxes(pMinX, pMaxX, plot->minY(), plot->maxY(),
                                  false, false, false, true, false, false);
                }
            } else if (   yAxisValuesChanged
                       && mSynchronizeYAxisAction->isChecked()) {
                for (auto plot : mNeighbors) {
                    plot->setAxes(plot->minX(), plot->maxX(), pMinY, pMaxY,
                                  false, false, false, true, false, false);
                }
            }

            alignWithNeighbors(pCanReplot);
        }

        if (pEmitSignal) {
            emit axesChanged(pMinX, pMaxX, pMinY, pMaxY);
        }

        return pCanReplot;
    }

    return false;
}

//==============================================================================

bool GraphPanelPlotWidget::resetAxes()
{
    // Reset our axes by setting their values to either default ones or to some
    // that allow us to see all the graphs
    // Note: mDirtyAxes gets set to true as a result of our call to setAxes(),
    //       yet it should be false once our axes have been reset...

    QRectF dRect = realDataRect();
    bool res = setAxes(dRect.left(), dRect.right(), dRect.top(), dRect.bottom(),
                       true, true, true, true, false, false);

    mDirtyAxes = false;

    return res;
}

//==============================================================================

bool GraphPanelPlotWidget::scaleAxis(Scaling pScaling, bool pCanZoomIn,
                                     bool pCanZoomOut,
                                     const QwtScaleMap &pCanvasMap,
                                     double pPoint, double &pMin, double &pMax,
                                     double pCustomScaling)
{
    // Check whether we can scale the axis and, if so, determine what its new
    // values should be

    if (   (   (    (pScaling == Scaling::ScalingIn)
                ||  (pScaling == Scaling::BigScalingIn)
                || ((pScaling == Scaling::CustomScaling) && (pCustomScaling < 1.0)))
            && pCanZoomIn)
        || (   (    (pScaling == Scaling::ScalingOut)
                ||  (pScaling == Scaling::BigScalingOut)
                || ((pScaling == Scaling::CustomScaling) && (pCustomScaling > 1.0)))
            && pCanZoomOut)) {
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
        case Scaling::NoScaling:
            break;
        case Scaling::ScalingIn:
            range *= ScalingInFactor;

            break;
        case Scaling::BigScalingIn:
            range *= BigScalingInFactor;

            break;
        case Scaling::ScalingOut:
            range *= ScalingOutFactor;

            break;
        case Scaling::BigScalingOut:
            range *= BigScalingOutFactor;

            break;
        case Scaling::CustomScaling:
            range *= pCustomScaling;

            break;
        }

        pMin = qMax(MinAxis, pCanvasMap.invTransform(pPoint-factor*range));
        pMax = qMin(MaxAxis, pCanvasMap.invTransform(pCanvasMap.transform(pMin)+range));
        pMin = pCanvasMap.invTransform(pCanvasMap.transform(pMax)-range);
        // Note: the last statement is in case pMax has been set to MaxAxis, in
        //       which case we need to re-update pMin...

        return true;
    }

    return false;
}

//==============================================================================

void GraphPanelPlotWidget::scaleAxes(const QPoint &pPoint, Scaling pScalingX,
                                     Scaling pScalingY, double pCustomScaling)
{
    // Rescale our X axis, but only if zooming in/out is possible on that axis

    QPointF point = pPoint-plotLayout()->canvasRect().topLeft();

    double newMinX = minX();
    double newMaxX = maxX();
    double newMinY = minY();
    double newMaxY = maxY();

    bool scaledAxisX = scaleAxis(pScalingX, mCanZoomInX, mCanZoomOutX,
                                 canvasMap(QwtPlot::xBottom), point.x(),
                                 newMinX, newMaxX, pCustomScaling);
    bool scaledAxisY = scaleAxis(pScalingY, mCanZoomInY, mCanZoomOutY,
                                 canvasMap(QwtPlot::yLeft), point.y(),
                                 newMinY, newMaxY, pCustomScaling);
    // Note: we want to make both calls to scaleAxis(), hence they are not part
    //       of the if() statement below...

    if (scaledAxisX || scaledAxisY) {
        setAxes(newMinX, newMaxX, newMinY, newMaxY,
                true, true, true, true, false, false);
    }
}

//==============================================================================

void GraphPanelPlotWidget::setTitleAxis(int pAxisId, const QString &pTitleAxis)
{
    // Set the title for our axis

    QwtText newAxisTitle = QwtText(pTitleAxis);
    QFont newAxisTitleFont = newAxisTitle.font();

    newAxisTitleFont.setPointSizeF(1.25*((pAxisId == QwtPlot::xBottom)?
                                            fontSizeAxisX():
                                            fontSizeAxisY()));

    newAxisTitle.setColor(mSurroundingAreaForegroundColor);
    newAxisTitle.setFont(newAxisTitleFont);

    if (newAxisTitle != axisTitle(pAxisId)) {
        setAxisTitle(pAxisId, newAxisTitle);
    }
}

//==============================================================================

void GraphPanelPlotWidget::doUpdateGui(bool pForceAlignment)
{
    // Resize our legend and that of our neighbours

    GraphPanelPlotWidgets selfPlusNeighbors = GraphPanelPlotWidgets() << this << mNeighbors;
    int legendWidth = 0;

    for (auto plot : selfPlusNeighbors)  {
        auto legend = static_cast<GraphPanelPlotLegendWidget *>(plot->legend());

        if (legend != nullptr) {
            legendWidth = qMax(legendWidth, legend->QwtLegend::sizeHint().width());

            if (legend->needScrollBar()) {
                legendWidth = qMax(legendWidth,
                                   legend->QwtLegend::sizeHint().width()+legend->scrollExtent(Qt::Vertical));
            }
        }
    }

    for (auto plot : selfPlusNeighbors) {
        auto legend = static_cast<GraphPanelPlotLegendWidget *>(plot->legend());

        if (legend != nullptr) {
            legend->setSizeHintWidth(legend->needScrollBar()?
                                         legendWidth-legend->scrollExtent(Qt::Vertical):
                                         legendWidth);
        }
    }

    // Reenable updates for our legend
    // Note: see addGraph() for the reasoning behind it...

    if (legend() != nullptr) {
        legend()->setUpdatesEnabled(true);
    }

    // Make sure that we are still properly aligned with our neighbours

    alignWithNeighbors(true, pForceAlignment);
}

//==============================================================================

void GraphPanelPlotWidget::updateGui(bool pSingleShot, bool pForceAlignment)
{
    // Update our GUI, either through a single shot or directly

    if (pSingleShot) {
        QTimer::singleShot(0, this, std::bind(&GraphPanelPlotWidget::doUpdateGui,
                                              this, pForceAlignment));
    } else {
        doUpdateGui(pForceAlignment);
    }
}

//==============================================================================

QPointF GraphPanelPlotWidget::canvasPoint(const QPoint &pPoint) const
{
    // Return the mouse position using canvas coordinates, making sure that they
    // are within our ranges

    return { qMin(maxX(), qMax(minX(), canvasMap(QwtPlot::xBottom).invTransform(pPoint.x()))),
             qMin(maxY(), qMax(minY(), canvasMap(QwtPlot::yLeft).invTransform(pPoint.y()))) };
}

//==============================================================================

void GraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Carry out the action

    switch (mAction) {
    case Action::None:
        // No action, so do nothing

        break;
    case Action::Pan: {
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
                canvasMapY.invTransform(canvasMapY.transform(maxY())-shiftY),
                true, true, true, true, false, false);

        break;
    }
    case Action::ShowCoordinates:
        // Update the point of our overlay widget

        mOverlayWidget->setPoint(pEvent->pos());

        break;
    case Action::Zoom: {
        // Determine our X/Y delta values

        int deltaX = pEvent->pos().x()-mPoint.x();
        int deltaY = pEvent->pos().y()-mPoint.y();

        mPoint = pEvent->pos();

        // Rescale ourselves

        scaleAxes(mOriginPoint,
                  (deltaX != 0)?
                      (deltaX > 0)?
                          Scaling::ScalingIn:
                          Scaling::ScalingOut:
                      Scaling::NoScaling,
                  (deltaY != 0)?
                      (deltaY < 0)?
                          Scaling::ScalingIn:
                          Scaling::ScalingOut:
                      Scaling::NoScaling);

        break;
    }
    case Action::ZoomRegion:
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

    if (!plotLayout()->canvasRect().contains(pEvent->pos())) {
        return;
    }

    // Make sure that we are not already carrying out a action (e.g. we were
    // zooming in/out and then pressed on the left mouse button) and if so, then
    // cancel it by resetting our action

    if (mAction != Action::None) {
        resetAction();

        return;
    }

    // Check which action to can carry out

    if (   (pEvent->modifiers() == Qt::NoModifier)
        && (pEvent->button() == Qt::LeftButton)) {
        // We want to pan, but only do this if we are not completely zoomed out

        if (mCanZoomOutX || mCanZoomOutY) {
            mAction = Action::Pan;

            mPoint = pEvent->pos();
        }
    } else if (   (pEvent->modifiers() == Qt::ShiftModifier)
               && (pEvent->button() == Qt::LeftButton)) {
        // We want to show the coordinates

        mAction = Action::ShowCoordinates;

        mOverlayWidget->setPoint(pEvent->pos());
    } else if (   (pEvent->modifiers() == Qt::NoModifier)
               && (pEvent->button() == Qt::RightButton)) {
        // We want to zoom in/out

        mAction = Action::Zoom;

        mOriginPoint = pEvent->pos();
        mPoint = pEvent->pos();
    } else if (   (pEvent->modifiers() == Qt::ControlModifier)
               && (pEvent->button() == Qt::RightButton)) {
        // We want to zoom a region, but only allow it if we are not already
        // fully zoomed in

        if (mCanZoomInX || mCanZoomInY) {
            mAction = Action::ZoomRegion;

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

    if (mAction == Action::None) {
        return;
    }

    // Finish carrying out the action, if needed

    if (mAction == Action::ZoomRegion) {
        // Retrieve our zoom region

        QRect zoomRegionRect = mOverlayWidget->zoomRegion();

        // Reset our action

        resetAction();

        // Effectively zoom our region, if possible, by updating our axes

        QRectF zoomRegion = QRectF(canvasPoint(zoomRegionRect.topLeft()),
                                   canvasPoint(zoomRegionRect.topLeft()+QPoint(zoomRegionRect.width(), zoomRegionRect.height())));

        if (   !qFuzzyIsNull(zoomRegion.width())
            && !qFuzzyIsNull(zoomRegion.height())) {
            setAxes(zoomRegion.left(), zoomRegion.right(),
                    zoomRegion.bottom(), zoomRegion.top(),
                    true, true, true, true, false, false);
        }
    } else {
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

    // Update our GUI (and that of our neighbours)

    updateGui();
}

//==============================================================================

void GraphPanelPlotWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Handle the wheel mouse button for zooming in/out

    if (   (pEvent->modifiers() == Qt::NoModifier)
        && canvas()->rect().contains(pEvent->pos()-canvas()->pos())) {
        // Make sure that we are not already carrying out an action

        if (mAction == Action::None) {
            int delta = pEvent->delta();
            Scaling scaling = Scaling::NoScaling;

            if (delta > 0) {
                scaling = Scaling::ScalingIn;
            } else if (delta < 0) {
                scaling = Scaling::ScalingOut;
            }

            if (scaling != Scaling::NoScaling) {
                scaleAxes(pEvent->pos(), scaling, scaling);
            }
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

    if (mGraphs.contains(pGraph)) {
        return false;
    }

    // Attach the given graph to ourselves and keep track of it, as well as ask
    // our legend to keep track of it too
    // Note: we temporarily disable updates for our legend, this to avoid some
    //       black areas from quickly appearing on macOS (when adding a graph
    //       that results in the vertical scroll bar being shown). It gets
    //       reenabled in updateGUI()...

    legend()->setUpdatesEnabled(false);

    pGraph->attach(this);

    mGraphs << pGraph;

    mLegend->addGraph(pGraph);

    // Initialise the checked state of the corresponding legend item

    mLegend->setChecked(pGraph, true);

    // To add a graph may affect our GUI (and that of our neighbours), so update
    // it
    // Note: we do it through a single shot since otherwise the new legend label
    //       won't have been added yet and our legend may not be of the right
    //       width (e.g. after having added a graph that would result in the
    //       legend's vertical scroll bar to appear)...

    updateGui(true, true);

    return true;
}

//==============================================================================

bool GraphPanelPlotWidget::removeGraph(GraphPanelPlotGraph *pGraph)
{
    // Check that the given graph is attached to us

    if (!mGraphs.contains(pGraph)) {
        return false;
    }

    // Detach the given graph from ourselves, stop tracking it (and ask our
    // legend to do the same) and delete it
    // Note: see addGraph() when it comes to temporarily disabling updates for
    //       our legend...

    legend()->setUpdatesEnabled(false);

    pGraph->detach();

    mGraphs.removeOne(pGraph);

    mLegend->removeGraph(pGraph);

    delete pGraph;

    // To remove a graph may affect our GUI (and that of our neighbours), so
    // update it

    updateGui(false, isLegendVisible());

    return true;
}

//==============================================================================

int GraphPanelPlotWidget::graphIndex(GraphPanelPlotGraph *pGraph) const
{
    // Return the index of the given graph

    return mGraphs.indexOf(pGraph);
}

//==============================================================================

bool GraphPanelPlotWidget::drawGraphFrom(GraphPanelPlotGraph *pGraph,
                                         quint64 pFrom)
{
    // Direct paint our graph from the given point unless we can't direct paint
    // (due to the axes having been changed), in which case we replot ourselves

    if (mCanDirectPaint) {
        mDirectPainter->drawSeries(pGraph->lastRun(), int(pFrom), -1);

        return false;
    }

    if (mCanReplot) {
        replot();

        mCanReplot = false;
    }

    return true;
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

bool GraphPanelPlotWidget::hasDirtyAxes() const
{
    // Return whether we have dirty axes

    return mDirtyAxes;
}

//==============================================================================

void GraphPanelPlotWidget::getBorderDistances(QwtScaleDraw *pScaleDraw,
                                              const QwtScaleMap &pScaleMap,
                                              const QFont &pFont, int &pStart,
                                              int &pEnd)
{
    // Note: this is based on QwtScaleDraw::getBorderDistHint(), except that we
    //       don't on pScaleDraw's scale map, but use the one provided by
    //       pScaleMap...

    pStart = 0;
    pEnd = 1;

    if (!pScaleDraw->hasComponent(QwtAbstractScaleDraw::Labels)) {
        return;
    }

    QDoubleList ticks = pScaleDraw->scaleDiv().ticks(QwtScaleDiv::MajorTick);

    if (ticks.isEmpty()) {
        return;
    }

    // Find the ticks, that are mapped to the borders (minTick is the tick that
    // is mapped to the top/left-most position in widget coordinates)

    double minTick = ticks[0];
    double maxTick = minTick;
    double minPos = pScaleMap.transform(minTick);
    double maxPos = minPos;

    for (int i = 1, iMax = ticks.count(); i < iMax; ++i) {
        double tickPos = pScaleMap.transform(ticks[i]);

        if (tickPos < minPos) {
            minTick = ticks[i];
            minPos = tickPos;
        }

        if (tickPos > pScaleMap.transform(maxTick)) {
            maxTick = ticks[i];
            maxPos = tickPos;
        }
    }

    double start = 0.0;
    double end = 0.0;

    if (pScaleDraw->orientation() == Qt::Horizontal) {
        start = -pScaleDraw->labelRect(pFont, minTick).left();
        start -= qAbs(minPos-pScaleMap.p1());

        end = pScaleDraw->labelRect(pFont, maxTick).right();
        end -= qAbs(maxPos-pScaleMap.p2());
    } else {
        start = -pScaleDraw->labelRect(pFont, minTick).top();
        start -= qAbs(minPos-qRound(pScaleMap.p2()));

        end = pScaleDraw->labelRect(pFont, maxTick).bottom();
        end -= qAbs(maxPos-pScaleMap.p1());
    }

    if (start < 0.0) {
        start = 0.0;
    }

    if (end < 0.0) {
        end = 0.0;
    }

    pStart = qCeil(start);
    pEnd = qCeil(end);
}

//==============================================================================

void GraphPanelPlotWidget::alignWithNeighbors(bool pCanReplot,
                                              bool pForceAlignment)
{
    // Align ourselves with our neighbours by taking into account the size it
    // takes to draw the Y axis and, if any, its corresponding title (including
    // the gap between the Y axis and its corresponding title)

    bool forceAlignment =    pForceAlignment
                          || mSynchronizeXAxisAction->isChecked()
                          || mSynchronizeYAxisAction->isChecked();

    GraphPanelPlotWidgets selfPlusNeighbors = GraphPanelPlotWidgets() << this << mNeighbors;
    int oldMinBorderDistStartX = 0;
    int oldMinBorderDistEndX = 0;
    int newMinBorderDistStartX = 0;
    int newMinBorderDistEndX = 0;
    double oldMinExtentY = axisWidget(QwtPlot::yLeft)->scaleDraw()->minimumExtent();
    double newMinExtentY = 0.0;

    axisWidget(QwtPlot::xBottom)->getMinBorderDist(oldMinBorderDistStartX, oldMinBorderDistEndX);

    for (auto plot : selfPlusNeighbors) {
        // Determine how much space we should have directly to the left and
        // right of the X axis

        QwtScaleWidget *xScaleWidget = plot->axisWidget(QwtPlot::xBottom);
        int defaultMinBorderDistStartX;
        int defaultMinBorderDistEndX;
        int customMinBorderDistStartX;
        int customMinBorderDistEndX;

        getBorderDistances(xScaleWidget->scaleDraw(), canvasMap(QwtPlot::xBottom), xScaleWidget->font(), defaultMinBorderDistStartX, defaultMinBorderDistEndX);
        xScaleWidget->scaleDraw()->getBorderDistHint(xScaleWidget->font(), customMinBorderDistStartX, customMinBorderDistEndX);

        newMinBorderDistStartX = qMax(newMinBorderDistStartX,
                                      qMax(defaultMinBorderDistStartX,
                                           customMinBorderDistStartX));
        newMinBorderDistEndX = qMax(newMinBorderDistEndX,
                                    qMax(defaultMinBorderDistEndX,
                                         customMinBorderDistEndX));

        // Determine how much space we should have to the left of the Y axis

        QwtScaleWidget *yScaleWidget = plot->axisWidget(QwtPlot::yLeft);
        QwtScaleDraw *yScaleDraw = yScaleWidget->scaleDraw();

        yScaleDraw->setMinimumExtent(0.0);

        plot->updateAxes();
        // Note: this ensures that our major ticks (which are used to compute
        //       the extent) are up to date...

        double minExtentY =  yScaleDraw->extent(yScaleWidget->font())
                            +(plot->titleAxisY().isEmpty()?
                                  0.0:
                                  yScaleWidget->spacing()+yScaleWidget->title().textSize().height());

        newMinExtentY = qMax(newMinExtentY, minExtentY);
    }

    bool xAlignmentChanged =     forceAlignment
                             || (newMinBorderDistStartX != oldMinBorderDistStartX)
                             || (newMinBorderDistEndX != oldMinBorderDistEndX);
    bool yAlignmentChanged =     forceAlignment
                             || !qFuzzyCompare(newMinExtentY, oldMinExtentY);
    bool alignmentChanged = xAlignmentChanged || yAlignmentChanged;

    for (auto plot : selfPlusNeighbors) {
        auto xScaleWidget = static_cast<GraphPanelPlotScaleWidget *>(plot->axisWidget(QwtPlot::xBottom));

        xScaleWidget->setMinBorderDist(newMinBorderDistStartX, newMinBorderDistEndX);

        auto yScaleWidget = static_cast<GraphPanelPlotScaleWidget *>(plot->axisWidget(QwtPlot::yLeft));

        yScaleWidget->scaleDraw()->setMinimumExtent( newMinExtentY
                                                    -(plot->titleAxisY().isEmpty()?
                                                          0.0:
                                                          yScaleWidget->spacing()+yScaleWidget->title().textSize().height()));

        if (pCanReplot) {
            if (alignmentChanged) {
                if (xAlignmentChanged) {
                    xScaleWidget->updateLayout();
                }

                if (yAlignmentChanged) {
                    yScaleWidget->updateLayout();
                }

                plot->replot();
            } else if (plot == this) {
                replot();
            }
        }
    }
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

    static const QString ImageBmp             = "image/bmp";
    static const QString ImageJpeg            = "image/jpeg";
    static const QString ImagePng             = "image/png";
    static const QString ImagePortableBitmap  = "image/x-portable-bitmap";
    static const QString ImagePortableGraymap = "image/x-portable-graymap";
    static const QString ImagePortablePixmap  = "image/x-portable-pixmap";
    static const QString ImageXbitmap         = "image/x-xbitmap";
    static const QString ImageXpixmap         = "image/x-xpixmap";

    QString pdfFilter = tr("PDF File - Portable Document Format (*.pdf)");
    QStringList filters = { pdfFilter,
                            tr("SVG File - Scalable Vector Graphics (*.svg)") };

    for (const auto &supportedMimeType : QImageWriter::supportedMimeTypes()) {
        if (supportedMimeType == ImageBmp) {
            filters << tr("BMP File - Windows Bitmap (*.bmp)");
        } else if (supportedMimeType == ImageJpeg) {
            filters << tr("JPEG File - Joint Photographic Experts Group (*.jpg)");
        } else if (supportedMimeType == ImagePng) {
            filters << tr("PNG File - Portable Network Graphics (*.png)");
        } else if (supportedMimeType == ImagePortableBitmap) {
            filters << tr("PBM File - Portable Bitmap (*.pbm)");
        } else if (supportedMimeType == ImagePortableGraymap) {
            filters << tr("PGM File - Portable Graymap (*.pgm)");
        } else if (supportedMimeType == ImagePortablePixmap) {
            filters << tr("PPM File - Portable Pixmap (*.ppm)");
        } else if (supportedMimeType == ImageXbitmap) {
            filters << tr("XBM File - X11 Bitmap (*.xbm)");
        } else if (supportedMimeType == ImageXpixmap) {
            filters << tr("XPM File - X11 Pixmap (*.xpm)");
        }
    }

    std::sort(filters.begin(), filters.end());

    QString fileName = Core::getSaveFileName(tr("Export To"), filters, &pdfFilter);

    if (!fileName.isEmpty()) {
        static int Dpi = 85;
        static double InToMmPerDpi = 25.4/Dpi;

        if (QFileInfo(fileName).completeSuffix().isEmpty()) {
            QwtPlotRenderer().renderDocument(this, fileName, "pdf", QSizeF(width()*InToMmPerDpi, height()*InToMmPerDpi), Dpi);
        } else {
            QwtPlotRenderer().renderDocument(this, fileName, QSizeF(width()*InToMmPerDpi, height()*InToMmPerDpi), Dpi);
        }
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

        if (   !qFuzzyCompare(newMinX, oldMinX)
            || !qFuzzyCompare(newMaxX, oldMaxX)
            || !qFuzzyCompare(newMinY, oldMinY)
            || !qFuzzyCompare(newMaxY, oldMaxY)) {
            setAxes(newMinX, newMaxX, newMinY, newMaxY,
                    true, true, true, true, false, false);
        }
    }
}

//==============================================================================

void GraphPanelPlotWidget::zoomIn()
{
    // Zoom in by scaling our two axes around the point where the context menu
    // was shown

    scaleAxes(mOriginPoint, Scaling::BigScalingIn, Scaling::BigScalingIn);
}

//==============================================================================

void GraphPanelPlotWidget::zoomOut()
{
    // Zoom out by scaling our two axes around the point where the context menu
    // was shown

    scaleAxes(mOriginPoint, Scaling::BigScalingOut, Scaling::BigScalingOut);
}

//==============================================================================

void GraphPanelPlotWidget::resetZoom()
{
    // Reset the zoom level by resetting our axes

    resetAxes();
}

//==============================================================================

} // namespace GraphPanelWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

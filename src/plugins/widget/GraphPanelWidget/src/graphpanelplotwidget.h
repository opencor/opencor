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

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "graphpanelwidgetglobal.h"

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_legend.h"
    #include "qwt_plot.h"
    #include "qwt_plot_curve.h"
    #include "qwt_scale_draw.h"
    #include "qwt_scale_widget.h"
    #include "qwt_symbol.h"
    #include "qwt_text.h"
#include "qwtend.h"

//==============================================================================

class QMenu;

//==============================================================================

class QwtLegendLabel;
class QwtPlotDirectPainter;
class QwtPlotGrid;

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

static const double DefaultMinAxis    =    0.0;
static const double DefaultMinLogAxis =    1.0;
static const double DefaultMaxAxis    = 1000.0;

//==============================================================================

static const QColor DarkBlue  = QColor::fromRgb(0, 114, 189);
static const QColor Orange    = QColor::fromRgb(217, 83, 25);
static const QColor Yellow    = QColor::fromRgb(237, 177, 32);
static const QColor Purple    = QColor::fromRgb(126, 47, 142);
static const QColor Green     = QColor::fromRgb(119, 172, 48);
static const QColor LightBlue = QColor::fromRgb(77, 190, 238);
static const QColor Red       = QColor::fromRgb(162, 20, 47);

//==============================================================================

static const bool DefaultGraphSelected                = true;
static const auto DefaultGraphTitle                   = QStringLiteral("");
static const Qt::PenStyle DefaultGraphLineStyle       = Qt::SolidLine;
static const int DefaultGraphLineWidth                = 2;
static const QColor DefaultGraphLineColor             = DarkBlue;
static const QwtSymbol::Style DefaultGraphSymbolStyle = QwtSymbol::NoSymbol;
static const int DefaultGraphSymbolSize               = 8;
static const QColor DefaultGraphSymbolColor           = DarkBlue;
static const bool DefaultGraphSymbolFilled            = true;
static const QColor DefaultGraphSymbolFillColor       = Qt::white;

//==============================================================================

class GRAPHPANELWIDGET_EXPORT GraphPanelPlotGraphProperties
{
public:
    explicit GraphPanelPlotGraphProperties(bool pSelected = DefaultGraphSelected,
                                           const QString &pTitle = DefaultGraphTitle,
                                           const Qt::PenStyle &pLineStyle = DefaultGraphLineStyle,
                                           int pLineWidth = DefaultGraphLineWidth,
                                           const QColor &pLineColor = DefaultGraphLineColor,
                                           const QwtSymbol::Style &pSymbolStyle = DefaultGraphSymbolStyle,
                                           int pSymbolSize = DefaultGraphSymbolSize,
                                           const QColor &pSymbolColor = DefaultGraphSymbolColor,
                                           bool pSymbolFilled = DefaultGraphSymbolFilled,
                                           const QColor &pSymbolFillColor = DefaultGraphSymbolFillColor);

    bool isSelected() const;

    QString title() const;

    Qt::PenStyle lineStyle() const;
    int lineWidth() const;
    QColor lineColor() const;

    QwtSymbol::Style symbolStyle() const;
    int symbolSize() const;
    QColor symbolColor() const;
    bool symbolFilled() const;
    QColor symbolFillColor() const;

private:
    bool mSelected;

    QString mTitle;

    Qt::PenStyle mLineStyle;
    int mLineWidth;
    QColor mLineColor;

    QwtSymbol::Style mSymbolStyle;
    int mSymbolSize;
    QColor mSymbolColor;
    bool mSymbolFilled;
    QColor mSymbolFillColor;
};

//==============================================================================

class GraphPanelPlotGraph;

//==============================================================================

class GraphPanelPlotGraphRun : public QwtPlotCurve
{
public:
    explicit GraphPanelPlotGraphRun(GraphPanelPlotGraph *pOwner);

    GraphPanelPlotGraph * owner() const;

    void setRawSamples(const double *pDataX, const double *pDataY, int pSize);

protected:
    void drawLines(QPainter *pPainter, const QwtScaleMap &pMapX,
                   const QwtScaleMap &pMapY, const QRectF &pCanvasRect,
                   int pFrom, int pTo) const override;
    void drawSymbols(QPainter *pPainter, const QwtSymbol &pSymbol,
                     const QwtScaleMap &pMapX, const QwtScaleMap &pMapY,
                     const QRectF &pCanvasRect, int pFrom, int pTo) const override;

private:
    GraphPanelPlotGraph *mOwner;

    int mSize = 0;
    QList<QPair<int, int>> mValidData;
};

//==============================================================================

using GraphPanelPlotGraphRuns = QList<GraphPanelPlotGraphRun *>;

//==============================================================================

class GraphPanelPlotWidget;
class GraphPanelWidget;

//==============================================================================

class GRAPHPANELWIDGET_EXPORT GraphPanelPlotGraph
{
public:
    explicit GraphPanelPlotGraph(void *pParameterX, void *pParameterY,
                                 GraphPanelWidget *pOwner);
    explicit GraphPanelPlotGraph(GraphPanelWidget *pOwner);

    bool isValid() const;

    void attach(GraphPanelPlotWidget *pPlot);
    void detach();

    GraphPanelPlotWidget * plot() const;

    int runsCount() const;

    void addRun();
    void removeRuns();
    GraphPanelPlotGraphRun * lastRun() const;

    bool isSelected() const;
    void setSelected(bool pSelected);

    QString fileName() const;
    void setFileName(const QString &pFileName);

    void * parameterX() const;
    void setParameterX(void *pParameterX);

    void * parameterY() const;
    void setParameterY(void *pParameterY);

    QColor color() const;

    const QPen & pen() const;
    void setPen(const QPen &pPen);

    const QwtSymbol * symbol() const;
    void setSymbol(const QwtSymbol::Style &pStyle, const QBrush &pBrush,
                   const QPen &pPen, const QSize &pSize);

    QString title() const;
    void setTitle(const QString &pTitle);

    bool isVisible() const;
    void setVisible(bool pVisible);

    bool hasData() const;
    quint64 dataSize() const;

    QwtSeriesData<QPointF> *data(int pRun = -1) const;
    void setData(double *pDataX, double *pDataY, quint64 pSize, int pRun = -1);

    QRectF boundingRect();
    QRectF boundingLogRect();

private:
    bool mSelected = true;

    QString mFileName;

    void *mParameterX;
    void *mParameterY;

    QColor mColor;

    QRectF mBoundingRect;
    QMap<GraphPanelPlotGraphRun *, QRectF> mBoundingRects;

    QRectF mBoundingLogRect;
    QMap<GraphPanelPlotGraphRun *, QRectF> mBoundingLogRects;

    GraphPanelPlotWidget *mPlot = nullptr;

    GraphPanelPlotGraphRun *mDummyRun = nullptr;
    GraphPanelPlotGraphRuns mRuns;
};

//==============================================================================

using GraphPanelPlotGraphs = QList<GraphPanelPlotGraph *>;

//==============================================================================

class GraphPanelPlotOverlayWidget : public QWidget
{
public:
    explicit GraphPanelPlotOverlayWidget(GraphPanelPlotWidget *pParent);

    void setOriginPoint(const QPoint &pOriginPoint);
    void setPoint(const QPoint &pPoint);

    QRect zoomRegion() const;

protected:
    void paintEvent(QPaintEvent *pEvent) override;

private:
    enum class Position {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    GraphPanelPlotWidget *mOwner;

    QPoint mOriginPoint;
    QPoint mPoint;

    QPoint optimizedPoint(const QPoint &pPoint) const;

    void drawCoordinates(QPainter *pPainter, const QPoint &pPoint,
                         const QColor &pBackgroundColor,
                         const QColor &pForegroundColor, int pLineWidth,
                         Position pPosition, bool pCanMovePosition = true);
};

//==============================================================================

class GraphPanelPlotScaleDraw : public QwtScaleDraw
{
public:
    void retranslateUi();

protected:
    QwtText label(double pValue) const override;
};

//==============================================================================

class GraphPanelPlotScaleWidget : public QwtScaleWidget
{
public:
    void updateLayout();
};

//==============================================================================

class GraphPanelPlotLegendWidget : public QwtLegend
{
    Q_OBJECT

public:
    explicit GraphPanelPlotLegendWidget(GraphPanelPlotWidget *pParent);

    bool isActive() const;
    void setActive(bool pActive);

    bool isEmpty() const override;

    void setChecked(GraphPanelPlotGraph *pGraph, bool pChecked);

    void setFontSize(int pFontSize);
    void setBackgroundColor(const QColor &pBackgroundColor);
    void setForegroundColor(const QColor &pForegroundColor);

    void renderLegend(QPainter *pPainter, const QRectF &pRect,
                      bool pFillBackground) const override;

    void setSizeHintWidth(int pSizeHintWidth);

    QSize sizeHint() const override;

    void addGraph(GraphPanelPlotGraph *pGraph);
    void removeGraph(GraphPanelPlotGraph *pGraph);

    bool needScrollBar() const;

protected:
    void updateWidget(QWidget *pWidget, const QwtLegendData &pLegendData) override;

private:
    GraphPanelPlotWidget *mOwner;

    bool mActive = false;

    int mFontSize;
    QColor mBackgroundColor;
    QColor mForegroundColor;

    QMap<GraphPanelPlotGraph *, QwtLegendLabel *> mLegendLabels;

    int mSizeHintWidth = 0;

signals:
    void graphToggled(GraphPanelPlotGraph *pGraph);

private slots:
    void legendChecked(const QVariant &pItemInfo);
};

//==============================================================================

class GraphPanelWidget;

//==============================================================================

using GraphPanelPlotWidgets = QList<GraphPanelPlotWidget *>;

//==============================================================================

class GRAPHPANELWIDGET_EXPORT GraphPanelPlotWidget : public QwtPlot,
                                                     public Core::CommonWidget
{
    Q_OBJECT

public:
    enum class Action {
        None,
        Pan,
        ShowCoordinates,
        Zoom,
        ZoomRegion
    };

    enum class Optimization {
        Default,
        Linear,
        Logarithmic
    };

    explicit GraphPanelPlotWidget(const GraphPanelPlotWidgets &pNeighbors,
                                  QAction *pSynchronizeXAxisAction,
                                  QAction *pSynchronizeYAxisAction,
                                  GraphPanelWidget *pParent);
    ~GraphPanelPlotWidget() override;

    void retranslateUi() override;

    void setActive(bool pActive);

    GraphPanelPlotGraphs graphs() const;

    bool addGraph(GraphPanelPlotGraph *pGraph);
    bool removeGraph(GraphPanelPlotGraph *pGraph);

    int graphIndex(GraphPanelPlotGraph *pGraph) const;

    bool hasData() const;

    bool dataRect(QRectF &pDataRect) const;
    bool dataLogRect(QRectF &pDataLogRect) const;

    bool isOptimizedAxes() const;

    void optimizeAxisX(double &pMin, double &pMax,
                       Optimization pOptimization = Optimization::Default);
    void optimizeAxisY(double &pMin, double &pMax,
                       Optimization pOptimization = Optimization::Default);

    double minX() const;
    double maxX() const;
    double minY() const;
    double maxY() const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &pBackgroundColor);

    int fontSize() const;
    void setFontSize(int pFontSize, bool pForceSetting = false);

    QColor foregroundColor() const;
    void setForegroundColor(const QColor &pForegroundColor);

    Qt::PenStyle gridLinesStyle() const;
    void setGridLinesStyle(const Qt::PenStyle &pGridLinesStyle);

    int gridLinesWidth() const;
    void setGridLinesWidth(int pGridLinesWidth);

    QColor gridLinesColor() const;
    void setGridLinesColor(const QColor &pGridLinesColor);

    bool isLegendActive() const;
    void setLegendActive(bool pLegendActive);

    void setLegendWidth(int pLegendWidth);

    Qt::PenStyle pointCoordinatesStyle() const;
    void setPointCoordinatesStyle(const Qt::PenStyle &pPointCoordinatesStyle);

    int pointCoordinatesWidth() const;
    void setPointCoordinatesWidth(int pPointCoordinatesWidth);

    QColor pointCoordinatesColor() const;
    void setPointCoordinatesColor(const QColor &pPointCoordinatesColor);

    QColor pointCoordinatesFontColor() const;
    void setPointCoordinatesFontColor(const QColor &pPointCoordinatesFontColor);

    QColor surroundingAreaBackgroundColor() const;
    void setSurroundingAreaBackgroundColor(const QColor &pSurroundingAreaBackgroundColor);

    QColor surroundingAreaForegroundColor() const;
    void setSurroundingAreaForegroundColor(const QColor &pSurroundingAreaForegroundColor);

    void setTitle(const QString &pTitle);

    bool logAxisX() const;
    void setLogAxisX(bool pLogAxisX);

    QString titleAxisX() const;
    void setTitleAxisX(const QString &pTitleAxisX);

    bool logAxisY() const;
    void setLogAxisY(bool pLogAxisY);

    QString titleAxisY() const;
    void setTitleAxisY(const QString &pTitleAxisY);

    Qt::PenStyle zoomRegionStyle() const;
    void setZoomRegionStyle(const Qt::PenStyle &pZoomRegionStyle);

    int zoomRegionWidth() const;
    void setZoomRegionWidth(int pZoomRegionWidth);

    QColor zoomRegionColor() const;
    void setZoomRegionColor(const QColor &pZoomRegionColor);

    QColor zoomRegionFontColor() const;
    void setZoomRegionFontColor(const QColor &pZoomRegionFontColor);

    bool zoomRegionFilled() const;
    void setZoomRegionFilled(bool pZoomRegionFilled);

    QColor zoomRegionFillColor() const;
    void setZoomRegionFillColor(const QColor &pZoomRegionFillColor);

    void setDefaultAxesValues(double pDefaultMinX, double pDefaultMaxX,
                              double pDefaultMinLogX, double pDefaultMaxLogX,
                              double pDefaultMinY, double pDefaultMaxY,
                              double pDefaultMinLogY, double pDefaultMaxLogY);

    bool setAxes(double pMinX, double pMaxX, double pMinY, double pMaxY,
                 bool pSynchronizeAxes, bool pCanReplot, bool pEmitSignal,
                 bool pForceAxesSetting, bool pSynchronizeXAxis,
                 bool pSynchronizeYAxis);

    bool drawGraphFrom(GraphPanelPlotGraph *pGraph, quint64 pFrom);

    GraphPanelPlotWidgets neighbors() const;

    void addNeighbor(GraphPanelPlotWidget *pPlot);
    void removeNeighbor(GraphPanelPlotWidget *pPlot);

    bool hasDirtyAxes() const;

    Action action() const;

    bool canZoomInX() const;
    bool canZoomOutX() const;
    bool canZoomInY() const;
    bool canZoomOutY() const;

    QPointF canvasPoint(const QPoint &pPoint) const;

    void updateGui(bool pSingleShot = false, bool pForceAlignment = false);

protected:
    void changeEvent(QEvent *pEvent) override;
    bool event(QEvent *pEvent) override;
    bool eventFilter(QObject *pObject, QEvent *pEvent) override;
    void mouseMoveEvent(QMouseEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *pEvent) override;
    void mouseReleaseEvent(QMouseEvent *pEvent) override;
    void paintEvent(QPaintEvent *pEvent) override;
    void resizeEvent(QResizeEvent *pEvent) override;
    void wheelEvent(QWheelEvent *pEvent) override;

private:
    enum class Scaling {
        NoScaling,
        ScalingIn,
        BigScalingIn,
        ScalingOut,
        BigScalingOut,
        CustomScaling
    };

    GraphPanelWidget *mOwner;

    QwtPlotDirectPainter *mDirectPainter;

    QColor mBackgroundColor;
    QColor mForegroundColor;

    Qt::PenStyle mPointCoordinatesStyle = Qt::DashLine;
    int mPointCoordinatesWidth = 1;
    QColor mPointCoordinatesColor;
    QColor mPointCoordinatesFontColor = Qt::white;

    QColor mSurroundingAreaBackgroundColor;
    QColor mSurroundingAreaForegroundColor;

    Qt::PenStyle mZoomRegionStyle = Qt::SolidLine;
    int mZoomRegionWidth = 1;
    QColor mZoomRegionColor;
    QColor mZoomRegionFontColor = Qt::white;
    bool mZoomRegionFilled = true;
    QColor mZoomRegionFillColor;

    bool mLogAxisX = false;
    bool mLogAxisY = false;

    GraphPanelPlotGraphs mGraphs;

    QColor mEnabledBackgroundColor;
    QColor mEnabledForegroundColor;

    QColor mEnabledSurroundingAreaBackgroundColor;
    QColor mEnabledSurroundingAreaForegroundColor;

    QColor mEnabledGridLinesColor;

    QList<QPen> mEnabledGraphPens;
    QList<QBrush> mEnabledGraphSymbolBrushes;
    QList<QPen> mEnabledGraphSymbolPens;

    Action mAction = Action::None;

    QPoint mOriginPoint;
    QPoint mPoint;

    GraphPanelPlotOverlayWidget *mOverlayWidget;

    GraphPanelPlotLegendWidget *mLegend;

    bool mCanDirectPaint = true;
    bool mCanReplot = true;

    bool mCanZoomInX = true;
    bool mCanZoomOutX = true;
    bool mCanZoomInY = true;
    bool mCanZoomOutY = true;

    bool mNeedContextMenu = false;
    QMenu *mContextMenu;

    bool mCanUpdateActions = true;

    QAction *mExportToAction;
    QAction *mCopyToClipboardAction;
    QAction *mSynchronizeXAxisAction;
    QAction *mSynchronizeYAxisAction;
    QAction *mGraphPanelSettingsAction;
    QAction *mGraphsSettingsAction;
    QAction *mLegendAction;
    QAction *mLogarithmicXAxisAction;
    QAction *mLogarithmicYAxisAction;
    QAction *mCustomAxesAction;
    QAction *mZoomInAction;
    QAction *mZoomOutAction;
    QAction *mResetZoomAction;

    GraphPanelPlotScaleDraw *mAxisX;
    GraphPanelPlotScaleDraw *mAxisY;

    QwtPlotGrid *mGrid;

    bool mOptimizedAxisX = true;
    bool mOptimizedAxisY = true;

    double mDefaultMinX = DefaultMinAxis;
    double mDefaultMaxX = DefaultMaxAxis;
    double mDefaultMinY = DefaultMinAxis;
    double mDefaultMaxY = DefaultMaxAxis;

    double mDefaultMinLogX = DefaultMinLogAxis;
    double mDefaultMaxLogX = DefaultMaxAxis;
    double mDefaultMinLogY = DefaultMinLogAxis;
    double mDefaultMaxLogY = DefaultMaxAxis;

    GraphPanelPlotWidgets mNeighbors;

    bool mDirtyAxes = false;

    void checkAxisValues(bool pLogAxis, double &pMin, double &pMax);

    void updateActions();

    void resetAction();

    QRectF realDataRect();

    void optimizeAxis(int pAxisId, double &pMin, double &pMax,
                      Optimization pOptimization);

    void setAxis(int pAxisId, double pMin, double pMax);

    bool resetAxes();

    bool scaleAxis(Scaling pScaling, bool pCanZoomIn, bool pCanZoomOut,
                   const QwtScaleMap &pCanvasMap, double pPoint,
                   double &pMin, double &pMax, double pCustomScaling);
    void scaleAxes(const QPoint &pPoint, Scaling pScalingX, Scaling pScalingY,
                   double pCustomScaling = 0.0);

    void setTitleAxis(int pAxisId, const QString &pTitleAxis);

    void getBorderDistances(QwtScaleDraw *pScaleDraw,
                            const QwtScaleMap &pScaleMap, const QFont &pFont,
                            int &pStart, int &pEnd);

    void alignWithNeighbors(bool pCanReplot, bool pForceAlignment = false);

signals:
    void axesChanged(double pMinX, double pMaxX, double pMinY, double pMaxY);

    void graphPanelSettingsRequested();
    void graphsSettingsRequested();

    void graphToggled(GraphPanelPlotGraph *pGraph);

    void legendToggled();

    void logarithmicXAxisToggled();
    void logarithmicYAxisToggled();

private slots:
    void doUpdateGui(bool pForceAlignment);

    void cannotUpdateActions();

    void exportTo();
    void copyToClipboard();
    void customAxes();
    void zoomIn();
    void zoomOut();
    void resetZoom();
};

//==============================================================================

} // namespace GraphPanelWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

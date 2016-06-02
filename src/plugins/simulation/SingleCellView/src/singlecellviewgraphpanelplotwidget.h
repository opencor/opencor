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

#pragma once

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_scale_draw.h"
#include "qwt_text.h"

//==============================================================================

class QMenu;

//==============================================================================

class QwtPlotDirectPainter;

//==============================================================================

namespace Ui {
    class SingleCellViewGraphPanelPlotWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntimeParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

static const double DefMinAxis =    0.0;
static const double DefMaxAxis = 1000.0;

static const QRectF DefPlotRect = QRectF(DefMinAxis, DefMinAxis,
                                         DefMaxAxis, DefMaxAxis);

//==============================================================================

class SingleCellViewGraphPanelPlotGraph : public QwtPlotCurve
{
public:
    explicit SingleCellViewGraphPanelPlotGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX = 0,
                                               CellMLSupport::CellmlFileRuntimeParameter *pParameterY = 0);

    bool isValid() const;

    bool isSelected() const;
    void setSelected(const bool &pSelected);

    QString fileName() const;
    void setFileName(const QString &pFileName);

    CellMLSupport::CellmlFileRuntimeParameter * parameterX() const;
    void setParameterX(CellMLSupport::CellmlFileRuntimeParameter *pParameterX);

    CellMLSupport::CellmlFileRuntimeParameter * parameterY() const;
    void setParameterY(CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

private:
    bool mSelected;

    QString mFileName;

    CellMLSupport::CellmlFileRuntimeParameter *mParameterX;
    CellMLSupport::CellmlFileRuntimeParameter *mParameterY;
};

//==============================================================================

typedef QList<SingleCellViewGraphPanelPlotGraph *> SingleCellViewGraphPanelPlotGraphs;

//==============================================================================

class SingleCellViewGraphPanelPlotWidget;

//==============================================================================

class SingleCellViewGraphPanelPlotOverlayWidget : public QWidget
{
public:
    explicit SingleCellViewGraphPanelPlotOverlayWidget(SingleCellViewGraphPanelPlotWidget *pParent);

    void setOriginPoint(const QPoint &pOriginPoint);
    void setPoint(const QPoint &pPoint);

    QRect zoomRegion() const;

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    enum Location {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    SingleCellViewGraphPanelPlotWidget *mOwner;

    QPoint mOriginPoint;
    QPoint mPoint;

    QPoint optimisedPoint(const QPoint &pPoint) const;

    void drawCoordinates(QPainter *pPainter, const QPoint &pPoint,
                         const QColor &pBackgroundColor,
                         const QColor &pForegroundColor,
                         const Location &pLocation,
                         const bool &pCanMoveLocation = true);
};

//==============================================================================

class SingleCellViewGraphPanelPlotScaleDraw : public QwtScaleDraw
{
public:
    void retranslateUi();

protected:
    virtual QwtText label(double pValue) const;
};

//==============================================================================

typedef QList<SingleCellViewGraphPanelPlotWidget *> SingleCellViewGraphPanelPlotWidgets;

//==============================================================================

class SingleCellViewGraphPanelPlotWidget : public QwtPlot,
                                           public Core::CommonWidget
{
    Q_OBJECT

    friend class SingleCellViewGraphPanelPlotOverlayWidget;

public:
    explicit SingleCellViewGraphPanelPlotWidget(const SingleCellViewGraphPanelPlotWidgets &pNeighbors,
                                                QWidget *pParent);
    ~SingleCellViewGraphPanelPlotWidget();

    virtual void retranslateUi();

    SingleCellViewGraphPanelPlotGraphs graphs() const;

    bool addGraph(SingleCellViewGraphPanelPlotGraph *pGraph);
    bool removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph);

    QRectF dataRect() const;

    void optimiseAxisX(double &pMin, double &pMax) const;
    void optimiseAxisY(double &pMin, double &pMax) const;

    double minX() const;
    double maxX() const;
    double minY() const;
    double maxY() const;

    bool setAxes(double pMinX, double pMaxX, double pMinY, double pMaxY,
                 const bool &pCanReplot = true);
    bool setAxes(const QRectF &pAxesRect, const bool &pCanReplot = true);

    bool resetAxes(const bool &pCanReplot = true);

    void drawGraphFrom(SingleCellViewGraphPanelPlotGraph *pGraph,
                       const qulonglong &pFrom);

    void replotNow();

    SingleCellViewGraphPanelPlotWidgets neighbors() const;

    void addNeighbor(SingleCellViewGraphPanelPlotWidget *pPlot);
    void removeNeighbor(SingleCellViewGraphPanelPlotWidget *pPlot);

    void alignWithNeighbors(const bool &pCanReplot,
                            const bool &pForceAlignment = false);
    void forceAlignWithNeighbors();

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseReleaseEvent(QMouseEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    enum Action {
        None,
        Pan,
        ShowCoordinates,
        Zoom,
        ZoomRegion
    };

    Ui::SingleCellViewGraphPanelPlotWidget *mGui;

    QwtPlotDirectPainter *mDirectPainter;

    SingleCellViewGraphPanelPlotGraphs mGraphs;

    Action mAction;

    QPoint mOriginPoint;
    QPoint mPoint;

    SingleCellViewGraphPanelPlotOverlayWidget *mOverlayWidget;

    bool mCanDirectPaint;

    bool mCanZoomInX;
    bool mCanZoomOutX;
    bool mCanZoomInY;
    bool mCanZoomOutY;

    bool mNeedContextMenu;
    QMenu *mContextMenu;

    SingleCellViewGraphPanelPlotScaleDraw *mAxisX;
    SingleCellViewGraphPanelPlotScaleDraw *mAxisY;

    SingleCellViewGraphPanelPlotWidgets mNeighbors;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void checkAxisValues(double &pMin, double &pMax);
    void checkAxesValues(double &pMinX, double &pMaxX,
                         double &pMinY, double &pMaxY);

    void updateActions();

    Action action() const;

    void resetAction();

    bool canZoomInX() const;
    bool canZoomOutX() const;
    bool canZoomInY() const;
    bool canZoomOutY() const;

    void optimiseAxis(const int &pAxisId, double &pMin, double &pMax) const;

    QRectF optimisedRect(const QRectF &pAxes) const;

    void setAxis(const int &pAxis, double pMin, double pMax);

    bool scaleAxis(const double &pScalingFactor,
                   const bool &pCanZoomIn, const bool &pCanZoomOut,
                   const double pOriginPoint, double &pMin, double &pMax);
    void scaleAxes(const QPoint &pPoint,
                   const double &pScalingFactorX,
                   const double &pScalingFactorY);

    QPointF canvasPoint(const QPoint &pPoint,
                        const bool pNeedOffset = true) const;

Q_SIGNALS:
    void axesChanged(const double &pMinX, const double &pMaxX,
                     const double &pMinY, const double &pMaxY);

private Q_SLOTS:
    void on_actionCopyToClipboard_triggered();

    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();

    void on_actionResetZoom_triggered();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

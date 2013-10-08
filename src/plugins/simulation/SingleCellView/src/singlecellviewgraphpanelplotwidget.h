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

#ifndef SINGLECELLVIEWGRAPHPANELPLOTWIDGET_H
#define SINGLECELLVIEWGRAPHPANELPLOTWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_scale_draw.h"

//==============================================================================

class QAction;
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

class SingleCellViewGraphPanelPlotWidget;

//==============================================================================

class SingleCellViewGraphPanelPlotOverlayWidget : public QWidget
{
public:
    explicit SingleCellViewGraphPanelPlotOverlayWidget(SingleCellViewGraphPanelPlotWidget *pParent = 0);

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
protected:
    virtual QwtText label(double pValue) const;
};

//==============================================================================

class SingleCellViewGraphPanelPlotWidget : public QwtPlot,
                                           public Core::CommonWidget
{
    Q_OBJECT

    friend class SingleCellViewGraphPanelPlotOverlayWidget;

public:
    explicit SingleCellViewGraphPanelPlotWidget(QWidget *pParent = 0);
    ~SingleCellViewGraphPanelPlotWidget();

    virtual void retranslateUi();

    QList<SingleCellViewGraphPanelPlotGraph *> graphs() const;

    bool addGraph(SingleCellViewGraphPanelPlotGraph *pGraph);
    bool removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph);

    void setWantedAxes(const double &pWantedMinX, const double &pWantedMaxX,
                       const double &pWantedMinY, const double &pWantedMaxY);

    void drawGraphSegment(SingleCellViewGraphPanelPlotGraph *pGraph,
                          const qulonglong &pFrom, const qulonglong &pTo);

    void replotNow();

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseReleaseEvent(QMouseEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    enum MouseAction {
        None,
        Pan,
        ShowCoordinates,
        Zoom,
        ZoomRegion
    };

    enum SettingAction {
        Set,
        Merge,
        Reset
    };

    Ui::SingleCellViewGraphPanelPlotWidget *mGui;

    QwtPlotDirectPainter *mDirectPainter;

    QList<SingleCellViewGraphPanelPlotGraph *> mGraphs;

    MouseAction mMouseAction;

    QPoint mPoint;

    double mWantedMinX;
    double mWantedMaxX;
    double mWantedMinY;
    double mWantedMaxY;

    SingleCellViewGraphPanelPlotOverlayWidget *mOverlayWidget;

    bool mCanZoomInX;
    bool mCanZoomOutX;
    bool mCanZoomInY;
    bool mCanZoomOutY;

    bool mNeedContextMenu;
    QMenu *mContextMenu;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void checkAxisValues(double &pMin, double &pMax);
    void checkAxesValues(double &pMinX, double &pMaxX,
                         double &pMinY, double &pMaxY);

    void updateActions();

    MouseAction mouseAction() const;

    void resetMouseAction();

    double minX() const;
    double maxX() const;
    double minY() const;
    double maxY() const;

    bool canZoomInX() const;
    bool canZoomOutX() const;
    bool canZoomInY() const;
    bool canZoomOutY() const;

    void optimiseAxisValues(const int &pAxisId, double &pMin, double &pMax);

    void doSetAxis(const int &pAxis, double pMin, double pMax);
    bool doSetAxes(const SettingAction &pSettingAction,
                   double pMinX = 0, double pMaxX = 0,
                   double pMinY = 0, double pMaxY = 0);

    void scaleAxes(const double &pScalingFactorX,
                   const double &pScalingFactorY);

    QPointF canvasPoint(const QPoint &pPoint,
                        const bool pNeedOffset = true) const;

private Q_SLOTS:
    void on_actionCopy_triggered();

    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();

    void on_actionResetZoom_triggered();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

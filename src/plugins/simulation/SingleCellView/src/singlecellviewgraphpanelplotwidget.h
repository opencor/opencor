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

//==============================================================================

class QAction;
class QMenu;

//==============================================================================

class QwtPlotDirectPainter;

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
    enum Action {
        None,
        ShowCoordinates,
        ZoomRegion
    };

    explicit SingleCellViewGraphPanelPlotOverlayWidget(SingleCellViewGraphPanelPlotWidget *pParent = 0);

    void reset();

    void setAction(const Action &pAction);

    void setOriginPoint(const QPointF &pOriginPoint);
    void setEndPoint(const QPointF &pEndPoint);

    QRectF zoomRegion() const;

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

    Action mAction;

    QPointF mOriginPoint;
    QPointF mEndPoint;

    void drawCoordinates(QPainter *pPainter, const QPointF &pCoordinates,
                         const QPointF &pCoordinatesPosition,
                         const QColor &pBackgroundColor,
                         const QColor &pForegroundColor,
                         const Location &pLocation = TopLeft,
                         const bool &pCanMoveLocation = true);
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

    void setAxes(const double &pMinX, const double &pMaxX,
                 const double &pMinY, const double &pMaxY);

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
    enum Action {
        None,
        Pan,
        ShowCoordinates,
        Zoom,
        ZoomRegion
    };

    QwtPlotDirectPainter *mDirectPainter;

    QList<SingleCellViewGraphPanelPlotGraph *> mGraphs;

    Action mAction;

    QPointF mOriginPoint;

    double mMinX;
    double mMaxX;

    double mMinY;
    double mMaxY;

    double mNeedMinX;
    double mNeedMaxX;

    double mNeedMinY;
    double mNeedMaxY;

    SingleCellViewGraphPanelPlotOverlayWidget *mOverlayWidget;

    double mZoomFactorX;
    double mZoomFactorY;

    bool mNeedCustomContextMenu;
    QMenu *mCustomContextMenu;

    QAction *mCopyAction;

    QAction *mZoomInAction;
    QAction *mZoomOutAction;

    QAction *mResetZoomAction;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void checkLocalAxisValues(const int &pAxis, double &pMin, double &pMax);

    void checkAnyAxesValues(double &pMinX, double &pMaxX,
                            double &pMinY, double &pMaxY);

    void updateZoomFactors();

    double minX() const;
    double maxX() const;

    void setMinMaxX(const double &pMinX, const double &pMaxX);

    double minY() const;
    double maxY() const;

    void setMinMaxY(const double &pMinY, const double &pMaxY);

    double localMinX() const;
    double localMaxX() const;

    void setLocalMinMaxX(const double &pLocalMinX, const double &pLocalMaxX);

    double localMinY() const;
    double localMaxY() const;

    double zoomFactorX() const;
    double zoomFactorY() const;

    void setLocalMinMaxY(const double &pLocalMinY, const double &pLocalMaxY);

    void checkLocalAxes(const bool &pCanReplot = true,
                        const bool &pForceMinMaxValues = false,
                        const bool &pUpdateMinMaxValues = false);

    double optimisedValue(const double &pValue, const double &pPower,
                          const bool &pLowValue);

    void setLocalAxis(const int &pAxis, const double &pMin, const double &pMax);
    void setLocalAxes(const double &pLocalMinX, const double &pLocalMaxX,
                      const double &pLocalMinY, const double &pLocalMaxY,
                      const bool &pCanReplot = true,
                      const bool &pForceMinMaxValues = false,
                      const bool &pUpdateMinMaxValues = false,
                      const bool &pResetMinMaxValues = false);

    void scaleLocalAxes(const double &pScalingFactorX,
                        const double &pScalingFactorY);

    QPointF mousePositionWithinCanvas(const QPoint &pPoint) const;

private Q_SLOTS:
    void copy();

    void zoomIn();
    void zoomOut();

    void resetZoom();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

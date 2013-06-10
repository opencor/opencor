//==============================================================================
// Single cell view graph panel plot widget
//==============================================================================

#ifndef SINGLECELLVIEWGRAPHPANELPLOTWIDGET_H
#define SINGLECELLVIEWGRAPHPANELPLOTWIDGET_H

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"

//==============================================================================

class QwtPlotDirectPainter;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntimeModelParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewGraphPanelPlotCurve : public QwtPlotCurve
{
public:
    explicit SingleCellViewGraphPanelPlotCurve();
};

//==============================================================================

class SingleCellViewGraphPanelPlotWidget : public QwtPlot
{
    Q_OBJECT

public:
    explicit SingleCellViewGraphPanelPlotWidget(QWidget *pParent = 0);
    ~SingleCellViewGraphPanelPlotWidget();

    void replotNow();

    void attach(SingleCellViewGraphPanelPlotCurve *pCurve);
    void detach(SingleCellViewGraphPanelPlotCurve *pCurve);

    void drawCurveSegment(SingleCellViewGraphPanelPlotCurve *pCurve,
                          const qulonglong &pFrom, const qulonglong &pTo);

    void checkLocalAxes(const bool &pCanReplot = true,
                        const bool &pForceMinMaxValues = false,
                        const bool &pUpdateMinMaxValues = false);
    void resetLocalAxes(const bool &pCanReplot = true);

    double minX() const;
    void setMinX(const double &pMinX);

    double maxX() const;
    void setMaxX(const double &pMaxX);

    double minY() const;
    void setMinY(const double &pMinY);

    double maxY() const;
    void setMaxY(const double &pMaxY);

    double localMinX() const;
    void setLocalMinX(const double &pLocalMinX);

    double localMaxX() const;
    void setLocalMaxX(const double &pLocalMaxX);

    double localMinY() const;
    void setLocalMinY(const double &pLocalMinY);

    double localMaxY() const;
    void setLocalMaxY(const double &pLocalMaxY);

    void setFixedAxisX(const bool &pFixedAxisX);
    void setFixedAxisY(const bool &pFixedAxisY);

    void setInteractive(const bool &pInteractive);

    virtual void drawCanvas(QPainter *pPainter);

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseReleaseEvent(QMouseEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    enum Action {
        None,
        Pan,
        ShowCoordinates,
        Zoom,
        ZoomRegion
    };

    enum Location {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    QwtPlotDirectPainter *mDirectPainter;

    QList<SingleCellViewGraphPanelPlotCurve *> mCurves;

    bool mInteractive;

    Action mAction;

    QPoint mOriginPoint;
    QPoint mEndPoint;

    double mMinX;
    double mMinY;

    double mMaxX;
    double mMaxY;

    bool mFixedAxisX;
    bool mFixedAxisY;

    QPixmap mCanvasPixmap;

    QwtScaleMap mCanvasMapX;
    QwtScaleMap mCanvasMapY;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void checkLocalAxisValues(const int &pAxis, double &pMin, double &pMax,
                              const bool &pCanResetMin = true,
                              const bool &pCanResetMax = true);

    void checkAnyAxesValues(double &pMinX, double &pMaxX,
                            double &pMinY, double &pMaxY);

    void setLocalAxis(const int &pAxis, const double &pMin, const double &pMax);
    void setLocalAxes(const double &pLocalMinX, const double &pLocalMaxX,
                      const double &pLocalMinY, const double &pLocalMaxY,
                      const bool &pCanReplot = true,
                      const bool &pForceMinMaxValues = false,
                      const bool &pUpdateMinMaxValues = false,
                      const bool &pResetMinMaxValues = false);

    void scaleLocalAxes(const double &pScalingFactorX,
                        const double &pScalingFactorY);

    QPoint mousePositionWithinCanvas(const QPoint &pPoint) const;

    void drawCoordinates(QPainter *pPainter, const QPoint &pCoordinates,
                         const QColor &pBackgroundColor,
                         const QColor &pForegroundColor,
                         const Location &pLocation = TopLeft,
                         const bool &pCanMoveLocation = true);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

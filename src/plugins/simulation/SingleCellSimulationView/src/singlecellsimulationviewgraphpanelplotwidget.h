//==============================================================================
// Single cell simulation view graph panel plot widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWGRAPHPANELPLOTWIDGET_H
#define SINGLECELLSIMULATIONVIEWGRAPHPANELPLOTWIDGET_H

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

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanelPlotCurve : public QwtPlotCurve
{
public:
    explicit SingleCellSimulationViewGraphPanelPlotCurve();
};

//==============================================================================

class SingleCellSimulationViewGraphPanelPlotWidget : public QwtPlot
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewGraphPanelPlotWidget();

    void replotNow();

    void attach(SingleCellSimulationViewGraphPanelPlotCurve *pCurve);
    void detach(SingleCellSimulationViewGraphPanelPlotCurve *pCurve);

    void drawCurveSegment(SingleCellSimulationViewGraphPanelPlotCurve *pCurve,
                          const qulonglong &pFrom, const qulonglong &pTo);

    void checkAxes(const bool &pCanReplot = true,
                   const bool &pForceMinMaxValues = false,
                   const bool &pUpdateMinMaxValues = false);
    void resetAxes(const bool &pCanReplot = true);

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

    QList<SingleCellSimulationViewGraphPanelPlotCurve *> mCurves;

    bool mInteractive;

    Action mAction;

    QPoint mOriginPoint;
    QPoint mEndPoint;

    double mMinX;
    double mMaxX;
    double mMinY;
    double mMaxY;

    bool mFixedAxisX;
    bool mFixedAxisY;

    QPixmap mCanvasPixmap;

    QwtScaleMap mCanvasMapX;
    QwtScaleMap mCanvasMapY;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void checkAxesValues(double &pMinX, double &pMaxX,
                         double &pMinY, double &pMaxY);

    void setLocalAxis(const int &pAxis, const double &pMin, const double &pMax);
    void setAxes(const double &pMinX, const double &pMaxX,
                 const double &pMinY, const double &pMaxY,
                 const bool &pCanReplot = true,
                 const bool &pForceMinMaxValues = false,
                 const bool &pUpdateMinMaxValues = false,
                 const bool &pResetMinMaxValues = false);

    void scaleAxes(const double &pScalingFactorX,
                   const double &pScalingFactorY);

    QPoint mousePositionWithinCanvas(QMouseEvent *pEvent) const;

    void drawCoordinates(QPainter *pPainter, const QPoint &pCoordinates,
                         const QColor &pBackgroundColor,
                         const QColor &pForegroundColor,
                         const Location &pLocation = TopLeft,
                         const bool &pCanMoveLocation = true);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

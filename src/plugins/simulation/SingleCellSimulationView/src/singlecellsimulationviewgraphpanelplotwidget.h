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
                   const bool &pResetAndForceMinMaxValues = false);
    void resetAxes(const bool &pCanReplot = true);

    double minX() const;
    void setMinX(const double &pValue, const bool &pCheckAxes = true);

    double maxX() const;
    void setMaxX(const double &pValue, const bool &pCheckAxes = true);

    double minY() const;
    void setMinY(const double &pValue, const bool &pCheckAxes = true);

    double maxY() const;
    void setMaxY(const double &pValue, const bool &pCheckAxes = true);

    double localMinX() const;
    void setLocalMinX(const double &pValue, const bool &pCheckAxes = true);

    double localMaxX() const;
    void setLocalMaxX(const double &pValue, const bool &pCheckAxes = true);

    double localMinY() const;
    void setLocalMinY(const double &pValue, const bool &pCheckAxes = true);

    double localMaxY() const;
    void setLocalMaxY(const double &pValue, const bool &pCheckAxes = true);

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual void mouseReleaseEvent(QMouseEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    enum Action {
        None,
        Zoom
    };

    QwtPlotDirectPainter *mDirectPainter;

    QList<SingleCellSimulationViewGraphPanelPlotCurve *> mCurves;

    Action mAction;

    QPoint mOriginPoint;

    double mMinX;
    double mMaxX;
    double mMinY;
    double mMaxY;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void setAxes(const double &pMinX, const double &pMaxX,
                 const double &pMinY, const double &pMaxY,
                 const bool &pCanReplot = true,
                 const bool &pResetAndForceMinMaxValues = false);

    void scaleAxes(const double &pScalingFactorX,
                   const double &pScalingFactorY);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

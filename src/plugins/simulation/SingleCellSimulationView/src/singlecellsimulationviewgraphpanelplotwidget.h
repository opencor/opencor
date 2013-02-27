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

    bool isValid() const;

    bool isEnabled() const;
    void setEnabled(const bool &pEnabled);

private:
    bool mEnabled;
};

//==============================================================================

class SingleCellSimulationViewGraphPanelPlotWidget : public QwtPlot
{
    Q_OBJECT

public:
    enum Axis {
        AxisX,
        AxisY
    };

    explicit SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewGraphPanelPlotWidget();

    void replotNow();

    SingleCellSimulationViewGraphPanelPlotCurve * addCurve();

    void drawCurveSegment(SingleCellSimulationViewGraphPanelPlotCurve *pCurve,
                          const qulonglong &pFrom, const qulonglong &pTo);

    void checkAxesScales(const bool &pCanReplot = true);

    void setFixedAxisScale(const Axis &pAxis,
                           const double &pMin = 0.0, const double &pMax = 0.0);

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

    double mMinFixedScaleX;
    double mMaxFixedScaleX;

    double mMinFixedScaleY;
    double mMaxFixedScaleY;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void setAxesScales(const double &pMinX, const double &pMaxX,
                       const double &pMinY, const double &pMaxY,
                       const bool &pCanReplot = true);
    void resetAxesScales();
    void scaleAxesScales(const double &pScalingFactorX,
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

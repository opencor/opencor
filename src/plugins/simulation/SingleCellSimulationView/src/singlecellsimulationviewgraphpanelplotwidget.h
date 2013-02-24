//==============================================================================
// Single cell simulation view graph panel plot widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWGRAPHPANELPLOTWIDGET_H
#define SINGLECELLSIMULATIONVIEWGRAPHPANELPLOTWIDGET_H

//==============================================================================

#include "qwt_plot.h"

//==============================================================================

class QwtPlotCurve;
class QwtPlotDirectPainter;

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanelPlotWidget : public QwtPlot
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewGraphPanelPlotWidget();

    QwtPlotCurve * addTrace(double *pX, double *pY,
                            const qulonglong &pOriginalSize);

    void removeTrace(QwtPlotCurve *pTrace, const bool &pReplot = true);
    void removeTraces();

    void drawTraceSegment(QwtPlotCurve *pTrace,
                          const qulonglong &pFrom, const qulonglong &pTo);

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

    QList<QwtPlotCurve *> mTraces;

    Action mAction;

    QPoint mOriginPoint;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

    void rescaleAxes(const double &pXScalingFactor,
                     const double &pYScalingFactor);

public Q_SLOTS:
    virtual void replot();
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

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

    void drawTraceSegment(QwtPlotCurve *pTrace,
                          const qulonglong &pFrom, const qulonglong &pTo);

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);

private:
    QwtPlotDirectPainter *mDirectPainter;

    void handleMouseDoubleClickEvent(QMouseEvent *pEvent);

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

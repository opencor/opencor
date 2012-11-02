//==============================================================================
// Single cell simulation view graph panel widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWGRAPHPANELWIDGET_H
#define SINGLECELLSIMULATIONVIEWGRAPHPANELWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

class QFrame;

//==============================================================================

class QwtPlot;
class QwtPlotCurve;

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewGraphPanelWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanelWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewGraphPanelWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewGraphPanelWidget();

    QwtPlotCurve * addCurve();
    void resetCurves();

    bool isActive() const;
    void setActive(const bool &pActive);

    QwtPlot * plot();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    Ui::SingleCellSimulationViewGraphPanelWidget *mGui;

    QFrame *mMarker;
    QwtPlot *mPlot;

    bool mActive;

    QList<QwtPlotCurve *> mPlotCurves;

    void setMarkerColor();

Q_SIGNALS:
    void activated(SingleCellSimulationViewGraphPanelWidget *pGraphPanel);
    void inactivated(SingleCellSimulationViewGraphPanelWidget *pGraphPanel);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

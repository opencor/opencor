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

class QwtPlotCurve;

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewGraphPanelWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanelPlotWidget;

//==============================================================================

class SingleCellSimulationViewGraphPanelWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewGraphPanelWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewGraphPanelWidget();

    bool isActive() const;
    void setActive(const bool &pActive);

    SingleCellSimulationViewGraphPanelPlotWidget * plot();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    Ui::SingleCellSimulationViewGraphPanelWidget *mGui;

    QFrame *mMarker;
    SingleCellSimulationViewGraphPanelPlotWidget *mPlot;

    bool mActive;

    void updateMarkerColor();

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

//==============================================================================
// Single cell simulation view graph panel widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWGRAPHPANEL_H
#define SINGLECELLSIMULATIONVIEWGRAPHPANEL_H

//==============================================================================

#include "widget.h"

//==============================================================================

class QFrame;

//==============================================================================

class QwtPlot;
class QwtPlotCurve;

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewGraphPanel;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanel : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewGraphPanel(QWidget *pParent);
    ~SingleCellSimulationViewGraphPanel();

    QwtPlotCurve * addCurve();
    void resetCurves();

    bool isActive() const;
    void setActive(const bool &pActive);

    QwtPlot * plot();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    Ui::SingleCellSimulationViewGraphPanel *mGui;

    QFrame *mMarker;
    QwtPlot *mPlot;

    bool mActive;

    QList<QwtPlotCurve *> mPlotCurves;

    void setMarkerColor();

Q_SIGNALS:
    void activated(SingleCellSimulationViewGraphPanel *pGraphPanel);
    void inactivated(SingleCellSimulationViewGraphPanel *pGraphPanel);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Single cell simulation view graph panel widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWGRAPHPANEL_H
#define SINGLECELLSIMULATIONVIEWGRAPHPANEL_H

//==============================================================================

#include <QWidget>

//==============================================================================

class QFrame;

//==============================================================================

class QwtPlot;
class QwtPlotCurve;

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanel : public QWidget
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
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    QFrame *mMarker;
    QwtPlot *mPlot;

    bool mActive;

    QList<QwtPlotCurve *> mPlotCurves;

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

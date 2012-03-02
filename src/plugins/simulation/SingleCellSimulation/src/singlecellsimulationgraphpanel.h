//==============================================================================
// Single cell simulation graph panel widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONGRAPHPANEL_H
#define SINGLECELLSIMULATIONGRAPHPANEL_H

//==============================================================================

#include <QWidget>

//==============================================================================

class QFrame;

//==============================================================================

class QwtPlot;
class QwtPlotCurve;

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

class SingleCellSimulationGraphPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationGraphPanel(QWidget *pParent = 0);
    ~SingleCellSimulationGraphPanel();

    QwtPlotCurve * addCurve();
    void resetCurves();

    bool isActive() const;
    void setActive(const bool &pActive);

    void replot();

protected:
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    QFrame *mMarker;
    QwtPlot *mPlot;

    bool mActive;

    QList<QwtPlotCurve *> mPlotCurves;

Q_SIGNALS:
    void activated(SingleCellSimulationGraphPanel *);
    void inactivated(SingleCellSimulationGraphPanel *);
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

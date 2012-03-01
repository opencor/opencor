//==============================================================================
// Single cell simulation graph panel widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONGRAPHPANEL_H
#define SINGLECELLSIMULATIONGRAPHPANEL_H

//==============================================================================

#include "qwt_plot.h"

//==============================================================================

class QwtPlotCurve;

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

class SingleCellSimulationGraphPanel : public QwtPlot
{
public:
    explicit SingleCellSimulationGraphPanel(QWidget *pParent = 0);
    ~SingleCellSimulationGraphPanel();

    QwtPlotCurve * addCurve();
    void resetCurves();

private:
    QList<QwtPlotCurve *> mCurves;
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

//==============================================================================
// Single cell simulation view graph panel plot widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWGRAPHPANELPLOTWIDGET_H
#define SINGLECELLSIMULATIONVIEWGRAPHPANELPLOTWIDGET_H

//==============================================================================

#include "qwt_plot.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanelPlotWidget : public QwtPlot
{
public:
    explicit SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent = 0);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

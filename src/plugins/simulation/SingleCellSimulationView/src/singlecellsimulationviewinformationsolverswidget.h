//==============================================================================
// Single cell simulation view information solvers widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONSOLVERSWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONSOLVERSWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationSolversWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSolversWidget(QWidget *pParent = 0);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

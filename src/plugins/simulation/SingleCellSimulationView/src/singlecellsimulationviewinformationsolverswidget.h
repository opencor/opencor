//==============================================================================
// Single cell simulation view information solvers widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONSOLVERSWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONSOLVERSWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewInformationSolversWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulation;

//==============================================================================

class SingleCellSimulationViewInformationSolversWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSolversWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewInformationSolversWidget();

    virtual void retranslateUi();

    void initialize(SingleCellSimulationViewSimulation *pSimulation);

private:
    Ui::SingleCellSimulationViewInformationSolversWidget *mGui;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

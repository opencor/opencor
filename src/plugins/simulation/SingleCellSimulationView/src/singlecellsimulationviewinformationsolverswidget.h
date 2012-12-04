//==============================================================================
// Single cell simulation view information solvers widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONSOLVERSWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONSOLVERSWIDGET_H

//==============================================================================

#include "qtpropertybrowserwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationSolversWidget : public QtPropertyBrowserSupport::QtPropertyBrowserWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSolversWidget(QWidget *pParent = 0);

    void initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);

private:
    bool mOdeSolver;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================

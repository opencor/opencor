//==============================================================================
// Single cell simulation view information solvers widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "singlecellsimulationviewinformationsolverswidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationSolversWidget::SingleCellSimulationViewInformationSolversWidget(QWidget *pParent) :
    QtPropertyBrowserWidget(pParent),
    mOdeSolver(true)
{
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime)
{
    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Make sure that the CellML file runtime is valid

    if (pCellmlFileRuntime->isValid())
        // Check whether we need an ODE or DAE solver

        mOdeSolver = pCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode;
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

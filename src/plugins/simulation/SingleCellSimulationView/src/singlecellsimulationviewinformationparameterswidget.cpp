//==============================================================================
// Single cell simulation view information parameters widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "singlecellsimulationviewinformationparameterswidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationParametersWidget::SingleCellSimulationViewInformationParametersWidget(QWidget *pParent) :
    PropertyEditorWidget(pParent)
{
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime)
{
    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

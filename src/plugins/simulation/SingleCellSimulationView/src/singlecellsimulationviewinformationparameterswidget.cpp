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

void SingleCellSimulationViewInformationParametersWidget::initialize(const QString &pFileName,
                                                                     CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime)
{
    Q_UNUSED(pFileName);

    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::finalize(const QString &pFileName)
{
    Q_UNUSED(pFileName);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

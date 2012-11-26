//==============================================================================
// Single cell simulation view information solvers widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "singlecellsimulationviewinformationsolverswidget.h"
#include "singlecellsimulationviewsimulation.h"

//==============================================================================

#include "ui_singlecellsimulationviewinformationsolverswidget.h"

//==============================================================================

#include <QLabel>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationSolversWidget::SingleCellSimulationViewInformationSolversWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewInformationSolversWidget)
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

SingleCellSimulationViewInformationSolversWidget::~SingleCellSimulationViewInformationSolversWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::retranslateUi()
{
    // Retranslate the whole widget

    mGui->retranslateUi(this);
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::initialize(SingleCellSimulationViewSimulation *pSimulation)
{
    // Prevent the GUI from updating itself (in case the initialisation takes
    // time, thus avoiding flickering)

    setUpdatesEnabled(false);

    // Remove all existing children

    qDeleteAll(findChildren<QWidget *>());

    // Make sure that the simulation has a valid CellML runtime

    CellMLSupport::CellmlFileRuntime *cellmlFileRuntime = pSimulation->cellmlFileRuntime();

    if (cellmlFileRuntime->isValid()) {
//---GRY--- TO BE DONE...
    }

    // Re-allow the GUI to be update itself

    setUpdatesEnabled(true);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

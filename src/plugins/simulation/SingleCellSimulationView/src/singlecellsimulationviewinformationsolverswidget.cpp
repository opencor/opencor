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
    mGui(new Ui::SingleCellSimulationViewInformationSolversWidget),
    mOdeSolver(true)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Add a label and a property widgets for our ODE/DAE solver

    mOdeDaeSolverLabel = new QLabel(this);

    mGui->layout->addWidget(mOdeDaeSolverLabel);

    // Add a label and a property widgets for our NLA solver

    mNlaSolverLabel = new QLabel(this);

    mGui->layout->addWidget(mNlaSolverLabel);
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

    // Retranslate our widgets

    mOdeDaeSolverLabel->setText(mOdeSolver?tr("ODE solver:"):tr("DAE solver:"));
    mNlaSolverLabel->setText(tr("Non-linear algebraic solver:"));
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime)
{
    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Make sure that the CellML file runtime is valid

    if (pCellmlFileRuntime->isValid()) {
        // Check whether we need an ODE or DAE solver

        mOdeSolver = pCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode;

        // Show/hide the NLA solver information

        mNlaSolverLabel->setVisible(pCellmlFileRuntime->needNlaSolver());

        // Retranslate our GUI

        retranslateUi();
    }
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

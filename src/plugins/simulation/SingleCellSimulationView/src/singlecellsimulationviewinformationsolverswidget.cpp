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
    PropertyEditorWidget(true, pParent),
    mOdeOrDaeSolversIndex(-1),
    mNlaSolversIndex(-1)
{
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                                  const SolverInterfaces &pSolverInterfaces)
{
    // Reset our solvers information by romoving all of its contents

    removeAllProperties();

    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Make sure that the CellML file runtime is valid

    if (pCellmlFileRuntime->isValid()) {
        // Check whether we need an ODE or DAE solver, and add a list property
        // for the ODE or DEA solvers

        bool needOdeSolver = pCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode;
        Solver::Type solverType = needOdeSolver?Solver::Ode:Solver::Dae;
        QStringList solvers = QStringList();

        foreach (SolverInterface *solverInterface, pSolverInterfaces)
            if (solverInterface->type() == solverType)
                solvers << solverInterface->name();

        mOdeOrDaeSolversIndex = addListProperty(solvers);

        // Check whether we need an NLA solver, and add a list property if, if
        // needed

        if (pCellmlFileRuntime->needNlaSolver()) {
            solvers = QStringList();

            foreach (SolverInterface *solverInterface, pSolverInterfaces)
                if (solverInterface->type() == Solver::Nla)
                    solvers << solverInterface->name();

            mNlaSolversIndex = addListProperty(solvers);
        } else {
            // We don't need an NLA solver, so...

            mNlaSolversIndex = -1;
        }
    }
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

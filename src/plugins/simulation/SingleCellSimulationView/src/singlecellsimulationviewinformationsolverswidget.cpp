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
    mNeedOdeSolver(true),
    mOdeOrDaeSolversIndex(-1),
    mNlaSolversIndex(-1)
{
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::retranslateUi()
{
    // Update our property names

    if (mOdeOrDaeSolversIndex != -1) {
        setPropertyName(mOdeOrDaeSolversIndex, mNeedOdeSolver?tr("ODE solver"):tr("DAE solver"));

        propertyValue(mOdeOrDaeSolversIndex)->setEmptyListValue(tr("None available"));
    }

    if (mNlaSolversIndex != -1) {
        setPropertyName(mNlaSolversIndex, tr("NLA solver"));

        propertyValue(mNlaSolversIndex)->setEmptyListValue(tr("None available"));
    }

    // Default retranslation
    // Note: we must do it last since we set the empty list value of some
    //       properties above...

    PropertyEditorWidget::retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                                  const SolverInterfaces &pSolverInterfaces)
{
    // Reset our solvers information

    mOdeOrDaeSolversIndex = -1;
    mNlaSolversIndex      = -1;

    removeAllProperties();

    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Make sure that the CellML file runtime is valid

    if (pCellmlFileRuntime->isValid()) {
        // Check whether we need an ODE or DAE solver, and add a list property
        // for the ODE or DEA solvers

        mNeedOdeSolver = pCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode;
        Solver::Type solverType = mNeedOdeSolver?Solver::Ode:Solver::Dae;
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

        // Retranslate ourselves so that the property names get properly set

        retranslateUi();
    }
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

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
    mOdeOrDaeSolversProperty(Core::Property()),
    mNlaSolversProperty(Core::Property())
{
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::retranslateUi()
{
    // Update our property names

    if (!mOdeOrDaeSolversProperty.isEmpty()) {
        setStringProperty(mOdeOrDaeSolversProperty.name, mNeedOdeSolver?tr("ODE solver"):tr("DAE solver"));

        mOdeOrDaeSolversProperty.value->setEmptyListValue(tr("None available"));
    }

    if (!mNlaSolversProperty.isEmpty()) {
        setStringProperty(mNlaSolversProperty.name, tr("NLA solver"));

        mNlaSolversProperty.value->setEmptyListValue(tr("None available"));
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

    mOdeOrDaeSolversProperty = Core::Property();
    mNlaSolversProperty      = Core::Property();

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

        mOdeOrDaeSolversProperty = addListProperty(solvers);

        // Check whether we need an NLA solver, and add a list property if, if
        // needed

        mNeedNlaSolver = pCellmlFileRuntime->needNlaSolver();

        if (mNeedNlaSolver) {
            solvers = QStringList();

            foreach (SolverInterface *solverInterface, pSolverInterfaces)
                if (solverInterface->type() == Solver::Nla)
                    solvers << solverInterface->name();

            mNlaSolversProperty = addListProperty(solvers);
        } else {
            // We don't need an NLA solver, so...

            mNlaSolversProperty = Core::Property();
        }

        // Retranslate ourselves so that the property names get properly set

        retranslateUi();
    }
}

//==============================================================================

bool SingleCellSimulationViewInformationSolversWidget::needOdeSolver() const
{
    // Return whether we need an ODE solver

    return mNeedOdeSolver;
}

//==============================================================================

bool SingleCellSimulationViewInformationSolversWidget::needNlaSolver() const
{
    // Return whether we need an NLA solver

    return mNeedNlaSolver;
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::odeOrDaeSolvers() const
{
    // Return the available ODE/DAE solvers, if any

    return mOdeOrDaeSolversProperty.isEmpty()?QStringList():mOdeOrDaeSolversProperty.value->list();
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::nlaSolvers() const
{
    // Return the available NLA solvers, if any

    return mNlaSolversProperty.isEmpty()?QStringList():mNlaSolversProperty.value->list();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

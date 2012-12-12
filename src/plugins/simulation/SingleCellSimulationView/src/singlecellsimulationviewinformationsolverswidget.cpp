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
    mNeedDaeSolver(false),
    mNeedNlaSolver(false),
    mOdeSolversProperty(Core::Property()),
    mDaeSolversProperty(Core::Property()),
    mNlaSolversProperty(Core::Property())
{
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::retranslateUi()
{
    // Update our property names

    if (!mOdeSolversProperty.isEmpty()) {
        setStringProperty(mOdeSolversProperty.name, tr("ODE solver"));

        mOdeSolversProperty.value->setEmptyListValue(tr("None available"));
    }

    if (!mDaeSolversProperty.isEmpty()) {
        setStringProperty(mDaeSolversProperty.name, tr("DAE solver"));

        mDaeSolversProperty.value->setEmptyListValue(tr("None available"));
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

Core::Property SingleCellSimulationViewInformationSolversWidget::addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                                     const Solver::Type &pSolverType)
{
    // Retrieve the name of the solvers which type is the one we are interested
    // in

    QStringList solvers = QStringList();

    foreach (SolverInterface *solverInterface, pSolverInterfaces)
        if (solverInterface->type() == pSolverType)
            solvers << solverInterface->name();

    // Return the property that contains the requested solvers

    return solvers.isEmpty()?Core::Property():addListProperty(solvers);
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::setSolverInterfaces(const SolverInterfaces &pSolverInterfaces)
{
    // Remove all our properties

    removeAllProperties();

    // Add properties for our different solvers

    mOdeSolversProperty = addSolverProperties(pSolverInterfaces, Solver::Ode);
    mDaeSolversProperty = addSolverProperties(pSolverInterfaces, Solver::Dae);
    mNlaSolversProperty = addSolverProperties(pSolverInterfaces, Solver::Nla);
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                                  const SolverInterfaces &pSolverInterfaces)
{
    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Make sure that the CellML file runtime is valid

    if (pCellmlFileRuntime->isValid()) {
        // Check whether we need an ODE or DAE solver, and add a list property
        // for the ODE or DEA solvers

        mNeedOdeSolver = pCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode;
        mNeedDaeSolver = !mNeedOdeSolver;

        setPropertyVisible(mOdeSolversProperty, mNeedOdeSolver);
        setPropertyVisible(mDaeSolversProperty, mNeedDaeSolver);

        // Check whether we need an NLA solver, and add a list property if, if
        // needed

        mNeedNlaSolver = pCellmlFileRuntime->needNlaSolver();

        setPropertyVisible(mNlaSolversProperty, mNeedNlaSolver);

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

bool SingleCellSimulationViewInformationSolversWidget::needDaeSolver() const
{
    // Return whether we need a DAE solver

    return mNeedDaeSolver;
}

//==============================================================================

bool SingleCellSimulationViewInformationSolversWidget::needNlaSolver() const
{
    // Return whether we need an NLA solver

    return mNeedNlaSolver;
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::odeSolvers() const
{
    // Return the available ODE solvers, if any

    return mOdeSolversProperty.isEmpty()?QStringList():mOdeSolversProperty.value->list();
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::daeSolvers() const
{
    // Return the available DAE solvers, if any

    return mDaeSolversProperty.isEmpty()?QStringList():mDaeSolversProperty.value->list();
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

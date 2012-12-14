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
        setNonEditablePropertyItem(mOdeSolversProperty.name, tr("ODE solver"));
        setNonEditablePropertyItem(mOdeSolversListProperty.name, tr("Name"));

        mOdeSolversListProperty.value->setEmptyListValue(tr("None available"));
    }

    if (!mDaeSolversProperty.isEmpty()) {
        setNonEditablePropertyItem(mDaeSolversProperty.name, tr("DAE solver"));
        setNonEditablePropertyItem(mDaeSolversListProperty.name, tr("Name"));

        mDaeSolversListProperty.value->setEmptyListValue(tr("None available"));
    }

    if (!mNlaSolversProperty.isEmpty()) {
        setNonEditablePropertyItem(mNlaSolversProperty.name, tr("NLA solver"));
        setNonEditablePropertyItem(mNlaSolversListProperty.name, tr("Name"));

        mNlaSolversListProperty.value->setEmptyListValue(tr("None available"));
    }

    // Default retranslation
    // Note: we must do it last since we set the empty list value of some
    //       properties above...

    PropertyEditorWidget::retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                           const Solver::Type &pSolverType,
                                                                           Core::Property &pSolversProperty,
                                                                           Core::Property &pSolversListProperty)
{
    // Make sure that we have at least one solver interface

    if (pSolverInterfaces.isEmpty()) {
        pSolversProperty     = Core::Property();
        pSolversListProperty = Core::Property();

        return;
    }

    // Add our category property

    pSolversProperty = addCategoryProperty();

    // Add our list property for the solvers

    pSolversListProperty = addListProperty(pSolversProperty);

    // Retrieve the name of the solvers which type is the one we are interested
    // in

    QStringList solvers = QStringList();

    foreach (SolverInterface *solverInterface, pSolverInterfaces)
        if (solverInterface->type() == pSolverType) {
            // Keep track of the solver's name

            solvers << solverInterface->name();

            // Add the solver's properties

            Core::Property property;
            Core::Properties properties = Core::Properties();

            foreach (const Solver::Property &solverInterfaceProperty,
                     solverInterface->properties()) {
                // Add the solver's property

                switch (solverInterfaceProperty.type) {
                case Solver::Double:
                    property = addDoubleProperty(pSolversProperty);

                    break;
                default:
                    // Solver::Integer

                    property = addIntegerProperty(pSolversProperty);
                }

                // Set the solver's property's name

                setNonEditablePropertyItem(property.name, solverInterfaceProperty.name);

                // Keep track of the solver's property

                properties << property;
            }

            // Keep track of the solver's properties

            mSolversProperties.insert(solverInterface->name(), properties);
        }

    // Add the list of solvers to our list property value item

    pSolversListProperty.value->setList(solvers);

    // Keep track of changes to list properties

    connect(this, SIGNAL(listPropertyChanged(const QString &)),
            this, SLOT(updateProperties(const QString &)));
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::setSolverInterfaces(const SolverInterfaces &pSolverInterfaces)
{
    // Remove all our properties

    removeAllProperties();

    // Add properties for our different solvers

    addSolverProperties(pSolverInterfaces, Solver::Ode, mOdeSolversProperty, mOdeSolversListProperty);
    addSolverProperties(pSolverInterfaces, Solver::Dae, mDaeSolversProperty, mDaeSolversListProperty);
    addSolverProperties(pSolverInterfaces, Solver::Nla, mNlaSolversProperty, mNlaSolversListProperty);

    // Expand all our properties

    expandAll();
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

    return mOdeSolversListProperty.isEmpty()?QStringList():mOdeSolversListProperty.value->list();
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::daeSolvers() const
{
    // Return the available DAE solvers, if any

    return mDaeSolversListProperty.isEmpty()?QStringList():mDaeSolversListProperty.value->list();
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::nlaSolvers() const
{
    // Return the available NLA solvers, if any

    return mNlaSolversListProperty.isEmpty()?QStringList():mNlaSolversListProperty.value->list();
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::updateProperties(const QString &pValue)
{
qDebug("---------------------------------------");
qDebug(">>> updateProperties() | List property changed...");
Core::Property property = currentProperty();
qDebug(">>> updateProperties() | Property: %s", qPrintable(property.name->text()));
qDebug(">>> updateProperties() | Current value: %s", qPrintable(property.value->text()));
qDebug(">>> updateProperties() | New value: %s", qPrintable(pValue));
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

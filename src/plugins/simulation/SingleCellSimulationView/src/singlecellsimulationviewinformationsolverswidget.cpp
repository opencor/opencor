//==============================================================================
// Single cell simulation view information solvers widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "cellmlfilevariable.h"
#include "singlecellsimulationviewinformationsolverswidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationSolversWidgetData::SingleCellSimulationViewInformationSolversWidgetData() :
    needSolver(true),
    solversProperty(Core::Property()),
    solversListProperty(Core::Property()),
    solversProperties(QMap<QString, Core::Properties>())
{
}

//==============================================================================

SingleCellSimulationViewInformationSolversWidget::SingleCellSimulationViewInformationSolversWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent),
    mOdeSolverData(SingleCellSimulationViewInformationSolversWidgetData()),
    mDaeSolverData(SingleCellSimulationViewInformationSolversWidgetData()),
    mNlaSolverData(SingleCellSimulationViewInformationSolversWidgetData()),
    mGuiStates(QMap<QString, Core::PropertyEditorWidgetGuiState>()),
    mDefaultGuiState(Core::PropertyEditorWidgetGuiState())
{
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::retranslateUi()
{
    // Update our property names

    if (!mOdeSolverData.solversProperty.isEmpty()) {
        setNonEditablePropertyItem(mOdeSolverData.solversProperty.name, tr("ODE solver"));
        setNonEditablePropertyItem(mOdeSolverData.solversListProperty.name, tr("Name"));

        mOdeSolverData.solversListProperty.value->setEmptyListValue(tr("None available"));
    }

    if (!mDaeSolverData.solversProperty.isEmpty()) {
        setNonEditablePropertyItem(mDaeSolverData.solversProperty.name, tr("DAE solver"));
        setNonEditablePropertyItem(mDaeSolverData.solversListProperty.name, tr("Name"));

        mDaeSolverData.solversListProperty.value->setEmptyListValue(tr("None available"));
    }

    if (!mNlaSolverData.solversProperty.isEmpty()) {
        setNonEditablePropertyItem(mNlaSolverData.solversProperty.name, tr("NLA solver"));
        setNonEditablePropertyItem(mNlaSolverData.solversListProperty.name, tr("Name"));

        mNlaSolverData.solversListProperty.value->setEmptyListValue(tr("None available"));
    }

    // Default retranslation
    // Note: we must do it last since we set the empty list value of some
    //       properties above...

    PropertyEditorWidget::retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                           const Solver::Type &pSolverType,
                                                                           SingleCellSimulationViewInformationSolversWidgetData &pSolverData)
{
    // Make sure that we have at least one solver interface

    if (pSolverInterfaces.isEmpty()) {
        pSolverData.solversProperty     = Core::Property();
        pSolverData.solversListProperty = Core::Property();

        return;
    }

    // Add our category property

    pSolverData.solversProperty = addCategoryProperty();

    // Add our list property for the solvers

    pSolverData.solversListProperty = addListProperty(pSolverData.solversProperty);

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

                switch (solverInterfaceProperty.type()) {
                case Solver::Double:
                    property = addDoubleProperty(pSolverData.solversProperty);

                    break;
                default:
                    // Solver::Integer

                    property = addIntegerProperty(pSolverData.solversProperty);
                }

                // Set the solver's property's name

                setNonEditablePropertyItem(property.name, solverInterfaceProperty.name());

                // Set the solver's property's default value

                switch (solverInterfaceProperty.type()) {
                case Solver::Double:
                    setDoublePropertyItem(property.value, solverInterfaceProperty.defaultValue().toDouble());

                    break;
                default:
                    // Solver::Integer

                    setIntegerPropertyItem(property.value, solverInterfaceProperty.defaultValue().toInt());
                }

                // Set the solver's property's 'unit', if needed

                if (solverInterfaceProperty.hasVoiUnit())
                    setNonEditablePropertyItem(property.unit, "???");
                    // Note: to assign a non-empty string to our unit item is
                    //       just a way for us to keep track of the fact that
                    //       the property should have a unit (see
                    //       setPropertiesUnit())...

                // Keep track of the solver's property

                properties << property;
            }

            // Keep track of the solver's properties

            pSolverData.solversProperties.insert(solverInterface->name(), properties);
        }

    // Add the list of solvers to our list property value item

    pSolverData.solversListProperty.value->setList(solvers);

    // Keep track of changes to list properties

    connect(this, SIGNAL(listPropertyChanged(const QString &)),
            this, SLOT(listPropertyChanged(const QString &)));
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::setSolverInterfaces(const SolverInterfaces &pSolverInterfaces)
{
    // Remove all our properties

    removeAllProperties();

    // Add properties for our different solvers

    addSolverProperties(pSolverInterfaces, Solver::Ode, mOdeSolverData);
    addSolverProperties(pSolverInterfaces, Solver::Dae, mDaeSolverData);
    addSolverProperties(pSolverInterfaces, Solver::Nla, mNlaSolverData);

    // Show/hide the relevant properties

    doListPropertyChanged(mOdeSolverData, mOdeSolverData.solversListProperty.value->text(), true);
    doListPropertyChanged(mDaeSolverData, mDaeSolverData.solversListProperty.value->text(), true);
    doListPropertyChanged(mNlaSolverData, mNlaSolverData.solversListProperty.value->text(), true);

    // Expand all our properties

    expandAll();

    // Clear any track of previous GUI states and retrieve our default GUI state
    // Note: we need to clear any track of GUI states since we removed all the
    //       properties and added 'new' ones, so any previous GUI state we might
    //       have had would have been wrong...

    mGuiStates.clear();

    mDefaultGuiState = guiState();
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::setPropertiesUnit(const SingleCellSimulationViewInformationSolversWidgetData &pSolverData,
                                                                         const QString &pVoiUnit)
{
    // Check whether we need this type of solver and, if not, leave

    if (!pSolverData.needSolver)
        return;

    // Go through the solvers' properties and set the unit of the relevant ones

    foreach (const Core::Properties &properties, pSolverData.solversProperties)
        foreach (const Core::Property &property, properties)
            if (!property.unit->text().isEmpty())
                property.unit->setText(pVoiUnit);
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::initialize(const QString &pFileName,
                                                                  CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime)
{
    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Retrieve and initialise our GUI state

    setGuiState(mGuiStates.contains(pFileName)?
                    mGuiStates.value(pFileName):
                    mDefaultGuiState);

    // Make sure that the CellML file runtime is valid

    if (pCellmlFileRuntime->isValid()) {
        // Show/hide the ODE/DAE solver information

        mOdeSolverData.needSolver = pCellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode;
        mDaeSolverData.needSolver = !mOdeSolverData.needSolver;

        setPropertyVisible(mOdeSolverData.solversProperty, mOdeSolverData.needSolver);
        setPropertyVisible(mDaeSolverData.solversProperty, mDaeSolverData.needSolver);

        // Show/hide the NLA solver information

        mNlaSolverData.needSolver = pCellmlFileRuntime->needNlaSolver();

        setPropertyVisible(mNlaSolverData.solversProperty, mNlaSolverData.needSolver);

        // Retranslate ourselves so that the property names get properly set

        retranslateUi();
    }

    // Set the unit of our different properties, if needed

    QString voiUnit = pCellmlFileRuntime->variableOfIntegration()->unit();

    setPropertiesUnit(mOdeSolverData, voiUnit);
    setPropertiesUnit(mDaeSolverData, voiUnit);
    setPropertiesUnit(mNlaSolverData, voiUnit);
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::finalize(const QString &pFileName)
{
    // Keep track of our GUI state

    mGuiStates.insert(pFileName, guiState());
}

//==============================================================================

bool SingleCellSimulationViewInformationSolversWidget::needOdeSolver() const
{
    // Return whether we need an ODE solver

    return mOdeSolverData.needSolver;
}

//==============================================================================

bool SingleCellSimulationViewInformationSolversWidget::needDaeSolver() const
{
    // Return whether we need a DAE solver

    return mDaeSolverData.needSolver;
}

//==============================================================================

bool SingleCellSimulationViewInformationSolversWidget::needNlaSolver() const
{
    // Return whether we need an NLA solver

    return mNlaSolverData.needSolver;
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::odeSolvers() const
{
    // Return the available ODE solvers, if any

    return mOdeSolverData.solversListProperty.isEmpty()?QStringList():mOdeSolverData.solversListProperty.value->list();
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::daeSolvers() const
{
    // Return the available DAE solvers, if any

    return mDaeSolverData.solversListProperty.isEmpty()?QStringList():mDaeSolverData.solversListProperty.value->list();
}

//==============================================================================

QStringList SingleCellSimulationViewInformationSolversWidget::nlaSolvers() const
{
    // Return the available NLA solvers, if any

    return mNlaSolverData.solversListProperty.isEmpty()?QStringList():mNlaSolverData.solversListProperty.value->list();
}

//==============================================================================

bool SingleCellSimulationViewInformationSolversWidget::doListPropertyChanged(const SingleCellSimulationViewInformationSolversWidgetData &pSolverData,
                                                                             const QString &pSolverName,
                                                                             const bool &pForceHandling)
{
    // By default, we don't handle the change in the list property

    bool res = false;

    // Check whether the list property that got changed is the one we are after

    if (   (pSolverData.solversListProperty == currentProperty())
        || pForceHandling) {
        // It is the list property we are after or we want to force the
        // handling, so update our result

        res = true;

        // Go through the different properties for the given type of solver and
        // show/hide whatever needs showing/hiding

        QMap<QString, Core::Properties>::const_iterator iter = pSolverData.solversProperties.constBegin();

        while (iter != pSolverData.solversProperties.constEnd()) {
            bool propertyVisible = !iter.key().compare(pSolverName);

            foreach (const Core::Property &property, iter.value())
                setPropertyVisible(property, propertyVisible);

            ++iter;
        }
    }

    // Return our result

    return res;
}

//==============================================================================

void SingleCellSimulationViewInformationSolversWidget::listPropertyChanged(const QString &pValue)
{
    // Try, for the ODE, DAE and NLA solvers list property, to handle the change
    // in the list property

    if (!doListPropertyChanged(mOdeSolverData, pValue))
        if (!doListPropertyChanged(mDaeSolverData, pValue))
            doListPropertyChanged(mNlaSolverData, pValue);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

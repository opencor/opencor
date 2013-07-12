//==============================================================================
// Single cell view information solvers widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "coreutils.h"
#include "singlecellviewinformationsolverswidget.h"
#include "singlecellviewsimulation.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationSolversWidgetData::SingleCellViewInformationSolversWidgetData(Core::Property *pSolversProperty,
                                                                                       Core::Property *pSolversListProperty,
                                                                                       const QMap<QString, Core::Properties> &pSolversProperties) :
    mSolversProperty(pSolversProperty),
    mSolversListProperty(pSolversListProperty),
    mSolversProperties(pSolversProperties)
{
}

//==============================================================================

Core::Property * SingleCellViewInformationSolversWidgetData::solversProperty() const
{
    // Return our solvers property

    return mSolversProperty;
}

//==============================================================================

Core::Property * SingleCellViewInformationSolversWidgetData::solversListProperty() const
{
    // Return our solvers list property

    return mSolversListProperty;
}

//==============================================================================

QMap<QString, Core::Properties> SingleCellViewInformationSolversWidgetData::solversProperties() const
{
    // Return our solvers properties

    return mSolversProperties;
}

//==============================================================================

SingleCellViewInformationSolversWidget::SingleCellViewInformationSolversWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent),
    mOdeSolverData(0),
    mDaeSolverData(0),
    mNlaSolverData(0),
    mGuiStates(QMap<QString, Core::PropertyEditorWidgetGuiState *>()),
    mDefaultGuiState(0),
    mDescriptions(QMap<Core::Property *, Descriptions>())
{
    // Keep track of changes to list properties

    connect(this, SIGNAL(listPropertyChanged(const QString &)),
            this, SLOT(solverChanged(const QString &)));
}

//==============================================================================

SingleCellViewInformationSolversWidget::~SingleCellViewInformationSolversWidget()
{
    // Delete some internal objects

    delete mOdeSolverData;
    delete mDaeSolverData;
    delete mNlaSolverData;

    resetAllGuiStates();
}

//==============================================================================

void SingleCellViewInformationSolversWidget::retranslateUi()
{
    // Update our property names

    if (mOdeSolverData) {
        mOdeSolverData->solversProperty()->setName(tr("ODE solver"));
        mOdeSolverData->solversListProperty()->setName(tr("Name"));

        mOdeSolverData->solversListProperty()->setEmptyListValue(tr("None available"));
    }

    if (mDaeSolverData) {
        mDaeSolverData->solversProperty()->setName(tr("DAE solver"));
        mDaeSolverData->solversListProperty()->setName(tr("Name"));

        mDaeSolverData->solversListProperty()->setEmptyListValue(tr("None available"));
    }

    if (mNlaSolverData) {
        mNlaSolverData->solversProperty()->setName(tr("NLA solver"));
        mNlaSolverData->solversListProperty()->setName(tr("Name"));

        mNlaSolverData->solversListProperty()->setEmptyListValue(tr("None available"));
    }

    // Update the name of our various properties, should they have a description
    // associated with them
    // Note: this is effectively to have the description of our solvers'
    //       properties properly updated...

    foreach (Core::Property *property, properties())
        if (mDescriptions.contains(property)) {
            // The property has a description associated with it, so retrieve
            // the version, if any, which corresponds to our current locale

            Descriptions descriptions = mDescriptions.value(property);
            QString description = descriptions.value(Core::locale());

            if (description.isEmpty())
                // No description exists for the current locale, so  retrieve
                // the english description (which, hopefully, should exist)

                description = descriptions.value("en");

            // Set the name of the property to the description

            property->setName(description);
        }

    // Default retranslation
    // Note: we must do it last since we set the empty list value of some
    //       properties above...

    PropertyEditorWidget::retranslateUi();
}

//==============================================================================

void SingleCellViewInformationSolversWidget::resetAllGuiStates()
{
    // Reset all our GUI states including our default one

    foreach (Core::PropertyEditorWidgetGuiState *guiState, mGuiStates)
        delete guiState;

    mGuiStates.clear();

    delete mDefaultGuiState;
}

//==============================================================================

SingleCellViewInformationSolversWidgetData * SingleCellViewInformationSolversWidget::addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                                                         const Solver::Type &pSolverType)
{
    // Make sure that we have at least one solver interface

    if (pSolverInterfaces.isEmpty())
        return 0;

    // Retrieve the name of the solvers which type is the one in which we are
    // interested

    Core::Property *solversProperty = 0;
    Core::Property *solversListProperty = 0;
    QStringList solvers = QStringList();
    QMap<QString, Core::Properties> solversProperties = QMap<QString, Core::Properties>();

    foreach (SolverInterface *solverInterface, pSolverInterfaces)
        if (solverInterface->type() == pSolverType) {
            // Keep track of the solver's name

            solvers << solverInterface->name();

            // Add our solvers section property and list property for our
            // solvers, if needed

            if (!solversProperty) {
                solversProperty = addSectionProperty();

                solversListProperty = addListProperty(solversProperty);
            }

            // Add the solver's properties

            Core::Property *property;
            Core::Properties properties = Core::Properties();

            foreach (const Solver::Property &solverInterfaceProperty,
                     solverInterface->properties()) {
                // Add the solver's property and set its default value

                switch (solverInterfaceProperty.type()) {
                case Solver::Double:
                    property = addDoubleProperty(solverInterfaceProperty.defaultValue().toDouble(),
                                                 solversProperty);

                    property->setId(solverInterfaceProperty.id());

                    break;
                default:
                    // Solver::Integer

                    property = addIntegerProperty(solverInterfaceProperty.defaultValue().toInt(),
                                                  solversProperty);

                    property->setId(solverInterfaceProperty.id());
                }

                // Make the property editable

                property->setEditable(true);

                // Set the solver's property's 'unit', if needed

                if (solverInterfaceProperty.hasVoiUnit())
                    property->setUnit("???");
                    // Note: to assign a non-empty string to our unit item is
                    //       just a way for us to make sure that the property's
                    //       will get initialised (see setPropertiesUnit())...

                // Keep track of the solver's property

                properties << property;

                // Keep track of the solver's property's descriptions

                mDescriptions.insert(property, solverInterfaceProperty.descriptions());
            }

            // Keep track of the solver's properties

            solversProperties.insert(solverInterface->name(), properties);
        }

    // Check whether we have at least one solver

    if (solvers.isEmpty()) {
        // We don't have any solver, so...

        return 0;
    } else {
        // We have at least one solver, so sort our list

        solvers.sort();

        // Assign the list of solvers to our list property

        solversListProperty->setListValue(solvers);

        // Create and return our solver data

        return new SingleCellViewInformationSolversWidgetData(solversProperty,
                                                              solversListProperty,
                                                              solversProperties);
    }
}

//==============================================================================

void SingleCellViewInformationSolversWidget::setSolverInterfaces(const SolverInterfaces &pSolverInterfaces)
{
    // Remove all our properties

    removeAllProperties();

    // Add properties for our different solvers

    delete mOdeSolverData;
    delete mDaeSolverData;
    delete mNlaSolverData;

    mDescriptions.clear();

    mOdeSolverData = addSolverProperties(pSolverInterfaces, Solver::Ode);
    mDaeSolverData = addSolverProperties(pSolverInterfaces, Solver::Dae);
    mNlaSolverData = addSolverProperties(pSolverInterfaces, Solver::Nla);

    // Show/hide the relevant properties

    if (mOdeSolverData)
        doSolverChanged(mOdeSolverData, mOdeSolverData->solversListProperty()->value(), true);

    if (mDaeSolverData)
        doSolverChanged(mDaeSolverData, mDaeSolverData->solversListProperty()->value(), true);

    if (mNlaSolverData)
        doSolverChanged(mNlaSolverData, mNlaSolverData->solversListProperty()->value(), true);

    // Expand all our properties

    expandAll();

    // Clear any track of previous GUI states and retrieve our default GUI state

    resetAllGuiStates();

    mDefaultGuiState = guiState();
}

//==============================================================================

void SingleCellViewInformationSolversWidget::setPropertiesUnit(SingleCellViewInformationSolversWidgetData *pSolverData,
                                                               const QString &pVoiUnit)
{
    // Make sure that we have some solver's data

    if (!pSolverData)
        return;

    // Go through the solvers' properties and set the unit of the relevant ones

    foreach (const Core::Properties &properties, pSolverData->solversProperties())
        foreach (Core::Property *property, properties)
            if (!property->unit().isEmpty())
                property->setUnit(pVoiUnit);
}

//==============================================================================

void SingleCellViewInformationSolversWidget::initialize(const QString &pFileName,
                                                        CellMLSupport::CellmlFileRuntime *pRuntime,
                                                        SingleCellViewSimulation *pSimulation)
{
    // Retrieve and initialise our GUI state

    setGuiState(mGuiStates.contains(pFileName)?
                    mGuiStates.value(pFileName):
                    mDefaultGuiState);

    // Make sure that the CellML file runtime is valid

    if (pRuntime->isValid()) {
        // Show/hide the ODE/DAE solver information

        if (mOdeSolverData)
            mOdeSolverData->solversProperty()->setVisible(pRuntime->needOdeSolver());

        if (mDaeSolverData)
            mDaeSolverData->solversProperty()->setVisible(pRuntime->needDaeSolver());

        // Show/hide the NLA solver information

        if (mNlaSolverData)
            mNlaSolverData->solversProperty()->setVisible(pRuntime->needNlaSolver());
    }

    // Set the unit of our different properties, if needed

    QString voiUnit = pRuntime->variableOfIntegration()->unit();

    setPropertiesUnit(mOdeSolverData, voiUnit);
    setPropertiesUnit(mDaeSolverData, voiUnit);
    setPropertiesUnit(mNlaSolverData, voiUnit);

    // Initialise our simulation's NLA solver's properties, so that we can then
    // properly reset our simulation the first time round

    if (mNlaSolverData) {
        pSimulation->data()->setNlaSolverName(mNlaSolverData->solversListProperty()->value(), false);

        foreach (Core::Property *property, mNlaSolverData->solversProperties().value(pSimulation->data()->nlaSolverName()))
            pSimulation->data()->addNlaSolverProperty(property->id(),
                                                      (property->type() == Core::Property::Integer)?
                                                          property->integerValue():
                                                          property->doubleValue(),
                                                      false);
    }
}

//==============================================================================

void SingleCellViewInformationSolversWidget::backup(const QString &pFileName)
{
    // Keep track of our GUI state

    mGuiStates.insert(pFileName, guiState());
}

//==============================================================================

void SingleCellViewInformationSolversWidget::finalize(const QString &pFileName)
{
    // Remove any track of our GUI state

    mGuiStates.remove(pFileName);
}

//==============================================================================

QStringList SingleCellViewInformationSolversWidget::odeSolvers() const
{
    // Return the available ODE solvers, if any

    return mOdeSolverData?mOdeSolverData->solversListProperty()->listValue():QStringList();
}

//==============================================================================

QStringList SingleCellViewInformationSolversWidget::daeSolvers() const
{
    // Return the available DAE solvers, if any

    return mDaeSolverData?mDaeSolverData->solversListProperty()->listValue():QStringList();
}

//==============================================================================

QStringList SingleCellViewInformationSolversWidget::nlaSolvers() const
{
    // Return the available NLA solvers, if any

    return mNlaSolverData?mNlaSolverData->solversListProperty()->listValue():QStringList();
}

//==============================================================================

SingleCellViewInformationSolversWidgetData * SingleCellViewInformationSolversWidget::odeSolverData() const
{
    // Return our ODE solver data

    return mOdeSolverData;
}

//==============================================================================

SingleCellViewInformationSolversWidgetData * SingleCellViewInformationSolversWidget::daeSolverData() const
{
    // Return our DAE solver data

    return mDaeSolverData;
}

//==============================================================================

SingleCellViewInformationSolversWidgetData * SingleCellViewInformationSolversWidget::nlaSolverData() const
{
    // Return our NLA solver data

    return mNlaSolverData;
}

//==============================================================================

bool SingleCellViewInformationSolversWidget::doSolverChanged(SingleCellViewInformationSolversWidgetData *pSolverData,
                                                             const QString &pSolverName,
                                                             const bool &pForceHandling)
{
    // Make sure that we have some solver's data

    if (!pSolverData)
        return false;

    // By default, we don't handle the change in the list property

    bool res = false;

    // Check whether the list property that got changed is the one we are after

    if (   (pSolverData->solversListProperty() == currentProperty())
        || pForceHandling) {
        // It is the list property we are after or we want to force the
        // handling, so update our result

        res = true;

        // Go through the different properties for the given type of solver and
        // show/hide whatever needs showing/hiding

        for (QMap<QString, Core::Properties>::ConstIterator iter = pSolverData->solversProperties().constBegin(),
                                                            iterEnd = pSolverData->solversProperties().constEnd();
             iter != iterEnd; ++iter) {
            bool propertyVisible = !iter.key().compare(pSolverName);

            foreach (Core::Property *property, iter.value())
                property->setVisible(propertyVisible);
        }
    }

    // Return our result

    return res;
}

//==============================================================================

void SingleCellViewInformationSolversWidget::solverChanged(const QString &pValue)
{
    // Try, for the ODE, DAE and NLA solvers list property, to handle the change
    // in the list property

    if (!doSolverChanged(mOdeSolverData, pValue))
        if (!doSolverChanged(mDaeSolverData, pValue))
            doSolverChanged(mNlaSolverData, pValue);
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

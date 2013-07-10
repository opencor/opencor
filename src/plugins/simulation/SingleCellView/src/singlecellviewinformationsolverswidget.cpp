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
    // Update the tool tip of any property which value gets changed by the user

    connect(this, SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(updatePropertyToolTip(Core::Property *)));
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
        setStringPropertyItem(mOdeSolverData->solversProperty()->name(), tr("ODE solver"));
        setStringPropertyItem(mOdeSolverData->solversListProperty()->name(), tr("Name"));

        mOdeSolverData->solversListProperty()->value()->setEmptyListValue(tr("None available"));
    }

    if (mDaeSolverData) {
        setStringPropertyItem(mDaeSolverData->solversProperty()->name(), tr("DAE solver"));
        setStringPropertyItem(mDaeSolverData->solversListProperty()->name(), tr("Name"));

        mDaeSolverData->solversListProperty()->value()->setEmptyListValue(tr("None available"));
    }

    if (mNlaSolverData) {
        setStringPropertyItem(mNlaSolverData->solversProperty()->name(), tr("NLA solver"));
        setStringPropertyItem(mNlaSolverData->solversListProperty()->name(), tr("Name"));

        mNlaSolverData->solversListProperty()->value()->setEmptyListValue(tr("None available"));
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

            setStringPropertyItem(property->name(), description);
        }

    // Retranslate the tool tip of all our solvers' properties

    updateToolTips();

    // Default retranslation
    // Note: we must do it last since we set the empty list value of some
    //       properties above...

    PropertyEditorWidget::retranslateUi();
}

//==============================================================================

void SingleCellViewInformationSolversWidget::updatePropertyToolTip(Core::Property *pProperty)
{
    // Update the tool tip of the given property

    if (pProperty->name()->type() != Core::PropertyItem::Section) {
        // We are dealing with a property (as opposed to a section), so we can
        // update its tool tip

        QString propertyToolTip = pProperty->name()->text()+tr(": ");

        if (pProperty->value()->text().isEmpty())
            propertyToolTip += "???";
        else
            propertyToolTip += pProperty->value()->text();

        if (!pProperty->unit()->text().isEmpty())
            propertyToolTip += " "+pProperty->unit()->text();

        pProperty->name()->setToolTip(propertyToolTip);
        pProperty->value()->setToolTip(propertyToolTip);
        pProperty->unit()->setToolTip(propertyToolTip);
    }
}

//==============================================================================

void SingleCellViewInformationSolversWidget::updateToolTips()
{
    // Update the tool tip of all our solvers' properties

    foreach (Core::Property *property, properties())
        updatePropertyToolTip(property);
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

    // Add our section property

    Core::Property *solversProperty = addSectionProperty();

    // Add our list property for the solvers

    Core::Property *solversListProperty = addListProperty(solversProperty);

    // Retrieve the name of the solvers which type is the one in whhich we are
    // interested

    QStringList solvers = QStringList();
    QMap<QString, Core::Properties> solversProperties = QMap<QString, Core::Properties>();

    foreach (SolverInterface *solverInterface, pSolverInterfaces)
        if (solverInterface->type() == pSolverType) {
            // Keep track of the solver's name

            solvers << solverInterface->name();

            // Add the solver's properties

            Core::Property *property;
            Core::Properties properties = Core::Properties();

            foreach (const Solver::Property &solverInterfaceProperty,
                     solverInterface->properties()) {
                // Add the solver's property and set its default value

                switch (solverInterfaceProperty.type()) {
                case Solver::Double:
                    property = addDoubleProperty(solversProperty);

                    property->setId(solverInterfaceProperty.id());

                    setDoublePropertyItem(property->value(), solverInterfaceProperty.defaultValue().toDouble());

                    break;
                default:
                    // Solver::Integer

                    property = addIntegerProperty(solversProperty);

                    property->setId(solverInterfaceProperty.id());

                    setIntegerPropertyItem(property->value(), solverInterfaceProperty.defaultValue().toInt());
                }

                // Make the property editable

                property->setEditable(true);

                // Set the solver's property's 'unit', if needed

                if (solverInterfaceProperty.hasVoiUnit())
                    setStringPropertyItem(property->unit(), "???");
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

    // Sort our list of solvers

    solvers.sort();

    // Add the list of solvers to our list property value item

    solversListProperty->value()->setList(solvers);

    // Keep track of changes to list properties

    connect(this, SIGNAL(listPropertyChanged(const QString &)),
            this, SLOT(solverChanged(const QString &)));

    // Return our solver data

    return new SingleCellViewInformationSolversWidgetData(solversProperty,
                                                          solversListProperty,
                                                          solversProperties);
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
        doSolverChanged(mOdeSolverData, mOdeSolverData->solversListProperty()->value()->text(), true);

    if (mDaeSolverData)
        doSolverChanged(mDaeSolverData, mDaeSolverData->solversListProperty()->value()->text(), true);

    if (mNlaSolverData)
        doSolverChanged(mNlaSolverData, mNlaSolverData->solversListProperty()->value()->text(), true);

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
            if (!property->unit()->text().isEmpty())
                setStringPropertyItem(property->unit(), pVoiUnit);
}

//==============================================================================

void SingleCellViewInformationSolversWidget::initialize(const QString &pFileName,
                                                        CellMLSupport::CellmlFileRuntime *pRuntime,
                                                        SingleCellViewSimulationData *pSimulationData)
{
    // Retrieve and initialise our GUI state

    setGuiState(mGuiStates.contains(pFileName)?
                    mGuiStates.value(pFileName):
                    mDefaultGuiState);

    // Make sure that the CellML file runtime is valid

    if (pRuntime->isValid()) {
        // Show/hide the ODE/DAE solver information

        if (mOdeSolverData)
            setPropertyVisible(mOdeSolverData->solversProperty(), pRuntime->needOdeSolver());

        if (mDaeSolverData)
            setPropertyVisible(mDaeSolverData->solversProperty(), pRuntime->needDaeSolver());

        // Show/hide the NLA solver information

        if (mNlaSolverData)
            setPropertyVisible(mNlaSolverData->solversProperty(), pRuntime->needNlaSolver());
    }

    // Set the unit of our different properties, if needed

    QString voiUnit = pRuntime->variableOfIntegration()->unit();

    setPropertiesUnit(mOdeSolverData, voiUnit);
    setPropertiesUnit(mDaeSolverData, voiUnit);
    setPropertiesUnit(mNlaSolverData, voiUnit);

    // Update the tool tip of all our solvers' properties

    updateToolTips();

    // Initialise our simulation's NLA solver's properties, so that we can then
    // properly reset our simulation the first time round

    if (mNlaSolverData) {
        pSimulationData->setNlaSolverName(mNlaSolverData->solversListProperty()->value()->text(), false);

        foreach (Core::Property *property, mNlaSolverData->solversProperties().value(pSimulationData->nlaSolverName()))
            pSimulationData->addNlaSolverProperty(property->id(),
                                                  (property->value()->type() == Core::PropertyItem::Integer)?
                                                      Core::PropertyEditorWidget::integerPropertyItem(property->value()):
                                                      Core::PropertyEditorWidget::doublePropertyItem(property->value()),
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

    return mOdeSolverData?mOdeSolverData->solversListProperty()->value()->list():QStringList();
}

//==============================================================================

QStringList SingleCellViewInformationSolversWidget::daeSolvers() const
{
    // Return the available DAE solvers, if any

    return mDaeSolverData?mDaeSolverData->solversListProperty()->value()->list():QStringList();
}

//==============================================================================

QStringList SingleCellViewInformationSolversWidget::nlaSolvers() const
{
    // Return the available NLA solvers, if any

    return mNlaSolverData?mNlaSolverData->solversListProperty()->value()->list():QStringList();
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
                setPropertyVisible(property, propertyVisible);
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

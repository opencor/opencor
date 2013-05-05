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

SingleCellViewInformationSolversWidgetData::SingleCellViewInformationSolversWidgetData
(
 Core::Property *pSolversProperty,
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
    mDefaultGuiState(0)
{
    setupBuiltInSolvers();

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
void
SingleCellViewInformationSolversWidget::setupBuiltInSolvers()
{
    Core::Property* solversProperty = addSectionProperty();
    Core::Property* solversListProperty = addListProperty(QString(), solversProperty);
    
    QStringList solvers = QStringList();
    solvers << "";
    
    solvers.sort();
    solversListProperty->value()->setList(solvers);
    

    addProperty(type, id, true, false, );
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

    // Retranslate the tool tip of all our solvers' properties

    updatePropertiesToolTip();

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

void SingleCellViewInformationSolversWidget::updatePropertiesToolTip()
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

SingleCellViewInformationSolversWidgetData * SingleCellViewInformationSolversWidget::setupBuiltInSolvers()
{
    // Add our section property
    Core::Property *solverSectionProperty = addSectionProperty();
    setStringPropertyItem(solverSectionProperty->name(), tr("Solver"));

    // Add our list property for the solvers
    Core::Property *selectSolverProperty =
        addListProperty(QString(), solverSectionProperty);
    QStringList solverNames = QStringList();
    solverNames << "IDA" << "CVODE";

    // Sort our list of solvers
    solverNames.sort();

    // Add the list of solvers to our list property value item
    solverSelectProperty->value()->setList(solverNames);

    Core::Property * relTol, * absTol, * maxStep, * maxSteps;

    // Note: These properties are owned by the PropertyEditorWidget parent class
    // so we don't need to delete them.
    {
        relTol = addDoubleProperty("relTol", true, false,
                                   solverSectionProperty);
        setStringPropertyItem(relTol->name(), tr("Relative Error Tolerance"));
        setDoublePropertyItem(relTol->value(), 1E-6);
    }
    {
        absTol = addDoubleProperty("absTol", true, false,
                                   solverSectionProperty);
        setStringPropertyItem(absTol->name(),
                              tr("Absolute Error Tolerance"));
        setDoublePropertyItem(absTol->value(), 1E-6);
    }
    {
        // property is owned by the list.
        maxStep = addDoubleProperty("maxStep", true, false,
                                    solverSectionProperty);
        setStringPropertyItem(maxStep->name(),
                              tr("Maximum Integrator Step"));
        setDoublePropertyItem(maxStep->value(), 0.0);
        // Note: assigning a non-empty string to our unit item is
        //       just a way to make sure that the property's unit
        //       string gets initialised (see setPropertiesUnit())...
        setStringPropertyItem(maxStep->unit(), "???");
    }
    {
        // property is owned by the list.
        maxSteps = addIntegerProperty("maxSteps", true, false,
                                    solverSectionProperty);
        setStringPropertyItem(maxSteps->name(),
                              tr("Maximum Number of Steps"));
        setIntegerPropertyItem(maxSteps->value(), 10000);
    }

    // Build a map which says which properties are used by each integrator.
    QMap<QString, Core::Properties> solversProperties = QMap<QString, Core::Properties>();
    {
        Core::Properties properties;
        properties << relTol << absTol << maxStep << maxSteps;
        solversProperties.insert("IDA", properties);
    }
    {
        Core::Properties properties;
        properties << relTol << absTol << maxStep << maxSteps;
        solversProperties.insert("CVODE", properties);
    }

    // Keep track of changes to list properties

    connect(this, SIGNAL(listPropertyChanged(const QString &)),
            this, SLOT(solverChanged(const QString &)));

    // Return our solver data

    return new SingleCellViewInformationSolversWidgetData(solverSectionProperty,
                                                          selectSolverProperty,
                                                          solversProperties);
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
    // Make sure that we have a CellML file runtime

    if (!pRuntime)
        return;

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

    updatePropertiesToolTip();

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

//==============================================================================
// Single cell view information simulation widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewsimulation.h"

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationSimulationWidget::SingleCellViewInformationSimulationWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent),
    mGuiStates(QMap<QString, Core::PropertyEditorWidgetGuiState *>())
{
    // Populate our property editor

    mStartingPointProperty = addDoubleProperty(QString(), true, false);
    mEndingPointProperty   = addDoubleProperty(QString(), true, false);
    mPointIntervalProperty = addDoubleProperty(QString(), true, false);

    // Initialise our property values

    setDoublePropertyItem(mStartingPointProperty->value(), 0.0);
    setDoublePropertyItem(mEndingPointProperty->value(), 1000.0);
    setDoublePropertyItem(mPointIntervalProperty->value(), 1.0);

    // Update the tool tip of any property which value gets changed by the user

    connect(this, SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(updatePropertyToolTip(Core::Property *)));

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();

    // Retrieve our default GUI state

    mDefaultGuiState = guiState();
}

//==============================================================================

SingleCellViewInformationSimulationWidget::~SingleCellViewInformationSimulationWidget()
{
    // Delete some internal objects

    foreach (Core::PropertyEditorWidgetGuiState *guiState, mGuiStates)
        delete guiState;

    delete mDefaultGuiState;
}

//==============================================================================

void SingleCellViewInformationSimulationWidget::retranslateUi()
{
    // Default retranslation

    PropertyEditorWidget::retranslateUi();

    // Update our property names

    setStringPropertyItem(mStartingPointProperty->name(), tr("Starting point"));
    setStringPropertyItem(mEndingPointProperty->name(), tr("Ending point"));
    setStringPropertyItem(mPointIntervalProperty->name(), tr("Point interval"));

    // Retranslate the tool tip of all our properties

    updatePropertiesToolTip();
}

//==============================================================================

void SingleCellViewInformationSimulationWidget::updatePropertyToolTip(Core::Property *pProperty)
{
    // Update the tool tip of the given property

    QString propertyToolTip = pProperty->name()->text()+tr(": ");

    if (pProperty->value()->text().isEmpty())
        propertyToolTip += "???";
    else
        propertyToolTip += pProperty->value()->text();

    propertyToolTip += " "+pProperty->unit()->text();

    pProperty->name()->setToolTip(propertyToolTip);
    pProperty->value()->setToolTip(propertyToolTip);
    pProperty->unit()->setToolTip(propertyToolTip);
}

//==============================================================================

void SingleCellViewInformationSimulationWidget::updatePropertiesToolTip()
{
    // Update the tool tip of all our properties

    foreach (Core::Property *property, properties())
        updatePropertyToolTip(property);
}

//==============================================================================

void SingleCellViewInformationSimulationWidget::initialize(const QString &pFileName,
                                                           CellMLSupport::CellmlFileRuntime *pRuntime,
                                                           SingleCellViewSimulationData *pSimulationData)
{
    // Retrieve and initialise our GUI state

    setGuiState(mGuiStates.contains(pFileName)?
                    mGuiStates.value(pFileName):
                    mDefaultGuiState);

    // Iniialise the unit of our different properties

    QString unit = pRuntime->variableOfIntegration()->unit();

    setStringPropertyItem(mStartingPointProperty->unit(), unit);
    setStringPropertyItem(mEndingPointProperty->unit(), unit);
    setStringPropertyItem(mPointIntervalProperty->unit(), unit);

    // Initialise our simulation's starting point so that we can then properly
    // reset our simulation the first time round

    pSimulationData->setStartingPoint(Core::PropertyEditorWidget::doublePropertyItem(mStartingPointProperty->value()), false);

    // Update the tool tip of all our properties

    updatePropertiesToolTip();
}

//==============================================================================

void SingleCellViewInformationSimulationWidget::backup(const QString &pFileName)
{
    // Keep track of our GUI state

    mGuiStates.insert(pFileName, guiState());
}

//==============================================================================

void SingleCellViewInformationSimulationWidget::finalize(const QString &pFileName)
{
    // Remove any track of our GUI state

    mGuiStates.remove(pFileName);
}

//==============================================================================

Core::Property * SingleCellViewInformationSimulationWidget::startingPointProperty() const
{
    // Return our starting point property

    return mStartingPointProperty;
}

//==============================================================================

Core::Property * SingleCellViewInformationSimulationWidget::endingPointProperty() const
{
    // Return our ending point property

    return mEndingPointProperty;
}

//==============================================================================

Core::Property * SingleCellViewInformationSimulationWidget::pointIntervalProperty() const
{
    // Return our point interval property

    return mPointIntervalProperty;
}

//==============================================================================

double SingleCellViewInformationSimulationWidget::startingPoint() const
{
    // Return our starting point

    return doublePropertyItem(mStartingPointProperty->value());
}

//==============================================================================

double SingleCellViewInformationSimulationWidget::endingPoint() const
{
    // Return our ending point

    return doublePropertyItem(mEndingPointProperty->value());
}

//==============================================================================

double SingleCellViewInformationSimulationWidget::pointInterval() const
{
    // Return our point interval

    return doublePropertyItem(mPointIntervalProperty->value());
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

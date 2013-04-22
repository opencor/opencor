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

    mStartingPointProperty = addDoubleProperty(true, false);
    mEndingPointProperty   = addDoubleProperty(true, false);
    mPointIntervalProperty = addDoubleProperty(true, false);

    // Initialise our property values

    setDoublePropertyItem(mStartingPointProperty->value(), 0.0);
    setDoublePropertyItem(mEndingPointProperty->value(), 1000.0);
    setDoublePropertyItem(mPointIntervalProperty->value(), 1.0);

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
}

//==============================================================================

void SingleCellViewInformationSimulationWidget::initialize(const QString &pFileName,
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

    // Iniialise the unit of our different properties

    QString unit = pRuntime->variableOfIntegration()->unit();

    setStringPropertyItem(mStartingPointProperty->unit(), unit);
    setStringPropertyItem(mEndingPointProperty->unit(), unit);
    setStringPropertyItem(mPointIntervalProperty->unit(), unit);

    // Initialise our simulation's starting point so that we can then properly
    // reset our simulation the first time round

    pSimulationData->setStartingPoint(Core::PropertyEditorWidget::doublePropertyItem(mStartingPointProperty->value()), false);
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

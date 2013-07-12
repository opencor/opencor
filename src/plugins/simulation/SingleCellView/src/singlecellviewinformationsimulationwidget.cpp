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

    mStartingPointProperty = addDoubleProperty(0.0);
    mEndingPointProperty   = addDoubleProperty(1000.0);
    mPointIntervalProperty = addDoubleProperty(1.0);

    mStartingPointProperty->setEditable(true);
    mEndingPointProperty->setEditable(true);
    mPointIntervalProperty->setEditable(true);

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

    mStartingPointProperty->setName(tr("Starting point"));
    mEndingPointProperty->setName(tr("Ending point"));
    mPointIntervalProperty->setName(tr("Point interval"));
}

//==============================================================================

void SingleCellViewInformationSimulationWidget::initialize(const QString &pFileName,
                                                           CellMLSupport::CellmlFileRuntime *pRuntime,
                                                           SingleCellViewSimulation *pSimulation)
{
    // Retrieve and initialise our GUI state

    setGuiState(mGuiStates.contains(pFileName)?
                    mGuiStates.value(pFileName):
                    mDefaultGuiState);

    // Iniialise the unit of our different properties

    QString unit = pRuntime->variableOfIntegration()->unit();

    mStartingPointProperty->setUnit(unit);
    mEndingPointProperty->setUnit(unit);
    mPointIntervalProperty->setUnit(unit);

    // Initialise our simulation's starting point so that we can then properly
    // reset our simulation the first time round

    pSimulation->data()->setStartingPoint(mStartingPointProperty->doubleValue(), false);
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

    return mStartingPointProperty->doubleValue();
}

//==============================================================================

double SingleCellViewInformationSimulationWidget::endingPoint() const
{
    // Return our ending point

    return mEndingPointProperty->doubleValue();
}

//==============================================================================

double SingleCellViewInformationSimulationWidget::pointInterval() const
{
    // Return our point interval

    return mPointIntervalProperty->doubleValue();
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

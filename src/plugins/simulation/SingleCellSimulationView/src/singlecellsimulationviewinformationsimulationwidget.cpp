//==============================================================================
// Single cell simulation view information simulation widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "cellmlfilevariable.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationSimulationWidget::SingleCellSimulationViewInformationSimulationWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent),
    mGuiStates(QMap<QString, Core::PropertyEditorWidgetGuiState>())
{
    // Populate our property editor

    mStartingPointProperty = addDoubleProperty();
    mEndingPointProperty   = addDoubleProperty();
    mPointIntervalProperty = addDoubleProperty();

    // Initialise our property values

    setDoublePropertyItem(mStartingPointProperty.value, 0.0);
    setDoublePropertyItem(mEndingPointProperty.value, 1000.0);
    setDoublePropertyItem(mPointIntervalProperty.value, 1.0);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();

    // Select our first property

    selectFirstProperty();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::retranslateUi()
{
    // Default retranslation

    PropertyEditorWidget::retranslateUi();

    // Update our property names

    setNonEditablePropertyItem(mStartingPointProperty.name, tr("Starting point"));
    setNonEditablePropertyItem(mEndingPointProperty.name, tr("Ending point"));
    setNonEditablePropertyItem(mPointIntervalProperty.name, tr("Point interval"));
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::initialize(const QString &pFileName,
                                                                     CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime)
{
    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Retrieve and initialise our GUI state

    setGuiState(mGuiStates.value(pFileName));

    // Iniialise the unit of our different properties

    QString unit = pCellmlFileRuntime->variableOfIntegration()->unit();

    setNonEditablePropertyItem(mStartingPointProperty.unit, unit);
    setNonEditablePropertyItem(mEndingPointProperty.unit, unit);
    setNonEditablePropertyItem(mPointIntervalProperty.unit, unit);
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::finalize(const QString &pFileName)
{
    // Keep track of our GUI state

    mGuiStates.insert(pFileName, guiState());
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::startingPoint() const
{
    // Return our starting point

    return doublePropertyItem(mStartingPointProperty.value);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::endingPoint() const
{
    // Return our ending point

    return doublePropertyItem(mEndingPointProperty.value);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::pointInterval() const
{
    // Return our point interval

    return doublePropertyItem(mPointIntervalProperty.value);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

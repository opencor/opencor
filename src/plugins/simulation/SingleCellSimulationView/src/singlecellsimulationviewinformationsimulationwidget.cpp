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

SingleCellSimulationViewInformationSimulationWidgetData::SingleCellSimulationViewInformationSimulationWidgetData() :
    guiState(Core::PropertyEditorWidgetGuiState()),
    startingPoint(0.0),
    endingPoint(1000.0),
    pointInterval(1.0)
{
}

//==============================================================================

SingleCellSimulationViewInformationSimulationWidget::SingleCellSimulationViewInformationSimulationWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent)
{
    // Populate our property editor

    mStartingPointProperty = addDoubleProperty();
    mEndingPointProperty   = addDoubleProperty();
    mPointIntervalProperty = addDoubleProperty();

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

    // Initialise our GUI state

    SingleCellSimulationViewInformationSimulationWidgetData data = mData.value(pFileName);

    setGuiState(data.guiState);

    // Initialise the value of our different properties

    setDoublePropertyItem(mStartingPointProperty.value, data.startingPoint);
    setDoublePropertyItem(mEndingPointProperty.value, data.endingPoint);
    setDoublePropertyItem(mPointIntervalProperty.value, data.pointInterval);

    // Iniialise the unit of our different properties

    QString unit = pCellmlFileRuntime->variableOfIntegration()->unit();

    setNonEditablePropertyItem(mStartingPointProperty.unit, unit);
    setNonEditablePropertyItem(mEndingPointProperty.unit, unit);
    setNonEditablePropertyItem(mPointIntervalProperty.unit, unit);
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::finalize(const QString &pFileName)
{
    // Cancel any property editing, if any

    cancelPropertyEditing();

    // Retrieve our GUI state

    SingleCellSimulationViewInformationSimulationWidgetData data = mData.value(pFileName);

    data.guiState = guiState();

    // Retrieve the value of our different properties

    data.startingPoint = doublePropertyItem(mStartingPointProperty.value);
    data.endingPoint   = doublePropertyItem(mEndingPointProperty.value);
    data.pointInterval = doublePropertyItem(mPointIntervalProperty.value);

    // Keep track of our data

    mData.insert(pFileName, data);
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

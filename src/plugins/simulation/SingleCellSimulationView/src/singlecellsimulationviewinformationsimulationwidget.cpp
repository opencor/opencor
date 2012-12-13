//==============================================================================
// Single cell simulation view information simulation widget
//==============================================================================

#include "singlecellsimulationviewinformationsimulationwidget.h"

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationSimulationWidget::SingleCellSimulationViewInformationSimulationWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent)
{
    // Populate our data model

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

void SingleCellSimulationViewInformationSimulationWidget::setUnit(const QString &pUnit)
{
    // Set the unit for our different properties, if needed

    setNonEditablePropertyItem(mStartingPointProperty.unit, pUnit);
    setNonEditablePropertyItem(mEndingPointProperty.unit, pUnit);
    setNonEditablePropertyItem(mPointIntervalProperty.unit, pUnit);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::startingPoint() const
{
    // Return our starting point

    return doublePropertyItem(mStartingPointProperty.value);
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setStartingPoint(const double &pValue)
{
    // Set our starting point

    setDoublePropertyItem(mStartingPointProperty.value, pValue);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::endingPoint() const
{
    // Return our ending point

    return doublePropertyItem(mEndingPointProperty.value);
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setEndingPoint(const double &pValue)
{
    // Set our ending point

    setDoublePropertyItem(mEndingPointProperty.value, pValue);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::pointInterval() const
{
    // Return our point interval

    return doublePropertyItem(mPointIntervalProperty.value);
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setPointInterval(const double &pValue)
{
    // Set our point interval

    setDoublePropertyItem(mPointIntervalProperty.value, pValue);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

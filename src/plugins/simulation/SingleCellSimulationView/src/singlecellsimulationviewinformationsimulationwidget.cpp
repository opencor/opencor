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

    mStartingPointIndex = addDoubleProperty();
    mEndingPointIndex   = addDoubleProperty();
    mPointIntervalIndex = addDoubleProperty();

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

    setPropertyName(mStartingPointIndex, tr("Starting point"));
    setPropertyName(mEndingPointIndex, tr("Ending point"));
    setPropertyName(mPointIntervalIndex, tr("Point interval"));
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setUnit(const QString &pUnit)
{
    // Set the unit for our different properties, if needed

    setPropertyUnit(mStartingPointIndex, pUnit);
    setPropertyUnit(mEndingPointIndex, pUnit);
    setPropertyUnit(mPointIntervalIndex, pUnit);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::startingPoint() const
{
    // Return our starting point

    return doublePropertyValue(mStartingPointIndex);
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setStartingPoint(const double &pValue)
{
    // Set our starting point

    setDoublePropertyValue(mStartingPointIndex, pValue);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::endingPoint() const
{
    // Return our ending point

    return doublePropertyValue(mEndingPointIndex);
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setEndingPoint(const double &pValue)
{
    // Set our ending point

    setDoublePropertyValue(mEndingPointIndex, pValue);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::pointInterval() const
{
    // Return our point interval

    return doublePropertyValue(mPointIntervalIndex);
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setPointInterval(const double &pValue)
{
    // Set our point interval

    setDoublePropertyValue(mPointIntervalIndex, pValue);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

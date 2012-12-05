//==============================================================================
// Single cell simulation view information simulation widget
//==============================================================================

#include "singlecellsimulationviewinformationsimulationwidget.h"

//==============================================================================

#include <QVariant>

//==============================================================================

#include <QtVariantProperty>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationSimulationWidget::SingleCellSimulationViewInformationSimulationWidget(QWidget *pParent) :
    QtPropertyBrowserWidget(true, pParent)
{
    // We don't have any hierarchy, so disable decoration

    setRootIsDecorated(false);

    // Populate our data model

    mStartingPoint = addProperty(QVariant::Double);
    mEndingPoint   = addProperty(QVariant::Double);
    mPointInterval = addProperty(QVariant::Double);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();

    // Select our first property

    selectFirstProperty();

    // Resize our columns

//    resizeColumnsToContents();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::retranslateUi()
{
    // Default retranslation

    QtPropertyBrowserWidget::retranslateUi();

    // Update our property names

    mStartingPoint->setPropertyName(tr("Starting point"));
    mEndingPoint->setPropertyName(tr("Ending point"));
    mPointInterval->setPropertyName(tr("Point interval"));
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setUnit(const QString &pUnit)
{
    // Set the unit for our different properties

//    if (pUnit.compare(mModel->invisibleRootItem()->child(0, 2)->text()))
//        for (int i = 0; i < 3; ++i)
//            mModel->invisibleRootItem()->child(i, 2)->setText(pUnit);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::startingPoint() const
{
    // Return our starting point

    return mStartingPoint->value().toDouble();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setStartingPoint(const double &pValue)
{
    // Set our starting point

    mStartingPoint->setValue(pValue);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::endingPoint() const
{
    // Return our ending point

    return mEndingPoint->value().toDouble();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setEndingPoint(const double &pValue)
{
    // Set our ending point

    mEndingPoint->setValue(pValue);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::pointInterval() const
{
    // Return our point interval

    return mPointInterval->value().toDouble();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setPointInterval(const double &pValue)
{
    // Set our point interval

    mPointInterval->setValue(pValue);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

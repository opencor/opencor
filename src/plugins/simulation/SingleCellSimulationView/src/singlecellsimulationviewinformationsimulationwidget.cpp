//==============================================================================
// Single cell simulation view information simulation widget
//==============================================================================

#include "singlecellsimulationviewinformationsimulationwidget.h"

//==============================================================================

#include <QSettings>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationSimulationWidget::SingleCellSimulationViewInformationSimulationWidget(QWidget *pParent) :
    PropertyEditorWidget(pParent)
{
    // We don't have any hierarchy, so disable decoration

    setRootIsDecorated(false);

    // Create and set our data model

    mModel = new QStandardItemModel(this);

    setModel(mModel);

    // Populate our data model

    mModel->invisibleRootItem()->setChild(0, 0, newString(false));
    mModel->invisibleRootItem()->setChild(1, 0, newString(false));
    mModel->invisibleRootItem()->setChild(2, 0, newString(false));

    mModel->invisibleRootItem()->setChild(0, 1, newDouble(true));
    mModel->invisibleRootItem()->setChild(1, 1, newDouble(true));
    mModel->invisibleRootItem()->setChild(2, 1, newDouble(true));

    mModel->invisibleRootItem()->setChild(0, 2, newString(false));
    mModel->invisibleRootItem()->setChild(1, 2, newString(false));
    mModel->invisibleRootItem()->setChild(2, 2, newString(false));

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();

    // Select our first property

    selectFirstItem();

    // Resize our columns

    resizeColumnsToContents();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::retranslateUi()
{
    // Update ou header labels

    mModel->setHorizontalHeaderLabels(QStringList() << tr("Property")
                                                    << tr("Value")
                                                    << tr("Unit"));

    // Update our property names

    mModel->invisibleRootItem()->child(0, 0)->setText(tr("Starting point"));
    mModel->invisibleRootItem()->child(1, 0)->setText(tr("Ending point"));
    mModel->invisibleRootItem()->child(2, 0)->setText(tr("Point interval"));
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setUnit(const QString &pUnit)
{
    // Set the unit for our different properties

    if (pUnit.compare(mModel->invisibleRootItem()->child(0, 2)->text()))
        for (int i = 0; i < 3; ++i)
            mModel->invisibleRootItem()->child(i, 2)->setText(pUnit);
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::startingPoint() const
{
    // Return our starting point

    return mModel->invisibleRootItem()->child(0, 1)->text().toDouble();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setStartingPoint(const double &pValue)
{
    // Set our starting point

    mModel->invisibleRootItem()->child(0, 1)->setText(QString::number(pValue));
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::endingPoint() const
{
    // Return our ending point

    return mModel->invisibleRootItem()->child(1, 1)->text().toDouble();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setEndingPoint(const double &pValue)
{
    // Set our ending point

    mModel->invisibleRootItem()->child(1, 1)->setText(QString::number(pValue));
}

//==============================================================================

double SingleCellSimulationViewInformationSimulationWidget::pointInterval() const
{
    // Return our point interval

    return mModel->invisibleRootItem()->child(2, 1)->text().toDouble();
}

//==============================================================================

void SingleCellSimulationViewInformationSimulationWidget::setPointInterval(const double &pValue)
{
    // Set our point interval

    mModel->invisibleRootItem()->child(2, 1)->setText(QString::number(pValue));
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

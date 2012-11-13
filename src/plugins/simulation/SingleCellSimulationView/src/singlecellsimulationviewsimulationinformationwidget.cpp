//==============================================================================
// Single cell simulation view simulation information widget
//==============================================================================

#include "singlecellsimulationviewsimulationinformationwidget.h"

//==============================================================================

#include <QSettings>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationInformationWidget::SingleCellSimulationViewSimulationInformationWidget(QWidget *pParent) :
    PropertyEditorWidget(pParent)
{
    // We don't have any hierarchy, so disable decoration

    setRootIsDecorated(false);

    // Create and set our data model

    mModel = new QStandardItemModel(this);

    setModel(mModel);

    // Populate our data model

    mModel->invisibleRootItem()->setChild(0, 0, newNonEditableString());
    mModel->invisibleRootItem()->setChild(1, 0, newNonEditableString());
    mModel->invisibleRootItem()->setChild(2, 0, newNonEditableString());

    mModel->invisibleRootItem()->setChild(0, 1, newEditableDouble(0 /*---GRY--- JUST FOR TESTING... */));
    mModel->invisibleRootItem()->setChild(1, 1, newEditableDouble(1000 /*---GRY--- JUST FOR TESTING... */));
    mModel->invisibleRootItem()->setChild(2, 1, newEditableDouble(1 /*---GRY--- JUST FOR TESTING... */));

    mModel->invisibleRootItem()->setChild(0, 2, newNonEditableString());
    mModel->invisibleRootItem()->setChild(1, 2, newNonEditableString());
    mModel->invisibleRootItem()->setChild(2, 2, newNonEditableString());

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();

    // Resize our columns

    resizeColumnsToContents();
}

//==============================================================================

void SingleCellSimulationViewSimulationInformationWidget::retranslateUi()
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

void SingleCellSimulationViewSimulationInformationWidget::setUnit(const QString &pUnit)
{
    // Set the unit for our different properties

    if (pUnit.compare(mModel->invisibleRootItem()->child(0, 2)->text()))
        for (int i = 0; i < 3; ++i)
            mModel->invisibleRootItem()->child(i, 2)->setText(pUnit);
}

//==============================================================================

double SingleCellSimulationViewSimulationInformationWidget::startingPoint() const
{
    // Return our starting point

    return mModel->invisibleRootItem()->child(0, 1)->text().toDouble();
}

//==============================================================================

double SingleCellSimulationViewSimulationInformationWidget::endingPoint() const
{
    // Return our starting point

    return mModel->invisibleRootItem()->child(1, 1)->text().toDouble();
}

//==============================================================================

double SingleCellSimulationViewSimulationInformationWidget::pointInterval() const
{
    // Return our starting point

    return mModel->invisibleRootItem()->child(2, 1)->text().toDouble();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

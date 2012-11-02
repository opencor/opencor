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

    // Create our data model

    mDataModel = new QStandardItemModel(this);

    // Initialise ourselves

    initialize(mDataModel);

    // Populate our data model

    mDataModel->invisibleRootItem()->setChild(0, 0, newNonEditableItem());
    mDataModel->invisibleRootItem()->setChild(1, 0, newNonEditableItem());
    mDataModel->invisibleRootItem()->setChild(2, 0, newNonEditableItem());

    mDataModel->invisibleRootItem()->setChild(0, 1, newEditableItem());
    mDataModel->invisibleRootItem()->setChild(1, 1, newEditableItem());
    mDataModel->invisibleRootItem()->setChild(2, 1, newEditableItem());

    mDataModel->invisibleRootItem()->setChild(0, 2, newNonEditableItem());
    mDataModel->invisibleRootItem()->setChild(1, 2, newNonEditableItem());
    mDataModel->invisibleRootItem()->setChild(2, 2, newNonEditableItem());

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewSimulationInformationWidget::retranslateUi()
{
    // Update ou header labels

    mDataModel->setHorizontalHeaderLabels(QStringList() << tr("Property")
                                                        << tr("Value")
                                                        << tr("Unit"));

    // Update our property names

    mDataModel->invisibleRootItem()->child(0, 0)->setText(tr("Starting point"));
    mDataModel->invisibleRootItem()->child(1, 0)->setText(tr("Ending point"));
    mDataModel->invisibleRootItem()->child(2, 0)->setText(tr("Point interval"));
}

//==============================================================================

void SingleCellSimulationViewSimulationInformationWidget::setUnit(const QString &pUnit)
{
    // Set the unit for our different properties

    if (pUnit.compare(mDataModel->invisibleRootItem()->child(0, 2)->text()))
        for (int i = 0; i < 3; ++i)
            mDataModel->invisibleRootItem()->child(i, 2)->setText(pUnit);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

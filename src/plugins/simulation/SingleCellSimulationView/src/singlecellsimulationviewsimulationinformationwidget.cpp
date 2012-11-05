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

    mModel = new QStandardItemModel(this);

    // Initialise ourselves

    initialize(mModel);

    // Populate our data model

    mModel->invisibleRootItem()->setChild(0, 0, newNonEditableString());
    mModel->invisibleRootItem()->setChild(1, 0, newNonEditableString());
    mModel->invisibleRootItem()->setChild(2, 0, newNonEditableString());

    mModel->invisibleRootItem()->setChild(0, 1, newEditableDouble(0 /*---GRY--- JUST FOR TESTING... */));
    mModel->invisibleRootItem()->setChild(1, 1, newEditableDouble(1000 /*---GRY--- JUST FOR TESTING... */));
    mModel->invisibleRootItem()->setChild(2, 1, newEditableDouble(1 /*---GRY--- JUST FOR TESTING... */));

    mModel->invisibleRootItem()->setChild(0, 2, newNonEditableString("ms" /*---GRY--- JUST FOR TESTING... */));
    mModel->invisibleRootItem()->setChild(1, 2, newNonEditableString("ms" /*---GRY--- JUST FOR TESTING... */));
    mModel->invisibleRootItem()->setChild(2, 2, newNonEditableString("ms" /*---GRY--- JUST FOR TESTING... */));

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
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

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

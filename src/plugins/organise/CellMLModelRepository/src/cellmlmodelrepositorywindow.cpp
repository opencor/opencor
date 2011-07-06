#include "cellmlmodelrepositorywindow.h"
#include "cellmlmodelrepositorywidget.h"

#include "ui_cellmlmodelrepositorywindow.h"

#include <QSettings>

namespace OpenCOR {
namespace CellMLModelRepository {

CellmlModelRepositoryWindow::CellmlModelRepositoryWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::CellmlModelRepositoryWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the PMR explorer widget

    mCellmlModelRepositoryWidget = new CellmlModelRepositoryWidget("CellmlModelRepositoryWidget",
                                                                   this);

    setWidget(mCellmlModelRepositoryWidget);
}

CellmlModelRepositoryWindow::~CellmlModelRepositoryWindow()
{
    // Delete the UI

    delete mUi;
}

void CellmlModelRepositoryWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);

    // Retranslate the PMR explorer widget

    mCellmlModelRepositoryWidget->retranslateUi();
}

void CellmlModelRepositoryWindow::loadSettings(QSettings *pSettings)
{
    pSettings->beginGroup(objectName());
        // Retrieve the settings of the PMR explorer widget

        mCellmlModelRepositoryWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

void CellmlModelRepositoryWindow::saveSettings(QSettings *pSettings)
{
    pSettings->beginGroup(objectName());
        // Keep track of the settings of the PMR explorer widget

        mCellmlModelRepositoryWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

} }

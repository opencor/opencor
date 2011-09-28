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

    // Create and add the CellML Model Repository widget

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
    // Retranslate the whole window

    mUi->retranslateUi(this);

    // Retranslate the CellML Model Repository widget

    mCellmlModelRepositoryWidget->retranslateUi();
}

void CellmlModelRepositoryWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the CellML Model Repository widget

    pSettings->beginGroup(mCellmlModelRepositoryWidget->objectName());
        mCellmlModelRepositoryWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

void CellmlModelRepositoryWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the CellML Model Repository widget

    pSettings->beginGroup(mCellmlModelRepositoryWidget->objectName());
        mCellmlModelRepositoryWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

} }

#include "cellmlmodelrepositorywindow.h"
#include "cellmlmodelrepositorywidget.h"

#include "ui_cellmlmodelrepositorywindow.h"

static const QString SettingsCellmlModelRepositoryWindow = "CellmlModelRepositoryWindow";

CellmlModelRepositoryWindow::CellmlModelRepositoryWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::CellmlModelRepositoryWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the PMR explorer widget

    mCellmlModelRepositoryWidget = new CellmlModelRepositoryWidget(this);

    setWidget(mCellmlModelRepositoryWidget);

    // Prevent objects from being dropped on the CellML Model Repository widget

    mCellmlModelRepositoryWidget->setAcceptDrops(false);
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

void CellmlModelRepositoryWindow::loadSettings(const QSettings &pSettings,
                                               const QString &)
{
    // Retrieve the settings of the PMR explorer widget

    mCellmlModelRepositoryWidget->loadSettings(pSettings, SettingsCellmlModelRepositoryWindow);
}

void CellmlModelRepositoryWindow::saveSettings(QSettings &pSettings, const QString &)
{
    // Keep track of the settings of the PMR explorer widget

    mCellmlModelRepositoryWidget->saveSettings(pSettings, SettingsCellmlModelRepositoryWindow);
}

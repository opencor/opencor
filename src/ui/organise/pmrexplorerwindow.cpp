#include "pmrexplorerwindow.h"
#include "pmrexplorerwidget.h"

#include "ui_pmrexplorerwindow.h"

#define SETTINGS_PMREXPLORERWINDOW "PmrExplorerWindow"

PmrExplorerWindow::PmrExplorerWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::PmrExplorerWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the PMR explorer widget

    mPmrExplorerWidget = new PmrExplorerWidget(this);

    setWidget(mPmrExplorerWidget);

    // Prevent objects from being dropped on the window

    setAcceptDrops(false);
}

PmrExplorerWindow::~PmrExplorerWindow()
{
    // Delete the UI

    delete mUi;
}

void PmrExplorerWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);

    // Retranslate the PMR explorer widget

    mPmrExplorerWidget->retranslateUi();
}

void PmrExplorerWindow::loadSettings(const QSettings &pSettings,
                                     const QString &)
{
    // Retrieve the settings of the PMR explorer widget

    mPmrExplorerWidget->loadSettings(pSettings, SETTINGS_PMREXPLORERWINDOW);
}

void PmrExplorerWindow::saveSettings(QSettings &pSettings, const QString &)
{
    // Keep track of the settings of the PMR explorer widget

    mPmrExplorerWidget->saveSettings(pSettings, SETTINGS_PMREXPLORERWINDOW);
}

#include "pmrexplorerwindow.h"
#include "pmrexplorerwidget.h"

#include "ui_pmrexplorerwindow.h"

PmrExplorerWindow::PmrExplorerWindow(QWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::PmrExplorerWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the PMR explorer widget

    mPmrExplorerWidget = new PmrExplorerWidget();

    mUi->verticalLayout->addWidget(mPmrExplorerWidget);

    // Prevent objects from being dropped on the PMR explorer widget

    mPmrExplorerWidget->setAcceptDrops(false);
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

void PmrExplorerWindow::defaultSettings()
{
    // Set the default settings for the PMR explorer widget

    mPmrExplorerWidget->defaultSettings();
}

#include "physiomemodelrepositoryclientwindow.h"
#include "physiomemodelrepositoryclientwidget.h"

#include "ui_physiomemodelrepositoryclientwindow.h"

PhysiomeModelRepositoryClientWindow::PhysiomeModelRepositoryClientWindow(QWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::PhysiomeModelRepositoryClientWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the Physiome Model Repository client widget

    mPhysiomeModelRepositoryClientWidget = new PhysiomeModelRepositoryClientWidget();

    mUi->verticalLayout->addWidget(mPhysiomeModelRepositoryClientWidget);

    // Prevent objects from being dropped on the Physiome Model Repository
    // client widget

    mPhysiomeModelRepositoryClientWidget->setAcceptDrops(false);
}

PhysiomeModelRepositoryClientWindow::~PhysiomeModelRepositoryClientWindow()
{
    // Delete the UI

    delete mUi;
}

void PhysiomeModelRepositoryClientWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);
}

void PhysiomeModelRepositoryClientWindow::defaultSettings()
{
    // Set the default settings for the Physiome Model Repository client widget

    mPhysiomeModelRepositoryClientWidget->defaultSettings();
}

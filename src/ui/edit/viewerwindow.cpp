#include "viewerwindow.h"
#include "mmlviewerwidget.h"

#include "ui_viewerwindow.h"

ViewerWindow::ViewerWindow(QWidget *pParent) :
    DockWidget(pParent),
    CommonWidget(pParent),
    mUi(new Ui::ViewerWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the viewer widget

    mMmlViewerWidget = new MmlViewerWidget("MmlViewerWidget", this);

    setWidget(mMmlViewerWidget);

    // Prevent objects from being dropped on the viewer widget

    mMmlViewerWidget->setAcceptDrops(false);
}

ViewerWindow::~ViewerWindow()
{
    // Delete the UI

    delete mUi;
}

void ViewerWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);

    // Retranslate the viewer widget

    mMmlViewerWidget->retranslateUi();
}

void ViewerWindow::loadSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Retrieve the settings of the viewer widget

        mMmlViewerWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

void ViewerWindow::saveSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Keep track of the settings of the viewer widget

        mMmlViewerWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

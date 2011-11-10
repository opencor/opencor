//==============================================================================
// Viewer window
//==============================================================================

#include "viewerwindow.h"
#include "mmlviewerwidget.h"

//==============================================================================

#include "ui_viewerwindow.h"

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

ViewerWindow::ViewerWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::ViewerWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the viewer widget

    mMmlViewerWidget = new MmlViewerWidget("MmlViewerWidget", this);

    setWidget(mMmlViewerWidget);
}

//==============================================================================

ViewerWindow::~ViewerWindow()
{
    // Delete the UI

    delete mUi;
}

//==============================================================================

void ViewerWindow::retranslateUi()
{
    // Retranslate the whole window

    mUi->retranslateUi(this);

    // Retranslate the viewer widget

    mMmlViewerWidget->retranslateUi();
}

//==============================================================================

void ViewerWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the viewer widget

    pSettings->beginGroup(mMmlViewerWidget->objectName());
        mMmlViewerWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void ViewerWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the viewer widget

    pSettings->beginGroup(mMmlViewerWidget->objectName());
        mMmlViewerWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

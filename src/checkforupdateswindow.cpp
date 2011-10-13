//==============================================================================
// Check for Updates window
//==============================================================================

#include "checkforupdateswindow.h"

//==============================================================================

#include "ui_checkforupdateswindow.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

CheckForUpdatesWindow::CheckForUpdatesWindow(QWidget *pParent) :
    QDialog(pParent),
    mUi(new Ui::CheckForUpdatesWindow)
{
    // Set up the UI

    mUi->setupUi(this);
}

//==============================================================================

CheckForUpdatesWindow::~CheckForUpdatesWindow()
{
    // Delete the UI

    delete mUi;
}

//==============================================================================

void CheckForUpdatesWindow::retranslateUi()
{
    // Retranslate the whole window

    mUi->retranslateUi(this);
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

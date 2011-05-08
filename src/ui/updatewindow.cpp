#include "updatewindow.h"

#include "ui_updatewindow.h"

namespace OpenCOR {

UpdateWindow::UpdateWindow(QWidget *pParent) :
    QDialog(pParent),
    CommonWidget(pParent),
    mUi(new Ui::UpdateWindow)
{
    // Set up the UI

    mUi->setupUi(this);
}

UpdateWindow::~UpdateWindow()
{
    // Delete the UI

    delete mUi;
}

void UpdateWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);
}

}

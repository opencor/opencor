#include "updatewindow.h"

#include "ui_updatewindow.h"

UpdateWindow::UpdateWindow(QWidget *parent) :
    QDialog(parent),
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

#include "filebrowserwindow.h"
#include "filebrowserwidget.h"

#include "ui_filebrowserwindow.h"

FileBrowserWindow::FileBrowserWindow(QWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::FileBrowserWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the file browser widget

    mFileBrowserWidget = new FileBrowserWidget();

    mUi->verticalLayout->addWidget(mFileBrowserWidget);

    // Prevent objects from being dropped on the file browser widget

    mFileBrowserWidget->setAcceptDrops(false);
}

FileBrowserWindow::~FileBrowserWindow()
{
    // Delete the UI

    delete mUi;
}

void FileBrowserWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);
}

void FileBrowserWindow::defaultSettings()
{
    // Set the default settings for the file browser widget

    mFileBrowserWidget->defaultSettings();
}

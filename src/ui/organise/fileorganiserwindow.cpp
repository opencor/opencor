#include "fileorganiserwindow.h"
#include "fileorganiserwidget.h"

#include "ui_fileorganiserwindow.h"

FileOrganiserWindow::FileOrganiserWindow(QWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::FileOrganiserWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the file organiser widget

    mFileOrganiserWidget = new FileOrganiserWidget();

    mUi->verticalLayout->addWidget(mFileOrganiserWidget);

    // Prevent objects from being dropped on the file organiser widget

    mFileOrganiserWidget->setAcceptDrops(false);
}

FileOrganiserWindow::~FileOrganiserWindow()
{
    // Delete the UI

    delete mUi;
}

void FileOrganiserWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);

    // Retranslate the file organiser widget

    mFileOrganiserWidget->retranslateUi();
}

void FileOrganiserWindow::defaultSettings()
{
    // Set the default settings for the file organiser widget

    mFileOrganiserWidget->defaultSettings();
}

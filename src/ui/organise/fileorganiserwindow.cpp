#include "docktoolbar.h"
#include "fileorganiserwindow.h"
#include "fileorganiserwidget.h"
#include "utils.h"

#include "ui_fileorganiserwindow.h"

#include <QSettings>

FileOrganiserWindow::FileOrganiserWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::FileOrganiserWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create a toolbar with different buttons
    // Note: this sadly can't be done using the design mode, so...

    DockToolBar *toolbar = new DockToolBar(this);

    toolbar->addAction(mUi->actionNew);
    toolbar->addAction(mUi->actionDelete);

    mUi->verticalLayout->addWidget(toolbar);

    // Create and add the file organiser widget

    mFileOrganiserWidget = new FileOrganiserWidget("FileOrganiserWidget", this);

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

void FileOrganiserWindow::loadSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Retrieve the settings of the file organiser widget

        mFileOrganiserWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

void FileOrganiserWindow::saveSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Keep track of the settings of the file organiser widget

        mFileOrganiserWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

void FileOrganiserWindow::on_actionNew_triggered()
{
    // Create a new folder

    mFileOrganiserWidget->newFolder();
}

void FileOrganiserWindow::on_actionDelete_triggered()
{
    notYetImplemented("void FileOrganiserWindow::on_actionDelete_triggered()");
}

//==============================================================================
// File organiser window
//==============================================================================

#include "fileorganiserwindow.h"
#include "fileorganiserwidget.h"
#include "toolbar.h"

//==============================================================================

#include "ui_fileorganiserwindow.h"

//==============================================================================

#include <QMenu>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

//==============================================================================

FileOrganiserWindow::FileOrganiserWindow(QWidget *pParent) :
    OrganisationWidget(pParent),
    mGui(new Ui::FileOrganiserWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a toolbar with different buttons

    Core::ToolBar *toolbar = new Core::ToolBar(this);

    toolbar->addAction(mGui->actionNew);
    toolbar->addAction(mGui->actionDelete);

    mGui->layout->addWidget(toolbar);

    // Create and add the file organiser widget

    mFileOrganiserWidget = new FileOrganiserWidget(this);

    mFileOrganiserWidget->setObjectName("FileOrganiserWidget");

    mGui->layout->addWidget(mFileOrganiserWidget);

    // We want our own context menu for the file organiser widget

    mFileOrganiserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileOrganiserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));
    connect(mFileOrganiserWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));
    connect(mFileOrganiserWidget, SIGNAL(filesOpened(const QStringList &)),
            this, SIGNAL(filesOpened(const QStringList &)));

    // Some connections to update the enabled state of our various actions

    connect(mFileOrganiserWidget, SIGNAL(newFolderEnabled(const bool &)),
            mGui->actionNew, SLOT(setEnabled(bool)));
    connect(mFileOrganiserWidget, SIGNAL(deleteItemsEnabled(const bool &)),
            mGui->actionDelete, SLOT(setEnabled(bool)));
}

//==============================================================================

FileOrganiserWindow::~FileOrganiserWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void FileOrganiserWindow::retranslateUi()
{
    // Retranslate the whole window

    mGui->retranslateUi(this);

    // Retranslate the file organiser widget

    mFileOrganiserWidget->retranslateUi();
}

//==============================================================================

void FileOrganiserWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the file organiser widget

    pSettings->beginGroup(mFileOrganiserWidget->objectName());
        mFileOrganiserWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the file organiser widget

    pSettings->beginGroup(mFileOrganiserWidget->objectName());
        mFileOrganiserWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindow::on_actionNew_triggered()
{
    // Create a new folder

    mFileOrganiserWidget->newFolder();
}

//==============================================================================

void FileOrganiserWindow::on_actionDelete_triggered()
{
    // Remove the current item(s)

    mFileOrganiserWidget->deleteItems();
}

//==============================================================================

void FileOrganiserWindow::showCustomContextMenu(const QPoint &) const
{
    // Create a custom context menu which items match the contents of our
    // toolbar

    QMenu menu;

    menu.addAction(mGui->actionNew);
    menu.addAction(mGui->actionDelete);

    menu.exec(QCursor::pos());
}

//==============================================================================

void FileOrganiserWindow::itemDoubleClicked(const QModelIndex &itemIndex)
{
    // Check what kind of item has been double clicked and if it is a file, then
    // let people know about it being double clicked

    QString fileName = mFileOrganiserWidget->filePath(itemIndex);

    if (!fileName.isEmpty())
        // We are dealing with a file (as opposed to a folder), so let people
        // know about it having been double clicked

        emit filesOpened(QStringList() << fileName);
}

//==============================================================================

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

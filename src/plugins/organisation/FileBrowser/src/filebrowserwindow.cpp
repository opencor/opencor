//==============================================================================
// File browser window
//==============================================================================

#include "filebrowserwindow.h"
#include "filebrowserwidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_filebrowserwindow.h"

//==============================================================================

#include <QFileInfo>
#include <QMenu>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileBrowser {

//==============================================================================

FileBrowserWindow::FileBrowserWindow(QWidget *pParent) :
    OrganisationWidget(pParent),
    mGui(new Ui::FileBrowserWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a tool bar widget with different buttons

    Core::ToolBarWidget *toolBar = new Core::ToolBarWidget(this);

    toolBar->addAction(mGui->actionHome);
    toolBar->addSeparator();
    toolBar->addAction(mGui->actionParent);
    toolBar->addSeparator();
    toolBar->addAction(mGui->actionPrevious);
    toolBar->addAction(mGui->actionNext);

    mGui->layout->addWidget(toolBar);

    // Create and add the file browser widget

    mFileBrowserWidget = new FileBrowserWidget(this);

    mFileBrowserWidget->setObjectName("FileBrowserWidget");

    mGui->layout->addWidget(mFileBrowserWidget);

    // We want our own context menu for the file organiser widget

    mFileBrowserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileBrowserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));
    connect(mFileBrowserWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));
    connect(mFileBrowserWidget, SIGNAL(filesOpened(const QStringList &)),
            this, SIGNAL(filesOpened(const QStringList &)));

    // Some connections to update the enabled state of our various actions

    connect(mFileBrowserWidget, SIGNAL(notHomeFolder(const bool &)),
            mGui->actionHome, SLOT(setEnabled(bool)));

    connect(mFileBrowserWidget, SIGNAL(goToParentFolderEnabled(const bool &)),
            mGui->actionParent, SLOT(setEnabled(bool)));

    connect(mFileBrowserWidget, SIGNAL(goToPreviousFileOrFolderEnabled(const bool &)),
            mGui->actionPrevious, SLOT(setEnabled(bool)));
    connect(mFileBrowserWidget, SIGNAL(goToNextFileOrFolderEnabled(const bool &)),
            mGui->actionNext, SLOT(setEnabled(bool)));
}

//==============================================================================

FileBrowserWindow::~FileBrowserWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void FileBrowserWindow::retranslateUi()
{
    // Retranslate the whole window

    mGui->retranslateUi(this);

    // Retranslate the file browser widget

    mFileBrowserWidget->retranslateUi();
}

//==============================================================================

void FileBrowserWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the file browser widget

    pSettings->beginGroup(mFileBrowserWidget->objectName());
        mFileBrowserWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileBrowserWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the file browser widget

    pSettings->beginGroup(mFileBrowserWidget->objectName());
        mFileBrowserWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileBrowserWindow::on_actionHome_triggered()
{
    // Go to the home folder

    mFileBrowserWidget->goToHomeFolder();
}

//==============================================================================

void FileBrowserWindow::on_actionParent_triggered()
{
    // Go to the parent item

    mFileBrowserWidget->goToParentFolder();
}

//==============================================================================

void FileBrowserWindow::on_actionPrevious_triggered()
{
    // Go to the previous file/folder

    mFileBrowserWidget->goToPreviousFileOrFolder();
}

//==============================================================================

void FileBrowserWindow::on_actionNext_triggered()
{
    // Go to the next file/folder

    mFileBrowserWidget->goToNextFileOrFolder();
}

//==============================================================================

void FileBrowserWindow::showCustomContextMenu(const QPoint &) const
{
    // Create a custom context menu which items match the contents of our
    // tool bar widget

    QMenu menu;

    menu.addAction(mGui->actionHome);

    menu.exec(QCursor::pos());
}

//==============================================================================

void FileBrowserWindow::itemDoubleClicked(const QModelIndex &)
{
    // Check what kind of item has been double clicked and if it is a file, then
    // let people know about it being double clicked

    QString fileName = mFileBrowserWidget->currentPath();
    QFileInfo fileInfo = fileName;

    if (fileInfo.isFile()) {
        // We are dealing with a file (as opposed to a folder), so let's see
        // whether we can let people know about it having been double clicked

        if (fileInfo.isSymLink()) {
            // The file is actually a symbolic link, so retrieve its target and
            // check that it exists, and if it does then let people know about
            // it having been double clicked

            fileName = fileInfo.symLinkTarget();

            if (QFileInfo(fileName).exists())
                emit filesOpened(QStringList() << fileName);
        } else {
            // This is a 'normal' file, so just go ahead and let people know
            // about it having been double clicked

            emit filesOpened(QStringList() << fileName);
        }
    }
}

//==============================================================================

}   // namespace FileBrowser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

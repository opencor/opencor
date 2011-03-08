#include "docktoolbar.h"
#include "filebrowserwindow.h"
#include "filebrowserwidget.h"
#include "onefieldwindow.h"
#include "utils.h"

#include "ui_filebrowserwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>

#define SETTINGS_FILEBROWSERWINDOW "FileBrowserWindow"

FileBrowserWindow::FileBrowserWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::FileBrowserWindow),
    mPrevFolder(),
    mKeepTrackOfPrevFolder(true)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create a dropdown menu for the New action

    QMenu *actionNewMenu = new QMenu(this);

    actionNewMenu->addAction(mUi->actionNewFolder);
    actionNewMenu->addSeparator();
    actionNewMenu->addAction(mUi->actionNewCellML10File);
    actionNewMenu->addAction(mUi->actionNewCellML11File);

    mUi->actionNew->setMenu(actionNewMenu);

    // Create a toolbar with different buttons
    // Note: this sadly can't be done using the design mode, so...

    DockToolBar *toolbar = new DockToolBar(this);

    toolbar->addAction(mUi->actionHome);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionParent);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionPrevious);
    toolbar->addAction(mUi->actionNext);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionNew);
    toolbar->addAction(mUi->actionDelete);

    mUi->verticalLayout->addWidget(toolbar);

    // Create and add the file browser widget
    // Note: we let the widget know that we want our own custom context menu

    mFileBrowserWidget = new FileBrowserWidget(this);

    mUi->verticalLayout->addWidget(mFileBrowserWidget);

    // We want our own context menu for the file browser widget

    mFileBrowserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Prevent objects from being dropped on the file browser widget

    mFileBrowserWidget->setAcceptDrops(false);

    // Some connections

    connect(mFileBrowserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(customContextMenu(const QPoint &)));

    connect(mFileBrowserWidget->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(currentItemChanged(const QModelIndex &, const QModelIndex &)));
    connect(mFileBrowserWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(needUpdateActions()));
    connect(mFileBrowserWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(needUpdateActions()));
}

FileBrowserWindow::~FileBrowserWindow()
{
    // Delete the UI

    delete mUi;
}

void FileBrowserWindow::updateActions()
{
    // Make sure that the various actions are properly enabled/disabled

    mUi->actionHome->setEnabled(   (mFileBrowserWidget->currentPath() != mFileBrowserWidget->homeFolder())
                                || (   (mFileBrowserWidget->currentPath() == mFileBrowserWidget->homeFolder())
                                    && !mFileBrowserWidget->isCurrentPathVisible()));

    mUi->actionParent->setEnabled(mFileBrowserWidget->currentPathParent() != "");

    mUi->actionPrevious->setEnabled(mPrevFolders.count());
    mUi->actionNext->setEnabled(mNextFolders.count());

    mUi->actionNew->setEnabled(mFileBrowserWidget->isCurrentPathDirWritable());
    mUi->actionDelete->setEnabled(false);
}

void FileBrowserWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);

    // Make sure that the enabled state of the various actions is correct
    // (indeed, to translate everything messes things up in that respect,
    // so...)

    updateActions();

    // Retranslate the file browser widget

    mFileBrowserWidget->retranslateUi();
}

void FileBrowserWindow::loadSettings(const QSettings &pSettings,
                                     const QString &)
{
    // Retrieve the settings of the file browser widget
    // Note: we must make sure that we don't keep track of the previous folder

    mKeepTrackOfPrevFolder = false;

    mFileBrowserWidget->loadSettings(pSettings, SETTINGS_FILEBROWSERWINDOW);

    mKeepTrackOfPrevFolder = true;

    // Initialise the previous folder information

    mPrevFolder = mFileBrowserWidget->currentPath();

    // Make sure that the current path is expanded
    // Note: this is important in case the current path is that of the C:\ drive
    //       or the root of the file system which during the loadSettings above
    //       won't trigger a directoryLoaded signal in the file browser widget

    if (!mFileBrowserWidget->isExpanded(mFileBrowserWidget->currentIndex()))
        mFileBrowserWidget->setExpanded(mFileBrowserWidget->currentIndex(), true);
}

void FileBrowserWindow::saveSettings(QSettings &pSettings, const QString &)
{
    // Keep track of the settings of the file browser widget

    mFileBrowserWidget->saveSettings(pSettings, SETTINGS_FILEBROWSERWINDOW);
}

void FileBrowserWindow::customContextMenu(const QPoint &)
{
    // Create a custom context menu which items match the contents of our
    // toolbar

    QMenu menu;

    menu.addAction(mUi->actionHome);
    menu.addSeparator();
    menu.addAction(mUi->actionParent);
    menu.addSeparator();
    menu.addAction(mUi->actionPrevious);
    menu.addAction(mUi->actionNext);
    menu.addSeparator();
    menu.addAction(mUi->actionNew);
    menu.addAction(mUi->actionDelete);

    menu.exec(QCursor::pos());
}

void FileBrowserWindow::needUpdateActions()
{
    // Something related to the help widget requires the actions to be udpated

    updateActions();
}

void FileBrowserWindow::currentItemChanged(const QModelIndex &pCurrentItem,
                                           const QModelIndex &)
{
    if (!mKeepTrackOfPrevFolder)
        // We don't want to keep track of the previous folder, so...

        return;

    // A new item has been selected, so we need to keep track of the old one in
    // case we want to go back to it

    // Retrieve the full path to the folder where the current item is located

    QString crtItemDir  = mFileBrowserWidget->currentPathDir();

    // Check whether there is a previous folder to keep track of
    // Note: to do so, we cannot rely on the previous item (i.e. the second
    //       parameter to this function), since both the current and previous
    //       items might be located in the same folder, so...

    if (crtItemDir != mPrevFolder) {
        // There is a previous folder to keep track of, so add it to our list of
        // previous folders and consider the current folder as our future
        // previous folder

        mPrevFolders.append(mPrevFolder);

        mPrevFolder = crtItemDir;

        // Reset the list of next folders since that list doesn't make sense
        // anymore

        mNextFolders.clear();
    }

    // Since a new item has been selected, we must update the actions

    updateActions();
}

void FileBrowserWindow::on_actionHome_triggered()
{
    // Go to the home folder (and ask for it to be expanded)

    mFileBrowserWidget->gotoHomeFolder(true);
}

void FileBrowserWindow::on_actionParent_triggered()
{
    // Go to the parent folder

    mFileBrowserWidget->gotoFolder(mFileBrowserWidget->currentPathParent());

    // Going to the parent folder may have changed some actions, so...

    updateActions();
}

void FileBrowserWindow::on_actionPrevious_triggered()
{
    // Go to the previous folder and move the last item from our list of
    // previous folders to our list of next folders
    // Note: we must make sure that we don't keep track of the previous folder

    mNextFolders.append(mFileBrowserWidget->currentPath());

    mKeepTrackOfPrevFolder = false;

    mFileBrowserWidget->gotoFolder(mPrevFolders.last());

    mKeepTrackOfPrevFolder = true;

    mPrevFolders.removeLast();

    // Going to the previous folder may have changed some actions, so...

    updateActions();
}

void FileBrowserWindow::on_actionNext_triggered()
{
    // Go to the next folder and move the last item from our list of next
    // folders to our list of previous folders
    // Note: we must make sure that we don't keep track of the previous folder

    mPrevFolders.append(mFileBrowserWidget->currentPath());

    mKeepTrackOfPrevFolder = false;

    mFileBrowserWidget->gotoFolder(mNextFolders.last());

    mKeepTrackOfPrevFolder = true;

    mNextFolders.removeLast();

    // Going to the next folder may have changed some actions, so...

    updateActions();
}

void FileBrowserWindow::on_actionNew_triggered()
{
    // The default new action is about creating a new folder, so...

    on_actionNewFolder_triggered();
}

void FileBrowserWindow::on_actionNewFolder_triggered()
{
    // Get the name of the new folder

    OneFieldWindow oneFieldWindow(tr("New Folder"), tr("Folder name:"),
                                  tr("Please provide a name for the new folder."),
                                  this);

    oneFieldWindow.exec();

    // Create the new folder in the currently selected folder unless the user
    // cancelled the action

    if (oneFieldWindow.result() == QDialog::Accepted) {
        if (QDir(mFileBrowserWidget->currentPathDir()).mkdir(oneFieldWindow.fieldValue()))
            // The folder was created, so select it

            mFileBrowserWidget->gotoFolder(mFileBrowserWidget->currentPathDir()+QDir::separator()+oneFieldWindow.fieldValue(),
                                           true);
        else
            // The folder couldn't be created, so...

            QMessageBox::information(this, qApp->applicationName(),
                                     tr("Sorry, but the folder could not be created."));
    }
}

void FileBrowserWindow::on_actionNewCellML10File_triggered()
{
    notYetImplemented("void FileBrowserWindow::on_actionNewCellML10File_triggered()");
}

void FileBrowserWindow::on_actionNewCellML11File_triggered()
{
    notYetImplemented("void FileBrowserWindow::on_actionNewCellML11File_triggered()");
}

void FileBrowserWindow::on_actionDelete_triggered()
{
    notYetImplemented("void FileBrowserWindow::on_actionDelete_triggered()");
}

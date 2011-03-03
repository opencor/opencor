#include "docktoolbar.h"
#include "filebrowserwindow.h"
#include "filebrowserwidget.h"

#include "ui_filebrowserwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QMenu>

#define SETTINGS_FILEBROWSERWINDOW "FileBrowserWindow"

FileBrowserWindow::FileBrowserWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::FileBrowserWindow),
    mPreviousFolder(),
    mKeepTrackOfPreviousFolder(true)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create a dropdown menu for the New action

    QMenu *actionNewMenu = new QMenu(this);

    actionNewMenu->addAction(mUi->actionFolder);
    actionNewMenu->addSeparator();
    actionNewMenu->addAction(mUi->actionCellML10File);
    actionNewMenu->addAction(mUi->actionCellML11File);

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
                                    && !mFileBrowserWidget->currentPathVisible()));

    mUi->actionParent->setEnabled(false);

    mUi->actionPrevious->setEnabled(mPreviousFolders.count());
    mUi->actionNext->setEnabled(mNextFolders.count());

    mUi->actionNew->setEnabled(false);
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

    mFileBrowserWidget->loadSettings(pSettings, SETTINGS_FILEBROWSERWINDOW);

    // Make sure that the list of previous folders is empty
    // Note: indeed, as a result of loading our settings, we may change folders,
    //       thus resulting in the the list of previous folders being populated
    //       (see currentItemChanged)

    mPreviousFolders.clear();
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
    if (!mKeepTrackOfPreviousFolder)
        // We don't want to keep track of the previous folder, so...

        return;

    // A new item has been selected, so we need to keep track of the old one in
    // case we want to go back to it

    // Retrieve the full path to the folder where the current item is located

    QString crtItemPath  = mFileBrowserWidget->path(pCurrentItem);
    QFileInfo crtItemFileInfo  = QFileInfo(crtItemPath);
    QString crtItemDir  = crtItemFileInfo.isDir()?
                              crtItemPath:
                              crtItemFileInfo.dir().canonicalPath();

    // Add (and update) the information about the previous folder to our list of
    // previous folders in case the folder of the current item is different from
    // the information about the previous fodler
    // Note: we cannot rely on the previous item (i.e. the second parameter to
    //       this function), since both the current and previous items might
    //       perfectly be located in the same folder, so...

    if (crtItemDir != mPreviousFolder) {
        mPreviousFolders.append(mPreviousFolder);

        mPreviousFolder = crtItemDir;
    }

    // Reset the list of next folders since that list doesn't make sense anymore

    mNextFolders.clear();

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

}

void FileBrowserWindow::on_actionPrevious_triggered()
{
    // Go to the previous folder and move the last item from our list of
    // previous folders to our list of next folders
    // Note: we make sure that we don't keep track of the previous folder

    mNextFolders.append(mFileBrowserWidget->currentPath());

    mKeepTrackOfPreviousFolder = false;

    mFileBrowserWidget->gotoFolder(mPreviousFolders.last());

    mKeepTrackOfPreviousFolder = true;

    mPreviousFolders.removeLast();

    // Going to the previous folder may have changed some actions, so...

    updateActions();
}

void FileBrowserWindow::on_actionNext_triggered()
{
    // Go to the next folder and move the last item from our list of next
    // folders to our list of previous folders
    // Note: we make sure that we don't keep track of the previous folder

    mPreviousFolders.append(mFileBrowserWidget->currentPath());

    mKeepTrackOfPreviousFolder = false;

    mFileBrowserWidget->gotoFolder(mNextFolders.last());

    mKeepTrackOfPreviousFolder = true;

    mNextFolders.removeLast();

    // Going to the next folder may have changed some actions, so...

    updateActions();
}

void FileBrowserWindow::on_actionNew_triggered()
{

}

void FileBrowserWindow::on_actionFolder_triggered()
{

}

void FileBrowserWindow::on_actionCellML10File_triggered()
{

}

void FileBrowserWindow::on_actionCellML11File_triggered()
{

}

void FileBrowserWindow::on_actionDelete_triggered()
{

}

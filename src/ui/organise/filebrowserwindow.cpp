#include "docktoolbar.h"
#include "filebrowserwindow.h"
#include "filebrowserwidget.h"
#include "mainwindow.h"
#include "utils.h"

#include "ui_filebrowserwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>

FileBrowserWindow::FileBrowserWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::FileBrowserWindow),
    mKeepTrackOfPrevFolder(true)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create a toolbar with different buttons
    // Note: this sadly can't be done using the design mode, so...

    DockToolBar *toolbar = new DockToolBar(this);

    toolbar->addAction(mUi->actionHome);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionParent);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionPrevious);
    toolbar->addAction(mUi->actionNext);

    mUi->verticalLayout->addWidget(toolbar);

    // Create and add the file browser widget

    mFileBrowserWidget = new FileBrowserWidget("FileBrowserWidget", this);

    mUi->verticalLayout->addWidget(mFileBrowserWidget);

    // Prevent objects from being dropped on the file browser widget

    mFileBrowserWidget->setAcceptDrops(false);

    // Some connections

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

    static QString homeFolder = mFileBrowserWidget->homeFolder();

    mUi->actionHome->setEnabled(mFileBrowserWidget->currentPath() != homeFolder);

    mUi->actionParent->setEnabled(mFileBrowserWidget->currentPathParent() != "");

    mUi->actionPrevious->setEnabled(mPrevFolders.count());
    mUi->actionNext->setEnabled(mNextFolders.count());
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

void FileBrowserWindow::loadSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Retrieve the settings of the file browser widget
        // Note: we must make sure that we don't keep track of the previous
        //       folder

        mKeepTrackOfPrevFolder = false;

        mFileBrowserWidget->loadSettings(pSettings);

        mKeepTrackOfPrevFolder = true;

        // Make sure that the current path is expanded
        // Note: this is important in case the current path is that of the C:
        //       drive or the root of the file system which, during the
        //       loadSettings above, won't trigger a directoryLoaded signal in
        //       the file browser widget

        if (!mFileBrowserWidget->isExpanded(mFileBrowserWidget->currentIndex()))
            mFileBrowserWidget->setExpanded(mFileBrowserWidget->currentIndex(),
                                            true);
    pSettings.endGroup();
}

void FileBrowserWindow::saveSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Keep track of the settings of the file browser widget

        mFileBrowserWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

void FileBrowserWindow::needUpdateActions()
{
    // Something related to the help widget requires the actions to be udpated

    updateActions();
}

void FileBrowserWindow::currentItemChanged(const QModelIndex &,
                                           const QModelIndex &pPrevItem)
{
    if (!mKeepTrackOfPrevFolder)
        // We don't want to keep track of the previous folder, so...

        return;

    // A new item has been selected, so we need to keep track of the old one in
    // case we want to go back to it

    mPrevFolders.append(mFileBrowserWidget->pathOf(pPrevItem));

    // Reset the list of next folders since that list doesn't make sense
    // anymore

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
    // Go to the parent folder

    mFileBrowserWidget->gotoPath(mFileBrowserWidget->currentPathParent());

    // Going to the parent folder may have changed some actions, so...

    updateActions();
}

void FileBrowserWindow::on_actionPrevious_triggered()
{
    // Go to the previous folder and move the last item from our list of
    // previous folders to our list of next folders
    // Note: we must make sure that we don't keep track of the previous folder
    //       when doing this

    mNextFolders.append(mFileBrowserWidget->currentPath());

    mKeepTrackOfPrevFolder = false;

    mFileBrowserWidget->gotoPath(mPrevFolders.last());

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
    //       when doing this

    mPrevFolders.append(mFileBrowserWidget->currentPath());

    mKeepTrackOfPrevFolder = false;

    mFileBrowserWidget->gotoPath(mNextFolders.last());

    mKeepTrackOfPrevFolder = true;

    mNextFolders.removeLast();

    // Going to the next folder may have changed some actions, so...

    updateActions();
}

void FileBrowserWindow::updateFolders(const QString &pFolderName,
                                      QStringList &pFolders)
{
    // Remove any instance of pFolderName in pFolders

    pFolders.removeAll(pFolderName);

    // Because of the above, we may have two or more consective identital
    // folders in the list, so we must reduce that to one

    if (pFolders.count() > 1) {
        QStringList newFolders;
        QString prevFolder = pFolders.at(0);

        newFolders.append(prevFolder);

        for (int i = 1; i < pFolders.count(); ++i) {
            QString crtFolder = pFolders.at(i);

            if (crtFolder != prevFolder) {
                // The current and previous folders are different, so we want to
                // keep track of it and add it to our new list

                newFolders.append(crtFolder);

                prevFolder = crtFolder;
            }
        }

        // Update the old list of folders with our new one

        pFolders = newFolders;
    }
}

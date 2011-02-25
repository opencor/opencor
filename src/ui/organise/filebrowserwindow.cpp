#include "docktoolbar.h"
#include "filebrowserwindow.h"
#include "filebrowserwidget.h"

#include "ui_filebrowserwindow.h"

#include <QMenu>
#include <QItemSelectionModel>

#define SETTINGS_FILEBROWSERWINDOW "FileBrowserWindow"

FileBrowserWindow::FileBrowserWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::FileBrowserWindow)
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
            this, SLOT(needUpdateActions()));
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

    mUi->actionPrevious->setEnabled(false);
    mUi->actionNext->setEnabled(false);

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
}

void FileBrowserWindow::saveSettings(QSettings &pSettings, const QString &)
{
    // Keep track of the settings of the file browser widget

    mFileBrowserWidget->saveSettings(pSettings, SETTINGS_FILEBROWSERWINDOW);
}

void FileBrowserWindow::customContextMenu(const QPoint &pPos)
{
    // Create a custom context menu which items match the contents of our
    // toolbar

    QMenu *menu = new QMenu;

    menu->addAction(mUi->actionHome);
    menu->addSeparator();
    menu->addAction(mUi->actionParent);
    menu->addSeparator();
    menu->addAction(mUi->actionPrevious);
    menu->addAction(mUi->actionNext);
    menu->addSeparator();
    menu->addAction(mUi->actionNew);
    menu->addAction(mUi->actionDelete);

    menu->exec(mapToGlobal(pPos));
}

void FileBrowserWindow::needUpdateActions()
{
    // Something related to the help widget requires the actions to be udpated

    updateActions();
}

void FileBrowserWindow::on_actionHome_triggered()
{
    // Go to the home folder

    mFileBrowserWidget->gotoHomeFolder();
}

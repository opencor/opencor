#include "docktoolbar.h"
#include "fileorganiserwindow.h"
#include "fileorganiserwidget.h"

#include "ui_fileorganiserwindow.h"

#include <QMenu>
#include <QSettings>

FileOrganiserWindow::FileOrganiserWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::FileOrganiserWindow),
    mContextMenuItemIndex(QModelIndex())
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

    // We want our own context menu for the file organiser widget

    mFileOrganiserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileOrganiserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(customContextMenu(const QPoint &)));

    connect(mFileOrganiserWidget->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(needUpdateActions()));
}

FileOrganiserWindow::~FileOrganiserWindow()
{
    // Delete the UI

    delete mUi;
}

void FileOrganiserWindow::updateActions()
{
    // Make sure that the various actions are properly enabled/disabled

    int nbOfSelectedIndexes = (mContextMenuItemIndex != QModelIndex())?
                                  1:
                                  mFileOrganiserWidget->selectionModel()->selectedIndexes().count();

    mUi->actionNew->setEnabled(nbOfSelectedIndexes <= 1);
    mUi->actionDelete->setEnabled(nbOfSelectedIndexes >= 1);
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

        // Make sure that all the actions are up-to-date

        updateActions();
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

    mFileOrganiserWidget->newFolder(mContextMenuItemIndex);
}

void FileOrganiserWindow::on_actionDelete_triggered()
{
    // Remove the current item(s)

    mFileOrganiserWidget->deleteItems(mContextMenuItemIndex);
}

void FileOrganiserWindow::customContextMenu(const QPoint &pPos)
{
    // Create a custom context menu which items match the contents of our
    // toolbar taking into account the item, if any, under the mouse

    mContextMenuItemIndex = mFileOrganiserWidget->indexAt(pPos);

    updateActions();   // To make sure that the actions are updated according to
                       // value of mContextMenuItemIndex

    QMenu menu;

    menu.addAction(mUi->actionNew);
    menu.addAction(mUi->actionDelete);

    menu.exec(QCursor::pos());

    // Reset the context menu item index

    mContextMenuItemIndex = QModelIndex();

    updateActions();   // To make sure that the actions are properly reset
}

void FileOrganiserWindow::needUpdateActions()
{
    // Something requires the actions to be udpated

    updateActions();
}

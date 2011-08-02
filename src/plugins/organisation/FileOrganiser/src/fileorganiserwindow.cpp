#include "docktoolbar.h"
#include "fileorganiserwindow.h"
#include "fileorganiserwidget.h"

#include "ui_fileorganiserwindow.h"

#include <QMenu>
#include <QSettings>

namespace OpenCOR {
namespace FileOrganiser {

FileOrganiserWindow::FileOrganiserWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::FileOrganiserWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create a toolbar with different buttons
    // Note: this sadly can't be done using the design mode, so...

    Core::DockToolBar *toolbar = new Core::DockToolBar(this);

    toolbar->addAction(mUi->actionNew);
    toolbar->addAction(mUi->actionDelete);

    mUi->verticalLayout->addWidget(toolbar);

    // Create and add the file organiser widget

    mFileOrganiserWidget = new FileOrganiserWidget("FileOrganiserWidget", this);

    mUi->verticalLayout->addWidget(mFileOrganiserWidget);

    // We want our own context menu for the file organiser widget

    mFileOrganiserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileOrganiserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(customContextMenu(const QPoint &)));
    connect(mFileOrganiserWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));
    connect(mFileOrganiserWidget, SIGNAL(filesOpened(const QStringList &)),
            this, SIGNAL(filesOpened(const QStringList &)));

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

    QModelIndexList selectedIndexes = mFileOrganiserWidget->selectionModel()->selectedIndexes();
    int nbOfSelectedIndexes = selectedIndexes.count();
    bool actionNewEnabled = nbOfSelectedIndexes <= 1;

    if (nbOfSelectedIndexes == 1)
        // One item is selected, but is it a folder item?

        actionNewEnabled = mFileOrganiserWidget->isFolderItem(selectedIndexes.at(0));

    mUi->actionNew->setEnabled(actionNewEnabled);
    mUi->actionDelete->setEnabled(nbOfSelectedIndexes >= 1);
}

void FileOrganiserWindow::retranslateUi()
{
    // Retranslate the whole window

    mUi->retranslateUi(this);

    // Retranslate the file organiser widget

    mFileOrganiserWidget->retranslateUi();
}

void FileOrganiserWindow::loadSettings(QSettings *pSettings)
{
    pSettings->beginGroup(objectName());
        // Retrieve the settings of the file organiser widget

        mFileOrganiserWidget->loadSettings(pSettings);

        // Make sure that all the actions are up-to-date

        updateActions();
    pSettings->endGroup();
}

void FileOrganiserWindow::saveSettings(QSettings *pSettings) const
{
    pSettings->beginGroup(objectName());
        // Keep track of the settings of the file organiser widget

        mFileOrganiserWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

void FileOrganiserWindow::on_actionNew_triggered()
{
    // Create a new folder

    mFileOrganiserWidget->newFolder();
}

void FileOrganiserWindow::on_actionDelete_triggered()
{
    // Remove the current item(s)

    mFileOrganiserWidget->deleteItems();
}

void FileOrganiserWindow::customContextMenu(const QPoint &) const
{
    // Create a custom context menu which items match the contents of our
    // toolbar

    QMenu menu;

    menu.addAction(mUi->actionNew);
    menu.addAction(mUi->actionDelete);

    menu.exec(QCursor::pos());
}

void FileOrganiserWindow::itemDoubleClicked(const QModelIndex &itemIndex)
{
    // Check what kind of item has been double clicked and if it is a file, then
    // let people know about it being double clicked

    QString fileName = mFileOrganiserWidget->filePath(itemIndex);

    if (!fileName.isEmpty())
        // We are dealing with a file (as opposed to a folder), so let's see
        // whether we can let people know about it having been double clicked

        emit filesOpened(QStringList() << fileName);
}

void FileOrganiserWindow::needUpdateActions()
{
    // Something requires the actions to be udpated

    updateActions();
}

} }

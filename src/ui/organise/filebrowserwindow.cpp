#include "docktoolbar.h"
#include "filebrowserwindow.h"
#include "filebrowserwidget.h"

#include "ui_filebrowserwindow.h"

#include <QMenu>

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

    mFileBrowserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    mUi->verticalLayout->addWidget(mFileBrowserWidget);

    connect(mFileBrowserWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(customContextMenu(const QPoint&)));

    // Prevent objects from being dropped on the window

    setAcceptDrops(false);
}

FileBrowserWindow::~FileBrowserWindow()
{
    // Delete the UI

    delete mUi;
}

void FileBrowserWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);

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

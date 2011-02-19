#include "filebrowserwindow.h"
#include "filebrowserwidget.h"

#include "ui_filebrowserwindow.h"

#define SETTINGS_FILEBROWSERWINDOW "FileBrowserWindow"

FileBrowserWindow::FileBrowserWindow(QWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::FileBrowserWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Assign an action to the different 'toolbar' buttons

    mUi->homeButton->setDefaultAction(mUi->actionHome);

    mUi->upButton->setDefaultAction(mUi->actionUp);

    mUi->backButton->setDefaultAction(mUi->actionBack);
    mUi->forwardButton->setDefaultAction(mUi->actionForward);

    // Create and add the file browser widget

    mFileBrowserWidget = new FileBrowserWidget(this);

#ifndef Q_WS_MAC
    mUi->verticalLayout->addSpacing(1);
#else
    mUi->verticalLayout->addSpacing(3);
    // Note: a bit more for Mac OS X because of the blue focus border around
    //       QTreeView (not neat, I know...)
#endif
    // Note: the above spacing is only used so that there is a tiny gap between
    //       the toolbar buttons and the file browser widget. The same could
    //       have been achieved using a style sheet on QToolButton (the margin
    //       style), but that would have had the undesired side effect of the
    //       image of the buttons not getting shifted to the bottom right when
    //       pressed, so...
    mUi->verticalLayout->addWidget(mFileBrowserWidget);

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

void FileBrowserWindow::defaultSettings()
{
    // Set the default settings for the file browser widget

    mFileBrowserWidget->defaultSettings();
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

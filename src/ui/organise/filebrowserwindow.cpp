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

    // Create and add the file browser widget

    mFileBrowserWidget = new FileBrowserWidget();

    setWidget(mFileBrowserWidget);

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

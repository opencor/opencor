//==============================================================================
// File browser window
//==============================================================================

#include "filebrowserwindow.h"
#include "filebrowserwidget.h"
#include "toolbar.h"

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
    mUi(new Ui::FileBrowserWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create a toolbar with different buttons

    Core::ToolBar *toolbar = new Core::ToolBar(this);

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

    // We want our own context menu for the file organiser widget

    mFileBrowserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileBrowserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(customContextMenu(const QPoint &)));
    connect(mFileBrowserWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));
    connect(mFileBrowserWidget, SIGNAL(filesOpened(const QStringList &)),
            this, SIGNAL(filesOpened(const QStringList &)));

    // Some connections to update the enabled state of our various actions

    connect(mFileBrowserWidget, SIGNAL(notHomeFolder(const bool &)),
            mUi->actionHome, SLOT(setEnabled(bool)));

    connect(mFileBrowserWidget, SIGNAL(goToParentFolderEnabled(const bool &)),
            mUi->actionParent, SLOT(setEnabled(bool)));

    connect(mFileBrowserWidget, SIGNAL(goToPreviousFileOrFolderEnabled(const bool &)),
            mUi->actionPrevious, SLOT(setEnabled(bool)));
    connect(mFileBrowserWidget, SIGNAL(goToNextFileOrFolderEnabled(const bool &)),
            mUi->actionNext, SLOT(setEnabled(bool)));
}

//==============================================================================

FileBrowserWindow::~FileBrowserWindow()
{
    // Delete the UI

    delete mUi;
}

//==============================================================================

void FileBrowserWindow::retranslateUi()
{
    // Retranslate the whole window

    mUi->retranslateUi(this);

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

    // Make sure that the current path is expanded
    // Note: this is important in case the current path is that of the C: drive
    //       or the root of the file system which, during the loadSettings
    //       above, won't trigger a directoryLoaded signal in the file browser
    //       widget

    if (!mFileBrowserWidget->isExpanded(mFileBrowserWidget->currentIndex()))
        mFileBrowserWidget->setExpanded(mFileBrowserWidget->currentIndex(),
                                        true);
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
    // Go to the home folder (and ask for it to be expanded)

    mFileBrowserWidget->goToHomeFolder(true);
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

void FileBrowserWindow::customContextMenu(const QPoint &) const
{
    // Create a custom context menu which items match the contents of our
    // toolbar

    QMenu menu;

    menu.addAction(mUi->actionHome);

    menu.exec(QCursor::pos());
}

//==============================================================================

void FileBrowserWindow::itemDoubleClicked(const QModelIndex &itemIndex)
{
    // Check what kind of item has been double clicked and if it is a file, then
    // let people know about it being double clicked

    QString fileName = mFileBrowserWidget->pathOf(itemIndex);
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

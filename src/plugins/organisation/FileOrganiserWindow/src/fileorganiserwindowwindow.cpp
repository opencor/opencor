/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// File Organiser window
//==============================================================================

#include "coreguiutils.h"
#include "fileorganiserwindowwindow.h"
#include "fileorganiserwindowwidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_fileorganiserwindowwindow.h"

//==============================================================================

#include <QFileIconProvider>
#include <QMenu>
#include <QPoint>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

//==============================================================================

FileOrganiserWindowWindow::FileOrganiserWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::FileOrganiserWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a tool bar widget with different buttons
    // Note: regarding the icon for mGui->actionNew, we create an overlayed icon
    //       based on the system's folder icon. However, for this to work
    //       properly on Windows (since on that platform, we only have 16x16 and
    //       32x32-pixel icons while we have 16x16, 32x32 and 128x128-pixel
    //       icons on Linux and 16x16, 32x32, 64x64 and 128x128-pixel icons on
    //       macOS), we need to make sure that we are creating the new icon
    //       using an icon size that is closest to the one we want and need...

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);
    QIcon folderIcon = QFileIconProvider().icon(QFileIconProvider::Folder);
    int folderIconSize = qMin(folderIcon.availableSizes().last().width(), 48);
    int folderIconHalfSize = folderIconSize >> 1;

    mGui->actionNew->setIcon(Core::overlayedIcon(folderIcon,
                                                 QIcon(":/oxygen/actions/list-add.png"),
                                                 folderIconSize, folderIconSize,
                                                 folderIconHalfSize, 0, folderIconHalfSize, folderIconHalfSize));

    toolBarWidget->addAction(mGui->actionNew);
    toolBarWidget->addAction(mGui->actionDelete);

    mGui->layout->addWidget(toolBarWidget);

    // Create and add the file organiser widget

    mFileOrganiserWindowWidget = new FileOrganiserWindowWidget(this);

    mFileOrganiserWindowWidget->setObjectName("FileOrganiserWindowWidget");

    mGui->layout->addWidget(mFileOrganiserWindowWidget);

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionNew);
    mContextMenu->addAction(mGui->actionDelete);

    // We want our own context menu for the file organiser widget

    mFileOrganiserWindowWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileOrganiserWindowWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));
    connect(mFileOrganiserWindowWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));
    connect(mFileOrganiserWindowWidget, SIGNAL(openFilesRequested(const QStringList &)),
            this, SLOT(openFiles(const QStringList &)));

    // Some connections to update the enabled state of our various actions

    connect(mFileOrganiserWindowWidget, SIGNAL(newFolderEnabled(const bool &)),
            mGui->actionNew, SLOT(setEnabled(bool)));
    connect(mFileOrganiserWindowWidget, SIGNAL(deleteItemsEnabled(const bool &)),
            mGui->actionDelete, SLOT(setEnabled(bool)));
}

//==============================================================================

FileOrganiserWindowWindow::~FileOrganiserWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void FileOrganiserWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);

    // Retranslate our file organiser widget

    mFileOrganiserWindowWidget->retranslateUi();
}

//==============================================================================

void FileOrganiserWindowWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the file organiser widget

    pSettings->beginGroup(mFileOrganiserWindowWidget->objectName());
        mFileOrganiserWindowWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindowWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the file organiser widget

    pSettings->beginGroup(mFileOrganiserWindowWidget->objectName());
        mFileOrganiserWindowWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindowWindow::on_actionNew_triggered()
{
    // Create a new folder

    mFileOrganiserWindowWidget->newFolder();
}

//==============================================================================

void FileOrganiserWindowWindow::on_actionDelete_triggered()
{
    // Remove the current item(s)

    mFileOrganiserWindowWidget->deleteItems();
}

//==============================================================================

void FileOrganiserWindowWindow::showCustomContextMenu() const
{
    // Show our context menu which items match the contents of our tool bar
    // widget

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void FileOrganiserWindowWindow::itemDoubleClicked(const QModelIndex &pItemIndex)
{
    // Check what kind of item has been double clicked and if it is a file, then
    // open it

    QString fileName = mFileOrganiserWindowWidget->filePath(pItemIndex);

    if (!fileName.isEmpty()) {
        // We are dealing with a file (as opposed to a folder), so just open it

        openFile(fileName);
    }
}

//==============================================================================

}   // namespace FileOrganiserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

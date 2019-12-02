/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// File Organiser window
//==============================================================================

#include "coreguiutils.h"
#include "fileorganiserwindowwidget.h"
#include "fileorganiserwindowwindow.h"
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

    connect(mGui->actionNew, &QAction::triggered,
            this, &FileOrganiserWindowWindow::actionNewTriggered);
    connect(mGui->actionDelete, &QAction::triggered,
            this, &FileOrganiserWindowWindow::actionDeleteTriggered);

    // Create a tool bar widget with different buttons
    // Note: normally, we would retrieve the folder icon through a call to
    //       QFileIconProvider().icon(QFileIconProvider::Folder), but on Windows
    //       it will, in this case, return the QStyle::SP_DirIcon icon while we
    //       really want the QStyle::SP_DirClosedIcon icon...

    static const QIcon PlusIcon = QIcon(":/oxygen/actions/list-add.png");

    auto toolBarWidget = new Core::ToolBarWidget();
    QIcon folderIcon = Core::standardIcon(QStyle::SP_DirClosedIcon);
    int folderIconSize = folderIcon.availableSizes().first().width();
    int plusIconSize = int(0.57*folderIconSize);
    int scaledIconSize = devicePixelRatio()*toolBarWidget->iconSize().width();
    // Note: we scale the icon in case we are on a non-HiDPI screen, in which
    //       case the icon would be smaller than what we need for our tool bar
    //       widget...

    mGui->actionNew->setIcon(Core::scaledIcon(Core::overlayedIcon(folderIcon, PlusIcon,
                                                                  folderIconSize, folderIconSize,
                                                                  folderIconSize-plusIconSize, 0,
                                                                  plusIconSize, plusIconSize),
                                              scaledIconSize, scaledIconSize,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));

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

    connect(mFileOrganiserWindowWidget, &FileOrganiserWindowWidget::customContextMenuRequested,
            this, &FileOrganiserWindowWindow::showCustomContextMenu);
    connect(mFileOrganiserWindowWidget, &FileOrganiserWindowWidget::doubleClicked,
            this, &FileOrganiserWindowWindow::itemDoubleClicked);
    connect(mFileOrganiserWindowWidget, &FileOrganiserWindowWidget::openFilesRequested,
            this, &FileOrganiserWindowWindow::openFiles);

    // Some connections to update the enabled state of our various actions

    connect(mFileOrganiserWindowWidget, &FileOrganiserWindowWidget::newFolderEnabled,
            mGui->actionNew, &QAction::setEnabled);
    connect(mFileOrganiserWindowWidget, &FileOrganiserWindowWidget::deleteItemsEnabled,
            mGui->actionDelete, &QAction::setEnabled);
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

void FileOrganiserWindowWindow::loadSettings(QSettings &pSettings)
{
    // Retrieve the settings of the file organiser widget

    pSettings.beginGroup(mFileOrganiserWindowWidget->objectName());
        mFileOrganiserWindowWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void FileOrganiserWindowWindow::saveSettings(QSettings &pSettings) const
{
    // Keep track of the settings of the file organiser widget

    pSettings.beginGroup(mFileOrganiserWindowWidget->objectName());
        mFileOrganiserWindowWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void FileOrganiserWindowWindow::actionNewTriggered()
{
    // Create a new folder

    mFileOrganiserWindowWidget->newFolder();
}

//==============================================================================

void FileOrganiserWindowWindow::actionDeleteTriggered()
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

} // namespace FileOrganiserWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

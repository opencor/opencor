/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
    // Note: normally, we would retrieve the folder icon through a call to
    //       QFileIconProvider().icon(QFileIconProvider::Folder), but on Windows
    //       it will, in this case, return the QStyle::SP_DirIcon icon while we
    //       really want the QStyle::SP_DirClosedIcon icon...

    static const QIcon PlusIcon = QIcon(":/oxygen/actions/list-add.png");

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);
    QIcon folderIcon = QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon);
    int folderIconSize = folderIcon.availableSizes().first().width();
    int plusIconSize = 0.57*folderIconSize;

    mGui->actionNew->setIcon(Core::overlayedIcon(folderIcon, PlusIcon,
                                                 folderIconSize, folderIconSize,
                                                 folderIconSize-plusIconSize, 0,
                                                 plusIconSize, plusIconSize));

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

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
// File Browser window
//==============================================================================

#include "filebrowserwindowwindow.h"
#include "filebrowserwindowwidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_filebrowserwindowwindow.h"

//==============================================================================

#include <QFileInfo>
#include <QMenu>
#include <QPoint>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileBrowserWindow {

//==============================================================================

FileBrowserWindowWindow::FileBrowserWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::FileBrowserWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a tool bar widget with different buttons

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionHome);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionParent);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionPrevious);
    toolBarWidget->addAction(mGui->actionNext);

    mGui->layout->addWidget(toolBarWidget);

    // Create and add the file browser widget

    mFileBrowserWindowWidget = new FileBrowserWindowWidget(this);

    mFileBrowserWindowWidget->setObjectName("FileBrowserWindowWidget");

    mGui->layout->addWidget(mFileBrowserWindowWidget);

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionHome);

    // We want our own context menu for the file organiser widget

    mFileBrowserWindowWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileBrowserWindowWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));
    connect(mFileBrowserWindowWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked()));
    connect(mFileBrowserWindowWidget, SIGNAL(filesOpenRequested(const QStringList &)),
            this, SLOT(openFiles(const QStringList &)));

    // Some connections to update the enabled state of our various actions

    connect(mFileBrowserWindowWidget, SIGNAL(notHomeFolder(const bool &)),
            mGui->actionHome, SLOT(setEnabled(bool)));

    connect(mFileBrowserWindowWidget, SIGNAL(goToParentFolderEnabled(const bool &)),
            mGui->actionParent, SLOT(setEnabled(bool)));

    connect(mFileBrowserWindowWidget, SIGNAL(goToPreviousFileOrFolderEnabled(const bool &)),
            mGui->actionPrevious, SLOT(setEnabled(bool)));
    connect(mFileBrowserWindowWidget, SIGNAL(goToNextFileOrFolderEnabled(const bool &)),
            mGui->actionNext, SLOT(setEnabled(bool)));
}

//==============================================================================

FileBrowserWindowWindow::~FileBrowserWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void FileBrowserWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);

    // Retranslate our file browser widget

    mFileBrowserWindowWidget->retranslateUi();
}

//==============================================================================

void FileBrowserWindowWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the file browser widget

    pSettings->beginGroup(mFileBrowserWindowWidget->objectName());
        mFileBrowserWindowWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileBrowserWindowWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the file browser widget

    pSettings->beginGroup(mFileBrowserWindowWidget->objectName());
        mFileBrowserWindowWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileBrowserWindowWindow::on_actionHome_triggered()
{
    // Go to the home folder

    mFileBrowserWindowWidget->goToHomeFolder();
}

//==============================================================================

void FileBrowserWindowWindow::on_actionParent_triggered()
{
    // Go to the parent item

    mFileBrowserWindowWidget->goToParentFolder();
}

//==============================================================================

void FileBrowserWindowWindow::on_actionPrevious_triggered()
{
    // Go to the previous file/folder

    mFileBrowserWindowWidget->goToPreviousFileOrFolder();
}

//==============================================================================

void FileBrowserWindowWindow::on_actionNext_triggered()
{
    // Go to the next file/folder

    mFileBrowserWindowWidget->goToNextFileOrFolder();
}

//==============================================================================

void FileBrowserWindowWindow::showCustomContextMenu() const
{
    // Show our context menu which items match the contents of our tool bar
    // widget

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void FileBrowserWindowWindow::itemDoubleClicked()
{
    // Check what kind of item has been double clicked and if it is a file, then
    // open it

    QString fileName = mFileBrowserWindowWidget->currentPath();
    QFileInfo fileInfo = fileName;

    if (fileInfo.isFile()) {
        // We are dealing with a file (as opposed to a folder), so just open it

        openFile(fileName);
    }
}

//==============================================================================

}   // namespace FileBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

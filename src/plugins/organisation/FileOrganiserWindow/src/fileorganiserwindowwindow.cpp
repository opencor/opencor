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
// File organiser window
//==============================================================================

#include "fileorganiserwindowwindow.h"
#include "fileorganiserwindowwidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_fileorganiserwindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

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

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionNew);
    toolBarWidget->addAction(mGui->actionDelete);

    mGui->layout->addWidget(toolBarWidget);

    // Create and add the file organiser widget

    mFileOrganiserWidget = new FileOrganiserWindowWidget(this);

    mFileOrganiserWidget->setObjectName("FileOrganiserWindowWidget");

    mGui->layout->addWidget(mFileOrganiserWidget);

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionNew);
    mContextMenu->addAction(mGui->actionDelete);

    // We want our own context menu for the file organiser widget

    mFileOrganiserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileOrganiserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));
    connect(mFileOrganiserWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));
    connect(mFileOrganiserWidget, SIGNAL(filesOpenRequested(const QStringList &)),
            this, SLOT(openFiles(const QStringList &)));

    // Some connections to update the enabled state of our various actions

    connect(mFileOrganiserWidget, SIGNAL(newFolderEnabled(const bool &)),
            mGui->actionNew, SLOT(setEnabled(bool)));
    connect(mFileOrganiserWidget, SIGNAL(deleteItemsEnabled(const bool &)),
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
    // Retranslate the whole window

    mGui->retranslateUi(this);

    // Retranslate the file organiser widget

    mFileOrganiserWidget->retranslateUi();
}

//==============================================================================

void FileOrganiserWindowWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the file organiser widget

    pSettings->beginGroup(mFileOrganiserWidget->objectName());
        mFileOrganiserWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindowWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the file organiser widget

    pSettings->beginGroup(mFileOrganiserWidget->objectName());
        mFileOrganiserWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindowWindow::on_actionNew_triggered()
{
    // Create a new folder

    mFileOrganiserWidget->newFolder();
}

//==============================================================================

void FileOrganiserWindowWindow::on_actionDelete_triggered()
{
    // Remove the current item(s)

    mFileOrganiserWidget->deleteItems();
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

    QString fileName = mFileOrganiserWidget->filePath(pItemIndex);

    if (!fileName.isEmpty())
        // We are dealing with a file (as opposed to a folder), so just open it

        openFile(fileName);
}

//==============================================================================

}   // namespace FileOrganiserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

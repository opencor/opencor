/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// File browser window
//==============================================================================

#include "filebrowserwindow.h"
#include "filebrowserwidget.h"
#include "toolbarwidget.h"

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
    mGui(new Ui::FileBrowserWindow)
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

    mFileBrowserWidget = new FileBrowserWidget(this);

    mFileBrowserWidget->setObjectName("FileBrowserWidget");

    mGui->layout->addWidget(mFileBrowserWidget);

    // Create and populate our custom context menu

    mCustomContextMenu = new QMenu(this);

    mCustomContextMenu->addAction(mGui->actionHome);

    // We want our own context menu for the file organiser widget

    mFileBrowserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mFileBrowserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));
    connect(mFileBrowserWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));
    connect(mFileBrowserWidget, SIGNAL(filesOpenRequested(const QStringList &)),
            this, SLOT(openFiles(const QStringList &)));

    // Some connections to update the enabled state of our various actions

    connect(mFileBrowserWidget, SIGNAL(notHomeFolder(const bool &)),
            mGui->actionHome, SLOT(setEnabled(bool)));

    connect(mFileBrowserWidget, SIGNAL(goToParentFolderEnabled(const bool &)),
            mGui->actionParent, SLOT(setEnabled(bool)));

    connect(mFileBrowserWidget, SIGNAL(goToPreviousFileOrFolderEnabled(const bool &)),
            mGui->actionPrevious, SLOT(setEnabled(bool)));
    connect(mFileBrowserWidget, SIGNAL(goToNextFileOrFolderEnabled(const bool &)),
            mGui->actionNext, SLOT(setEnabled(bool)));
}

//==============================================================================

FileBrowserWindow::~FileBrowserWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void FileBrowserWindow::retranslateUi()
{
    // Retranslate the whole window

    mGui->retranslateUi(this);

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
    // Go to the home folder

    mFileBrowserWidget->goToHomeFolder();
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

void FileBrowserWindow::showCustomContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Show our custom context menu which items match the contents of our tool
    // bar widget

    mCustomContextMenu->exec(QCursor::pos());
}

//==============================================================================

void FileBrowserWindow::itemDoubleClicked(const QModelIndex &pIndex)
{
    Q_UNUSED(pIndex);

    // Check what kind of item has been double clicked and if it is a file, then
    // open it

    QString fileName = mFileBrowserWidget->currentPath();
    QFileInfo fileInfo = fileName;

    if (fileInfo.isFile())
        // We are dealing with a file (as opposed to a folder), so just open it

        openFile(fileName);
}

//==============================================================================

}   // namespace FileBrowser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

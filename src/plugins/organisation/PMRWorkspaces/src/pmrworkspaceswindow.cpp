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
// Workspaces window
//==============================================================================

#include "pmrworkspacesnewworkspace.h"
#include "pmrworkspaceswindow.h"
#include "pmrworkspaceswidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_pmrworkspaceswindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QMenu>
#include <QPoint>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

PmrWorkspacesWindow::PmrWorkspacesWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::PmrWorkspacesWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create an instance of the Physiome Model Repository

    mPmrRepository = new PMRSupport::PmrRepository(this);

    // Create a tool bar widget with different buttons

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionAuthenticate);
    toolBarWidget->addAction(mGui->actionUnauthenticate);
    toolBarWidget->addAction(mGui->actionNew);

    // Right align info action

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBarWidget->addWidget(spacer);
    toolBarWidget->addAction(mGui->actionInfo);

    mGui->layout->addWidget(toolBarWidget);

    // Create and add the workspaces widget

    mWorkspacesWidget = new PmrWorkspacesWidget(mPmrRepository, this);

    mWorkspacesWidget->setObjectName("PmrWorkspacesWidget");

    mGui->layout->addWidget(mWorkspacesWidget);

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionAuthenticate);
    mContextMenu->addAction(mGui->actionUnauthenticate);
    mContextMenu->addAction(mGui->actionNew);
    mContextMenu->addAction(mGui->actionInfo);

    // We want our own context menu for the workspaces widget

    mWorkspacesWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mWorkspacesWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showCustomContextMenu()));

    // Some connections to process responses from the PMR repository

    connect(mPmrRepository, SIGNAL(authenticated(bool)), this, SLOT(updateAuthenticationStatus(bool)));
    connect(mPmrRepository, SIGNAL(workspacesList(PMRSupport::PmrWorkspaceList)),
            mWorkspacesWidget, SLOT(initialiseWorkspaces(PMRSupport::PmrWorkspaceList)));

    connect(mPmrRepository, SIGNAL(workspaceCreated(QString)), this, SLOT(workspaceCreated(QString)));
    connect(mPmrRepository, SIGNAL(workspaceCloned(PMRSupport::PmrWorkspace *)),
            this, SLOT(workspaceCloned(PMRSupport::PmrWorkspace *)));

    // Some connections to update our state

    connect(this, SIGNAL(refreshWorkspaces(void)), mPmrRepository, SLOT(getAuthenticationStatus()));

    emit refreshWorkspaces();
}

//==============================================================================

PmrWorkspacesWindow::~PmrWorkspacesWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesWindow::retranslateUi()
{
    // Retranslate the whole window

    mGui->retranslateUi(this);

    // Retranslate the workspaces widget

    mWorkspacesWidget->retranslateUi();
}

//==============================================================================

void PmrWorkspacesWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the workspaces widget

    pSettings->beginGroup(mWorkspacesWidget->objectName());
        mWorkspacesWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the workspaces widget

    pSettings->beginGroup(mWorkspacesWidget->objectName());
        mWorkspacesWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWindow::updateAuthenticationStatus(const bool &pAuthenticated)
{
    // Show authentication state and allow workspace creation if authenticated

    if (pAuthenticated) {
        mGui->actionAuthenticate->setVisible(false);
        mGui->actionNew->setEnabled(true);
        mGui->actionUnauthenticate->setVisible(true);
        mWorkspacesWidget->refreshWorkspaces();
    }
    else {
        mGui->actionAuthenticate->setVisible(true);
        mGui->actionNew->setEnabled(false);
        mGui->actionUnauthenticate->setVisible(false);
        mWorkspacesWidget->clearWorkspaces();
    }
}

//==============================================================================

void PmrWorkspacesWindow::on_actionAuthenticate_triggered()
{
    // Log on to PMR

    mPmrRepository->authenticate();
}

//==============================================================================

void PmrWorkspacesWindow::on_actionNew_triggered()
{
    // Create a new workspace

    auto newWorkspaceDialog = new PmrWorkspacesNewWorkspace();

    if (newWorkspaceDialog->exec() == QDialog::Accepted) {

        // Ask the PMR repository to create a new workspace

        mPmrRepository->requestNewWorkspace(newWorkspaceDialog->title(),
                                            newWorkspaceDialog->description(),
                                            newWorkspaceDialog->path());
    }
}

//==============================================================================

void PmrWorkspacesWindow::on_actionInfo_triggered()
{
    // Show information about the selected object
    mPmrRepository->requestWorkspacesList();  // ***** TEMP
}

//==============================================================================

void PmrWorkspacesWindow::on_actionUnauthenticate_triggered()
{
    // Log off PMR
    mPmrRepository->authenticate(false);
}

//==============================================================================

void PmrWorkspacesWindow::showCustomContextMenu() const
{
    // Show our context menu which items match the contents of our tool bar
    // widget

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void PmrWorkspacesWindow::workspaceCreated(const QString &pUrl)
{
    Q_UNUSED(pUrl)
}

//==============================================================================

void PmrWorkspacesWindow::workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace)
{
    if (pWorkspace) {
        // All good, so selected it in the list of workspaces

        mWorkspacesWidget->setSelected(pWorkspace->url());

        // And update the list of workspaces

        mPmrRepository->requestWorkspacesList();
    }
}

//==============================================================================

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

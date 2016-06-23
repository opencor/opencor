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

#include "borderedwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrworkspacesnewworkspace.h"
#include "pmrworkspaceswindow.h"
#include "pmrworkspaceswidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_pmrworkspaceswindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QDir>
#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QPoint>
#include <QSettings>
#include <QTimer>

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

    toolBarWidget->addAction(mGui->actionNew);
    toolBarWidget->addAction(mGui->actionRefresh);

    // Right align logon/off

    QWidget *spacer1 = new QWidget();
    spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBarWidget->addWidget(spacer1);

    toolBarWidget->addAction(mGui->actionAuthenticate);
    toolBarWidget->addAction(mGui->actionUnauthenticate);

    QWidget *spacer2 = new QWidget();
    spacer2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    spacer2->setMinimumSize(4, 0);
    toolBarWidget->addWidget(spacer2);

    mGui->layout->addWidget(toolBarWidget);

    // Create and add the workspaces widget

    mWorkspacesWidget = new PmrWorkspacesWidget(mPmrRepository, this);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mWorkspacesWidget,
                                                                           true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mWorkspacesWidget,
                                                                           true, false, false, false));
#else
    #error Unsupported platform
#endif

    // Keep track of the window's visibility, so that we can request the list of
    // exposures, if necessary

    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(retrieveWorkspacesList(bool)));

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionNew);
    mContextMenu->addAction(mGui->actionRefresh);
    mContextMenu->addAction(mGui->actionAuthenticate);
    mContextMenu->addAction(mGui->actionUnauthenticate);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionRescan);

    // We want our own context menu for the toolbar widget

    toolBarWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(toolBarWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showCustomContextMenu()));

    // Some connections

    connect(mWorkspacesWidget, SIGNAL(warning(QString)), this, SLOT(showWarning(QString)));

    // Some connections to process responses from the PMR repository

    connect(mPmrRepository, SIGNAL(busy(bool)), this, SLOT(busy(bool)));
    connect(mPmrRepository, SIGNAL(progress(double)), this, SLOT(showProgress(double)));

    connect(mPmrRepository, SIGNAL(error(QString, bool)), this, SLOT(showError(QString)));
    connect(mPmrRepository, SIGNAL(information(QString)), this, SLOT(showInformation(QString)));
    connect(mPmrRepository, SIGNAL(warning(QString)), this, SLOT(showWarning(QString)));

    connect(mPmrRepository, SIGNAL(authenticated(bool)), this, SLOT(updateAuthenticationStatus(bool)));
    connect(mPmrRepository, SIGNAL(workspacesList(PMRSupport::PmrWorkspaceList)),
            mWorkspacesWidget, SLOT(initialiseWorkspaces(PMRSupport::PmrWorkspaceList)));

    // Retranslate the GUI

    retranslateUi();
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

void PmrWorkspacesWindow::busy(const bool &pBusy)
{
    // Show ourselves as busy or not busy anymore

    static int counter = 0;

    counter += pBusy?1:-1;

    if (pBusy && (counter == 1)) {
        showBusyWidget(mWorkspacesWidget);

        mGui->dockWidgetContents->setEnabled(false);
    } else if (!pBusy && !counter) {
        // Re-enable the GUI side

        hideBusyWidget();

        mGui->dockWidgetContents->setEnabled(true);

        // And give focus to??
    }
}

//==============================================================================

void PmrWorkspacesWindow::showProgress(const double &pProgress)
{
    setBusyWidgetProgress(pProgress);
}

//==============================================================================

void PmrWorkspacesWindow::showError(const QString &pMessage)
{
    // Show the given message as an error

    QMessageBox::critical(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindow::showInformation(const QString &pMessage)
{
    // Show the given message as informative text

    QMessageBox::information(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindow::showWarning(const QString &pMessage)
{
    // Show the given message as a warning

    QMessageBox::warning(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindow::retrieveWorkspacesList(const bool &pVisible)
{
    // Request authentication status, if we are becoming visible and the list
    // of workspaces has never been requested before (through a single shot, this
    // to allow other events, such as the one asking OpenCOR's main window to
    // resize itself, to be handled properly). This will result in the workspace
    // list being populated if we are in fact authenticated with PMR.

    static bool firstTime = true;

    if (pVisible && firstTime) {
        firstTime = false;

        QTimer::singleShot(0, this, SLOT(getAuthenticationStatus()));
    }
}

//==============================================================================

void PmrWorkspacesWindow::getAuthenticationStatus(void)
{
    // Results in us being sent an `authenticated` signal.

    mPmrRepository->getAuthenticationStatus();
}

//==============================================================================

void PmrWorkspacesWindow::updateAuthenticationStatus(const bool &pAuthenticated)
{
    // Show authentication state and allow workspace creation if authenticated

    if (pAuthenticated) {
        mGui->actionAuthenticate->setVisible(false);
        mGui->actionNew->setEnabled(true);
        mGui->actionRefresh->setEnabled(true);
        mGui->actionRescan->setEnabled(true);
        mGui->actionUnauthenticate->setVisible(true);
        mWorkspacesWidget->refreshWorkspaces(false);
    }
    else {
        mGui->actionAuthenticate->setVisible(true);
        mGui->actionNew->setEnabled(false);
        mGui->actionRefresh->setEnabled(false);
        mGui->actionRescan->setEnabled(false);
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

        // Create the folder for the new workspace

        QDir workspaceFolder(newWorkspaceDialog->path());
        if (!workspaceFolder.exists()) {
            workspaceFolder.mkpath(".");
        }
        else {
            // TODO Need to check the folder is empty...
            // If not, give an error and return
        }

        // Ask the PMR repository to create a new workspace. This
        // will result in the (empty) workspace being cloned into
        // its folder.

        mPmrRepository->requestNewWorkspace(newWorkspaceDialog->title(),
                                            newWorkspaceDialog->description(),
                                            newWorkspaceDialog->path());
    }
}

//==============================================================================

void PmrWorkspacesWindow::on_actionRefresh_triggered()
{
    // Ask the workspaces widget to refresh itself

    mWorkspacesWidget->refreshWorkspaces(false);
}

//==============================================================================

void PmrWorkspacesWindow::on_actionRescan_triggered()
{
    // Rescan the default workspace folders directory before refreshing

    mWorkspacesWidget->refreshWorkspaces(true);
}

//==============================================================================

void PmrWorkspacesWindow::on_actionUnauthenticate_triggered()
{
    // Log off PMR

    if (QMessageBox::question(this, tr("OpenCOR"),
                              tr("Log off the Physiome Model Repository")) == QMessageBox::Ok)
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

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

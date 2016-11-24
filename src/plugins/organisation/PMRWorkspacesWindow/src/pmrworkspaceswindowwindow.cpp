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
// PMR Workspaces window
//==============================================================================

#include "borderedwidget.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "i18ninterface.h"
#include "toolbarwidget.h"
#include "pmrwebservice.h"
#include "pmrworkspaceswindownewworkspace.h"
#include "pmrworkspaceswindowwidget.h"
#include "pmrworkspaceswindowwindow.h"

//==============================================================================

#include "ui_pmrworkspaceswindowwindow.h"

//==============================================================================

#include <QDir>
#include <QGraphicsColorizeEffect>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowWindow::PmrWorkspacesWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::PmrWorkspacesWindowWindow),
    mAuthenticated(false),
    mColorizeEffect(new QGraphicsColorizeEffect(this))
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a tool bar widget with different actions
    // Note: we right-align our PMR-related action and make it ready for
    //       colorisation effect...

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionNew);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionReload);

    QWidget *spacer = new QWidget(toolBarWidget);

    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toolBarWidget->addWidget(spacer);
    toolBarWidget->addAction(mGui->actionPmr);

    toolBarWidget->widgetForAction(mGui->actionPmr)->setGraphicsEffect(mColorizeEffect);

    mGui->layout->addWidget(toolBarWidget);

    // Create an instance of our PMR web service

    mPmrWebService = new PMRSupport::PmrWebService(this);

    // Create and add our workspaces widget

    mPmrWorkspacesWindowWidget = new PmrWorkspacesWindowWidget(mPmrWebService, this);

    mPmrWorkspacesWindowWidget->setObjectName("PmrWorkspacesWindowWidget");

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWorkspacesWindowWidget,
                                                     true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWorkspacesWindowWidget,
                                                     true, false, false, false));
#else
    #error Unsupported platform
#endif

    // Keep track of the window's visibility, so that we can request the list of
    // workspaces, if necessary

    connect(this, SIGNAL(visibilityChanged(bool)),
            this, SLOT(retrieveWorkspaces(const bool &)));

    // Some connections to process responses from our PMR web service

    connect(mPmrWebService, SIGNAL(busy(const bool &)),
            this, SLOT(busy(const bool &)));

    connect(mPmrWebService, SIGNAL(information(const QString &)),
            this, SLOT(showInformation(const QString &)));
    connect(mPmrWebService, SIGNAL(warning(const QString &)),
            this, SLOT(showWarning(const QString &)));
    connect(mPmrWebService, SIGNAL(error(const QString &)),
            this, SLOT(showError(const QString &)));

    connect(mPmrWebService, SIGNAL(authenticated(const bool &)),
            this, SLOT(updateGui()));

    connect(mPmrWebService, SIGNAL(workspaces(const PMRSupport::PmrWorkspaces &)),
            mPmrWorkspacesWindowWidget, SLOT(initialiseWorkspaceWidget(const PMRSupport::PmrWorkspaces &)));

    // Connections to process requests from our workspaces widget

    connect(mPmrWorkspacesWindowWidget, SIGNAL(openFileRequested(const QString &)),
            this, SLOT(openFile(const QString &)));

    connect(mPmrWorkspacesWindowWidget, SIGNAL(information(const QString &)),
            this, SLOT(showInformation(const QString &)));
    connect(mPmrWorkspacesWindowWidget, SIGNAL(warning(const QString &)),
            this, SLOT(showWarning(const QString &)));

    // Retranslate our GUI

    retranslateUi();
}

//==============================================================================

PmrWorkspacesWindowWindow::~PmrWorkspacesWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);

    retranslateActionPmr();

    // Retranslate our workspaces widget

    mPmrWorkspacesWindowWidget->retranslateUi();
}

//==============================================================================

void PmrWorkspacesWindowWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the workspaces widget

    pSettings->beginGroup(mPmrWorkspacesWindowWidget->objectName());
        mPmrWorkspacesWindowWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWindowWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the workspaces widget

    pSettings->beginGroup(mPmrWorkspacesWindowWidget->objectName());
        mPmrWorkspacesWindowWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWindowWindow::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Core::OrganisationWidget::resizeEvent(pEvent);

    // Resize our busy widget

    mPmrWorkspacesWindowWidget->resizeBusyWidget();
}

//==============================================================================

void PmrWorkspacesWindowWindow::busy(const bool &pBusy)
{
    // Show ourselves as busy or not busy anymore

    static int counter = 0;

    counter += pBusy?1:-1;

    if (pBusy && (counter == 1)) {
        mPmrWorkspacesWindowWidget->showBusyWidget();

        mGui->dockWidgetContents->setEnabled(false);
    } else if (!pBusy && !counter) {
        // Re-enable the GUI side

        mPmrWorkspacesWindowWidget->hideBusyWidget();

        mGui->dockWidgetContents->setEnabled(true);
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::showInformation(const QString &pMessage)
{
    // Show the given message as an information message box

    Core::informationMessageBox(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindowWindow::showWarning(const QString &pMessage)
{
    // Show the given message as a warning message box

    Core::warningMessageBox(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindowWindow::showError(const QString &pMessage)
{
    // Either show the given message as an error message box or tell our
    // workspaces widget that we have a problem, this based on whether we
    // already retrieved the list of workspaces
    // Note: indeed, the idea is not to break the user's workflow, should an
    //       error occur when trying to retrieve the list of workspaces at
    //       startup...

//---GRY--- TO BE DONE...
//    if (mPmrWorkspacesWindowWidget->hasWorkspaces())
        Core::criticalMessageBox(Core::mainWindow(), windowTitle(), pMessage);
//    else
//        mPmrWorkspacesWindowWidget->initialize(PMRSupport::PmrExposures(), QString(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindowWindow::retrieveWorkspaces(const bool &pVisible)
{
    // Update our GUI, if we are becoming visible and the list of workspaces has
    // never been requested before (through a single shot, this to allow other
    // events, such as the one asking OpenCOR's main window to resize itself, to
    // be handled properly)
    // Note: this will result in the workspace list being populated if we are
    //       authenticated with PMR...

    static bool firstTime = true;

    if (pVisible && firstTime) {
        firstTime = false;

        QTimer::singleShot(0, this, SLOT(updateGui()));
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::updateGui()
{
    // Update our GUI based on whether we are authenticated with PMR

    mAuthenticated = mPmrWebService->isAuthenticated();

    Core::showEnableAction(mGui->actionNew, true, mAuthenticated);
    Core::showEnableAction(mGui->actionReload, true, mAuthenticated);

    mColorizeEffect->setColor(mAuthenticated?Qt::darkGreen:Qt::darkRed);

    retranslateActionPmr();

    if (mAuthenticated)
        mPmrWorkspacesWindowWidget->refreshWorkspaces();
    else
        mPmrWorkspacesWindowWidget->clearWorkspaces();
}

//==============================================================================

void PmrWorkspacesWindowWindow::retranslateActionPmr()
{
    // Retranslate our PMR action

    I18nInterface::retranslateAction(mGui->actionPmr,
                                     mAuthenticated?
                                         tr("Log Off"):
                                         tr("Log On"),
                                     mAuthenticated?
                                         tr("Log off PMR"):
                                         tr("Log on to PMR"));
}

//==============================================================================

void PmrWorkspacesWindowWindow::on_actionNew_triggered()
{
    // Create a new workspace

    PmrWorkspacesWindowNewWorkspace *newWorkspaceDialog = new PmrWorkspacesWindowNewWorkspace(Core::mainWindow());

    if (newWorkspaceDialog->exec() == QDialog::Accepted) {
        // Create the folder for the new workspace

        QDir workspaceFolder(newWorkspaceDialog->path());

        if (!workspaceFolder.exists()) {
            workspaceFolder.mkpath(".");
        } else {
            // TODO Need to check the folder is empty and if not, give
            // an error and return.
            //
            // However folder selection in the new workspace dialog
            // should ensure we have an empty folder.
            //
            // What error message do we see from git if the folder
            // isn't empty??
        }

        // Ask the PMR web service to create a new workspace, resulting in the
        // (empty) workspace being cloned into its folder

        mPmrWebService->requestNewWorkspace(newWorkspaceDialog->title(),
                                            newWorkspaceDialog->description(),
                                            newWorkspaceDialog->path());
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::on_actionReload_triggered()
{
    // Ask the workspaces widget to refresh itself

    mPmrWorkspacesWindowWidget->refreshWorkspaces();
}

//==============================================================================

void PmrWorkspacesWindowWindow::on_actionPmr_triggered()
{
    // Log on/off to/ PMR

    if (mAuthenticated) {
        if (Core::questionMessageBox(this, windowTitle(), tr("Log off PMR?")) == QMessageBox::Yes)
            mPmrWebService->authenticate(false);
    } else {
        mPmrWebService->authenticate();
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::fileReloaded(const QString &pFileName)
{
    mPmrWorkspacesWindowWidget->refreshWorkspaceFile(pFileName);
}

//==============================================================================

void PmrWorkspacesWindowWindow::fileRenamed(const QString &pOldFileName, const QString &pNewFileName)
{
    mPmrWorkspacesWindowWidget->refreshWorkspaceFile(pOldFileName);
    mPmrWorkspacesWindowWidget->refreshWorkspaceFile(pNewFileName);
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

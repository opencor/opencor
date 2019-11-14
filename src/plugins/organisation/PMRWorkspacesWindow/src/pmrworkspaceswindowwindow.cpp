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
// PMR Workspaces window
//==============================================================================

#include "borderedwidget.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "i18ninterface.h"
#include "pmrsupportplugin.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"
#include "pmrworkspacemanager.h"
#include "pmrworkspaceswindownewworkspacedialog.h"
#include "pmrworkspaceswindowwidget.h"
#include "pmrworkspaceswindowwindow.h"
#include "preferencesinterface.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_pmrworkspaceswindowwindow.h"

//==============================================================================

#include <QDesktopServices>
#include <QDir>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowWindow::PmrWorkspacesWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::PmrWorkspacesWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    connect(mGui->actionNew, &QAction::triggered,
            this, &PmrWorkspacesWindowWindow::actionNewTriggered);
    connect(mGui->actionReload, &QAction::triggered,
            this, &PmrWorkspacesWindowWindow::actionReloadTriggered);
    connect(mGui->actionPreferences, &QAction::triggered,
            this, &PmrWorkspacesWindowWindow::actionPreferencesTriggered);
    connect(mGui->actionPmr, &QAction::triggered,
            this, &PmrWorkspacesWindowWindow::actionPmrTriggered);

    // Create a tool bar widget with different actions
    // Note #1: normally, we would retrieve the folder icon through a call to
    //          QFileIconProvider().icon(QFileIconProvider::Folder), but on
    //          Windows it will, in this case, return the QStyle::SP_DirIcon
    //          icon while we really want the QStyle::SP_DirClosedIcon icon...
    // Note #2: we right-align our PMR-related action and make it ready for
    //          colorisation effect...

    static const QIcon PlusIcon = QIcon(":/oxygen/actions/list-add.png");
    static const QIcon UserIcon = QIcon(":/oxygen/apps/preferences-desktop-user-password.png");

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
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionReload);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionPreferences);

    auto spacer = new QWidget(toolBarWidget);

    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toolBarWidget->addWidget(spacer);
    toolBarWidget->addAction(mGui->actionPmr);

    mLoggedOnIcon = Core::tintedIcon(UserIcon, Qt::darkGreen);
    mLoggedOffIcon = Core::tintedIcon(UserIcon, Qt::darkRed);

    mGui->layout->addWidget(toolBarWidget);

    // Create and add a label to highlight the repository we are using

    mPmrInstanceLabel = new QLabel(this);

    QFont newFont = mPmrInstanceLabel->font();

    newFont.setPointSizeF(0.7*newFont.pointSizeF());

    mPmrInstanceLabel->setAlignment(Qt::AlignCenter);
    mPmrInstanceLabel->setEnabled(false);
    mPmrInstanceLabel->setFont(newFont);

    mGui->layout->addWidget(mPmrInstanceLabel);

    // Create an instance of our PMR Web service

    mPmrWebService = new PMRSupport::PmrWebService(this);

    // Create and add our workspaces widget

    mPmrWorkspacesWindowWidget = new PmrWorkspacesWindowWidget(mPmrUrl, mPmrWebService, this);

    mPmrWorkspacesWindowWidget->setObjectName("PmrWorkspacesWindowWidget");

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWorkspacesWindowWidget,
                                                     true, true, true, true));
#else
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWorkspacesWindowWidget,
                                                     true, false, false, false));
#endif

    // Initialise (update) our PMR URL

    update(PreferencesInterface::preference(PMRSupport::PluginName,
                                            PMRSupport::SettingsPreferencesPmrUrl,
                                            PMRSupport::SettingsPreferencesPmrUrlDefault).toString());

    // Keep track of the window's visibility, so that we can request the list of
    // workspaces, if necessary

    connect(this, &PmrWorkspacesWindowWindow::visibilityChanged,
            this, &PmrWorkspacesWindowWindow::retrieveWorkspaces);

    // Some connections to process responses from our PMR Web service

    connect(mPmrWebService, &PMRSupport::PmrWebService::busy,
            this, QOverload<bool>::of(&PmrWorkspacesWindowWindow::busy));

    connect(mPmrWebService, &PMRSupport::PmrWebService::information,
            this, &PmrWorkspacesWindowWindow::showInformation);
    connect(mPmrWebService, &PMRSupport::PmrWebService::warning,
            this, &PmrWorkspacesWindowWindow::showWarning);
    connect(mPmrWebService, &PMRSupport::PmrWebService::error,
            this, &PmrWorkspacesWindowWindow::showError);

    connect(mPmrWebService, &PMRSupport::PmrWebService::authenticated,
            this, &PmrWorkspacesWindowWindow::updateGui);
    connect(mPmrWebService, &PMRSupport::PmrWebService::authenticationCancelled,
            this, &PmrWorkspacesWindowWindow::updateGui);

    connect(mPmrWebService, &PMRSupport::PmrWebService::workspaces,
            mPmrWorkspacesWindowWidget, QOverload<const PMRSupport::PmrWorkspaces &>::of(&PmrWorkspacesWindowWidget::initialize));

    // Connections to process requests from our PMR workspaces widget

    connect(mPmrWorkspacesWindowWidget, &PmrWorkspacesWindowWidget::information,
            this, &PmrWorkspacesWindowWindow::showInformation);
    connect(mPmrWorkspacesWindowWidget, &PmrWorkspacesWindowWidget::warning,
            this, &PmrWorkspacesWindowWindow::showWarning);

    connect(mPmrWorkspacesWindowWidget, &PmrWorkspacesWindowWidget::openFileRequested,
            this, &PmrWorkspacesWindowWindow::openFile);
    connect(mPmrWorkspacesWindowWidget, &PmrWorkspacesWindowWidget::openFilesRequested,
            this, &PmrWorkspacesWindowWindow::openFiles);

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

    mPmrInstanceLabel->setText(mPmrWebService->siteName());

    // Retranslate our workspaces widget

    mPmrWorkspacesWindowWidget->retranslateUi();
}

//==============================================================================

void PmrWorkspacesWindowWindow::loadSettings(QSettings &pSettings)
{
    // Retrieve the settings of the workspaces window widget

    pSettings.beginGroup(mPmrWorkspacesWindowWidget->objectName());
        mPmrWorkspacesWindowWidget->loadSettings(pSettings);
    pSettings.endGroup();

    // We are fully initialised now

    mInitialized = true;
}

//==============================================================================

void PmrWorkspacesWindowWindow::saveSettings(QSettings &pSettings) const
{
    // Keep track of the settings of the workspaces window widget

    pSettings.beginGroup(mPmrWorkspacesWindowWidget->objectName());
        mPmrWorkspacesWindowWidget->saveSettings(pSettings);
    pSettings.endGroup();
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

Ui::PmrWorkspacesWindowWindow * PmrWorkspacesWindowWindow::gui() const
{
    // Return our GUI

    return mGui;
}

//==============================================================================

void PmrWorkspacesWindowWindow::update(const QString &pPmrUrl)
{
    // Update both our PMR Web service and workspaces widget, and then update
    // our GUI (which will, as a result, also update our workspaces widget), if
    // needed
    // Note: we make sure that no busy widget is visible. Indeed, this is in
    //       case an instance wasn't working and we decided to switch to another
    //       that does (in which case the busy widget of the first instance
    //       would still have been visible)...

    if (pPmrUrl != mPmrUrl) {
        if (PMRSupport::PmrWorkspaceManager::instance()->hasWorkspaces()) {
            mPmrWorkspacesWindowWidget->initialize();
        }

        busy(false, true);

        mFirstTimeRetrievingWorkspaces = true;

        mPmrUrl = pPmrUrl;

        mPmrWebService->update(pPmrUrl);
        mPmrWorkspacesWindowWidget->update(pPmrUrl);

        mPmrInstanceLabel->setText(mPmrWebService->siteName());

        updateGui();
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::busy(bool pBusy, bool pResetCounter)
{
    // Show ourselves as busy or not busy anymore, but only if we are
    // initialised

    if (!mInitialized) {
        return;
    }

    static int counter = 0;

    if (!pBusy && (counter == 0)) {
        return;
    }

    if (pResetCounter) {
        counter = 0;
    } else {
        counter += pBusy?1:-1;
    }

    if (pBusy && (counter == 1)) {
        mGui->dockWidgetContents->setEnabled(false);

        mPmrWorkspacesWindowWidget->showBusyWidget();
    } else if (!pBusy && (counter == 0)) {
        // Re-enable the GUI side

        mGui->dockWidgetContents->setEnabled(true);

        mPmrWorkspacesWindowWidget->hideBusyWidget();
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::busy(bool pBusy)
{
    // Show ourselves as busy or not busy anymore

    busy(pBusy, false);
}

//==============================================================================

void PmrWorkspacesWindowWindow::showInformation(const QString &pMessage)
{
    // Show the given message as an information message box, but only if our
    // workspace manager is keeping track of some workspaces
    // Note: indeed, the idea is not to break the user's workflow, should some
    //       information become available when trying to retrieve the list of
    //       workspaces at startup...

    if (!PMRSupport::PmrWorkspaceManager::instance()->hasWorkspaces()) {
        mPmrWorkspacesWindowWidget->initialize(PmrWorkspacesWindowWidget::Message::Information, pMessage);
    } else {
        Core::informationMessageBox(windowTitle(), pMessage);
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::showWarning(const QString &pMessage)
{
    // Show the given message as a warning message box, but only if our
    // workspace manager is keeping track of some workspaces
    // Note: indeed, the idea is not to break the user's workflow, should a
    //       warning occur when trying to retrieve the list of workspaces at
    //       startup...

    if (!PMRSupport::PmrWorkspaceManager::instance()->hasWorkspaces()) {
        mPmrWorkspacesWindowWidget->initialize(PmrWorkspacesWindowWidget::Message::Warning, pMessage);
    } else {
        Core::warningMessageBox(windowTitle(), pMessage);
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::showError(const QString &pMessage)
{
    // Either show the given message as an error message box or tell our
    // workspaces widget that we have a problem, this based on whether our
    // workspace manager is keeping track of some workspaces
    // Note: indeed, the idea is not to break the user's workflow, should an
    //       error occur when trying to retrieve the list of workspaces at
    //       startup...

    if (!PMRSupport::PmrWorkspaceManager::instance()->hasWorkspaces()) {
        mPmrWorkspacesWindowWidget->initialize(PmrWorkspacesWindowWidget::Message::Error, pMessage);
    } else {
        Core::criticalMessageBox(windowTitle(), pMessage);
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::retrieveWorkspaces(bool pVisible)
{
    // Update our GUI, if we are becoming visible and the list of workspaces has
    // never been requested before (through a single shot, this to allow other
    // events, such as the one asking OpenCOR's main window to resize itself, to
    // be handled properly)
    // Note: this will result in the workspace list being populated if we are
    //       authenticated with PMR...

    if (pVisible && mFirstTimeRetrievingWorkspaces) {
        mFirstTimeRetrievingWorkspaces = false;

        QTimer::singleShot(0, this, &PmrWorkspacesWindowWindow::updateGui);
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::updateGui()
{
    // Update our GUI

    mAuthenticated = mPmrWebService->isAuthenticated();

    mWaitingForPmrWebService = false;

    Core::showEnableAction(mGui->actionNew, true, mAuthenticated);
    Core::showEnableAction(mGui->actionReload, true, mAuthenticated);

    mGui->actionPmr->setIcon(mAuthenticated?mLoggedOnIcon:mLoggedOffIcon);

    retranslateActionPmr();

    // Ask ourselves to reload the list of workspaces from PMR, if we are
    // authenticated, or simply show a message asking us to authenticate
    // ourselves

    if (mAuthenticated) {
        actionReloadTriggered();
    } else if (mInitialized) {
        mPmrWorkspacesWindowWidget->initialize();
    }
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
                                         tr("Log off from PMR"):
                                         tr("Log on to PMR"));
}

//==============================================================================

void PmrWorkspacesWindowWindow::actionNewTriggered()
{
    // Create a new (owned) workspace

    PmrWorkspacesWindowNewWorkspaceDialog newWorkspaceDialog(Core::mainWindow());

    if (newWorkspaceDialog.exec() == QDialog::Accepted) {
        // Ask the PMR Web service to create a new workspace, resulting in the
        // (empty) workspace being cloned into its folder

        mPmrWebService->requestNewWorkspace(newWorkspaceDialog.title(),
                                            newWorkspaceDialog.description(),
                                            newWorkspaceDialog.path());
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::actionReloadTriggered()
{
    // Get the list of workspaces from our PMR Web service, after making sure
    // that we have cleared existing workspaces from our workspace manager

    PMRSupport::PmrWorkspaceManager::instance()->clearWorkspaces();

    mPmrWebService->requestWorkspaces();
}

//==============================================================================

void PmrWorkspacesWindowWindow::actionPreferencesTriggered()
{
    // Show the preferences for PMR support

    QDesktopServices::openUrl(QUrl("opencor://openPreferencesDialog/PMRSupport"));
}

//==============================================================================

void PmrWorkspacesWindowWindow::actionPmrTriggered()
{
    // Log on/off to/ PMR

    if (mWaitingForPmrWebService) {
        return;
    }

    mWaitingForPmrWebService = true;

    if (mAuthenticated) {
        if (Core::questionMessageBox(windowTitle(),
                                     tr("You are about to log off PMR. Do you want to proceed?")) == QMessageBox::Yes ) {
            mPmrWebService->authenticate(false);
        }
    } else {
        mPmrWebService->authenticate();
    }
}

//==============================================================================

} // namespace PMRWorkspacesWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
